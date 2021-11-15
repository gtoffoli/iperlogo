/* TOOLFRAME.CPP

000315	iniziato tooltips; correzioni varie
000310	corretto PR_TOOLBARCREATE per fsStyle == TBSTYLE_SEP (separatore)
990917	inserito PR_TOOLBARSETPOS (PULSANTIERAASPOS)
9909015	Aumentato MAX_BUTTONS
990715	Ritocco nel caso di pulsanti con testo
990714	Cancella background di Miniframe se non docked
990621	Message Mapping for Continuous Range of Command IDs
		(da www.formatics.com/AmpCh1.htm)
990617	WindowProc (eliminata 990621)
990616	gestione 8 argomenti
9903xx	prime prove
*/

// ToolFrame.cpp : implementation file
//

#include "stdafx.h"
#include <afxcmn.h>

#include "iperlogo.h"
#include "ToolFrame.h"

#include "ilcpp.h"
#include "ildialog.h"

#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>

#include "CDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWnd wndTop; // SetWindowPos's pWndInsertAfter

static UINT nToolbarID = IDC_TOOLBAR;
#define MAX_BUTTONS 128

static int nDockBarIDs [5] = {
//	0,
	AFX_IDW_DOCKBAR_TOP,
	AFX_IDW_DOCKBAR_TOP,	// 1
	AFX_IDW_DOCKBAR_RIGHT,	// 2
	AFX_IDW_DOCKBAR_BOTTOM,	// 3
	AFX_IDW_DOCKBAR_LEFT,	// 4
};

extern void IperlogoExit ();
extern void switch_tarta (int handle);
extern int dev2handle (int dev);
extern CWnd* TurtleHandle2Frame (int handle);
extern DlgList Dialogs;

/////////////////////////////////////////////////////////////////////////////
// CToolFrame

IMPLEMENT_DYNCREATE(CToolFrame, CMiniFrameWnd)

CToolFrame::CToolFrame()
{
	m_bAutoMenuEnable = FALSE;
}

CToolFrame::~CToolFrame()
{
}

