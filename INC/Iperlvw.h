// iperlvw.h : interface of the CIperlogoView class
//
/////////////////////////////////////////////////////////////////////////////

class CIperlogoView : public CFormView
{
protected: // create from serialization only
	CIperlogoView();
	DECLARE_DYNCREATE(CIperlogoView)

public:
	//{{AFX_DATA(CIperlogoView)
	enum { IDD = IDD_IPERLOGO_FORM };
	CEdit	m_txtout2;
	CListBox	m_txtout;
	CString	m_txtin;
	//}}AFX_DATA

// Attributes
public:
	CIperlogoDoc* GetDocument();

// Operations
public:

// Implementation
public:
	virtual ~CIperlogoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void Print(int fun, char *pIn, int nIn);// custom code
	void OnExec();						// custom code
	void OnOptionFont();				// custom code
	void SelChange(const char *s);		// 980901
	LOGFONT* RetrieveFont();			// 990223
	void AssignFont(LOGFONT* pLogFont);	// 990223
	void ActivateFont();				// 990223: was protected 

	LOGFONT m_logFont;					// 980401: was protected 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
// New	
	virtual void OnInitialUpdate();
//	LOGFONT m_logFont;		// 980401: was made public 
	LOGFONT* m_pLogFont;
	CFont *m_pFont;
//	void ActivateFont();	// 990223: was made public
//

// Generated message map functions
protected:
	//{{AFX_MSG(CIperlogoView)
	afx_msg void OnSelchangeList1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in iperlvw.cpp
inline CIperlogoDoc* CIperlogoView::GetDocument()
   { return (CIperlogoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

	////< custom code 1
// void cputs (CString s);	// emulates library function of Unix C
	////> custom code 1
