// DawnView.h : interface of class DawnView
//
// Copyright (c) 2001 Don Woodward.  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNVIEW_H_
#define _SOURCE_DAWN_APPFRAMEWORK_DAWNVIEW_H_

/////////////////////////////////////////////////////////////////////////////
// class DawnView
class DawnView : public CWnd
{
// Construction
public:
	DawnView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DawnView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DawnView();

	// Generated message map functions
protected:
	//{{AFX_MSG(DawnView)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnWindowPosChanging(WINDOWPOS * lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CString m_stringMessage;
public:
    void SetMessageString(const CString & messageString) { m_stringMessage = messageString; InvalidateRect(NULL); UpdateWindow(); }
};

#endif  // #ifndef _SOURCE_DAWN_APPFRAMEWORK_DAWNVIEW_H_

////////////////////////////////////////////////////////////////////////////////
//	LOG 
////////////////////////////////////////////////////////////////////////////////
//	09-Sep-01	dwoodward	created
