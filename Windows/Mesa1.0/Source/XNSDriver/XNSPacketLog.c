// XNSPacketLog.c : implementation of packet logging
//
// Copyright (c) 2001, Don Woodward.  All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////


#include "XNSPacketMain.h"

#ifdef XNSDRW_LOG_PACKETS

static unsigned long s_nNumPacketsRejected = 0;
static unsigned long s_nNumPacketsSent = 0;
static unsigned long s_nNumPacketsReceived = 0;

static unsigned char s_RejectHeader[] = "REJECT";
static unsigned char s_SendHeader[] = "TRNS";
static unsigned char s_ReceiveHeader[] = "RECV";
static unsigned char s_CopyHeader[] = "";

static const unsigned int s_kSizeRejectHeader = (sizeof(s_RejectHeader) - 1);
static const unsigned int s_kSizeSendHeader = (sizeof(s_SendHeader) - 1);
static const unsigned int s_kSizeReceiveHeader = (sizeof(s_ReceiveHeader) - 1);
static const unsigned int s_kSizeCopyHeader = (sizeof(s_CopyHeader) - 1);

static unsigned char s_buff[XNS_DRW_MAX_SNIFF_SIZE];
static unsigned int s_nNumBytesInBuffer = 0;

VOID
LogReset()
{
    s_nNumPacketsRejected = 0;    
    s_nNumPacketsSent = 0;    
    s_nNumPacketsReceived = 0;    
    s_nNumBytesInBuffer = 0;    
}

VOID
LogRejectedPacketHeader(IN VOID * pHeaderData, IN ULONG nHeaderDataSize)
{
    s_nNumPacketsRejected++;
    if ((nHeaderDataSize + s_kSizeRejectHeader + s_nNumBytesInBuffer) >= XNS_DRW_MAX_SNIFF_SIZE) return;
    memcpy(s_buff + s_nNumBytesInBuffer, s_RejectHeader, s_kSizeRejectHeader);
    s_nNumBytesInBuffer += s_kSizeRejectHeader;
    memcpy(s_buff + s_nNumBytesInBuffer, pHeaderData, nHeaderDataSize);
    s_nNumBytesInBuffer += nHeaderDataSize;
}

VOID
LogPacketReceive(IN VOID * pData, IN ULONG nDataSize)
{
    s_nNumPacketsReceived++;
    if ((nDataSize + s_kSizeReceiveHeader + s_nNumBytesInBuffer) >= XNS_DRW_MAX_SNIFF_SIZE) return;
    memcpy(s_buff + s_nNumBytesInBuffer, s_ReceiveHeader, s_kSizeReceiveHeader);
    s_nNumBytesInBuffer += s_kSizeReceiveHeader;
    memcpy(s_buff + s_nNumBytesInBuffer, pData, nDataSize);
    s_nNumBytesInBuffer += nDataSize;
}

VOID
LogPacketSend(IN VOID * pData, IN ULONG nDataSize)
{
    s_nNumPacketsSent++;
    if ((nDataSize + s_kSizeSendHeader + s_nNumBytesInBuffer) >= XNS_DRW_MAX_SNIFF_SIZE) return;
    memcpy(s_buff + s_nNumBytesInBuffer, s_SendHeader, s_kSizeSendHeader);
    s_nNumBytesInBuffer += s_kSizeSendHeader;
    memcpy(s_buff + s_nNumBytesInBuffer, pData, nDataSize);
    s_nNumBytesInBuffer += nDataSize;
}

VOID
FillPacketSniffData(IN OUT PacketSniffData * pSniffData)
{
    unsigned long len = 0;

    if ((s_kSizeCopyHeader + s_nNumBytesInBuffer) < XNS_DRW_MAX_SNIFF_SIZE)
    {
        memcpy(s_buff + s_nNumBytesInBuffer, s_CopyHeader, s_kSizeCopyHeader);
        s_nNumBytesInBuffer += s_kSizeCopyHeader;
    }

    pSniffData->m_nPacketsSent =  s_nNumPacketsSent;
    pSniffData->m_nPacketsReceived = s_nNumPacketsReceived;
    pSniffData->m_nPacketsRejected = s_nNumPacketsRejected;
    pSniffData->m_nDriverSniffedDataLength = s_nNumBytesInBuffer;
    len = (pSniffData->m_nSniffDataLength < s_nNumBytesInBuffer ? pSniffData->m_nSniffDataLength : s_nNumBytesInBuffer);
    if (len > 0) memcpy(pSniffData->m_data, s_buff, len);
}

#endif  // #ifdef XNSDRW_LOG_PACKETS

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//  16-May-01   dwoodward   created
