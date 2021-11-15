// mainfrm.cpp : implementation of the CMainFrame class
//
/* MAINFRM.CPP

031029	corretto DevToWin, ramo TARTA
020208	dialoghi di uscita condizionati a isExitDialog 
010201	ridefinito WINDOW_OFFSET per tener conto di possibile collasso del menu Finestre
991124	OnWindowsPlayer: cambiato interfaccia di MciPlayerCreate (MciPlay.cpp)
991123	corretto DevToWin per device PLAYER
990727	modif. Help Release e attivato Help Tutorial
990722	modificato gestione di workDirectory e execDirectory
990517	aggiunto handler di OnHelp (vuoto !!)
990513	corretto GetFocusDev
990401	introdotto devFocus e modularizzato PR_FOCUS
		in CommandPrompt, focus a console se non a browser
990308	corretto MainMenuRemoveWindowItem !!!
990303	gestione di finestre Browser in DevToWin
990223	gestione dell'attributo FONT
990211	modificato ShowBrowserHelp e OnHelpRelease
990126	disattivato m_bAutoMenuEnable
		modifica aggiornamento dinamico del menu 
990112	CommanderGetAttr e CommanderSetAttr
980916	disattivato help in linea
980903	chiusura morbida: ConfirmExit
		tolto redefinizione (dummy) di OnDestroy
980902	RIVEDERE DIMENSIONI in OnSize !!!!!
980901	sostitituito CListBox con CEdit per output commander
980831	eliminato extern "C" per conversione a C++ dei moduli C
		interfaccia di metti_sc, execute: char * -> const char *
980713	aggiunto primitive ICON, KB, FOCUS, REFRESH, SCROLL, ..
980709	ClearAllTimers in INTERROMPI e PAUSA
980701	OnWindowsPlayer (finestra di controllo MCI)
		WindowProc
980624	registrazione di classe; opzione per TopMost
		selezione multipla (OnFileLoad), titolo esteso
980210	sostituito ExecOnEvent con NestedExec
980203	Ridotto al minimo le modifiche del menu Main-Finestre
980202	Sostituito ovunque "SelectWindow" con "ShowWindow"
		separato SELEZIONA fin. (corrente) da MOSTRA (fai emergere)
980126	WIN32: eliminato spazi dalla stringa filtro dei CFileDialog
980122	disattivato parte di OnSize !!!!
971217	modificato MainMenuRemoveWindowItem e MainMenuSelectWindowItem
		ritoccato OnPause
971118	aggiunto dialogo per nuova finestra TARTA
971017	spostato in menu comandi dal pannello di controllo;
		iniziato implemetazione di MainStatusDialog
970911	completato selezione interattiva di finestra corrente TARTA/FOGLIO
970910	esteso funzioni globali del menu Windows:
		provato a ridefinire OnMenuSelect (non succede niente)
		provato a specializzare WindowProc (funziona), ma poi disattivato
		definito staticamente un massimo numero di elementi di menu
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"
#include "mainstat.h"

#include "turtlevw.h"
#include "editorvw.h"

#include "selprocd.h"
#include "selglossd.h"
#include "turtlenw.h"
#include "register.h"

#include "mmsystem.h"
#include "vfw.h"

#include <fcntl.h>
#include <sys\stat.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define IDC_DYNAWINDOWS 33000

//>010201
// #define WINDOW_OFFSET 5
#define WINDOW_OFFSET (is_lib ? 0 : 5)
//<010201
// #define MAX_WINDOWS 20

extern CIperlogoApp theApp;
extern int isProfile;
extern BOOL ForceTopmost;
//extern char WorkDirectory [];
extern CString execDirectory;

extern CREATESTRUCT Commander;
extern char CommanderFont[];
extern node FontToList (LOGFONT* lpLogFont);

extern int isAppExit;

int isExitConfirmed = FALSE;
int isExitDialog = TRUE;	// 020208

int MatchKw (node parola, node kw);
int f_apri_s (const char *filename, int mode, int pmode);
void SaveToDev (int dev_conserva);
void EraseProcByName (char *name);
int string2dev (char *name);
int dev2handle (int dev);
int dev2stato (int dev);
void TurtleBringToTop (int nview); 
void EditorBringToTop (int nview); 
void BrowserBringToTop (int handle); 
void PlayerBringToTop (int handle); 
void ClearAllTimers ();
extern void NotYetMsg ();                                                                               
int currentWindow (int mask);
int openDefaultBrowser ();
void BrowserFileOpen (int nview, CString pathName);

int ListToFont (node tail, LOGFONT* lpLogFont);
node FontToList (LOGFONT* lpLogFont);

void PULISCITESTO ();
void CANCETUTTO ();
void C_PRO_IN ();

void PR_ICON();
void PR_UNICON();
void PR_SETFOCUS();
void PR_GETFOCUS();

void PR_KBON();
void PR_KBOFF();
void PR_KBVAL();

#define yesTitleStyles (WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define noTitleStyles (WS_POPUP /* | WS_BORDER */)
#define andTitleStyles (yesTitleStyles & noTitleStyles)

void CMainFrame::SetTitleBar (BOOL setOn)
{
	BOOL isOn;
	DWORD dwAdd, dwRemove, nFlags = 1;
	DWORD oldStyles = GetStyle ();
	isOn = oldStyles & WS_DLGFRAME;
	if ((setOn && isOn) ||  ((! setOn) && (! isOn)))
		return;
	if (setOn) {
		dwRemove = noTitleStyles & (~andTitleStyles);
		dwAdd = yesTitleStyles & (~andTitleStyles);
	} else {
		dwRemove = yesTitleStyles & (~andTitleStyles);
		dwAdd = noTitleStyles & (~andTitleStyles);
	}
	BOOL isModified =
		ModifyStyle (dwRemove, dwAdd, nFlags);
	DWORD newStyles = GetStyle ();
	// tentativo di ridisegnare il frame
	// per rendere effettivo il cambiamento di stile
	if (GetMenu () != NULL)
		DrawMenuBar();
	else if (m_hMenu != NULL) {
		SetMenuBar (TRUE);
		SetMenuBar (FALSE);
	}
}

