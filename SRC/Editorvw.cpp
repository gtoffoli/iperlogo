/* EDITORVW.CPP		G. Toffoli

020603	in CEditorView::OnFileLoad() agg. test e messaggio di errore
020601	in CEditorView::LoadSingleFile agg. test di errore
010207	allineato con IL32: commentato "#ifdef INTEGRA"
000819	sotto switch INTEGRA:
	- aggiunto PreCreateWindow,
	- in apri_foglio aggiunto attributo ACCAPO (= word wrapping, default: FALSO)
990914	corretto OnFileNew 
990629	in LoadSingleFile salvataggio di path completo in m_savefile 
990615	corretto FOGLIOESEGUITUTTO
990525	fatto emergere l'implementazione default di OnFilePrint
		per uso in FOGLIOSTAMPA
990523	corretto FOGLIOESEGUISELEZIONE
		e FOGLIOESEGUITUTTO (confronta con PR_EXECFOGLIO)
990418	aggiunto FOGLIOIMPOSTASTAMPANTE, FOGLIOSTAMPA (rivedere)
		FOGLIOTAGLIA,FOGLIOCOPIA, FOGLIOINCOLLA,
		FOGLIOASSEGNAFONT, FOGLIOESEGUISELEZIONE (rivedere)
990413	aggiunto CONTACARATTERI (_CONTAC) e _CONTALINEE (_CONTAL)
990408	inizio del recupero delle primitive di Foglio
990311	corretto gestione di titolo in GETATTR
990222	gestione dell'attributo FONT
990126	gestione editor corrente in menu Foglio
990112	PR_FOGLIO, PR_CHIUDIFOGLIO,
		PR_RECUFOGLIO, PR_CONSFOGLIO, PR_EXECFOGLIO
990111	merge con ILFOGLIO.CPP
981224	controllo sulla chiusura
		da parte di codice eseguito dall'interno
981208	semplificato sintassi di ATTR e SETATTR per valori coppia
		aggiunto primitiva CREAFOGLIO nome lista-associativa
981124	aggiunto ::Attr e ::SetAttr, come in CTurtleView
980902	fattorizzato EditPrint in OnPrint
		gestione del "modified flag" per chiusura morbida
980901	completato OnPrint
980831	eliminato extern "C" per conversione a C++ dei moduli C
980721	aggiunto controllo su esistenza menu
980709	ritoccato OnFileLoad (come per Editor)
980623	modifica a OnFileLoad:
	- tolto clear, selezione multipla, titolo esteso
980210	esteso TestSelection (?)
		sostituito ExecOnEvent con NestedExec
980126	WIN32: eliminato spazi dalla stringa filtro dei CFileDialog
971223	modificato EditorBringToTop
971218	Menu Editor-Imposta: disattivato Sfondo e EFFETTI (colore) in Font
970627	ritocchi al menu di Editor
970618	eliminato mediazione di ILVIEW.C
970512	implementazione operazioni GetLineCount, GetLineIndex
970512	implementazione operazioni Clear, Replace
*/
// editorvw.cpp : implementation file
//

#include "stdafx.h"                   

#include "logo.h"
#include "iperlogo.h"
#include "editorvw.h"

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"
#include "editorfr.h"
#include "selprocd.h"

#include "ilcpp.h"

#define BIG_SIZE (4 * K)
#define MAXI_SIZE (64 * K)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SAVE
#define RESTORE
#define PROVASTAMPA

extern CIperlogoApp theApp;
extern CREATESTRUCT Editor;

int ListToFont (node tail, LOGFONT* lpLogFont);
node FontToList (LOGFONT* lpLogFont);

extern CString ini_name;
extern CString editor_name;
extern char EditorFont[]; 

node dev2nome (int dev);
int chiudi_foglio (int dev);
void ProcToDevByName (int dev, char* name);
void EditAllProcs ();
int handle2dev (int handle, int stato);

int MatchKw (node, node);
node KwAttr (node lName, node tail, node kWord);

int currentWindow (int mask);
void MainMenuShowWindow (char *name);
void MainMenuAppendWindowItem (char *name);
void MainMenuCheckWindowItem (UINT iMenu, char *name, BOOL status);
void MainMenuRemoveWindowItem (char *name);

int dev_editor = -1;
int newEditorCounter = 1;
char DefaultEditor[64] = "";

//>990412
// deve riportare la "posizione corrente del cursore"
// ora riporta quella ufficiale (m_iChar)
// forse potrebbe riportare quella reale, tenuto conto di interazione
#define GetIChar editor->m_iChar
//int getIChar ()
//{
//  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
//	int startChar, lastChar;
//	(editor->GetEditCtrl()).GetSel (startChar, lastChar);
//	return startChar; 
//}
#define SetIChar(i) editor->m_iChar = i; \
		(editor->GetEditCtrl()).SetSel (i, i, ! editor->m_isUpdate);
//<

int view_to_devfoglio (int view)
{
	int i;
	for (i=0; i<MAX_DEV; i++)
		if ((_file[i] == view) && (_fstato[i] & O_FOGLIO)) break;
	return i;
}

void CEditorView::GetName(char* name)
{
	int dev = view_to_devfoglio (m_self);
	getnam (name, _nome [dev]);
}

CMenu* EditorGetMenuItem (int nview, int iPos)
{
	CEditorView* editorView = theApp.m_editorViews[nview];
	CWnd* editorFrame = editorView->m_frame;
	CMenu* editorMenu = editorFrame->GetMenu ();
	if (! editorMenu) return NULL;	// 980717
	return editorMenu->GetSubMenu (iPos);
}

CMenu* EditorGetMenu (int nview)
{
	CEditorView* editorView = theApp.m_editorViews[nview];
	CWnd* editorFrame = editorView->m_frame;
	return editorFrame->GetMenu ();
}

void EditorMenuItemEnable (int nview, UINT itemID, BOOL status)
{
	UINT nEnable = (status) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	CMenu* menu = EditorGetMenu (nview);
	if (! menu) return;		// 980717
	UINT oldStatus =
		menu->EnableMenuItem(itemID, MF_BYCOMMAND | nEnable);
}

void EditorMenuActivateSelect (int nview, BOOL status)
{
	UINT nEnable = (status) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	CMenu* menu = EditorGetMenu (nview);
	if (! menu) return;
	UINT oldStatus =
		menu->EnableMenuItem(ID_SELECT_EDITOR, MF_BYCOMMAND | nEnable);
}

void EditorMenuCheckSelect (int nview, BOOL status)
{
	UINT nCheck = (status) ? MF_CHECKED : MF_UNCHECKED; 
	CMenu* menu = EditorGetMenu (nview);
	if (! menu) return;
	menu->CheckMenuItem(ID_SELECT_EDITOR, MF_BYCOMMAND | nCheck);
}

/////////////////////////////////////////////////////////////////////////////
// imitato da TURTLEVW.CPP

CMainFrame* GetNewEditorFrame(int x, int y, int dimx, int dimy, char *pTitle, int menu)
{
    int oldX = Editor.x, oldY = Editor.y;
    int oldDimx = Editor.cx, oldDimy = Editor.cy;
	char * pOldTitle = (char *) Editor.lpszName;
	HMENU oldMenu = Editor.hMenu;					// 980721

	if (x != -1) Editor.x = x;
	if (y != -1) Editor.y = y;
	if (dimx != -1) Editor.cx = dimx;
	if (dimy != -1) Editor.cy = dimy;
	Editor.lpszName = pTitle;
	if (menu != -1) Editor.hMenu = (HMENU) menu;	// 980721

	CSingleDocTemplate* pTemplate = theApp.NewEditorTemplate();
	ASSERT(pTemplate != NULL);
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
	pTemplate->OpenDocumentFile(NULL);
	CMainFrame* editorFrame = (CMainFrame*) theApp.m_pMainWnd;

    Editor.x = oldX; Editor.y = oldY;
    Editor.cx = oldDimx; Editor.cy = oldDimy;
	Editor.lpszName = pOldTitle;
	Editor.hMenu = oldMenu;							// 980721

	return editorFrame;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView

int EditorOpenView (int x, int y, int dimx, int dimy, char *pTitle, int menu)
{
	int nview;
	// salvo il MainFrame nominale dell' applicazione 
	CWnd* pOldMain = AfxGetApp()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = 0L;	 

	CMainFrame* editorFrame = GetNewEditorFrame (x, y, dimx, dimy, pTitle, menu);    
	CEditorView* editorView = (CEditorView*) editorFrame->GetActiveView();
    editorView->GetDocument()->SetTitle("Editor");

	for (nview = 0; nview < MAX_EDITOR_VIEWS; nview++)
		if (theApp.m_editorViews[nview] == NULL) break;
	theApp.m_editorViews[nview] = editorView;

	editorView->m_self = nview;
    editorView->m_frame = editorFrame;
    ((CEditorFrame*) editorFrame)->m_view = editorView;
	editorView->m_isExecute = FALSE;
	editorView->m_saveFile[0] = 0;
	editorView->m_saveAs = FALSE;

	if (pTitle) {
		editorFrame->SetWindowText((pTitle == (char *) -1L) ? "EditorWindow" : pTitle);
	}
    
	// ripristino il MainFrame nominale dell' applicazione 
	AfxGetApp()->m_pMainWnd = pOldMain;

    return nview;
}

BOOL EditorCloseView (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	if (editor->m_isExecute)
		return FALSE;
	else {
		delete(editor->m_frame);
		theApp.m_editorViews[nview] = NULL;
		return TRUE;
	}
}

void EditorBringToTop (int nview) 
{
	CEditorView* editor = theApp.m_editorViews[nview];
//	(editor->m_frame)->BringWindowToTop ();		spostato
	(editor->m_frame)->ShowWindow (SW_RESTORE);	// 971223
	(editor->m_frame)->BringWindowToTop ();		// 990305
}


node EditorGetAttr (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	return editor->GetAttr ();
} 
void EditorSetAttr (int nview, node attrList)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	editor->SetAttr (attrList);
} 

void EditorClear (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	(editor->GetEditCtrl()).SetSel (0, -1, FALSE);
	(editor->GetEditCtrl()).Clear ();
//>990914
	editor->m_saveFile[0]= '\0';
	editor->UpdateTitle ();
//<
} 

int EditorLoad (int nview, char* fileName)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	return editor->LoadSingleFile (fileName);
} 

int EditorSave (int nview, char* fileName)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	return editor->SaveFile (fileName);
} 

void EditorReplace (int nview, char* s)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	(editor->GetEditCtrl()).ReplaceSel (s);
//>990412
	int startChar, lastChar;
	(editor->GetEditCtrl()).GetSel (startChar, lastChar);
	int i = lastChar;
	SetIChar(i);
//<
} 

void EditorResetRead (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	editor->SetReadLine (0);
} 

int EditorGetLineCount (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	int n = (editor->GetEditCtrl()).GetLineCount ();
	return n;
} 

int EditorGetLineIndex (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	int n = editor->GetReadLine ();
	return n;
} 

BOOL EditorEOF (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	if (editor->m_nNextChar == -1)
		return (editor->GetReadLine() >=
				(editor->GetEditCtrl()).GetLineCount ());
	else
		return (editor->m_nNextChar >=
				editor->m_nLastChar);
} 

