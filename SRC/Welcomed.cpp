// welcomed.cpp : implementation file
//
/*
   980218 - Aggiunto argomento a LoadBitmapFromFile
   980122 - Disattivato OnPaint !!!! NO
  */

#include "stdafx.h"
#include "iperlogo.h"
#include "welcomed.h"
#include "CDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define NEW_WELCOME

/////////////////////////////////////////////////////////////////////////////
// CWelcomeDialog dialog


CWelcomeDialog::CWelcomeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWelcomeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWelcomeDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_counter = 0;                                                                
}

void CWelcomeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWelcomeDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWelcomeDialog, CDialog)
	//{{AFX_MSG_MAP(CWelcomeDialog)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWelcomeDialog message handlers

#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>

// extern CBitmap* LoadBitMapFromFile(int hFile);
extern CBitmap* LoadBitMapFromFile(int hFile, CDC* pDC, CPalette*);

int Welcome () {
//	UINT nIDTemplate = IDD_WELCOME;
 	CWnd* pParentWnd = NULL;
	CWelcomeDialog welcomeDialog (pParentWnd);
	int result = welcomeDialog.DoModal();
	return result;
}

extern CREATESTRUCT Welcom;
void CWelcomeDialog::OnInitialUpdate()
{
	m_counter = 0;
}

BOOL CWelcomeDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_counter = 0;

#ifdef NEW_WELCOME
	// Get DC for the window
	CDC* pDC = GetDC();
	// Create and select a palette
	m_pPalette = CPaletteFromStock();	// 980305
	CPalette* pOldPalette =
		pDC->SelectPalette (m_pPalette, FALSE);
	pDC->RealizePalette();
	// Load a bitmap from file
	CString PathName = "ilcover.bmp";
	int handle = _open (PathName, _O_RDONLY | _O_RAW, _S_IWRITE | _S_IREAD);
	if (handle) {
		m_pBitmap = LoadBitMapFromFile(handle, pDC, m_pPalette);
		_close (handle);
	}
	// Release DC
	ReleaseDC (pDC);
#endif // NEW_WELCOME

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWelcomeDialog::OnPaint()
{
	BringWindowToTop ();	// disattivato 980305 ?

	if (m_counter == 0) {
		++m_counter;
		MoveWindow(Welcom.x, Welcom.y, Welcom.cx, Welcom.cy, TRUE);
	}

	CPaintDC dc(this); // device context for painting

	CPalette* pOldPalette =
		dc.SelectPalette (m_pPalette, FALSE);
	dc.RealizePalette();

	// TODO: Add your message handler code here

	int result;
	if (m_counter == 1) {
		++m_counter;
		GetDlgItem(IDOK)->
			MoveWindow(Welcom.cx*7/16, Welcom.cy*9/10, Welcom.cx/8, Welcom.cy/16, TRUE);
	}

	if (m_counter == 2) {
		++m_counter;

//		CBitmap* pOldBitmap;
		if (m_pBitmap) {	
			CSize bitSize = m_pBitmap->GetBitmapDimension();
			RECT clientArea;
			GetClientRect(&clientArea);
			int cx2 = clientArea.right - clientArea.left;
			int cy2 = clientArea.bottom - clientArea.top;
			int cx1 = bitSize.cx;
			int cy1 = bitSize.cy;
			int cx, cy, x1, y1, x2, y2;
			if (cx2 <= cx1) {
				x2 = 0; x1 = (cx1-cx2) / 2; cx = cx2;
			} else {
				x1 = 0; x2 = (cx2-cx1) /2; cx = cx1;
			}
			if (cy2 <= cy1) {
				y2 = 0; y1 = (cy1-cy2) / 2; cy = cy2;
			} else {
				y1 = 0; y2 = (cy2-cy1) /2; cy = cy1;
			}
			// We attach the bitmap to an auxiliary memory DC  
			CDC memDC;
			result = memDC.CreateCompatibleDC(NULL);
			CBitmap* pOldBit = memDC.SelectObject(m_pBitmap);

			// We copy it to the control  
			result = dc.BitBlt(x2, y2, cx, cy, &memDC, x1, y1, SRCCOPY);

			memDC.SelectObject(pOldBit);
			memDC.DeleteDC();
		}
	}
	if (m_counter == 3) {
		++m_counter;
		GetDlgItem(IDOK)->
			SetWindowPos(&wndTop/*this*/, Welcom.cx*7/16, Welcom.cy*9/10, Welcom.cx/8, Welcom.cy/16,
									SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
//		GetDlgItem(IDOK)->SetFocus();
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CWelcomeDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	if (m_pPalette) {
		CPaintDC dc(this);
		dc.SelectPalette (m_pOldPalette, FALSE);
		dc.RealizePalette();
		m_pPalette->DeleteObject();
	}

	if (m_pBitmap)
		delete(m_pBitmap);

	CDialog::OnClose();
}
