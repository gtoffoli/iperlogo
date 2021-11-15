// modal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModaDialog dialog

class CModaDialog : public CDialog
{
// Construction
public:
	CModaDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CModaDialog)
	enum { IDD = IDD_MODAL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CModaDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
