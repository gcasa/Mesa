// AgentTTY.cpp : implementation of class AgentTTY
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentTTY.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentTTY
AgentTTY::AgentTTY(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pTTYFCB = (FCB *)pFCB;
    pTTYFCB->m_nInterruptSelector = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nCommand = C_TO_MESA_UNSPECIFIED(kNop);
    pTTYFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
    pTTYFCB->m_data = 0;
    pTTYFCB->m_unused = 0;
    pTTYFCB->m_nLineSpeed = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nCharLength = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nParity = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nStopBits = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nCTS = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nDSR = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nRTS = C_TO_MESA_UNSPECIFIED(1);
    pTTYFCB->m_nDTR = C_TO_MESA_UNSPECIFIED(1);
    pTTYFCB->m_nReadyToReceive = C_TO_MESA_UNSPECIFIED(0);
    pTTYFCB->m_nReadyToTransmit = C_TO_MESA_UNSPECIFIED(1);
}

AgentTTY::~AgentTTY()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentTTY public virtual functions
bool
AgentTTY::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;
    switch (MESA_TO_C_UNSPECIFIED(pFCB->m_nCommand))
    {
        case kTransmit:
            TRACE("%c", pFCB->m_data);
            pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
            break;
        case kReceive:
            if (MESA_TO_C_UNSPECIFIED(pFCB->m_nReadyToReceive))
            {
                pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
            }
            else
            {
                pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kNotReady);
            }
            break;
        default:
            pFCB->m_nStatus = C_TO_MESA_UNSPECIFIED(kSuccess);
            break;
    }
    return true;
}

bool
AgentTTY::Poll()
{
    return false;
}

u32
AgentTTY::GetFCBSize()
{
    return sizeof(FCB);
}

/////////////////////////////////////////////////////////////////////////////
// class AgentTTY private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
