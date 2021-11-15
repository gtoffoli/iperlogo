// editorfr.cpp : implementation file
//
/* EDITORFR.CPP

010605	menu sotto condizione iperlogoIsMenu
980624	introdotto opzione ForceTopmost
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "editorfr.h"
#include "editorvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern char DefaultEditor[];
extern CREATESTRUCT Editor;
extern char EditorFont[];
extern node FontToList (LOGFONT* lpLogFont);
extern BOOL ForceTopmost;
extern int iperlogoIsMenu;	// 010605

/////////////////////////////////////////////////////////////////////////////
// CEditorFrame

IMPLEMENT_DYNCREATE(CEditorFrame, CFrameWnd)

CEditorFrame::CEditorFrame()
{
}

CEditorFrame::~CEditorFrame()
{
}

//> 981123
#define yesTitleStyles (WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define noTitleStyles (WS_POPUP /* | WS_BORDER */)
#define andTitleStyles (yesTitleStyles & noTitleStyles)
//<

BOOL CEditorFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (! CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.x=Editor.x;
	cs.y=Editor.y;
	cs.cx=Editor.cx;
	cs.cy=Editor.cy;
	cs.hMenu=0;
//>010605
//	if (Editor.hMenu)
	if (iperlogoIsMenu && Editor.hMenu)
//<010605
		cs.hMenu=LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_EDITOR));
	cs.style=cs.style &
		(~ (WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

	if (Editor.lpszName == (char *) 0L) {
		m_title[0] = '\0';
//		cs.style |= (WS_POPUP | WS_BORDER);
		cs.style |= noTitleStyles;
	} else {
		strcpy (m_title, Editor.lpszName);
//		cs.style |= (WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION  | WS_BORDER | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		cs.style |= yesTitleStyles;
	}	
//>980624
	if (ForceTopmost)
		cs.dwExStyle |= WS_EX_TOPMOST;
//<980624


	return TRUE;
}

static UINT NEAR WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);	// added                                                                

BEGIN_MESSAGE_MAP(CEditorFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CEditorFrame)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE( WM_FINDREPLACE, OnFindReplace )	// added

END_MESSAGE_MAP()


void CEditorFrame::SetTitleBar (BOOL setOn)
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

void CEditorFrame::SetMenuBar (BOOL setOn)
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

/////////////////////////////////////////////////////////////////////////////
// CEditorFrame message handlers


// The framework calls this member function as a signal that
// the CWnd or an application is to terminate.
// The default implementation calls DestroyWindow.
void CEditorFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	// close client view
	CEditorView* editorView = (CEditorView*) GetActiveView();
	editorView->OnFileClose();
}

void CEditorFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here

	// record current normal position of the window
	char name [MAX_CHARS];
	CEditorView* editorView = (CEditorView*) GetActiveView();
	editorView->GetName(name);
	if (strcmp (name, DefaultEditor) == 0) { 
		WINDOWPLACEMENT wndPl;
		if (GetWindowPlacement (&wndPl)) {
			RECT pos = wndPl.rcNormalPosition;
			Editor.x = pos.left;
			Editor.y = pos.top;
			Editor.cx = pos.right - pos.left;
			Editor.cy = pos.bottom - pos.top;
		}
		MemScrivi (EditorFont, FontToList(&(editorView->m_logFont)));
	}
}
