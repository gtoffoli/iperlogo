/*	kbdialog.cpp : implementation file

	980831	eliminato extern "C" per conversione a C++ dei moduli C
	980714	modifica a meccanismo di READCAR(S)
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "kbdialog.h"

#include "ilcpp.h"

// extern "C"
void EventCheck ();

// extern "C"
extern
node risultato;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern void ConInputFocus ();

unsigned char* pConReadChars;
int nConReadChars = 0;
int iConReadChars = 0;

void ReadKeyboard (unsigned char *pBuffer, int nChars)
{
//>980714
//	CKbDialog dialog(NULL);
//	dialog.pBuffer = pBuffer; 
//	dialog.nChars = nChars; 
//	int result = dialog.DoModal();
	pConReadChars = pBuffer;
	nConReadChars = nChars;
	iConReadChars = 0;
	while (iConReadChars < nConReadChars) {
		EventCheck ();
		ConInputFocus ();
	}
//<980714
}
  
// extern "C"
int cgetc_ ()	// emulates library function of Unix C
{    
	unsigned char s[MAX_CHARS];

	ReadKeyboard (&s[0], 1);
	return s[0];
}

// extern "C"
void READCHARS ()
{
	unsigned char s[MAX_CHARS];
	int nChars = rnum_int (args[0]);
	ReadKeyboard (&s[0], nChars);
	s[nChars] = '\0';
	risultato = logstr ((char *) &s[0]);
}

int CKbDialog::TransferText ()
{
	int length = m_text.GetLength();
	if (length >= nChars) { 
		memcpy (pBuffer, (const char*) m_text, length);
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CKbDialog dialog


CKbDialog::CKbDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CKbDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKbDialog)
	m_text = "";
	//}}AFX_DATA_INIT
}

void CKbDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKbDialog)
	DDX_Text(pDX, IDC_EDIT1, m_text);
	DDV_MaxChars(pDX, m_text, 256);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKbDialog, CDialog)
	//{{AFX_MSG_MAP(CKbDialog)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_UPDATE(IDC_EDIT1, OnUpdateEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKbDialog message handlers

void CKbDialog::OnChangeEdit1()
{
	// TODO: Add your control notification handler code here
	
}

void CKbDialog::OnUpdateEdit1()
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if (TransferText()) EndDialog(0);
}
