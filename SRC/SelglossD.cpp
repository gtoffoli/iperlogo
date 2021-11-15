// SelglossD.cpp : implementation file
//
//	990722	modificato gestione di workDirectory e execDirectory
//	990707	esteso dialogo; modificato OnAll e OnOK
//	990510	cambiato nomi a campi di tabelle

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "selglossd.h"

//extern char WorkDirectory [];
extern CString execDirectory;

node DaoQuery (const char* dbName, const char* sql, BOOL withHeading, BOOL collapseRows);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelglossDialog dialog


CSelglossDialog::CSelglossDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelglossDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelglossDialog)
	m_textIni = _T("");
	m_iperlogo = FALSE;
	m_noalias = FALSE;
	m_superlogo = FALSE;
	//}}AFX_DATA_INIT
}


void CSelglossDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelglossDialog)
	DDX_Text(pDX, IDC_TEXT_INI, m_textIni);
	DDX_Check(pDX, IDC_CHECK_IPERLOGO, m_iperlogo);
	DDX_Check(pDX, IDC_CHECK_NOALIAS, m_noalias);
	DDX_Check(pDX, IDC_CHECK_SUPERLOGO, m_superlogo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelglossDialog, CDialog)
	//{{AFX_MSG_MAP(CSelglossDialog)
	ON_BN_CLICKED(IDALL, OnAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelglossDialog message handlers

#define listCat S
#define listEnv T

BOOL CSelglossDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

//	CString dbName = WorkDirectory;
//	dbName = dbName + "\\aiuti\\iperlogo.mdb";
	CString dbName = execDirectory + "\\aiuti\\iperlogo.mdb";
	node nome;
	char name [MAXCHARS];
	int result;

	CComboBox* comboCat = (CComboBox*) GetDlgItem(IDC_LIST_CAT);
	CString sqlCat = "SELECT IDcategoria, nome FROM categorie";
	A = listCat = DaoQuery (dbName, sqlCat, FALSE, FALSE);
	listCat = cdr (listCat);
	while (listCat != NULLP) {
		nome = car (cdr (car (listCat)));
		listCat = cdr (listCat);
		getnam (name, nome);
		result = comboCat->AddString(name);
	}

	CComboBox* comboEnv = (CComboBox*) GetDlgItem(IDC_LIST_ENV);
	CString sqlEnv = "SELECT IDambiente, nome FROM ambienti";
	B = listEnv = DaoQuery (dbName, sqlEnv, FALSE, FALSE);
	listEnv = cdr (listEnv);
	while (listEnv != NULLP) {
		nome = car (cdr (car (listEnv)));
		listEnv = cdr (listEnv);
		getnam (name, nome);
		result = comboEnv->AddString(name);
	}

	m_textIni = "*";
	m_noalias = TRUE;
	m_superlogo = FALSE;
	m_iperlogo = FALSE;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelglossDialog::OnAll() 
{
	// TODO: Add your control notification handler code here
	
	((CComboBox*) GetDlgItem(IDC_LIST_CAT))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_LIST_ENV))->SetCurSel(-1);
	m_textIni = "*";
	m_noalias = TRUE;
	m_superlogo = FALSE;
	m_iperlogo = FALSE;
	UpdateData(FALSE);
}

void ShowGlossary (char* likeName, char* strCat, char* strEnv,
	int noAlias, int soloSuperlogo, int soloIperlogo);

void CSelglossDialog::OnOK() 
{
	// TODO: Add extra validation here

	char strCat [MAXCHARS] = "";
	char strEnv [MAXCHARS] = "";

	int iCat = ((CComboBox*) GetDlgItem(IDC_LIST_CAT))->GetCurSel();
	int iEnv = ((CComboBox*) GetDlgItem(IDC_LIST_ENV))->GetCurSel();

	if (iCat >= 0) {
		node cat = car (_ele (iCat + 2, A));
		getnam (strCat, cat);
	}
	if (iEnv >= 0) {
		node env = car (_ele (iEnv + 2, B));
		getnam (strEnv, env);
	}

	A = listCat = NULLP;
	B = listEnv = NULLP;

	CDialog::OnOK();
	
	ShowGlossary (m_textIni.GetBuffer (1), strCat, strEnv,
					m_noalias, m_superlogo, m_iperlogo);

}

void CSelglossDialog::OnCancel() 
{
	// TODO: Add extra cleanup here

	listCat = NULLP;
	listEnv = NULLP;

	CDialog::OnCancel();
}
