// modal.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "modal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModaDialog dialog


CModaDialog::CModaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CModaDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModaDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CModaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModaDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CModaDialog, CDialog)
	//{{AFX_MSG_MAP(CModaDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModaDialog message handlers
