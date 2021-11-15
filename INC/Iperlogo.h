/* iperlogo.h

010110	LibraryProtectionIn
991226	m_pBitmap[] da iperlvw.h
*/
// iperlogo.h : main header file for the IPERLOGO application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

/// #include "..\resource.h"       // main symbols
#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIperlogoApp:
// See iperlogo.cpp for the implementation of this class
//
           
#define MAX_TURTLE_VIEWS    64 // 16
#define MAX_EDITOR_VIEWS    64 // 16
#define MAX_BROWSER_VIEWS	64 // 16
#define MAX_PLAYER_VIEWS    64 // 16
#define	MAX_BITMAPS 1024
    
class CIperlogoApp : public CWinApp
{
public:
	CIperlogoApp();
	virtual ~CIperlogoApp();

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CSingleDocTemplate* NewTurtleTemplate();
	CSingleDocTemplate* NewEditorTemplate();
	CSingleDocTemplate* NewBrowserTemplate();

	BOOL FirstInstance();			// 980624
	int LoadUpdateProfile(int Update);
	void ExitApplication();
	int ProtectionIn ();
	int ProtectionOut ();
	int LibraryProtectionIn ();	// 010110

	int GetTurtleViewIndex (class CTurtleView* pView);
	int GetEditorViewIndex (class CEditorView* pView);
#ifdef _WEB
	int GetBrowserViewIndex (class CWebView* pView);
#endif _WEB

// Implementation
	CPalette *m_pPalette;				// 980223
	COLORREF m_customColors[16];		// 980225
	CBitmap *m_pBitmap[MAX_BITMAPS];	// 991226

	class CTurtleView *m_turtleViews[MAX_TURTLE_VIEWS];
	class CEditorView *m_editorViews[MAX_EDITOR_VIEWS];
#ifdef _WEB
	class CWebView *m_browserViews[MAX_BROWSER_VIEWS];
#endif _WEB
	class CWnd *m_playerViews[MAX_PLAYER_VIEWS];

	CString m_ClasseConClick;
	BOOL m_mainStatusShown;			/* status window shown toggle */ 
	HGLOBAL m_mainPrinterData;		/* printer initialization handle for Turtle window*/
	HGLOBAL m_turtlePrinterData;	/* printer initialization handle for Turtle window*/
	HGLOBAL m_editorPrinterData;	/* printer initialization handle for Editor window*/
	DEVMODE m_mainPrinterMode;		/* printer initialization data for Turtle window*/
	DEVMODE m_turtlePrinterMode;	/* printer initialization data for Turtle window*/
	DEVMODE m_editorPrinterMode;	/* printer initialization data for Editor window*/

// Registration-related info
	CString m_regPath;				// protection-related data
	CString m_regSerial;			// 
	CString m_regName;				//
	CString m_regCompany;			//
	CString m_regCode;				//
	int m_regDays;					//
	int m_regTimes;					//
	int m_regAccess;				//

// Display capabilities				// 980327
	int driverversion;
	int technology;

	int rastercaps;
	int sizepalette;
	int numreserved;
	int colorres;

	int curvecaps;
	int linecaps;
	int polygonalcaps;
	int textcaps;
	int clipcaps;

	int horzsize;
	int vertsize;
	int horzres;
	int vertres;
	int logpixelsx;
	int logpixelsy;
	int bitspixel;
	int planes;
	int numbrushes;
	int numpens;
	int numfonts;
	int numcolors;
	int aspectx;
	int aspecty;
	int aspectxy;
	int pdevicesize;

	//{{AFX_MSG(CIperlogoApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

                   
extern CIperlogoApp theApp;                   
/////////////////////////////////////////////////////////////////////////////