void CMainFrame::SetMenuBar (BOOL setOn)
{
	BOOL isOn = (GetMenu () != NULL);
	if ((setOn && isOn) ||  ((! setOn) && (! isOn)))
		return;
	if (setOn)
		::SetMenu (m_hWnd, m_hMenu);
	else {
		m_hMenu = ::GetMenu (m_hWnd);
		::SetMenu (m_hWnd, NULL);
	}
}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
node CommanderGetAttr ()
{
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CIperlogoView* pMainView =
		(CIperlogoView*) mainFrame->GetActiveView ();

	node lista = NULLP;

	node font = FontToList(pMainView->RetrieveFont());
	font = cons (pc_FONT, font, LIST);
	lista = cons (font, lista, LIST);

	CRect winRect;
	mainFrame->GetWindowRect(&winRect);

	lista = rassoc2 (pc_DIM,
		consreal (winRect.right - winRect.left),
		consreal (winRect.bottom - winRect.top), lista);

	lista = rassoc2 (pc_POS,
		consreal (winRect.left), consreal (winRect.top), lista);

	CMenu* pMenu = mainFrame->GetMenu ();
	node menu = (pMenu == NULL) ? pc_FALSO : pc_VERO;
	lista = rassoc (pc_MENU, menu, lista);

	char sTitolo [MAXCHARS];
	int nTitolo = mainFrame->GetWindowText(sTitolo, MAXCHARS);
	node titolo = (nTitolo == 0) ? pc_FALSO : _parse (sTitolo);
	lista = rassoc (pc_TITLE, titolo, lista);

	return lista;
}

node KwAssoc (node tail, node kWord);
void CommanderSetAttr (node attrList)
{
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CIperlogoView* pMainView =
		(CIperlogoView*) mainFrame->GetActiveView ();

	CRect winRect;
	mainFrame->GetWindowRect(&winRect);
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
		mainFrame->SetTitleBar (! isFalso);
		if ((! isFalso) && (! isVero)) {
			MemScrivi (sTitolo, title);
			mainFrame->SetWindowText(sTitolo);
		}
	}
	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		if (MatchKw (menu, pc_FALSO))
			mainFrame->SetMenuBar (FALSE);
		else if (MatchKw (menu, pc_VERO))
			mainFrame->SetMenuBar (TRUE);
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
		mainFrame->MoveWindow(xPos, yPos, xDim, yDim, TRUE);
	}

	node font = KwAssoc (attrList, pc_FONT);
	if (font != NULLP) {
		font = cdr (font);
		LOGFONT *lpLogFont = (LOGFONT *) calloc(1, sizeof(LOGFONT));
		if (! ListToFont (font, lpLogFont)) {
			free(lpLogFont);
			err2 (5, arg_2); return;
		}
		pMainView->AssignFont(lpLogFont);
		pMainView->ActivateFont();
		free(lpLogFont);
	}
}

CMenu* MainMenuGetItem (UINT iPos)
{
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CMenu* mainMenu = mainFrame->GetMenu ();
	return mainMenu->GetSubMenu (iPos);
}

void MainMenuCollapse ()
{
// #define MAXCHARS 256
	int i;
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CMenu* mainMenu = mainFrame->GetMenu ();
	mainMenu->RemoveMenu(6, MF_BYPOSITION);
	mainMenu->RemoveMenu(5, MF_BYPOSITION);
	mainMenu->RemoveMenu(4, MF_BYPOSITION);
	mainMenu->RemoveMenu(2, MF_BYPOSITION);
	mainMenu->RemoveMenu(1, MF_BYPOSITION);
//	mainMenu->RemoveMenu(0, MF_BYPOSITION);
	CMenu* menuPopup = MainMenuGetItem (0);
	int itemCount = menuPopup->GetMenuItemCount();
	for (i=itemCount-1; i>=0; i--) {
		if (menuPopup->GetMenuItemID(i) != ID_APP_EXIT)
			menuPopup->RemoveMenu (i, MF_BYPOSITION); 
	}
	mainFrame-> DrawMenuBar();
}

void MainMenuAppendWindowItem (char *name)
{
	CMenu* windowPopup = MainMenuGetItem (1);
	UINT itemCount = windowPopup->GetMenuItemCount();

#ifndef IDC_DYNAWINDOWS
	UINT iPos;
	if (itemCount == MAX_WINDOWS+WINDOW_OFFSET) {
		for (iPos=MAX_WINDOWS+WINDOW_OFFSET-1; iPos>=WINDOW_OFFSET; --iPos)
			windowPopup->RemoveMenu (iPos, MF_BYPOSITION );
		itemCount = WINDOW_OFFSET;
	}
#endif // IDC_DYNAWINDOWS

#ifdef IDC_DYNAWINDOWS
	windowPopup->AppendMenu (MF_STRING | MF_ENABLED, IDC_DYNAWINDOWS+itemCount-WINDOW_OFFSET, name);
#else // IDC_DYNAWINDOWS
	windowPopup->AppendMenu (MF_STRING | MF_ENABLED, ID_WINDOWS_WINDOW+itemCount-WINDOW_OFFSET, name);
#endif // IDC_DYNAWINDOWS
	itemCount = windowPopup->GetMenuItemCount();
	windowPopup->EnableMenuItem (itemCount-1, MF_BYPOSITION | MF_ENABLED);
}

