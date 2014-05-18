// XNSPacketPublic.h : public interface of XNS packet driver
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _XNS_DRW_XNSPACKETPULIC_H
#define _XNS_DRW_XNSPACKETPULIC_H

/////////////////////////////////////////////////////////////////////////////
// #defines
#undef NT_DEVICE_NAME
#undef DOS_DEVICE_NAME
#undef FILE_DEVICE_PROTOCOL
#undef METHOD_BUFFERED
#undef FILE_ANY_ACCESS
#undef CTL_CODE
#undef IOCTL_PROTOCOL_SET_OID
#undef IOCTL_PROTOCOL_QUERY_OID
#undef IOCTL_PROTOCOL_RESET
#undef IOCTL_ENUM_ADAPTERS
#undef IOCTL_SNIFF_PACKETS
#undef OID_GEN_CURRENT_PACKET_FILTER
#undef NDIS_PACKET_TYPE_DIRECTED	
#undef NDIS_PACKET_TYPE_BROADCAST
#undef NDIS_PACKET_TYPE_PROMISCUOUS

#define ETHERNET_HEADER_LENGTH      14
#define XNS_PACKET_HEADER_SIZE      ETHERNET_HEADER_LENGTH
#define XNS_PACKET_MAX_SIZE         1536
#define XNS_PACKET_MAX_DATA_SIZE    (XNS_PACKET_MAX_SIZE - XNS_PACKET_HEADER_SIZE)
#define XNS_PACKET_TYPE_HI          6
#define XNS_PACKET_TYPE_LO          0

#define NT_DEVICE_NAME              L"\\Device\\XNSDRWPacket"
#define DOS_DEVICE_NAME             L"\\DosDevices\\XNSDRWPacket"
#define FILE_DEVICE_PROTOCOL        0x8000
#define METHOD_BUFFERED             0
#define FILE_ANY_ACCESS             0
#define CTL_CODE(DeviceType, Function, Method, Access ) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define IOCTL_PROTOCOL_SET_OID      CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_QUERY_OID    CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_RESET        CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ENUM_ADAPTERS         CTL_CODE(FILE_DEVICE_PROTOCOL, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SNIFF_PACKETS         CTL_CODE(FILE_DEVICE_PROTOCOL, 2222 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define XNS_DRW_MAX_SNIFF_SIZE      (8192)
#define OID_GEN_CURRENT_PACKET_FILTER			0x0001010E
#define OID_802_3_CURRENT_ADDRESS				0x01010102
#define NDIS_PACKET_TYPE_DIRECTED				0x00000001
#define NDIS_PACKET_TYPE_MULTICAST				0x00000002
#define NDIS_PACKET_TYPE_ALL_MULTICAST			0x00000004
#define NDIS_PACKET_TYPE_BROADCAST				0x00000008
#define NDIS_PACKET_TYPE_SOURCE_ROUTING			0x00000010
#define NDIS_PACKET_TYPE_PROMISCUOUS			0x00000020
#define NDIS_PACKET_TYPE_SMT					0x00000040
#define NDIS_PACKET_TYPE_ALL_LOCAL				0x00000080
#define NDIS_PACKET_TYPE_GROUP					0x00001000
#define NDIS_PACKET_TYPE_ALL_FUNCTIONAL			0x00002000
#define NDIS_PACKET_TYPE_FUNCTIONAL				0x00004000
#define NDIS_PACKET_TYPE_MAC_FRAME				0x00008000

typedef struct _PacketOIDData
{
    ULONG m_Oid;
    ULONG m_nLength;
    UCHAR m_data[1];                    // make a structure with the size you want

} PacketOIDData;

/////////////////////////////////////////////////////////////////////////////
//  types
typedef struct _PacketSniffData
{
    ULONG m_nPacketsSent;
    ULONG m_nPacketsReceived;
    ULONG m_nPacketsRejected;
    ULONG m_nDriverSniffedDataLength;
    ULONG m_nSniffDataLength;
    UCHAR m_data[1];                    // make a structure with the size you want

} PacketSniffData;

#endif // #ifndef _XNS_DRW_XNSPACKETPULIC_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  16-May-01   dwoodward   created
