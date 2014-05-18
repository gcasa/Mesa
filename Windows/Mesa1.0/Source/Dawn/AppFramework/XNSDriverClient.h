// XNSDriverClient.h : interface of class XNSDriverClient
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_DAWN_APPFRAMEWORK_XNSDRIVERCLIENT_H
#define _SOURCE_DAWN_APPFRAMEWORK_XNSDRIVERCLIENT_H

#include <PrincOps/PrincOps_PlatformTypes.h>

/////////////////////////////////////////////////////////////////////////////
// class XNSDriverClient
class XNSDriverClient 
{
public:
    XNSDriverClient();
    virtual ~XNSDriverClient();

    static XNSDriverClient & GetXNSDriverClient() { return s_this; }

public:
    bool OpenDriver();
    bool CloseDriver();
    bool IsDriverOpen() { return m_hFile != NULL; }

    bool SendPacket(void * pPacket, u16 nLength);
    bool ReceivePacket(void * pPacket, u16 nPacketLength, u16 & nPacketLengthReceivedToSet);

    bool GetXNSProcID(char * procIDToSet);
        // will set 6 bytes starting at procIDToSet with the proc id

    bool CancelPendingIO();

protected:
    bool StartDriver();
    bool StopDriver();
    bool EnumAdapters();
    bool OpenAdapter(int nAdapterIndex);
    bool GetAdapterXNSProcID();
    bool DeviceIoControlSync(HANDLE hDevice, DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned);
    void ResetOverlapped(OVERLAPPED & overlapped);
    void DumpPacket(const char * pHeader, u32 nLen, u8 * pData);

private:
    XNSDriverClient(XNSDriverClient & xnsDriverClient); // disallow copy construction by providing no implementation

private:
    static XNSDriverClient s_this;

public:

    enum { kMaxEthernetPacketSize = 1536, };

private:
    HANDLE m_hFile;
    OVERLAPPED m_overlappedSend;
    OVERLAPPED m_overlappedReceive;
    bool m_bReceivePending;
    u8 m_receiveBuffer[kMaxEthernetPacketSize];
    u8 m_sendBuffer[kMaxEthernetPacketSize];

    unsigned char m_xnsProcID[6];
};

#endif // #ifndef _SOURCE_DAWN_APPFRAMEWORK_XNSDRIVERCLIENT_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  04-May-01   dwoodward   created
