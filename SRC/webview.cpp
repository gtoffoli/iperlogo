// webview.cpp : implementation file
/*	WEBVIEW.CPP

990917  aggiunto override di PreCreteWindow per soluzione di problemi rinfresco
990914  Override di OnWndMsg per soluzione problemi di focus
		// Nota: ma i messaggi di mouse non arrivano qui !!!
		Override di WindowProc della finestra Browser ospitata
990708	aggiunto metodo GetName
990513	modificato OnLButtonUp per evitare rinfreschi inutili
990318	ritoccato OnNavigateComplete;
		aggiunto Refresh a OnLButtonUp
		aggiungere Refresh in OnFileLoad ? NO
		aggiunto Refresh a OnLButtonDown : non va
		disattivato OnPaint ?
990317	investigazione su mancato rinfresco in alcuni casi
		corretto lettura posizione corrente in SetAttr
990315	corretto gestione di strumenti e stato in WebOpenView
990311	corretto gestione di titolo in GETATTR
990303	provato a disabilitare scrollbar
990129	tolto baco a WebOpenView
990128	gestita mancanza di Internet Explorer 4.0
		creato sempre dialogBar e statusBar e gestito
		il loro stato di visualizzazione in SetAttr e GetAttr
990124	copiato handler di alcuni messaggi da MFCIE
990123	provato a eliminare OpenDocumentFile, per ovviare
		a errore "Unable to open empty document" (non c'entra)
990115	PR_BROWSER, PR_CHIUDIBROWSER, 
990107	aggiunto URLDecode
		(da http://www.cs.uga.edu/~gita/guest_form.txt)
9812dd	prima versione
*/

#define SOLUZIONE4

#include "stdafx.h"
#include "logo.h"
#include "iperlogo.h"
#include "webframe.h"
#include "webview.h"
#include "mainfrm.h"
#include "ilcpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define pc_URL	kw [74]
#define pc_VAI	kw [76]
#define pc_STRUMENTI kw [78]


void execute (const char *s);
node dev2nome (int dev);

extern CIperlogoApp theApp;
extern CREATESTRUCT Browser;

int dev_browser = -1;
int newBrowserCounter = 1;
//char DefaultBrowser[64] = "Browser";
char DefaultBrowser[64] = "";

int isBrowserStatus = 1;
int isBrowserDialog = 1;


/////////////////////////////////////////////////////////////////////////////
// imitato da TURTLEVW.CPP

