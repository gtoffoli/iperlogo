// kbdialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKbDialog dialog

class CKbDialog : public CDialog
{
// Construction
public:
	CKbDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKbDialog)
	enum { IDD = IDD_KEYBOARD };
	CString	m_text;
	//}}AFX_DATA

	unsigned char *pBuffer;			//// pointer to char array (custom extension) 
	int nChars;			//// max no. of chars to be transferred (custom extension) 
	int TransferText ();	//// transfer text to char array (custom extension)

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CKbDialog)
	afx_msg void OnChangeEdit1();
	afx_msg void OnUpdateEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
