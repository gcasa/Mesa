// DawnFrame.cpp : implementation of class DawnFrame
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DawnApp.h"
#include "DawnFrame.h"
#include <PrincOps/PrincOps.h>
#include "../MesaProcessor/MesaProcessor.h"
#include "../MesaAgent/MesaAgent.h"
#include <afxpriv.h>

#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler
#define WM_KILL_DAWN    (WM_USER + 4)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DawnFrame MESSAGE_MAP
IMPLEMENT_DYNAMIC(DawnFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(DawnFrame, CFrameWnd)
	//{{AFX_MSG_MAP(DawnFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MP_CODE, OnUpdateMPCode)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_KILL_DAWN, OnKillDawnMessage)
	ON_COMMAND(ID_APP_START_DAWN, OnAppStartDawn)
	ON_UPDATE_COMMAND_UI(ID_APP_START_DAWN, OnUpdateAppStartDawn)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DawnFrame MESSAGE_MAP static global variables
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
#if 0
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
#endif
	ID_INDICATOR_MP_CODE,
};

/////////////////////////////////////////////////////////////////////////////
// DawnFrame construction/destruction
DawnFrame::DawnFrame()
{
    m_hIcon = ::LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
    m_nMPCode = 0;
    m_stringMPCode = " 0000 ";
    m_bPrincOpsRunning = false;
}

DawnFrame::~DawnFrame()
{
    if (m_hIcon) ::DestroyIcon(m_hIcon);
}

int DawnFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

    // make the view the desired size
    #if 0
        // this code makes it as big as possible
        CRect rect;
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
        rect.right -= 40; rect.bottom -= 100;
        SetViewSize(CSize(rect.right, rect.bottom));
    #else
        // use a fixed size
        //SetViewSize(CSize(750, 450)); // good for 800 x 600
        //SetViewSize(CSize(896, 552)); // good for 1024 x 768
        //SetViewSize(CSize(1376, 900)); // good for 1280 x 1024
        SetViewSize(CSize(896, 552));
    #endif

	return 0;
}

void
DawnFrame::SetViewSize(CSize size)
{
    // sanity check the view size to make sure that it is not bigger than the screen
    // and to make sure the width is a whole multiple of 32 pixels
#if 1
    CSize screenSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
    screenSize.cx &= ~31;
    size.cx &= ~31;

    if (size.cx > screenSize.cx) size.cx = screenSize.cx;
    if (size.cy > screenSize.cy) size.cy = screenSize.cy;
#endif

    // make the view the desired size
    CRect rectView;
    CRect rectFrame;
    RecalcLayout();
    m_wndView.GetClientRect(&rectView);
    GetWindowRect(&rectFrame);
    int cx = size.cx - rectView.Width();
    int cy = size.cy - rectView.Height();
    rectFrame.bottom = rectFrame.top + rectFrame.Height() + cy;
    rectFrame.right = rectFrame.left + rectFrame.Width() + cx;
    SetWindowPos(NULL, 0, 0, rectFrame.Width(), rectFrame.Height(),
        SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}

BOOL DawnFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~(WS_MAXIMIZEBOX|WS_THICKFRAME);
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW), 0, m_hIcon);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// DawnFrame diagnostics
#ifdef _DEBUG
void DawnFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void DawnFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DawnFrame message handlers
void DawnFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL DawnFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void DawnFrame::SetMPCode(unsigned int mpCode)
{
    if (m_nMPCode != mpCode)
    {
        m_nMPCode = mpCode;
        MesaAgent::SetCursorMPCode(mpCode);
        m_stringMPCode.Format(" %04d ", mpCode);
        m_wndStatusBar.SendMessage(WM_IDLEUPDATECMDUI, 1);
    }
}

void DawnFrame::OnUpdateMPCode(CCmdUI* pCmdUI)
{   
    pCmdUI->Enable(TRUE);
    pCmdUI->SetText(m_stringMPCode);
}

void DawnFrame::OnClose() 
{
	if (m_bPrincOpsRunning)
    {
        AfxMessageBox("Stop the Mesa Processor before quitting.", MB_OK|MB_ICONSTOP);
    }
    else CFrameWnd::OnClose();
}

LRESULT DawnFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
    if ((wParam == AFX_IDS_IDLEMESSAGE) && m_bPrincOpsRunning)
    {
        wParam = IDS_MESA_PROCESSOR_RUNNING;
    }
    return CFrameWnd::OnSetMessageString(wParam, lParam);
}

LRESULT DawnFrame::OnKillDawnMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_bPrincOpsRunning)
	{
        m_bPrincOpsRunning = false;
        SetMessageText(AFX_IDS_IDLEMESSAGE);
        PrincOps_Halt();
	}
    return 0;
}

void DawnFrame::OnAppStartDawn()
{
    if (m_bPrincOpsRunning)
    {
        if (IDYES == AfxMessageBox("Really pull the plug on the Mesa Machine?", MB_YESNO|MB_ICONSTOP))
        {
            //m_bPrincOpsRunning = false;
            //SetMessageText(AFX_IDS_IDLEMESSAGE);
            //PrincOps_Halt();
            PostMessage(WM_KILL_DAWN);
        }
        else
        {
            m_wndToolBar.SendMessage(WM_IDLEUPDATECMDUI);
        }
    }
    else
    {
        m_bPrincOpsRunning = true;
        SetMessageText(IDS_MESA_PROCESSOR_RUNNING);
        MesaProcessor::Initialize();
        MesaProcessor::Run();
        m_bPrincOpsRunning = false;
        SetMessageText(AFX_IDS_IDLEMESSAGE);
    }
}

void DawnFrame::OnUpdateAppStartDawn(CCmdUI* pCmdUI) 
{
    pCmdUI->SetCheck(m_bPrincOpsRunning ? 1 : 0);	
}

extern "C" void RepaintDawnView()
{
    DawnView * pView = DawnApp::GetDawnView();
    pView->InvalidateRect(NULL);
    pView->UpdateWindow();
}


////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created

