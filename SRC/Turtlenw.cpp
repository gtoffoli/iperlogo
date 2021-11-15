// turtlenw.cpp : implementation file
//
/*	turtlenw.cpp

000419	modificato turtleMode
991227	in OnOK corretto chiamata ad apri_tarta
980831	eliminato extern "C" per conversione a C++ dei moduli C
*/

#include "stdafx.h"

#include "logo.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "turtlenw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurtleNew dialog        CString

extern char DefaultTurtle[];
extern int newTurtleCounter;
extern int turtleMode;					// modalita' finestra TARTA da aprire
extern int defaultTurtleMode;

int apri_tarta (unsigned long lnome, const char *name, int x1, int y1, int dimx, int dimy,
						int pixel_x, int pixel_y, double left, double right, double bottom, double top);

CTurtleNew::CTurtleNew(CWnd* pParent /*=NULL*/)
	: CDialog(CTurtleNew::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTurtleNew)
	m_winPosX = 0;
	m_winPosY = 0;
	m_winDimX = 0;
	m_winDimY = 0;
	m_winName = "";
	m_viewDimX = 0;
	m_viewDimY = 0;
	m_viewRight = 0;
	m_viewLeft = 0;
	m_viewTop = 0;
	m_viewBottom = 0;
	m_turtleCounter = newTurtleCounter;
	m_modeOVs = FALSE;
	//}}AFX_DATA_INIT

	Reset();
}

void CTurtleNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurtleNew)
	DDX_Text(pDX, IDC_WIN_POSX, m_winPosX);
	DDX_Text(pDX, IDC_WIN_POSY, m_winPosY);
	DDX_Text(pDX, IDC_WIN_DIMX, m_winDimX);
	DDX_Text(pDX, IDC_WIN_DIMY, m_winDimY);
	DDX_Text(pDX, IDC_VIEW_NAME2, m_winName);
	DDV_MaxChars(pDX, m_winName, 32);
	DDX_Text(pDX, IDC_VIEW_PIXELX, m_viewDimX);
	DDX_Text(pDX, IDC_VIEW_PIXELY, m_viewDimY);
	DDX_Text(pDX, IDC_VIEW_XHIGH2, m_viewRight);
	DDX_Text(pDX, IDC_VIEW_XLOW2, m_viewLeft);
	DDX_Text(pDX, IDC_VIEW_YHIGH2, m_viewTop);
	DDX_Text(pDX, IDC_VIEW_YLOW2, m_viewBottom);
	DDX_Check(pDX, IDC_VIEW_MODE2, m_modeOVs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTurtleNew, CDialog)
	//{{AFX_MSG_MAP(CTurtleNew)
	ON_BN_CLICKED(IDRESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTurtleNew message handlers

void CTurtleNew::OnReset()
{
	// TODO: Add your control notification handler code here
	
	Reset();
	
	UpdateData(FALSE);	
}

void CTurtleNew::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CTurtleNew::Reset()
{
	// TODO: Add your control notification handler code here
	
extern CREATESTRUCT Turtle;
char sCounter[32] = "";
char turtleName[32];

	if (m_turtleCounter > 1)
		sprintf (sCounter, "%d", m_turtleCounter);
	strcpy (turtleName, DefaultTurtle);
	strcat (turtleName, sCounter);
	m_winName = turtleName;
	m_winPosX = Turtle.x;
	m_winPosY = Turtle.y;
	m_winDimX = Turtle.cx;
	m_winDimY = Turtle.cy;
	m_viewDimX = 800;
	m_viewDimY = 800;
	m_viewLeft = -400.0;
	m_viewRight = 400.0;
	m_viewTop = 400.0;
	m_viewBottom = -400.0;
	m_modeOVs = FALSE;
}

void CTurtleNew::OnOK()
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
//> 000419
//	turtleMode = (m_modeOVs) ? 1 : 0;
	int saveDefaultTurtleMode = defaultTurtleMode;
	defaultTurtleMode = (m_modeOVs) ? 1 : 0;
//<
	apri_tarta (NULLP, (const char*) m_winName, m_winPosX, m_winPosY, m_winDimX, m_winDimY,
				m_viewDimX, m_viewDimY, m_viewLeft, m_viewRight, m_viewBottom, m_viewTop);
//> 000419
	defaultTurtleMode = saveDefaultTurtleMode;
//<
}