void MainMenuRemoveWindowItem (char *name)
{
#define MAXCHARS 256
	char local[MAXCHARS];
	CMenu* windowPopup = MainMenuGetItem (1);
	int itemCount, charCount;
	itemCount = windowPopup->GetMenuItemCount ();
	int i = 0;
	while (i < itemCount) {
		charCount = windowPopup->GetMenuString (i, local, 32, MF_BYPOSITION);
		if ((charCount != 0) && (_stricmp (name, local) == 0)) {
			windowPopup->RemoveMenu (i, MF_BYPOSITION /* MF_BYCOMMAND */);
/*>990308
			(theApp.m_pMainWnd)->DrawMenuBar();
*/
			--itemCount;
			HMENU hMenu = windowPopup->m_hMenu;
			MENUITEMINFO itemInfo;
//			LPMENUITEMINFO lpMenuItemInfo = (LPMENUITEMINFO) &itemInfo[0];
			char itemText[MAXCHARS];
			BOOL fByPosition = TRUE;
			BOOL result;
			DWORD extError;
			while (i < itemCount) {
				itemInfo.cbSize = sizeof(MENUITEMINFO);
				itemInfo.fMask = MIIM_ID | MIIM_TYPE;
				itemInfo.dwTypeData = &itemText[0];
				itemInfo.cch = MAXCHARS;
				result =
				windowPopup->GetMenuItemInfo(i, &itemInfo, fByPosition);
				if (! result)
					extError = GetLastError();
				itemInfo.fMask = MIIM_ID;
				itemInfo.wID = IDC_DYNAWINDOWS+i-WINDOW_OFFSET;
				result =
				SetMenuItemInfo(hMenu, i, fByPosition, &itemInfo);
				if (! result)
					extError = GetLastError();
				++i;
			}
	break;
/*<*/
		}
		++i;
	}
	(theApp.m_pMainWnd)->DrawMenuBar();
}

void MainMenuCheckWindowItem (UINT iMenu, char *name, BOOL status)
{
	char local[MAXCHARS];
	CMenu* windowPopup = MainMenuGetItem (iMenu);
	int itemCount, charCount;
	itemCount = windowPopup->GetMenuItemCount ();
	UINT nCheck = (status) ? MF_CHECKED : MF_UNCHECKED; 
	int i = 0;
	while (i < itemCount) {
		charCount = windowPopup->GetMenuString (i, local, 32, MF_BYPOSITION);
		if ((charCount != 0) && (_stricmp (name, local) == 0))
			windowPopup->CheckMenuItem (i, MF_BYPOSITION | nCheck); 
		++i;
	}
}

void ShowWindowByDev (int dev)
{
	int handle = dev2handle (dev);
	int stato = dev2stato (dev);
	if (stato & O_TARTA) {					/* .. tipo TARTA */
		TurtleBringToTop (handle); 
	}
	else if (stato & O_FOGLIO) {			/* .. tipo FOGLIO */
		EditorBringToTop (handle);
	}
	else if (stato & O_PLAYER) {			/* .. tipo PLAYER */
		PlayerBringToTop (handle);
	}
	else if (stato & O_BROWSER) {			/* .. tipo BROWSER */
		BrowserBringToTop (handle);
	}
}

void ShowWindowByName (char *name)
{
	int dev = string2dev (name);
	ShowWindowByDev (dev);
/*					// 990304
	int handle = dev2handle (dev);
	int stato = dev2stato (dev);
	if (stato & O_TARTA) {
		TurtleBringToTop (handle); 
	}
	else if (stato & O_FOGLIO) {
		EditorBringToTop (handle);
	}
	else if (stato & O_PLAYER) {
		PlayerBringToTop (handle);
	}
	else if (stato & O_BROWSER) {
		BrowserBringToTop (handle);
	}
*/
}

void MainMenuShowWindowItem (UINT n)
{
	char name [MAX_CHARS];
	int iPos;
	CMenu* windowPopup = MainMenuGetItem (1);
	iPos = n+WINDOW_OFFSET-1;
	windowPopup->GetMenuString( iPos, name, MAX_CHARS-1, MF_BYPOSITION );
	ShowWindowByName (name);
	//	(theApp.m_pMainWnd)->DrawMenuBar();
}

void MainMenuShowWindow (char *inName)	// USATO SOLO DA SEL_FOGLIO
{
	char name [MAX_CHARS];
	int iPos;
	CMenu* windowPopup = MainMenuGetItem (1);
	int itemCount = windowPopup->GetMenuItemCount ();
	for (iPos=WINDOW_OFFSET; iPos<=itemCount-1; iPos++) {
		windowPopup->GetMenuString( iPos, name, MAX_CHARS-1, MF_BYPOSITION );
		if (stricmp (name, inName) == 0) break;
	}
	if (iPos > itemCount-1)
		return;
	ShowWindowByName (name);	// aggiunto 980203
	//	(theApp.m_pMainWnd)->DrawMenuBar();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
//{{AFX_MSG_MAP(CMainFrame)
ON_COMMAND(ID_FILE_CLEAR, OnFileClear)
ON_COMMAND(ID_FILE_NEW, OnFileNew)
ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
ON_COMMAND(ID_FILESAVE, OnFilesave)
ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
ON_COMMAND(ID_FILE_ERASE, OnFileErase)
ON_COMMAND(ID_FILE_PRINTSETUP, OnFilePrintsetup)
ON_COMMAND(ID_WINDOWS_EDITOR, OnWindowsEditor)
ON_COMMAND(ID_WINDOWS_TURTLE, OnWindowsTurtle)
	ON_COMMAND(ID_WINDOWS_BROWSER, OnWindowsBrowser)
	ON_COMMAND(ID_WINDOWS_PLAYER, OnWindowsPlayer)
ON_COMMAND(ID_OPTION_TRACE, OnOptionTrace)
ON_COMMAND(ID_OPTION_SHOWSTATUS, OnOptionShowStatus)
ON_COMMAND(ID_EXEC, OnExec)
ON_COMMAND(ID_PAUSE, OnPause)
ON_COMMAND(ID_HALT, OnHalt)
ON_WM_SIZE()
ON_COMMAND(ID_OPTION_FONT, OnOptionFont)
ON_COMMAND(ID_OPTION_ECO, OnOptionEco)
ON_COMMAND(ID_OPTION_PROMPT, OnOptionPrompt)
ON_COMMAND(ID_OPTION_ECOINT, OnOptionEcoint)
	ON_COMMAND(ID_REGISTER, OnRegister)
	ON_WM_DESTROY()
	ON_COMMAND(ID_OPTION_PROFILE, OnOptionProfile)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_DEMO, OnHelpDemo)
	ON_COMMAND(ID_HELP_EXAMPLES, OnHelpExamples)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_HELP_RELEASE, OnHelpRelease)
	ON_COMMAND(ID_HELP_SEARCH, OnHelpSearch)
	ON_COMMAND(ID_HELP_TOPIC, OnHelpTopic)
	ON_COMMAND(ID_HELP_TUTORIAL, OnHelpTutorial)
	ON_COMMAND(ID_HELP_USING, OnHelpUsing)
	ON_COMMAND(ID_HELP_GLOSSARY, OnHelpGlossary)
	ON_COMMAND(ID_HELP, OnHelp)
	//}}AFX_MSG_MAP
// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
	m_status = NULL;
	m_saveFile[0] = 0;
	m_bAutoMenuEnable = FALSE;
}

CMainFrame::~CMainFrame()
{
	if (m_status != NULL) {
		m_status->DestroyWindow();
		delete (m_status);
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
//	if (! CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.x=Commander.x;
	cs.y=Commander.y;
	cs.cx=Commander.cx;
	cs.cy=Commander.cy;
//>980624
//	return TRUE;
	if (ForceTopmost)
		cs.dwExStyle |= WS_EX_TOPMOST;

	// Use the specific class name we established earlier
	cs.lpszClass = _T("IperlogoClass");

	return CFrameWnd::PreCreateWindow(cs);
//<980624
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

UINT menuItemId;

void MciNotifyError (WPARAM wParam, LPARAM lParam);
void MciNotifyMedia (WPARAM wParam, LPARAM lParam);
void MciNotifyMode (WPARAM wParam, LPARAM lParam);
void MciNotifyPos (WPARAM wParam, LPARAM lParam);
void MciNotifySize (WPARAM wParam, LPARAM lParam);

LRESULT CMainFrame::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message){
	case WM_COMMAND:
#ifdef IDC_DYNAWINDOWS
		if ((wParam >= IDC_DYNAWINDOWS) && (wParam < IDC_DYNAWINDOWS+MAX_DEV)) {
			menuItemId = wParam;
			OnWindowsWindow();
			return 1;
		}
#endif // IDC_DYNAWINDOWS
		break;
	case WM_CREATE:
		break;
	case WM_SIZE:
		break;
	case WM_DESTROY:
		break;
	case WM_CLOSE:
		break;
	case WM_CHAR:
		break;
	case MCIWNDM_NOTIFYERROR:
		MciNotifyError (wParam, lParam);
		break;
	case MCIWNDM_NOTIFYMEDIA:
		MciNotifyMedia (wParam, lParam);
		break;
	case MCIWNDM_NOTIFYMODE:
		MciNotifyMode (wParam, lParam);
		break;
	case MCIWNDM_NOTIFYPOS:
		MciNotifyPos (wParam, lParam);
		break;
	case MCIWNDM_NOTIFYSIZE:
		MciNotifySize (wParam, lParam);
		break;
	case WM_ACTIVATE:
	case WM_QUERYNEWPALETTE:
	case WM_PALETTECHANGED:
//		MessageBox("Player: palette");
		// Forward palette-related messages through to the MCIWnd,
		// so it can do the right thing.
//		if (hwndMovie)
//			return SendMessage(hwndMovie, message, wParam, lParam);
		break;
	} /* switch */

	return CFrameWnd::WindowProc(message, wParam, lParam );
}

void CMainFrame::OnMenuSelect(UINT nItemId, UINT nFlags, HMENU hSysMenu)
{
	CWnd::OnMenuSelect(nItemId, nFlags, hSysMenu);
}

int CMainFrame::ConfirmExit()
{
	CString msgCaption = "Iperlogo";
	CString msgText =
		"Uscendo da Iperlogo si perde il contenuto delle finestre e dello spazio di lavoro se non salvato su archivio. Vuoi procedere ugualmente ?";
	if (isExitConfirmed)
		return TRUE;
	isExitConfirmed = (MessageBox(msgText, msgCaption, MB_OKCANCEL) == IDOK);
	return isExitConfirmed;
}

void CMainFrame::OnFileClear()
{
	// TODO: Add your command handler code here
	
	PULISCITESTO ();
}

void CMainFrame::OnFileNew()
{
	// TODO: Add your command handler code here
	
	int save = n_argomenti;
	CANCETUTTO ();
	n_argomenti = save; 
}

#define DEL_VIRGOLETTE	4L /* parola '"' */
#define pc_RECUPERA kw [25]

