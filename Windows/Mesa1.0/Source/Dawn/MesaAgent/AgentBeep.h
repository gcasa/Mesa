// AgentBeep.h : interface of class AgentBeep
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTBEEP_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTBEEP_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentBeep
class AgentBeep : public MesaAgent::Agent
{
public:
    AgentBeep(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentBeep();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:
    struct FCB
    {
	    u16	m_nFrequency;
    };
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTBEEP_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
