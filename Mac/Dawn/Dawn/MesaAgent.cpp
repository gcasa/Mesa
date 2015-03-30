// MesaAgent.cpp : implements MesaProcessor Agent interaction
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MesaAgent.h"
#include "AgentBeep.h"
#include "AgentDisk.h"
#include "AgentDisplay.h"
#include "AgentFloppy.h"
#include "AgentKeyboard.h"
#include "AgentMouse.h"
#include "AgentNetwork.h"
#include "AgentParallel.h"
#include "AgentProcessor.h"
#include "AgentSerial.h"
#include "AgentStream.h"
#include "AgentTTY.h"
#include "../MesaProcessor/MesaProcessor.h"
#include "../MesaProcessor/MesaMicrocodeOps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class MesaAgent static members
MesaAgent * MesaAgent::s_pThis = NULL;

/////////////////////////////////////////////////////////////////////////////
// class MesaAgent public functions
bool
MesaAgent::Initialize(u16 * pDeviceMem, u16 * pDisplayMem)
{
    if (s_pThis == NULL) s_pThis = new MesaAgent(pDeviceMem, pDisplayMem);
    if (s_pThis == NULL) return false;

    for (u32 index = kFirstAgent; index < kNumAgents; index++)
    {
        if (s_pThis->m_agents[index] == NULL)
        {
            delete s_pThis;
            s_pThis = NULL;
            return false;
        }
    }

    return true;
}

bool
MesaAgent::Uninitialize()
{
    if (s_pThis != NULL)
    {
        delete s_pThis;
        s_pThis = NULL;
    }
    return true;
}

u8
MesaAgent::CallAgent(AgentIndex agentIndex)
{
    u32 index = (u32)agentIndex;
#if 0
    const char * pAgentName = NULL;
    switch (index)
    {
        case Null:
            pAgentName = "Null";
            break;
        case Disk:
            //pAgentName = "Disk";
            break;
        case Floppy:
            pAgentName = "Floppy";
            break;
        case Network:
            //pAgentName = "Network";
            break;
        case Parallel:
            pAgentName = "Parallel";
            break;
        case Keyboard:
            pAgentName = "Keyboard";
            break;
        case Beep:
            pAgentName = "Beep";
            break;
        case Mouse:
            pAgentName = "Mouse";
            break;
        case Processor:
            //pAgentName = "Processor";
            break;
        case Stream:
            pAgentName = "Stream";
            break;
        case Serial:
            pAgentName = "Serial";
            break;
        case Tty:
            //pAgentName = "Tty";
            break;
        case Display:
            pAgentName = "Display";
            break;
        case Reserved3:
            pAgentName = "Reserved3";
            break;
        case Reserved2:
            pAgentName = "Reserved2";
            break;
        case Reserved1:
            pAgentName = "Reserved1";
            break;
        default:
            pAgentName = "Unknown";
            break;
    }
    if (pAgentName) TRACE("Agent Call: %s\n", pAgentName);
#endif
    return (true == s_pThis->m_agents[index]->Initiate() ? 0 : 1);
}

u32
MesaAgent::GetNumDevicePages()
{
    return (s_pThis ? s_pThis->m_nNumDevicePages : 0);
}

void
MesaAgent::RaiseInterrupt(u16 nInterrupt)
{
    MesaMicrocodeOps::RaiseInterrupt(nInterrupt);
}

void
MesaAgent::PollAgents()
{
    for (u32 index = kFirstAgent; index < kNumAgents; index++)
    {
        s_pThis->m_agents[index]->Poll();
    }
}

u32
MesaAgent::GetDisplayMemoryPageRequirement()
{
    return AgentDisplay::GetDisplayMemoryPageRequirement();
}

bool
MesaAgent::BlitDisplayRectToWindow(void * hDC, void * pRect)
{
    return (s_pThis ? ((AgentDisplay *)(s_pThis->m_agents[Display]))->BlitDisplayRectToWindow((HDC)hDC, (const CRect *)pRect) : false);
}

void
MesaAgent::SetCursorMPCode(u16 nMPCode)
{
    if (s_pThis) ((AgentDisplay *)(s_pThis->m_agents[Display]))->SetCursorMPCode(nMPCode);
}

void
MesaAgent::OnMouseMove(s16 x, s16 y)
{
    if (s_pThis) ((AgentMouse *)(s_pThis->m_agents[Mouse]))->OnMouseMove(x, y);
}

void
MesaAgent::SetMouseButtonState(u32 nVKButton, bool bDown, bool bOtherMouseButtonsDown)
{
    if (s_pThis) ((AgentKeyboard *)(s_pThis->m_agents[Keyboard]))->SetMouseButtonState(nVKButton, bDown, bOtherMouseButtonsDown);
}

void
MesaAgent::SetKeyState(u32 nVKKey, u32 nFlags, bool bDown)
{
    if (s_pThis) ((AgentKeyboard *)(s_pThis->m_agents[Keyboard]))->SetKeyState(nVKKey, nFlags, bDown);
}

void
MesaAgent::SetToggleKeyState()
{
    if (s_pThis) ((AgentKeyboard *)(s_pThis->m_agents[Keyboard]))->SetToggleKeyState();
}


