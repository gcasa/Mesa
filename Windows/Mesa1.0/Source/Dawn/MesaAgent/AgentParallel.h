// AgentParallel.h : interface of class AgentParallel
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTPARALLEL_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTPARALLEL_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentParallel
class AgentParallel : public MesaAgent::Agent
{
public:
    AgentParallel(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentParallel();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:

#pragma pack (1)

    struct FCB
    {
       u32 m_nQueueNext;
       u16 m_nInterruptSelector;
       u16 m_nStopAgent;
       u16 m_nAgentStopped;
       u16 m_nNumberPorts;
    };

#pragma pack ()

};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTPARALLEL_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
