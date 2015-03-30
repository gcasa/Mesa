// XNSPacketRead.c : implementation of packet receiving
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "XNSPacketMain.h"

NTSTATUS
PacketRead(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    // This is the dispatch routine for read requests.

    POPEN_INSTANCE      open;
    PNDIS_PACKET        pPacket;
    NDIS_STATUS         status;
    NTSTATUS            ntStatus;
    PIO_STACK_LOCATION  irpSp;

    DebugPrint(("Read\n"));

    open = DeviceObject->DeviceExtension;

    // Check to see whether you are still bound to the adapter
    IoIncrement(open);
    if(! open->Bound)
    {
        ntStatus = STATUS_DEVICE_NOT_READY;
        goto ERROR;
    }
    
    irpSp = IoGetCurrentIrpStackLocation(Irp);

    //  See if the buffer is at least big enough to hold the ethernet header
    if (irpSp->Parameters.Read.Length < ETHERNET_HEADER_LENGTH)
    {
        ntStatus = STATUS_BUFFER_TOO_SMALL;
        goto ERROR;
    }

    // Try to get a packet from our list of free ones. 
    NdisAllocatePacket(&status, &pPacket, open->PacketPool);
    if (status != NDIS_STATUS_SUCCESS)
    {
        DebugPrint(("Packet: Read- No free packets\n"));
        ntStatus = STATUS_INSUFFICIENT_RESOURCES;
        goto ERROR;
    }

    //  Get a pointer to the packet itself.
    RESERVED(pPacket)->Irp=Irp;
    RESERVED(pPacket)->pMdl=NULL;

    IoMarkIrpPending(Irp);
    // Set the cancel routine so that we can cancel the requests
    // pending in the queue if the application terminates.
    IoSetCancelRoutine(Irp, PacketCancelRoutine);

    //  Put this packet in a list of pending reads.
    //  The PacketReceiveIndicate or PacketReceivePacket handler will 
    //  attempt to remove packets from this list.
    ExInterlockedInsertTailList(&open->RcvList, &RESERVED(pPacket)->ListElement, &open->RcvQSpinLock);

    return STATUS_PENDING;
    
ERROR:
    Irp->IoStatus.Status = ntStatus;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    IoDecrement(open);
    return ntStatus;
}

