// AgentMouse.h : interface of class AgentMouse
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_MESADLL_MESAAGENT_AGENTMOUSE_H
#define _SOURCE_MESADLL_MESAAGENT_AGENTMOUSE_H

#include "MesaAgent.h"

/////////////////////////////////////////////////////////////////////////////
// class AgentMouse
class AgentMouse : public MesaAgent::Agent
{
public:
    AgentMouse(MesaAgent::AgentIndex agentIndex, void * pFCB);
    virtual ~AgentMouse();
public:
    virtual bool Initiate();
    virtual bool Poll();
    virtual u32  GetFCBSize();
public:
    void OnMouseMove(s16 x, s16 y);
private:

    enum
    {
        kMouseCommandSetPosition = 1,
        kMouseCommandSetCursorPosition = 2,
    };

#pragma pack (1)

    struct MesaPoint
    {
        s16 x;
        s16 y;
    };

    struct FCB
    {
	    MesaPoint	m_currentMousePosition;
	    MesaPoint	m_cursorOffset;
	    MesaPoint	m_set;
	    u16 	    m_nCommand;
    };

#pragma pack ()

private:
    void SetWindowsCursorPosition(s16 x, s16 y);
};

#endif // #ifndef _SOURCE_MESADLL_MESAAGENT_AGENTMOUSE_H

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
