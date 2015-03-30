// DawnAboutBox.cpp : implementation of class DawnAboutBox
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DawnAboutBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DawnAboutBox
BEGIN_MESSAGE_MAP(DawnAboutBox, CDialog)
	//{{AFX_MSG_MAP(DawnAboutBox)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DawnAboutBox construction
DawnAboutBox::DawnAboutBox() : CDialog(DawnAboutBox::IDD)
{
	//{{AFX_DATA_INIT(DawnAboutBox)
	//}}AFX_DATA_INIT
}

void DawnAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DawnAboutBox)
	//}}AFX_DATA_MAP
}

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	27-Jul-01	dwoodward	created