CWebFrame* GetNewWebFrame(int x, int y, int dimx, int dimy, char *pTitle, int menu)
{
    int oldX = Browser.x, oldY = Browser.y;
    int oldDimx = Browser.cx, oldDimy = Browser.cy;
	char * pOldTitle = (char *) Browser.lpszName;
	HMENU oldMenu = Browser.hMenu;					// 980721

	if (x != -1) Browser.x = x;
	if (y != -1) Browser.y = y;
	if (dimx != -1) Browser.cx = dimx;
	if (dimy != -1) Browser.cy = dimy;
	Browser.lpszName = pTitle;
	if (menu != -1) Browser.hMenu = (HMENU) menu;	// 980721

	CSingleDocTemplate* pTemplate = theApp.NewBrowserTemplate();
	ASSERT(pTemplate != NULL);
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
	
	CDocument* pDocument =							// 990128
		pTemplate->OpenDocumentFile(NULL);
//	pTemplate->CreateNewDocument();	NON VA

    Browser.x = oldX; Browser.y = oldY;
    Browser.cx = oldDimx; Browser.cy = oldDimy;
	Browser.lpszName = pOldTitle;
	Browser.hMenu = oldMenu;						// 980721

	if (pDocument)
		return (CWebFrame*) theApp.m_pMainWnd;
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CWebView

int WebOpenView (int x, int y, int dimx, int dimy, char *pTitle, int menu, int isDialog, int isStatus, CString url)
{
	// salvo il MainFrame nominale dell' applicazione 
	CWnd* pOldMain = AfxGetApp()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = 0L;	 

	CWebFrame* webFrame = GetNewWebFrame (x, y, dimx, dimy, pTitle, menu);    

	if (! webFrame) {
		// ripristina il MainFrame nominale dell' applicazione 
		AfxGetApp()->m_pMainWnd = pOldMain;
		return -1;
	}

	CWebView* webView = (CWebView*) webFrame->GetActiveView();
	webView->GetDocument()->SetTitle("Browser");

	int nview;
	for (nview = 0; nview < MAX_BROWSER_VIEWS; nview++)
		if (theApp.m_browserViews[nview] == NULL) break;
	theApp.m_browserViews[nview] = webView;

	webView->m_isExecute = FALSE;
	webView->m_self = nview;
    webView->m_frame = webFrame;
    ((CWebFrame*) webFrame)->m_view = webView;

	if (pTitle) {
		webFrame->SetWindowText((pTitle == (char *) -1L) ? "WebWindow" : pTitle);
	}
    
	// ripristina il MainFrame nominale dell' applicazione 
	AfxGetApp()->m_pMainWnd = pOldMain;

	// crea dialog bar
	CDialogBar* dialogBar = new (CDialogBar);
	dialogBar->Create(webFrame, IDD_BROWSER_DIALOGBAR, CBRS_TOP, AFX_IDW_CONTROLBAR_FIRST);
	webFrame->m_dialogBar = dialogBar;
	webFrame->ShowControlBar(dialogBar, isDialog, FALSE);
	webFrame->m_shownDialogBar = isDialog;

	// crea status bar
	CStatusBar* statusBar = new (CStatusBar);
	statusBar->Create(webFrame, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, AFX_IDW_STATUS_BAR);
	webFrame->m_statusBar = statusBar;
		webFrame->ShowControlBar(statusBar, isStatus, FALSE);
		webFrame->m_shownStatusBar = isStatus;	// +

	// eventualmente apre url
	if (url.GetLength() > 0) {
		webView->FileOpen (url);
	}

    return nview;
}

BOOL WebCloseView (int nview)
{
	CWebView* web = theApp.m_browserViews[nview];
	if (web->m_isExecute)
		return FALSE;
	else {
		delete((CFrameWnd*) (web->m_frame));
		theApp.m_browserViews[nview] = NULL;
		return TRUE;
	}
}

void BrowserBringToTop (int nview) 
{
	CWebView* web = theApp.m_browserViews[nview];
	(web->m_frame)->ShowWindow (SW_RESTORE);
	(web->m_frame)->BringWindowToTop ();
}

node BrowserGetAttr (int nview)
{
	CWebView* web = theApp.m_browserViews[nview];
	return web->GetAttr ();
} 
void BrowserSetAttr (int nview, node attrList)
{
	CWebView* web = theApp.m_browserViews[nview];
	web->SetAttr (attrList);
} 

void BrowserFileOpen (int nview, CString pathName)
{
	CWebView* web = theApp.m_browserViews[nview];
	web->FileOpen (pathName);
} 

int view_to_devbrowser (int view)
{
	int i;
	for (i=0; i<MAX_DEV; i++)
		if ((_file[i] == view) && (_fstato[i] & O_BROWSER)) break;
	return i;
}

/////////////////////////////////////////////////////////////////////////////
// CWebView

IMPLEMENT_DYNCREATE(CWebView, CHtmlView)

CWebView::CWebView()
{
	//{{AFX_DATA_INIT(CWebView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_frame = NULL;
}

CWebView::~CWebView()
{
}

void CWebView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CWebView::GetName(char* name)
{
	int dev = view_to_devbrowser (m_self);
	getnam (name, _nome [dev]);
}

void CWebView::ShowScroll (UINT tipo, BOOL stato)
{
	ShowScrollBar (tipo, stato);
	CScrollBar* vertScrollBar = GetScrollBarCtrl (tipo);
	if (vertScrollBar != NULL)
		vertScrollBar->ShowScrollBar (stato);

}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
unsigned long CWebView::GetAttr ()
{
	node lista = NULLP;
	((CMainFrame*) m_frame)->DrawMenuBar();
	CMenu* browserMenu = ((CMainFrame*) m_frame)->GetMenu ();

	node location =
		(m_frame->m_shownDialogBar) ? pc_VERO : pc_FALSO;
	lista = rassoc (pc_STRUMENTI, location, lista);

	node status =
		(m_frame->m_shownStatusBar) ? pc_VERO : pc_FALSO;
	lista = rassoc (pc_STATO, status, lista);

	CMenu* pMenu = m_frame->GetMenu ();
	node menu = (pMenu == NULL) ? pc_FALSO : pc_VERO;
	lista = rassoc (pc_MENU, menu, lista);

	char sTitolo [MAXCHARS];
	int nTitolo = m_frame->GetWindowText(sTitolo, MAXCHARS);
	BOOL isTitolo = ((m_frame->GetStyle() & WS_CAPTION) != 0);
	node titolo = (isTitolo) ? _parse (sTitolo) : pc_FALSO;
	lista = rassoc (pc_TITLE, titolo, lista);

	CRect winRect;
	m_frame->GetWindowRect(&winRect);

	lista = rassoc2 (pc_DIM,
		consreal (winRect.right - winRect.left),
		consreal (winRect.bottom - winRect.top), lista);

	lista = rassoc2 (pc_POS,
		consreal (winRect.left), consreal (winRect.top), lista);

	CString sUrl = GetLocationURL();
	node url = metti_sc (sUrl);
	lista = rassoc (pc_URL, url, lista);

	return lista;
}

node KwAssoc (node tail, node kWord);
void CWebView::SetAttr (node attrList)
{
	CRect winRect;
	m_frame->GetWindowRect(&winRect);
	int xPos = winRect.left;
	int yPos = winRect.top;
	int xDim = winRect.right - winRect.left;
	int yDim = winRect.bottom - winRect.top;

	int isDialog, isStatus;

	char sTitolo [MAX_CHARS];
	node title = KwAssoc (attrList, pc_TITLE);
	if (title != NULLP) {
		title = car (cdr (title));
		BOOL isFalso = MatchKw (title, pc_FALSO);
		BOOL isVero = MatchKw (title, pc_VERO);
		((CWebFrame*) m_frame)->SetTitleBar (! isFalso);
		if ((! isFalso) && (! isVero)) {
			MemScrivi (sTitolo, title);
			m_frame->SetWindowText(sTitolo);
		}
	}
	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		if (MatchKw (menu, pc_FALSO))
			((CMainFrame*) m_frame)->SetMenuBar (FALSE);
		else if (MatchKw (menu, pc_VERO))
			((CMainFrame*) m_frame)->SetMenuBar (TRUE);
	}
	node location = KwAssoc (attrList, pc_VAI);
	if (location == NULL)
		location = KwAssoc (attrList, pc_STRUMENTI);
	if (location != NULLP) {
		location = car (cdr (location));
		isDialog = MatchKw (location, pc_VERO);
		((CMainFrame*) m_frame)->ShowControlBar((CDialogBar*) (m_frame->m_dialogBar), isDialog, FALSE);
		m_frame->m_shownDialogBar = isDialog;
	}
	node status = KwAssoc (attrList, pc_STATO);
	if (status != NULLP) {
		status = car (cdr (status));
		isStatus = MatchKw (status, pc_VERO);
		((CMainFrame*) m_frame)->ShowControlBar((CStatusBar*) (m_frame->m_statusBar), isStatus, FALSE);
		m_frame->m_shownStatusBar = isStatus;
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

	char sSource [MAX_CHARS];
	node source = KwAssoc (attrList, pc_URL);
	if (source != NULLP) {
		source = car (cdr (source));
		MemScrivi (sSource, source);
		CString pathName = sSource;
		FileOpen (pathName);
	}
}


BEGIN_MESSAGE_MAP(CWebView, CHtmlView)
	//{{AFX_MSG_MAP(CWebView)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_NAVIGATE_BACK, OnNavigateBack)
	ON_COMMAND(ID_NAVIGATE_HOME, OnNavigateHome)
	ON_COMMAND(ID_NAVIGATE_RELOAD, OnNavigateReload)
	ON_COMMAND(ID_NAVIGATE_STOP, OnNavigateStop)
	ON_COMMAND(ID_NAVIGATE_FORWARD, OnNavigateForward)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, OnViewFontsLargest)
	ON_COMMAND(ID_SELECT_BROWSER, OnSelectBrowser)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebView diagnostics

