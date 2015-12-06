// XNSDriverClient.cpp : implementation of class XNSDriverClient
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
// TODO: filter packets here
//
/////////////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
// #include <winsvc.h>
#include "XNSDriverClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef __APPLE__
#define ULONG unsigned long
#define UCHAR unsigned char
#define CString char*
#endif

/////////////////////////////////////////////////////////////////////////////
// module XNSDriverClient private #defines and types
#define FILE_DEVICE_PROTOCOL                            0x8000
#define METHOD_BUFFERED                                 0
#define FILE_ANY_ACCESS                                 0
#define CTL_CODE(DeviceType, Function, Method, Access)  (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define IOCTL_PROTOCOL_SET_OID                          CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_QUERY_OID                        CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_RESET                            CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ENUM_ADAPTERS                             CTL_CODE(FILE_DEVICE_PROTOCOL, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define OID_802_3_CURRENT_ADDRESS				        0x01010102
#define OID_GEN_CURRENT_PACKET_FILTER			        0x0001010E
#define NDIS_PACKET_TYPE_DIRECTED				        0x00000001
#define NDIS_PACKET_TYPE_MULTICAST			            0x00000002
#define NDIS_PACKET_TYPE_BROADCAST			            0x00000008
#define NDIS_PACKET_TYPE_ALL_LOCAL				        0x00000080

typedef struct _PacketOIDData
{
    ULONG m_Oid;
    ULONG m_nLength;
    UCHAR m_data[1];
} PacketOIDData;

/////////////////////////////////////////////////////////////////////////////
// module XNSDriverClient private class Adapter
class Adapter
{
public:
    CString m_adapterName;
    CString m_dosDeviceName;
};

/////////////////////////////////////////////////////////////////////////////
// module XNSDriverClient static variables
// static CArray<Adapter, Adapter &> s_adapters;

/////////////////////////////////////////////////////////////////////////////
// class XNSDriverClient static variables
XNSDriverClient XNSDriverClient::s_this;

/////////////////////////////////////////////////////////////////////////////
// class XNSDriverClient construction
XNSDriverClient::XNSDriverClient()
{
    /*
    m_hFile = NULL;
    memset(&m_overlappedSend, 0, sizeof(OVERLAPPED));
    m_overlappedSend.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    memset(&m_overlappedReceive, 0, sizeof(OVERLAPPED));
    m_overlappedReceive.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    m_bReceivePending = false;
    memset(m_xnsProcID, 0, sizeof(m_xnsProcID));
     */
}

XNSDriverClient::~XNSDriverClient()
{
    CloseDriver();
}

/////////////////////////////////////////////////////////////////////////////
// class XNSDriverClient public functions
bool
XNSDriverClient::OpenDriver()
{
    /*
    if (m_hFile != NULL) return true;

    // steps to opening the driver:
    // start the driver
    // enumerate the adapters
    // select an adapter (we'll use adapter 0 for now)
    // open the adapter
    // set the packet filter for the adapter

    bool bSuccess = false;
    do
    {
        if (! StartDriver())
        {
            TRACE("XNSDRW: StartDriver() failed.  Error = %d\n", ::GetLastError());
            break;
        }
        
        // after the driver is started, it takes a little while before all adapters
        // are bound; call EnumAdapters until successful; note this is a hack since
        // we really want to wait until all adapters are bound, not until EnumAdapters
        // succeeds
        ::Sleep(1);
        for (int k = 0; k < 100; k++)
        {
            if (EnumAdapters()) break;
            ::Sleep(1);
        }
        if (k == 100)
        {
            TRACE("XNSDRW: EnumAdapters() failed.  Error = %d\n", ::GetLastError());
            break;
        }

        if (! OpenAdapter(0))    // will set the packet filter and m_hFile to non-NULL on success
        {
            TRACE("XNSDRW: OpenAdapter(0) failed.  Error = %d\n", ::GetLastError());
            break;
        }

        if (! GetAdapterXNSProcID())
        {
            TRACE("XNSDRW: GetAdapterXNSProcID() failed.  Error = %d\n", ::GetLastError());
            break;
        }
        bSuccess = true;
    }
    while (false);

    if (bSuccess)
    {
        TRACE("XNSProcID = %02X%02X%02X%02X%02X%02XH\n",
            (u32)m_xnsProcID[0], (u32)m_xnsProcID[1], (u32)m_xnsProcID[2],
            (u32)m_xnsProcID[3], (u32)m_xnsProcID[4], (u32)m_xnsProcID[5]);
    }
    else
    {
        if (m_hFile != NULL) CloseDriver();
        else StopDriver();
    }
    
    return (m_hFile == NULL ? false : true); 
     */
    return false;
}

