// iperlvw.cpp : implementation of the CIperlogoView class
/* IPERLVW.CPP

000101	Riprovato a rimettere SetLimitText; MAX_OUTPUT a 40000
991229	In WriteOutput2 pulizia del Testo in caso di trabocco
		modificato ancora MAX_OUTPUT: sembra non superi 30000 !
990507	in cputs_ resa condizionale chiamata a SetFocus
990401	in OnExec, focus a console solo se non lo ha browser
990315	diminuito MAX_OUTPUT a 32K-1 a causa di limitazione
990310	aumentato MAX_OUTPUT a 64K-1
990223	gestione dell'attributo FONT
980909	aggiunto salto pagina anche a CIperlogoView::Print (1, ..)
980902	fattorizzato EditPrint in OnPrint
980901	sostituito CListBox con CEdit per output commander
		subclassata nuova finestra di Output
		in OnPrint riattivato StartDoc e EndDoc
980831	eliminato extern "C" per conversione a C++ dei moduli C
980714	subclassata finestra di Input (CEdit)
		per controllo tastiera (ASTASTO, TASTO)
		modifica a meccanismo di READCAR(S)
980713	cputs_ ripristina il Focus corrente
971212	eliminato codice gia' inattivo del pannello di stato
971211	reintrodotto prompt dei comandi (sollecita)
971014	introdotto stampa da programma
9710xx	introdotto stampa della storia dei comandi 
*/                                        

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"		// 980831

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif
 
#include "ilcpp.h"

#define EDIT_OUTPUT
#define BIG_SIZE (4 * K)
//#define MAX_OUTPUT 100000
//#define MAX_OUTPUT 64*K - 1		// 990310
//#define MAX_OUTPUT 32*K - 1		// 990315
//#define MAX_OUTPUT 30000			// 991229
#define MAX_OUTPUT 40000			// 000101

extern CString ini_name;
extern CString commander_name;
extern char CommanderFont[]; 

node KeyProg (UINT nChar);
int ListToFont (node tail, LOGFONT* lpLogFont);
void EditPrint (CEdit* pEdit, char *docName, CDC* pDC, CPrintInfo* pInfo);

extern int is_eco_interactive;

// void PULISCITESTO ();

WNDPROC pOldInputWindowProc;
LRESULT CALLBACK NewInputWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CMainFrame* GetLogoFrame ()
{
	return
	(CMainFrame*)
	((CIperlogoApp*) AfxGetApp())->m_pMainWnd;
}

CIperlogoView* GetLogoView ()
{
	return
	(CIperlogoView*)
	((CMainFrame*)
	((CIperlogoApp*) AfxGetApp())->m_pMainWnd
	)->GetActiveView();
}

#ifdef LISTBOX_OUTPUT

CListBox* txtout;	// reference to output Selection list
int txtout_index;	// index of current (= last) element in txtout

CListBox* GetTxtOut ()
{
	return
	&(GetLogoView ()->m_txtout);
}

#endif // LISTBOX_OUTPUT

#ifdef EDIT_OUTPUT

CEdit* txtout2;		// reference to output Editor
int txtout_index2;	// index of current (= last) element in txtout2

WNDPROC pOldOutput2WindowProc;
LRESULT CALLBACK NewOutput2WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CEdit* GetTxtOut2 ()	// 980901
{
	return
	&(GetLogoView ()->m_txtout2);
}

#endif // EDIT_OUTPUT

#ifdef LISTBOX_OUTPUT

void TxtoutClearText ()
{
	txtout->ResetContent();
}

#endif // LISTBOX_OUTPUT

#ifdef EDIT_OUTPUT

void Txtout2ClearText ()
{
	txtout2->SetReadOnly(FALSE);
	txtout2->SetSel (0, -1);
	txtout2->Clear ();
	txtout2->SetReadOnly(TRUE);
}

#endif // EDIT_OUTPUT

