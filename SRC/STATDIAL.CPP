// statdial.cpp : implementation file
//

#include "stdafx.h"
#include "iperlogo.h"
#include "statdial.h"
#include "turtlefr.h"
#include "turtlevw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusDialog dialog


CStatusDialog::CStatusDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusDialog)
	m_fontName = "";
	m_fontWeight = 0;
	m_fontSize = 0;
	m_penContact = "";
	m_turtleVisibility = "";
	m_colorFloodB = 0;
	m_colorFloodG = 0;
	m_colorFloodR = 0;
	m_colorScreenB = 0;
	m_colorScreenG = 0;
	m_colorScreenR = 0;
	m_colorPenB = 0;
	m_colorPenG = 0;
	m_colorPenR = 0;
	m_penStyle = 0;
	m_penWidth = 0;
	m_turtleHeading = 0;
	m_turtlePosX = 0;
	m_turtlePosY = 0;
	m_winDimX = 0;
	m_winDimY = 0;
	m_viewTop = 0;
	m_viewBottom = 0;
	m_viewLeft = 0;
	m_viewRight = 0;
	m_viewDimX = 0;
	m_viewDimY = 0;
	m_winPosX = 0;
	m_winPosY = 0;
	m_name = "";
	//}}AFX_DATA_INIT
}

void CStatusDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusDialog)
	DDX_Text(pDX, IDC_FONT_NAME2, m_fontName);
	DDX_Text(pDX, IDC_FONT_WEIGHT2, m_fontWeight);
	DDX_Text(pDX, IDC_FONT_SIZE2, m_fontSize);
	DDX_Text(pDX, IDC_PEN_CONTACT2, m_penContact);
	DDX_Text(pDX, IDC_TURTLE_VISIBILITY2, m_turtleVisibility);
	DDX_Text(pDX, IDC_COLOR_FLOODB, m_colorFloodB);
	DDX_Text(pDX, IDC_COLOR_FLOODG, m_colorFloodG);
	DDX_Text(pDX, IDC_COLOR_FLOODR, m_colorFloodR);
	DDX_Text(pDX, IDC_COLOR_SCREENB, m_colorScreenB);
	DDX_Text(pDX, IDC_COLOR_SCREENG, m_colorScreenG);
	DDX_Text(pDX, IDC_COLOR_SCREENR, m_colorScreenR);
	DDX_Text(pDX, IDC_COLOR_PENB, m_colorPenB);
	DDX_Text(pDX, IDC_COLOR_PENG, m_colorPenG);
	DDX_Text(pDX, IDC_COLOR_PENR, m_colorPenR);
	DDX_Text(pDX, IDC_PEN_STYLE2, m_penStyle);
	DDX_Text(pDX, IDC_PEN_WIDTH2, m_penWidth);
	DDX_Text(pDX, IDC_TURTLE_HEADING2, m_turtleHeading);
	DDX_Text(pDX, IDC_TURTLE_POSX, m_turtlePosX);
	DDX_Text(pDX, IDC_TURTLE_POSY, m_turtlePosY);
	DDX_Text(pDX, IDC_WIN_DIMX, m_winDimX);
	DDX_Text(pDX, IDC_WIN_DIMY, m_winDimY);
	DDX_Text(pDX, IDC_VIEW_YHIGH2, m_viewTop);
	DDX_Text(pDX, IDC_VIEW_YLOW2, m_viewBottom);
	DDX_Text(pDX, IDC_VIEW_XLOW2, m_viewLeft);
	DDX_Text(pDX, IDC_VIEW_XHIGH2, m_viewRight);
	DDX_Text(pDX, IDC_VIEW_PIXELX, m_viewDimX);
	DDX_Text(pDX, IDC_VIEW_PIXELY, m_viewDimY);
	DDX_Text(pDX, IDC_WIN_POSX, m_winPosX);
	DDX_Text(pDX, IDC_WIN_POSY, m_winPosY);
	DDX_Text(pDX, IDC_TURTLE_NAME2, m_name);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStatusDialog, CDialog)
	//{{AFX_MSG_MAP(CStatusDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatusDialog message handlers

void CStatusDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
	
	((CTurtleView*)((CTurtleFrame*) GetParent())->GetActiveView ())->OnTurtlestatusHide();	// custom
}