bool
XNSDriverClient::CloseDriver()
{
    /*
    if (m_hFile == NULL) return true;

    // steps to stopping the driver:
    // cancel IO
    // reset overlappeds
    // clear pending flag
    // close the open adapter
    // clear the adapter list
    // stop the driver

    ::CancelIo(m_hFile);
    ::CloseHandle(m_hFile);
    m_hFile = NULL;

    ResetOverlapped(m_overlappedSend);
    ResetOverlapped(m_overlappedReceive);
    m_bReceivePending = false;

    // clear s_adapters
    s_adapters.RemoveAll();

    // clear m_xnsProcID
    memset(&m_xnsProcID, 0, sizeof(m_xnsProcID));
    
    return StopDriver();
     */
    return true;
}

bool
XNSDriverClient::SendPacket(void * pPacket, u16 nLength)
{
    /*
    if (m_hFile == NULL) return false;
    if ((pPacket == NULL) || (nLength > kMaxEthernetPacketSize)) return false;

    DWORD bytesToSend = (DWORD)nLength;
    DWORD bytesTransferred = 0;
    BOOL bSuccess = false;

    // byte swap packet data
    u32 nWords = (nLength + 1) / 2;
    u8 * pPkt = (u8 *)pPacket;
    for (u32 i = 0; i < nWords; i++)
    {
        m_sendBuffer[i*2] = *(pPkt + 1);
        m_sendBuffer[i*2+1] = *pPkt;
        pPkt +=2;        
    }

    //DumpPacket("PACKET AGENT SEND:", nLength, (u8*)m_sendBuffer);
    //DumpPacket("PACKET AGENT SEND MESA:", nLength, (u8*)pPacket);

    ResetOverlapped(m_overlappedSend);
    bSuccess = ::WriteFile(m_hFile, m_sendBuffer, bytesToSend, &bytesTransferred, &m_overlappedSend);
    if (bSuccess) return ((bytesTransferred == bytesToSend) ? true : false);
    if (::GetLastError() != ERROR_IO_PENDING) return false;
    if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_overlappedSend.hEvent, INFINITE)) return false;
    bSuccess = ::GetOverlappedResult(m_hFile, &m_overlappedSend, &bytesTransferred, TRUE);
    return (bSuccess && (bytesTransferred == bytesToSend)) ? true : false;
     */
    return false;
}

