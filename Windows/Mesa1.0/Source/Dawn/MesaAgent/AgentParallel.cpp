// AgentParallel.cpp : implementation of class AgentParallel
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentParallel.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentParallel
AgentParallel::AgentParallel(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pPrinterFCB = (FCB *)pFCB;
    memset(pPrinterFCB, 0, sizeof(FCB));
}

AgentParallel::~AgentParallel()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentParallel public virtual functions
bool
AgentParallel::Initiate()
{
    return true;
}

bool
AgentParallel::Poll()
{
    return false;
}

u32
AgentParallel::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentParallel private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