#ifdef _DEBUG
void CWebView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CWebView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWebView message handlers

BOOL chiudi_browser (int dev);
void CWebView::OnFileClose() 
{
	// TODO: Add your command handler code here
	
	int handle = theApp.GetBrowserViewIndex (this);
	int dev = handle2dev (handle, O_BROWSER);
	chiudi_browser (dev);
}

void CWebView::FileOpen (CString pathName) 
{
	Navigate2 (pathName, 0, NULL, NULL, NULL, 0);
}

void CWebView::OnFileLoad() 
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		"File HTML (*.htm)|*.htm|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = NULL;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = TRUE;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);

	int result = FileDialog.DoModal();
	if (result == IDOK) {
		CString PathName = FileDialog.GetPathName();
		FileOpen (PathName);
	} else if (result == IDCANCEL) {
		int extError = CommDlgExtendedError ();
		if (extError) {
			CString errMsg;
			char errCode [64];
			_itoa (extError, errCode, 10);
			CString errNum = errCode;
			errMsg = "Web: error #" + errNum;
			AfxMessageBox (errMsg);
		}
	}
}

void CWebView::OnFileExit() 
{
	// TODO: Add your command handler code here
	
		chiudi_browser (view_to_devbrowser (m_self));
}

node KwAssoc (node tail, node kWord);
void MainMenuAppendWindowItem (char *name);