void CMainFrame::OnFileLoad()
{
	// CDialogFile
	// TODO: Add your command handler code here
	
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = NULL;
//>980624
//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
//<980624
	CWnd* pParentWnd = NULL;
	BOOL bOpenFileDialog = TRUE;
	node arg1;		 
	
	static char BASED_CODE lpszFilter[] =
		"File IperLogo (*.il)|*.il|File WinLogo (*.lgo)|*.lgo|Tutti i file (*.*)|*.*||";
	
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
//>980709
	char PathNames[1024+1] = "";
	FileDialog.m_ofn.lpstrFile = &PathNames[0];
	FileDialog.m_ofn.nMaxFile = 1024;
//<980709
	int result = FileDialog.DoModal();
	if (result == IDOK) {
//>980624
//		CString PathName = "";
//		PathName = FileDialog.GetPathName();
//		arg1 = metti_sc ((const char*) PathName);
//		//		ExecOnEvent (cons (pc_RECUPERA, cons (DEL_VIRGOLETTE, cons (arg1, NULLP, LIST), LIST), LIST));		 
//		NestedExec (cons (pc_RECUPERA,
//			cons (DEL_VIRGOLETTE,
//			cons (arg1, NULLP, LIST), LIST), LIST));		 

		POSITION pos = FileDialog.GetStartPosition();
		CString PathName;
		while (pos != NULL) {
			PathName = FileDialog.GetNextPathName(pos);
//			arg1 = metti_sc ((const char*) PathName);
			arg1 = metti_sc (PathName);
			NestedExec (cons (pc_RECUPERA,
				cons (DEL_VIRGOLETTE,
				cons (arg1, NULLP, LIST), LIST), LIST));
		}
//<980624
	}


}

void CMainFrame::OnFileSaveas()
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		//		"File IperLogo (*.il) | *.il | File WinLogo (*.lgo) | *.lgo | Tutti i file (*.*) | *.* ||";
		"File IperLogo (*.il)|*.il|File WinLogo (*.lgo)|*.lgo|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt= "il";
	if (m_saveFile[0] != 0)
		lpszFileName = m_saveFile;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd* pParentWnd = this;
	BOOL bOpenFileDialog = FALSE;
	int dev_conserva;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	
	int result = FileDialog.DoModal();
	if (result == IDOK) {
		CString PathName = FileDialog.GetPathName();
		if ((dev_conserva = f_apri_s ((const char *) PathName, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) <= 0) {
		}
		else {
			strcpy (m_saveFile, (const char *) PathName);
			SaveToDev (dev_conserva);
		}
	} else {
		/* file not found */
	}
}

void CMainFrame::OnFilesave()
{
	// TODO: Add your command handler code here
	
	if (m_saveFile[0] == 0)
		OnFileSaveas();
	else {
		int dev_conserva;
		if ((dev_conserva = f_apri_s (m_saveFile, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) <= 0) {
		}
		else {
			SaveToDev (dev_conserva);
		}
	}
}

void CMainFrame::OnFileErase()
{
	// TODO: Add your command handler code here
	
	int result;
	CWnd* pParentWnd = NULL;
	CSelprocDialog selprocDialog (pParentWnd);
	result = selprocDialog.DoModal();
	if (result == IDOK) {
		if (strcmp ((char *) &selprocDialog.m_text, "*") == 0) {
			int save = n_argomenti;
			n_argomenti = 0;
			C_PRO_IN ();
			n_argomenti = save; 
		}
		else
			EraseProcByName ((char *) (&selprocDialog.m_text));
	}
}

//void CMainFrame::OnWindowsNew()
//{
//	// TODO: Add your command handler code here
//	
//}

//void CMainFrame::OnHelpTutorial()
//{
//	// TODO: Add your command handler code here
//	
//}

//void CMainFrame::OnHelpTopic()
//{
//	// TODO: Add your command handler code here
//	
//}

int openDefaultEditor ();
void CMainFrame::OnWindowsEditor()
{
	// TODO: Add your command handler code here
	
	openDefaultEditor ();
}

void CMainFrame::OnWindowsTurtle()
{
	// TODO: Add your command handler code here
	
	CTurtleNew newDialog (this);
	int result = newDialog.DoModal();
	if (result == IDOK) {
	}
}

int openDefaultBrowser ();
void CMainFrame::OnWindowsBrowser() 
{
	// TODO: Add your command handler code here
	
	openDefaultBrowser ();
}

// HWND MciPlayerCreate (int dev, HWND hWnd, HINSTANCE hInstance, node winTitle, LPSTR szFile);
HWND MciPlayerCreate (int dev, HWND hWnd, HINSTANCE hInstance, node winTitle, node attrList);
void CMainFrame::OnWindowsPlayer() 
{
	// TODO: Add your command handler code here

//	MciPlayerCreate (-1, m_hWnd, AfxGetInstanceHandle(), NULLP, "");	// 980709
	MciPlayerCreate (-1, m_hWnd, AfxGetInstanceHandle(), NULLP, NULLP);	// 991124
}

void CMainFrame::OnWindowsWindow()
{
	// TODO: Add your command handler code here
#ifdef IDC_DYNAWINDOWS
	MainMenuShowWindowItem (menuItemId - IDC_DYNAWINDOWS + 1);
#else // IDC_DYNAWINDOWS
	MainMenuShowWindowItem (1);
#endif // IDC_DYNAWINDOWS
}

#ifndef IDC_DYNAWINDOWS

void CMainFrame::OnW2()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (2);
}

void CMainFrame::OnW3()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (3);
}

void CMainFrame::OnW4()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (4);
}

void CMainFrame::OnW5()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (5);
}

void CMainFrame::OnW6()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (6);
}

void CMainFrame::OnW7()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (7);
}

void CMainFrame::OnW8()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (8);
}

void CMainFrame::OnW9()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (9);
}

void CMainFrame::OnW10()
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (10);
}

void CMainFrame::OnW11() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (11);
}

void CMainFrame::OnW12() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (12);
}

void CMainFrame::OnW13() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (13);
}

void CMainFrame::OnW14() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (14);
}

void CMainFrame::OnW15() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (15);
}

void CMainFrame::OnW16() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (16);
}

void CMainFrame::OnW17() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (17);
}

void CMainFrame::OnW18() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (18);
}

void CMainFrame::OnW19() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (19);
}

void CMainFrame::OnW20() 
{
	// TODO: Add your command handler code here
	MainMenuShowWindowItem (20);
}

#endif // IDC_DYNAWINDOWS

