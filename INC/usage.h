#if !defined(AFX_USAGE_H__834A6562_A468_11D1_BE3E_444553540000__INCLUDED_)
#define AFX_USAGE_H__834A6562_A468_11D1_BE3E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Usage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsage dialog

class CUsage : public CDialog
{
// Construction
public:
	CUsage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUsage)
	enum { IDD = IDD_USAGE };
	int		m_times;
	CString	m_last;
	CString	m_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USAGE_H__834A6562_A468_11D1_BE3E_444553540000__INCLUDED_)
