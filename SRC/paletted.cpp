// PaletteD.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "PaletteD.h"
#include "CDib.h"
#include "turtlevw.h"
#include "turtlefr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define FILL_SOLID
// #define SOLID_BRUSH
#define FILL_DIB

extern CIperlogoApp theApp;

COLORREF IndexToColor(int index)
{
	PALETTEENTRY entry;
	theApp.m_pPalette->GetPaletteEntries (index, 1, &entry);
	COLORREF color =
		RGB(entry.peRed, entry.peGreen, entry.peBlue);
	return color;
}

CBrush* BrushFromColorAndPalette(CDC *pDC, COLORREF color)
{
	HGLOBAL hDib = ColorToDib (pDC, color, 8);
	BITMAPINFOHEADER* pDib =
		(BITMAPINFOHEADER*) GlobalLock (hDib);
	if (pDib == NULL)
		return NULL;
	CBitmap* pBitmap =
		DibRealize (pDib, pDC, theApp.m_pPalette);
	CBrush* pBrush = new(CBrush);
	pBrush->CreatePatternBrush(pBitmap);
	pBitmap->DeleteObject();
	delete(pBitmap);
	GlobalFree (hDib);
	return pBrush;
}

// CPaletteDialog dialog


CPaletteDialog::CPaletteDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaletteDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPaletteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaletteDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaletteDialog, CDialog)
	//{{AFX_MSG_MAP(CPaletteDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_PEN, OnPen)
	ON_BN_CLICKED(ID_FILL, OnFill)
	ON_BN_CLICKED(ID_BK, OnBk)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


int CALLBACK EXPORT CallBack (VOID* par1, LPARAM par2)
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CPaletteDialog message handlers


BOOL CPaletteDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CRect clientArea;
	GetClientRect(&clientArea);
	int width = clientArea.Width();
	m_step = width / 16;
	m_side = m_step - 2;
	m_offset = (width - (m_step * 16)) / 2;
	m_index = 0;
	m_nEntries = theApp.m_pPalette->GetEntryCount();

	CTurtleFrame* turtleFrame = (CTurtleFrame*) GetOwner();
	m_view = (CTurtleView*) turtleFrame->GetActiveView();
//	ButtonCreate ();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaletteDialog::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
	
	int index;
	CPoint origin;
	COLORREF color;
	
//	int nEntries = theApp.m_pPalette->GetEntryCount();
	CPalette* pOldPalette =
		dc.SelectPalette(theApp.m_pPalette, FALSE);	   
	
	BringWindowToTop ();
	SetFocus();
	UINT nRemapped = dc.RealizePalette();
	
	for (index=0; index<m_nEntries; index++) {
		color = IndexToColor(index);
		DrawBox (&dc, index, color, (index == m_index));
	}

	int j;
	for (j=0; j<3; j++) {
		switch (j) {
		case 0:
			color = m_view->GetPenColor();
			index = 256 + 32 + 3;
			break;
		case 1:
			color = m_view->GetBkColor();
			index = 256 + 32 + 7;
			break;
		case 2:
			color = m_view->GetFloodColor();
			index = 256 + 32 + 11;
			break;
		}
		DrawBox (&dc, index, color, FALSE);
	}
	
	//	nRemapped = dc.RealizePalette(); 980305 tolto
}

int CPaletteDialog::PointToIndex(CPoint point)
{
	int row, column;
	int index = -1;
	column = (point.x - m_offset) / m_step;
	row = (point.y - m_offset) / m_step;
	if (	(column >=0) && (column < 16)
		&&	(row >=0) && (row < 16)
		) {
		index = row * 16 + column;
		if (index >= 256)
			index = -1;
	}
	return index;
}

CPoint CPaletteDialog::IndexToOrigin(int index)
{
	int row, column;
	int x, y;
	row = index / 16;
	column = index - (row * 16);
	x = column * m_step + m_offset;
	y = row * m_step + m_offset;
	return CPoint(x, y);
}

void CPaletteDialog::DrawBox (CPaintDC* pDC, int index, COLORREF color, BOOL isSelected)
{
	CPoint origin = IndexToOrigin (index);

#ifdef FILL_DIB
	CBrush* pBrush =
		BrushFromColorAndPalette (pDC, color);
	if (pBrush != NULL) {
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(pBrush);
		pDC->FillRect(CRect(origin.x, origin.y,
				origin.x+m_side, origin.y+m_side), pBrush);
		pDC->SelectObject(pOldBrush);
		pBrush->DeleteObject();
		delete(pBrush);
	}
#endif // FILL_DIB
		
#ifdef FILL_SOLID
		pDC->FillSolidRect(origin.x, origin.y,
							m_side, m_side, color);
#endif // FILL_SOLID
		
#ifdef SOLID_BRUSH
		CBrush brush;
		CBrush *pOldBrush;
		brush.CreateSolidBrush(color);
		pOldBrush = pDC->SelectObject(&brush);
		pDC->FillRect(CRect(origin.x, origin.y,
				origin.x+m_side, origin.y+m_side), &brush);
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
#endif // SOLID_BRUSH
		
	if (isSelected) {
		COLORREF borderColor;
		int sumColor =
			GetRValue(color) + GetGValue(color) + GetBValue(color);
		if (sumColor >= 255) 
			borderColor = RGB(0,0,0);
		else
			borderColor = RGB(255,255,255);
		CPen newPen;
		newPen.CreatePen(PS_SOLID, 1, borderColor);
		CPen* oldPen = pDC->SelectObject(&newPen);

		// draws box border with 4 lines
		int xLeft = origin.x;
		int yTop = origin.y;
		int xRight = xLeft + m_side -1;
		int yBottom = yTop + m_side -1;
		pDC->MoveTo(xLeft, yBottom);
		pDC->LineTo(xRight, yBottom);
		pDC->LineTo(xRight, yTop);
		pDC->LineTo(xLeft, yTop);
		pDC->LineTo(xLeft, yBottom);

		pDC->SelectObject(oldPen);
	}
}

void CPaletteDialog::OnPen() 
{
	// TODO: Add your control notification handler code here

	COLORREF color = IndexToColor (m_index);
	if (color != m_view->GetPenColor()) {
		m_view->SetPenColor(color);
		InvalidateRect (NULL);
	}
}

void CPaletteDialog::OnFill() 
{
	// TODO: Add your control notification handler code here
	
	COLORREF color = IndexToColor (m_index);
	if (color != m_view->GetFloodColor()) {
		m_view->SetFloodColor(color);
		InvalidateRect (NULL);
	}
}

void CPaletteDialog::OnBk() 
{
	// TODO: Add your control notification handler code here
	
	COLORREF color = IndexToColor (m_index);
	if (color != m_view->GetBkColor()) {
		m_view->SetBkColor(color);
		m_view->BitmapNew();
		InvalidateRect(NULL);
	}
}

void CPaletteDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CPaletteDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);

	int index = PointToIndex(point);
	if ((index >=  0) && (index < m_nEntries) && (index != m_index)) {
		m_index = index;
		InvalidateRect (NULL);
	}

}

#ifdef DOC

#define IDC_COLORBUTTON		10

CButton* CPaletteDialog::ButtonCreate (CRect rect, COLORREF color)
{
	CButton *Button;
	static UINT nID = IDC_COLORBUTTON;

	CWnd* pParentWnd = this;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	Button = new (CButton);
	int result = Button->Create(
		"",
		dwStyle,
		rect,
		pParentWnd,
		++nID);
	if (result) {
		return Button;
	}
	else
		return NULL;
}

#endif // DOC
