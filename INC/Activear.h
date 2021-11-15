// actvarea.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActiveArea dialog

class CActiveArea : public CDialog
{
// Construction
public:
	CActiveArea(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CActiveArea)
	enum { IDD = IDD_ACTIVE_AREA };
	double	m_areaXhigh;
	double	m_areaXlow;
	double	m_areaYhigh;
	double	m_areaYlow;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CActiveArea)
	virtual void OnOK();
	afx_msg void OnReset();
	afx_msg void OnSelection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
