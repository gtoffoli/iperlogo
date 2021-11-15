// selprocd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelprocDialog dialog

class CSelprocDialog : public CDialog
{
// Construction
public:
	CSelprocDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSelprocDialog)
	enum { IDD = IDD_SELPROC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

#define MAX_CHARS 128
	char m_text[MAX_CHARS];		// added

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSelprocDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