int EditorGetLine (int nview, char* s, int nMaxLength)
{
	int nLength, iLine;
	CEditorView* editor = theApp.m_editorViews[nview];
	if (editor->m_nNextChar == -1) {
		iLine = editor->GetReadLine ();
		nLength = (editor->GetEditCtrl()).GetLine (iLine, s, nMaxLength);
		editor->SetReadLine (iLine + 1);
	}
	else {
		nLength = editor->ReadSelLine (nMaxLength);
		nLength = min(nLength, nMaxLength);
		// aggiungere controllo !!
		strcpy (s, editor->m_selLine);
	}
	return nLength;
} 

void EditorResetReadSel (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	editor->ResetReadSel ();
} 

void EditorExec (int nview)
{
	CEditorView* editor = theApp.m_editorViews[nview];
	editor->OnTestAll();
} 

IMPLEMENT_DYNCREATE(CEditorView, CEditView)

                                
                                
CEditorView::CEditorView()
{
}

CEditorView::~CEditorView()
{
	if (m_pFont) delete(m_pFont);
}

// #ifdef INTEGRA
// set word wrapping in place of horizontal scrolling
// (see note on CPadView class in the doc of SUPERPAD sample in MSDN Library)
int bWordWrapping = FALSE;

BOOL CEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (! CEditView::PreCreateWindow(cs)) return FALSE;

	if (bWordWrapping) {
		cs.style = cs.style & (~ (WS_HSCROLL | ES_AUTOHSCROLL));
	}
	return TRUE;
}
// #endif // INTEGRA

void CEditorView::OnInitialUpdate()
{
	m_iChar = 0;
	m_iReadChar = m_iWriteChar = 0;
	m_iBeginBlock = m_iEndBlock = -1;
	m_isUpdate = TRUE;

	m_nStartChar = m_nLastChar = m_nNextChar = -1;
	// Default values for colors (not used ?) ..
	m_screenColor = RGB(255,0,0);
	m_penColor = RGB(0,0,0);
	// Default font ..
	::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	// .. possibly to be replaced from custom font
	int n =
		GetPrivateProfileString(editor_name, "Font", "",
					EditorFont, MAX_CHARS, ini_name);
	if (n > 0) {
		node FontParList = _parse (EditorFont);
		ListToFont (FontParList, &m_logFont);
	}
	// Create CFont object
	m_pLogFont = &m_logFont;
	m_pFont = new(CFont);
	m_pFont->CreateFontIndirect(&m_logFont);
	SetFont(m_pFont);

//	CScrollView::OnInitialUpdate();
}

void CEditorView::OnDraw(CDC* pDC)
{
	pDC->SetBkColor(m_screenColor);
	pDC->SetTextColor(m_penColor);
	CEditView::OnDraw(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView printing

	CPrintDialog* pOldPrintDialog;

BOOL CEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
//	return CView::OnPreparePrinting(pInfo);

	// default preparation
//	return DoPreparePrinting(pInfo);

	// custom preparation      PRINTDLG
	BOOL bPrintSetupOnly = FALSE;
	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CWnd* pParentWnd = (CWnd*) m_frame;
	static CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd); // reso globale
	if (theApp.m_editorPrinterData != NULL)
		printDialog.m_pd.hDevMode = theApp.m_editorPrinterData;
	int result = printDialog.DoModal();
	if (result == IDOK) {
		theApp.m_editorPrinterData = printDialog.m_pd.hDevMode;
		pOldPrintDialog = pInfo->m_pPD; 
		pInfo->m_pPD = &printDialog;
		pInfo->SetMaxPage(0);
	}
//	else delete(pPrintDialog);
	return (result == IDOK);
}

void CEditorView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
//	CView::OnBeginPrinting(pDC, pInfo);
}

void CEditorView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
//	CView::OnEndPrinting(pDC, pInfo);
 	int result = pDC->EndDoc();
	pInfo->m_pPD = pOldPrintDialog;
}

// void CEditorView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
void EditPrint (CEdit* pEdit, char *docName, CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add code to print the controls
		int result;
//>980901
		DOCINFO DocInfo; LPDOCINFO lpDocInfo = &DocInfo;
		DocInfo.lpszDocName = docName;
		DocInfo.lpszOutput = NULL;
		DocInfo.cbSize = sizeof(int) + 2 * sizeof(LPCSTR);
		result = pDC->StartDoc(lpDocInfo);

		int horzSize = pDC->GetDeviceCaps(HORZRES);
		int vertSize = pDC->GetDeviceCaps(VERTRES);
//<
 		result = pDC->StartPage();
		int y = 0, x = 0, cy = 0, maxHeight, minHeight = 8;

		int lineCount = pEdit->GetLineCount ();
		int nIndex, nChars;
		char lineBuffer [BIG_SIZE+1];
		for (nIndex=0; nIndex<lineCount; nIndex++) {
			maxHeight = max(cy, minHeight);
			nChars = pEdit->GetLine(nIndex, lineBuffer, BIG_SIZE);
			if (nChars == 0)
				y += maxHeight;
			else {
				if (y+maxHeight > vertSize) {
 					result = pDC->EndPage();
 					result = pDC->StartPage();
					y = 0;
				}
				lineBuffer[nChars] = '\0';
				cy = pDC->GetTextExtent(lineBuffer, nChars).cy;
				result = pDC->TextOut(x, y, lineBuffer, nChars);
        		if (! result) AfxMessageBox("NON ha risposto");
				y += cy;
			}
		}
		 
 		result = pDC->EndPage();
//>980901
 		result = pDC->EndDoc();
//<
}

void CEditorView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add code to print the controls

	EditPrint (&(GetEditCtrl()), "Foglio", pDC, pInfo);
}

int CEditorView::GetReadLine ()			// new : get read line index
{
  return m_readLine;
}

void CEditorView::SetReadLine (int i)	// new : get read line index
{
  m_readLine = i;
}

void CEditorView::FindNext (LPCSTR FindString, BOOL isSearchDown, BOOL isMatchCase)
{
	OnFindNext (FindString, isSearchDown, isMatchCase);
//>990412
	int startChar, lastChar;
	GetEditCtrl().GetSel (startChar, lastChar);
	m_iChar = lastChar;
//<
}

void CEditorView::ReplaceSel (LPCSTR FindString, BOOL isSearchDown, BOOL isMatchCase, LPCSTR ReplaceString)
{
	OnReplaceSel (FindString, isSearchDown, isMatchCase, ReplaceString);
//>990412
	int startChar, lastChar;
	GetEditCtrl().GetSel (startChar, lastChar);
	m_iChar = lastChar;
//<
}

void CEditorView::ReplaceAll (LPCSTR FindString, LPCSTR ReplaceString, BOOL isMatchCase)
{
	OnReplaceAll (FindString, ReplaceString, isMatchCase);
//>990412
	int startChar, lastChar;
	GetEditCtrl().GetSel (startChar, lastChar);
	m_iChar = lastChar;
//<
}


afx_msg LRESULT CEditorFrame::OnFindReplace(WPARAM wParam, LPARAM lParam)	// added and modified
{
	CEditorView* pView = (CEditorView*) m_view;
	static CFindReplaceDialog* pDialog;
	pDialog = pDialog->GetNotifier (lParam);
//	DWORD DialogFlags = (pDialog->m_fr).Flags;
	BOOL isTerminating = pDialog->IsTerminating ();
	BOOL isSearchDown = pDialog->SearchDown ();
	BOOL isMatchCase = pDialog->MatchCase ();
	BOOL isMatchWholeWord = pDialog->MatchWholeWord ();
	CString FindString = pDialog->GetFindString ();
	BOOL isFindNext = pDialog->FindNext ();
	BOOL isReplaceCurrent = pDialog->ReplaceCurrent ();
	BOOL isReplaceAll = pDialog->ReplaceAll ();
	CString ReplaceString = pDialog->GetReplaceString ();
	CEditorView* editorView = (CEditorView*) GetActiveView();
	editorView->m_findString = FindString;
	editorView->m_replaceString = ReplaceString;
	editorView->m_isSearchDown = isSearchDown;
	editorView->m_isMatchCase = isMatchCase;
	if (isFindNext) {
		pView->FindNext (FindString, isSearchDown, isMatchCase);
		editorView->m_findReplaceFunction = 1;
	}
	else if (isReplaceCurrent) {
		pView->ReplaceSel (FindString, isSearchDown, isMatchCase, ReplaceString);
		editorView->m_findReplaceFunction = 2;
	}
	else if (isReplaceAll) {
		pView->ReplaceAll (FindString, ReplaceString, isMatchCase);
		editorView->m_findReplaceFunction = 3;
	}
	return 0;
}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
unsigned long CEditorView::GetAttr ()
{
	node lista = NULLP;

	CRect winRect;
	m_frame->GetWindowRect(&winRect);

	node font = FontToList(m_pLogFont);
//	lista = rassoc (pc_FONT, font, lista);
	font = cons (pc_FONT, font, LIST);
	lista = cons (font, lista, LIST);

	lista = rassoc2 (pc_DIM,
		consreal (winRect.right - winRect.left),
		consreal (winRect.bottom - winRect.top), lista);

	lista = rassoc2 (pc_POS,
		consreal (winRect.left), consreal (winRect.top), lista);

	CMenu* pMenu = m_frame->GetMenu ();
	node menu = (pMenu == NULL) ? pc_FALSO : pc_VERO;
	lista = rassoc (pc_MENU, menu, lista);

	char sTitolo [MAXCHARS];
	int nTitolo = m_frame->GetWindowText(sTitolo, MAXCHARS);
/*>990311
	node titolo = (nTitolo == 0) ? pc_FALSO : _parse (sTitolo);
*/
	BOOL isTitolo = ((m_frame->GetStyle() & WS_CAPTION) != 0);
	node titolo = (isTitolo) ? _parse (sTitolo) : pc_FALSO;
/*<*/
	lista = rassoc (pc_TITLE, titolo, lista);

	return lista;
}

node KwAssoc (node tail, node kWord);
void CEditorView::SetAttr (node attrList)
{
	CRect winRect;
	m_frame->GetWindowRect(&winRect);
	int xPos = winRect.left;
	int yPos = winRect.top;
	int xDim = winRect.right - winRect.left;
	int yDim = winRect.bottom - winRect.top;

	char sTitolo [MAX_CHARS];
	node title = KwAssoc (attrList, pc_TITLE);
	if (title != NULLP) {
		title = car (cdr (title));
		BOOL isFalso = MatchKw (title, pc_FALSO);
		BOOL isVero = MatchKw (title, pc_VERO);
		((CEditorFrame*) m_frame)->SetTitleBar (! isFalso);
		if ((! isFalso) && (! isVero)) {
			MemScrivi (sTitolo, title);
			m_frame->SetWindowText(sTitolo);
		}
	}
	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		if (MatchKw (menu, pc_FALSO))
			((CEditorFrame*) m_frame)->SetMenuBar (FALSE);
		else if (MatchKw (menu, pc_VERO))
			((CEditorFrame*) m_frame)->SetMenuBar (TRUE);
	}
	node pos = KwAssoc (attrList, pc_POS);
	if (pos != NULLP) {
		pos = cdr (pos);
		xPos = rnum_int (car (pos));
		yPos = rnum_int (car (cdr (pos)));
	}
	node dim = KwAssoc (attrList, pc_DIM);
	if (dim != NULLP) {
		dim = cdr (dim);
		xDim = rnum_int (car (dim));
		yDim = rnum_int (car (cdr (dim)));
	}
	if ((pos != NULLP) || (dim != NULLP)) {
		m_frame->MoveWindow(xPos, yPos, xDim, yDim, TRUE);
	}

	node font = KwAssoc (attrList, pc_FONT);
	if (font != NULLP) {
		font = cdr (font);
		LOGFONT *lpLogFont = (LOGFONT *) calloc(1, sizeof(LOGFONT));
		if (! ListToFont (font, lpLogFont)) {
			free(lpLogFont);
			err2 (5, arg_2); return;
		}
		m_logFont = *lpLogFont;
		free(lpLogFont);
		if (m_pFont != NULL) delete(m_pFont);
		m_pFont = new(CFont);
		m_pFont->CreateFontIndirect(&m_logFont);
		m_pLogFont = &m_logFont;
		SetFont(m_pFont);
	}
}