//>980714
void InitCommander () {
	CIperlogoView* pView = GetLogoView ();

	CWnd* pInput= pView->GetDlgItem(IDC_EDIT1);// pointer to Input Control object w.
	HWND hwndInput = pInput->m_hWnd;	// handle of Input control window 
	pOldInputWindowProc = (WNDPROC) SetWindowLong(
		hwndInput,						// handle of window
		GWL_WNDPROC,					// offset of value to set
		(LONG) &NewInputWindowProc		// new value
	);

#ifdef EDIT_OUTPUT

	txtout2 = GetTxtOut2 ();
//	CWnd* pOutput2= pView->GetDlgItem(IDC_EDIT2);// pointer to Input Control object w.
//	HWND hwndOutput2 = pOutput2->m_hWnd;// handle of Input control window 
	HWND hwndOutput2 = txtout2->m_hWnd;// handle of Input control window 
	pOldOutput2WindowProc = (WNDPROC) SetWindowLong(
		hwndOutput2,					// handle of window
		GWL_WNDPROC,					// offset of value to set
		(LONG) &NewOutput2WindowProc	// new value
	);
//> agg.990310 - tolto 990315 (da' fastidio ?) - riprovato 000101
//	UINT limitText;
//	limitText = txtout2->GetLimitText();
//	txtout2->SetLimitText(MAX_OUTPUT);
//	limitText = txtout2->GetLimitText();
//<
#endif
}

//<980714

#ifdef EDIT_OUTPUT

// writes a character string, possibly including \r as line delimiter,
// to the output edit control
void WriteOutput2 (const char* s)
{
	CString s_local = s;
	CString text;
	int i_s;
	char c;

	int nStartChar, nEndChar;
//	txtout2->SetSel(100000, 100000);
	txtout2->SetSel(MAX_OUTPUT, MAX_OUTPUT);
	txtout2->GetSel(nStartChar, nEndChar);

//>991229 spurgo buffer di output limitato
	if ((MAX_OUTPUT - nStartChar) < K) {
		Txtout2ClearText ();
		WriteOutput2 ("-- Pulizia del Testo per trabocco --\r\r");
		txtout2->SetSel(MAX_OUTPUT, MAX_OUTPUT);
		txtout2->GetSel(nStartChar, nEndChar);	// non usato ?
	}
//<991229

	for (i_s = 0; i_s < s_local.GetLength(); ++i_s) {
		c = s_local.GetAt(i_s);
		if (c == '\r') {
			txtout2->ReplaceSel(text);
			text = "";
			txtout2->ReplaceSel("\r\n");
		} else
			text += c;
	}
	txtout2->ReplaceSel(text);
}

#endif

// void cputs_ (const char* s)	// emulates library function of Unix C

#ifdef LISTBOX_OUTPUT

// writes a character string, possibly including \r as line delimiter,
// to the output selection list
void WriteOutput (const char* s)
{
//	CWnd* pFocusWin = theApp.m_pMainWnd->GetFocus();	// 980713

	CString s_local = s;
	CString text;
	int i_s;
	char c;
	txtout = GetTxtOut ();

//	txtout_index = txtout->AddString(s_locale);	// sostituito con ...

	if ((txtout_index = txtout->GetCount() - 1) < 0) {
		text = "";
		txtout_index = txtout->AddString(text);
	} else
		txtout->GetText(txtout_index, text);
	for (i_s = 0; i_s < s_local.GetLength(); ++i_s) {
		c = s_local.GetAt(i_s);
		if (c == '\r') {
			txtout->DeleteString(txtout_index);
			txtout->AddString(text);
			text = "";
			txtout_index = txtout->AddString(text);
		} else
			text += c;
	}
	txtout->DeleteString(txtout_index);
	txtout->AddString(text);

	txtout->SetCurSel(txtout_index);
//	pFocusWin->SetFocus();	// 980713
}

#endif

// writes a character string, possibly including \r as line delimiter,
// to the output component of the commander
void cputs_ (const char* s)	// emulates library function of Unix C
{
	CWnd* pFocusWin = theApp.m_pMainWnd->GetFocus();	// 980713

#ifdef LISTBOX_OUTPUT
	WriteOutput (s);
#endif
#ifdef EDIT_OUTPUT
	WriteOutput2 (s);
#endif

	if (pFocusWin != NULL)		// 990507
		pFocusWin->SetFocus();							// 980713
}

void LogoPrint (int fun, char *pIn, int nIn)
{
	GetLogoView()->Print (fun, pIn, nIn);
}

