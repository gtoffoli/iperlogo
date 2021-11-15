// mainstat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainStatusDialog dialog

class CMainStatusDialog : public CDialog
{
// Construction
public:
	CMainStatusDialog(CWnd* pParent = NULL);	// standard constructor
//<aggiunto ...
	BOOL CreateDir (UINT templateID, CWnd* pParentWnd = NULL ) {
	   return CDialog::Create(templateID, pParentWnd);
	}
//> ... fin qui  

// Dialog Data
	//{{AFX_DATA(CMainStatusDialog)
	enum { IDD = IDD_MAIN_STATUS };
	long	m_nodes_free;
	long	m_nodes_total;
	long	m_nodes_used;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMainStatusDialog)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