BEGIN_MESSAGE_MAP(CEditorView, CEditView)
	//{{AFX_MSG_MAP(CEditorView)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_FILE_EXECANDEXIT, OnFileExecandexit)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_SEARCH_FIND, OnSearchFind)
	ON_COMMAND(ID_TEST_ALL, OnTestAll)
	ON_COMMAND(ID_SEARCH_REPLACE, OnSearchReplace)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_PRINTSETUP, OnFilePrintsetup)
	ON_COMMAND(ID_SET_FONT, OnSetFont)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_EDITPROC, OnFileEditproc)
	ON_COMMAND(ID_TEST_SELECTION, OnTestSelection)
	ON_COMMAND(ID_SET_PENCOLOR, OnSetPencolor)
	ON_COMMAND(ID_SET_SCREENCOLOR, OnSetScreencolor)
	ON_COMMAND(ID_SEARCH_FINDNEXT, OnSearchFindnext)
	ON_COMMAND(ID_SELECT_EDITOR, OnSelectEditor)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)					// inserito manualmente
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)	// inserito manuale
END_MESSAGE_MAP()


void CEditorView::OnFileNew()
{
	// TODO: Add your command handler code here
	
	GetEditCtrl().SetSel (0, -1, FALSE);
	GetEditCtrl().Clear ();
//>990914
	m_saveFile[0]= '\0';
	UpdateTitle ();
//<
}

void CEditorView::OnFileEditproc()
{
	// TODO: Add your command handler code here
	
	int result;
 	CWnd* pParentWnd = NULL;
	CSelprocDialog selprocDialog (pParentWnd);
	result = selprocDialog.DoModal();
	if (result == IDOK) {
		if (strcmp ((char *) &selprocDialog.m_text, "*") == 0)
			EditAllProcs ();
		else
			ProcToDevByName (view_to_devfoglio (m_self), (char *) (&selprocDialog.m_text));
	}
}

void CEditorView::OnEditDelete()
{
	// TODO: Add your command handler code here
	
	GetEditCtrl().Clear ();
}

void CEditorView::OnEditSelectall()
{
	// TODO: Add your command handler code here
	
	GetEditCtrl().SetSel (0, -1, FALSE);
}

int CEditorView::ConfirmExit()
{
	CString msgCaption = "Iperlogo - Foglio";
	CString msgText =
		"Il testo modificato non è stato salvato. Vuoi procedere ugualmente ?";
	if (GetEditCtrl().GetModify())
		return (MessageBox(msgText, msgCaption, MB_OKCANCEL) == IDOK);
	else
		return TRUE;
}

void CEditorView::OnFileExit()
{
	// TODO: Add your command handler code here
	
	if (ConfirmExit ())		// 980902
		chiudi_foglio (view_to_devfoglio (m_self));
}

void CEditorView::ResetReadSel ()
{
	GetEditCtrl().GetSel(m_nStartChar, m_nLastChar);
	m_nNextChar = m_nStartChar;
}

int CEditorView::ReadSelLine (int nMaxLength)
{
	int nLeft, nLength;
	static CString Delimiters = "\n";
	nLeft = m_nLastChar - m_nNextChar;
	if (nLeft == 0)
		return 0;
	const char* lpBuffer = LockBuffer();
	CString text (lpBuffer+m_nNextChar, nLeft);
	UnlockBuffer();
	m_selLine = text.SpanExcluding(Delimiters);
	nLength = m_selLine.GetLength();
	m_nNextChar += m_selLine.GetLength();
	if (nLength < nLeft)
		++m_nNextChar;
	if (m_selLine.GetAt(nLength-1) == '\r') {
		--nLength;
		m_selLine = m_selLine.Left(nLength);
	}
	return nLength;
}

// non essendo una primitiva, prima di "RECUPERARE"
// deve creare un'attivazione dell'interprete
void CEditorView::OnTest()
{
	int dev = view_to_devfoglio (m_self);
	node lname = dev2nome (dev);	
//	ExecOnEvent (cons (pc_RECUPERA, cons (DEL_VIRGOLETTE, cons (lname, NULLP, LIST), LIST), LIST)); 		 
	m_isExecute = TRUE;
	NestedExec (cons (pc_RECUPERA,
		cons (DEL_VIRGOLETTE,
		cons (lname, NULLP, LIST), LIST), LIST)); 		 
	m_isExecute = FALSE;
}

void CEditorView::OnTestAll()
{
	// TODO: Add your command handler code here

	m_nStartChar = m_nLastChar = m_nNextChar = -1;
	OnTest ();
}

#define MULTISEL
void CEditorView::OnTestSelection()
{
	// TODO: Add your command handler code here

#ifndef MULTISEL
	CString text;
	GetSelectedText(text);
	m_isExecute = TRUE;
	execute (text);
	m_isExecute = FALSE;
#else // MULTISEL
	ResetReadSel ();
	OnTest ();
#endif // MULTISEL
}

void CEditorView::OnFileExecandexit()
{
	// TODO: Add your command handler code here
	
	OnTestAll();
	OnFileExit();
}

void CEditorView::UpdateTitle ()
{
	if (((CEditorFrame*)m_frame)->m_title[0] != '\0') {
		CString extTitle = ((CEditorFrame*)m_frame)->m_title;
		if (m_saveFile[0] != '\0') {
			extTitle += " - ";
			extTitle += m_saveFile;
		}
		m_frame->SetWindowText(extTitle);
	}
}

//void CEditorView::LoadSingleFile(CString PathName)
int CEditorView::LoadSingleFile(CString fileName)
{
	int result;
	CString filePath;
	UINT nOpenFlags;
	int lineCount = GetEditCtrl().GetLineCount();
	if (lineCount <= 1) {
		CFile file;
		nOpenFlags = CFile::modeRead;
		result = file.Open (fileName, nOpenFlags, NULL);
//>020601
		if (! result) return 0;
//<020601
//>990629
		filePath = file.GetFilePath();
//<
		UINT nMode = CArchive::load;
		int nBufSize = 512;
		void FAR* lpBuf = NULL;
		CArchive archive (&file, nMode, nBufSize, lpBuf);
		SerializeRaw (archive);
		archive.Close ();	
		file.Close ();
		int lc = GetEditCtrl().GetLineCount();
		if (lc > 1) {
			int li = GetEditCtrl().LineIndex(lc-1);
			BOOL bNoScroll = FALSE;
			GetEditCtrl().SetSel(li, li, bNoScroll);
		}
	} else {
		CString lineString;
		result = 1;
		nOpenFlags = CFile::modeRead | CFile::typeText;
		TRY {
			CStdioFile stdioFile (fileName, nOpenFlags);
			BOOL status = TRUE;
			while (status) {
				status = stdioFile.ReadString(lineString);
				if (status) {
					GetEditCtrl().ReplaceSel(lineString);
					GetEditCtrl().ReplaceSel("\r\n");
				}
			}
			stdioFile.Close();
		}
		CATCH (CFileException, e) {  
			result = 0;
		}
		END_CATCH
	}
	if (result && (! m_saveAs)) { 
		if (lineCount <= 1)
//>990629
//			strcpy (m_saveFile, fileName);
			strcpy (m_saveFile, filePath);
//<
		else
			m_saveFile[0]= '\0';
		UpdateTitle ();
	}
//>990412
	int startChar, lastChar;
	GetEditCtrl().GetSel (startChar, lastChar);
	m_iChar = lastChar;
//<
	return result;
}

void CEditorView::OnFileLoad()
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		"File IperLogo (*.il)|*.il|File WinLogo (*.lgo)|*.lgo|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = NULL;
///	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = TRUE;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);

	char PathNames[1024+1] = "";
	FileDialog.m_ofn.lpstrFile = &PathNames[0];
	FileDialog.m_ofn.nMaxFile = 1024;

	CString errMsg;	// 020603 : anticipatp
	int result = FileDialog.DoModal();
	if (result == IDOK) {
		POSITION pos = FileDialog.GetStartPosition();
		CString PathName;
//>020603
//		while (pos != NULL) {
//			PathName = FileDialog.GetNextPathName(pos);
//			LoadSingleFile(PathName);
//		}
		result = 1;
		while ((pos != NULL) && (result != 0)) {
			PathName = FileDialog.GetNextPathName(pos);
			result = LoadSingleFile(PathName);
		}
		if (result == 0) {
			errMsg = "Non riesco a leggere " + PathName;
			AfxMessageBox (errMsg);
		}
//>020603
	} else if (result == IDCANCEL) {
		int extError = CommDlgExtendedError ();
		if (extError) {
			CString errMsg;
			char errCode [64];
			_itoa (extError, errCode, 10);
			CString errNum = errCode;
			errMsg = "Editor: error #" + errNum;
			AfxMessageBox (errMsg);
		}
	}
}

int CEditorView::SaveFile(CString PathName)
{
	int result;
	UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite;
	CFile file;
	result =
		file.Open (PathName, nOpenFlags, NULL);
	if (result) {
		UINT nMode = CArchive::store;
		int nBufSize = 512;
		void FAR* lpBuf = NULL;
		CArchive archive (&file, nMode, nBufSize, lpBuf);
		SerializeRaw (archive);
		archive.Close ();	
		file.Close ();
		GetEditCtrl().SetModify(FALSE);
	}
	return result;
}

void CEditorView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		"File IperLogo (*.il)|*.il|File WinLogo (*.lgo)|*.lgo|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = "il";
	if (m_saveFile[0] != 0)
		lpszFileName = m_saveFile;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = FALSE;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);

	int result = FileDialog.DoModal();
	if (result == IDOK) {
		CString PathName = FileDialog.GetPathName();
		SaveFile(PathName);
		strcpy (m_saveFile, PathName);
		m_saveAs = TRUE;
		UpdateTitle ();
	} else {
		/* file not found */
	}
}

void CEditorView::OnFileSave()
{
	// TODO: Add your command handler code here
	
	if (m_saveFile[0] == 0)
		OnFileSaveAs();
	else
		SaveFile(m_saveFile);
}

