// editorfr.h : header file
//

#include "stdafx.h"		// added
#include "afxdlgs.h"	// added

/////////////////////////////////////////////////////////////////////////////
// CEditorFrame frame

class CEditorFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CEditorFrame)
protected:
	CEditorFrame();			// protected constructor used by dynamic creation

// Attributes
public:
	HMENU m_hMenu;					// 981123

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void SetTitleBar (BOOL setOn);	// 981123
	void SetMenuBar (BOOL setOn);	// 981123

// Implementation
protected:
	virtual ~CEditorFrame();

// New
public:
	CWnd* m_view;			// associated view window (added)
	char m_title [64];		// (added)

	// Generated message map functions
	//{{AFX_MSG(CEditorFrame)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg LRESULT OnFindReplace(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
