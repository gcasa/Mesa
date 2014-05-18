// AgentNetwork.h : interface of class AgentNetwork
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTNETWORK_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTNETWORK_H

#include "MesaAgent.h"
#include "../AppFramework/XNSDriverClient.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentNetwork
class AgentNetwork : public MesaAgent::Agent
{
public:
    AgentNetwork(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentNetwork();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:

    void Start();
    void Stop();
    void CancelIO();
    void Transmit();
    void Receive();

    void ReceivePacketsIntoBuffer();

private:
    class ReceivedPacket
    {
    public:
        u8 m_packet[XNSDriverClient::kMaxEthernetPacketSize];
        u16 m_nSizeActual;
    };

    CArray<ReceivedPacket, ReceivedPacket &> m_packetBuffer;
    CArray<u32, u32 &> m_arrayReceive;
    XNSDriverClient & m_xnsDriverClient;
private:
    enum Status
    {
        kInProgress                 = 0x01,
        kCompletedOK                = 0x02,
        kTooManyCollisions          = 0x04,
        kBadCRC                     = 0x08, // also for Tx errors
        kAlignmentError             = 0x10,
        kPacketTooLong              = 0x20,
        kBadCRCAndAlignmentError    = 0x80,
    };

    enum
    {
        kXNSPacketType              = 0x0600,
        kMaxPacketSize              = 1536,
    };

#pragma pack (1)

    struct IOCB
    {
        u32 m_nVirtualAddressBuffer;
        u16 m_nBufferLength;
        u16 m_nActualLength;
        u8  m_status;
        u8  m_packetType;
        u16 m_nRetries;
        u32 m_nVirtualAddressNext;
    };

    struct FCB
    {
        u32 m_nVirtualAddressReceiveIOCB;   // only one of Receive or Trasmit
        u32 m_nVirtualAddressTransmitIOCB;  // will be set at a time
        u16 m_nInterruptSelectorReceive;    // notify head of packet Rx
        u16 m_nInterruptSelectorTransmit;   // notify head of Tx complete
        u16 m_nStopAgent;
        u16 m_nReceiveStopped;
        u16 m_nTransmitStopped;
        u16 m_nHearSelf;
        u16 m_procID[3];
        u16 m_nPacketsMissed;
        u16 m_nUnused;
    };

#pragma pack ()

};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTNETWORK_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
