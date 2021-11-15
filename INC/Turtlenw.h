// turtlenw.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTurtleNew dialog

class CTurtleNew : public CDialog
{
// Construction
public:
	CTurtleNew(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTurtleNew)
	enum { IDD = IDD_TURTLE_NEW };
	UINT	m_winPosX;
	UINT	m_winPosY;
	UINT	m_winDimX;
	UINT	m_winDimY;
	CString	m_winName;
	UINT	m_viewDimX;
	UINT	m_viewDimY;
	double	m_viewRight;
	double	m_viewLeft;
	double	m_viewTop;
	double	m_viewBottom;
	BOOL	m_modeOVs;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CTurtleNew)
	afx_msg void OnReset();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_turtleCounter;	// added
	void Reset();			// added
};
