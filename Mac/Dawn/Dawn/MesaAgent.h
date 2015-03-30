// MesaAgent.h : header file for Win32 Microcode ops for Mesa Processor
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_MESAAGENT_H
#define _SOURCE_MESADLL_MESAAGENT_MESAAGENT_H

#include "PrincOps/PrincOps_PlatformTypes.h"

/////////////////////////////////////////////////////////////////////////////
// class MesaAgent
class MesaAgent
{
public:
    enum AgentIndex
    {
        kFirstAgent = 0,

        Null        = kFirstAgent,
        Disk        = 1,
        Floppy      = 2,
        Network     = 3,
        Parallel    = 4,
        Keyboard    = 5,
        Beep        = 6,
        Mouse       = 7,
        Processor   = 8,
        Stream      = 9,
        Serial      = 10,
        Tty         = 11,
        Display     = 12,
        Reserved3   = 13,
        Reserved2   = 14,
        Reserved1   = 15,

        kLastAgent = Reserved1,
        kNumAgents = kLastAgent + 1
    };

public:
    static bool Initialize(u16 * pDeviceMem, u16 * pDisplayMem);
    static bool Uninitialize();
    static u8   CallAgent(AgentIndex agentIndex);
    static u32  GetNumDevicePages();
    static void RaiseInterrupt(u16 nInterrupt);
    static void PollAgents();
    static u32  GetDisplayMemoryPageRequirement();
    static bool BlitDisplayRectToWindow(void * hDC, void * pRect);
    static void SetCursorMPCode(u16 nMPCode);
        // void *'s to stay away from windows types
    static void OnMouseMove(s16 x, s16 y);
    static void SetMouseButtonState(u32 nVKButton, bool bDown, bool bOtherMouseButtonsDown);
    static void SetKeyState(u32 nVKKey, u32 nFlags, bool bDown);
    static void SetToggleKeyState();

private:
    MesaAgent(u16 * pDeviceMem, u16 * pDisplayMem);
    ~MesaAgent();

public:

    class Agent
    {
    public:
        Agent(AgentIndex agentIndex, void * pFCB);
        virtual ~Agent();
    public:
        virtual bool Initiate();
        virtual bool Poll();
        virtual u32  GetFCBSize();
    public:
        static void RaiseInterrupt(u16 nInterrupt) { MesaAgent::RaiseInterrupt(nInterrupt); }
    protected:
        AgentIndex m_agentIndex;
        void * m_pFCB;
    };

private:
    Agent * m_agents[kNumAgents];
    u32     m_nNumDevicePages;

private:
    static MesaAgent * s_pThis;
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_MESAAGENT_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
//  18-Oct-03   dwoodward   added SetCursorMPCode()