BEGIN_MESSAGE_MAP(CToolFrame, CMiniFrameWnd)
	//{{AFX_MSG_MAP(CToolFrame)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(IDC_TOOLBAR, IDC_TOOLBAR+RANGE_TOOLBAR-1, OnButton)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TOOLBAR, IDC_TOOLBAR+RANGE_TOOLBAR-1, OnButtonClicked)
	ON_UPDATE_COMMAND_UI_RANGE(IDC_TOOLBAR, IDC_TOOLBAR+RANGE_TOOLBAR-1, OnUpdateButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolFrame message handlers

void CToolFrame::OnButton(UINT uID)
{
	m_uCurrentBtn = uID;
}

void CToolFrame::OnButtonClicked(UINT uID)
{
	m_uCurrentBtn = uID;
	DlgEntry buttonEntry = IdToEntry (Dialogs, uID);
	if (buttonEntry) {
       	node logoList = GetSetup(buttonEntry);
		if (logoList != NULLP) {
			DlgEntry dialogEntry = DialogGetParent(buttonEntry);
//			CToolFrame* toolFrame = (CToolFrame*) DialogGetWindow(dialogEntry);
//			int parentHandle = toolFrame->m_parentHandle;
			int parentHandle = m_parentHandle;
			int oldHandle = i_tarta;
			if ((parentHandle != -1) && (parentHandle != oldHandle))
				switch_tarta (parentHandle);
			NestedExec(logoList);
			if ((parentHandle != -1) && (parentHandle != oldHandle))
				switch_tarta (oldHandle);
		}
	}
	else
		TRACE("invalid CtrlID");
	ASSERT(buttonEntry);
}

void CToolFrame::OnUpdateButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

// ATTENZIONE ! E' diverso da AddControl !! la lista dei figli
// non contiene i nomi ma direttamente gli entry !!
DlgEntry AddToolbarButton (DlgEntry parent, int id, node setup) {
	DlgEntry buttonEntry = CreateDlgEntry(NULL, BUTTON, id, parent, NULLP, NULLP, setup, NULLP);
	node name = NULLP;
	AddEntry(&Dialogs, buttonEntry, name);
	PutSons (parent, cons (buttonEntry, GetSons (parent), LIST));
	return buttonEntry;
}

// ATTENZIONE ! E' diverso da DeleteSons !! elimina direttamente
// gli entry, che non contengono il nome ma NULLP
void RemoveToolbarButtons (node sons)
{
	DlgEntry buttonEntry;
	while (sons) {
		buttonEntry = car (sons);
		RemoveEntry (&Dialogs, buttonEntry);
		sons = cdr (sons);
	}
} 

void PR_TOOLBARCREATE ()
{
	DlgEntry dialogEntry;
	UINT nID = nToolbarID;

	CString PathName;
	char titleText[MAX_CHARS] = "";
	char fileText[MAX_CHARS] = "";
	int iPos = 0;			// default docking side
	int x1 = 0;				// default x pos (left)
	int y1 = 0;				// default y pos (top)
	int leftBorder = 0;		// default left border
	int topBorder = 0;		// default top border
	int rightBorder = 0;	// default right border
	int bottomBorder = 0;	// default bottom border

	node parent = args[0];	// nome eventuale finestra Tarta 
	node name = args[1];	// nome toolbar
	node title = args[2];	// titolo toolbar
	node filename = args[3];// file bitmap
	node pos = args[4];		// posizione [x y]
	node dim = args[5];		// dimensione (pixel o caselle?)
	node borders = args[6];	// dimensione bordi [n1 n2 n3 n4]
	node buttons = args[7];	// lista descrizioni bottoni

	int parentHandle = -1;
	CFrameWnd* parentWindow = NULL;
	if ((parent != NULLP) && (parent != pc_VUOTA) && (parent != S_VUOTA)) {
		int parentDev = nome2dev (parent);
		if ((parentDev != -1) && (_fstato [parentDev] & O_TARTA)) {
			parentHandle = dev2handle (parentDev);
			parentWindow = (CFrameWnd*) TurtleHandle2Frame (parentHandle);
		}
		else err2 (52, parent);
	}

	if (types[2] == LIST) {
		if (title != NULLP)
			MemScrivi (titleText, title);
		else
			getnam (titleText, name);
	}
	else {
		if (! MatchKw (title, pc_FALSO))
			getnam (titleText, title);
	}

	if (types[3] == LIST) MemScrivi (fileText, filename);
	else getnam (fileText, filename);
//	PathName += (strlen (fileText) > 0) ? fileText : "toolbar.bmp";
	PathName = fileText;

//> 000315
//	if (pos != NULLP) {
//		x1 = rnum_int (car (pos)); pos = cdr (pos);
//		if (pos != NULLP) {
//			y1 = rnum_int (car (pos));
//		}
//	}
	if (typ (pos) == LBCD)
		iPos = num_int (pos);
	else if (pos != NULLP) {
		x1 = num_int (car (pos)); pos = cdr (pos);
		if (pos != NULLP) {
			y1 = num_int (car (cdr (pos)));
		}
	}
	if ((iPos < 0) || (iPos > 4)) {
		err2 (5, args[4]); return;	// codice di posizione illegale
	}
//<
	if (borders != NULLP) {
		leftBorder = rnum_int (car (borders)); borders = cdr (borders);
		if (borders != NULLP) {
			topBorder = rnum_int (car (borders)); borders = cdr (borders);
			if (borders != NULLP) {
				rightBorder = rnum_int (car (borders)); borders = cdr (borders);
				if (borders != NULLP) {
					bottomBorder = rnum_int (car (borders));
				}
			}
		}
	}

	CToolFrame* toolFrame = NULL;
	CToolBar* toolBar;
	CToolTipCtrl* toolTips;
	int dialogType = TOOLBAR;
	int dialogId = 0;
	Geometry geometry = NULLP;
	node setup = NULLP;
	node sons = NULLP;
	dialogEntry = CreateDlgEntry(toolFrame, dialogType, dialogId, parent, title, geometry, setup, sons);
	AddEntry(&Dialogs, dialogEntry, name);

	int nButtons = lstlen (buttons);
	int nBitmaps = nButtons;
	int i;
	node button;
	int buttonStyle;
	int iBitmap;
	int iString;
	int nString = 0;
	node buttonLabel;
	char buttonText[MAX_CHARS] = "";
	CString buttonString = "";
	TBBUTTON buttonArray [MAX_BUTTONS+1];
	for (i=0; i < nButtons; ++i) {
		button = _ele (i+1, buttons); // should be 1: TBSTYLE_BUTTON
		buttonStyle = rnum_int (_ele (1, button));
		if (buttonStyle == TBSTYLE_SEP) {	// separatore ?
			buttonArray[i].iBitmap = -1;
			buttonArray[i].idCommand = 0;
			buttonArray[i].iString = -1;
		}
		else {
			iBitmap = rnum_int (_ele (2, button)) - 1;
			buttonLabel = _ele (3, button);
			iString = -1;
			if (buttonLabel != NULLP) {
				if (typ(buttonLabel) == LIST) MemScrivi (buttonText, buttonLabel);
				else getnam (buttonText, buttonLabel);
				buttonString += buttonText;
				buttonString += '\0';
				iString = nString++;
			}
			buttonArray[i].iBitmap = iBitmap;
			buttonArray[i].idCommand = nID++;
			buttonArray[i].iString = iString;
		}
		buttonArray[i].fsStyle = buttonStyle;
		buttonArray[i].fsState = TBSTATE_ENABLED;
		buttonArray[i].dwData = 0;
	}

//	DWORD tbStyle = 0;
	DWORD dwCtrlStyle = 0;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_ANY;
//> 000315
	dwStyle |= CBRS_TOOLTIPS;
//<
	CSize buttonSize;
	CSize bitmapSize (16, 15);
	if (nString == 0) {
		dwCtrlStyle |= TBSTYLE_TRANSPARENT;
		buttonSize.cx = 24;
		buttonSize.cy = 22;
	}
	else {
		dwCtrlStyle |= TBSTYLE_FLAT; 
		buttonSize.cx = 36;
		buttonSize.cy = 34;
	}
	dwCtrlStyle |= TBSTYLE_WRAPABLE;  
//	dwCtrlStyle |= CBRS_SIZE_DYNAMIC;  // non serve ?

//> 000315
	// dim si riferisce ai bottoni ?
	if ((parentWindow) && (dim != NULLP)) {
		buttonSize.cx = rnum_int (car (dim)); dim = cdr (dim);
		if (dim != NULLP)
			buttonSize.cy = rnum_int (car (dim));
	}
//<

	int index;
	int hFile;
	hFile = _open (PathName, _O_RDONLY | _O_RAW, _S_IWRITE | _S_IREAD);
	if (hFile <= 0)
		err2 (21, filename);
	else {

		int xDim = 6 + leftBorder + nButtons * (buttonSize.cx - 1) + rightBorder;
		int yDim = 24 + topBorder + (buttonSize.cy - 1) + bottomBorder;

		// dim si riferisce alla bottoniera ?
//> 000315
//		if (dim != NULLP) {
		if ((! parentWindow) && (dim != NULLP)) {
//<
			xDim = rnum_int (car (dim)); dim = cdr (dim);
			if (dim != NULLP)
				yDim = rnum_int (car (dim));
		}

		BOOL result;
		int x2 = x1 + xDim;
		int y2 = y1 + yDim;
		CRect posDim (x1, y1, x2, y2);
		CRect toolbarBorders (leftBorder, topBorder, rightBorder, bottomBorder);

		// l'oggetto toolFrame viene creato comunque, perche' deve diventare
		// lo "owner" dell'oggetto toolBar, quale che sia il "parent"
		toolFrame = new (CToolFrame);
		DWORD frameStyle = 0;
		if (! parentWindow)
			frameStyle |= (WS_CAPTION|WS_SYSMENU|WS_VISIBLE);
		result =
			toolFrame->Create (NULL, titleText, frameStyle, posDim, NULL, 0);
		DialogPutWindow(dialogEntry, toolFrame);

		toolBar = new (CToolBar);
		CFrameWnd* pParentFrame = (parentWindow) ? parentWindow : toolFrame;
		result =
		toolBar->CreateEx (pParentFrame, dwCtrlStyle, dwStyle, toolbarBorders, AFX_IDW_TOOLBAR);
		toolBar->SetWindowText(titleText);

#ifdef DOC
void CWnd::SetOwner (CWnd* pOwnerWnd);

Sets the current window’s owner to the specified window object.
This owner can then receive command messages from the current window object.
By default, the parent of the current window is its owner.
It is often useful to establish connections between window objects that are unrelated
to the window hierarchy.
For example, CToolBar sends notifications to its owner instead of to its parent.
This allows the toolbar to become the child of one window
(such as an OLE container application window)
while sending notifications to another window (such as the in-place frame window).
...
#endif // DOC

		toolBar->SetOwner(toolFrame);	// nuovo: 990917
		toolBar->m_bAutoDelete = TRUE;
		// a toglierlo peggiora la situazione ?
		toolBar->SetSizes(buttonSize, bitmapSize);
		CToolBarCtrl& toolBarCtrl = toolBar->GetToolBarCtrl();
		index = toolBarCtrl.AddStrings (buttonString);
		CDC *pDC = pParentFrame->GetDC();
		CPalette* pTargetPalette = NULL;
		CBitmap* pBitmap =
			LoadBitMapFromFile(hFile, pDC, pTargetPalette);
		index = toolBarCtrl.AddBitmap (nBitmaps, pBitmap);
		result = toolBarCtrl.AddButtons (nButtons, &buttonArray[0]);
		pParentFrame->ShowControlBar(toolBar, TRUE, FALSE);

		node button;
		int id;
		node setup;
		for (i=0; i < nButtons; ++i) {
			if (buttonArray[i].fsStyle != TBSTYLE_SEP) {
				button = _ele (i+1, buttons);
				id = buttonArray[i].idCommand;
				setup = _ele (4, button);
				AddToolbarButton (dialogEntry, id, setup);
			}
		}

///		delete (pBitmap);
//		toolFrame->ReleaseDC (pDC);	// 990917
		pParentFrame->ReleaseDC (pDC);
		_close (hFile);

//>000315
		toolTips = new(CToolTipCtrl);
		toolTips->Create(toolBar, TTS_ALWAYSTIP);
		CRect r;
		CString text;
		for (i=0; i < nButtons; ++i) {
			if (buttonArray[i].fsStyle != TBSTYLE_SEP) {
				toolBar->GetItemRect(i, &r);
				toolBar->GetButtonText(i, text);
				VERIFY(toolTips->AddTool(toolBar, text, &r, buttonArray[i].idCommand));
			}
		}
		toolTips->Activate(TRUE);
		toolBar->GetToolBarCtrl().SetToolTips(toolTips);
//<

		toolFrame->m_uFirstBtn = nToolbarID;
		toolFrame->m_uLastBtn = nToolbarID + nButtons - 1;
		nToolbarID += nButtons;
		toolFrame->m_toolBar = toolBar;
		toolFrame->m_parentHandle = parentHandle;

		if (parentWindow) {
			toolBar->EnableDocking (CBRS_ALIGN_ANY);
			parentWindow->EnableDocking (CBRS_ALIGN_ANY);
//> 000315
//			parentWindow->DockControlBar(toolBar, AFX_IDW_DOCKBAR_TOP, NULL);
			parentWindow->DockControlBar(toolBar, nDockBarIDs [iPos], NULL);
//<
		}

		int nRows = toolBarCtrl.GetRows ();
		// se docked in Tarta, non funziona bene
		if ((! parentWindow) && (nRows > 1)) {
//			perche' non funziona bene ?
//			DWORD dwButtonSize = toolBarCtrl.GetButtonSize ();
//			int buttonHeight = HIWORD(dwButtonSize);
//			toolBar->SetHeight (nRows * buttonHeight);
//			buttonSize.cy *= nRows; 
			buttonSize.cy *= (buttonSize.cy + 1) * nRows; 
			toolBar->SetSizes(buttonSize, bitmapSize);
			if (! parentWindow) {
				toolFrame->ShowControlBar(toolBar, FALSE, FALSE);
				toolFrame->ShowControlBar(toolBar, TRUE, FALSE);
			}
		}

		if (! parentWindow) {
			BOOL bErase = TRUE;	// erase background
			toolFrame->Invalidate (bErase);
			toolFrame->UpdateWindow ();
//			toolFrame->BeginModalState ();
		}
	}
}

void PR_TOOLBARSETPOS ()
{
	DlgEntry dialogEntry;
	if (dialogEntry = NameToEntry (Dialogs, args[0], TOOLBAR)) {
		CToolFrame* toolFrame = (CToolFrame *) DialogGetWindow (dialogEntry);
		CToolBar* toolBar = toolFrame->m_toolBar;
		int parentHandle = toolFrame->m_parentHandle;
		if (parentHandle == -1) {
			err2 (5, args[0]); return;	// toolbar creata senza finestra asscociata
		}
		CFrameWnd* parentWindow = (CFrameWnd*) TurtleHandle2Frame (parentHandle);

		node pos = args[1];
		int iPos = 0;
		int x = 0, y = 0;
		if (typ (pos) == LBCD)
			iPos = num_int (pos);
		else {
			x = num_int (car (pos));
			y = num_int (car (cdr (pos)));
		}
		if ((iPos < 0) || (iPos > 4)) {
			err2 (5, args[1]); return;	// codice di posizione illegale
		}

		if (iPos == 0)
			parentWindow->FloatControlBar(toolBar, CPoint (x, y), CBRS_ALIGN_TOP);
		else {
			int nDockBarID = nDockBarIDs [iPos];
			parentWindow->DockControlBar(toolBar, nDockBarID, NULL);
		}
	}
	else err2 (52, args[0]);
}

//	vedi WindowDelete (entry) in ILGUI.CPP
void PR_TOOLBARDELETE ()
{
	DlgEntry dialogEntry;
	if (dialogEntry = NameToEntry (Dialogs, args[0], TOOLBAR)) {
		CToolFrame* toolFrame = (CToolFrame *) DialogGetWindow (dialogEntry);
		node sons = GetSons (dialogEntry);
		RemoveToolbarButtons (sons);
		RemoveEntry(&Dialogs, dialogEntry);
		toolFrame->ShowControlBar(toolFrame->m_toolBar, FALSE, FALSE); // ??
		// cerca di riciclare range di ID
		if (toolFrame->m_uLastBtn == (nToolbarID-1))
			nToolbarID = toolFrame->m_uFirstBtn;
//		delete (toolFrame);
		toolFrame->DestroyWindow();
	}
	else err2 (52, args[0]);
}

void CToolFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect clientRect;
	GetClientRect (&clientRect);
	HBRUSH stockBrushHandle = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
	dc.FillRect (&clientRect, CBrush::FromHandle(stockBrushHandle));

	// Do not call CMiniFrameWnd::OnPaint() for painting messages
}
