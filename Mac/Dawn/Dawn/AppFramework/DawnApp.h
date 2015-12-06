// DawnApp.h : interface of class DawnApp
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNAPP_H_
#define _SOURCE_DAWN_APPFRAMEWORK_DAWNAPP_H_

// #include "resource.h"

#define DAWN_IN_FRAME_WINDOW

/////////////////////////////////////////////////////////////////////////////
// class DawnApp
class DawnApp : public CWinApp
{
public:
	DawnApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DawnApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(DawnApp)
	afx_msg void OnAppAbout();
    afx_msg void OnAppByteSwapVirtualDiskFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    static class DawnFrame * GetDawnFrame();
    static class DawnView * GetDawnView();
};

#endif  // #ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNAPP_H_

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created