// funzioni di stampa da programma (SCRIVI_SU, SCRIVI)
void CIperlogoView::Print(int fun, char *pIn, int nIn)
{
	static int horzSize, vertSize;
	static int nChars;
	static int x, y, cx, cy, minHeight = 8, maxHeight;
	static HDC hDC;
	static char *pOut, outBuffer[MAX_CHARS+1];
	
	int result; 
	char c;
	BOOL bPrintSetupOnly = FALSE;
	DWORD dwFlags =
		PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CWnd* pParentWnd =
		(CWnd*) ((CIperlogoApp*) AfxGetApp())->m_pMainWnd;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	CDC printerDC;
	CDC* pDC;

	switch (fun) {
	case 0:
		pDC = printerDC.FromHandle (hDC);	// 980909: anticipato
		maxHeight = max(cy, minHeight);		// 980909: aggiunto

		while (nIn-- != 0) {
			c = *(pIn++);
			if (c != '\r') {
				*(pOut++) = c;
				++nChars;
			}

    		if ((c == '\r') || (nChars == MAX_CHARS)) {
    			*pOut = '\0';
//> 980909
//				if (nChars == 0)
//					y += max(cy, minHeight);
//				else {
//					pDC = printerDC.FromHandle (hDC);
//					result = pDC->TextOut(x, y, outBuffer, nChars);
//					if (! result) MessageBox("TextOut");
//					cy = pDC->GetTextExtent(outBuffer, nChars).cy;
//					y += cy;
//				}
				if (nChars == 0)
					y += maxHeight;
				else {
					if (y+maxHeight > vertSize) {
 						result = pDC->EndPage();
 						result = pDC->StartPage();
						y = 0;
					}
					cy = pDC->GetTextExtent(outBuffer, nChars).cy;
					result = pDC->TextOut(x, y, outBuffer, nChars);
        			if (! result) AfxMessageBox("TextOut");
					y += cy;
				}
//< 980909
				pOut = &outBuffer[0];
				nChars = 0;
				x = 0;
			}
		}
    break;
	case 1:
		result = printDialog.DoModal();
		if (result == IDOK) {
			hDC = printDialog.GetPrinterDC ();
			pDC = printerDC.FromHandle (hDC);

			DOCINFO DocInfo; LPDOCINFO lpDocInfo = &DocInfo;
			DocInfo.lpszDocName = "ProvaStampa";
			DocInfo.lpszOutput = NULL;
			DocInfo.cbSize = sizeof(int) + 2 * sizeof(LPCSTR);
 			result = pDC->StartDoc(lpDocInfo);
			if (! result) MessageBox("StartDoc");

			horzSize = pDC->GetDeviceCaps(HORZRES);	// 980909
			vertSize = pDC->GetDeviceCaps(VERTRES);	//

			result = pDC->StartPage();
			if (! result) MessageBox("StartPage");
			pOut = &outBuffer[0];
			nChars = 0;
			x = 0; y = 0;
			cy = 0;			// 980909
			minHeight = 8;	//
		}
    break;
	case 2:
		pDC = printerDC.FromHandle (hDC);
		if (nChars != 0) {
    		*pOut = '\0';
			result = pDC->TextOut(x, y, outBuffer, nChars);
			if (! result) MessageBox("TextOut");
		}
		result = pDC->EndPage();
 		result = pDC->EndDoc();

		result = DeleteDC (hDC);
    break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoView

IMPLEMENT_DYNCREATE(CIperlogoView, CFormView)

BEGIN_MESSAGE_MAP(CIperlogoView, CFormView)
	//{{AFX_MSG_MAP(CIperlogoView)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_EXEC, OnExec)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIperlogoView construction/destruction

CIperlogoView::CIperlogoView()
	: CFormView(CIperlogoView::IDD)
{
	//{{AFX_DATA_INIT(CIperlogoView)
	m_txtin = "";
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pFont = NULL;
}

CIperlogoView::~CIperlogoView()
{
	m_txtin = "";
}

void CIperlogoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIperlogoView)
	DDX_Control(pDX, IDC_EDIT2, m_txtout2);
	DDX_Control(pDX, IDC_LIST1, m_txtout);
	DDX_Text(pDX, IDC_EDIT1, m_txtin);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoView printing

CPrintDialog* pMainOldPrintDialog;

BOOL CIperlogoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
//	return DoPreparePrinting(pInfo);

	// custom preparation      
	BOOL bPrintSetupOnly = FALSE;
	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CWnd* pParentWnd = (CWnd*)theApp.m_pMainWnd;
	static CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd); // reso globale
	if (theApp.m_mainPrinterData != NULL)
		printDialog.m_pd.hDevMode = theApp.m_mainPrinterData;
	int result = printDialog.DoModal();
	if (result == IDOK) {
		theApp.m_mainPrinterData = printDialog.m_pd.hDevMode;
		pMainOldPrintDialog = pInfo->m_pPD; 
		pInfo->m_pPD = &printDialog;
		pInfo->SetMaxPage(0);
	}
