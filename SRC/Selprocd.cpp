// selprocd.cpp : implementation file
//

//	980831	eliminato extern "C" per conversione a C++ dei moduli C

#include "stdafx.h"
#include "iperlogo.h"
#include "selprocd.h"

// extern "C"
void IniProc ();
// extern "C"
char* NextProc ();

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelprocDialog dialog


CSelprocDialog::CSelprocDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelprocDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelprocDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSelprocDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelprocDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelprocDialog, CDialog)
	//{{AFX_MSG_MAP(CSelprocDialog)
	ON_BN_CLICKED(IDALL, OnAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelprocDialog message handlers

BOOL CSelprocDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int result;
	char* name;
	CComboBox* combo = (CComboBox*) GetDlgItem(IDC_SELPROC);
	IniProc ();
	while ((name = NextProc()) != NULL) {
		result = combo->AddString(name);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelprocDialog::OnOK()
{
	// TODO: Add extra validation here

	CComboBox* combo = (CComboBox*) GetDlgItem(IDC_SELPROC);
	combo->GetWindowText (m_text, MAX_CHARS);

	CDialog::OnOK();
}

void CSelprocDialog::OnAll()
{
	// TODO: Add your control notification handler code here
	
	CComboBox* combo = (CComboBox*) GetDlgItem(IDC_SELPROC);
	combo->SetWindowText ("*");
}
