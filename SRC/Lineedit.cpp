// lineedit.cpp : implementation file
//

//	980831	eliminato extern "C" per conversione a C++ dei moduli C

#include "stdafx.h"
#include "iperlogo.h"
#include "lineedit.h"
#include "ilcpp.h"

// extern "C"
extern
node risultato;

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

//// public C function for asynchronous input from the 'CONSOLE' device

// extern "C"
int cgets_ (char* s, int m)	// emulates library function of Unix C
{
	CLineEdit lineEdit;
	lineEdit.pstring = s;
	lineEdit.maxchars = m;
	*s = '\0';
	lineEdit.DoModal();
	return strlen(s);
}


#ifdef DA_CONSERVARE

// Questa versione di READCHAR e READCHARS e' potenzialmente piu' potente,
// dato che potrebbe gestire i caratteri estesi, ma non so come fare la
// traduzione dai Virtual Codes ai codici ASCII (eccetto che per i casi semplici
// di lettere e numeri), a partire dall' informazione disponibile.

// La versione in uso, basata su una sottoclasse di CDialog contenente un CEdit
// (entrambi non visibili perche' piccolissimi), e' nel modulo kbdialog.cpp/h
     
unsigned int *pCharBuffer;
int nExpectedChars;
                           
// extern "C"
BOOL CALLBACK MyDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) 
{     
  static int  nReadChars;
  static BOOL shift = FALSE;
  unsigned int cCode;
  switch (msg) {
                   
    case WM_INITDIALOG :               
      nReadChars = 0;
      return -1;        
    break;  
    
    case WM_KEYDOWN :  
      if (wParam == VK_SHIFT) shift = TRUE;
      else {
      	cCode = wParam;
      	if (! shift) {
      		if ((cCode >= 'A') && (cCode <= 'Z'))
      			cCode |= ('a' - 'A');
      	}
      	pCharBuffer[nReadChars] = cCode;
      	if (++nReadChars >= nExpectedChars) EndDialog(hwndDlg, 0);
      }
      return -1;
    break;
    
    case WM_KEYUP :  
      if (wParam == VK_SHIFT) shift = FALSE;
      return -1;
    break;
    
    case WM_CHAR :
      msg = msg;    
      return -1;
    break;
    
    default :   
      return 0;
    
  }  
}
  
void ReadKeyboard (unsigned int *pBuffer, int nChars)
{    
	pCharBuffer = pBuffer;
	nExpectedChars = nChars; 
	
	FARPROC ProcInst = MakeProcInstance((FARPROC) MyDlgProc, AfxGetInstanceHandle());
	int result =
		DialogBox(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDD_INPUT), theApp.m_pMainWnd->m_hWnd, (DLGPROC) ProcInst);
	FreeProcInstance(ProcInst);
}
  
// extern "C"
int cgetc_ ()	// emulates library function of Unix C
{    
	unsigned int charBuffer[MAX_CHARS];

	ReadKeyboard (&charBuffer[0], 1);
	return charBuffer[0];
}

// extern "C"
void READCHARS ()
{
	unsigned int charBuffer[MAX_CHARS];
	unsigned char s[MAX_CHARS];
	int nChars = rnum_int (args[0]);
	int i;
	ReadKeyboard (&charBuffer[0], nChars);
	for (i=0; i<nChars; i++)
		s[i] = (unsigned char) charBuffer[i];
	s[nChars] = '\0';
	risultato = logstr ((char *) &s[0]);
}

#endif // CONSERVARE


void CLineEdit::TransferText ()
{
	int length = min(maxchars-1, m_text.GetLength());
	if (length > 0) { 
		memcpy (pstring, (const char*) m_text, length);
		pstring[length] = '\0';
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLineEdit dialog


CLineEdit::CLineEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CLineEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineEdit)
	m_text = "";
	//}}AFX_DATA_INIT
}

void CLineEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineEdit)
	DDX_Text(pDX, IDC_LINE_EDIT, m_text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLineEdit, CDialog)
	//{{AFX_MSG_MAP(CLineEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLineEdit message handlers

BOOL CLineEdit::OnInitDialog()	//// ovveride default
{
	CDialog::OnInitDialog();
	GotoDlgCtrl(GetDlgItem (IDC_LINE_EDIT));
	return FALSE;	
}

void CLineEdit::OnOK()
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);
	TransferText();
    m_text = "";
	UpdateData(FALSE);	
	EndDialog(0);
	
	CDialog::OnOK();
}

#ifdef DOC

#endif // DOC
