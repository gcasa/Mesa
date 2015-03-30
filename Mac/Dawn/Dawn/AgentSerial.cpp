// AgentSerial.cpp : implementation of class AgentSerial
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentSerial.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentSerial
AgentSerial::AgentSerial(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pSerialFCB = (FCB *)pFCB;
    memset(pSerialFCB, 0, sizeof(FCB));
    pSerialFCB->m_nStopAgent = C_TO_MESA_UNSPECIFIED(0);
    pSerialFCB->m_nAgentStopped = C_TO_MESA_UNSPECIFIED(1);
    pSerialFCB->m_nCommand = C_TO_MESA_UNSPECIFIED(kNop);
    pSerialFCB->m_parameterBlock.m_nRS232Chars = C_TO_MESA_UNSPECIFIED(0x4C01);
}

AgentSerial::~AgentSerial()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentSerial public virtual functions
bool
AgentSerial::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;
    pFCB->m_nCommandStatus = C_TO_MESA_UNSPECIFIED(kParamUnimplemented);

    return true;
}

bool
AgentSerial::Poll()
{
    return false;
}

u32
AgentSerial::GetFCBSize()
{
    return 0;
    //return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentSerial private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
