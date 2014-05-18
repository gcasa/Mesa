// AgentNetwork.cpp : implementation of class AgentNetwork
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentNetwork.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentNetwork
AgentNetwork::AgentNetwork(MesaAgent::AgentIndex agentIndex, void * pFCB) :
    MesaAgent::Agent(agentIndex, pFCB), m_xnsDriverClient(XNSDriverClient::GetXNSDriverClient())
{
    FCB * pNetworkFCB = (FCB *)pFCB;
    memset(pNetworkFCB, 0, sizeof(FCB));
    Stop();
}

AgentNetwork::~AgentNetwork()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentNetwork public virtual functions
bool
AgentNetwork::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;

    if (MESA_TO_C_UNSPECIFIED(pFCB->m_nStopAgent))
    {
        if (0 == MESA_TO_C_UNSPECIFIED(pFCB->m_nReceiveStopped)) Stop();
    }
    else if (MESA_TO_C_LONG_UNSPECIFIED(pFCB->m_nVirtualAddressReceiveIOCB))
    {
        if (0 == MESA_TO_C_UNSPECIFIED(pFCB->m_nReceiveStopped)) Receive();
    }
    else if (MESA_TO_C_LONG_UNSPECIFIED(pFCB->m_nVirtualAddressTransmitIOCB))
    {
        if (0 == MESA_TO_C_UNSPECIFIED(pFCB->m_nTransmitStopped)) Transmit();
    }
    else
    {
        if (0 != MESA_TO_C_UNSPECIFIED(pFCB->m_nReceiveStopped)) Start();
    }
    
    return true;
}

#define PACKET_BUFF_MAX_SIZE 5

void
AgentNetwork::ReceivePacketsIntoBuffer()
{
    ReceivedPacket packet;
    while (m_xnsDriverClient.ReceivePacket(packet.m_packet, sizeof(packet.m_packet), packet.m_nSizeActual))
    {
        m_packetBuffer.Add(packet);
    }
    while (m_packetBuffer.GetSize() > PACKET_BUFF_MAX_SIZE) m_packetBuffer.RemoveAt(0);
}

bool
AgentNetwork::Poll()
{
    ReceivePacketsIntoBuffer();

    if ((0 == m_arrayReceive.GetSize()) || (0 == m_packetBuffer.GetSize())) return true;
    bool bRaiseInterrupt = false;
    
    FCB * pFCB = (FCB *)m_pFCB;
    while (0 != m_arrayReceive.GetSize())
    {
        // try to receive a packet
        // if not successful break;
        // otherwise fill in the IOCB from slot 0 in the array, remove it, and raise the receive
        // interrupt
        u32 nVirtualAddressIOCB = m_arrayReceive[0];
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(nVirtualAddressIOCB, true);
        void * pMesaPacketBuffer = (void *)MesaProcessor::GetRealAddress(MESA_TO_C_LONG_UNSPECIFIED(pIOCB->m_nVirtualAddressBuffer), true);
        u16 nMesaBufferLen = MESA_TO_C_UNSPECIFIED(pIOCB->m_nBufferLength);
        
#if 0
        if (m_packetBuffer.GetSize()) TRACE("MesaRealAddr = %d, MesaBuffLen = %d, PacketLen = %d\n",
            (u32)pMesaPacketBuffer - (u32)MesaProcessor::GetStartOfRealMemory(), pIOCB->m_nBufferLength, m_packetBuffer[0].m_nSizeActual);
#endif
        
        while (m_packetBuffer.GetSize() && (m_packetBuffer[0].m_nSizeActual > nMesaBufferLen)) m_packetBuffer.RemoveAt(0);

        if (m_packetBuffer.GetSize() == 0) break;

        pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kCompletedOK);
        pIOCB->m_nActualLength = C_TO_MESA_UNSPECIFIED(m_packetBuffer[0].m_nSizeActual);
        memcpy(pMesaPacketBuffer, m_packetBuffer[0].m_packet, m_packetBuffer[0].m_nSizeActual);
        //TRACE("ENET RECV: %d bytes\n", m_packetBuffer[0].m_nSizeActual);
        m_packetBuffer.RemoveAt(0);
        m_arrayReceive.RemoveAt(0);
        bRaiseInterrupt = true;
    }

    if (bRaiseInterrupt) RaiseInterrupt(MESA_TO_C_UNSPECIFIED(pFCB->m_nInterruptSelectorReceive));
    return true;
}