int apri_browser (node lDevName, node attrList)
{
	int i, j;
	char nameString [MAXCHARS];

	int dev = nome2dev (lDevName);
	if (dev >= MIN_DEV) {
		err2 (23, lDevName);		// .. gia' aperto
		return -1;
	}
	if ((i = nuovo_dev ()) == 0)	// cerca posto per nuovo device
		return -1;

	char *pTitle = &DefaultBrowser[0];
	int isMenu = TRUE;
	int isDialog = isBrowserDialog;
	int isStatus = isBrowserStatus;
	int xPos = -1, yPos = -1;
	int xDim = -1, yDim = -1;

	BOOL isFalso = FALSE, isVero = FALSE;
	char sTitolo [MAX_CHARS];
	node title = KwAssoc (attrList, pc_TITLE);
	if (title == NULLP)
		title = lDevName;
	else {
		title = car (cdr (title));
		isFalso = MatchKw (title, pc_FALSO);
		isVero = MatchKw (title, pc_VERO);
	}
	if (isFalso)
		pTitle = NULL;
	else if (! isVero) {
		MemScrivi (sTitolo, title);
		pTitle = &sTitolo[0];
	}

	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		isMenu = ! MatchKw (menu, pc_FALSO);
	}
	node location = KwAssoc (attrList, pc_VAI);
	if (location == NULL)
		location = KwAssoc (attrList, pc_STRUMENTI);
	if (location != NULLP) {
		location = car (cdr (location));
		isDialog = MatchKw (location, pc_VERO);
	}
	node status = KwAssoc (attrList, pc_STATO);
	if (status != NULLP) {
		status = car (cdr (status));
		isStatus = MatchKw (status, pc_VERO);
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

	CString url = "";
	char sSource [MAX_CHARS];
	node source = KwAssoc (attrList, pc_URL);
	if (source != NULLP) {
		source = car (cdr (source));
		MemScrivi (sSource, source);
		url = sSource;
	}

	j = WebOpenView (xPos, yPos, xDim, yDim, pTitle, isMenu, isDialog, isStatus, url);
	if (j < 0) {
		err2 (39, lDevName);	// .. richiede Internet Explorer 4.0
		return -1;
	}

	_file [i] = j;                  	// handle del device
	_nome [i] = lDevName;              	// nome del device
	_fstato [i] = O_FINESTRA | O_BROWSER;// stato del device

	pacstr (nameString, nome (lDevName));// nuova voce in menu WINDOWS
	MainMenuAppendWindowItem (nameString);

	dev_browser = i;					// ?
	++newBrowserCounter;
	return i;							// riporta indice device
}

/*------------------------------------------------------
  attiva nuovo browser senza cancellare quello precedente
  ------------------------------------------------------*/
void MainMenuShowWindow (char *name);

void sel_browser (/*nview*/ int dev) {
	char name[MAXCHARS];
	dev_browser = dev;
	pacstr (name, nome (_nome [dev]));
	MainMenuShowWindow (name);
}

void MainMenuRemoveWindowItem (char *name);
int currentWindow (int mask);

BOOL chiudi_browser (int dev)
{
	char nameString[MAXCHARS];
	BOOL result = WebCloseView (_file [dev]);
	if (result) {
		_file [dev] = -1;
		pacstr (nameString, nome (_nome [dev]));
		MainMenuRemoveWindowItem (nameString);
		dev_browser = -1;
		if ((dev = currentWindow (O_BROWSER)) != -1)
		sel_browser (dev);
	}
	return result;
}