//	else delete(pPrintDialog);
	return (result == IDOK);
}

void CIperlogoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIperlogoView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
 	int result = pDC->EndDoc();
	pInfo->m_pPD = pMainOldPrintDialog;
}

void CIperlogoView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add code to print the controls

#ifndef VECCHIO

	EditPrint (&m_txtout2, "Storia dei comandi", pDC, pInfo);

#else // VECCHIO

		int result;
		DOCINFO DocInfo; LPDOCINFO lpDocInfo = &DocInfo;
		DocInfo.lpszDocName = "Storia dei comandi";
		DocInfo.lpszOutput = NULL;
		DocInfo.cbSize = sizeof(int) + 2 * sizeof(LPCSTR);
		result = pDC->StartDoc(lpDocInfo);	// 980901: riattivato (?)
 		result = pDC->StartPage();
		int lineCount, nIndex, nChars;
		char lineBuffer [BIG_SIZE+1];
		int y = 0, cy, minHeight = 8;

#ifdef LISTBOX_OUTPUT
		lineCount = m_txtout.GetCount();
#endif
#ifdef EDIT_OUTPUT
		lineCount = m_txtout2.GetLineCount();
#endif

		for (nIndex=0; nIndex<lineCount; nIndex++) {
#ifdef LISTBOX_OUTPUT
			nChars = m_txtout.GetTextLen(nIndex);
			nChars = m_txtout.GetText(nIndex, lineBuffer);
#endif
#ifdef EDIT_OUTPUT
			nChars = m_txtout2.GetLine(nIndex, lineBuffer, BIG_SIZE);
#endif
			if (nChars == 0)
				y += max(cy, minHeight);
			else {
				lineBuffer[nChars] = '\0';
				result = pDC->TextOut(0, y, lineBuffer, nChars);
        		if (! result) MessageBox("NON ha risposto");
				cy = pDC->GetTextExtent(lineBuffer, nChars).cy;
				y += cy;
			}
		}

		result = pDC->EndPage();
 		result = pDC->EndDoc();		// 980901: riattivato (?)

#endif VECCHIO
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoView diagnostics

#ifdef _DEBUG
void CIperlogoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIperlogoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CIperlogoDoc* CIperlogoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIperlogoDoc)));
	return (CIperlogoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIperlogoView message handlers

int GetFocusDev ();
void CIperlogoView::OnExec()
{
	// TODO: Add your control notification handler code here
	char linedel[] = "\r";

	UpdateData(TRUE);
	// m_txtout.AddString (m_txtin);
	if (is_maiuscole) m_txtin.MakeUpper();
	if (is_eco_interactive) {		// aggiunto test 971221
		cputs_ ((const char *) m_txtin);
		cputs_ (linedel);
	}
	execute ((const char *) m_txtin);		//// execution in module ILMAIN.C 
	if (is_ciao) {
//		WinAppExit ();
	}
	else {
		int devFocus = GetFocusDev ();

		m_txtin= "";
		UpdateData(FALSE);	
		sollecita ();	// 971211

		if ((devFocus == -1) || (! (_fstato [devFocus] & O_BROWSER)))
			GetDlgItem(IDC_EDIT1)->SetFocus();
	}
}

void CIperlogoView::OnSelchangeList1()
{   
	int index = m_txtout.GetCurSel();
	m_txtout.GetText(index, m_txtin);
	UpdateData(FALSE);	
}

void CIperlogoView::SelChange(const char *s)
{   
	m_txtin = s;
	UpdateData(FALSE);	
}

void CIperlogoView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	GetDlgItem(IDC_EDIT1)->SetFocus();
}

