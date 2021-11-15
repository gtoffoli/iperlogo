// turtleVp.cpp : implementation of the CTurtleViewport class
//

#include "stdafx.h"
//#include "test.h"
#include "iperlogo.h"
#include "turtleVp.h"
#include "turtleCtlr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CIperlogoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTurtleViewport

CTurtleViewport::CTurtleViewport()
{
	m_pCtlr = NULL;	// 991018 (vedi uso in turtlevw.cpp
}

CTurtleViewport::~CTurtleViewport()
{
}

extern int globalPixelX, globalPixelY;

// imitato da sample OfficeLayoutVp.cpp
void CTurtleViewport::OnInitialUpdate()
{
//	SetMapMode(MM_ANISOTROPIC);
	SetMapMode(MM_TEXT);

//	CSize sizeDiagram = GetModel()->GetDiagramSize();

	// Origin is initially 0,0.
///	SetOrigin(0,0);

//	SetExtents(1000, 1000);
//	SetLogExtents(1000, 1000);

	// The virtual size defines the area that can be viewed by scrolling.
	// Set the virtual size to the entire diagram.
//	SetVirtualSize(CSize(sizeDiagram.cx, sizeDiagram.cy));
	SetVirtualSize(globalPixelX, globalPixelY);

	MvcViewport::DC dc(this);
	POINT ptPad = { 4, 4 };
	POINT ptOrig = { 0, 0 };
	dc.DPtoLP(&ptPad);
	dc.DPtoLP(&ptOrig);
	m_nPadding = abs(ptPad.y - ptOrig.y);

///	OnUpdateMappingMode();

//	CODMeasure unitsIn(OD_INCHES, 1);
//	CODMeasure unitsOut(OD_FEET, 4);
	CODMeasure unitsIn(OD_CENTIMETERS, 1);
	CODMeasure unitsOut(OD_CENTIMETERS, 1);
	GetRuler()->SetMeasurementScale(unitsIn, unitsOut);

//	m_gridSpacingX.SetUnits(OD_FEET);
//	m_gridSpacingX = 0.5f;
//	m_gridSpacingY.SetUnits(OD_FEET);
//	m_gridSpacingY = 0.5f;

//>OVs
	CDC *pDC = theApp.m_pMainWnd->GetDC();
	int logpixelsx = pDC->GetDeviceCaps(LOGPIXELSX);
	int logpixelsy = pDC->GetDeviceCaps(LOGPIXELSY);
	int gridPixelsX = 20;	// pixel per divisione orizzontale
	int gridPixelsY = 20;	// pixel per divisione orizzontale
	double gridCmX = ((double) gridPixelsX * 2.54) / logpixelsx;
	double gridCmY = ((double) gridPixelsY * 2.54) / logpixelsy;

	m_gridSpacingX.SetUnits(OD_CENTIMETERS);
	m_gridSpacingX = (float) gridCmX;
	m_gridSpacingY.SetUnits(OD_CENTIMETERS);
	m_gridSpacingY = (float) gridCmY;
//>OVs

//	MvcViewport::OnInitialUpdate();
	CODViewport::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CTurtleViewport implementation

BOOL CTurtleViewport::CreateController()
{
	m_pCtlr = new CTurtleController(this);
	m_bAutoDelCtlr = TRUE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
