// turtlefr.cpp : implementation file
//
/* TURTLEFR.CPP

010605	menu sotto condizione iperlogoIsMenu
000419	differenziato menu su turtleMode; disattivato OVsMenuCollapse
991001  revisione di zoom, centratura e resize:
		in OnGetMinMaxInfo abolito limitazioni
981123	aggiunto SetTitleBar e SetMenuBar
980831	eliminato extern "C" per conversione a C++ dei moduli C
980625	introdotto menu MIDI
980624	introdotto opzione ForceTopmost
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "turtlefr.h"
#include "turtlevw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern int turtleMode;		// modalita' finestra TARTA da aprire

extern
char DefaultTurtle[];
extern CREATESTRUCT Turtle;
extern char TurtleFont[];
extern BOOL ForceTopmost;
extern node FontToList (LOGFONT* lpLogFont);
extern int iperlogoIsMenu;	// 010605

/////////////////////////////////////////////////////////////////////////////
// CTurtleFrame

IMPLEMENT_DYNCREATE(CTurtleFrame, CFrameWnd)

CTurtleFrame::CTurtleFrame()
{
#ifdef OVs
	int i;
	for (i=0; i<N_OVS_TOOLBARS; i++) {
		m_pToolbar[i] = NULL;
		m_toolbarShown [i] = FALSE;
	}
#endif // OVs
}

CTurtleFrame::~CTurtleFrame()
{
#ifdef OVs
	int i;
	for (i=0; i<N_OVS_TOOLBARS; i++)
		if (m_pToolbar[i])
			delete(m_pToolbar[i]);
#endif // OVs
}

#define yesTitleStyles (WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define noTitleStyles (WS_POPUP /* | WS_BORDER */)
#define andTitleStyles (yesTitleStyles & noTitleStyles)

BOOL CTurtleFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (! CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.x=Turtle.x;
	cs.y=Turtle.y;

	cs.cx=Turtle.cx;
	cs.cy=Turtle.cy;
	cs.hMenu=0;
//>010605
//	if (Turtle.hMenu) {
	if (iperlogoIsMenu && Turtle.hMenu) {
//<010605
		if (turtleMode == 1)
		cs.hMenu=LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TURTLE_OBJ));
			else
		cs.hMenu=LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_TURTLE));
	}
	cs.style=cs.style &
		(~ (WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

	if (Turtle.lpszName == (char *) 0L)
		cs.style |= noTitleStyles;
	else
		cs.style |= yesTitleStyles;
	if (ForceTopmost)
		cs.dwExStyle |= WS_EX_TOPMOST;

	m_hMenu = NULL;	// 981123
	return TRUE;
}
                                                                
BEGIN_MESSAGE_MAP(CTurtleFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CTurtleFrame)
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTurtleFrame::SetTitleBar (BOOL setOn)
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

void CTurtleFrame::SetMenuBar (BOOL setOn)
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

#ifdef OVs
void CTurtleFrame::OVsMenuCollapse ()
{
	return;

	CMenu* turtleMenu = GetMenu ();
	if (! turtleMenu)
		return;
	turtleMenu->RemoveMenu(14, MF_BYPOSITION);	// Nudge
	turtleMenu->RemoveMenu(13, MF_BYPOSITION);	// Align
	turtleMenu->RemoveMenu(12, MF_BYPOSITION);	// Structure
	turtleMenu->RemoveMenu(11, MF_BYPOSITION);	// Rotate
	turtleMenu->RemoveMenu(10, MF_BYPOSITION);	// Drawing
	turtleMenu->RemoveMenu(9, MF_BYPOSITION);	// View
	turtleMenu->RemoveMenu(8, MF_BYPOSITION);	// Edit
	turtleMenu->RemoveMenu(7, MF_BYPOSITION);	// File
	turtleMenu->RemoveMenu(6, MF_BYPOSITION);	// sep
	turtleMenu->RemoveMenu(5, MF_BYPOSITION);	// Diagramma
	turtleMenu->RemoveMenu(4, MF_BYPOSITION);	// sep
	DrawMenuBar();
}
#endif // OVs

/////////////////////////////////////////////////////////////////////////////
// CTurtleFrame message handlers

void CTurtleFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnGetMinMaxInfo(lpMMI);	// default
//>991001
	return;
//<
	
	// customisation
	///static count = 0;
	static int count = 0;
	if (count++ > 0) {
		CTurtleView* turtleView = (CTurtleView*) GetActiveView();
		if (turtleView == NULL)
			return;
#ifdef OVs
		if (turtleView->m_isOVs)
			return;
#endif // OVs
		int horzMinPos, horzMaxPos, vertMinPos, vertMaxPos;
		turtleView->GetScrollRange(SB_HORZ, &horzMinPos, &horzMaxPos);
		turtleView->GetScrollRange(SB_VERT, &vertMinPos, &vertMaxPos);
		CRect rect (0, 0, 0, 0);
		turtleView->CalcWindowRect(&rect, CWnd::adjustOutside);
		int sx = (vertMaxPos) ? rect.right + 2 : 0;
		int sy = (horzMaxPos) ? rect.bottom : 0;
//>991001
//		int dx = min(lpMMI->ptMaxSize.x, turtleView->m_xPixel + sx + 6);
//		int dy = min(lpMMI->ptMaxSize.y, turtleView->m_yPixel + sy + 6);
		int dx = min(lpMMI->ptMaxSize.x,
				turtleView->m_xPixel * turtleView->m_zoomFactor + sx + 6);
		int dy = min(lpMMI->ptMaxSize.y,
				turtleView->m_yPixel * turtleView->m_zoomFactor + sy + 6);
//<
		lpMMI->ptMaxSize.x = dx;
		lpMMI->ptMaxSize.y = dy;
		lpMMI->ptMaxTrackSize.x = dx;
		lpMMI->ptMaxTrackSize.y = dy;
	}
}

void CTurtleFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
//	CFrameWnd::OnClose();

	CTurtleView* turtleView = (CTurtleView*) GetActiveView();
	turtleView->OnFileClose();
}

void CTurtleFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here

	// record current normal position of the window
	char name [MAX_CHARS];
	CTurtleView* turtleView = (CTurtleView*) GetActiveView();
	turtleView->GetName(name);
	if (strcmp (name, DefaultTurtle) == 0) { 
		WINDOWPLACEMENT wndPl;
		if (GetWindowPlacement (&wndPl)) {
			RECT pos = wndPl.rcNormalPosition;
			Turtle.x = pos.left;
			Turtle.y = pos.top;
			Turtle.cx = pos.right - pos.left;
			Turtle.cy = pos.bottom - pos.top;
		}
		MemScrivi (TurtleFont, FontToList(&(turtleView->m_logFont)));
	}
}
