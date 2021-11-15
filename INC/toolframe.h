#if !defined(AFX_TOOLFRAME_H__A4537BA1_CD09_11D2_9A8E_E85A03C10E27__INCLUDED_)
#define AFX_TOOLFRAME_H__A4537BA1_CD09_11D2_9A8E_E85A03C10E27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolFrame frame

/* TOOLFRAME.H

	990621	Message Mapping for Continuous Range of Command IDs
			(da www.formatics.com/AmpCh1.htm)
*/

class CToolFrame : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CToolFrame)
/// protected:
public:
	CToolFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar* m_toolBar;	// 990621
	UINT m_uFirstBtn;		// 990621: inizio range ID
	UINT m_uLastBtn;		// 990621: fine range ID
	UINT m_uCurrentBtn;		// 990621: ID bottone premuto
	int m_parentHandle;		// 990623: handle di finestra Tarta

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolFrame)
	//}}AFX_VIRTUAL

// Implementation
///protected:
public:
	virtual ~CToolFrame();

	// Generated message map functions
	//{{AFX_MSG(CToolFrame)
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnButton(UINT uID);				// 990621
	afx_msg void OnButtonClicked(UINT uID);			// 990621
	afx_msg void OnUpdateButton(CCmdUI *pCmdUI);	// 990621
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLFRAME_H__A4537BA1_CD09_11D2_9A8E_E85A03C10E27__INCLUDED_)
