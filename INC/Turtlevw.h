/* TURTLEVW.H

040130	aggiunto BitMultiSave
040126	in BitLoad e BitSave mod. tipo terzo argomento
040123	nuovi CTurtleView::BitLoad e CTurtleView::BitSave; rinominato vecchi in TurtleLoad e TurtleSave

031030	aggiunto metodo OnSetCursor e attributo m_hCursor
000502	aggiunto SetTextProperties
000402	aggiunto m_bPenTransparent e m_bFloodTransparent
000113	aggiunto m_dosFloodColor, GetDosFloodColor, SetDosFloodColor, SetPixel
991226	m_pBitmap[] in iperlvw.h (condivise tra finestre Tarta
991116	aggiunto m_isSelectionOn
991105	spostato gestione menu da turtleCtlr a turtleVw
991014  OVsZoom (OVs)
991014  mediato print e print-preview (OVs)
991012  riattivato override di PreCreateWindow
991011  aggiunto m_ogID
990920  aggiunto m_dlgEntry
990901  per compatibilita' con v.3.06, reinserito OnCmdMsg e OnWndMsg
990825	uso di MvcScrollView_T anziche' MvcScrollWrapper_T
		come da sample ShowcaseView.h
990819	impaccamento membri di struttura: /Zp1 -> /Zp8 (standard)
990818	sostituito CScrollView con CView
990814	aggiunto OnPrepareDC (OVs); m_vpCanvas -> m_vpDiagram
990813	aggiunto OnCmdMsg e OnWndMsg (OVs)
990812	variabile per viewport di MVC (OVs)
970801	Iniziato implementazione funzioni di menu
*/

// turtlevw.h : header file
//

// #include "ilcpp.h"

#include "stdafx.h"
#include "logo.h"

// #include "turtleDoc.h"		// OVs
// #include "turtleVp.h"		// OVs
// #include "MvcScrollView.h"	/// OVs

/////////////////////////////////////////////////////////////////////////////
// CTurtleView view
     
#ifndef DEF_TURTLEVIEW
#define DEF_TURTLEVIEW

//#define	MAX_BITMAPS 1024
    
/// class CTurtleView : public CView
#ifndef OVs
class CTurtleView : public CView
#else // OVs
class CTurtleView : public MvcScrollView_T<CTurtleViewport>
#endif // OVs
{
	DECLARE_DYNCREATE(CTurtleView)
protected:
	CTurtleView();			// protected constructor used by dynamic creation

	BOOL PreCreateWindow(CREATESTRUCT& cs);	////
	CMenu*  MenuGetItem (UINT iPos);

// Attributes
protected:
#ifdef OVs
//>OVs
	// This is the viewport that draws the visual representataion of
	// the model contained in the document object. Wrapper classes
	// are used to extend the viewport to support scrolling and
	// offscreen buffering.
///	MvcScrollWrapper_T<CTurtleViewport> m_vpDiagram;

	CTurtleModel* m_pModel;
	CODComponent* m_pCursor;	// graphic cursor (tarta)
	CODLineComponent* m_pLine;	// current line (spezzata)
	BOOL m_isPolyline;
	CODLineComponent* m_pPolygon;// current polygon
	BOOL m_isPolygon;
#endif // OVs

public:
	BOOL m_isOVs;
	node m_dlgEntry;

#ifdef OVs
	CTurtleDoc* GetDocument();
#endif OVs
///	CTurtleViewport* GetViewport();
//<OVs
 
// Operations
public:
//>OVs

// Overrides (da CShowcaseView)
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo*);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);

//	int CreateToolbar (UINT toolbarId, char* toolbarTitle, UINT toolarStyle, UINT toolbarDocking);
#ifdef OVs
	int OVsCreateToolbar (UINT toolbarId, char* toolbarTitle, UINT toolarStyle, UINT toolbarDocking);
	int OVsShowToolbar (CToolBar* pToolbar, int show);
	int OVsTools (int nextTools);
	int OVsProps (int nextProps);
	void GoHome();
	void ClearScreen (BOOL goHome);
	CODLineComponent* CloseLine();
	void CreateCursor ();
	void ShowCursor (BOOL show);
	void MoveCursor (int x, int y, double alfa);
	int BitLoadFromFile (CString strImagePath, int mode);
	BOOL FileLoad(BOOL bOVsMode, CString strFileName);
	BOOL FileSave(BOOL bOVsMode, BOOL bSaveAs, CString strFileName);
	void PrintPreview ();
	void SetComponentProperties (CODComponent* pComp);
	void SetTextProperties (CODTextComponent* pText);
	int OVsZoom (double factor);