#if 0
bool
AgentNetwork::Poll()
{
    if (0 == m_arrayReceive.GetSize()) return true;
    bool bRaiseInterrupt = false;
    
    FCB * pFCB = (FCB *)m_pFCB;
    u16 nBytesReceived = 0;
    while (0 != m_arrayReceive.GetSize())
    {
        // try to receive a packet
        // if not successful break;
        // otherwise fill in the IOCB from slot 0 in the array, remove it, and raise the receive
        // interrupt
        u32 nVirtualAddressIOCB = m_arrayReceive[0];
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(nVirtualAddressIOCB, true);

        if (m_xnsDriverClient.ReceivePacket(
            (void *)MesaProcessor::GetRealAddress(MESA_TO_C_LONG_UNSPECIFIED(pIOCB->m_nVirtualAddressBuffer), true),
            MESA_TO_C_UNSPECIFIED(pIOCB->m_nBufferLength),
            nBytesReceived))
        {
            //TRACE("ENET RECV: %d bytes\n", nBytesReceived);
            m_arrayReceive.RemoveAt(0);
            pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kCompletedOK);
            pIOCB->m_nActualLength = C_TO_MESA_UNSPECIFIED(nBytesReceived);
            bRaiseInterrupt = true;
        }
        else break;
    }

    if (bRaiseInterrupt) RaiseInterrupt(MESA_TO_C_UNSPECIFIED(pFCB->m_nInterruptSelectorReceive));
    return true;
}
#endif

u32
AgentNetwork::GetFCBSize()
{
    return sizeof(FCB);
}

void
AgentNetwork::Start()
{
    //TRACE("ENET STRT\n");
    FCB * pFCB = (FCB *)m_pFCB;
    CancelIO();
    pFCB->m_nReceiveStopped = MESA_TO_C_UNSPECIFIED(0);
    pFCB->m_nTransmitStopped = MESA_TO_C_UNSPECIFIED(0);
}

void
AgentNetwork::Stop()
{
    //TRACE("ENET STOP\n");
    FCB * pFCB = (FCB *)m_pFCB;
    CancelIO();
    pFCB->m_nReceiveStopped = MESA_TO_C_UNSPECIFIED(1);
    pFCB->m_nTransmitStopped = MESA_TO_C_UNSPECIFIED(1);
}

void
AgentNetwork::CancelIO()
{
    u32 size = m_arrayReceive.GetSize();
    m_xnsDriverClient.CancelPendingIO();
    while (size > 0)
    {
        size--;
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(m_arrayReceive[size], true);
        pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kCompletedOK);
        pIOCB->m_nActualLength = C_TO_MESA_UNSPECIFIED(0);
    }
    m_arrayReceive.RemoveAll();
}

void
AgentNetwork::Transmit()
{
    FCB * pFCB = (FCB *)m_pFCB;
    // send all packets right away, setting status on the IOCBs then raise the interrupt
    u32 nVirtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pFCB->m_nVirtualAddressTransmitIOCB);
    while (nVirtualAddressIOCB)
    {
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(nVirtualAddressIOCB, true);
        if (pIOCB)
        {
            u16 nBufferLength = MESA_TO_C_UNSPECIFIED(pIOCB->m_nBufferLength);
            //TRACE("ENET SEND: %d bytes\n", nBufferLength);
            void * pPacket = (void *)MesaProcessor::GetRealAddress(MESA_TO_C_LONG_UNSPECIFIED(pIOCB->m_nVirtualAddressBuffer), false);
            
                 if (NULL == pPacket)                                       pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kBadCRC);
            else if (nBufferLength > kMaxPacketSize)                        pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kPacketTooLong);
            else if (m_xnsDriverClient.SendPacket(pPacket, nBufferLength))
            {
                pIOCB->m_nActualLength = pIOCB->m_nBufferLength;
                pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kCompletedOK);
            }
            else pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kBadCRC);
        }
        nVirtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pIOCB->m_nVirtualAddressNext);
    }
    RaiseInterrupt(MESA_TO_C_UNSPECIFIED(pFCB->m_nInterruptSelectorTransmit));
}

void
AgentNetwork::Receive()
{
    FCB * pFCB = (FCB *)m_pFCB;

   // add the IOCBs to the receive array and return
   u32 nVirtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pFCB->m_nVirtualAddressReceiveIOCB);
   while (nVirtualAddressIOCB)
   {
        //TRACE("ENET RECV: Add Buffer\n");
        IOCB * pIOCB = (IOCB *)MesaProcessor::GetRealAddress(nVirtualAddressIOCB, true);
        pIOCB->m_status = C_TO_MESA_UNSPECIFIED(kInProgress);
        m_arrayReceive.Add(nVirtualAddressIOCB);
        nVirtualAddressIOCB = MESA_TO_C_LONG_UNSPECIFIED(pIOCB->m_nVirtualAddressNext);
   }
}

/////////////////////////////////////////////////////////////////////////////
// class AgentNetwork private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
