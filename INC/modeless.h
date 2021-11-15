#if !defined(AFX_MODELESS_H__43F99D02_E44A_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_MODELESS_H__43F99D02_E44A_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Modeless.h : header file
//
/* MODELESS.H

991008	aggiunto OnUpdateControl
991007	pulizia
*/

/////////////////////////////////////////////////////////////////////////////
// CModelessDialog dialog

class CModelessDialog : public CDialog
{
// Construction
public:
	CModelessDialog(CWnd* pParent = NULL);   // standard constructor

	void DialogCancel();	// aggancio public a CDialog::OnCancel(), che è protected

// Dialog Data
	//{{AFX_DATA(CModelessDialog)
	enum { IDD = IDD_MODELESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	BOOL m_bCanceled;	// 991004: per simulare dialogo modale

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelessDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelessDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnUpdateControl(CCmdUI *pCmdUI);	// 991008
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELESS_H__43F99D02_E44A_11D1_BE3E_444553540000__INCLUDED_)
