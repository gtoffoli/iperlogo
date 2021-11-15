// ilregcodDlg.cpp : implementation file
//
// 991223 GT reso facoltativi i trattini tra i 3 gruppi di 4 caratteri

#include "stdafx.h"
#include "ilregcod.h"
#include "ilregcodDlg.h"

// #include "\il32\inc\Cprot30.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIlregcodDlg dialog

CIlregcodDlg::CIlregcodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIlregcodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIlregcodDlg)
	m_access = 0;
	m_serial = _T("");
	m_code = _T("");
	m_status = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_serial = _T("____-____-____");
	m_access = 1;
}

void CIlregcodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIlregcodDlg)
	DDX_Text(pDX, IDC_EDIT_ACCESS, m_access);
	DDV_MinMaxInt(pDX, m_access, 1, 15);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_serial);
	DDV_MaxChars(pDX, m_serial, 14);
	DDX_Text(pDX, IDC_EDIT_CODE, m_code);
	DDX_Text(pDX, IDC_TEXT_STATUS, m_status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIlregcodDlg, CDialog)
	//{{AFX_MSG_MAP(CIlregcodDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXEC, OnExec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIlregcodDlg message handlers

BOOL CIlregcodDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIlregcodDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIlregcodDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIlregcodDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CIlregcodDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL isLetter (char c) {
	c = toupper(c);
	return ((c >= 'A') && (c <= 'Z'));
}

BOOL isDigit (char c) {
	return ((c >= '0') && (c <= '9'));
}

#ifdef OLD_REGCOD

void CIlregcodDlg::OnExec() 
{
	// TODO: Add your control notification handler code here

	int result;
	char regcode[20];                

	UpdateData(TRUE);

	if (	(m_serial.GetLength() == 14)
		&&	(m_serial.GetAt(4) == '-')
		&&	(m_serial.GetAt(9) == '-')

		&&	(isLetter (m_serial.GetAt(0)))
		&&	(isLetter (m_serial.GetAt(2)))
		&&	(isLetter (m_serial.GetAt(6)))

		&&	(isDigit (m_serial.GetAt(1)))
		&&	(isDigit (m_serial.GetAt(3)))
		&&	(isDigit (m_serial.GetAt(5)))
		&&	(isDigit (m_serial.GetAt(7)))
		&&	(isDigit (m_serial.GetAt(8)))
		&&	(isDigit (m_serial.GetAt(10)))
		&&	(isDigit (m_serial.GetAt(11)))
		&&	(isDigit (m_serial.GetAt(12)))
		&&	(isDigit (m_serial.GetAt(13)))
		)
		m_status = "OK";
	else m_status = "Numero di Serie non corretto";

	result = cpMakeRegCode(m_access, (const char*) m_serial, regcode);
	if (result == 0)
		m_code = regcode;
	else m_code = "";
	UpdateData(FALSE);	
	
}

#else // OLD_REGCOD

void CIlregcodDlg::OnExec() 
{
	// TODO: Add your control notification handler code here

	int result;
	// char regcode[20];
	char regcode[] = "";
	int m = 0, n = 0;

	UpdateData(TRUE);

	if ((  (   (m_serial.GetLength() == 14)
			&& (m_serial.GetAt(4) == '-')
			&& (m_serial.GetAt(9) == '-')
			&& (m = 1) && (n = 2)				// side effects
			)
		 ||
		  (m_serial.GetLength() == 12)
		)
		&&	(isLetter (m_serial.GetAt(0)))
		&&	(isLetter (m_serial.GetAt(2)))

		&&	(isLetter (m_serial.GetAt(5+m)))

		&&	(isDigit (m_serial.GetAt(1)))
		&&	(isDigit (m_serial.GetAt(3)))

		&&	(isDigit (m_serial.GetAt(4+m)))
		&&	(isDigit (m_serial.GetAt(6+m)))
		&&	(isDigit (m_serial.GetAt(7+m)))

		&&	(isDigit (m_serial.GetAt(8+n)))
		&&	(isDigit (m_serial.GetAt(9+n)))
		&&	(isDigit (m_serial.GetAt(10+n)))
		&&	(isDigit (m_serial.GetAt(11+n)))
		)
		m_status = "OK";
	else m_status = "Numero di Serie non corretto";

	result = 0; // cpMakeRegCode(m_access, (const char*)m_serial, regcode);
	if (result == 0)
		m_code = regcode;
	else m_code = "";
	UpdateData(FALSE);	
	
}

#endif // OLD_REGCOD