NDIS_STATUS
PacketReceiveIndicate (
    IN NDIS_HANDLE ProtocolBindingContext,
    IN NDIS_HANDLE MacReceiveContext,
    IN PVOID       HeaderBuffer,
    IN UINT        HeaderBufferSize,
    IN PVOID       LookAheadBuffer,
    IN UINT        LookaheadBufferSize,
    IN UINT        PacketSize
    )
{
    // PacketReceiveIndicate is called with a pointer to a 
    // lookahead buffer. If this buffer contains less than 
    // the full, received network packet, it 
    // calls NdisTransferData with a protocol-allocated packet 
    // descriptor specifying protocol-allocated buffer(s) to obtain 
    // the remainder of the received packet. 

    POPEN_INSTANCE      open;
    PIO_STACK_LOCATION  irpSp;
    PIRP                irp;
    PLIST_ENTRY         packetListEntry;
    PNDIS_PACKET        pPacket;
    ULONG               sizeToTransfer;
    NDIS_STATUS         status;
    UINT                bytesTransfered = 0;
    ULONG               bufferLength;
    PPACKET_RESERVED    reserved;
    PMDL                pMdl;

    DebugPrint(("ReceiveIndicate\n"));

    // make sure this is an XNS packet
    if ((HeaderBufferSize != XNS_PACKET_HEADER_SIZE) ||
        (*((char *)HeaderBuffer + 12) != XNS_PACKET_TYPE_HI) ||
        (*((char *)HeaderBuffer + 13) != XNS_PACKET_TYPE_LO) ||
        (PacketSize > XNS_PACKET_MAX_DATA_SIZE))
    {
#ifdef XNSDRW_LOG_PACKETS
        LogRejectedPacketHeader(HeaderBuffer, HeaderBufferSize);
#endif
        return (NDIS_STATUS_NOT_ACCEPTED);
    }

    // if (HeaderBufferSize > ETHERNET_HEADER_LENGTH) return NDIS_STATUS_SUCCESS;
    
    open = (POPEN_INSTANCE)ProtocolBindingContext;

    //  See if there are any pending read that we can satisfy
    packetListEntry = ExInterlockedRemoveHeadList(&open->RcvList, &open->RcvQSpinLock);

    if (packetListEntry == NULL)
    {
        DebugPrint(("No pending read, dropping packets\n"));
        return NDIS_STATUS_NOT_ACCEPTED;
    }

    reserved = CONTAINING_RECORD(packetListEntry, PACKET_RESERVED, ListElement);
    pPacket = CONTAINING_RECORD(reserved, NDIS_PACKET, ProtocolReserved);

    irp = RESERVED(pPacket)->Irp;
    irpSp = IoGetCurrentIrpStackLocation(irp);

    // We don't have to worry about the situation where the IRP is cancelled
    // after we remove it from the queue and before we reset the cancel
    // routine because the cancel routine has been coded to cancel an IRP
    // only if it's in the queue.
    IoSetCancelRoutine(irp, NULL);
    
    //  This is the length of our partial MDL
    bufferLength = irpSp->Parameters.Read.Length - ETHERNET_HEADER_LENGTH;

    //  Find out how much to transfer
    sizeToTransfer = (PacketSize < bufferLength) ? PacketSize : bufferLength;

    //  copy the ethernet header into the actual readbuffer
    NdisMoveMappedMemory(
        MmGetSystemAddressForMdlSafe(irp->MdlAddress, NormalPagePriority),
        HeaderBuffer,
        HeaderBufferSize
        );

    //  Allocate an MDL to map the portion of the buffer following the header
    pMdl = IoAllocateMdl(
              MmGetMdlVirtualAddress(irp->MdlAddress),
              MmGetMdlByteCount(irp->MdlAddress),
              FALSE,
              FALSE,
              NULL
              );

    if (pMdl == NULL)
    {
        DebugPrint(("Packet: Read-Failed to allocate Mdl\n"));
        status = NDIS_STATUS_RESOURCES;
        goto ERROR;
    }

    //
    //  Build the mdl to point to the the portion of the buffer following the header
    IoBuildPartialMdl(
        irp->MdlAddress,
        pMdl,
        ((PUCHAR)MmGetMdlVirtualAddress(irp->MdlAddress)) + ETHERNET_HEADER_LENGTH,
        0
        );

    //  Clear the next link in the new MDL
    pMdl->Next = NULL;

    RESERVED(pPacket)->pMdl = pMdl;

    //  Attach our partial MDL to the packet
    NdisChainBufferAtFront(pPacket, pMdl);

    //  Call the Mac to transfer the packet
    NdisTransferData(
        &status,
        open->AdapterHandle,
        MacReceiveContext,
        0,
        sizeToTransfer,
        pPacket,
        &bytesTransfered);

    if (status == NDIS_STATUS_PENDING)
    {
        return NDIS_STATUS_SUCCESS;
    }

ERROR:
    //  If it didn't pend, call the completeion routine now
    PacketTransferDataComplete(open, pPacket, status, bytesTransfered);
    return NDIS_STATUS_SUCCESS;
}



VOID
PacketTransferDataComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_PACKET  pPacket,
    IN NDIS_STATUS   Status,
    IN UINT          BytesTransfered
    )
{
    // This is a required function unless the protocol binds itself 
    // exclusively to underlying NIC driver(s) that indicate packets 
    // with NdisMIndicateReceivePacket. ProtocolTransferDataComplete 
    // is called when a previous call to NdisTransferData returned 
    // NDIS_STATUS_PENDING and the remaining data has been copied 
    // into the protocol-supplied buffers chained to a given packet 
    // descriptor. 

    PIO_STACK_LOCATION   irpSp;
    POPEN_INSTANCE       open;
    PIRP                 irp;
    PMDL                 pMdl;

    DebugPrint(("Packet: TransferDataComplete\n"));

    open = (POPEN_INSTANCE)ProtocolBindingContext;
    irp = RESERVED(pPacket)->Irp;
    irpSp = IoGetCurrentIrpStackLocation(irp);
    pMdl = RESERVED(pPacket)->pMdl;

    //  Free the partial MDL that we allocated
    if (pMdl) IoFreeMdl(pMdl);

    //  Put the packet on the free queue
    NdisFreePacket(pPacket);

    if (Status == NDIS_STATUS_SUCCESS)
    {
        irp->IoStatus.Status = STATUS_SUCCESS;
        irp->IoStatus.Information = BytesTransfered+ETHERNET_HEADER_LENGTH;
    }
    else
    {
        irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
        irp->IoStatus.Information = 0;
    }

    DebugPrint(("BytesTransfered:%d\n", irp->IoStatus.Information));

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    IoDecrement(open);
}