void CEditorView::OnFileClose()
{
	// TODO: Add your command handler code here

	//	CWnd::OnClose ();
	if (ConfirmExit ())	{
		int handle = theApp.GetEditorViewIndex (this);
		int dev = handle2dev (handle, O_FOGLIO);
		chiudi_foglio (dev);
	}
}

void CEditorView::OnSearchFind()
{
	// TODO: Add your command handler code here
	CFindReplaceDialog* pFindDialog = new(CFindReplaceDialog);
	pFindDialog->Create(TRUE, "", "", FR_MATCHCASE | FR_DOWN | FR_HIDEWHOLEWORD, m_frame); 
}

void CEditorView::OnSearchFindnext()
{
	// TODO: Add your command handler code here

	m_textFound = TRUE;
	switch (m_findReplaceFunction) {
	case 1:	// Search
		OnFindNext (m_findString, m_isSearchDown, m_isMatchCase);
		break;
	case 2: // Replace (current)
		OnFindNext (m_findString, m_isSearchDown, m_isMatchCase);
		if (m_textFound)
			OnReplaceSel (m_findString, m_isSearchDown, m_isMatchCase, m_replaceString);
		break;
	case 3:	// ReplaceAll
		OnReplaceAll (m_findString, m_replaceString, m_isMatchCase);
		break;
	}
//>990412
	int startChar, lastChar;
	GetEditCtrl().GetSel (startChar, lastChar);
	m_iChar = lastChar;
//<
}

void CEditorView::OnSearchReplace()
{
	// TODO: Add your command handler code here

	CFindReplaceDialog* pReplaceDialog = new(CFindReplaceDialog);
	pReplaceDialog->Create(FALSE, "", "", FR_MATCHCASE | FR_DOWN | FR_HIDEWHOLEWORD, m_frame); 
}

void CEditorView::OnTextNotFound(LPCSTR lpszFind) 	// overridden to add behaviour
{
	// TODO: Add your command handler code here

	m_textFound = FALSE;
}

void CEditorView::OnFilePrintsetup()
{
	// TODO: Add your command handler code here	LPDEVMODE lpInitData = printDialog.GetDevMode ();	/* device specific initialization data */
	BOOL bPrintSetupOnly = TRUE;
	DWORD dwFlags = 0;
	CWnd* pParentWnd = (CWnd*) m_frame;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	if (theApp.m_editorPrinterData != NULL) {
//    	HGLOBAL hDevMode = GlobalAlloc(GMEM_MOVEABLE, sizeof(DEVMODE));
		printDialog.m_pd.hDevMode = theApp.m_editorPrinterData;
//    	GlobalFree(hDevMode);  
	}
	int result = printDialog.DoModal();
	if (result == IDOK) {
//		theApp.m_editorPrinterData = printDialog.GetDevMode ();
		theApp.m_editorPrinterData = printDialog.m_pd.hDevMode;
	}
}


void CEditorView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	
	CEditView::OnFilePrint();
}

#ifdef SOSTITUITO

// implementazione fatta non conoscendo quella default
void CEditorView::OnFilePrint()
{
	// TODO: Add your command handler code here
	BOOL bPrintSetupOnly = FALSE;
	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CWnd* pParentWnd = (CWnd*) m_frame;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	if (theApp.m_editorPrinterData != NULL)
		printDialog.m_pd.hDevMode = theApp.m_editorPrinterData;
	int result = printDialog.DoModal();
	if (result == IDOK) {
		theApp.m_editorPrinterData = printDialog.m_pd.hDevMode;
//		LPDEVMODE lpInitData = printDialog.GetDevMode ();	/* device specific initialization data */
		CDC printerDC;
		HDC hDC = printDialog.GetPrinterDC ();
		CDC* ppDC = printerDC.FromHandle (hDC);
				
		int rasterCaps = ppDC->GetDeviceCaps(RASTERCAPS);	/* 0x2A19 per EPL-7500 */
		int printerPlanes = ppDC->GetDeviceCaps(PLANES);

		DOCINFO DocInfo; LPDOCINFO lpDocInfo = &DocInfo;
		DocInfo.lpszDocName = "ProvaStampa";
		DocInfo.lpszOutput = NULL;
		DocInfo.cbSize = sizeof(int) + 2 * sizeof(LPCSTR);
 		result = ppDC->StartDoc(lpDocInfo);
 		result = ppDC->StartPage();

		int lineCount = GetEditCtrl().GetLineCount ();
		int nIndex, nChars;
		char lineBuffer [MAX_CHARS+1];
		int y = 0, cy, minHeight = 8;
//		ppDC->SetTextAlign(TA_UPDATECP);
		for (nIndex=0; nIndex<lineCount; nIndex++) {
			nChars = GetEditCtrl().GetLine(nIndex, lineBuffer, MAX_CHARS);
			if (nChars == 0)
				y += max(cy, minHeight);
			else {
				lineBuffer[nChars] = '\0';
				result = ppDC->TextOut(0, y, lineBuffer, nChars);
        		if (! result) MessageBox("NON ha risposto");
				cy = ppDC->GetTextExtent(lineBuffer, nChars).cy;
				y += cy;
			}
		}
		 
 		result = ppDC->EndPage();
 		result = ppDC->EndDoc();
	}
}

#endif // SOSTITUITO

#ifdef HELP

void CEditorView::OnHelpIndex()
{
	// TODO: Add your command handler code here
	
	
HWND hwnd = this->m_hWnd;				/* handle of window requesting help	*/
LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string	*/
UINT fuCommand = HELP_CONTENTS;			/* type of help	*/
DWORD dwData = 0L;						/* additional data	*/
BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CEditorView::OnHelpTopic()
{
	// TODO: Add your command handler code here
	
	
HWND hwnd = this->m_hWnd;				/* handle of window requesting help	*/
LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string	*/
UINT fuCommand = HELP_PARTIALKEY;		/* type of help	*/
DWORD dwData = (DWORD) "";				/* additional data	*/
BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CEditorView::OnHelpEditor()
{
	// TODO: Add your command handler code here
	
#define HID_TURTLE	0x30001
#define HID_EDITOR	0x30002
	
HWND hwnd = this->m_hWnd;				/* handle of window requesting help	*/
LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string	*/
UINT fuCommand = HELP_CONTEXT;			/* type of help	*/
DWORD dwData = HID_EDITOR;				/* additional data	*/
BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

#endif // HELP

void CEditorView::OnSetFont()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	if (m_pFont != NULL)
	   m_pFont->GetObject(sizeof(LOGFONT), &m_logFont);
	else
	   ::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);

	CFontDialog FontDialog(&m_logFont, /*CF_EFFECTS|*/ CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT, NULL, pParentWnd);
	if (FontDialog.DoModal() == IDOK)
	{
		if (m_pFont != NULL) delete(m_pFont);
		m_pFont = new(CFont);
		m_pFont->CreateFontIndirect(&m_logFont);
		m_penColor = FontDialog.GetColor();
	}
	m_pLogFont = &m_logFont;
	SetFont(m_pFont);
}

void CEditorView::OnSetPencolor()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	CColorDialog ColorDialog(m_screenColor, 0, pParentWnd);

	int result = ColorDialog.DoModal();
	if (result == IDOK) {
		m_penColor = ColorDialog.GetColor ();
	}
}

void CEditorView::OnSetScreencolor()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	CColorDialog ColorDialog(m_screenColor, 0, pParentWnd);

	int result = ColorDialog.DoModal();
	if (result == IDOK) {
		m_screenColor = ColorDialog.GetColor ();
	}
}

void PR_CREAFOGLIO ()
{
	node lDevName = arg_1;
	int dev = nome2dev (lDevName);
	if (dev >= MIN_DEV)
		err2 (23, lDevName);		// .. gia' aperto
	else {
		arg_1 = arg_2;
		if (apri_foglio (lDevName) == -1)
			err2 (31, lDevName);	// non riesco ad aprire ..
	}
}

/*ILFOGLIO.C	G. Toffoli

	990111	migliorato gestione foglio selezionato
	981224	controllo sulla chiusura
			da parte di codice eseguito dall'interno
	980828	preparazione per conversione a C++; rivedere nomearc !!!!
	980721	Modificato KwAttr secondo vera sintassi di lista associativa
			Introdotto controllo su esistenza del menu
	980717	Iniziato a usare KwAttr per uso di associazioni in lista argomenti
	980107	Gestione del foglio corrente
			aggiunto openDefaultEditor; modif. sel_foglio
	971229	Introdotto ini_foglio
	971217	Modificato apri_foglio
	970904	In apri_foglio/chiudi_foglio aggiunta/tolta voce a menu WINDOWS
	970618	prima versione sotto Windows
*/

void EditorCheck (int dev)
{
	char name[MAXCHARS];
	for (dev = 0; dev <= MAX_DEV; ++dev)
		if ((_file [dev] != -1) && (_fstato [dev] & O_FOGLIO)) {
			pacstr (name, nome (_nome [dev]));
			MainMenuCheckWindowItem (1, name, (dev == dev_editor));	
			EditorMenuCheckSelect (_file[dev], (dev == dev_editor));
			EditorMenuActivateSelect (_file[dev], (dev == dev_editor));
		};
}

/*------------------------------------------------------
  attiva nuovo foglio senza cancellare quello precedente
  ------------------------------------------------------*/
void sel_foglio (/*nview*/ int dev) {
	char name[MAXCHARS];

	dev_editor = dev;
	pacstr (name, nome (_nome [dev]));
	MainMenuShowWindow (name);
	EditorCheck (dev);	/* opera in menu Main-Finestre */
}

/* ---------------------------------------------------------------------------
  apre una finestra tipo FOGLIO con il nome specificato;
  se non ci riesce ritorna -1, altrimenti ritorna l' indice i dopo aver
  assegnato a _file [i] l' handle di tarta e a _nome [i] il nome
--------------------------------------------------------------------------- */

int apri_foglio (node lnome)
{
	int i, j;
	node title;
	node menu; int is_menu = -1;		// 980721
	char nameString [MAXCHARS], titleString [MAXCHARS], *pTitle;
	node pos, dim;
	int x1, y1;
	int dimx, dimy;

// #ifdef INTEGRA
	node accapo = KwAttr (lnome, arg_1, pc_ACCAPO);
	if (accapo)
		bWordWrapping = MatchKw (car (accapo), pc_VERO);
// #endif // INTEGRA

	menu = KwAttr (lnome, arg_1, pc_MENU);
	if (menu)
		is_menu = MatchKw (menu = car (menu), pc_VERO);

	title = KwAttr (lnome, arg_1, pc_TITLE);
	if ((title) && (MatchKw (title = car (title), pc_FALSO)))
		pTitle = (char *) 0L;
	else {
		if (! title)
			title = lnome;
		if (typ (title) == LIST) MemScrivi (titleString, title);
		else getnam (titleString, title);
		pTitle = &titleString[0];
	}
	if ((i = nuovo_dev ()) == 0)		/* cerca posto per nuovo device */
		return -1;
	if (_file [i] == -1) {
		pos = KwAttr (lnome, arg_1, pc_POS);
		if (! pos) {
			x1 = -1; y1 = -1;
		} else {
			x1 = rnum_int (car (pos));
			y1 = rnum_int (car (cdr (pos)));
		};

		dim = KwAttr (lnome, arg_1, pc_DIM);
		if (! dim) {
			dimx = -1; dimy = -1;
		} else {
			dimx = rnum_int (car (dim));
			dimy = rnum_int (car (cdr (dim)));
		};

		j = EditorOpenView (x1, y1, dimx, dimy, pTitle, is_menu);	//
		_file [i] = j;                  	/* handle del device */
	}

	_nome [i] = lnome;              	/* nome del device */
	_fstato [i] = O_FINESTRA | O_FOGLIO;/* stato del device */

	pacstr (nameString, nome (lnome));		/* aggiunge voce a menu WINDOWS */
	MainMenuAppendWindowItem (nameString);

	dev_editor = i;	// NOTA: c'e' qualche ridondanza
	EditorCheck (i);
	++newEditorCounter;
	return i;                       /* riporta indice device */
}