void ConInputFocus ()
{
	GetLogoView()->GetDlgItem(IDC_EDIT1)->SetFocus();
}

void PULISCITESTO ()
{
#ifdef LISTBOX_OUTPUT
	TxtoutClearText ();
#endif

#ifdef EDIT_OUTPUT
	Txtout2ClearText ();
#endif
}

LOGFONT* CIperlogoView::RetrieveFont()	// 990223
{
	return m_pLogFont;
}

void CIperlogoView::AssignFont(LOGFONT* pLogFont)	// 990223
{
	m_pLogFont = pLogFont;
	m_logFont = *m_pLogFont;
}

// m_logFont e' gia' stato assegnato !
void CIperlogoView::ActivateFont()	// 980401
{
	if (m_pFont != NULL) delete(m_pFont);
	m_pFont = new(CFont);
	m_pFont->CreateFontIndirect(&m_logFont);
	m_pLogFont = &m_logFont;

	GetDlgItem(IDC_EDIT1)->SetFont(m_pFont);

#ifdef LISTBOX_OUTPUT
	GetDlgItem(IDC_LIST1)->SetFont(m_pFont);
#endif
#ifdef EDIT_OUTPUT
	GetDlgItem(IDC_EDIT2)->SetFont(m_pFont);	// 980901
#endif
}

void CIperlogoView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();	// Ma ce l'aveva messo Class Wizard ??
	
	// TODO: Add extra initialization here

	m_pFont = NULL;
	// Default font ..
	::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	// .. possibly to be replaced from custom font
	int n =
		GetPrivateProfileString(commander_name, "Font", "",
					CommanderFont, MAX_CHARS, ini_name);
	if (n > 0) {
		node FontParList = _parse (CommanderFont);
		ListToFont (FontParList, &m_logFont);
	}
	// Create CFont object make it current for child controls
	ActivateFont();
}

void CIperlogoView::OnOptionFont()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = this;
	if (m_pFont != NULL)
		m_pFont->GetObject(sizeof(LOGFONT), &m_logFont);
	else
		::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	
	CFontDialog FontDialog(&m_logFont, /*CF_EFFECTS|*/ CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT, NULL, pParentWnd);
	if (FontDialog.DoModal() == IDOK)
		ActivateFont();
}

LRESULT CALLBACK NewInputWindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	extern unsigned char* pConReadChars;
	extern int nConReadChars;
	extern int iConReadChars;

	node KeyVal;
	char KeyStr [MAX_CHARS+1];
	BOOL bCanUndo;
	UINT nChar = wParam;
	CWnd* pInput = NULL;

	switch (uMsg){
	case WM_CHAR:
		if (iConReadChars < nConReadChars) {
			*(pConReadChars++) = nChar;
			++iConReadChars;
			return 0;
		}
		KeyVal = KeyProg (nChar);
		if (KeyVal != S_VUOTA)
		{
			if (typ (KeyVal) == LIST)
				NestedExec (KeyVal);
			else {
				bCanUndo = FALSE;
				pacstr (KeyStr, nome (KeyVal));
				((CEdit *) pInput->FromHandle(hwnd))->ReplaceSel(KeyStr, bCanUndo);
			}
			return 0;
		}
		break;
	default:
		break;
	}
	return CallWindowProc(pOldInputWindowProc,
				hwnd, uMsg, wParam, lParam);
}

#ifdef EDIT_OUTPUT

LRESULT CALLBACK NewOutput2WindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{

	LRESULT lResult = 
	CallWindowProc(pOldOutput2WindowProc, hwnd, uMsg, wParam, lParam);

	int nStartChar, nEndChar;
	int nIndex, nCount;

	switch (uMsg){
	case WM_LBUTTONDBLCLK:
		char text [BIG_SIZE+1];
//		txtout2->GetSel (nStartChar, nEndChar);
		nStartChar = txtout2->LineIndex(-1);
		nIndex = txtout2->LineFromChar (nStartChar);		
		nCount = txtout2->GetLine(nIndex, text, BIG_SIZE);
		text [nCount] = '\0';
		if (nCount > 0) {
			nEndChar = nStartChar + nCount;
			txtout2->SetSel(nStartChar, nEndChar);
			GetLogoView()->SelChange (text);
		}
		break;
	default:
		break;
	}

	return lResult;
}

#endif