bool
XNSDriverClient::ReceivePacket(void * pPacket, u16 nPacketLength, u16 & nPacketLengthReceivedToSet)
{
    /*
    if (m_hFile == NULL) return false;

    DWORD bufferLength = (DWORD)nPacketLength;
    DWORD bytesReceived = 0;

    BOOL bSuccess = FALSE;
    
    if (m_bReceivePending)
    {
        bSuccess = ::GetOverlappedResult(m_hFile, &m_overlappedReceive, &bytesReceived, FALSE);
        if ((! bSuccess) && (::GetLastError() != ERROR_IO_INCOMPLETE)) m_bReceivePending = false;
    }
    else
    {
        // no receive pending, try to read anew
        ResetOverlapped(m_overlappedReceive);
        bSuccess = ::ReadFile(m_hFile, m_receiveBuffer, sizeof(m_receiveBuffer), &bytesReceived, &m_overlappedReceive);
        if ((! bSuccess) &&  (::GetLastError() == ERROR_IO_PENDING)) m_bReceivePending = true;
    }

    if (bSuccess)
    {
        if (bytesReceived > nPacketLength) bSuccess = FALSE;
        else
        {
            m_bReceivePending = false;
    
#if 1
            // byte swap packet data
            u32 nWords = (bytesReceived + 1) / 2;
            u8 * pPkt = (u8 *)pPacket;
            for (u32 i = 0; i < nWords; i++)
            {
                *(pPkt+1) = m_receiveBuffer[i*2];
                *pPkt = m_receiveBuffer[i*2+1];
                pPkt +=2;        
            }
#else
            memcpy(pPacket, m_receiveBuffer, bytesReceived);
#endif

            nPacketLengthReceivedToSet = (u16)bytesReceived;

            //DumpPacket("PACKET AGENT RECV:", bytesReceived, (u8*)m_receiveBuffer);
            //DumpPacket("PACKET AGENT RECV MESA:", bytesReceived, (u8*)pPacket);
        }
    }

    return bSuccess ? true : false;
     */
    return false;
}

bool
XNSDriverClient::GetXNSProcID(char * pProcIDToSet)
{
    /*
    if (m_hFile == NULL) return false;
    memcpy(pProcIDToSet, m_xnsProcID, sizeof(m_xnsProcID));
    return true; */
    return true;
}

bool
XNSDriverClient::CancelPendingIO()
{
    /*
    if (m_hFile != NULL)
    {
        ::CancelIo(m_hFile);
        m_bReceivePending = false;
        ResetOverlapped(m_overlappedReceive);
    }
    return true;
     */
    return true;
}

