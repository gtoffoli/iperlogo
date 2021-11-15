// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CWnd* m_status;
	char m_saveFile [64];
	HMENU m_hMenu;					// 990112

// Operations
public:
	
	void SetTitleBar (BOOL setOn);	// 990112
	void SetMenuBar (BOOL setOn);	// 990112

// Implementation
public:
	virtual ~CMainFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnMainStatusRefresh();		// custom
	void OnHideStatus();			//
	void CommandPrompt();			//
	void RecordProfile();			// 980403 
	int ConfirmExit();				// 980903
	void OnWindowsWindow();			// 990126
protected:

	afx_msg void OnMenuSelect(UINT nItemId, UINT nFlags, HMENU hSysMenu);
    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnFileClear();
	afx_msg void OnFileNew();
	afx_msg void OnFileLoad();
	afx_msg void OnFilesave();
	afx_msg void OnFileSaveas();
	afx_msg void OnFileErase();
	afx_msg void OnFilePrintsetup();
	afx_msg void OnWindowsEditor();
	afx_msg void OnWindowsTurtle();
	afx_msg void OnWindowsBrowser();
	afx_msg void OnWindowsPlayer();
	afx_msg void OnOptionTrace();
	afx_msg void OnOptionShowStatus();
	afx_msg void OnExec();
	afx_msg void OnPause();
	afx_msg void OnHalt();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOptionFont();
	afx_msg void OnOptionEco();
	afx_msg void OnOptionPrompt();
	afx_msg void OnOptionEcoint();
	afx_msg void OnRegister();
	afx_msg void OnDestroy();
	afx_msg void OnOptionProfile();
	afx_msg void OnClose();
	afx_msg void OnHelpDemo();
	afx_msg void OnHelpExamples();
	afx_msg void OnHelpIndex();
	afx_msg void OnHelpRelease();
	afx_msg void OnHelpSearch();
	afx_msg void OnHelpTopic();
	afx_msg void OnHelpTutorial();
	afx_msg void OnHelpUsing();
	afx_msg void OnHelpGlossary();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
