// DawnAboutBox.h : interface of class DawnAboutBox
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNABOUTBOX_H_
#define _SOURCE_DAWN_APPFRAMEWORK_DAWNABOUTBOX_H_

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// class DawnAboutBox
class DawnAboutBox : public CDialog
{
public:
	DawnAboutBox();

// Dialog Data
	//{{AFX_DATA(DawnAboutBox)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DawnAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(DawnAboutBox)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif  // #ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNABOUTBOX_H_

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created
