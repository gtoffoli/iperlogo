// Modeless.cpp : implementation file
//
/* MODELESS.CPP

991015	completato case WM_COMMAND in WindowProc (arrivavano messaggi da CEdit !)
991008	aggiunto OnUpdateControl, modificato OnInitDialog
991007	pulizia; modificato o commentato alcune righe con /// (3 slash)
		non usare più Class Wizard !!!
990623	rinominato GetParent -> DialogGetParent
980831	eliminato extern "C" per conversione a C++ dei moduli C
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "Modeless.h"

#include "ilcpp.h"
#include "ildialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// extern "C"
extern
DlgList Dialogs;   

/////////////////////////////////////////////////////////////////////////////
// CModelessDialog dialog


CModelessDialog::CModelessDialog(CWnd* pParent /*=NULL*/)
///	: CDialog(CModelessDialog::IDD, pParent)
	: CDialog()
{
	//{{AFX_DATA_INIT(CModelessDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CModelessDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelessDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelessDialog, CDialog)
	//{{AFX_MSG_MAP(CModelessDialog)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(2000, 10000, OnUpdateControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelessDialog message handlers


void CModelessDialog::OnUpdateControl(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

BOOL CModelessDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

#ifdef OLD_MODAL
    DlgEntry entry = WinToEntry (Dialogs, this);
    int controlType = GetType (entry);
	if (controlType == MODAL) {
		node setup = GetSetup (entry);
		if(setup)
			NestedExec (setup);	
	}
#else // OLD_MODAL
	m_bCanceled = FALSE;	// 991004: per simulare dialogo modale
#endif // OLD_MODAL

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CModelessDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	LRESULT result = (IsWindow(m_hWnd)) ?                  
		CDialog::WindowProc(message, wParam, lParam) :
		NULL;

	HWND hwndCtl;
    int nCtrlID;
    DlgEntry entry;
    int controlType;
    int minPos, maxPos, curPos;
    node logoList;
    
    if (message == WM_COMMAND) {
//>991015
		WORD wNotifyCode = HIWORD(wParam);
		if (wNotifyCode == BN_CLICKED) {
//<
			nCtrlID = LOWORD(wParam);
			entry = IdToEntry (Dialogs, nCtrlID);
			if (entry) {
				controlType = GetType(entry);
	        	logoList = GetSetup(entry);
	        	switch (controlType) {
	        	case BUTTON:
					NestedExec(logoList);
				break;
				}
	        }
			else
				TRACE("invalid CtrlID");
			ASSERT(entry);
//>991015
		}
//<
    }
    if ((message == WM_HSCROLL) || (message == WM_VSCROLL)) {
		hwndCtl = (HWND) lParam;
		nCtrlID = ::GetDlgCtrlID(hwndCtl);
		entry = IdToEntry (Dialogs, nCtrlID);
		ASSERT (entry);
		if (! entry)  TRACE("invalid CtrlID");
		else {
			CScrollBar *ScrollBar = (CScrollBar *) DialogGetWindow(entry);
			ScrollBar->GetScrollRange(&minPos, &maxPos);
			curPos = ScrollBar->GetScrollPos();
    		switch (LOWORD(wParam)) {
			case SB_BOTTOM:			// Scroll to bottom. 
			break; 
			case SB_ENDSCROLL:		// End scroll. 
        		logoList = GetSetup(entry);
				NestedExec(logoList);
			break; 
			case SB_LINEDOWN:		// Scroll one line down.
				ScrollBar->SetScrollPos(min(maxPos, curPos+1));
			break; 
			case SB_LINEUP:			// Scroll one line up. 
				ScrollBar->SetScrollPos(max(minPos, curPos-1));
			break; 
			case SB_PAGEDOWN:		// Scroll one page down. 
				ScrollBar->SetScrollPos((maxPos+curPos+1) / 2);
			break; 
			case SB_PAGEUP:			// Scroll one page up. 
				ScrollBar->SetScrollPos((minPos+curPos) / 2);
			break; 
			case SB_THUMBPOSITION:	// Scroll to absolute position. 
			case SB_THUMBTRACK:		// Drag scroll box (thumb) to specified position.
				ScrollBar->SetScrollPos(HIWORD(wParam));
			break; 
    		}
		}
    }

	return result;                  
}

void CModelessDialog::DialogCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
	m_bCanceled = TRUE;		// 991004: per simulare dialogo modale
}
