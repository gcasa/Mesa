// AgentFloppy.h : interface of class AgentFloppy
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTFLOPPY_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTFLOPPY_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentFloppy
class AgentFloppy : public MesaAgent::Agent
{
public:
    AgentFloppy(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentFloppy();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
private:
    struct FCB
    {
	    u8	m_nDummy;                           //  0
    };
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTFLOPPY_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
