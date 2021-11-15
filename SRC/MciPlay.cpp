/* MciPlay.cpp	G.Toffoli

000705	esteso ATTR e ASATTR a finestre Player (interfaccia usa dev, non view)
991207	AUTOSIZEWINDOW diventa default; agg. attributo AUTODIM per disabilitarlo
991124	cambiato interfaccia di MciPlayerCreate (MciPlay.cpp);
		abilitato primitive di PLAYER; aggiunto PR_PLAYERMCI
981020	gestione di preferenza ForceTopmost (non funziona ?)
		disattivato MCIWndSetOwner
980831	eliminato extern "C" per conversione a C++ dei moduli C
980707
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"		// 980831

#include "mmsystem.h"
#include "vfw.h"

LRESULT CALLBACK NewMciWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDPROC pOldMciWindowProc;

#define pc_APRI			kw [68]
#define pc_CHIUDI		kw [69]
#define pc_CONSERVA		kw [85]

#define BUFFER_LENGTH 256

extern CIperlogoApp theApp;
extern BOOL ForceTopmost;

void MainMenuAppendWindowItem (char *name);
void MainMenuRemoveWindowItem (char *name);
int handle2dev (int handle, int status);
node AssocWord (node name, node tail);
node dev2nome (int dev);
node KwAssoc (node tail, node kWord);

int dev_player = -1;
int newPlayerCounter = 1;
char DefaultPlayer[64] = "";

void PlayerBringToTop (int handle) 
{
	CWnd::FromHandle((HWND) handle)->BringWindowToTop ();
	int result = SetWindowPos(
		(HWND) handle,				// handle to window
		HWND_TOPMOST,				// placement-order handle
		0, 0, 0, 0,					// dummy x, y, cx, cy
		SWP_NOMOVE | SWP_NOSIZE		// window-positioning flags);
	);
}

HWND MciPlayerCreate (int devIn, HWND hWnd, HINSTANCE hInstance, node lWinTitle, node attrList)
{
	HWND hwndMci;						// window handle of the movie
	char sCounter[32] = "";
	char playerName[MAXCHARS];
	char fileName[MAXCHARS];			// 991125
	LPSTR pFileName = NULL;				// 991125
	int dev;

	if (devIn == -1) {					// gia' allocato ?
		if ((dev = nuovo_dev ()) == 0)	// cerca posto per nuovo device
			return 0;
	}
	else
		dev = devIn;

	if (lWinTitle)
		pacstr (playerName, nome (lWinTitle));
	else {
		if (newPlayerCounter > 1)
			sprintf (sCounter, "%d", newPlayerCounter);
		if (DefaultPlayer[0] != '\0')
			strcpy (playerName, DefaultPlayer);
		else
			strcpy (playerName, "Player");
		strcat (playerName, sCounter);
		lWinTitle = metti_sc(playerName);
	}

	node apri = KwAssoc (attrList, pc_APRI);
	if (apri != NULLP) {
		apri = car (cdr (apri));
		MemScrivi (fileName, apri);
		pFileName = &fileName[0];
	}

	char *pTitle = &playerName[0];
	int isMenu = TRUE;

	BOOL isTitoloFalso = FALSE, isTitoloVero = FALSE;
	char sTitolo [MAX_CHARS];
	node title = KwAssoc (attrList, pc_TITLE);
	if (title == NULLP)
		title = lWinTitle;
	else {
		title = car (cdr (title));
		isTitoloFalso = MatchKw (title, pc_FALSO);
		isTitoloVero = MatchKw (title, pc_VERO);
	}
	if (isTitoloFalso)
		pTitle = NULL;
	else if (! isTitoloVero) {
		MemScrivi (sTitolo, title);
		pTitle = &sTitolo[0];
	}

	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		isMenu = ! MatchKw (menu, pc_FALSO);
	}

	dev_player = dev;

	/* create a movie window that has no file open initially */
	BOOL result = 0;
	DWORD dwStyle = 0;