/////////////////////////////////////////////////////////////////////////////
// class MesaAgent construction
MesaAgent::MesaAgent(u16 * pDeviceMemory, u16 * pDisplayMemory)
{
    // need to setup the pointers to the fcb's
    // there are kNumAgents Virtual Memory addresses starting
    // at Virtual address of the device io area.
    // The device io area is always mapped (I think).

    u32 * pAgentFCBPointers = (u32 *)pDeviceMemory;
    u32 * pFCB = pAgentFCBPointers + kNumAgents;
    u32   nFCBVirtualAddress = (sizeof(u32) * kNumAgents) / 2; // page (0) based virtual address in words   
    u32   nSizeFCB = 0;

    u32 index = 0;
    for (index = kFirstAgent; index < kNumAgents; index++)
    {
        switch (index)
        {
            case Null:
                m_agents[index] = new Agent((AgentIndex)index, pFCB);
                break;
            case Disk:
                m_agents[index] = new AgentDisk((AgentIndex)index, pFCB);
                break;
            case Floppy:
                m_agents[index] = new AgentFloppy((AgentIndex)index, pFCB);
                break;
            case Network:  
                m_agents[index] = new AgentNetwork((AgentIndex)index, pFCB);
                break;
            case Parallel: 
                m_agents[index] = new AgentParallel((AgentIndex)index, pFCB);
                break;
            case Keyboard: 
                m_agents[index] = new AgentKeyboard((AgentIndex)index, pFCB);
                break;
            case Beep:     
                m_agents[index] = new AgentBeep((AgentIndex)index, pFCB);
                break;
            case Mouse:    
                m_agents[index] = new AgentMouse((AgentIndex)index, pFCB);
                break;
            case Processor:
                m_agents[index] = new AgentProcessor((AgentIndex)index, pFCB);
                break;
            case Stream:   
                m_agents[index] = new AgentStream((AgentIndex)index, pFCB);
                break;
            case Serial:   
                m_agents[index] = new AgentSerial((AgentIndex)index, pFCB);
                break;
            case Tty:      
                m_agents[index] = new AgentTTY((AgentIndex)index, pFCB);
                break;
            case Display:  
                m_agents[index] = new AgentDisplay((AgentIndex)index, pFCB, pDisplayMemory);
                break;
            case Reserved3:
                m_agents[index] = new Agent((AgentIndex)index, pFCB);
                break;
            case Reserved2:
                m_agents[index] = new Agent((AgentIndex)index, pFCB);
                break;
            case Reserved1:
                m_agents[index] = new Agent((AgentIndex)index, pFCB);
                break;
            default:
                ASSERT(0);
                break;
        }
        if (NULL == m_agents[index]) return;    // error!  this will be caught in MesaAgent::Initialize

        nSizeFCB = m_agents[index]->GetFCBSize();               // size in bytes
        nSizeFCB = ((nSizeFCB / 4) + ((nSizeFCB & 3) ? 1 : 0)); // convert to 32bit words words
        pFCB += nSizeFCB;                                       // increment our pointer
        
        *pAgentFCBPointers = (nSizeFCB == 0 ? 0 : nFCBVirtualAddress);
        pAgentFCBPointers++;

        nFCBVirtualAddress += (nSizeFCB * 2);                   // add num 16 bit words to the virtual address
    }

    // now that we've created all of the agents, we know the total size of all memory
    // used by the table of addresses to fcb's + all of the fcb's - it's in nFCBVirtualAddress
    m_nNumDevicePages = (nFCBVirtualAddress / MesaProcessor::kWordsPerPage) + (((nFCBVirtualAddress % MesaProcessor::kWordsPerPage)) == 0 ? 0 : 1);
    u32 nFirstDevicePage = 256 - m_nNumDevicePages;

    // now that we know the first device page we can spin through the pFCB table and put in the
    // correct mesa virtual addresses
    u32 nFirstDevicePageAddress = nFirstDevicePage * MesaProcessor::kWordsPerPage;
    pAgentFCBPointers = (u32 *)pDeviceMemory;
    for (index = kFirstAgent; index < kNumAgents; index++)
    {
        if (0 != *pAgentFCBPointers)
        {
            *pAgentFCBPointers = C_TO_MESA_LONG_UNSPECIFIED((nFirstDevicePageAddress + *pAgentFCBPointers));
        }
        pAgentFCBPointers++;
    }
}

MesaAgent::~MesaAgent()
{
    for (u32 index = kFirstAgent; index < kNumAgents; index++)
    {
        if (m_agents[index] != NULL)
        {
            delete m_agents[index];
            m_agents[index] = NULL;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// class MesaAgent::Agent
MesaAgent::Agent::Agent(AgentIndex agentIndex, void * pFCB) : m_agentIndex(agentIndex), m_pFCB(pFCB)
{
}

MesaAgent::Agent::~Agent()
{
}

bool
MesaAgent::Agent::Initiate()
{
    return true;
}

bool
MesaAgent::Agent::Poll()
{
    return false;
}

u32
MesaAgent::Agent::GetFCBSize()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
//  18-Oct-03   dwoodward   added SetCursorMPCode()
