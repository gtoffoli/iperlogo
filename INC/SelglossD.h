#if !defined(AFX_SELGLOSSD_H__6E0A76A4_E8E5_11D2_86F0_00C0DFA89376__INCLUDED_)
#define AFX_SELGLOSSD_H__6E0A76A4_E8E5_11D2_86F0_00C0DFA89376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelglossD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelglossDialog dialog

class CSelglossDialog : public CDialog
{
// Construction
public:
	CSelglossDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelglossDialog)
	enum { IDD = IDD_SELGLOSS };
	CString	m_textIni;
	BOOL	m_iperlogo;
	BOOL	m_noalias;
	BOOL	m_superlogo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelglossDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelglossDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAll();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELGLOSSD_H__6E0A76A4_E8E5_11D2_86F0_00C0DFA89376__INCLUDED_)