//	dwStyle |= MCIWNDF_NOAUTOSIZEWINDOW | MCIWNDF_NOTIFYALL;
	if (isTitoloFalso)
		dwStyle |=
			  MCIWNDF_NOMENU | MCIWNDF_NOOPEN | MCIWNDF_NOPLAYBAR;
	else
		dwStyle |=
			  WS_BORDER | WS_THICKFRAME	| WS_SYSMENU | WS_CAPTION
			  | MCIWNDF_RECORD;
	if (isTitoloVero)
		dwStyle |= MCIWNDF_SHOWNAME | MCIWNDF_SHOWPOS | MCIWNDF_SHOWMODE;
	if (ForceTopmost)
		dwStyle |= WS_EX_TOPMOST;
	hwndMci = MCIWndCreate(
		NULL,		// hWnd
		hInstance,
		dwStyle,
		pFileName);
	if (! hwndMci)
		return NULL;

	node pos = KwAssoc (attrList, pc_POS);
	node dim = KwAssoc (attrList, pc_DIM);
	int xPos, yPos, xDim, yDim;
	if ((pos != NULLP) || (dim != NULLP)) {
		CRect winRect;
		CWnd::FromHandle(hwndMci)->GetWindowRect(&winRect);
		xPos = winRect.left;
		yPos = winRect.top;
		xDim = winRect.right - winRect.left;
		yDim = winRect.bottom - winRect.top;
	}
	if (pos != NULLP) {
		pos = cdr (pos);
		xPos = rnum_int (car (pos));
		yPos = rnum_int (car (cdr (pos)));
	}
	if (dim != NULLP) {
		dim = cdr (dim);
		xDim = rnum_int (car (dim));
		yDim = rnum_int (car (cdr (dim)));
	}
	if ((pos != NULLP) || (dim != NULLP))
		CWnd::FromHandle(hwndMci)->MoveWindow(xPos, yPos, xDim, yDim, TRUE);

	if (pTitle)
		SetWindowText (hwndMci, pTitle);
	else if (isTitoloFalso)
		result =
			CWnd::FromHandle(hwndMci)->ModifyStyle (
//			WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_CAPTION, 0, SWP_FRAMECHANGED);
			WS_THICKFRAME | WS_SYSMENU | WS_CAPTION, 0, SWP_FRAMECHANGED);

	_file[dev] = (int) hwndMci;
	_nome [dev] = lWinTitle;
	_fstato [dev] = O_FINESTRA | O_PLAYER;
	result = ShowWindow(hwndMci, SW_SHOW);
	MainMenuAppendWindowItem (playerName);
	++newPlayerCounter;
	pOldMciWindowProc = (WNDPROC) SetWindowLong (
		hwndMci,					// handle of window
		GWL_WNDPROC,				// offset of value to set
		(LONG) &NewMciWindowProc	// new value
	);
	PlayerBringToTop ((int) hwndMci);	// agg. 981020

	return hwndMci;
}

void MciTestClose (HWND hwnd) {
	char playerName[MAXCHARS];
	int dev = handle2dev ((int) hwnd, O_PLAYER);
	if (dev != -1) {
		pacstr (playerName, nome (_nome [dev]));
		MainMenuRemoveWindowItem (playerName);
		_file[dev] = -1;
	}
}

void MciNotifyError (WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND) wParam;
	LONG errorCode = (LONG) lParam;
	AfxMessageBox ("Player: errore");
}

void MciNotifyMedia (WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND) wParam;
	LPSTR lp = (LPSTR) lParam;
}

void MciNotifyMode (WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND) wParam;
	LONG mode = (LONG) lParam;
}

void MciNotifyPos (WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND) wParam;
	LONG pos = (LONG) lParam;
}

void MciNotifySize (WPARAM wParam, LPARAM lParam) {
	HWND hwnd = (HWND) wParam;
}



/*--------------------------------------------------------------------
| LangPlay.c - Sample Win app to play AVI movies using MCIWnd. Handles
|       multiple language track movies and lets the
|       user select the track to listen to at playback.
+--------------------------------------------------------------------*/

