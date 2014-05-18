// XNSPacketMain.h : main header file of XNS packet driver
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _XNS_DRW_XNSPACKETMAIN_H
#define _XNS_DRW_XNSPACKETMAIN_H

#include "ntddk.h"
#include "ndis.h"
#include "XNSPacketPublic.h"

/////////////////////////////////////////////////////////////////////////////
// #defines
//#define XNSDRW_LOG_PACKETS

#undef  ExAllocatePool
#define ExAllocatePool(a,b) ExAllocatePoolWithTag(a, b, 'kcaP')

#if DBG
#define DebugPrint(_x_) DbgPrint("XNSDRW: "); DbgPrint _x_;
#else
#define DebugPrint(_x_) 
#endif

//#define NT_DEVICE_NAME L"\\Device\\XNSDRWPacket"
//#define DOS_DEVICE_NAME L"\\DosDevices\\XNSDRWPacket"

#define RESERVED(_p) ((PPACKET_RESERVED)((_p)->ProtocolReserved))
#define  TRANSMIT_PACKETS    16

/////////////////////////////////////////////////////////////////////////////
// types
typedef struct _GLOBAL
{
    PDRIVER_OBJECT DriverObject;
    NDIS_HANDLE    NdisProtocolHandle;
    UNICODE_STRING RegistryPath; // Path to the driver's Services Key in the registr
    LIST_ENTRY  AdapterList;// List of deviceobjecs that are created for every adapter we bind to
    KSPIN_LOCK  GlobalLock; // To synchronize access to the list.
    PDEVICE_OBJECT  ControlDeviceObject; // Control deviceObject for the driver.

} GLOBAL, *PGLOBAL;

// ISSUE: messy
GLOBAL Globals;

typedef struct _INTERNAL_REQUEST
{
    PIRP           Irp;
    NDIS_REQUEST   Request;

} INTERNAL_REQUEST, *PINTERNAL_REQUEST;

typedef struct _OPEN_INSTANCE
{
    PDEVICE_OBJECT      DeviceObject;
    ULONG               IrpCount;
    NDIS_STRING         AdapterName;
    NDIS_STRING         SymbolicLink;
    NDIS_HANDLE         AdapterHandle;
    NDIS_HANDLE         PacketPool;
    KSPIN_LOCK          RcvQSpinLock;
    LIST_ENTRY          RcvList;
    NDIS_MEDIUM         Medium;
    KSPIN_LOCK          ResetQueueLock;
    LIST_ENTRY          ResetIrpList;
    NDIS_STATUS         Status;   
    NDIS_EVENT          Event;     
    NDIS_EVENT          CleanupEvent;
    LIST_ENTRY          AdapterListEntry; // List entry to link to the other deviceobjects.
    BOOLEAN             Bound; // Set to TRUE when OpenAdapter; FALSE when CloseAdpater is complete
    CHAR                Filler[3];

} OPEN_INSTANCE, *POPEN_INSTANCE;

typedef struct _PACKET_RESERVED
{
    LIST_ENTRY     ListElement;
    PIRP           Irp;
    PMDL           pMdl;

} PACKET_RESERVED, *PPACKET_RESERVED;