void CMainFrame::OnFilePrintsetup()
{
	// TODO: Add your command handler code here
	//	theApp.OnFilePrintSetup();	
	BOOL bPrintSetupOnly = TRUE;
	DWORD dwFlags = 0;
	CWnd* pParentWnd = (CWnd*) this;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	if (theApp.m_mainPrinterData != NULL)
		printDialog.m_pd.hDevMode = theApp.m_mainPrinterData;
	int result = printDialog.DoModal();
	if (result == IDOK) {
		theApp.m_mainPrinterData = printDialog.m_pd.hDevMode;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Comandi gia' nel pannello di controllo (da IPERLVW.CPP)

// #define pc_TRACCIA		kw [26]
// #define pc_NONTRACCIARE kw [27]
// #define pc_PAUSA		kw [64]
// #define pc_CONTINUA 	kw [65]

//	extern
//	int is_pausa, is_continua, is_traccia, is_eco,
extern int is_eco_interactive, is_prompt;
int IsFuncRunning ();
int IsProcRunning ();
int PauseLevel ();
void execute (const char *text);	// 980831

void CaseLabel (char *s)
{
	unsigned char c;	
	while ((c = *(++s)) != '\0')
		*s = tolower(c);
}

// extern
// long int tot_nodi, nodi;

void CMainFrame::OnMainStatusRefresh()
{
	// TODO: Add your command handler code here
	
	if (m_status != NULL) {
		((CMainStatusDialog *) m_status)->m_nodes_total = tot_nodi;
		((CMainStatusDialog *) m_status)->m_nodes_used = tot_nodi - nodi;
		((CMainStatusDialog *) m_status)->m_nodes_free = nodi;
		
		((CMainStatusDialog *) m_status)->UpdateData(FALSE);	
	}
}

void CMainFrame::OnOptionProfile() 
{
	// TODO: Add your command handler code here
	isProfile = ! isProfile;	
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_PROFILE, MF_BYCOMMAND | ((isProfile) ? MF_CHECKED : MF_UNCHECKED)); 
}

void CMainFrame::OnOptionPrompt()
{
	// TODO: Add your command handler code here
	is_prompt = ! is_prompt;	
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_PROMPT, MF_BYCOMMAND | ((is_prompt) ? MF_CHECKED : MF_UNCHECKED)); 
}

void CMainFrame::OnOptionEcoint()
{
	// TODO: Add your command handler code here
	is_eco_interactive = ! is_eco_interactive;	
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_ECOINT, MF_BYCOMMAND | ((is_eco_interactive) ? MF_CHECKED : MF_UNCHECKED)); 
}

void CMainFrame::OnOptionEco()
{
	// TODO: Add your command handler code here
	is_eco = ! is_eco;	
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_ECO, MF_BYCOMMAND | ((is_eco) ? MF_CHECKED : MF_UNCHECKED)); 
}

void CMainFrame::OnOptionTrace()
{
	// TODO: Add your command handler code here
	is_traccia = ! is_traccia;	
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_TRACE, MF_BYCOMMAND | ((is_traccia) ? MF_CHECKED : MF_UNCHECKED)); 
}

void CMainFrame::OnHideStatus()
{
	// TODO: Add your command handler code here
	//	theApp.m_mainStatusShown = ! theApp.m_mainStatusShown;
	
	delete (m_status);
	m_status = NULL;
	CMenu* windowPopup = MainMenuGetItem (2);
	windowPopup->CheckMenuItem (ID_OPTION_SHOWSTATUS, MF_BYCOMMAND | MF_UNCHECKED); 
}

void CMainFrame::OnOptionShowStatus()
{
	// TODO: Add your command handler code here
	//	theApp.m_mainStatusShown = ! theApp.m_mainStatusShown;
	
	if (m_status == NULL) {
		CWnd* pParentWnd = this;
		CMainStatusDialog *pDialog;
		pDialog = new(CMainStatusDialog);
		BOOL result = pDialog->CreateDir (IDD_MAIN_STATUS, pParentWnd);
		m_status = (CWnd*) pDialog;
		OnMainStatusRefresh();
		CMenu* windowPopup = MainMenuGetItem (2);
		windowPopup->CheckMenuItem (ID_OPTION_SHOWSTATUS, MF_BYCOMMAND | MF_CHECKED); 
	} else {
		m_status->DestroyWindow();
		OnHideStatus();
	}
}

void ShowBrowserHelp (CString fileName, CString dirName)
{
	// costruisce nome completo
//	CString pathName = WorkDirectory;
	CString pathName = execDirectory;
	pathName += "\\";
//	if (dirName != NULL)
	if (dirName)	// attenzione: non confrontare CString con NULL !!
		pathName += dirName;
	else
		pathName += "Aiuti";
	pathName += "\\";
	pathName += fileName;
	// cerca browser aperto
	int dev = currentWindow (O_BROWSER);
	// se non lo trova lo apre
	if (dev == -1)
		dev = openDefaultBrowser ();
	if (dev == -1)
		;
	// apre documento HTML da cartella aiuti
	else
		BrowserFileOpen (dev2handle (dev), pathName);
}

void CMainFrame::OnHelpUsing() 
{
	// TODO: Add your command handler code here
	
	NotYetMsg ();
	return;
}

void CMainFrame::OnHelpGlossary() 
{
	// TODO: Add your command handler code here
	
	int result;
	CWnd* pParentWnd = NULL;
	CSelglossDialog selglossDialog (pParentWnd);
	result = selglossDialog.DoModal();
	if (result == IDOK) {
	}
}

