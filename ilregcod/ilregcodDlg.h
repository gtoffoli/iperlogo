// ilregcodDlg.h : header file
//

#if !defined(AFX_ILREGCODDLG_H__29C48AA8_A307_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_ILREGCODDLG_H__29C48AA8_A307_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CIlregcodDlg dialog

class CIlregcodDlg : public CDialog
{
// Construction
public:
	CIlregcodDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CIlregcodDlg)
	enum { IDD = IDD_ILREGCOD_DIALOG };
	int		m_access;
	CString	m_serial;
	CString	m_code;
	CString	m_status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIlregcodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIlregcodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILREGCODDLG_H__29C48AA8_A307_11D1_BE3E_444553540000__INCLUDED_)
