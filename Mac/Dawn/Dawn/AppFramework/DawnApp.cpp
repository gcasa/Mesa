// DawnApp.cpp : implementation of class DawnApp
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DawnApp.h"
#include "DawnAboutBox.h"
#include "DawnFrame.h"
#include "XNSDriverClient.h"
#include "../MesaProcessor/MesaProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DawnApp MESSAGE_MAP
BEGIN_MESSAGE_MAP(DawnApp, CWinApp)
	//{{AFX_MSG_MAP(DawnApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_BYTE_SWAP_VIRTUAL_DISK_FILE, OnAppByteSwapVirtualDiskFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DawnApp construction
DawnApp::DawnApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only DawnApp object
DawnApp theApp;

/////////////////////////////////////////////////////////////////////////////
// DawnApp initialization
BOOL DawnApp::InitInstance()
{
	// AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("woodward.org"));

    // Open the XNS Driver
    XNSDriverClient::GetXNSDriverClient().OpenDriver();

#ifdef DAWN_IN_FRAME_WINDOW
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.
	DawnFrame * pFrame = new DawnFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
    
    // The one and only window has been initialized, so show and update it.
    pFrame->CenterWindow();
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();

	return TRUE;
 #else
    CRect rect;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
    rect.right &= ~31;
    //rect.right /= 8; rect.right *= 8;
    m_pMainWnd = new DawnView;
    m_pMainWnd->CreateEx(
        WS_EX_LEFT, NULL, NULL, WS_POPUP, rect, NULL, 0);
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    MesaProcessor::Initialize();
    MesaProcessor::Run();
    return TRUE;
#endif
}

DawnFrame *
DawnApp::GetDawnFrame()
{
    return (DawnFrame *)theApp.m_pMainWnd;
}

DawnView *
DawnApp::GetDawnView()
{
#ifdef DAWN_IN_FRAME_WINDOW
    return (theApp.m_pMainWnd ? &((DawnFrame*)theApp.m_pMainWnd)->m_wndView : NULL);
#else
    return (theApp.m_pMainWnd ? (DawnView *)theApp.m_pMainWnd : NULL);
#endif
}

int
DawnApp::ExitInstance() 
{
	// Close the XNS Driver
    XNSDriverClient::GetXNSDriverClient().CloseDriver();
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// DawnApp message handlers
void DawnApp::OnAppAbout()
{
	DawnAboutBox aboutBox;
	aboutBox.DoModal();
}

void DawnApp::OnAppByteSwapVirtualDiskFile()
{
    // return right away; make this function a noop for now
    return;

    HANDLE hFileIn = INVALID_HANDLE_VALUE;
    HANDLE hFileOut = INVALID_HANDLE_VALUE;
    DWORD nFileSize = 0;
    u8 * pData = NULL;
    DWORD nBytesReadWrite = 0;

    bool bSuccess = false;
    do
    {
        hFileIn = ::CreateFile(
                    "c:\\CoPilot.dsk",
                    GENERIC_READ,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
        
        if (hFileIn == INVALID_HANDLE_VALUE) break;
        
        nFileSize = ::GetFileSize(hFileIn, NULL);
        if (nFileSize == -1) break;
        if ((nFileSize == 0) || (nFileSize & 1)) break;

        pData = new u8 [nFileSize];
        if (NULL == pData) break;

        if ((0 == ::ReadFile(hFileIn, pData, nFileSize, &nBytesReadWrite, NULL)) || (nFileSize != nBytesReadWrite)) break;

        for (u32 i = 0; i < (nFileSize / 2); i++)
        {
            u8 data = pData[i];
            pData[i] = pData[i+1];
            pData[i+1] = data;
        }

        hFileOut = ::CreateFile(
                    "c:\\CoPilotSwapped.dsk",
                    GENERIC_WRITE,
                    0,
                    NULL,
                    CREATE_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);

        if (hFileOut == INVALID_HANDLE_VALUE) break;

        if ((0 == ::WriteFile(hFileOut, pData, nFileSize, &nBytesReadWrite, NULL)) || (nFileSize != nBytesReadWrite)) break;

        bSuccess = true;
    }
    while (false);

    if (INVALID_HANDLE_VALUE != hFileIn) ::CloseHandle(hFileIn);
    if (INVALID_HANDLE_VALUE != hFileOut) ::CloseHandle(hFileOut);
    if (NULL != pData) delete [] pData;

    AfxMessageBox(bSuccess ? "Success!" : "Failed.");
}

////////////////////////////////////////////////////////////////////////////////
// DEBUG CODE
extern "C" void DebugPrint(const char * pString)
{
    TRACE("%s", pString);
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created
