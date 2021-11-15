#if !defined(AFX_WEBFRAME_H__B29EC441_8C99_11D2_9A8E_E48102C10E27__INCLUDED_)
#define AFX_WEBFRAME_H__B29EC441_8C99_11D2_9A8E_E48102C10E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// webframe.h : header file
//
/* WEBFRAME.H

990914  Override di OnWndMsg per soluzione problemi di focus
*/

/////////////////////////////////////////////////////////////////////////////
// Cwebframe frame

class CWebFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CWebFrame)
protected:
	CWebFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	HMENU m_hMenu;					// +
	CWnd* m_statusBar;
	CWnd* m_dialogBar;

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void SetTitleBar (BOOL setOn);	// +
	void SetMenuBar (BOOL setOn);	// +
// Overrides

	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CWebFrame();

public:
	CWnd* m_view;			// +
	char m_title [64];		// +
	BOOL m_shownDialogBar;	// +
	BOOL m_shownStatusBar;	// +

	// Generated message map functions
	//{{AFX_MSG(CWebFrame)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnGo();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBFRAME_H__B29EC441_8C99_11D2_9A8E_E48102C10E27__INCLUDED_)