// Sommario
void CMainFrame::OnHelpIndex() 
{
	// TODO: Add your command handler code here

	NotYetMsg ();
	return;
	
	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_FINDER; 			/* type of help */
	DWORD dwData = 0L;						/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CMainFrame::OnHelpTopic() 
{
	// TODO: Add your command handler code here
	
	NotYetMsg ();                                                                               
}

void CMainFrame::OnHelpTutorial()
{
	// TODO: Add your command handler code here

//	NotYetMsg ();

	CString fileName = "manuali.htm";
	CString dirName = "aiuti";
	ShowBrowserHelp (fileName, dirName);
	return;
	
	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_FINDER; 			/* type of help */
	DWORD dwData = 0L;						/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CMainFrame::OnHelpSearch()
{
	// TODO: Add your command handler code here

	NotYetMsg ();
	return;

	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_PARTIALKEY;		/* type of help */
	DWORD dwData = (DWORD) "";				/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CMainFrame::OnHelpDemo() 
{
	// TODO: Add your command handler code here
	
	NotYetMsg ();                                                                               
}

void CMainFrame::OnHelpExamples() 
{
	// TODO: Add your command handler code here
	
	NotYetMsg ();                                                                               
}

void CMainFrame::OnHelpRelease()
{
	// TODO: Add your command handler code here

//	CString fileName = "Notedirilascio.htm";
//	CString dirName = "Esempi";
	CString fileName = "release.htm";
	CString dirName = "aiuti";
	ShowBrowserHelp (fileName, dirName);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	int cy_input = (int) (((double) cy) * 0.25);
	cy_input = max(cy_input, 42); cy_input = min(cy_input, 84);
	int cy_output = cy - cy_input;
	
	BOOL bOnlyPerm = FALSE;
	CView* form = GetActiveView();
	CWnd* output = GetDescendantWindow(IDC_LIST1, bOnlyPerm);
	CWnd* output2 = GetDescendantWindow(IDC_EDIT2, bOnlyPerm);	// 980901
	CWnd* input = GetDescendantWindow(IDC_EDIT1, bOnlyPerm);
	CWnd* exec = GetDescendantWindow(IDC_EXEC, bOnlyPerm);
	
	output->MoveWindow(0, 0, 1, 1, TRUE);
	output2->MoveWindow(0, 0, cx-4, cy_output, TRUE);
	input->MoveWindow(-1, cy_output, cx+1, cy_input+1, TRUE);
	exec->MoveWindow(0, 0, 20, 10, TRUE);
}

void CMainFrame::OnExec()
{
	// TODO: Add your command handler code here
	
	((CIperlogoView *) GetActiveView ())->OnExec();
}

void LabelPause (char *label)
{
	//	CMenu* windowPopup = MainMenuGetItem (3);
	//	windowPopup->ModifyMenu( ID_OPTION_PAUSE, MF_BYCOMMAND, ID_OPTION_PAUSE, label );
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CMenu* mainMenu = mainFrame->GetMenu ();
	mainMenu->ModifyMenu(ID_PAUSE, MF_BYCOMMAND, ID_PAUSE, label);
	(theApp.m_pMainWnd)->DrawMenuBar();
}

void CMainFrame::OnPause()
{
	// TODO: Add your command handler code here
	
	int level;
	ClearAllTimers ();	// 980709
	if (IsProcRunning ()) {
		is_pausa = TRUE;
	}
	else if ((level = PauseLevel ()) != 0) {
		execute ("continua");
		CommandPrompt();
	}
}

void CMainFrame::OnHalt()
{
	// TODO: Add your command handler code here
	
	int level;
	ClearAllTimers ();	// 980709
	if (IsFuncRunning ()) {
		_BREAK ();
		CommandPrompt();
	}
	else if ((level = PauseLevel ()) != 0) {
		_BREAK ();
		execute ("continua");
		CommandPrompt();
	}
}

void LabelExec (char *label)
{
	//	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	//	CMenu* mainMenu = mainFrame->GetMenu ();
	//	mainMenu->ModifyMenu( 2, MF_BYPOSITION, ID_EXEC, label );
	//	(theApp.m_pMainWnd)->DrawMenuBar();
}

int GetFocusDev ();
void CMainFrame::CommandPrompt()
{
	// da' fuoco a input da console se non ce l'ha un browser
	int devFocus = GetFocusDev ();
	sollecita ();
	if ((devFocus == -1) || (! (_fstato [devFocus] & O_BROWSER)))
		((CIperlogoView *) GetActiveView ())->GetDlgItem(IDC_EDIT1)->SetFocus();
}

void CMainFrame::OnOptionFont()
{
	// TODO: Add your command handler code here

	CIperlogoView* view = (CIperlogoView *) GetActiveView ();
	view->OnOptionFont();
}

void CMainFrame::OnRegister() 
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = this;
	CRegisterDialog RegisterDialog;
	int result = RegisterDialog.DoModal();
	if (result == IDOK) {
	}
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CMainFrame::RecordProfile()	// 980403: aggiunto 
{
	// record current normal position of the window
	CIperlogoView* mainView = (CIperlogoView*) GetActiveView();
	WINDOWPLACEMENT wndPl;
	if (GetWindowPlacement (&wndPl)) {
		RECT pos = wndPl.rcNormalPosition;
		Commander.x = pos.left;
		Commander.y = pos.top;
		Commander.cx = pos.right - pos.left;
		Commander.cy = pos.bottom - pos.top;
		MemScrivi (CommanderFont, FontToList(&(mainView->m_logFont)));
	}
}

void CMainFrame::OnClose()		// 980403: ridefinito 
{
	if (! isAppExit) {			// 980403: aggiunto
//>020208
//		if (ConfirmExit ())					// 980903
		if ((! isExitDialog) || ConfirmExit ())	
//<
			theApp.OnAppExit();	//
	}							//
	else						//

	// TODO: Add your message handler code here and/or call default

		CFrameWnd::OnClose();
}

CWnd* DevToWin (int dev)
{
	int handle;
	CWnd* pWnd;
	if (_fstato [dev] & O_TARTA) {
//>031029
//		int iview = _file [dev];
//		handle = view [iview]->view;
		handle = _file [dev];
//<031029
		CTurtleView* turtleView =
			theApp.m_turtleViews[handle];
		pWnd = turtleView;
	}
	else if (_fstato [dev] & O_FOGLIO) {
		handle = _file [dev];
		CEditorView* editorView =
			theApp.m_editorViews[handle];
		pWnd = editorView;
	}
#ifdef _WEB
	else if (_fstato [dev] & O_BROWSER) {
		handle = _file [dev];
		CWebView* browserView =
			theApp.m_browserViews[handle];
		pWnd = (CWnd*) browserView;
	}
#endif _WEB
	else if (_fstato [dev] & O_PLAYER) {
		handle = _file [dev];
//>991123
//		pWnd = (CWnd*) handle;
		pWnd = CWnd::FromHandle ((HWND) handle);
//<
	}
	return pWnd;
}

CWnd* ArgToWin (node arg)
{
	int dev = nome2dev (arg);
	if (	(dev < MIN_DEV) 					/* device o sconosc. */
		|| (_fstato [dev] & O_ARCHIVIO))		/* archivio aperto */
	{
		err2 (52, arg);
		return NULL;
	}
	if (! (_fstato [dev] & O_FINESTRA)) {
		return NULL;
	}
	return DevToWin (dev);
}

void PR_MINIMIZE()
{
	CWnd* pWin;
	if (MatchKw (arg_1, pc_COMANDI))
		pWin = theApp.m_pMainWnd;
	else
		pWin = ArgToWin (arg_1);
	if (pWin) {
		pWin = pWin->GetParentOwner();
		pWin->ShowWindow(SW_SHOWMINIMIZED);
	}
}

void PR_MAXIMIZE()
{
	CWnd* pWin;
	if (MatchKw (arg_1, pc_COMANDI))
		pWin = theApp.m_pMainWnd;
	else
		pWin = ArgToWin (arg_1);
	if (pWin) {
		pWin = pWin->GetParentOwner();
		pWin->ShowWindow(SW_SHOWMAXIMIZED);
	}
}

void PR_NORMALIZE()
{
	CWnd* pWin;
	if (MatchKw (arg_1, pc_COMANDI))
		pWin = theApp.m_pMainWnd;
	else
		pWin = ArgToWin (arg_1);
	if (pWin) {
		pWin = pWin->GetParentOwner();
		pWin->ShowWindow(SW_SHOWNORMAL);
	}
}

void PR_RESTORE()
{
	CWnd* pWin;
	if (MatchKw (arg_1, pc_COMANDI))
		pWin = theApp.m_pMainWnd;
	else
		pWin = ArgToWin (arg_1);
	if (pWin) {
		pWin = pWin->GetParentOwner();
		pWin->ShowWindow(SW_RESTORE);
	}
}

void PR_SETFOCUS()
{
	CWnd* pWin;
	int MatchKw (node parola, node kw);
	if (MatchKw (arg_1, pc_COMANDI)) {
		pWin = ((CMainFrame*)(theApp.m_pMainWnd))->GetActiveView();
		pWin->GetDlgItem(IDC_EDIT1)->SetFocus();
	} else {
		pWin = ArgToWin (arg_1);
		if (pWin)
			pWin->SetFocus();
	}
}

int GetFocusDev ()
{
	CWnd* pWin1 = theApp.m_pMainWnd->GetFocus();
//>990513
	if (pWin1 == NULL)
		return -1;
//<
	pWin1 = pWin1->GetParentOwner();
	HWND hWin1 = pWin1->m_hWnd; // GetSafeHwnd();
	CWnd* pWin2 = theApp.m_pMainWnd;
	HWND hWin2 = pWin2->m_hWnd; // GetSafeHwnd();
	if ((pWin1 == pWin2) || (hWin1 == hWin2))
		return CONSOLE;
	int dev;
	for (dev=MIN_DEV; dev <= MAX_DEV; ++dev)
		if ((_file [dev] != -1) && ((_fstato [dev] & O_FINESTRA))) {
			pWin2 = DevToWin (dev);
			pWin2 = pWin2->GetParentOwner();
			hWin2 = pWin2->m_hWnd; // GetSafeHwnd();
			if ((pWin1 == pWin2) || (hWin1 == hWin2))
				return dev;
		}
	return -1;
}

void PR_GETFOCUS()
{
/*> 990401
	risultato = NULLP;
	CWnd* pWin1 = theApp.m_pMainWnd->GetFocus();
	pWin1 = pWin1->GetParentOwner();
	HWND hWin1 = pWin1->m_hWnd; // GetSafeHwnd();
	CWnd* pWin2 = theApp.m_pMainWnd;
	HWND hWin2 = pWin2->m_hWnd; // GetSafeHwnd();
	if ((pWin1 == pWin2) || (hWin1 == hWin2))
	{
		risultato = pc_COMANDI;
		return;
	}
	int dev;
	for (dev=MIN_DEV; dev <= MAX_DEV; ++dev)
		if ((_file [dev] != -1) && ((_fstato [dev] & O_FINESTRA))) {
			pWin2 = DevToWin (dev);
			pWin2 = pWin2->GetParentOwner();
			hWin2 = pWin2->m_hWnd; // GetSafeHwnd();
			if ((pWin1 == pWin2) || (hWin1 == hWin2))
			{
				risultato = _nome [dev];
				return;
			}
		}
*/
	int devFocus = GetFocusDev ();
	if (devFocus == -1)
		risultato = NULLP;
	else
		risultato = _nome [devFocus];
/*<*/
}

void PR_WINDOWSHOW()
{
	int dev = nome2dev (arg_1);
	if (   (dev >= MIN_DEV)
		&& (_fstato [dev] & O_FINESTRA))
		ShowWindowByDev (dev);
	else
		err2 (5, arg_1);
}

void CMainFrame::OnHelp() 
{
	// TODO: Add your command handler code here
	
}
