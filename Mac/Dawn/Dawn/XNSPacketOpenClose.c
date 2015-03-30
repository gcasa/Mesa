// XNSPacketOpenClose.c
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "XNSPacketMain.h"

NTSTATUS
PacketOpen(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    // This is the dispatch routine for create/open and close requests.
    // These requests complete successfully.

    POPEN_INSTANCE      open;
    NTSTATUS            status = STATUS_SUCCESS;

    DebugPrint(("OpenAdapter\n"));

#ifdef XNSDRW_LOG_PACKETS
    // reset the packet sniffing log
    LogReset();
#endif

    if (DeviceObject == Globals.ControlDeviceObject)
    {
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }
    
    open = DeviceObject->DeviceExtension;

    DebugPrint(("AdapterName :%ws\n", open->AdapterName.Buffer));

    IoIncrement(open);

    // Check to see whether you are still bound to the adapter
    if(! open->Bound)
    {
        status = STATUS_DEVICE_NOT_READY;
    }

    Irp->IoStatus.Information = 0;    
    Irp->IoStatus.Status = status;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    IoDecrement(open);
    return status;
}

NTSTATUS
PacketClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    // This is the dispatch routine for create/open and close requests.
    // These requests complete successfully.

    POPEN_INSTANCE      open;
    NTSTATUS            status = STATUS_SUCCESS;

    DebugPrint(("CloseAdapter \n"));

#ifdef XNSDRW_LOG_PACKETS
    // reset the packet sniffing log
    LogReset();
#endif

    if (DeviceObject == Globals.ControlDeviceObject)
    {
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }

    open = DeviceObject->DeviceExtension;

    IoIncrement(open);

    Irp->IoStatus.Information = 0;    
    Irp->IoStatus.Status = status;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);

    IoDecrement(open);
    return status;
}

NTSTATUS
PacketCleanup(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
    // This is the dispatch routine for cleanup requests.
    // This routine is called whenever a handle to the device is closed.

    POPEN_INSTANCE      open;
    NTSTATUS            status = STATUS_SUCCESS;

    DebugPrint(("Packet: Cleanup\n"));

    if (DeviceObject == Globals.ControlDeviceObject)
    {
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }

    open = DeviceObject->DeviceExtension;

    IoIncrement(open);
    
    // Cancel all the pending reads.
    PacketCancelReadIrps(DeviceObject);

    // Since the current implementation of NDIS doesn't
    // allow us to cancel requests pending at the 
    // miniport, we must wait here until they complete.
    IoDecrement(open);

    NdisWaitEvent(&open->CleanupEvent, 0);

    Irp->IoStatus.Information = 0;    
    Irp->IoStatus.Status = status;
    IoCompleteRequest (Irp, IO_NO_INCREMENT);
    return status;
}

VOID
PacketResetComplete(IN NDIS_HANDLE  ProtocolBindingContext, IN NDIS_STATUS  Status)
{
    // This routine  is called when a protocol-initiated reset operation, 
    // begun with a call to NdisReset that returned NDIS_STATUS_PENDING, 
    // is completed. 

    POPEN_INSTANCE      open;
    PIRP                irp;

    PLIST_ENTRY         resetListEntry;

    DebugPrint(("PacketResetComplte\n"));

#ifdef XNSDRW_LOG_PACKETS
    // reset the packet sniff log
    LogReset();
#endif

    open = (POPEN_INSTANCE)ProtocolBindingContext;


    //  remove the reset IRP from the list
    resetListEntry = ExInterlockedRemoveHeadList(&open->ResetIrpList, &open->ResetQueueLock);

#if DBG
    if (resetListEntry == NULL)
    {
        DbgBreakPoint();
        return;
    }
#endif

    irp = CONTAINING_RECORD(resetListEntry, IRP, Tail.Overlay.ListEntry);

    if (Status == NDIS_STATUS_SUCCESS) irp->IoStatus.Status = STATUS_SUCCESS;
    else irp->IoStatus.Status = STATUS_UNSUCCESSFUL;

    irp->IoStatus.Information = 0;    
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    IoDecrement(open);

    DebugPrint(("PacketResetComplte exit\n"));
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  16-May-01   dwoodward   created