/* AVI stuff to keep around */
HWND hwndMovie = NULL;		/* window handle of the movie */
BOOL fMovieOpen = FALSE;	/* Open flag: TRUE == movie open, FALSE = none */


LRESULT CALLBACK NewMciWindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	UINT uCmdType;

	switch (uMsg){
	case WM_SYSCOMMAND:
		uCmdType = wParam & 0xFFFFFFF0;
		if (uCmdType == SC_CLOSE)
			MciTestClose (hwnd);
		break;
	}
	return CallWindowProc(pOldMciWindowProc,
				hwnd, uMsg, wParam, lParam);
}

void PLAYWAVE()
{
}

void MCI()
{
	node CommandList = args[0];
	char FileName[MAXCHARS] = "";
	char CommandString [MAXCHARS];
	MemScrivi (CommandString, CommandList);
	char ReturnString[MAXCHARS];  
	HWND hwndCallback = NULL;   

	MCIERROR MciError = mciSendString (
		CommandString,
		ReturnString,
		MAXCHARS,
		hwndCallback
	);
}

/*------------------------------------------------------
  attiva nuovo player senza cancellare quello precedente
  ------------------------------------------------------*/
void MainMenuShowWindow (char *name);
void MainMenuRemoveWindowItem (char *name);
int currentWindow (int mask);

void sel_player (int dev) {
	char name[MAXCHARS];
	dev_player = dev;
	pacstr (name, nome (_nome [dev]));
	MainMenuShowWindow (name);
}

int OpenPlayer (int dev, node CommandList)
{
	char FileName[MAXCHARS] = "";
	node lWinTitle = _pri (CommandList);

	HWND hwndMci =			/* window handle */
		MciPlayerCreate (dev, theApp.m_pMainWnd->m_hWnd, AfxGetInstanceHandle(),
			lWinTitle, NULLP);
	return (hwndMci) ? 0 : -1;
}

int ClosePlayer (int dev)
{
	HWND hwndMci =
		 (HWND) _file [dev];
	BOOL result = DestroyWindow(hwndMci);
	if (result) {
		char playerName[MAXCHARS];
		_file [dev] = -1;
		pacstr (playerName, nome (_nome [dev]));
		MainMenuRemoveWindowItem (playerName);
		dev_player = -1;
		if ((dev = currentWindow (O_PLAYER)) != -1)
		sel_player (dev);
	}
	return result;
}

void PR_CREAPLAYER ()
{
	MciPlayerCreate (-1, theApp.m_pMainWnd->m_hWnd, AfxGetInstanceHandle(),
						args[0], args[1]);
}

void PR_CHIUDIPLAYER ()
{
	if (dev_player == -1)
		{ err1 (15); return; }	// non posso fare PLAYERCHIUDI
	ClosePlayer (dev_player);
}

void PR_PLAYER ()
{
	if (dev_player == -1)
		risultato = NULLP;
	else
		risultato = dev2nome (dev_player);
}

