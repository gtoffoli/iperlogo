// MciView.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "MciView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMciView

IMPLEMENT_DYNCREATE(CMciView, CScrollView)

CMciView::CMciView()
{
}

CMciView::~CMciView()
{
}


BEGIN_MESSAGE_MAP(CMciView, CScrollView)
	//{{AFX_MSG_MAP(CMciView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMciView drawing

void CMciView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMciView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMciView diagnostics

#ifdef _DEBUG
void CMciView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMciView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMciView message handlers
