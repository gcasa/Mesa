// AgentProcessor.h : interface of class AgentProcessor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTPROCESSOR_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTPROCESSOR_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentProcessor
class AgentProcessor : public MesaAgent::Agent
{
public:
    AgentProcessor(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentProcessor();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:
    void Init();
    void GetGMT();
    void SetGMT();
protected:

#pragma pack(1)

    struct FCB
    {
	    u8	m_procID[6];                        //  0
	    u16	m_nMicrosecondsPerHundredPulses;    //  6
	    u16	m_nMillisecondsPerTick;			    //  8
	    u16	m_alignmentFiller;                  // 10
	    u32	m_nRealMemoryPageCount;             // 12
	    u32	m_nVirtualMemoryPageCount;          // 16
	    u32	m_nMesaTimeGMT;                     // 20
	    u16	m_nCommand;                         // 24
	    u16	m_nStatus;                          // 26
    };

#pragma pack()

    enum Command
    {
        kCommandInit = 0,
        kCommandGetGMT = 1,
        kCommandSetGMT = 2,
    };
    enum Status
    {
        kStatusInProgress = 0,
        kStatusSuccess = 1,
        kStatusFailure = 2,
    };
    enum
    {
        kMesaGMTEpoch = 2177452800,             // (69 years * 365 days + 17 leap days) * 24 hours * 60 minutes * 60 seconds
    };
protected:
    unsigned __int64 m_nMesaEpoch;
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTPROCESSOR_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