/////////////////////////////////////////////////////////////////////////////
// class XNSDriverClient private functions
/*
bool
XNSDriverClient::StartDriver()
{

    ASSERT(m_hFile == NULL);

    BOOL bSuccess = FALSE;
    SERVICE_STATUS serviceStatus;
    SC_HANDLE hServiceControlManager = NULL;
    SC_HANDLE hServiceControlService = NULL;

    do
    {
        if (NULL == (hServiceControlManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS))) break;
        if (NULL == (hServiceControlService = ::OpenService(hServiceControlManager, "XNSDRW", SERVICE_ALL_ACCESS))) break;
        bSuccess = ::StartService(hServiceControlService, 0, NULL);
        if (! bSuccess)
        {
            if (::GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) bSuccess = TRUE;
            break;
        }

        // bSuccess is now true
        for (;;) // ever
        {
            if (! :: QueryServiceStatus(hServiceControlService, &serviceStatus))
            {
                bSuccess = FALSE;
                break;
            }
            if (serviceStatus.dwCurrentState == SERVICE_START_PENDING) { ::Sleep(2000); continue; }
            if (serviceStatus.dwCurrentState != SERVICE_RUNNING) bSuccess = FALSE;
            break;
        }
    }
    while (false);

    if (NULL != hServiceControlService) ::CloseServiceHandle(hServiceControlService);
    if (NULL != hServiceControlManager) ::CloseServiceHandle(hServiceControlManager);

    return bSuccess ? true : false;
}

bool
XNSDriverClient::StopDriver()
{
    ASSERT(m_hFile == NULL);

    BOOL bSuccess = FALSE;
    SERVICE_STATUS serviceStatus;
    SC_HANDLE hServiceControlManager = NULL;
    SC_HANDLE hServiceControlService = NULL;

    do
    {
        if (NULL == (hServiceControlManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS))) break;
        if (NULL == (hServiceControlService = ::OpenService(hServiceControlManager, "XNSDRW", SERVICE_ALL_ACCESS))) break;
        bSuccess = ::ControlService(hServiceControlService, SERVICE_CONTROL_STOP, &serviceStatus);
    }
    while (false);

    if (NULL != hServiceControlService) ::CloseServiceHandle(hServiceControlService);
    if (NULL != hServiceControlManager) ::CloseServiceHandle(hServiceControlManager);

    return bSuccess ? true : false;
}

bool
XNSDriverClient::EnumAdapters()
{
    if (m_hFile != NULL) return true;   // already did this once!

    unsigned char buff[2048];
    unsigned char * pBuff = buff;
    ULONG nBuffLen = 0;
    HANDLE hFileDriver = INVALID_HANDLE_VALUE;
    bool bSuccess = false;

    memset(buff, 0, sizeof(buff));

    do
    {
        // issue: changed to overlapped and wait; check reset adapter
        if (INVALID_HANDLE_VALUE == (hFileDriver = ::CreateFile("\\\\.\\XNSDRWPacket", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0))) break;
        if (! DeviceIoControlSync(hFileDriver, IOCTL_ENUM_ADAPTERS, NULL, 0, buff, sizeof(buff), &nBuffLen)) break;
        bSuccess = true;
    }
    while (false);

    // REPORT REPORT REPORT REPORT
    TRACE("ENUM ADAPTERS ");
    if (! bSuccess)
    {
        TRACE("FAILED: ");
        if (hFileDriver) TRACE("DeviceIoControl Error = %d\n", ::GetLastError());
        else TRACE("CreateFile Error = %d\n", ::GetLastError());
    }

    if (hFileDriver != INVALID_HANDLE_VALUE) ::CloseHandle(hFileDriver);

    if (! bSuccess) return false;

    ULONG nNumAdapters = *((PULONG)pBuff);
    pBuff += sizeof(ULONG);

    // REPORT REPORT REPORT REPORT
    if (nNumAdapters == 0xFF)
    {
        // WARNING: driver thinks the requiredLength is > than the buffer length
        ULONG nRequiredLength = *((PULONG)pBuff);
        pBuff += sizeof(ULONG);
        ULONG nBuffSizeLength = *((PULONG)pBuff);
        pBuff += sizeof(ULONG);
        ULONG nReportedNumAdapters = *((PULONG)pBuff);
        TRACE("FAILED BAD LEN: nRequiredLen = %d, nProvidedLen = %d, nReportedNumAdapters = %d\n", nRequiredLength, nBuffSizeLength, nReportedNumAdapters);
        return false;
    }
    if (nNumAdapters == 0)
    {
        TRACE("FAILED: 0 ADAPTERS\n");
    }

    if (nNumAdapters == 0) return false;

    // REPORT REPORT REPORT REPORT
    TRACE("SUCCESS: %d ADAPTERS\n", nNumAdapters);

    s_adapters.SetSize(nNumAdapters);

    unsigned long i = 0;
    for (;;)
    {
        s_adapters[i].m_adapterName = pBuff; pBuff += 2;
        while (*pBuff) { s_adapters[i].m_adapterName += pBuff; pBuff += 2; }
        pBuff += 2;
        s_adapters[i].m_dosDeviceName = pBuff; pBuff += 2;
        while (*pBuff) { s_adapters[i].m_dosDeviceName += pBuff; pBuff += 2; }
        if (++i == nNumAdapters) break;
        pBuff += 2;
    }

    return true;
}

bool
XNSDriverClient::OpenAdapter(int nAdapterIndex)
{
    ASSERT(m_hFile == NULL);
    if ((nAdapterIndex < 0) || (nAdapterIndex > s_adapters.GetUpperBound())) return false;

    CString symbolicLinkName = "\\\\.\\" + s_adapters[nAdapterIndex].m_dosDeviceName.Mid(12);
    ULONG nSize = sizeof(PacketOIDData) + sizeof(ULONG) - 1;
    PacketOIDData * pData = (PacketOIDData *) new UCHAR [nSize];
    DWORD bytesReturned = 0;
    
    bool bSuccess = false;
    do
    {
        if (pData == NULL) break;
        
        m_hFile = ::CreateFile((const char *)symbolicLinkName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
        if (m_hFile == INVALID_HANDLE_VALUE) break;

        pData->m_Oid = OID_GEN_CURRENT_PACKET_FILTER;
        pData->m_nLength = sizeof(ULONG);

        // This filter business is confusing.  The driver has a PacketReceiveIndicate function,
        // that allows me to filter out packets I don't want.  If I don't set the filter here to
        // everything, then whatever isn't set as a filter here bypasses PacketReceiveIndicate
        // and gets put on the driver's packet receive queue.  So, I try to set a filter on everything
        // here so that PacketReceiveIndicate can trap out packets I don't want.
        // The following values are all the combinations I can set.  For some reason PROMISCUOUS doesn't
        // work with my driver.
        *((PULONG)(pData->m_data))  = NDIS_PACKET_TYPE_DIRECTED;
        *((PULONG)(pData->m_data)) |= NDIS_PACKET_TYPE_BROADCAST;
        *((PULONG)(pData->m_data)) |= NDIS_PACKET_TYPE_MULTICAST;
        *((PULONG)(pData->m_data)) |= NDIS_PACKET_TYPE_ALL_LOCAL;

        if (! DeviceIoControlSync(m_hFile, IOCTL_PROTOCOL_SET_OID, pData, nSize, pData, nSize, &bytesReturned)) break;
    
        bSuccess = true;
    }
    while (false);

    if (pData) delete [] pData;

    if (! bSuccess)
    {
        if (m_hFile != INVALID_HANDLE_VALUE) ::CloseHandle(m_hFile);
        m_hFile = NULL;
    }

    return (m_hFile == NULL ? false : true);
}

bool
XNSDriverClient::GetAdapterXNSProcID()
{
    if (m_hFile == NULL) return false;

    ULONG nSize = sizeof(PacketOIDData) + sizeof(m_xnsProcID) - 1;
    PacketOIDData * pData = (PacketOIDData *) new UCHAR [nSize];
    DWORD bytesReturned = 0;

    bool bSuccess = false;
    do
    {
        if (pData == NULL) break;

        pData->m_Oid = OID_802_3_CURRENT_ADDRESS;
        pData->m_nLength = sizeof(m_xnsProcID);
        memset(pData->m_data, 0, sizeof(m_xnsProcID));

        if (! DeviceIoControlSync(m_hFile, IOCTL_PROTOCOL_QUERY_OID, pData, nSize, pData, nSize, &bytesReturned)) break;
    
        bSuccess = true;
    }
    while (false);

    if (bSuccess && (bytesReturned != nSize)) bSuccess = false;
    if (bSuccess) memcpy(m_xnsProcID, pData->m_data, sizeof(m_xnsProcID));
    if (pData) delete [] pData;

    return bSuccess;
}

bool
XNSDriverClient::DeviceIoControlSync(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned)
{
    OVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(overlapped));
    if (NULL == (overlapped.hEvent = ::CreateEvent(NULL, true, false, NULL))) return false;

    BOOL bResult = ::DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, &overlapped);
    if ((! bResult) && (::GetLastError() == ERROR_IO_PENDING))
    {
        bResult = ::GetOverlappedResult(hDevice, &overlapped, lpBytesReturned, true);
    }

    ::CloseHandle(overlapped.hEvent);

    return bResult ? true : false;
}

void
XNSDriverClient::ResetOverlapped(OVERLAPPED & overlapped)
{
    HANDLE hEvent = overlapped.hEvent;
    ::ResetEvent(hEvent);
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = hEvent;
}

void
XNSDriverClient::DumpPacket(const char * pHeader, u32 nLen, u8 * pData)
{
    TRACE("%s\n", pHeader);

    while (nLen)
    {
        u32 limit = (nLen < 16 ? nLen : 16);
        TRACE("DATA: ");
        for (u32 i = 0; i < limit; i++)
        {
            TRACE("%02X ", (u32)(*pData));
            pData++;
        }
        nLen -= limit;
        TRACE("\n");
    }
} */

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  15-May-01   dwoodward   created