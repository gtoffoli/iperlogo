/* EDITORVW.H		G. Toffoli

010207	allineato con IL32: commentato "#ifdef INTEGRA"
000819	aggiunto PreCreateWindow, sotto switch INTEGRA
*/

// editorvw.h : header file
//

#include "stdafx.h"
// #include "afxext.h"

/////////////////////////////////////////////////////////////////////////////
// CEditorView view
     
#ifndef DEF_EDITORVIEW
#define DEF_EDITORVIEW
    
class CEditorView : public CEditView
{
	DECLARE_DYNCREATE(CEditorView)
protected:
	CEditorView();			// protected constructor used by dynamic creation
// BOOL PreCreateWindow(CREATESTRUCT& cs);	////

// Attributes
public:
  
// Operations
public:
// #ifdef INTEGRA
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// #endif // INTEGRA

unsigned long GetAttr ();
void SetAttr (node attrList);
void UpdateTitle ();			// new : 980623
//void LoadSingleFile(CString); // new
int LoadSingleFile(CString);	// new
int SaveFile(CString);			// new
void GetName(char* name);
void OnTest ();					// new
void ResetReadSel ();			// new
int ReadSelLine (int nMaxLength);	// new : increment n_NextChar
int GetReadLine ();				// new : increment read line index
void SetReadLine (int);			// new : increment read line index
void FindNext (LPCSTR FindString, BOOL isSearchDown, BOOL isMatchCase);	// new
void ReplaceSel (LPCSTR FindString, BOOL isSearchDown, BOOL isMatchCase, LPCSTR ReplaceString);	// new
void ReplaceAll (LPCSTR FindString, LPCSTR ReplaceString, BOOL isMatchCase);	// new
int ConfirmExit();			// 980903

virtual ~CEditorView();
	
// Implementation
protected:
//	virtual ~CEditorView();

// New	
	int m_readLine;			// index of next line to be read
	COLORREF m_screenColor;
	COLORREF m_penColor;
//	LOGFONT m_logFont;		// reso public
	LOGFONT* m_pLogFont;
	CFont *m_pFont;

	virtual	void OnInitialUpdate();						// first time after construct
	virtual	void OnDraw(CDC* pDC);						// overridden to draw this view
	virtual void OnTextNotFound( LPCSTR lpszFind ); 	// overridden to add behaviour
// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);

public:
	int m_self;					// index of self in editor array
	CWnd* m_frame;
	BOOL m_isExecute;
	char m_saveFile [128];
	BOOL m_saveAs;				// nome file esplicitato (Save As)
	int m_findReplaceFunction;	// 1: find, 2: replace current, 3: replace all
	CString m_findString;
	CString m_replaceString;
	BOOL m_isSearchDown;
	BOOL m_isMatchCase;
	BOOL m_textFound;
	int m_nStartChar, m_nLastChar, m_nNextChar;
	CString m_selLine;
	LOGFONT m_logFont;		// gia' protected

	int m_iChar;					// pos. carattere corrente
	int m_iReadChar, m_iWriteChar;	// pos. car. da leggere / scrivere
	int m_iBeginBlock, m_iEndBlock;	// pos. inizio / fine (+1) blocco
	BOOL m_isUpdate;
	BOOL m_isInsert;

	// Generated message map functions
	//{{AFX_MSG(CEditorView)
	afx_msg void OnFileNew();
	afx_msg void OnEditDelete();
	afx_msg void OnEditSelectall();
	afx_msg void OnFileExecandexit();
	afx_msg void OnFileExit();
	afx_msg void OnFileLoad();
	afx_msg void OnSearchFind();
	afx_msg void OnTestAll();
	afx_msg void OnSearchReplace();
	afx_msg void OnFileSave();
	afx_msg void OnFileClose();
	afx_msg void OnFilePrintsetup();
	afx_msg void OnSetFont();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileEditproc();
	afx_msg void OnTestSelection();
	afx_msg void OnSetPencolor();
	afx_msg void OnSetScreencolor();
	afx_msg void OnSearchFindnext();
	afx_msg void OnSelectEditor();
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
