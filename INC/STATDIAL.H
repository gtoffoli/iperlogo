// statdial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog

class CStatusDialog : public CDialog
{
// Construction
public:
	CStatusDialog(CWnd* pParent = NULL);	// standard constructor
//<aggiunto ...
	BOOL CreateDir (UINT templateID, CWnd* pParentWnd = NULL ) {
	   return CDialog::Create(templateID, pParentWnd);
	}
//> ... fin qui  

// Dialog Data
	//{{AFX_DATA(CStatusDialog)
	enum { IDD = IDD_TURTLE_STATUS };
	CString	m_fontName;
	int		m_fontWeight;
	int		m_fontSize;
	CString	m_penContact;
	CString	m_turtleVisibility;
	UINT	m_colorFloodB;
	UINT	m_colorFloodG;
	UINT	m_colorFloodR;
	UINT	m_colorScreenB;
	UINT	m_colorScreenG;
	UINT	m_colorScreenR;
	UINT	m_colorPenB;
	UINT	m_colorPenG;
	UINT	m_colorPenR;
	UINT	m_penStyle;
	UINT	m_penWidth;
	double	m_turtleHeading;
	double	m_turtlePosX;
	double	m_turtlePosY;
	UINT	m_winDimX;
	UINT	m_winDimY;
	double	m_viewTop;
	double	m_viewBottom;
	double	m_viewLeft;
	double	m_viewRight;
	UINT	m_viewDimX;
	UINT	m_viewDimY;
	int		m_winPosX;
	int		m_winPosY;
	CString	m_name;
	//}}AFX_DATA

//	UINT	m_already;
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CStatusDialog)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
