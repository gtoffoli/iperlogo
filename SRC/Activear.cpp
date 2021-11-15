// activear.cpp : implementation file
//
/*	ACTIVEAR.CPP

	981103	modificato ordine argomenti di SETACTIVEAREA e ACTIVEAREA
			per compatibilita' con MswLogo
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "logo.h"

#include "activear.h"
#include "turtlefr.h"
#include "turtlevw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActiveArea dialog


CActiveArea::CActiveArea(CWnd* pParent /*=NULL*/)
	: CDialog(CActiveArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActiveArea)
//	m_areaXhigh = 0;
//	m_areaXlow = 0;
//	m_areaYhigh = 0;
//	m_areaYlow = 0;
	//}}AFX_DATA_INIT

	CTurtleView* turtle = (CTurtleView*) ((CTurtleFrame*) pParent)->GetActiveView();
//	turtle->GetActiveArea (&m_areaXlow, &m_areaXhigh, &m_areaYlow, &m_areaYhigh);
	turtle->GetActiveArea (&m_areaXlow, &m_areaYlow, &m_areaXhigh, &m_areaYhigh);
}

void CActiveArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActiveArea)
	DDX_Text(pDX, IDC_AREA_XHIGH, m_areaXhigh);
	DDX_Text(pDX, IDC_AREA_XLOW, m_areaXlow);
	DDX_Text(pDX, IDC_AREA_YHIGH, m_areaYhigh);
	DDX_Text(pDX, IDC_AREA_YLOW, m_areaYlow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CActiveArea, CDialog)
	//{{AFX_MSG_MAP(CActiveArea)
	ON_BN_CLICKED(IDRESET, OnReset)
	ON_BN_CLICKED(IDSELECTION, OnSelection)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CActiveArea message handlers

void CActiveArea::OnOK()
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);	
	CTurtleView* turtle = (CTurtleView*) ((CTurtleFrame*) GetParent ())->GetActiveView();
//	turtle->SetActiveArea (m_areaXlow, m_areaXhigh, m_areaYlow, m_areaYhigh);
	turtle->SetActiveArea (m_areaXlow, m_areaYlow, m_areaXhigh, m_areaYhigh);
	
	CDialog::OnOK();
}

void CActiveArea::OnReset()
{
	// TODO: Add your control notification handler code here

	CTurtleView* turtle =
		(CTurtleView*) ((CTurtleFrame*) GetParent ())->GetActiveView();
	turtle->ResetActiveArea ();
//	turtle->GetActiveArea (&m_areaXlow, &m_areaXhigh, &m_areaYlow, &m_areaYhigh);
	turtle->GetActiveArea (&m_areaXlow, &m_areaYlow, &m_areaXhigh, &m_areaYhigh);

	UpdateData(FALSE);	
}

void CActiveArea::OnSelection() 
{
	// TODO: Add your control notification handler code here
	
	CTurtleView* turtle =
		(CTurtleView*) ((CTurtleFrame*) GetParent ())->GetActiveView();
	if (turtle->SelectionToActiveArea ()) {
//		turtle->GetActiveArea (&m_areaXlow, &m_areaXhigh, &m_areaYlow, &m_areaYhigh);
		turtle->GetActiveArea (&m_areaXlow, &m_areaYlow, &m_areaXhigh, &m_areaYhigh);
		UpdateData(FALSE);	
	}
}