/////////////////////////////////////////////////////////////////////////////
// functions
NTSTATUS    DriverEntry(                IN PDRIVER_OBJECT   DriverObject,           IN PUNICODE_STRING  RegistryPath);
NTSTATUS    PacketCancelReadIrps(       IN PDEVICE_OBJECT   DeviceObject                                                                                                                                                                                            );
NTSTATUS    PacketCleanup(              IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
VOID        PacketBindAdapter(         OUT PNDIS_STATUS     Status,                 IN NDIS_HANDLE      BindContext,            IN PNDIS_STRING DeviceName,     IN PVOID SystemSpecific1, IN PVOID SystemSpecific2                                                  );
VOID        PacketUnbindAdapter(       OUT PNDIS_STATUS     Status,                 IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_HANDLE  UnbindContext                                                                                                       );
VOID        PacketOpenAdapterComplete(  IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_STATUS      Status,                 IN NDIS_STATUS  OpenErrorStatus                                                                                                     );
VOID        PacketCloseAdapterComplete( IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_STATUS      Status                                                                                                                                                      );
NDIS_STATUS PacketReceiveIndicate(      IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_HANDLE      MacReceiveContext,      IN PVOID        HeaderBuffer,   IN UINT HeaderBufferSize, IN PVOID LookAheadBuffer, IN UINT LookaheadBufferSize, IN UINT PacketSize );
VOID        PacketReceiveComplete(      IN NDIS_HANDLE      ProtocolBindingContext                                                                                                                                                                                  );
VOID        PacketRequestComplete(      IN NDIS_HANDLE      ProtocolBindingContext, IN PNDIS_REQUEST    pRequest,               IN NDIS_STATUS  Status                                                                                                              );
VOID        PacketSendComplete(         IN NDIS_HANDLE      ProtocolBindingContext, IN PNDIS_PACKET     pPacket,                IN NDIS_STATUS  Status                                                                                                              );
VOID        PacketResetComplete(        IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_STATUS      Status                                                                                                                                                      );
VOID        PacketStatus(               IN NDIS_HANDLE      ProtocolBindingContext, IN NDIS_STATUS      Status,                 IN PVOID        StatusBuffer,   IN UINT StatusBufferSize                                                                            );
VOID        PacketStatusComplete(       IN NDIS_HANDLE      ProtocolBindingContext                                                                                                                                                                                  );
VOID        PacketTransferDataComplete( IN NDIS_HANDLE      ProtocolBindingContext, IN PNDIS_PACKET     Packet,                 IN NDIS_STATUS  Status,         IN UINT BytesTransferred                                                                            );
NTSTATUS    PacketShutdown(             IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
VOID        PacketUnload(               IN PDRIVER_OBJECT   DriverObject                                                                                                                                                                                            );
NTSTATUS    PacketOpen(                 IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
NTSTATUS    PacketClose(                IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
NTSTATUS    PacketWrite(                IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
NTSTATUS    PacketRead(                 IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
NTSTATUS    PacketIoControl(            IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
VOID        PacketCancelRoutine(        IN PDEVICE_OBJECT   DeviceObject,           IN PIRP             Irp                                                                                                                                                         );
INT         PacketReceivePacket(        IN NDIS_HANDLE      ProtocolBindingContext, IN PNDIS_PACKET     Packet                                                                                                                                                      );
NTSTATUS    PacketGetAdapterList(       IN PVOID            Buffer,                 IN ULONG            Length,                 IN OUT PULONG   DataLength                                                                                                          );
NDIS_STATUS PacketPNPHandler(           IN NDIS_HANDLE      ProtocolBindingContext, IN PNET_PNP_EVENT   pNetPnPEvent                                                                                                                                                );
VOID        IoIncrement (           IN OUT POPEN_INSTANCE   Open                                                                                                                                                                                                    );
VOID        IoDecrement (           IN OUT POPEN_INSTANCE   Open                                                                                                                                                                                                    );

#ifdef XNSDRW_LOG_PACKETS
VOID        LogRejectedPacketHeader(    IN VOID *           pHeaderData,            IN ULONG            nHeaderDataSize                                                                                                                                             );
VOID        LogPacketReceive(           IN VOID *           pData,                  IN ULONG            nDataSize                                                                                                                                                   );
VOID        LogPacketSend(              IN VOID *           pData,                  IN ULONG            nDataSize                                                                                                                                                   );
VOID        FillPacketSniffData(    IN OUT PacketSniffData* pSniffData                                                                                                                                                                                              );
VOID        LogReset(                                                                                                                                                                                                                                               );
#endif

#endif // #ifndef _XNS_DRW_XNSPACKETMAIN_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  16-May-01   dwoodward   created