int openDefaultBrowser ()	// imitato da openDefaultEditor
{
	char sCounter[32] = "";
	char browserName[MAXCHARS];

	if (newBrowserCounter > 1)
		sprintf (sCounter, "%d", newBrowserCounter);
	if (DefaultBrowser[0] != '\0')
		strcpy (browserName, DefaultBrowser);
	else
		pacstr (browserName, nome (pc_BROWSER));
	strcat (browserName, sCounter);
  	return apri_browser (mettip (logstr (browserName)), NULLP);
}

#ifdef SOLUZIONE3
WNDPROC pOldBrowserWindowProc;
LRESULT CALLBACK NewBrowserWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif // SOLUZIONE3

void CWebView::OnInitialUpdate() 
{
	//TODO: This code navigates to a popular spot on the web.
	//Change the code to go where you'd like.
//	Navigate2(_T("http://www.microsoft.com/visualc/"),NULL,NULL);

/*> eliminato 990318
	SetScaleToFitSize (CSize (400, 400));
*/
#ifdef SOLUZIONE3
	HWND hwndBrowser = m_wndBrowser.m_hWnd;// handle of Input control window 
	pOldBrowserWindowProc = (WNDPROC) SetWindowLong(
		hwndBrowser,					// handle of window
		GWL_WNDPROC,					// offset of value to set
		(LONG) &NewBrowserWindowProc	// new value
	);
#endif // SOLUZIONE3
}

void CWebView::OnNavigateBack() 
{
	// TODO: Add your command handler code here
	
	GoBack( );
}

void CWebView::OnNavigateForward() 
{
	// TODO: Add your command handler code here
	
	GoForward( );
}

void CWebView::OnNavigateHome() 
{
	// TODO: Add your command handler code here
	
	GoHome( );
}

void CWebView::OnNavigateReload() 
{
	// TODO: Add your command handler code here
	
	Refresh( );
}

void CWebView::OnNavigateStop() 
{
	// TODO: Add your command handler code here
	
	Stop( );
}

void PR_CREABROWSER ()
{
	int dev =
		apri_browser (arg_1, arg_2);
}

// SwapChar:  This routine swaps one character for another
void SwapChar(char * pOriginal, char cBad, char cGood) {
    int i;    // generic counter variable

    // Loop through the input string (cOriginal), character by
    // character, replacing each instance of cBad with cGood

    i = 0;
    while (pOriginal[i]) {
        if (pOriginal[i] == cBad) pOriginal[i] = cGood;
        i++;
    }
}

// IntFromHex:  A subroutine to unescape escaped characters.
int IntFromHex (char *pChars) {
    int Hi;        // holds high byte
    int Lo;        // holds low byte
    int Result;    // holds result

    // Get the value of the first byte to Hi
    Hi = pChars[0];
    if ('0' <= Hi && Hi <= '9') {
        Hi -= '0';
    } else
    if ('a' <= Hi && Hi <= 'f') {
        Hi -= ('a'-10);
    } else
    if ('A' <= Hi && Hi <= 'F') {
        Hi -= ('A'-10);
    }

    // Get the value of the second byte to Lo
    Lo = pChars[1];
    if ('0' <= Lo && Lo <= '9') {
        Lo -= '0';
    } else
    if ('a' <= Lo && Lo <= 'f') {
        Lo -= ('a'-10);
    } else
    if ('A' <= Lo && Lo <= 'F') {
        Lo -= ('A'-10);
    }
    Result = Lo + (16 * Hi);
    return (Result);
}

// URLDecode: This routine loops through the string pEncoded
// (passed as a parameter), and decodes it in place. It checks for
// escaped values, and changes all plus signs to spaces. The result
// is a normalized string. It calls the two subroutines directly
// above in this listing, IntFromHex() and SwapChar().

void URLDecode (char *pEncoded)
{
    char *pDecoded;          // generic pointer
/*>990107
    // First, change those pesky plusses to spaces
    SwapChar (pEncoded, '+', ' ');
<*/
    // Now, loop through looking for escapes
    pDecoded = pEncoded;
    while (*pEncoded) {
        if (*pEncoded=='%') {
            // A percent sign followed by two hex digits means
            // that the digits represent an escaped character. We
            // must decode it.
            pEncoded++;
            if (isxdigit(pEncoded[0]) && isxdigit(pEncoded[1])) {
                *pDecoded++ = (char) IntFromHex(pEncoded);
                pEncoded += 2;
            }
        } else {
            *pDecoded ++ = *pEncoded++;
        }
    }
    *pDecoded = '\0';
}