int chiudi_foglio (int dev)
{
	int result = EditorCloseView (_file [dev]);
	if (result) {
		_file [dev] = -1;
		char nameString[MAXCHARS];
		pacstr (nameString, nome (_nome [dev]));
		MainMenuRemoveWindowItem (nameString);

		if (dev == dev_leggi) dev_leggi = dev_in = CONSOLE;
		else if (dev == dev_scrivi) dev_scrivi = dev_out = CONSOLE;
		dev_editor = -1;
		if ((dev = currentWindow (O_FOGLIO)) != -1)
			sel_foglio (dev);
		return TRUE;
	}
	return result;
}

void ini_foglio ()
{
	arg_1 = NULLP;	// 980717 : per gestione di listra di attributi
	if (DefaultEditor[0] != '\0')
		dev_editor = apri_foglio (mettip (logstr (DefaultEditor)));
}

int openDefaultEditor ()	// spostato qui da MAINFRM.CPP
{
	char sCounter[32] = "";
	char editorName[MAXCHARS];

	if (newEditorCounter > 1)
		sprintf (sCounter, "%d", newEditorCounter);
	if (DefaultEditor[0] != '\0')
		strcpy (editorName, DefaultEditor);
	else
		pacstr (editorName, nome (pc_FOGLIO));
	strcat (editorName, sCounter);
	arg_1 = NULLP;	// 980717 : per gestione di listra di attributi
  	return apri_foglio (mettip (logstr (editorName)));
}

/* -------------------------------------------------
Predicato VERO se (un) FOGLIO e' visibile (esiste ?)
---------------------------------------------------- */
// int currentWindow (int mask);
void PR_FOGLIO_ ()
{
/*>990111
	int dev = currentWindow (O_FOGLIO);
	risultato = logval (dev != -1);
*/
	risultato = logval (dev_editor != -1);
/*<*/
}

/*>990111
void cance_foglio ()
{
	int dev = currentWindow (O_FOGLIO);
	if (dev == -1)
		err1 (49);
	else
		EditorClear (_file [dev]);	
}
<*/

void PR_CANCEFOGLIO ()
{
/*>990111
	cance_foglio ();
*/
	if (dev_editor == -1)
		err1 (49);
	else
		EditorClear (_file [dev_editor]);
/*<*/
}

void PR_FOGLIO ()
{
	if (dev_editor == -1)
		risultato = NULLP;
	else
		risultato = dev2nome (dev_editor);
}

void PR_RECUFOGLIO ()
{
	if (dev_editor == -1) {
	}
	else {
//>990922 : estendere con gestione lista di file
		if (tipo_1 == LIST) {
			err2 (5, arg_1);
			return;
		}
//<
		char fileName[MAX_CHARS];
		getnam (fileName, arg_1);
		if (! EditorLoad (_file [dev_editor], fileName))
			err2 (31, arg_1);
	}
}

void PR_CONSFOGLIO ()
{
	if (dev_editor == -1) {
	}
	else {
		char fileName[MAX_CHARS];
		getnam (fileName, arg_1);
		if (! EditorSave (_file [dev_editor], fileName))
			err2 (31, arg_1);
	}
}

void PR_CHIUDIFOGLIO ()
{
	if (dev_editor == -1) {
	}
	else
		chiudi_foglio (dev_editor);
}

void CEditorView::OnSelectEditor() 
{
	// TODO: Add your command handler code here
	int handle = theApp.GetEditorViewIndex (this);
	int dev = handle2dev (handle, O_FOGLIO);
	sel_foglio (dev);
}

void FOGLIOIMPOSTASTAMPANTE ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->OnFilePrintsetup();
}

void FOGLIOSTAMPA ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->OnFilePrint();
}

void FOGLIOTAGLIA ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->GetEditCtrl().Cut();
}

void FOGLIOCOPIA ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->GetEditCtrl().Copy();
}

void FOGLIOINCOLLA ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->GetEditCtrl().Paste();
}

void FOGLIOIMPOSTAFONT ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->OnSetFont();
}

// essendo una primitiva, si deve comportare come RECUPERA
void FOGLIOESEGUISELEZIONE ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
//>990523
//	editor->OnTestSelection();
	editor->ResetReadSel ();
	dev_temp = view_to_devfoglio (editor->m_self);
	is_recupera = TRUE;
//<
}

// essendo una primitiva, si deve comportare come RECUPERA
void FOGLIOESEGUITUTTO ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
//>990523
//	editor->OnTestAll();
	editor->SetReadLine (0);	// aggiunto 990615
	editor->m_nStartChar = editor->m_nLastChar = editor->m_nNextChar = -1;
	dev_temp = view_to_devfoglio (editor->m_self);
	is_recupera = TRUE;
//<
}

void RECUPERA ();
void PR_EXECFOGLIO ()
{
	if (dev_editor == -1) {
	}
	else {
//>		va in errore:
//		EditorExec (_file [dev_editor]);
		arg_1 = dev2nome (dev_editor);
		tipo_1 = LWORD;
		RECUPERA ();
//<
	}
}

void FOGLIOSELEZIONABLOCCO ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0))
		(editor->GetEditCtrl()).SetSel (iBegin, iEnd+1, FALSE);
	else err1 (28);
}
 
/***********************************************************************
  			COMANDI RELATIVI AL CURSORE
  ***********************************************************************/

/*---------------------------------------------------------------
  posiziona il "cursore" del buffer e quello di schermo a seconda 
  dell' intero LOGO passato come argomento 
  ---------------------------------------------------------------*/
void VAICURSORE ()
{
#ifdef IL89
  int n;
  if ((n = rnum_int (arg_1)) > i_foglio) n = i_foglio;
  vaiveloce (n);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int n = (int) editor->GetBufferLength();
	int i = rnum_int (arg_1);
	if (i >= n) i = n;
//	editor->m_iChar = i;
//	(editor->GetEditCtrl()).SetSel (i, i, FALSE);
	SetIChar(i);
#endif // IL89
}
 
/*-----------------------------------------------------------
  ritorna l' indirizzo di un intero LOGO contenente il valore
  del "cursore" di buffer
  -----------------------------------------------------------*/
void DOVECURSORE ()
{
#ifdef IL89
  risultato = int_num (j_foglio);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	risultato = int_num (GetIChar);
#endif // IL89
}
 
 
/***********************************************************************
  			COMANDI RELATIVI AI CARATTERI
  ***********************************************************************/
 
/*--------------------------------------------------------
  sposta di una posizione indietro il "cursore" del buffer
  e quello di schermo
  --------------------------------------------------------*/
void INDCARATTERE ()
{
#ifdef IL89
  if (j_foglio > 0) {
    quadro (nq_foglio, VAI_C, --j_foglio);
    _commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	if (i > 0) --i;
	SetIChar(i);
#endif // IL89
}
 
/*---------------------------------------------------------
  sposta di una posizione in avanti il "cursore" del buffer
  e quello di schermo
  ---------------------------------------------------------*/
void AVANTICARATTERE ()
{
#ifdef IL89
  if (j_foglio < i_foglio) {
    quadro (nq_foglio, VAI_C, ++j_foglio);
    _commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i <= n) ++i;
	editor->m_iChar = i;
	(editor->GetEditCtrl()).SetSel (i, i, FALSE);
#endif // IL89
}
 
/*------------------------------------------------------------------------
  ritorna l' indirizzo della stringa LOGO contenente il carattere corrente
  ------------------------------------------------------------------------*/
void DAMMICARATTERE ()
{
#ifdef IL89
  risultato = scrives_L (j_foglio, j_foglio + 1);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i > n)
		risultato = S_VUOTA;
	else {
		CString text;
		int startChar, lastChar;
		(editor->GetEditCtrl()).GetSel (startChar, lastChar);
		(editor->GetEditCtrl()).SetSel (i, i+1, FALSE);
		editor->GetSelectedText(text);
		(editor->GetEditCtrl()).SetSel (startChar, lastChar, FALSE);
		risultato = logstr (text);
	}
#endif // IL89
}
 
/*---------------------------------------------------------------------
  prende il primo carattere della stringa LOGO, passata come argomento,
  e lo scrive nel buffer
  ---------------------------------------------------------------------*/
void METTICARATTERE ()
{
#ifdef IL89
  node arg;
  if ((arg = nome (arg_1)) != S_VUOTA) {
    irdstr (arg);
    _metti_ca (rdstr (), pq_foglio->a_default);
  }
#else // IL89
	node stringa;
	if ((stringa = nome (arg_1)) != S_VUOTA) {
		char text [MAXCHARS];
		pacstr (text, stringa);
		text[1] = NULLC;
  		CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
		int i = editor->m_iChar;
		(editor->GetEditCtrl()).SetSel (i, i, FALSE);
		(editor->GetEditCtrl()).ReplaceSel (text);
		++i;
		SetIChar(i);
	}
#endif // IL89
}
 
/*----------------------------------------------------------------------------
  sostituisce il carattere corrente con il primo carattere della stringa LOGO,
  passata come argomento
  ---------------------------------------------------------------------------*/
void MODCARATTERE ()
{
#ifdef IL89
  node arg;
  if ((arg = nome (arg_1)) != S_VUOTA) {
    irdstr (arg);
    _mod_ca (rdstr (), *(a_foglio + j_foglio));
  }
#else // IL89
	node stringa;
	if ((stringa = nome (arg_1)) != S_VUOTA) {
		char text [MAXCHARS];
		pacstr (text, stringa);
		text[1] = NULLC;
  		CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
		int i = GetIChar;
		(editor->GetEditCtrl()).SetSel (i, i+1, FALSE);
		(editor->GetEditCtrl()).ReplaceSel (text);
		SetIChar(i);
	}
#endif // IL89
}
 
/*---------------------------------------------------------
  cancella il carattere corrente dal buffer e dallo shermo,
  cancella, anche, l ' attributo relativo
  ---------------------------------------------------------*/
void CANCECARATTERE ()
{
#ifdef IL89
  char c_cance;
  if (j_foglio < i_foglio) {
    c_cance = *(c_foglio + j_foglio);
    n_c = 1;
    fine = j_foglio;
    mod_buf0 ();		/* aggiorna i buffers e le marche del blocco */
    quadro (nq_foglio, DEL_AC, c_cance);
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == n) return;
	(editor->GetEditCtrl()).SetSel (i, i+1, FALSE);
	(editor->GetEditCtrl()).ReplaceSel ("");
#endif // IL89
}

