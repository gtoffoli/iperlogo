// turtleCtlr.h : interface of the CTurtleController class
//
/*	turtleCtrl.h

000421	aggiunto OnFileOpen, OnFileSave, OnFileSaveAs, FileLoad, FileSave
		sostituito FileExportPrompt con FileOpenSavePrompt
000228	aggiunto AddToSelection
991105	spostato gestione menu da turtleCtlr a turtleVw
991014	aggiunto SpecialZoom
991014	mediato OnPageSetup con PageSetup
990923	PR_SYMBOL_INSERT e m_ptInsertPoint
		ovverride di OnInsertSymbol e InsertSymbol
990922	integrato menu dinamici
990920  override di OnLButtonDblClk e StartTextEdit
990909  override e aggiunte per estensione di CODController::OnFileExport
*/

#if !defined(AFX_TURTLECTLR_H__BCD4B56F_5259_11D3_A012_444553540000__INCLUDED_)
#define AFX_TURTLECTLR_H__BCD4B56F_5259_11D3_A012_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTurtleViewport;

class CTurtleController : public CODController
{
public:
	CTurtleController(CTurtleViewport* pViewport);

// Attributes
public:

	CPoint m_ptInsertPoint;
	BOOL m_bInserted;

// Operations
public:

// Overrides
	BOOL SaveJpeg(LPCTSTR lpszFileName);
	BOOL FileLoad(CString PathName);
	BOOL FileSave(CString PathName, BOOL bSaveAs);
//	BOOL FileExportPrompt(CString& strFileName, DWORD* pFilterIdx, CString strTitle, CString strFilter);
	BOOL FileOpenSavePrompt(BOOL bOpenFileDialog, CString& strFileName, DWORD* pFilterIdx, CString strTitle, CString strFilter);
	BOOL SymbolLoadPrompt(CString& strFileName, CString strTitle, CString strFilter);
	void CTurtleController::OnInsertSymbol (CODSymbolComponent* pSymbol);
	void InsertSymbol(UINT nFlags, CPoint ptDev);
	void SpecialZoom (int function);
	void AddToSelection (CODComponentSet* pComps);
	void OnPan ();
	void OnDrawSelect ();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	BOOL StartTextEdit(UINT nFlags, CPoint ptDev);
	void PageSetup();
	void OnMouseEvent(int iEvent, CPoint point);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTurtleController)
	afx_msg void OnFileExport();
//>000421
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
//<
//>991017
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//<
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTurtleController();

	// Generated message map functions
	//{{AFX_MSG(CTurtleController)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnSymbolLoad();
	afx_msg void OnEdgeCreate();
	afx_msg void OnEdgeInvert();
	//}}AFX_MSG

//	afx_msg void OnMenuButton(UINT uID);				// 990922, 991105
//	afx_msg void OnUpdateMenuButton(CCmdUI *pCmdUI);	// 990922, 991105
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURTLECTLR_H__BCD4B56F_5259_11D3_A012_444553540000__INCLUDED_)