//<OVs
#endif // OVs

unsigned long GetAttr ();
void SetAttr (node attrList);
void GetName(char* name);
void BitmapNew();
int GetPenStyle();
void SetPenStyle(int style);
int GetPenWidth();
void SetPenWidth(int width);
COLORREF GetPenColor();
void SetPenColor(COLORREF color);
COLORREF GetTextBkColor();
void SetTextBkColor(COLORREF color);
COLORREF GetBkColor();
void SetBkColor(COLORREF color);
COLORREF GetFloodColor();
void SetFloodColor(COLORREF color);

int GetDosPalette();
void SetDosPalette(int iPal);
int GetDosPenColor();
void SetDosPenColor(int iCol);
int GetDosBkColor();
void SetDosBkColor(int iCol);
int GetDosFloodColor();
void SetDosFloodColor(int iCol);

int GetDrawMode();
void SetDrawMode(int DrawMode);
int GetTextBkMode();
void SetTextBkMode(int mode);
LOGFONT *GetTextFont();
BOOL SetTextFont(LOGFONT *lpLogFont);
void SetRefresh(BOOL noyes);
COLORREF GetPixel(int x, int y);
int SetPixel(int x, int y, COLORREF color);	// 000113
int Point(int x, int y);
int MoveTo(int x, int y);
int LineTo(int x, int y);
int Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
int TextOut(int x, int y, char *s);
BOOL FloodFill(int x, int y);
void FillRect (int x, int y, int x2, int y2, COLORREF *pColor);
int Zoom (double factor, int xPos, int yPos, int xDim, int yDim);
int Resize (int xPos, int yPos, int xDim, int yDim);
void SetMouseState (int state);
CPoint GetMousePos ();
void SetKeybState (int state);
UINT GetKey ();
CPoint ViewToCanvas (CPoint point);
void ShowSelectArea ();
//>040123
//int BitLoad (int handle, int mode);
//int BitSave (int handle, int mode);
int TurtleLoad (int handle, int mode);
int TurtleSave (int handle, int mode);
int BitLoad (int bitIndex, char *filename, DWORD imagetype);
int BitSave (int bitIndex, char *filename, DWORD imagetype);
//<040123
int BitMultiSave (char *filename, DWORD imagetype, int nLoops, node bitIndexList, node delayList); // 040130
int BitCutCopy (int bitIndex, int is_cut, int x, int y, int width, int height);
int BitPaste (int bitIndex, int bitMode, int x, int y);
int BitFit (int bitIndex, int width, int height);
// int BitPrint ();
int TurtlePrint ();
int BitmapPrint (int xLowView, int yLowView, int xHighView, int yHighView);
void ActivatePalette (CDC *pDC);
void ClearPalette ();
// void SetActiveArea (double xLowArea, double xHighArea, double yLowArea, double yHighArea);
// void GetActiveArea (double *xLowArea, double *xHighArea, double *yLowArea, double *yHighArea);
void SetActiveArea (double xLowArea, double yLowArea, double xHighArea, double yHighArea);
void GetActiveArea (double *xLowArea, double *yLowArea, double *xHighArea, double *yHighArea);
int SelectionToActiveArea ();
void ResetActiveArea ();
void Center (int x, int y);
void OnTurtlestatusHide();
void OnTurtlestatusRefresh();
void MapClipInvalidateRect (int top, int left, int bottom, int right);

BOOL CTurtleView::GetModify();
void CTurtleView::SetModify(BOOL bModified);
int CTurtleView::ConfirmExit();

// Implementation
public:
	virtual ~CTurtleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// New	
	virtual	void OnDraw(CDC* pDC);		// overridden to draw this view
	virtual	void OnInitialUpdate();		// first time after construct

	HBITMAP GetClipboardData ();		// Clipboard data handle
	BOOL SetClipboardData(HBITMAP hBitmap, HPALETTE hPalette, HGLOBAL hDib);

	int m_nDC;
	CBitmap *m_pCanvas;					// screen image

