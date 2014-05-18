// AgentMouse.cpp : implementation of class AgentMouse
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AgentMouse.h"
#include "../MesaProcessor/MesaProcessor.h"
#include "../AppFramework/DawnApp.h"
#include "../AppFramework/DawnView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// class AgentMouse
AgentMouse::AgentMouse(MesaAgent::AgentIndex agentIndex, void * pFCB) : MesaAgent::Agent(agentIndex, pFCB)
{
    FCB * pMouseFCB = (FCB *)pFCB;
    memset(pMouseFCB, 0, sizeof(FCB));
}

AgentMouse::~AgentMouse()
{
}

/////////////////////////////////////////////////////////////////////////////
// class AgentMouse public virtual functions
bool
AgentMouse::Initiate()
{
    FCB * pFCB = (FCB *)m_pFCB;
    
    switch (MESA_TO_C_UNSPECIFIED(pFCB->m_nCommand))
    {
        case kMouseCommandSetPosition:
            if ((pFCB->m_set.x == pFCB->m_currentMousePosition.x) && (pFCB->m_set.y == pFCB->m_currentMousePosition.y)) break;
            pFCB->m_currentMousePosition = pFCB->m_set;
            SetWindowsCursorPosition(
                MESA_TO_C_UNSPECIFIED(pFCB->m_set.x) + MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.x),
                MESA_TO_C_UNSPECIFIED(pFCB->m_set.y) + MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.y));
            break;
        case kMouseCommandSetCursorPosition:
            SetWindowsCursorPosition(
                MESA_TO_C_UNSPECIFIED(pFCB->m_currentMousePosition.x) + MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.x),
                MESA_TO_C_UNSPECIFIED(pFCB->m_currentMousePosition.y) + MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.y));
            break;
        default:
            break;
    }

    return true;
}

void
AgentMouse::SetWindowsCursorPosition(s16 x, s16 y)
{
    // only set the current mouse position is over the client window
    CRect clientRect;
    CPoint cursorPoint;
    DawnView * pView = DawnApp::GetDawnView();

    pView->GetClientRect(&clientRect);
    ::GetCursorPos(&cursorPoint);
    if (pView != pView->WindowFromPoint(cursorPoint)) return;

         if (x < clientRect.left) x = (s16)clientRect.left;
    else if (x >= clientRect.right) x = (s16)clientRect.right - 1;
         if (y < clientRect.top) y = (s16)clientRect.top;
    else if (y >= clientRect.bottom) y = (s16)clientRect.bottom - 1;

    cursorPoint.x = x;
    cursorPoint.y = y;
    pView->ClientToScreen(&cursorPoint);
    ::SetCursorPos(cursorPoint.x, cursorPoint.y);
}

bool
AgentMouse::Poll()
{
    return false;
}

u32
AgentMouse::GetFCBSize()
{
    return sizeof(FCB);
}

void
AgentMouse::OnMouseMove(s16 x, s16 y)
{
    FCB * pFCB = (FCB *)m_pFCB;
    x -= MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.x);
    y -= MESA_TO_C_UNSPECIFIED(pFCB->m_cursorOffset.y);
    pFCB->m_currentMousePosition.x = C_TO_MESA_UNSPECIFIED(x);
    pFCB->m_currentMousePosition.y = C_TO_MESA_UNSPECIFIED(y);
}



/////////////////////////////////////////////////////////////////////////////
// class AgentMouse private functions

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	11-Aug-01	dwoodward	created