void CWebView::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) 
{
	// TODO: Add your specialized code here and/or call the base class

	CString iperlogoKey = "iperlogo:";
	CString url = lpszURL;
	int pos;

	// nucleo di interprete di url del tipo 'iperlogo:<stringa>'
	pos = url.Find (iperlogoKey);
	if (pos == 0) {
		CString logoCode = url.Mid(iperlogoKey.GetLength());
		if (logoCode.GetLength() > 0) {
			logoCode += '\0';
			LPTSTR pEncoded = logoCode.GetBuffer(1);
			URLDecode (pEncoded);
			logoCode.ReleaseBuffer();
			m_isExecute = TRUE;
			execute (logoCode);
			m_isExecute = FALSE;
		}
		*pbCancel = TRUE;
	}

	CHtmlView::OnBeforeNavigate2(lpszURL, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);
}

void CWebView::OnNavigateComplete2(LPCTSTR strURL) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (m_frame->m_statusBar != NULL)
		((CStatusBar*) (m_frame->m_statusBar))->GetStatusBarCtrl().SetText(strURL, 255, 0);

	CHtmlView::OnNavigateComplete2(strURL);
}

void PR_BROWSER ()
{
	if (dev_browser == -1)
		risultato = NULLP;
	else
		risultato = dev2nome (dev_browser);
}

void PR_CHIUDIBROWSER ()
{
	if (dev_browser == -1) {
	}
	else
		chiudi_browser (dev_browser);
}

#ifdef MFCIE_SAMPLE

// This is part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// mfcieVw.cpp : implementation of the CMfcieView class
//

#include "stdafx.h"
#include "mfcie.h"
#include "MainFrm.h"

#include "mfcieDoc.h"
#include "mfcieVw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcieView

IMPLEMENT_DYNCREATE(CMfcieView, CHtmlView)

BEGIN_MESSAGE_MAP(CMfcieView, CHtmlView)
	//{{AFX_MSG_MAP(CMfcieView)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_COMMAND(ID_GO_SEARCH_THE_WEB, OnGoSearchTheWeb)
	ON_COMMAND(ID_GO_START_PAGE, OnGoStartPage)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_HELP_WEB_TUTORIAL, OnHelpWebTutorial)
	ON_COMMAND(ID_HELP_ONLINE_SUPPORT, OnHelpOnlineSupport)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREE_STUFF, OnHelpMicrosoftOnTheWebFreeStuff)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_FREQUENTLY_ASKED_QUESTIONS, OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_GET_FASTER_INTERNET_ACCESS, OnHelpMicrosoftOnTheWebGetFasterInternetAccess)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_MICROSOFT_HOME_PAGE, OnHelpMicrosoftOnTheWebMicrosoftHomePage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEARCH_THE_WEB, OnHelpMicrosoftOnTheWebSearchTheWeb)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_SEND_FEEDBACK, OnHelpMicrosoftOnTheWebSendFeedback)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_INTERNET_START_PAGE, OnHelpMicrosoftOnTheWebInternetStartPage)
	ON_COMMAND(ID_HELP_MICROSOFT_ON_THE_WEB_BEST_OF_THE_WEB, OnHelpMicrosoftOnTheWebBestOfTheWeb)
	ON_COMMAND(ID_VIEW_FONTS_LARGEST, c)
	ON_COMMAND(ID_VIEW_FONTS_LARGE, OnViewFontsLarge)
	ON_COMMAND(ID_VIEW_FONTS_MEDIUM, OnViewFontsMedium)
	ON_COMMAND(ID_VIEW_FONTS_SMALL, OnViewFontsSmall)
	ON_COMMAND(ID_VIEW_FONTS_SMALLEST, OnViewFontsSmallest)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcieView construction/destruction

CMfcieView::CMfcieView()
{
}

CMfcieView::~CMfcieView()
{
}

BOOL CMfcieView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(0);
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcieView drawing

