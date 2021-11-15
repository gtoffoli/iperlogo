// mainstat.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "mainstat.h"
#include "mainfrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainStatusDialog dialog


CMainStatusDialog::CMainStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainStatusDialog)
	m_nodes_free = 0;
	m_nodes_total = 0;
	m_nodes_used = 0;
	//}}AFX_DATA_INIT
}

void CMainStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainStatusDialog)
	DDX_Text(pDX, IDC_NODES_FREE2, m_nodes_free);
	DDX_Text(pDX, IDC_NODES_TOTAL2, m_nodes_total);
	DDX_Text(pDX, IDC_NODES_USED2, m_nodes_used);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CMainStatusDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainStatusDialog message handlers

void CMainStatusDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	
	((CMainFrame*) GetParent())->OnHideStatus();	// custom
}
