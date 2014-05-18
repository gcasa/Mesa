// DawnView.cpp : implementation of class DawnView
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DawnApp.h"
#include "DawnView.h"
#include "../MesaAgent/MesaAgent.h"
#include <PrincOps/PrincOps.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DawnView MESSAGE_MAP
BEGIN_MESSAGE_MAP(DawnView,CWnd)
	//{{AFX_MSG_MAP(DawnView)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	ON_WM_SETFOCUS()
	ON_WM_WINDOWPOSCHANGING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DawnView construction
DawnView::DawnView()
{
    m_stringMessage = "Mesa Processor Idle.";
}

DawnView::~DawnView()
{
}

/////////////////////////////////////////////////////////////////////////////
// DawnView message handlers
BOOL DawnView::PreCreateWindow(CREATESTRUCT& cs) 
{
    cs.lpszClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

#ifdef DAWN_IN_FRAME_WINDOW
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
#else
	cs.dwExStyle = WS_EX_LEFT;
	cs.style = WS_POPUP;
#endif

	return TRUE;
}

void DawnView::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    CRect paintRect(dc.m_ps.rcPaint);

    if (PrincOps_IsRunning())
    {
        if (! MesaAgent::BlitDisplayRectToWindow(dc.GetSafeHdc(), &paintRect))
        {
            ::FillRect(dc.GetSafeHdc(), paintRect, (HBRUSH)::GetStockObject(GRAY_BRUSH));
        }
    }
    else
    {
        CRect rect;
        GetClientRect(&rect);
        CRect textRect(rect);

        ::FillRect(dc.GetSafeHdc(), paintRect, (HBRUSH)::GetStockObject(WHITE_BRUSH));

        dc.DrawText(m_stringMessage, textRect, DT_CALCRECT|DT_CENTER|DT_WORDBREAK);
        rect.top += (((rect.bottom - rect.top) / 2) - (textRect.Height() / 2));
        rect.bottom = rect.top + textRect.Height();
        dc.DrawText(m_stringMessage, rect, DT_CENTER|DT_WORDBREAK);
    }
}

void
DawnView::OnMouseMove(UINT nFlags, CPoint point) 
{
    CRect rect;
    GetClientRect(&rect);

         if (point.x < rect.left)    point.x = rect.left;
    else if (point.x >= rect.right)  point.x = rect.right-1;
         if (point.y < rect.top)     point.y = rect.top;
    else if (point.y >= rect.bottom) point.y = rect.bottom-1;

    MesaAgent::OnMouseMove((s16)point.x, (s16)point.y);
}

void
DawnView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_LBUTTON, true, (nFlags & (MK_MBUTTON | MK_RBUTTON)) ? true : false);
}

void
DawnView::OnLButtonUp(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_LBUTTON, false, (nFlags & (MK_MBUTTON | MK_RBUTTON)) ? true : false);
}

void
DawnView::OnMButtonDown(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_MBUTTON, true, (nFlags & (MK_LBUTTON | MK_RBUTTON)) ? true : false);
}

void
DawnView::OnMButtonUp(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_MBUTTON, false, (nFlags & (MK_LBUTTON | MK_RBUTTON)) ? true : false);
}

void
DawnView::OnRButtonDown(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_RBUTTON, true, (nFlags & (MK_LBUTTON | MK_MBUTTON)) ? true : false);
}

void
DawnView::OnRButtonUp(UINT nFlags, CPoint point) 
{
    MesaAgent::SetMouseButtonState(VK_RBUTTON, false, (nFlags & (MK_LBUTTON | MK_MBUTTON)) ? true : false);
}

void
DawnView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    MesaAgent::SetKeyState(nChar, nFlags, true);
}

void
DawnView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	MesaAgent::SetKeyState(nChar, nFlags, false);
}

void
DawnView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	MesaAgent::SetKeyState(nChar, nFlags, true);
}

void
DawnView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	MesaAgent::SetKeyState(nChar, nFlags, false);
}

void
DawnView::OnSetFocus(CWnd* pOldWnd) 
{
	MesaAgent::SetToggleKeyState();
}

void
DawnView::OnWindowPosChanging(WINDOWPOS * lpwndpos)
{
    int a;
    a = 1;
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created

