// welcomed.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWelcomeDialog dialog

class CWelcomeDialog : public CDialog
{
// Construction
public:
	CWelcomeDialog(CWnd* pParent = NULL);	// standard constructor
	virtual	void OnInitialUpdate();			// added

// Dialog Data
	//{{AFX_DATA(CWelcomeDialog)
	enum { IDD = IDD_WELCOME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	int m_counter;
	CPalette* m_pPalette;
	CPalette* m_pOldPalette;
	CBitmap* m_pBitmap;

	// Generated message map functions
	//{{AFX_MSG(CWelcomeDialog)
	afx_msg void OnPaint();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
