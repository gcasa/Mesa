// AgentBeep.cpp : implementation of class AgentBeep
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentBeep.h"
#include "../MesaProcessor/MesaProcessor.h"
#include <dos.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentBeep
AgentBeep::AgentBeep(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    ((FCB*)m_pFCB)->m_nFrequency = 0;
}

AgentBeep::~AgentBeep()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentBeep public virtual functions
bool
AgentBeep::Initiate()
{
    u16 nFrequency = MESA_TO_C_UNSPECIFIED(((FCB*)m_pFCB)->m_nFrequency);
    // ISSUE: if (nFrequency) ::sound(nFrequency) else ::nosound();
    return true;
}

bool
AgentBeep::Poll()
{
    return false;
}

u32
AgentBeep::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentBeep private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
