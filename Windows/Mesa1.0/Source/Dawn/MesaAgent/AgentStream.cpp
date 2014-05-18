// AgentStream.cpp : implementation of class AgentStream
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentStream.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentStream
AgentStream::AgentStream(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pStreamFCB = (FCB *)pFCB;
    pStreamFCB->m_nVirtualAddressFirstStream = C_TO_MESA_LONG_UNSPECIFIED(0);
    pStreamFCB->m_nVirtualAddressStreamToManipulate = C_TO_MESA_LONG_UNSPECIFIED(0);
    pStreamFCB->m_nCommand = C_TO_MESA_LONG_UNSPECIFIED(kIdle);
    pStreamFCB->m_nResult = C_TO_MESA_LONG_UNSPECIFIED(kCompleted);
    pStreamFCB->m_nInterruptSelector = C_TO_MESA_UNSPECIFIED(0);
    pStreamFCB->m_nStopAgent = C_TO_MESA_UNSPECIFIED(0);
    pStreamFCB->m_nAgentStopped = C_TO_MESA_UNSPECIFIED(1);
    pStreamFCB->m_nStreamWordSize = C_TO_MESA_UNSPECIFIED((sizeof(Stream) / 2));

}

AgentStream::~AgentStream()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentStream public virtual functions
bool
AgentStream::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;
    pFCB->m_nResult = C_TO_MESA_UNSPECIFIED(kError);
    return true;
}

bool
AgentStream::Poll()
{
    return false;
}

u32
AgentStream::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentStream private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
