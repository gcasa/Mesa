// AgentFloppy.cpp : implementation of class AgentFloppy
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentFloppy.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentFloppy
AgentFloppy::AgentFloppy(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pFloppyFCB = (FCB *)pFCB;
    memset(pFloppyFCB, 0, sizeof(FCB));
}

AgentFloppy::~AgentFloppy()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentFloppy public virtual functions
bool
AgentFloppy::Initiate()
{
    return true;
}

bool
AgentFloppy::Poll()
{
    return false;
}

u32
AgentFloppy::GetFCBSize()
{
    return 0;
    //return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentFloppy private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