void PR_PLAYERCOMMAND ()
{
	if (dev_player == -1)
		{ err1 (15); return; }	// non posso fare PLAYERMCI
	HWND hwndMci =
		 (HWND) _file [dev_player];
	char playerString[MAXCHARS];
	char returnString[MAXCHARS];
	MemScrivi (playerString, args[0]);
	LONG result =
		MCIWndSendString (hwndMci, playerString);
	result =
		MCIWndReturnString (hwndMci, returnString, MAXCHARS);
	risultato = logstr (returnString);
}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
node PlayerGetAttr (int dev)
{
	HWND hwndMci =
		 (HWND) _file [dev];

	node lista = NULLP;
	LONG error;

	char modeName[MAXCHARS];
	LONG operatingMode = MCIWndGetMode(hwndMci, modeName, MAXCHARS);
//	if (operatingMode != MCI_MODE_NOT_READY)
		if (strlen(modeName) > 0)
			lista = rassoc (pc_MODO, metti_sc (modeName), lista);

//	char aliasName[MAXCHARS];
	UINT uintAlias = MCIWndGetAlias(hwndMci);
//	if (! error)
//		lista = rassoc (pc_ALIAS, metti_sc (aliasName), lista);

	char fileName[MAXCHARS];
	error = MCIWndGetFileName(hwndMci, fileName, MAXCHARS);
	if ((! error) && (strlen(fileName) > 0))
		lista = rassoc (pc_ARCHIVIO, metti_sc (fileName), lista);

	char deviceName[MAXCHARS];
	error = MCIWndGetDevice(hwndMci, deviceName, MAXCHARS);
	if ((! error) && (strlen(deviceName) > 0))
		lista = rassoc (pc_TIPO, metti_sc (deviceName), lista);

	char sTitolo [MAXCHARS];
	int nTitolo = GetWindowText(hwndMci, sTitolo, MAXCHARS);
	LONG style = GetWindowLong (hwndMci, GWL_STYLE);
	BOOL isTitolo = ((style & WS_CAPTION) != 0);
	node titolo = (isTitolo) ? _parse (sTitolo) : pc_FALSO;
	lista = rassoc (pc_TITLE, titolo, lista);

	CRect winRect;
	GetWindowRect(hwndMci, &winRect);

	lista = rassoc2 (pc_DIM,
		consreal (winRect.right - winRect.left),
		consreal (winRect.bottom - winRect.top), lista);

	lista = rassoc2 (pc_POS,
		consreal (winRect.left), consreal (winRect.top), lista);

	return lista;
}

void PlayerSetAttr (int dev, node attrList)
{
	HWND hwndMci =
		 (HWND) _file [dev];

	// The MCIWndOpen macro opens an MCI device and associates it with an MCIWnd window.
	// For MCI devices that use data files, this macro can also open a specified data file,
	// name a new file to be created,
	// or display a dialog box to let the user select a file to open.
	// You can use this macro or explicitly send the MCIWNDM_OPEN message. 
	node apri = KwAssoc (attrList, pc_APRI);
	if (apri != NULLP) {
		apri = cdr (apri);
		char fileName[MAXCHARS];
		LPSTR pFileName = &fileName[0];
		node logoFileName = car (apri);
		apri = cdr (apri);
		if (typ(logoFileName) == LBCD)
			pFileName = (LPSTR) -1;
		else
			MemScrivi (fileName, logoFileName);
		WORD wFlags = 0;
		if (apri != NULLP)
			wFlags = MCIWNDOPENF_NEW;
		LONG result = MCIWndOpen(hwndMci, fileName, wFlags);
	}

	// The MCIWndClose macro closes an MCI device or file associated with an MCIWnd window.
	// Although the MCI device closes, the MCIWnd window is still open and can be associated with another MCI device.
	// You can use this macro or explicitly send the MCI_CLOSE command. 
	node chiudi = KwAssoc (attrList, pc_CHIUDI);
	if (chiudi != NULLP) {
		LONG result = MCIWndClose(hwndMci);
	}

	// The MCIWndSave macro saves the content currently used by an MCI device.
	// This macro can save the content to a specified data file or display
	// the Save dialog box to let the user select a filename to store the content.
	// You can use this macro or explicitly send the MCI_SAVE command. 
	node conserva = KwAssoc (attrList, pc_CONSERVA);
	if (conserva != NULLP) {
		conserva = cdr (conserva);
		char fileName[MAXCHARS];
		MemScrivi (fileName, car (conserva));
		LONG result = MCIWndSave(hwndMci, fileName);
	}

	CRect winRect;
	GetWindowRect(hwndMci, &winRect);
	int xPos = winRect.left;
	int yPos = winRect.top;
	int xDim = winRect.right - winRect.left;
	int yDim = winRect.bottom - winRect.top;

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
		MoveWindow(hwndMci, xPos, yPos, xDim, yDim, TRUE);
	}
}
