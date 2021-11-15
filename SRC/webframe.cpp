// webframe.cpp : implementation file
//
/*
010606	menu sotto condizione iperlogoIsMenu
990914  Override di OnWndMsg per soluzione problemi di focus
990708	implementato OnDestroy
990325	riattivato refresh in OnSetFocus (GL: meglio side-effect)
990318	ritoccato handler di OnSetFocus,
		poi annullato per side effect negativo su bottoni
990317	handler di OnSetFocus per ovviare a mancato rinfresco
*/

#define SOLUZIONE4

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "webframe.h"
#include "webview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char DefaultBrowser[];
extern CREATESTRUCT Browser;
extern BOOL ForceTopmost;
extern int iperlogoIsMenu;	// 010606

/////////////////////////////////////////////////////////////////////////////
// CWebFrame

IMPLEMENT_DYNCREATE(CWebFrame, CFrameWnd)

CWebFrame::CWebFrame()
{
}

CWebFrame::~CWebFrame()
{
}

#define yesTitleStyles (WS_SYSMENU | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX)
#define noTitleStyles (WS_POPUP /* | WS_BORDER */)
#define andTitleStyles (yesTitleStyles & noTitleStyles)

BOOL CWebFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (! CFrameWnd::PreCreateWindow(cs)) return FALSE;

	cs.x=Browser.x;
	cs.y=Browser.y;
	cs.cx=Browser.cx;
	cs.cy=Browser.cy;
	cs.hMenu=0;
//>010605
//	if (Browser.hMenu)
	if (iperlogoIsMenu && Browser.hMenu)
//<010605
		cs.hMenu=LoadMenu(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_WEB));
	cs.style=cs.style &
		(~ (WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX));

	if (Browser.lpszName == (char *) 0L) {
		m_title[0] = '\0';
		cs.style |= noTitleStyles;
	} else {
		strcpy (m_title, Browser.lpszName);
		cs.style |= yesTitleStyles;
	}

//	cs.style |= (WS_VSCROLL | WS_HSCROLL);

	if (ForceTopmost)
		cs.dwExStyle |= WS_EX_TOPMOST;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CWebFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CWebFrame)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_GO, OnGo)
END_MESSAGE_MAP()


void CWebFrame::SetTitleBar (BOOL setOn)
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

void CWebFrame::SetMenuBar (BOOL setOn)
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
// CWebFrame message handlers

void CWebFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
//	CFrameWnd::OnClose();

	// close client view
	CWebView* webView = (CWebView*) GetActiveView();
	webView->OnFileClose();
}

void CWebFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
	// record current normal position of the window
	char name [MAX_CHARS];
	CWebView* webView = (CWebView*) GetActiveView();
	webView->GetName(name);
	if (strcmp (name, DefaultBrowser) == 0) { 
		WINDOWPLACEMENT wndPl;
		if (GetWindowPlacement (&wndPl)) {
			RECT pos = wndPl.rcNormalPosition;
			Browser.x = pos.left;
			Browser.y = pos.top;
			Browser.cx = pos.right - pos.left;
			Browser.cy = pos.bottom - pos.top;
		}
//		MemScrivi (BrowserFont, FontToList(&(webView->m_logFont)));
	}
}

afx_msg void CWebFrame::OnGo()
{
	CWebView* webView = (CWebView*) GetActiveView();
//	CDialogBar* dialogBar = (CDialogBar*) webView->m_dialogBar;
//	CEdit* editCtrl = (CEdit*) dialogBar->GetDlgItem(IDC_EDIT1);
	CEdit* editCtrl = (CEdit*) m_dialogBar->GetDlgItem(IDC_EDIT1);
	char url [MAX_CHARS];
	int l = editCtrl->GetLine(0, url, MAX_CHARS);
	if (l > 0) {
		url[l] = '\0';
		editCtrl->SetSel(0, l-1, FALSE);
		webView->Navigate2 (url, 0, NULL, NULL, NULL, 0);
	}
}

#ifdef HANDLE_NOTIFICATION

BOOL CWebFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

// The EN_CHANGE notification message is sent when the user has
// taken an action that may have altered text in an edit control.
// Unlike the EN_UPDATE notification message, this notification
// message is sent after the system updates the screen.
// The parent window of the edit control receives
// this notification message through the WM_COMMAND message. 

// The EN_KILLFOCUS notification message is sent when an edit control
// loses the keyboard focus.
// The parent window of the edit control receives this notification
// message through the WM_COMMAND message.

	CWebView* webView = (CWebView*) GetActiveView();
	CDialogBar* dialogBar = (CDialogBar*) webView->m_dialogBar;

	int isRelevant = FALSE;
	CEdit* editCtrl;
// #define FOCUS
// #ifdef FOCUS
	if (HIWORD(wParam) == EN_KILLFOCUS) {
		int idEditCtrl = (int) LOWORD(wParam);	// identifier of edit control 
		HWND hwndEditCtrl = (HWND) lParam;		// handle of edit control 
		editCtrl = (CEdit*) CWnd::FromHandle(hwndEditCtrl);
		isRelevant = TRUE;
	}
// #endif // FOCUS
// #ifdef CLICK
	if (HIWORD(wParam) == BN_CLICKED) {
		int idButton = (int) LOWORD(wParam);	// identifier of button 
		HWND hwndButton = (HWND) lParam;		// handle to button 
		editCtrl = (CEdit*) dialogBar->GetDlgItem(IDC_EDIT1);
		isRelevant = TRUE;
	}
// #endif // CLICK
	if (isRelevant) {
		char url [MAX_CHARS];
		int l = editCtrl->GetLine(0, url, MAX_CHARS);
		if (l > 0) {
			url[l] = '\0';
			editCtrl->SetSel(0, l-1, FALSE);
			webView->Navigate2 (url, 0, NULL, NULL, NULL, 0);
		}
	}

	return CFrameWnd::OnCommand(wParam, lParam);
}

#endif // HANDLE_NOTIFICATION

#ifndef SOLUZIONE4

// il refresh funziona,
// ma impedisce che funzioni il click sui bottoni !!!
void CWebFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CFrameWnd::OnSetFocus(pOldWnd);	// generato da class Wizard
	
	// TODO: Add your message handler code here
	((CWebView*) GetActiveView())->Refresh();
}

#endif // SOLUZIONE4

// il refresh non funziona;
// il click sui bottoni continua a funzionare
//void CWebFrame::OnLButtonUp(UINT nFlags, CPoint point) 
//{
//	// TODO: Add your message handler code here and/or call default
//
// 	CFrameWnd::OnLButtonUp(nFlags, point);	// generato da class Wizard
//
//	((CWebView*) GetActiveView())->Refresh();
//}


// Nota: ma i messaggi di mouse non arrivano qui !!!
BOOL CWebFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message > WM_MOUSEFIRST)
		if (message < WM_MOUSELAST)
	{
		BOOL bHandled = FALSE;
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd) {
			HWND hFocusWnd = pFocusWnd->m_hWnd;
			BOOL bHasFocus = (hFocusWnd == m_hWnd) || (hFocusWnd == GetActiveView()->m_hWnd);
			if (! bHasFocus)
				BOOL bHandled = CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
		}
		return bHandled || CWebFrame::OnWndMsg(message, wParam, lParam, pResult);
	}

	return CFrameWnd::OnWndMsg(message, wParam, lParam, pResult);
}
