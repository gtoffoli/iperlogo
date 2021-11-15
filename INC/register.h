#if !defined(AFX_REGISTER_H__107A17E2_A316_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_REGISTER_H__107A17E2_A316_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Register.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog

class CRegisterDialog : public CDialog
{
// Construction
public:
	CRegisterDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegisterDialog)
	enum { IDD = IDD_REGISTER };
	CString	m_registration;
	CString	m_company;
	CString	m_name;
	CString	m_serial;
	CString	m_status;
	int		m_access;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void GetRegistrationNames ();	// custom 980212

	// Generated message map functions
	//{{AFX_MSG(CRegisterDialog)
	afx_msg void OnRegister();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTER_H__107A17E2_A316_11D1_BE3E_444553540000__INCLUDED_)