VOID
PacketReceiveComplete(IN NDIS_HANDLE ProtocolBindingContext)
{
    // This is a required function. PacketReceiveComplete 
    // is called to indicate that any received packets previously 
    // indicated to PacketReceivePacket can now be postprocessed.

#if 0
#ifdef XNSDRW_LOG_PACKETS    
    // log the packet to the sniff log
    if (Irp->MdlAddress != NULL)
    {
        void * pData = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, LowPagePriority);
        ULONG nDataSize = MmGetMdlByteCount(Irp->MdlAddress);
        if (pData && (nDataSize > 0)) LogPacketSend(pData, nDataSize);
    }
#endif
#endif
}

INT
PacketReceivePacket(
    IN    NDIS_HANDLE         ProtocolBindingContext,
    IN    PNDIS_PACKET        Packet
    )
{
    // ReceivePacket handler. Called up by the miniport below 
    // when it supports NDIS 4.0 style receives.
    // A ProtocolReceivePacket function should be provided if 
    // the protocol driver might be bound to a NIC driver that 
    // indicates an array of one or more packets by calling 
    // NdisMIndicateReceivePacket. 

    // return 0 if we are done with the packet, or non-zero to indicate
    // we will keep the packet and call NdisReturnPackets() n many times when done

    UINT                bytesTransfered = 0;
    POPEN_INSTANCE      open;
    PIRP                irp;
    PNDIS_PACKET        myPacket;
    PLIST_ENTRY         packetListEntry;
    ULONG               bufferLength;
    PPACKET_RESERVED    reserved;
    PIO_STACK_LOCATION  irpSp;
    PMDL                mdl;
    NTSTATUS            status = STATUS_SUCCESS;

    DebugPrint(("PacketReceivePacket\n"));

    open = (POPEN_INSTANCE)ProtocolBindingContext;

    //  See if there are any pending read that we can satisfy
    packetListEntry = ExInterlockedRemoveHeadList(&open->RcvList, &open->RcvQSpinLock);

    if (packetListEntry == NULL)
    {
        // No pending reads so just the dropping the packet on the floor.
        DebugPrint(("No pending read, dropping packets\n"));
        return 0;
    }

    reserved = CONTAINING_RECORD(packetListEntry, PACKET_RESERVED, ListElement);
    myPacket = CONTAINING_RECORD(reserved, NDIS_PACKET, ProtocolReserved);
    irp = RESERVED(myPacket)->Irp;
    irpSp = IoGetCurrentIrpStackLocation(irp);

    // We don't have to worry about the situation where the IRP is cancelled
    // after we remove it from the queue and before we reset the cancel
    // routine because the cancel routine has been coded to cancel an IRP
    // only if it's in the queue.
    IoSetCancelRoutine(irp, NULL);

    // Following block of code locks the destination packet
    // MDLs in a safe manner. This is a temporary workaround
    // for NdisCopyFromPacketToPacket that currently doesn't use
    // safe functions to lock pages of MDL. This is required to
    // prevent system from bugchecking under low memory resources.
    {
        PVOID           virtualAddress;
        PNDIS_BUFFER    firstBuffer, nextBuffer;
        ULONG           totalLength;

        NdisQueryPacket(Packet, NULL, NULL, &firstBuffer, &totalLength);
        while (firstBuffer != NULL)
        {
            NdisQueryBufferSafe(firstBuffer, &virtualAddress, &totalLength, NormalPagePriority);
            if (! virtualAddress)
            {
                // System is running low on memory resources.  So fail the read.
                status = STATUS_INSUFFICIENT_RESOURCES;
                goto CleanExit;
            }
            NdisGetNextBuffer(firstBuffer,  &nextBuffer);
            firstBuffer = nextBuffer;
        }
    }
    
    //  Attach the actual MDL to the packet
    NdisChainBufferAtFront(myPacket, irp->MdlAddress);  
    bufferLength = irpSp->Parameters.Read.Length;
    NdisCopyFromPacketToPacket(myPacket, 0, bufferLength, Packet, 0, &bytesTransfered);    

CleanExit:
    //  Put the packet on the free queue

    NdisFreePacket(myPacket);
    irp->IoStatus.Status = status;
    irp->IoStatus.Information = bytesTransfered;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    DebugPrint(("BytesTransfered:%d\n", bytesTransfered));
    IoDecrement(open);

    return 0;
}