void CONTACARATTERI ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int n = (int) editor->GetBufferLength();
	risultato = int_num (n);
}

void CONTALINEE ()
{
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int n = (int) (editor->GetEditCtrl()).GetLineCount();
	risultato = int_num (n);
}


/***********************************************************************
  			COMANDI RELATIVI ALLE PAROLE
  ***********************************************************************/
 
/*--------------------------------------------------
  ritorna l' indice del primo carattere della parola
  --------------------------------------------------*/
#ifdef IL89
int ini_p (i)
int i;
{
  int c;
  if ((c = *(c_foglio + i)) == ' ')	/* sequenza di spazi */
    while ((i > 0) && (*(c_foglio + (i-1)) == ' ')) --i;
  else if (! IS_DEL_PAROLA(c))		/* sequenza di caratteri qualsiasi */
    while ((i > 0) && (! IS_DEL_PAROLA(*(c_foglio + (i-1))))) --i;
  return i;
}
#else // IL89
int ini_parola (int i, const char* pBuf)
{
  int c;
  if ((c = *(pBuf + i)) == SPACE)	/* sequenza di spazi */
    while ((i > 0) && (*(pBuf + (i-1)) == SPACE)) --i;
  else if (! IS_DELIMITATORE(c))		/* sequenza di caratteri qualsiasi */
    while ((i > 0) && (! IS_DELIMITATORE(*(pBuf + (i-1))))) --i;
  return i;
}
#endif // IL89
 
/*---------------------------------------------------------
  ritorna l' indice dell' ultimo carattere della parola + 1
  ---------------------------------------------------------*/
#ifdef IL89
int fine_p (i)
int i;
{
  int c;
  if ((c = *(c_foglio + i)) == ' ')		/* sequenza di spazi */
    while (*(c_foglio + i) == ' ') ++i;
  else if (IS_DEL_PAROLA(c)) ++i;		/* LINEDEL e assimilati */
  else while (! IS_DEL_PAROLA(*(c_foglio + i))) ++i;/* parola */
  return i;
}
#else // IL89
int fine_parola (int i, const char* pBuf)
{
	int c;
	if ((c = *(pBuf + i)) == SPACE)			/* sequenza di spazi */
		while (*(pBuf + i) == SPACE) ++i;
	else if (IS_DELIMITATORE(c)) ++i;			/* LINEDEL e assimilati */
	else while (! IS_DELIMITATORE(*(pBuf + i))) ++i;	/* parola */
	return i;
}
#endif // IL89

/*---------------------------------------------------------------
  sposta il "cursore" del buffer sul primo carattere della parola 
  corrente e sincronizza il cursore di schermo.
  NOTARE: una sequenza di spazi bianchi e' considerata parola 
  ---------------------------------------------------------------*/
void AINIZIOPAROLA ()
{
#ifdef IL89
  if ((j_foglio > 0) && (j_foglio < i_foglio)) {
    quadro (nq_foglio, VAI_C, (j_foglio = ini_p (j_foglio)));
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == 0) return;
	const char* lpBuffer = editor->LockBuffer();
	i = ini_parola (i, lpBuffer);
	editor->UnlockBuffer();
	SetIChar(i);
#endif // IL89
}
 
/*---------------------------------------------------------------
  sposta il "cursore" del buffer sul primo carattere della parola
  che segue quella corrente e sincronizza il cursore di schermo.
  NOTARE: una sequenza di spazi bianchi e' considerata parola 
  ---------------------------------------------------------------*/
void AFINEPAROLA ()
{
#ifdef IL89
  if (j_foglio < i_foglio) {
    quadro (nq_foglio, VAI_C, (j_foglio = fine_p (j_foglio)));
    if (IS_DEL_LINEA(*(c_foglio + (j_foglio - 1)))) ++cont_l; 
    commento ();
 }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == n) return;
	const char* lpBuffer = editor->LockBuffer();
	i = fine_parola (i, lpBuffer);
	editor->UnlockBuffer();
	SetIChar(i);
#endif // IL89
}
 
/*---------------------------------------------------------------
  sposta il "cursore" del buffer sul primo carattere della parola
  che precede quella corrente e sincronizza il cursore di schermo 
  ---------------------------------------------------------------*/
void INDPAROLA ()
{
#ifdef IL89
  if (j_foglio > 0) {
    if ((j_foglio == i_foglio) || ((j_foglio = ini_p (j_foglio)) > 0)) {
      if ((j_foglio = ini_p (--j_foglio)) > 0)
        if (*(c_foglio + j_foglio) == SPACE) j_foglio = ini_p (--j_foglio);
    }
    quadro (nq_foglio, VAI_C, j_foglio);
    _commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == 0) return;
	const char* lpBuffer = editor->LockBuffer();
	if ((i == n) || ((i = ini_parola (i, lpBuffer)) > 0)) {
		if ((i = ini_parola (--i, lpBuffer)) > 0)
			if (*(lpBuffer + i) == SPACE) i = ini_parola (--i, lpBuffer);
	}
	editor->UnlockBuffer();
	SetIChar(i);
#endif // IL89
}
 
/*----------------------------------------------------------------
  sposta il "cursore" di buffer sul primo carattere della parola 
  che segue la parola corrente e sincronizza il cursore di schermo 
  ----------------------------------------------------------------*/
void AVANTIPAROLA ()
{
#ifdef IL89
  if (j_foglio < i_foglio) {
    if (*(c_foglio + (j_foglio = fine_p (j_foglio))) == SPACE)
      j_foglio = fine_p (j_foglio);
    quadro (nq_foglio, VAI_C, j_foglio);
    _commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == n) return;
	const char* lpBuffer = editor->LockBuffer();
    if (*(lpBuffer + (i = fine_parola (i, lpBuffer))) == SPACE)
		i = fine_parola (i, lpBuffer);
	editor->UnlockBuffer();
	SetIChar(i);
#endif // IL89
}
 
/*---------------------------------------------------------------------
  ritorna l' indirizzo della stringa LOGO contenente la parola corrente
  ---------------------------------------------------------------------*/
void DAMMIPAROLA ()
{
#ifdef IL89
  risultato = scrives_L (ini_p (j_foglio), fine_p (j_foglio));
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == n) return;
	const char* lpBuffer = editor->LockBuffer();
	int beginWord = ini_parola (i, lpBuffer);
	int endWord = fine_parola (i, lpBuffer);
	int lWord = endWord - beginWord;
	if (lWord == 0)
		risultato = S_VUOTA;
	else {
		CString text (lpBuffer+beginWord, lWord);
		risultato = logstr (text);
	}
	editor->UnlockBuffer();
#endif // IL89
}
 
/*--------------------------------------------------------------
  inserisce nel buffer, a partire dalla posizione del "cursore", 
  la parola LOGO, passata come argomento e la visualizza
  --------------------------------------------------------------*/
void METTIPAROLA ()
{
#ifdef IL89
  int locale, i;
  node arg;
  if ((arg = nome (arg_1)) != S_VUOTA) {
    n_c = wrdlen (arg);
    if (n_c <= ((d_foglio-1) - i_foglio)) { 
      if (j_foglio < i_foglio) mod_buf1 ();
      i_foglio += n_c; 
      locale = j_foglio;
      irdstr (arg);
      for (i = 1; i <= n_c; i++) *(c_foglio + (locale++)) = rdstr ();
      memset (a_foglio+j_foglio, pq_foglio->a_default, n_c);
      quadro (nq_foglio, MOD_Q, j_foglio);
    } 
    else
      msg_com (12);
  }
#else // IL89
	node stringa;
	if ((stringa = nome (arg_1)) != S_VUOTA) {
		char text[MAXI_SIZE];
		pacstr (text, stringa);
	  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
		int i = GetIChar;
		(editor->GetEditCtrl()).SetSel (i, i, FALSE);
		(editor->GetEditCtrl()).ReplaceSel (text);
		i += wrdlen (stringa);		// ??
		SetIChar(i);
	}
#endif // IL89
}
 
/*------------------------------------------------------------------
  cancella un' intera parola + gli spazi che la seguono dal buffer e
  dallo schermo, cancella, anche, i relativi attributi
  ------------------------------------------------------------------*/ 