void CMfcieView::OnDraw(CDC* pDC)
{
	CMfcieDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CMfcieView diagnostics

#ifdef _DEBUG
void CMfcieView::AssertValid() const
{
	CView::AssertValid();
}

void CMfcieView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMfcieDoc* CMfcieView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcieDoc)));
	return (CMfcieDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcieView message handlers

void CMfcieView::OnTitleChange(LPCTSTR lpszText)
{
	// this will change the main frame's title bar
	if (m_pDocument != NULL)
		m_pDocument->SetTitle(lpszText);
}

void CMfcieView::OnDocumentComplete(LPCTSTR lpszUrl)
{
	// make sure the main frame has the new URL.  This call also stops the animation
	((CMainFrame*)GetParentFrame())->SetAddress(lpszUrl);
}

void CMfcieView::OnInitialUpdate()
{
	// Go to the home page initially
	CHtmlView::OnInitialUpdate();
	GoHome();
}

void CMfcieView::OnBeforeNavigate2(LPCTSTR /*lpszURL*/, DWORD /*nFlags*/,
	LPCTSTR /*lpszTargetFrameName*/, CByteArray& /*baPostedData*/,
	LPCTSTR /*lpszHeaders*/, BOOL* /*pbCancel*/)
{
	// start the animation so that is plays while the new page is being loaded
	((CMainFrame*)GetParentFrame())->StartAnimation();
}

// these are all simple one-liners to do simple controlling of the browser
void CMfcieView::OnGoBack()
{
	GoBack();
}

void CMfcieView::OnGoForward()
{
	GoForward();
}

void CMfcieView::OnGoSearchTheWeb()
{
	GoSearch();
}

void CMfcieView::OnGoStartPage()
{
	GoHome();
}

void CMfcieView::OnViewStop()
{
	Stop();
}

void CMfcieView::OnViewRefresh()
{
	Refresh();
}

// these all go to specific web pages, just like Internet Explorer's help menu
void CMfcieView::OnHelpWebTutorial()
{
	CString str;

	str.LoadString(IDS_WEB_TUTORIAL);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpOnlineSupport()
{
	CString str;

	str.LoadString(IDS_ONLINE_SUPPORT);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebBestOfTheWeb()
{
	CString str;

	str.LoadString(IDS_BEST_OF_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebFreeStuff()
{
	CString str;

	str.LoadString(IDS_FREE_STUFF);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebFrequentlyAskedQuestions()
{
	CString str;

	str.LoadString(IDS_FREQUENTLY_ASKED_QUESTIONS);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebGetFasterInternetAccess()
{
	CString str;

	str.LoadString(IDS_GET_FASTER_INTERNET_ACCESS);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebMicrosoftHomePage()
{
	CString str;

	str.LoadString(IDS_MICROSOFT_HOME_PAGE);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebSearchTheWeb()
{
	CString str;

	str.LoadString(IDS_SEARCH_THE_WEB);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebSendFeedback()
{
	CString str;

	str.LoadString(IDS_SEND_FEEDBACK);
	Navigate2(str, 0, NULL);
}

void CMfcieView::OnHelpMicrosoftOnTheWebInternetStartPage()
{
	CString str;

	str.LoadString(IDS_INTERNET_START_PAGE);
	Navigate2(str, 0, NULL);
}

// these functions control the font size.  There is no explicit command in the
// CHtmlView class to do this, but we can do it by using the ExecWB() function.
void CMfcieView::OnViewFontsLargest()
{
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsLarge()
{
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsMedium()
{
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsSmall()
{
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CMfcieView::OnViewFontsSmallest()
{
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

// This demonstrates how we can use the Navigate2() function to load local files
// including local HTML pages, GIFs, AIFF files, etc.
void CMfcieView::OnFileOpen()
{
	CString str;

// HTML Files|*.htm;*.html|Text Files|*.txt|GIF Files|*.gif|JPEG Files|*.jpg;*.jpeg|AU Files|*.au|AIFF Files|*.aif;*.aiff|XBM Files|*.xbm|All Files|*.*||
	str.LoadString(IDS_FILETYPES);

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);

	if(fileDlg.DoModal() == IDOK)
		Navigate2(fileDlg.GetPathName(), 0, NULL);
}

#endif // MFCIE_SAMPLE

//adattato da MFCIE
void CWebView::OnFileOpen() 
{
	// TODO: Add your command handler code here
	
	CString str;

// HTML Files|*.htm;*.html|Text Files|*.txt|GIF Files|*.gif|JPEG Files|*.jpg;*.jpeg|AU Files|*.au|AIFF Files|*.aif;*.aiff|XBM Files|*.xbm|All Files|*.*||
//	str.LoadString(IDS_FILETYPES);

	str = "File HTML|*.htm;*.html|File di testo|*.txt|File GIF|*.gif|File JPEG|*.jpg;*.jpeg|File AU|*.au|File AIFF|*.aif;*.aiff|File XBM|*.xbm|Tutti i file|*.*||";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, str);

	if(fileDlg.DoModal() == IDOK)
		Navigate2(fileDlg.GetPathName(), 0, NULL);
}

void CWebView::OnViewFontsSmallest() 
{
	// TODO: Add your command handler code here
	
	COleVariant vaZoomFactor(0l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebView::OnViewFontsSmall() 
{
	// TODO: Add your command handler code here
	
	COleVariant vaZoomFactor(1l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebView::OnViewFontsMedium() 
{
	// TODO: Add your command handler code here
	
	COleVariant vaZoomFactor(2l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebView::OnViewFontsLarge() 
{
	// TODO: Add your command handler code here
	
	COleVariant vaZoomFactor(3l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebView::OnViewFontsLargest() 
{
	// TODO: Add your command handler code here
	
	COleVariant vaZoomFactor(4l);

	ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER,
		   &vaZoomFactor, NULL);
}

void CWebView::OnSelectBrowser() 
{
	// TODO: Add your command handler code here
	
}

#ifdef SOLUZIONE1

int GetFocusDev ();
// Nota: ma i messaggi di mouse non arrivano qui !!!
void CWebView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CHtmlView::OnLButtonUp(nFlags, point);
	int devFocus = GetFocusDev ();
	if ((devFocus > 0) && (_fstato [devFocus] & O_BROWSER))
		return;
	Refresh ();
}

#endif // SOLUZIONE1

#ifdef SOLUZIONE2

// Nota: ma i messaggi di mouse non arrivano qui !!!
BOOL CWebView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (m_frame)
		if (message > WM_MOUSEFIRST)
			if (message < WM_MOUSELAST)
	{
		BOOL bHandled = FALSE;
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd) {
			HWND hFocusWnd = pFocusWnd->m_hWnd;
			BOOL bHasFocus = (hFocusWnd == m_hWnd) || (hFocusWnd == m_frame->m_hWnd);
			if (! bHasFocus)
				BOOL bHandled = CHtmlView::OnWndMsg(message, wParam, lParam, pResult);
		}
		return bHandled || CWebView::OnWndMsg(message, wParam, lParam, pResult);
	}

	return CHtmlView::OnWndMsg(message, wParam, lParam, pResult);
}
#endif // SOLUZIONE2

#ifdef SOLUZIONE3

LRESULT CALLBACK NewBrowserWindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
// Nota: ma i messaggi di mouse non arrivano qui !!!
{
	LRESULT lResult = 
	CallWindowProc(pOldBrowserWindowProc, hwnd, uMsg, wParam, lParam);

	if ((uMsg > WM_MOUSEFIRST) && (uMsg < WM_MOUSELAST)) {

	switch (uMsg){
	case WM_LBUTTONDOWN:
		break;
	default:
		break;
	}

	}

	return lResult;
}

#endif // SOLUZIONE3

#ifdef SOLUZIONE4

#ifdef DOC
da: support.microsoft.com/support/kb/articles/q220/0/21.asp

BUG: CHtmlView Does Not Repaint Correctly in an AppWizard-Generated MDI Application

The information in this article applies to:

Microsoft Visual C++, 32-bit Editions, version 6.0
on the following platforms: NT, Win95, Win98 
The Microsoft Foundation Classes (MFC)

SYMPTOMS

In an AppWizard-generated MDI application with more than two view windows overlapping
each other, if the user clicks in the client area of the MDI frame window other than
the view, such that the view loses the focus, and then clicks back in the client area
of the view, the CHtmlView does not repaint correctly. However,
if the user clicks the nonclient area of the view, the CHtmlView repaints correctly. 

CAUSE

The WS_CLIPCHILDREN style is not set for CHtmlView, and therefore,
on activating the view by clicking the view area, CHtmlView does not send a
message to the embedded Web Browser control to repaint itself. 

RESOLUTION

To work around this problem, in PreCreateWindow of CHtmlView,
add the WS_CLIPCHILDREN style. Following is a code example:
#endif // DOC

BOOL CWebView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= WS_CLIPCHILDREN;

	return CHtmlView::PreCreateWindow(cs);
}  

#endif // SOLUZIONE4