VOID
PacketCancelRoutine (
    IN PDEVICE_OBJECT   DeviceObject,
    IN PIRP             Irp
    )
{
    // The cancel routine. It will remove the IRP from the queue 
    // and will complete it. The cancel spin lock is already acquired 
    // when this routine is called.

    POPEN_INSTANCE      open = DeviceObject->DeviceExtension; 
    KIRQL               oldIrql;
    PIRP                irpToComplete = NULL;
    PLIST_ENTRY         thisEntry, listHead;
    PIRP                pendingIrp;
    PNDIS_PACKET        myPacket = NULL;
    PPACKET_RESERVED    reserved;
    PMDL                mdl;

    // Don't assume that the IRP being cancelled is in the queue.
    // Only complete the IRP if it IS in the queue.
    // Must acquire the local spinlock before releasing 
    // the global cancel spinlock
    DebugPrint(("PacketCancelRoutine\n"));

    oldIrql = Irp->CancelIrql;

    // One should not intermix KeAcquireSpinLock(AtDpcLevel) 
    // and ExInterlocked...List() functions on the same spinlock if the
    // routines that use the lock run at IRQL > DISPATCH_LEVEL.
    // After acquiring the lock using Ke function, if we got interrupted
    // and entered into an ISR and tried to manipulate the list using
    // ExInterlocked...List function with the same lock, we deadlock.
    // In this sample we can safely do that because none of our routines 
    // will be called at IRQL > DISPATCH_LEVEL.
    KeAcquireSpinLockAtDpcLevel(&open->RcvQSpinLock);
    IoReleaseCancelSpinLock(KeGetCurrentIrql());

    // Remove the IRP from the queue
    listHead = &open->RcvList;
    
    for (thisEntry = listHead->Flink; thisEntry != listHead; thisEntry = thisEntry->Flink)
    {
        reserved = CONTAINING_RECORD(thisEntry, PACKET_RESERVED, ListElement);
        myPacket = CONTAINING_RECORD(reserved, NDIS_PACKET, ProtocolReserved);
        pendingIrp = RESERVED(myPacket)->Irp;

        if (pendingIrp == Irp) 
        {
                RemoveEntryList(thisEntry);
                irpToComplete = pendingIrp;
                break;
        }
    }
    
    // Release the queue spinlock
    KeReleaseSpinLock(&open->RcvQSpinLock, oldIrql);

    // Complete the IRP with status canclled
    if (irpToComplete)
    {
        DebugPrint(("Cancelling IRP\n"));
        ASSERT(myPacket);       
        
        //  Put the packet on the free queue
        NdisFreePacket(myPacket);
        irpToComplete->IoStatus.Status = STATUS_CANCELLED;
        irpToComplete->IoStatus.Information = 0;
        IoCompleteRequest(irpToComplete, IO_NO_INCREMENT);
        IoDecrement(open);
    }
}

NTSTATUS
PacketCancelReadIrps( IN PDEVICE_OBJECT DeviceObject)
{
    POPEN_INSTANCE      open = DeviceObject->DeviceExtension; 
    PLIST_ENTRY         thisEntry;
    PIRP                pendingIrp;
    PNDIS_PACKET        myPacket = NULL;
    PPACKET_RESERVED    reserved;
    PMDL                mdl;

    DebugPrint(("PacketCancelReadIrps\n"));

    // Walk through the RcvList and cancel all read IRPs.
    while (thisEntry = ExInterlockedRemoveHeadList(&open->RcvList, &open->RcvQSpinLock))
    {
        reserved = CONTAINING_RECORD(thisEntry, PACKET_RESERVED, ListElement);
        myPacket = CONTAINING_RECORD(reserved, NDIS_PACKET, ProtocolReserved);
        ASSERT(myPacket);

        pendingIrp = RESERVED(myPacket)->Irp;

        //  Put the packet on the free queue
        NdisFreePacket(myPacket);
        
        DebugPrint(("Cancelled : 0%0x\n", pendingIrp));
        
        IoSetCancelRoutine(pendingIrp, NULL);

        // Cancel the Irp
        pendingIrp->IoStatus.Information = 0;
        pendingIrp->IoStatus.Status = STATUS_CANCELLED;
        IoCompleteRequest(pendingIrp, IO_NO_INCREMENT);
        IoDecrement(open);
    }

    return STATUS_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  16-May-01   dwoodward   created
