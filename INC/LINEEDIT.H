// lineedit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineEdit dialog

class CLineEdit : public CDialog
{
// Construction
public:
	CLineEdit(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLineEdit)
	enum { IDD = IDD_LINE_DIALOG };
	CString	m_text;
	//}}AFX_DATA
	
	char *pstring;			//// pointer to char array (custom extension) 
	int maxchars;			//// max no. of chars to be transferred (custom extension) 
	void TransferText ();	//// transfer text to char array (custom extension)

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CLineEdit)
	virtual void OnOK();
	//}}AFX_MSG
	
	BOOL OnInitDialog();		//// ovveride default
	
	DECLARE_MESSAGE_MAP()
};
