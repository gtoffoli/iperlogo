// Register.cpp : implementation file
//
/* REGISTER.CPP		G. Toffoli

040306	corretto registrazione di libreria in OnRegister; ritoccato OnInitDialog
040305	corretto LibRegisterLevel (c'era un ; di troppo)
040205	introdotto uso dei livelli anche nella registrazione di applicazioni
010604	ritoccato OnInitDialog
010601	esteso da 4 a 16 le posizioni riservare alibrerie in file di protezione
010201	IDC_EDIT_SERIAL ReadOnly invece che disabled (come era in definizione risorsa) 
010111	DIM_FIRMA: 16 -> 24
001218	spostato scrittura firma libreria in ilnodi.cpp
001211	libreria: funzioni di verifica e dialogo semplificato
981209	predisposto per modifica livello di registrazione
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "Register.h"

// #include "Cprot30.h"

#include <io.h>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CIperlogoApp theApp;
BOOL isChangeLevel;
extern int is_lib;
extern int isIperlogoRegistered, isLibraryRegistered;	// 010604
extern CString exeName;
// *******************************
//	Nuova struttura Firma:
//	12 byte : codice di registrazione
//	 4 byte : liberi
//	 4 byte : numero libreria
//	 4 byte : data prima esecuzione
// *******************************
// #define DIM_FIRMA 16
#define DIM_REGCODE 12
#define OFFSET_LIBSERIAL (DIM_REGCODE + 4 )
#define OFFSET_DATA (OFFSET_LIBSERIAL + 4)
#define DIM_FIRMA (OFFSET_DATA + 4)
#define MAX_REGLEVEL 3 // 040205
extern char firma_lib[];
BOOL ScriviFirma();
BOOL ScriviDataInFirma();
//int FreeIntParam (int val);
int FreeLibParam (int val);

// calcola codice di registrazione a partire da numero di identificazione
// (regSerial) e nome della libreria, e lo riporta per indirizzo
void ComputeFirmaLib (char* dest, CString regSerial, CString exeName, int librarySerial, int regLevel)
{
	#define baseDec		10
	#define baseAlfa	('Z' - 'A' + 1)
	memset(dest, 0, DIM_REGCODE+1);
	CString s1 = regSerial; s1.MakeUpper();
	s1.Remove('-');
	CString s2 = exeName + exeName; s2.MakeUpper();
	int level = 3 - regLevel; // 040205: aggiunto (per regLevel = 3, level non influisce) 
	int c;
	int c1, c2;
	int i, j = 0, l = s2.GetLength();
	for (i = 0; i < DIM_REGCODE; i++) {
		c1 = s1.GetAt(i);
		librarySerial = - librarySerial;
		level = - level; // 040205: agiunto
		if (j < l)
			c2 = s2.GetAt(j);
		else
			c2 = 0;
		if ((c1 >= 'A') && (c1 <= 'Z')) {
			c1 = c1 - 'A';
			c = c1 + c2 + librarySerial + level;
			if (c < 0)
				c = c + baseAlfa;
			c = c % baseAlfa;
			c = c + 'A';
		}
		else {
			c1 = c1 - '0';
			c = c1 + c2 + librarySerial + level;
			if (c < 0)
				c = c + baseDec;
			c = c % baseDec;
			c = c + '0';
		}
		j++;
		dest[i] = c;
	}
}

// registra file di libreria associato all'applicazione: calcola il codice
// di registrazione e lo confronta con quello fornito dall'utente;
// se coincidono, riporta TRUE
BOOL LibRegister (const char* reg_code, int regLevel)
{
/*
	ComputeFirmaLib (firma_lib, theApp.m_regSerial, exeName, * ((int *) &firma_lib[OFFSET_LIBSERIAL]), regLevel);
	if (strcmp(reg_code, firma_lib) == 0) {
		BOOL firmaOk = ScriviFirma();
		if (firmaOk) {
			// libera annotazione in file di protezione
			int librarySerial = * ((int *) &firma_lib[OFFSET_LIBSERIAL]);
			int freePos = FreeLibParam (librarySerial);
			ScriviDataInFirma();
			isLibraryRegistered = TRUE;
		}
		return firmaOk;
	}
	else
		return FALSE;
*/
	return TRUE;
}

// verifica registrazione di file di libreria associato ad applicazione 
int LibRegisterLevel ()
{
/*
	char reg_code[DIM_FIRMA];
	int regLevel = MAX_REGLEVEL;
	while (regLevel > 0) {
		ComputeFirmaLib (reg_code, theApp.m_regSerial, exeName, * ((int *) &firma_lib[OFFSET_LIBSERIAL]), regLevel);
		if (strcmp(reg_code, firma_lib) == 0)
			return regLevel;
		--regLevel;
	}
*/
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog


CRegisterDialog::CRegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDialog)
	m_registration = _T("");
	m_company = _T("");
	m_name = _T("");
	m_serial = _T("");
	m_status = _T("");
	m_access = 0;
	//}}AFX_DATA_INIT
	m_name = theApp.m_regName;
	m_company = theApp.m_regCompany;
	m_serial = theApp.m_regSerial;
	m_registration = theApp.m_regCode;
	m_access = theApp.m_regAccess;
}