void CANCEPAROLA ()
{
#ifdef IL89
  int locale;
  if (j_foglio < i_foglio) {
    j_foglio = ini_p (j_foglio);
    if (*(c_foglio+(fine = fine_p (j_foglio))) == ' ') fine = fine_p (fine);
    locale = *(c_foglio + j_foglio);
    n_c = fine - j_foglio;
    quadro (nq_foglio, VAI_C, j_foglio);
    mod_buf0 (); 
    if (locale == LINEDEL) quadro (nq_foglio, DEL_AC, LINEDEL);
    else quadro (nq_foglio, MOD_L, j_foglio);
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	int n = (int) editor->GetBufferLength();
	if (i == n) return;
	const char* lpBuffer = editor->LockBuffer();
	int beginWord = ini_parola (i, lpBuffer);
	int endWord = fine_parola (i, lpBuffer);
	editor->UnlockBuffer();
	int lWord = endWord - beginWord;
	if (lWord != 0) {
		(editor->GetEditCtrl()).SetSel (beginWord, endWord, FALSE);
		(editor->GetEditCtrl()).ReplaceSel ("");
		i = beginWord;
		SetIChar(i);
	}
#endif // IL89
}
 
 
/***********************************************************************
  			COMANDI RELATIVI ALLE LINEE
  ***********************************************************************/
 
/*-------------------------------------------------
  ritorna l' indirizzo dell' intero LOGO contenente
  il numero di linea corrente
  -------------------------------------------------*/
void DOVELINEA ()
{
#ifdef IL89
  risultato = int_num (conta_l ());
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	risultato = int_num (n);
#endif // IL89
}
 
/*----------------------------------------------------------
  porta il cursore sul primo carattere della linea richiesta
  ----------------------------------------------------------*/
void VAILINEA ()
{
#ifdef IL89
  int n, ult_l, locale, ind_l;
  n = rnum_int (arg_1);
  if (n < 0) n = 0;
  else {
    j_foglio = i_foglio;
    if ((ult_l = conta_l ()) < n) n = ult_l;
  }
  j_foglio = 0;
  locale = 0;
  while (locale < n) {
    while (*(c_foglio + j_foglio) != LINEDEL) ++j_foglio;
    ++j_foglio; ++locale;
  }
  vaiveloce (ind_l = j_foglio);
#else // IL89
	int n = rnum_int (arg_1);
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = (editor->GetEditCtrl()).LineIndex (n);
	SetIChar(i);
#endif // IL89
}
 
/*----------------------------------------------------------------------
  porta il "cursore" del buffer sul primo carattere della linea corrente
  e sincronizza il cursore di schermo 
  ----------------------------------------------------------------------*/
void AINIZIOLINEA ()
{
#ifdef IL89
  if ((j_foglio > 0) && (! IS_DEL_LINEA(*(c_foglio + (j_foglio - 1))))) {
    quadro (nq_foglio, VAI_C, (j_foglio = ini_l ()));
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	i = (editor->GetEditCtrl()).LineIndex (n);
#endif // IL89
}
 
/*-------------------------------------------------------------------------
  porta il "cursore" del buffer sull' ultimo carattere della linea corrente
  (terminatore di linea o di testo) e sincronizza il cursore di schermo
  -------------------------------------------------------------------------*/
void AFINELINEA ()
{
#ifdef IL89
  if ((j_foglio < i_foglio) && (! IS_DEL_LINEA(*(c_foglio + j_foglio)))) {
    quadro (nq_foglio, VAI_C, (j_foglio = fine_l ()));
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	i = (editor->GetEditCtrl()).LineIndex (n + 1);
	if (i == -1)
		i = editor->GetBufferLength ();
	else
		i -= 2;
	SetIChar(i);
#endif // IL89
} 
 
/*--------------------------------------------------------------------
  sposta indietro il "cursore" del buffer:
  se si trova sul primo carattere della linea lo porta all' inizio 
  della linea precedente, altrimenti all' inizio della linea corrente;
  sincronizza il cursore di schermo
  --------------------------------------------------------------------*/
void INDLINEA ()
{
#ifdef IL89
  if (j_foglio > 0) {
    if (IS_DEL_LINEA(*(c_foglio + (j_foglio - 1)))) {
      --j_foglio; --cont_l; 
    }
    quadro (nq_foglio, VAI_C, (j_foglio = ini_l ()));
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	i = (editor->GetEditCtrl()).LineIndex (n);
	if ((i == GetIChar) && (n > 0))
		i = (editor->GetEditCtrl()).LineIndex (n - 1);
	editor->m_iChar = i;
	(editor->GetEditCtrl()).SetSel (i, i, editor->m_isUpdate);
#endif // IL89
}
 
/*---------------------------------------------------------------------------
  porta il "cursore" del buffer sul 1" carattere dopo il terminatore di linea
  corrente o sul terminatore di testo e sincronizza il cursore di schermo
  ---------------------------------------------------------------------------*/
void AVANTILINEA ()
{
#ifdef IL89
  if (j_foglio < i_foglio) {
    if ((j_foglio = fine_l ()) < i_foglio) {
      ++j_foglio; ++cont_l;
    }
    quadro (nq_foglio, VAI_C, j_foglio);
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	if (n < (editor->GetEditCtrl()).GetLineCount())
		++n;
	i = (editor->GetEditCtrl()).LineIndex (n);
	SetIChar(i);
#endif // IL89
}
 
/*--------------------------------------------------------------------
  ritorna l' indirizzo della stringa LOGO contenente la linea corrente
  --------------------------------------------------------------------*/
void DAMMILINEA ()
{
#ifdef IL89
  if ((j_foglio == i_foglio) && (IS_DEL_LINEA(*(c_foglio + (j_foglio - 1))))) ;
  else risultato = scrives_L (ini_l (), fine_l () + 1);
#else // IL89
	char text[MAXCHARS+1];
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	int l = (editor->GetEditCtrl()).GetLine (n, text, MAXCHARS);
	text[l] = NULLC;
	risultato = logstr (text);
	SetIChar(i);
#endif // IL89
}
 
/*------------------------------------------------------------------
  crea una linea nuova o spezza una linea gia' esistente scrivendo
  il terminatore di linea nel buffer e  lo visualizza.
  ATTENZIONE: non viene alterata la posizione dei rispettivi cursori
  ------------------------------------------------------------------*/
void METTILINEA ()
{
#ifdef IL89
  if (i_foglio < (d_foglio-1)) {
    if (j_foglio < i_foglio) {
      n_c = 1;
      mod_buf1 ();
    }
    *(c_foglio + j_foglio) = LINEDEL;
    ++i_foglio;
    quadro (nq_foglio, INS_L, ++j_foglio);
    quadro (nq_foglio, VAI_C, --j_foglio);
  } 
  else _beep ();  	/* ex msg_com (11) */ 
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	(editor->GetEditCtrl()).SetSel (i, i, FALSE);
	(editor->GetEditCtrl()).ReplaceSel ("\r\n");
	SetIChar(i);
#endif // IL89
}
 
/*-----------------------------------------------------------------
  cancella l' intera linea (compreso terminatore di linea) corrente
  dal buffer e dallo schermo, cancella i relativi attributi
  -----------------------------------------------------------------*/ 
void CANCELINEA ()
{
#ifdef IL89
  if ((j_foglio = ini_l ()) < i_foglio) {
    fine = fine_l ();
    n_c = fine - j_foglio;
    if (fine < i_foglio) ++n_c;
    mod_buf0 ();
    quadro (nq_foglio, DEL_L, j_foglio);
    commento ();
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	i = (editor->GetEditCtrl()).LineIndex (n);
	int i2 = (editor->GetEditCtrl()).LineIndex (n + 1);
	if (i2 == -1)
		i2 = editor->GetBufferLength ();
	if (i2 != i) {
		(editor->GetEditCtrl()).SetSel (i, i2, FALSE);
		(editor->GetEditCtrl()).ReplaceSel ("");
	}
	SetIChar(i);
#endif // IL89
}
 
/*-------------------------------------------------------
  cancella il contenuto della linea corrente (terminatore 
  di linea escluso) a partire dalla posizione del cursore
  -------------------------------------------------------*/
void TRONCALINEA ()
{
#ifdef IL89
  if (j_foglio < i_foglio) {
    fine = fine_l ();
    n_c = fine - j_foglio;
    mod_buf0 ();
    quadro (nq_foglio, MOD_L, j_foglio);
  }
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = GetIChar;
	int n = (editor->GetEditCtrl()).LineFromChar (i);
	int i2 = (editor->GetEditCtrl()).LineIndex (n + 1);
	if (i2 == -1)
		i2 = editor->GetBufferLength ();
	else
		--i2;
	if (i2 != i) {
		(editor->GetEditCtrl()).SetSel (i, i2, FALSE);
		(editor->GetEditCtrl()).ReplaceSel ("");
	}
	SetIChar (i);
#endif // IL89
}
 
/***********************************************************************
  			COMANDI RELATIVI AI BLOCCHI
  ***********************************************************************/
 
/*-----------------------------------------------------------------------
  assegna alla variabile i_ini_b l' indice del primo carattere del blocco
  -----------------------------------------------------------------------*/
void QUIINIZIOBLOCCO ()
{
#ifdef IL89
  i_ini_b = j_foglio;
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	editor->m_iBeginBlock = i;
#endif // IL89
}
 
/*--------------------------------------------------------------------------
  assegna alla variabile i_fin_b l' indice dell' ultimo carattere del blocco 
  --------------------------------------------------------------------------*/
void QUIFINEBLOCCO ()
{
#ifdef IL89
  if (j_foglio > 0) i_fin_b = j_foglio - 1;
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int i = editor->m_iChar;
	if (i > 0)
		editor->m_iEndBlock = i - 1;
#endif // IL89
}
 
/*--------------------------------------------------------------
  sposta il "cursore" di buffer sul primo carattere del blocco e
  sincronizza il cursore di schermo 
  --------------------------------------------------------------*/
void AINIZIOBLOCCO ()
{
#ifdef IL89
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b)) {
    vaiveloce (i_ini_b);
  }
  else err (28);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		int i = iBegin;
		SetIChar(i);
	}
	else err1 (28);
#endif // IL89
}
 
/*-----------------------------------------------------------------------
  sposta il "cursore" di buffer sul primo carattere che segue il blocco e
  sincronizza il cursore di schermo 
  -----------------------------------------------------------------------*/
void AFINEBLOCCO ()
{
#ifdef IL89
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b))
    vaiveloce (i_fin_b+1);
  else err (28);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		int i = iEnd + 1;
		SetIChar(i);
	}
	else err1 (28);
#endif // IL89
}
 
/*---------------------------------------------------------------------
  ritorna l' indirizzo della stringa LOGO contenente il blocco corrente
  ---------------------------------------------------------------------*/
void DAMMIBLOCCO ()
{
#ifdef IL89
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b))
    risultato = scrives_L (i_ini_b, i_fin_b + 1);
  else err (28);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		const char* lpBuffer = editor->LockBuffer();
		CString text (lpBuffer + iBegin, lBlock);
		risultato = logstr (text);
		editor->UnlockBuffer();
	}
	else err1 (28);
#endif // IL89
}
 
/*---------------------------------------------------------------------------
  cancella il blocco corrente ed i relativi attributi dai rispettivi buffers;
  cancella la parte di blocco presente sullo schermo
  ---------------------------------------------------------------------------*/
void CANCEBLOCCO ()
{
#ifdef IL89
  int inizio;
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b)) {
    n_c = i_fin_b - i_ini_b + 1;
/* modifica dei buffers */
    elimina (i_ini_b);
/* modifica della finestra di schermo */
    if ((pq_foglio->offfin - 1) >= i_ini_b) {
      if (pq_foglio->offini > i_fin_b) 
        inizio = pq_foglio->offini - n_c;
      else
        inizio = i_ini_b; 
      if (pq_foglio->offini > i_ini_b)
        ric_quadro (inizio);
      else {
        quadro (nq_foglio, VAI_C, inizio); 
        quadro (nq_foglio, MOD_Q, inizio);
      }
      if (j_foglio < i_ini_b)
        quadro (nq_foglio, VAI_C, j_foglio);
      else
        if (j_foglio > i_fin_b) 
          quadro (nq_foglio, VAI_C, j_foglio -= n_c);
        else
          j_foglio = inizio;
    }
   i_ini_b = i_fin_b = -1;	/* marche per blocco non esistente */
    _commento ();
  }
  else err (28);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		(editor->GetEditCtrl()).SetSel (iBegin, iEnd+1, FALSE);
		(editor->GetEditCtrl()).ReplaceSel ("");
		int i = editor->m_iChar;
		if (i > iEnd)
			i -= lBlock;
		else if (i > iBegin)
			i = iBegin;
		SetIChar(i);
	}
	else err1 (28);
#endif // IL89
}
 
   
/*---------------------------------------------------------------
  riscrive il blocco nel buffer partendo dalla posizione corrente
  del "cursore", lo cancella dalla zona precedentemente occupata
  e aggiorna la versione su schermo
  ---------------------------------------------------------------*/