public:		// 000113
	int m_dosPalette;
	int m_dosPenColor;
	int m_dosScreenColor;
	int m_dosFloodColor;
	int m_dosTextBkColor;
	COLORREF m_screenColor;
	COLORREF m_floodColor;
	COLORREF m_penColor;
	COLORREF m_textBkColor;		// sfondo per testo e hatched lines
	int m_textBkMode;			// modalita' sfondo per testo e hatched lines 
	int m_penStyle;
	int m_penWidth;
	int m_drawMode;
	BOOL m_bPenTransparent;		// if TTRUE, performs no writing
	BOOL m_bFloodTransparent;	// if TTRUE, performs no filling
public:
	BOOL m_refresh;
	int m_mouseState;
	CPoint m_mousePos;
	CPoint m_buttonDownPos;
	CPoint m_buttonUpPos;
protected:
	int m_keybState;
	UINT m_key;
	UINT m_keybFlags;
	LOGFONT* m_pLogFont;
	CFont *m_pFont;
//	CBitmap *m_pBitmap[MAX_BITMAPS];		// result of BitCut / BitCopy
	double m_xLowArea, m_xHighArea, m_yLowArea, m_yHighArea;
	CRect m_rectSelectArea;		// area selezionata (coordin. fisiche)
	BOOL m_isSelectArea;		// l'area selezionata e' mostrata
	BOOL m_isDrag;				// e' in corso dragging del cursore
	BOOL m_bModified;			// l'immagine e' stata modificata
	CWnd* m_pOldMain;			// main application window (v. print preview)

public:
	BOOL m_isSelectionOn;		// e' consentita selezione interattiva (down-drag-up)
	LOGFONT m_logFont;			// 980401: was protected
	int m_xDim, m_yDim;
	int m_xPos, m_yPos;
	int m_xPixel, m_yPixel;		// size of memory canvas
	int m_xScroll, m_yScroll;	// size of scrolled window
	int *m_pMaxX, *m_pMaxY;		// pointers to fields in C structures
	CWnd* m_frame;
	char m_saveFile [64];
	struct dview* m_logoView;
	int m_nView;
	CWnd* m_status;
	double m_zoomFactor;
	BOOL m_eraseFlag;			// modo penna ERASE: usare col.sfondo
#ifdef OVs
	ODComponentId m_ogID;		// id ultimo oggetto doppiocliccato
#endif // OVs
	HCURSOR m_hCursor;			// 031030: handle di cursore grafico

	// Generated message map functions
	public:
	//{{AFX_MSG(CTurtleView)
	afx_msg void OnBitmapPrint();
	afx_msg void OnBitmapActivearea();
	afx_msg void OnBitmapLoad();
	afx_msg void OnBitmapNew();
	afx_msg void OnBitmapSave();
	afx_msg void OnBitmapSaveas();
	afx_msg void OnSetFloodcolor();
	afx_msg void OnSetFont();
	afx_msg void OnSetPencolor();
	afx_msg void OnSetScreencolor();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomNormal();
	afx_msg void OnZoomOut();
	afx_msg void OnTurtlestatusShow();
	afx_msg void OnFileClose();
	afx_msg void OnPrintSetup();
	afx_msg void OnViewCenter();
	afx_msg void OnSetPenminus();
	afx_msg void OnSetPenplus();
	afx_msg void OnSelectTurtle();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditDelete();
	afx_msg void OnEditPaste();
	afx_msg void OnSetPalette();
	afx_msg void OnEditFill();
	afx_msg void OnEditSelectall();
	afx_msg void OnEditSelectarea();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrintsetup();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditButta(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditTaglia(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRiempi(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopia(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);// 031029
	//}}AFX_MSG
	afx_msg void OnMenuButton(UINT uID);				// 991105
	afx_msg void OnUpdateMenuButton(CCmdUI *pCmdUI);	// 991105
	DECLARE_MESSAGE_MAP()
};

#ifdef OVs
#ifndef _DEBUG  // debug version in turtleVw.cpp
inline CTurtleDoc* CTurtleView::GetDocument()
   { return (CTurtleDoc*)m_pDocument; }
#endif
#endif OVs

#endif
/////////////////////////////////////////////////////////////////////////////