BOOL CRegisterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if ((is_lib && isLibraryRegistered) || ((! is_lib) && isIperlogoRegistered)) {
		m_status = "Istallazione gia' registrata";
		GetRegistrationNames ();
		((CEdit*) GetDlgItem(IDC_EDIT_NAME))->SetReadOnly(TRUE);
		((CEdit*) GetDlgItem(IDC_EDIT_COMPANY))->SetReadOnly(TRUE);
		((CEdit*) GetDlgItem(IDC_EDIT_CODE))->SetReadOnly(TRUE);
		GetDlgItem(IDREGISTER)->SetWindowText("Cambia livello");
		if (theApp.m_regAccess >= MAX_REGLEVEL)
			GetDlgItem(IDREGISTER)->EnableWindow(FALSE);
	}
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GotoDlgCtrl(GetDlgItem(IDCANCEL));
	isChangeLevel = FALSE;
	if (is_lib) {
		CString windowText = "Registrazione di ";
		windowText += exeName;
		SetWindowText(windowText);
	}
	//	ReadOnly invece che disabled (come era in definizione risorsa)
	((CEdit*) GetDlgItem(IDC_EDIT_SERIAL))->SetReadOnly(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDialog)
	DDX_Text(pDX, IDC_EDIT_CODE, m_registration);
	DDX_Text(pDX, IDC_EDIT_COMPANY, m_company);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_serial);
	DDX_Text(pDX, IDC_TEXT_STATUS, m_status);
	DDX_Text(pDX, IDC_EDIT_ACCESS, m_access);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDialog, CDialog)
	//{{AFX_MSG_MAP(CRegisterDialog)
	ON_BN_CLICKED(IDREGISTER, OnRegister)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog message handlers

#define someerror 1


void CRegisterDialog::GetRegistrationNames() 
{
	int result;
	char name[64] = "name"; // "";
	char company[64] = "company"; // "",
	result = 0; // cpGetRegisteredName(theApp.m_regPath, name);
	m_name = name;
	result = 0; // cpGetRegisteredCompany(theApp.m_regPath, company);
	m_company = company;
	UpdateData(FALSE);
}

void CRegisterDialog::OnRegister() 
{
	// TODO: Add your control notification handler code here

int result;
BOOL ok = FALSE;

UpdateData(TRUE);

if ((m_access > 0) && (! isChangeLevel)) {
	isChangeLevel = TRUE;
	((CEdit*) GetDlgItem(IDC_EDIT_CODE))->SetReadOnly(FALSE);
	GetDlgItem(IDREGISTER)->SetWindowText("Registra");
}
else
{

	if (m_name.IsEmpty() || m_company.IsEmpty() || m_registration.IsEmpty())
	{
		m_status = "Dati incompleti";
		UpdateData(FALSE);
		return;
	}

	if (is_lib) {
		m_registration.Remove('-'); m_registration.Remove('_');
		m_registration.Remove('.'); m_registration.Remove('/');
		m_access = MAX_REGLEVEL;
		while (m_access > theApp.m_regAccess) {
			if (result = LibRegister ((LPCTSTR) m_registration, m_access)) {
				theApp.m_regAccess = m_access;
				break;
			}
			--m_access;
		}
		if (result)
		{
			ok = TRUE;
			m_status = "Registrazione OK";
		}
		else
		{
			m_status = "Codice di registrazione non valido";
		}
	}
	else {
		result = 1; // cpRegisterProgram(theApp.m_regPath, m_registration);

		switch (result) {
		/*
		case FILE_NOT_FOUND:	// if path is not valid.
			m_status = "Errore di sistema";
			break;
		case INI_CONTROL_FAILED:// if the number in the '.ini' file is different from that stored in the security file.
		case INI_CONTROL_MISSING:// if the '.ini' file or the entry is not found
			m_status = "Istallazione corrotta";
			break;
		case WRONG_CODE:		// if  regcode is incompatible with the serial number stored in the security file.
			m_status = "Codice di registrazione non valido";
			break;
		*/
		case 1: /*IS_REGISTERED:*/ 	// if the supplied regcode is valid.
			result = 1; // cpIsRegistered (theApp.m_regPath);
			switch (result) {
			case 1: // IS_REGISTERED:
				int f_code = 1; //cpCheckRegCode(theApp.m_regPath, "\0");
				if((f_code < 1) || (f_code > 15))
				{
						//	return someerror;
				}
				ok = TRUE;
				/* result = cpSetRegisteredNames (theApp.m_regPath, m_name, m_company); */
				GetRegistrationNames ();
				theApp.m_regName = m_name;
				theApp.m_regCompany = m_company;
				theApp.m_regCode = m_registration;
				theApp.m_regAccess = m_access = f_code;
				m_status = "Registrazione OK";
				break;
			}
			break;
		}
	}
}
	if (ok)
	{
		((CEdit*) GetDlgItem(IDC_EDIT_NAME))->SetReadOnly(TRUE);
		((CEdit*) GetDlgItem(IDC_EDIT_COMPANY))->SetReadOnly(TRUE);
		((CEdit*) GetDlgItem(IDC_EDIT_CODE))->SetReadOnly(TRUE);

		GetDlgItem(IDREGISTER)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GotoDlgCtrl(GetDlgItem(IDOK));
	}
	UpdateData(FALSE);
}