void SPOSTABLOCCO ()
{
#ifdef IL89
  int locale, spazio, n_gruppi, i, inizio;
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b)) {
    if (i_foglio < (d_foglio - 1)) {
/* spostamento fisico del blocco nel buffer e sullo schermo */
      if ((j_foglio < i_ini_b) || (j_foglio > i_fin_b)) 
        if ((n_c = i_fin_b - i_ini_b + 1) <= (spazio = (d_foglio-1)-i_foglio))
          _sposta ();	/* num car blocco <= num car vuoti nel buffer */
        else {		/* num car blocco > num car vuoti nel buffer */
          locale = n_c;		/* num car blocco */
          n_c = spazio;		/* num car vuoti nel buffer */
          n_gruppi = locale / n_c;
          for (i = 1; i <= n_gruppi; i++) _sposta ();
          if ((n_c = locale % n_c) != 0) _sposta ();
          n_c = locale;
        }
        if ((pq_foglio->offfin - 1) < i_ini_b)
          quadro (nq_foglio, MOD_Q, (j_foglio -=n_c));
        else {
          if (pq_foglio->offini > i_ini_b) {
            if (pq_foglio->offini > i_fin_b) {
              inizio = pq_foglio->offini - n_c;
              j_foglio -= n_c;
            }
            else {
              inizio = i_ini_b;
              if (j_foglio > i_fin_b) j_foglio -= n_c; else j_foglio = i_ini_b;
            }
            ric_quadro (inizio);
            quadro (nq_foglio, VAI_C, j_foglio);
          }
          else
            if (j_foglio < i_ini_b)
              quadro (nq_foglio, MOD_Q, (j_foglio -= n_c));
            else
              if (j_foglio > i_fin_b) {
                quadro (nq_foglio, VAI_C, i_ini_b);
                quadro (nq_foglio, MOD_Q, i_ini_b);
                quadro (nq_foglio, VAI_C, (j_foglio -= n_c));
              }
              else
                quadro (nq_foglio, VAI_C, j_foglio = i_ini_b);
        }
/*aggiornamento marche blocco */
        i_ini_b = j_foglio;
        i_fin_b = j_foglio + n_c - 1;
    }
    else {msg_com (12); return;};
  }
  else {err (28); return;};
  _commento ();
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		int i = GetIChar;
		if ((i >= iBegin) && (i <= (iEnd+1))) return;
		(editor->GetEditCtrl()).SetSel (iBegin, iEnd+1, FALSE);
		CString text;
		editor->GetSelectedText(text);
		(editor->GetEditCtrl()).ReplaceSel ("");
		if (i > iEnd)
			i -= lBlock;
		(editor->GetEditCtrl()).SetSel (i, i, FALSE);
		(editor->GetEditCtrl()).ReplaceSel (text);
		editor->m_iBeginBlock = i;
		editor->m_iEndBlock = i + lBlock - 1;
		SetIChar(i);
	}
	else err1 (28);
#endif // IL89
}

 
/*----------------------------------------------------------------------------
  riscrive il blocco corrente partendo dalla posizione corrente del "cursore"
  e aggiorna la versione su schermo
  ---------------------------------------------------------------------------*/
void COPIABLOCCO ()
{
#ifdef IL89
  if ((i_ini_b != -1) && (i_fin_b != -1) && (i_ini_b <= i_fin_b))
    if ((n_c = (i_fin_b - i_ini_b + 1)) <= ((d_foglio-1) - i_foglio)) {
/* duplicazione del blocco nel buffer e sullo schermo */
      riscrive ();
      quadro (nq_foglio, MOD_Q, j_foglio);
/* aggiornamento marche blocco */
      i_ini_b = j_foglio;
      i_fin_b = j_foglio + n_c - 1;
      }
    else msg_com (12);
 else err (28);
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int iBegin = editor->m_iBeginBlock;
	int iEnd = editor->m_iEndBlock;
	int lBlock = iEnd + 1 - iBegin;
	if ((iBegin != -1) && (iEnd != -1) && (lBlock > 0)) {
		int i = GetIChar;
		(editor->GetEditCtrl()).SetSel (iBegin, iEnd+1, FALSE);
		CString text;
		editor->GetSelectedText(text);
////	(editor->GetEditCtrl()).ReplaceSel ("");
		(editor->GetEditCtrl()).SetSel (i, i, FALSE);
		(editor->GetEditCtrl()).ReplaceSel (text);
		if (i <= iBegin)
			editor->m_iBeginBlock += lBlock;
		if (i <= iEnd)
			editor->m_iEndBlock += lBlock;
	}
	else err1 (28);
#endif // IL89
}
 
 
/***********************************************************************
  			COMANDI RELATIVI AL FOGLIO 
  ***********************************************************************/
 
/*-----------------------------------------------------
  sposta il "cursore" di buffer all' inizio del testo e
  sincronizza il cursore di schermo
  -----------------------------------------------------*/
void AINIZIOTESTO ()
{
#ifdef IL89
  j_foglio = 0;
  if (pq_foglio->offini > 0) 
    ric_quadro (j_foglio);
  else
    quadro (nq_foglio, VAI_C, j_foglio);
  cont_l = 0;
  commento ();
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->m_iChar = 0;
	(editor->GetEditCtrl()).SetSel (0, 0, FALSE);
#endif // IL89
}

/*---------------------------------------------------
  sposta il "cursore" di buffer alla fine del testo e
  sincronizza il cursore di schermo
  ---------------------------------------------------*/
void AFINETESTO ()
{
#ifdef IL89
  int num_righe, num_col, cars, inizio, locale;
  if (pq_foglio->offfin < i_foglio) {
    num_righe = 0;
    if (IS_DEL_LINEA(*(c_foglio + ((j_foglio=i_foglio)-1)))) {
      --j_foglio;
      ++num_righe;
    }
    num_col = pq_foglio->fin.rc.c - pq_foglio->ini.rc.c;
    while (num_righe < pq_foglio->dim.r) {
      cars = 0;
      locale = ini_l ();
      num_righe = num_righe+((cars = (fine_l ()-locale)-1)/num_col)+1;
      if (num_righe < pq_foglio->dim.r) inizio = locale;
      j_foglio = --locale;
    }
    ric_quadro (inizio);
  }
  quadro (nq_foglio, VAI_C, (j_foglio = i_foglio));
  _commento ();
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	int n = (int) editor->GetBufferLength();
	editor->m_iChar = n;
	(editor->GetEditCtrl()).SetSel (n, n, FALSE);
#endif // IL89
}

void insmod ()
{
#ifdef IL89
  int locale;
  commento ();
  dev_out = CONSOLE;
  locale = nq_linea;
  nq_linea = Q_COMMENTO;
  quadro (nq_linea, VAI_C, (j_commento = 69));
  msgsenza ((is_ins) ? 16 : 17);
  nq_linea = locale;
  dev_out = dev_scrivi;
#endif // IL89
}
 
/*---------------------------------------------------------
  setta la modalita' di inserimento o modifica di carattere
  ---------------------------------------------------------*/
void ASINS ()
{
#ifdef IL89
  is_ins = bool (arg_1);
  insmod ();
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->m_isUpdate = (arg_1);
#endif // IL89
}


/*---------------------------------------------------------
  attiva/disattiva l' aggiornamento della linea di servizio
  ---------------------------------------------------------*/
void AGGIORNA ()
{
#ifdef IL89
  char valore;
	/* riassegna valore di BIT_SERVIZIO in campo STATO */
  if (pq_foglio->stile & BIT_SERVIZIO) {
    valore = bool (arg_1) ? BIT_SERVIZIO : FALSE;
    copy_bits (&valore, &pq_foglio->stato, (char) BIT_SERVIZIO);
  };
  _commento ();
#else // IL89
  	CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
	editor->m_isUpdate = bool_ (arg_1);
	if (editor->m_isUpdate)
			(editor->GetEditCtrl()).SetSel (editor->m_iChar, editor->m_iChar, FALSE);
#endif // IL89
}

/*-----------------------------------------------------------------
  cerca nel buffer a partire dalla posizione corrente del "cursore" 
  una parola o stringa LOGO passata come arg_1
  -----------------------------------------------------------------*/
void CERCAS ()
{
#ifdef IL89
  node arg;
  int lung, j;
  char  *locale, s_locale [MAXCHARS+1];
  if ((arg = nome (arg_1)) != S_VUOTA)
    if ((lung = pacstr (s_locale, arg) - 1) > MAXCHARS)
      msg (14, arg_1);
    else {
      if ((locale = _cerca (c_foglio+j_foglio, c_foglio+i_foglio, s_locale))
          != NULL) j = locale + lung - c_foglio;
      else j = i_foglio;
      vaiveloce (j);  
    }
#else // IL89
	node stringa;
	int lWord;
	char FindString [MAXCHARS+1];
	if ((stringa = nome (arg_1)) != S_VUOTA) {
		if ((lWord = pacstr (FindString, stringa) - 1) > MAXCHARS)	// ?
			msg (14, arg_1);
		else {
  			CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
			int i = GetIChar;
			(editor->GetEditCtrl()).SetSel (i, i, FALSE);
			BOOL isSearchDown = TRUE;
			BOOL isMatchCase = FALSE;
			editor->m_textFound = TRUE;
			editor->FindNext (FindString, isSearchDown, isMatchCase);
			if (editor->m_textFound) {
				int startChar, lastChar;
				(editor->GetEditCtrl()).GetSel (startChar, lastChar);
				editor->m_iChar = lastChar;
			}
//			else msg (13, arg_1);
		}
	}
#endif // IL89
}

/*-----------------------------------------------------------------------------
  cerca nel buffer a partire dalla posizione corrente del "cursore" una parola
  o stringa LOGO passata come arg_1, se la trova la sostituisce con arg_2
  ---------------------------------------------------------------------------*/
void CAMBIAS ()
{
#ifdef IL89
  node str1, str2;
  int lung1, lung2, locale1, i, j;
  char *locale, s_locale [MAXCHARS+1];
  if ((str1 = nome (arg_1)) != S_VUOTA)
    if ((lung1 = pacstr (s_locale, str1) - 1) > MAXCHARS) msg (14, arg_1);
    else
      if ((i_foglio-lung1+(lung2 = wrdlen (str2 = nome (arg_2)))) < (d_foglio-1))
        if ((locale = _cerca (c_foglio+j_foglio, c_foglio+i_foglio, s_locale)) != 0) {
          vaiveloce (j = locale - c_foglio);
          n_c = lung1;
          mod_buf0 ();
          locale1 = j_foglio;
          if (lung2 > 0) {
            n_c = lung2;
            mod_buf1 ();
            i_foglio += n_c; 
            irdstr (str2);
            for (i = 1; i <= n_c; i++) *(c_foglio + (locale1++)) = rdstr ();
          }
          quadro (nq_foglio, MOD_Q, j_foglio);
          quadro (nq_foglio, VAI_C, (j_foglio = locale1));
          _commento ();
        }
        else vaiveloce (j = i_foglio);
      else msg_com (12);
#else // IL89
	node stringa1 = nome (arg_1);
	node stringa2 = nome (arg_2);
	if (stringa1 != S_VUOTA) {
		char FindString [MAXCHARS+1];
		char ReplaceString [MAXCHARS+1];
		int lWord1 = pacstr (FindString, stringa1);
		int lWord2 = pacstr (ReplaceString, stringa2);
		if (lWord1 > MAXCHARS)						// ?
			msg (14, arg_1);
		else {
  			CEditorView* editor = theApp.m_editorViews[_file [dev_editor]];
			int i = GetIChar;
			(editor->GetEditCtrl()).SetSel (i, i, FALSE);
			BOOL isSearchDown = TRUE;
			BOOL isMatchCase = FALSE;
			editor->m_textFound = TRUE;
			editor->FindNext (FindString, isSearchDown, isMatchCase);
			if (editor->m_textFound) {
				int startChar, lastChar;
				(editor->GetEditCtrl()).GetSel (startChar, lastChar);
				(editor->GetEditCtrl()).ReplaceSel(ReplaceString);
				lastChar = lastChar + lWord2 - lWord1;
				(editor->GetEditCtrl()).SetSel (startChar, lastChar, ! editor->m_isUpdate);
				editor->m_iChar = lastChar;
			}
			else msg (13, arg_1);
		}
	}
#endif // IL89
}

