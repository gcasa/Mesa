// DawnFrame.h : interface of class DawnFrame
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNFRAME_H_
#define _SOURCE_DAWN_APPFRAMEWORK_DAWNFRAME_H_

#include "DawnView.h"

/////////////////////////////////////////////////////////////////////////////
// class DawnFrame
class DawnFrame : public CFrameWnd
{
	
public:
	DawnFrame();
protected: 
	DECLARE_DYNAMIC(DawnFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DawnFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
    virtual void OnUpdateMPCode(CCmdUI* pCmdUI);
	afx_msg void OnAppStartDawn();
	afx_msg void OnUpdateAppStartDawn(CCmdUI* pCmdUI);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DawnFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
friend class DawnApp;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	DawnView    m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(DawnFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnClose();
    afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnKillDawnMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    HICON m_hIcon;
    unsigned int m_nMPCode;
    CString m_stringMPCode;
    bool m_bPrincOpsRunning;
protected:
    void SetViewSize(CSize size);
public:
    void SetMPCode(unsigned int mpCode);
    void SetMessageString(const CString & messageString) { m_wndView.SetMessageString(messageString); }
};

#endif  // #ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNFRAME_H_

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created
