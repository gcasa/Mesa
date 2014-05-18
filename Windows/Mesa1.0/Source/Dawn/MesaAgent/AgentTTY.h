// AgentTTY.h : interface of class AgentTTY
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTTTY_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTTTY_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentTTY
class AgentTTY : public MesaAgent::Agent
{
public:
    AgentTTY(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentTTY();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:
    
    enum Command
    {
        kNop            = 0,
        kTransmit       = 1,
        kReceive        = 2,
        kOpen           = 3,
        kClose          = 4,
        kSetParams      = 5,
        kSendBreak      = 6,
    };

    enum Status
    {
        kSuccess        = 0,
        kBreakDetected  = 1,
        kDataLost       = 2,
        kParityError    = 3,
        kFramingError   = 4,
        kNotReady       = 5,
    };

    enum LineSpeed
    {
        kbps50          = 0,
        kbps75          = 1,
        kbps110         = 2,
        kbps150         = 3,
        kbps300         = 4,
        kbps600         = 5,
        kbps1200        = 6,
        kbps1800        = 7,
        kbps2000        = 8,
        kbps2400        = 9,
        kbps3600        = 10,
        kbps4800        = 11,
        kbps7200        = 12,
        kbps9600        = 13,
        kbps19200       = 14,
    };

    enum CharLength
    {
        kFiveBits = 0,
        kSixBits = 1,
        kSevenBits = 2,
        kEightBits = 3,
    };

    enum Parity
    {
        kNone = 0,
        kOdd  = 1,
        kEven = 2,
    };

    enum StopBits
    {
        kOne = 0,
        kOnePointFive = 1,
        kTwo = 2,
    };

#pragma pack (1)

    struct FCB
    {
	    u16 m_nInterruptSelector;
        u16 m_nCommand;
        u16 m_nStatus;
        u8  m_data;
        u8  m_unused;
        u16 m_nLineSpeed;
        u16 m_nCharLength;
        u16 m_nParity;
        u16 m_nStopBits;
        u16 m_nCTS;
        u16 m_nDSR;
        u16 m_nRTS;
        u16 m_nDTR;
        u16 m_nReadyToReceive;
        u16 m_nReadyToTransmit;
    };

#pragma pack ()
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTTTY_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
