// turtlevw.cpp : implementation file
/*	TURTLEVIEW.CPP

211015	commentato primitive Tarta a Oggetti (PR_OG.. ecc.)

PORT 2021

040130	aggiunto CTurtleView::BitMultiSave; incluso ximagif.h per sottoclasse di CxImage 
040126	controllo argomenti di BITLOAD e BITSAVE; ritoccato CTurtleView::BitSave per GIF
040124	completato 1^ versione di CTurtleView::BitLoad e CTurtleView::BitSave
040123	nuovi CTurtleView::BitLoad e CTurtleView::BitSave; rinominato vecchi in TurtleLoad e TurtleSave
		rinominato _BITLOAD -> _TURTLELOAD e _BITSAVE -> _TURTLESAVE
		rinominato TurtleBitLoad -> TurtleTurtleLoad e TurtleBitSave -> TurtleTurtleSave
		rinominato BITLOAD -> PR_TURTLELOAD e BITSAVE -> PR_TURTLESAVE
		introdotto nuove PR_BITLOAD e PR_BITSAVE
		incluso ximage.h

031030	aggiunto metodo OnSetCursor e attributo m_hCursor
000713	disattivato OnRButtonDown/Up in OVs (provvisoriamente)
000628	aggiunta la sezione canvas al profilo (CanvasSize)
		modificato interfaccia di TurtleOpenView
		corretto e/o ripulito apri_tarta, apri_view, GetNewTurtle, GetAttr
000508	aggiunto PR_GO_IMAGE e PR_GO_IMAGE_DIM
000508	in PR_GO_TEXT e PR_GO_LABEL corretto dim. per MULTILINE
000502	esteso PR_GO_TEXT; corretto dimensionamento in PR_GO_LABEL
		definito SetTextProperties (usato in PR_GO_TEXT e PR_GO_LABEL)
000428	in PR_GO_DRAW proprietà OD_EDIT_CANSELECT
		presa da attr. SELEZIONE della finestra
000421	spostato OnFileOpen, OnFileSave, OnFileSaveAs in turtleCtrl.cpp
000419	pulizia parziale
000402	aggiunto m_bPenTransparent, m_bFloodTransparent e TurtleSetFloodTransparent
000318	agg. terzo argomeno a PR_GO_LABEL (min 1, default 3)
000314	in PR_COMPTRANSLA ecc. aggiunto AdjustLinksIfSymbol
000313	aggiunto PR_GO_SETPANSELECT
000228	corretto PR_SETTURTLEPROPS
000130	mod. specifiche PR_SYMBOL_SETLABEL (già PR_SYMBOL_ADDLABEL)
000125	rifarrto GO_DRAW, separato da GO_APPEND
		corretto ASSFONDO: messo _PULISCI sotto condizione
		revisione primitive di posizione, direzione e scala (GO)
000116	aggiornato PR_GO_SETPOS (non ancora usato)
		aggiunto argomento a coordinate_double e adattato 4 chiamate
000113	aggiunto m_dosFloodColor, GetDosFloodColor, SetDosFloodColor, TurtleGetDosFloodColor, TurtleSetDosFloodColor
		modificato SETFLOODCOLOR
		aggiunto SetPixel e PR_SETPIXEL
000112	per compatibilità MswLogo:
		mod. convDosPalette, convColor, dosColors, SetDosPenColor, SetDosBkColor
		da completare analogo per colore di riempimento
000102	ripristinato pc_MODO, usata al posto di pc_TARTA
991227	corretto apri_tarta (caso in cui coordinate logiche sono esplicite)
991226	m_pBitmap[] in iperlvw.h (condivise tra finestre Tarta)
991224	implementato PR_GO_CURVE, PR_GO_CLOSEDCURVE e PR_GO_TEXT
		agg. widthViewToLogo e heightViewToLogo
991223	in OnInitialUpdate, reincluso inizializzazione palette anche per OVs
		aggiunto PR_SETTEXTFONTLOG e PR_LABELDIMLOG
991213	UpdateParent (disattivato)
991211	implementato PR_GO_ELLIPSE; aggiornato PR_COMPRUOTA
991208	PR_GO_LABEL e PR_SYMBOL_ADDLABEL
		PR_SYMBOL_INSERTLABEL, PR_SYMBOL_REMOVELABEL: arg_1 è Id o Pointer
991202	aggiunto PR_SYMBOLINSERT, simile a PR_NODEINSERT, ma senza copia
991130	riorganizzato primitive OG, separando creazione da append
991116	aggiunto attributo SELEZIONE; aggiunto m_isSelectionOn; gestito meglio m_isDrag
		aggiunto OnUpdateEditButta, OnUpdateEditTaglia, OnUpdateEditCopia, OnUpdateEditRiempi
991105	spostato gestione menu da turtleCtlr a turtleVw
		dimensionamento canvas alla view solo se mancano sia Titolo che Menu
991103	in OnInitialUpdate posposto richiamo a omonima di CViewport
991029	mod. Zoom e ClearScreen (OVs)
991028	PR_SYMBOL_INSERTLABEL (già ADD), PR_SYMBOL_REMOVELABEL e PR_SYMBOL_LABELNUMBER
991026	mod. specs di PR_SYMBOLLOAD e PR_NODEINSERT
		agg. PR_NODECREATE; mod. PR_SYMBOL_ADDLABEL
991021	ancora OnWndMsg; 8 primitive per oggetti grafici
		aggiunto LogoToText in ilutil.cpp
991018  rivisto OnWndMsg
991014  aggiunto OnUpdateFilePrintPreview, OnUpdateFilePrint
		distinto caso OV in OnFilePrint, OnFilePrintPreview
		copiato codice di CView::OnFilePrintPreview per customizzarlo
991012  aggiunto override di PreCreateWindow
991011  in PR_COMPREMOVE aggiunto ClearSelection
991001  revisione di zoom e centratura
990930  gestione di attributo SCALA (pc_SCALA) e OR (pc_OR) come in Iperlogo 0
		( #define NEW_SCALE )
990923  aggiunto PR_NODEINSERT
990920  aggiunto m_dlgEntry
990917	primitive PR_TURTLEPROPS (TARTAPROP) e PR_TURTLETOOLS (TARTATOOL) 
990913	primitive per diagrammi e oggetti grafici
990903  corretto  CTurtleView::BitCutCopy (non funzionava menu Composizione-Butta
		inserito voci di menu e toolbar di OVs
990902  gestione cursore grafico (tarta)
990901  per compatibilita' con v.3.06, reinserito OnCmdMsg e OnWndMsg
990831  cambiate specifiche di TurtleBitLoad
990830	disattivato ::Center
990825	uso di MvcScrollView_T anziche' MvcScrollWrapper_T
		come da sample ShowcaseView.cpp
		risostituito CView con CScrollView
990820	modificato CTurtleView::LineTo
		aggiunto CloseLine
990818	elim. codice obsoleto (ifndef DRAW_INDIRECT, NEW_ZOOM)
		sostituito CScrollView con CView
		commentato GetScrollPosition, SetScrollSizes
990814	aggiunto OnPrepareDC; mod. OnDraw (OVs)
		m_vpCanvas -> m_vpDiagram (come esempi)
990813	aggiunto e OnCmdMsg e e OnWndMsg (OVs)
990812	aggiunto viewport di MVC (OVs)
		aggiunto OnCreate, OnSize
	modificato OnDraw e OnInitialUpdate
990621	aggiunto TurtleHandle2Frame (per ToolFrame)
990526	aggiunto nuovo BLOCCOSTAMPA (BITPRINT ora = TARTASTAMPA)
990518	aggiunto primitive
		RIEMPISELEZIONE, SELEZIONAREATTIVA, ATTIVASELEZIONE, CENTRA
990515	riattivato scrittura su schermo TARTA (tputs)
990514	in GetKey aggiunto considerazione di Shift e Control
990513	implementazione dei tasti programmati con fuoco a Tarta
		(ASTASTO) in alternativa a KEYBOARDON/KEYBOARDOFF
		emulazione dei codici ASTASTO/TASTO di Superlogo
990317	in SetRefresh (REFRESH) uso di ActiveArea
990311	completato REFRESH / NOREFRESH
		corretto gestione di titolo in GETATTR; comunque
		titolo default posto uguale a nome della finestra
990222	PR_LABELDIM (ETICHETTADIM); esteso ::TextOut
990129	eliminato menu Imposta - Palette se manca palette
990119	in GetNewFrame eliminato pTemplate->OpenDocumentFile(NULL)
990112	PR_TARTA, PR_CHIUDITARTA
981208	semplificato sintassi di ATTR e SETATTR per valori coppia
		aggiunto primitiva CREATARTA nome lista-associativa
981123	aggiunto ::Attr e ::SetAttr
981119	corretto codifica di BITMODEs (ternary raster operations)
		per renderla compatibile con MswLogo;
981103	modificato ordine argomenti di SETACTIVEAREA e ACTIVEAREA
		per compatibilita' con MswLogo
981016	aggiunto arg. mode a BitLoad, TurtleBitLoad, _BITLOAD
		aggiunto arg. mode a BitSave, TurtleBitSave, _BITSAVE
		corretto ::BitLoad
		riattivato CONSERVA_DIS (=BITSAVE) e RECUPERA_DIS
		reso OnBitLoad equivalente a RECUPERA_DIS !!!
980911	aggiunto ASMODOPENNA (PR_SETPENMODE), ritoccato nomi e
		ristrutturato l'insieme delle primitive correlate
		aggiunto quarto elemento a risultato di PENNA (PEN)
		ASPENNA (SETPEN) accetta lista da 1 a 4 elementi
		Corretto CANCEPENNA con uso di m_eraseFlag e modifica
		di tutte le primitive che scrivono con la penna
		Eliminato 4' argomento (non usato) da line e TurtleLine,
		pset e TurtlePoint
980908	VAXY cambia specifiche (come SETXY di MswLogo) !!!!!!!!
		aggiunto VAPOS come precedente VAXY (SETPOS di MswLogo)
980903	chiusura morbida mediante SetModify, GetModify e ConfirmExit
980831	eliminato extern "C" per conversione a C++ dei moduli C
		interfaccia apri_tarta (2ndo argomento) : char * -> const char *
980722	Gestione colore di sfondo del testo e di linee hatched
		Ritoccato calcolo area di testo in TextOut
980721	Iniziato a gestire eventi da tastiera con focus su Tarta
		Modificato KwAttr secondo vera sintassi di lista associativa
980717	Iniziato a usare KwAttr per uso di associazioni in lista argomenti
		test per gestione di assenza menu
		tolto baco in chiudi_tarta
980512	Eliminato baco residuo in metodo TextOut
		Corretto errore di interfaccia in metodo Arc
980509	Eliminato baco nuovo in metodo FloodFill
980508	Corretto BITBLOCK
980430	Mod. interfaccia di TurtleFillRect e CTurtleView::FillRect
		ora l'ordine e': left, top, right, bottom
980429	Esteso nuovo zoom a ::CutCopy, ..
		Mod. turtle::FillRect (VERIFICARE ed ESTENDERE)
980428	Definito nuovo meccanismo di zoom
980424	esperimento su InvalidateRect (in LineTo)
980401	ristrutturato parse dei font
980331	finito (?)revisione stampa
		revisione BitSave (con uso di Dib)
980330  test su presenza di palette in RASTERCAPS
980327	pulizia e uso esteso della palette
980303	inserimento dei nuovi colori in palette di Iperlogo
980227	esperimenti di cattura del cursore
980226	modificato SetClipboardData
980223	aggiunto modulo CDib
		m_pPalette diventa globale (variabile di theApp)
980220	introdotto m_pPalette
980219	definito ReadDib
980217	aggiornato (corretto) SaveBitmapToFile
		(_lwrite -> _write, _llseek -> _lseek)
		DibToBitmap per modularita'
980217	implementato BITFIT
980216	BitmapToSharedDib per gestione di Clipboard
980213	iniziato gestione di Clipboard (BITINDEX = 0)
980209	aggiunto TurtleExit
980203	rivisto congruenza di funzioni relative a "view" TARTA
		aggiunto TurtleWindows
980202	ritoccato salvataggi di i_tarta, nview, pt
		eliminato riferimenti a Q_TARTA
		modificato switch_tarta (eliminare ridondanze ?)
980130	merge del file con ILTARTA.C

TURTLEVIEW
	980127	aggiunto ViewToCanvas, usato per aggiustare mousePos
	980126	WIN32: eliminato spazi dalla stringa filtro dei CFileDialog
	971229	modificato uso di InvalidateRect in Point, LineTo, Arc, ..
			aggiunto TurtleGetPixel (LEGGIPUNTO)
			corretto BitLoad, BitCut/Copy e BitPaste
	971223	provata modifica metodo FloodFill (poi ripristinato)
			realizzato OnSetScreencolor con PULISCI
			modificato interfaccia di _ZOOM (ora e' fattore assoluto) 
			modificato TurtleBringToTop
			aggiornato BitLoad (su posizione) e BitSave (su area attiva)
	970916	iniziato gestione della stampa 
	970915	iniziato gestione dell'Active Area 
	970908	per compatibilita' VC++5.0: _fmemset -> memset; eliminato huge e _huge (obsoleti) 
	970801	Iniziato implementazione funzioni di menu

ILTARTA
	980128	completato merge con ILVIEW.C (eliminato)
			persa residua compatibilita' con versione DOS e WIN16 (?)
	971229	riabilitato PUNTO
	971223	aggiunto GetIntPos; realizzato ASSFONDO con _PULISCI (anziche' RIEMPI)
			ZOOM: cambiato specifiche: ora argomento e' fattore assoluto
	971118	modificato apri_tarta per compatibilita' con creazione interattiva
	971023	adeguato _ZOOM a recenti modifiche
	971022	reso autonoma scelta della dimensione fisica della view
	971020	modificato default per ratio (rapporto tra coordinate logiche e fisiche)
  			arrotondamenti a zero in SPOSTATI
	970916	introdotto RESETACTIVEAREA; corretto ARC
	970911	sostituito nview con i_tarta come riferimento a variabile globale
  			in switch_tarta aggiunto emersione fisica della finestra 
	970908	per compatibilita' VC++5.0: _fstrcpy -> strcpy 
	970904	In apri_tarta/chiudi_tarta aggiunta/tolta voce a menu WINDOWS
	970804	Aggiunto paramentro MENU in apertura TARTA
	970804	Iniziato ripulitura (eliminazione codice commentato per vers. DOS)
	970801	Iniziato implementazione funzioni di menu
	970422	recupero update del 02.10.96 (normalizzazione)
	960924	aggiunto WaitMessage dopo apri_tarta
	960507	deivato parse da analisi
	960521	primo port sotto Windows da ILTARTA.C del 19.09.89
*/

#include "stdafx.h"

#include "logo.h"
// #define pc_VISTA pc_CENTRATO
#define pc_SELEZIONE kw [83]

#include "iperlogo.h"
// #include "turtleMdl.h"	// OVs
// #include "turtleDoc.h"	// OVs
// #include "turtleCtlr.h"	// OVs
#include "turtlevw.h"

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"
#include "turtlefr.h"
#include "statdial.h"
#include "activear.h"

#include "ilcpp.h"
#include "ildialog.h"
#include "CDib.h"
#include "paletted.h"

#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>

// #include "OdGuids.h"

// #include "ximage.h" // 040123
#include "../CxImage/ximage.h"
// #include "ximagif.h" // 040130
#include "../CxImage/ximagif.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SAVE
#define RESTORE
#define NEW_FONT
#define DRAW_INDIRECT
#define NEW_ZOOM
// #define OVs
#define NEW_SCALE
#define NEW_CENTER

#define writeColor ((m_eraseFlag) ? m_screenColor : m_penColor)

#ifdef OVs
extern int defaultTurtleMode;	// modalita' TARTA default (OVs o no)
int turtleMode;					// modalita' finestra TARTA da aprire
#endif // OVs

extern CIperlogoApp theApp;
extern CREATESTRUCT Turtle;
extern CString ini_name;
extern CString turtle_name;
extern char TurtleFont[]; 
extern DlgList Dialogs;

int CursorSide;
char DefaultTurtle[64] = "";
int newTurtleCounter = 1;

CSize CanvasSize;	// 000628

void MainMenuRemoveWindowItem (char *name);
void MainMenuAppendWindowItem (char *name);
void MainMenuCheckWindowItem (UINT iMenu, char *name, BOOL status);
int currentWindow (int mask);

//>040123
//int _BITLOAD (int nView, int handle, int mode);
//int _BITSAVE (int nView, int handle, int mode);
int _TURTLELOAD (int nView, int handle, int mode);
int _TURTLESAVE (int nView, int handle, int mode);
//<040123

void _BITPASTE (int nView, int bitIndex);
void _BITCUTCOPY (int nView, int bitIndex, int isCut, int x, int y, int width, int height);
void _BITBLOCK (int nView, int x, int y, int width, int height);
void PULISCISCHERMO ();
void PULISCI ();

node dev2nome (int dev);
int view_to_devtarta (int view);
int xLogoToView (double x);
int yLogoToView (double y);
void GetIntPos (int nView, int *x, int *y);
// void GetIntActiveArea (int nView, int *xLowView, int *xHighView, int *yLowView, int *yHighView);
void GetIntActiveArea (int nView, int *xLowView, int *yLowView, int *xHighView, int *yHighView);
int conv_num_double (node oggetto, double *num);
int conv_num_node (node oggetto, node *num);
int ListToFont (node tail, LOGFONT* lpLogFont);
void LogoToText (node logo, char* pText);
int bool_ (node arg);

node KeyProg (UINT nChar);

//>000112
// COLORREF dosColors[16] = {
COLORREF dosColors[32] = {
//<
	RGB(0,0,0),			//  0:	Nero
	RGB(0,0, 128),		//  1:	Blu
	RGB(0,128,0),		//  2:	Verde
	RGB(0, 0,255),		//  3:	Azzurro (medium blue: 0, 0,205)
	RGB(255,0,0),		//  4:	Rosso
	RGB(64,0,128),		//  5:	Viola (magenta: 255, 0, 255)
	RGB(128,64,0),		//  6:	Marrone
	RGB(192,192,192),	//  7:	Grigio chiaro
	RGB(128,128,128),	//  8:	Grigio
	RGB(128,192,255),	//  9:	Celeste (light blue: 173,216,230)
	RGB(64,255,64),		// 10:	Verde chiaro
	RGB(64, 128, 192),	// 11:	Azzurro chiaro (sky blue: 136, 206,237)
	RGB(255,128,255),	// 12:	Rosa, oppure (255,128,192)
	RGB(255,0,255),		// 13:	Viola chiaro (magenta: 255,0,255)
	RGB(255,255,0),		// 14:	Giallo (yellow)
	RGB(255,255,255),	// 15:	Bianco
//>000112
	RGB(0,0,0),			// 16+0:	Nero
	RGB(0,0,255),		// 16+1:	Blue
	RGB(0,255,0),		// 16+2:	Verde
	RGB(0,255,255),		// 16+3:	Turchese ?
	RGB(255,0,0),		// 16+4:	Rosso
	RGB(255,0,255),		// 16+5:	Giallo (yellow)
	RGB(255,255,0),		// 16+6:	Viola chiaro (magenta: 255,0,255)
	RGB(255,255,255),	// 16+7:	Bianco
	RGB(155,96,59),		// 16+8:
	RGB(197,136,18),	// 16+9:
	RGB(100,162,64),	// 16+10:
	RGB(120,187,187),	// 16+11:
	RGB(255,149,119),	// 16+12:
	RGB(144,113,208),	// 16+13:
	RGB(255,163,0),		// 16+14:	Marrone chiaro ?
	RGB(183,183,183),	// 16+15:	Grigio chiaro
//<
};

int dosPalettes [2][4] = {
	{ 0, 2, 4, 14 },	// tavolozza 0 (indici in dosColors)
	{ 0, 9, 5, 15 }		// tavolozza 1 (indici in dosColors)
};

void CTurtleView::GetName(char* name)
{
	int dev = view_to_devtarta (m_nView);
	getnam (name, _nome [dev]);
}

CWnd* TurtleHandle2Frame (int nview)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	return turtleView->m_frame;
}

CMenu* TurtleGetMenuItem (int nview, int iPos)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	CWnd* turtleFrame = turtleView->m_frame;
	CMenu* turtleMenu = turtleFrame->GetMenu ();
	if (! turtleMenu) return NULL;	// 980717
	return turtleMenu->GetSubMenu (iPos);
}

CMenu* TurtleGetMenu (int nview)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	CWnd* turtleFrame = turtleView->m_frame;
	return turtleFrame->GetMenu ();
}

void TurtleMenuItemEnable (int nview, UINT itemID, BOOL status)
{
	UINT nEnable = (status) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	CMenu* menu = TurtleGetMenu (nview);
	if (! menu) return;		// 980717
	UINT oldStatus =
		menu->EnableMenuItem(itemID, MF_BYCOMMAND | nEnable);
}

void TurtleMenuActivateSelect (int nview, BOOL status)
{
	UINT nEnable = (status) ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	CMenu* menu = TurtleGetMenu (nview);
	if (! menu) return;		// 980717
	UINT oldStatus =
		menu->EnableMenuItem(ID_SELECT_TURTLE, MF_BYCOMMAND | nEnable);
}

void TurtleMenuCheckSelect (int nview, BOOL status)
{
	UINT nCheck = (status) ? MF_CHECKED : MF_UNCHECKED; 
	CMenu* menu = TurtleGetMenu (nview);
	if (! menu) return;		// 980717
	menu->CheckMenuItem(ID_SELECT_TURTLE, MF_BYCOMMAND | nCheck);
}

/////////////////////////////////////////////////////////////////////////////
// from SECONDO.CPP

// usata temporaneamente al posto di argomento !!!
char nomeTarta[MAXCHARS];

CMainFrame* GetNewFrame(int x, int y, int dimx, int dimy, char *pTitle, int menu)
{
	int oldX = Turtle.x, oldY = Turtle.y;
	int oldDimx = Turtle.cx, oldDimy = Turtle.cy;
	char * pOldTitle = (char *) Turtle.lpszName;
	HMENU oldMenu = Turtle.hMenu;
	
	if (x != -1) Turtle.x = x;
	if (y != -1) Turtle.y = y;
	/// if (dimx != -1) Turtle.cx = dimx;
	/// if (dimy != -1) Turtle.cy = dimy;
	Turtle.lpszName = pTitle;
	if (menu != -1) Turtle.hMenu = (HMENU) menu ;
	
	CSingleDocTemplate* pTemplate = theApp.NewTurtleTemplate();
	ASSERT((pTemplate != NULL));
	ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
	pTemplate->OpenDocumentFile(NULL);
	CMainFrame* turtleFrame = (CMainFrame*) theApp.m_pMainWnd;
	
	Turtle.x = oldX; Turtle.y = oldY;
	Turtle.cx = oldDimx; Turtle.cy = oldDimy;
	Turtle.lpszName = pOldTitle;
	Turtle.hMenu = oldMenu ;
	
	return turtleFrame;
}

int *globalDimX, *globalDimY;
int globalPixelX, globalPixelY;

CTurtleView* GetNewTurtle (int nview, dview* logoView, int x, int y, int dimx, int dimy, int pixel_x, int pixel_y, int *p_maxx, int *p_maxy, char *pTitle, int menu)
{
	if (dimx != -1) Turtle.cx = dimx;		//anticipato, da GetNewFrame
	if (dimy != -1) Turtle.cy = dimy;		//

	globalPixelX = pixel_x;
	globalPixelY = pixel_y;
	
	// salvo il MainFrame nominale dell' applicazione 
	CWnd* pOldMain = AfxGetApp()->m_pMainWnd;
	AfxGetApp()->m_pMainWnd = 0L;	 
	
	CMainFrame* turtleFrame = GetNewFrame (x, y, dimx, dimy, pTitle, menu);    
	CTurtleView* turtleView = (CTurtleView*) turtleFrame->GetActiveView();
//	turtleView->GetDocument()->SetTitle("Turtle");
	turtleView->GetDocument()->SetTitle(nomeTarta);
	theApp.m_turtleViews[nview] = turtleView;
	turtleView->m_logoView = logoView;
	turtleView->m_frame = turtleFrame;

	TurtleMenuItemEnable (nview, ID_EDIT_TAGLIA, FALSE);
	TurtleMenuItemEnable (nview, ID_EDIT_COPIA, FALSE);
	TurtleMenuItemEnable (nview, ID_EDIT_BUTTA, FALSE);
	TurtleMenuItemEnable (nview, ID_EDIT_RIEMPI, FALSE);
	if (! (theApp.rastercaps & RC_PALETTE)) {
		CMenu* turtleMenu = turtleFrame->GetMenu ();
		if (turtleMenu)
			turtleMenu->RemoveMenu(ID_SET_PALETTE, MF_BYCOMMAND);
	}
	if (pTitle) {
		turtleFrame->SetWindowText((pTitle == (char *) -1L) ? nomeTarta : pTitle);
	}

//> 000628
//	turtleView->m_xPos = x;	// ma che c'entra ?? 990827
//	turtleView->m_yPos = y; //
//<
	turtleView->m_pMaxX = p_maxx;
	turtleView->m_pMaxY = p_maxy;
	turtleView->m_xPixel = globalPixelX;				
	turtleView->m_yPixel = globalPixelY;
#ifdef NEW_ZOOM
	turtleView->m_xScroll = turtleView->m_xPixel;
	turtleView->m_yScroll = turtleView->m_yPixel;
#endif // NEW_ZOOM

	*(turtleView->m_pMaxX) = turtleView->m_xPixel;		
	*(turtleView->m_pMaxY) = turtleView->m_yPixel;
	
	turtleView->m_saveFile[0] = 0;
	turtleView->m_logoView = logoView;
	turtleView->m_nView = nview;
	turtleView->m_status = NULL;
	turtleView->m_zoomFactor = 1.0; 
	
	// ripristino il MainFrame nominale dell' applicazione 
	AfxGetApp()->m_pMainWnd = pOldMain;
	
	return turtleView;
}

/////////////////////////////////////////////////////////////////////////////
// CTurtleView

// int TurtleOpenView (node name, int nview, struct dview* logoView, int x, int y, int dimx, int dimy, int *p_maxx, int *p_maxy, char *pTitle, int is_menu)
#ifdef GFX // 000628
int TurtleOpenView (node name, int nview, struct dview* logoView, int x, int y, int dim_x, int dim_y, int pixel_x, int pixel_y, int *p_maxx, int *p_maxy, char* pTitle,  int is_menu)
#else // GFX
CTurtleView* TurtleOpenView (node name, int nview, struct dview* logoView, int x, int y, int dim_x, int dim_y, int pixel_x, int pixel_y, int *p_maxx, int *p_maxy, char* pTitle,  int is_menu)
#endif // GFX
{
	pacstr (nomeTarta, nome (name));

	CTurtleView* turtleView;
	
	turtleView = GetNewTurtle (nview, logoView, x, y, dim_x, dim_y, pixel_x, pixel_y, p_maxx, p_maxy, pTitle, is_menu);

	node setup = cons (NULL, cons(NULL, cons(NULL, cons(NULL, cons(NULL, cons(NULL, cons(NULL, NULLP,
		LIST), LIST), LIST), LIST), LIST), LIST), LIST);
	DlgEntry entry = CreateDlgEntry(turtleView, TURTLE, 0, NULLP, name, NULLP, setup, NULLP);
	AddEntry(&Dialogs, entry, name);
	turtleView->m_dlgEntry = entry;

//>000628
//	return nview;
	return turtleView;
//<
}

void TurtlePostOpenView (int nview)
{
	return;
}

void TurtleCloseView (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	delete(turtle->m_frame);
	theApp.m_turtleViews[nview] = NULL;
}

void TurtleBringToTop (int nview) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	(turtle->m_frame)->ShowWindow (SW_RESTORE);
	(turtle->m_frame)->BringWindowToTop ();
}

node TurtleGetAttr (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetAttr ();
} 
void TurtleSetAttr (int nview, node attrList)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetAttr (attrList);
} 

int TurtleGetPenStyle (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetPenStyle ();
} 
void TurtleSetPenStyle (int nview, int style)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetPenStyle (style);
} 

int TurtleGetPenWidth (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetPenWidth ();
}
void TurtleSetPenWidth (int nview, int width)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetPenWidth (width);
}

void TurtleGetPenColor (int nview, DWORD *pColor)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	*pColor = turtle->GetPenColor ();
}
void TurtleSetPenColor (int nview, DWORD *pColor)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetPenColor(*pColor);
} 

int TurtleGetDosPalette (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetDosPalette ();
}
void TurtleSetDosPalette (int nview, int iPal)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetDosPalette(iPal);
} 

int TurtleGetDosPenColor (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetDosPenColor ();
}
void TurtleSetDosPenColor (int nview, int iCol)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetDosPenColor(iCol);
} 

int TurtleGetDosBkColor (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetDosBkColor ();
}
void TurtleSetDosBkColor (int nview, int iCol)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetDosBkColor(iCol);
} 

void TurtleGetTextBkColor (int nview, DWORD *pColor)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	*pColor = turtle->GetTextBkColor ();
}
void TurtleSetTextBkColor (int nview, DWORD *pColor)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetTextBkColor(*pColor);
} 

int TurtleGetTextBkMode (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetTextBkMode ();
}
void TurtleSetTextBkMode (int nview, int mode)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetTextBkMode(mode);
} 

void TurtleGetBkColor (int nview, DWORD *pColor) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	*pColor = turtle->GetBkColor();
}
void TurtleSetBkColor (int nview, DWORD *pColor) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetBkColor(*pColor);
}

void TurtleGetFloodColor (int nview, DWORD *pColor) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	*pColor = turtle->GetFloodColor();
}
void TurtleSetFloodColor (int nview, DWORD *pColor) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetFloodColor(*pColor);
}

void TurtleSetFloodTransparent (int nview, BOOL bTransparent) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->m_bFloodTransparent = bTransparent;
}

int TurtleGetDosFloodColor (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetDosFloodColor ();
}
void TurtleSetDosFloodColor (int nview, int iCol) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetDosFloodColor(iCol);
}

int TurtleGetDrawMode (int nview) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetDrawMode();
}
void TurtleSetDrawMode (int nview, int DrawMode) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetDrawMode(DrawMode);
}

BOOL TurtleGetEraseFlag (int nview) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->m_eraseFlag;
}
void TurtleSetEraseFlag (int nview, BOOL value) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->m_eraseFlag = value;
}

LOGFONT* TurtleGetTextFont (int nview)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetTextFont();
}
BOOL TurtleSetTextFont (int nview, LOGFONT *font)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->SetTextFont(font);
}

void TurtleSetRefresh (int nview, BOOL noyes) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetRefresh(noyes);
}

void TurtleGetPixel (int nview, int x, int y, DWORD *pColor) 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	*pColor = (DWORD) turtle->GetPixel (x, y);
}

void TurtlePoint (int nview, int x, int y) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->Point(x, y);
}

void TurtleLine (int nview, int x1, int y1, int x2, int y2, int type) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->MoveTo(x1, y1);
	turtle->LineTo(x2, y2);
}

void TurtleArc (int nview, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->Arc (x1, y1, x2, y2, x3, y3, x4, y4);
}

void TurtleTextOut (int nview, int x, int y, char *s) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->TextOut(x, y, s);
}

void TurtleFloodFill (int nview, int x, int y) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->FloodFill(x, y);
}

void TurtleFillRect (int nview, int x1, int y1, int x2, int y2, DWORD  *pColor) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->FillRect(x1, y1, x2, y2, pColor);
}

int TurtleZoom (int nview, double factor, int xPos, int yPos, int xDim, int yDim)
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	return turtle->Zoom (factor, xPos, yPos, xDim, yDim);
}

void TurtleSetZoomFactor (int nview, double zoomFactor)
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->m_zoomFactor = zoomFactor;
}

double TurtleGetZoomFactor (int nview)
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	return turtle->m_zoomFactor;
}

int TurtleResize (int nview, int xPos, int yPos, int xDim, int yDim)
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	return turtle->Resize (xPos, yPos, xDim, yDim);
}

void TurtleMouseOn (int nview, node leftDown, node leftUp, node rightDown, node rightUp, node move)
{ 
	CTurtleView* turtle;
	DlgEntry entry;
	turtle = theApp.m_turtleViews[nview];
	turtle->SetMouseState(TRUE);
	if (entry = WinToEntry (Dialogs, (CWnd *)turtle)) {
		node setup = GetSetup (entry);
		_setitem (setup, 1, leftDown);
		_setitem (setup, 2, leftUp);
		_setitem (setup, 3, rightDown);
		_setitem (setup, 4, rightUp);
		_setitem (setup, 5, move);
		PutSetup(entry, setup);
	}
	ASSERT(entry);
}

void TurtleMouseOff (int nview)
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->SetMouseState(FALSE);
}

void TurtleGetMousePos (int nview, int *x, int *y) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	CPoint point = turtle->GetMousePos();
	*x = point.x;
	*y = point.y;
}

#ifdef OVs

// int TurtleBitLoad (int nview, node logoPathName, int mode) 
int TurtleTurtleLoad (int nview, node logoPathName, int mode) // 040123
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	int error = 0;
	if (turtle->m_isOVs) {
		char strImagePath [MAXCHARS];
		getnam (strImagePath, logoPathName);
		error = turtle->BitLoadFromFile (strImagePath, mode);
	}
	else {
		int dev;
		if ((dev = f_apri (logoPathName, _O_RDONLY | _O_RAW, _S_IREAD)) <= 0)
			error = 1;
		else {
//>040123
//			if (! turtle->BitLoad (_file [dev], mode))
			if (! turtle->TurtleLoad (_file [dev], mode))
//<
				error = 2;
			f_chiudi (dev); 
		}
	}
	return error;
}

#else // OVs

// int TurtleBitLoad (int nview, int handle, int mode) 
int TurtleTurtleLoad (int nview, int handle, int mode) // 040123
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	return turtle->BitLoad (handle, mode);
}

#endif // OVs

// int TurtleBitSave (int nview, int handle, int mode)
int TurtleTurtleSave (int nview, int handle, int mode) // 040123
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
//>040123
//	return turtle->BitSave (handle, mode);
	return turtle->TurtleSave (handle, mode);
//<040123
}


void TurtleBitCutCopy (int nview, int bitIndex, int is_cut, int x, int y, int width, int height) 
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->BitCutCopy (bitIndex, is_cut, x, y, width, height);
}

void TurtleBitPaste (int nview, int bitIndex, int bitMode, int x, int y) 
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->BitPaste (bitIndex, bitMode, x, y);
}

void TurtleBitFit (int nview, int bitIndex, int width, int height) 
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->BitFit (bitIndex, width, height);
}

void TurtleBitPrint (int nview, int x, int y, int width, int height)
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->BitmapPrint (x, y, x + width - 1, y - height + 1);
}

void TurtlePrint (int nview)
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->TurtlePrint ();
}

void TurtleClearPalette (int nview) 
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->ClearPalette ();
}

void TurtleSetActiveArea (int nview, double xLowArea, double yLowArea, double xHighArea, double yHighArea)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetActiveArea (xLowArea, yLowArea, xHighArea, yHighArea);
}

void TurtleGetActiveArea (int nview, double *xLowArea, double *yLowArea, double *xHighArea, double *yHighArea)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->GetActiveArea (xLowArea, yLowArea, xHighArea, yHighArea);
}

void TurtleCenter (int nview, int x, int y)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->Center (x, y);
}

void TurtleStatusRefresh (int nview)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->OnTurtlestatusRefresh();
}

void TurtleShowSelectArea (int nview)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->ShowSelectArea();
}

BOOL TurtleGetModify()
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->GetModify();
}

void TurtleSetModify(BOOL bModified)
{		   
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SetModify(bModified);
}

IMPLEMENT_DYNCREATE(CTurtleView, CScrollView)
//// IMPLEMENT_DYNCREATE(CTurtleView, MvcScrollView_T<CTurtleViewport>)

CTurtleView::CTurtleView()
{
	m_isSelectionOn = TRUE;	// e' consentita selezione interattiva (down-drag-up)
	m_isSelectArea = FALSE;	// non esiste una selezione
	m_isDrag = FALSE;		// non è in corso dragging (per selezionare)

#ifdef OVs
	m_isOVs = turtleMode;	// da variabile gloabale
	if (m_isOVs) {
		m_pCursor = NULL;	// graphic cursor (tarta)
		m_pLine = NULL;		// current line
		m_pPolygon = NULL;	// current polygon
		m_isPolyline = FALSE;
		m_isPolygon = FALSE;
		m_ogID = -1;
	}
#endif // OVs
}

CTurtleView::~CTurtleView()
{
	if (m_pFont) delete(m_pFont);
#ifdef OVs
	if (! m_isOVs)
#endif // OVs
		if (m_pCanvas) delete(m_pCanvas);
}

//// BEGIN_MESSAGE_MAP(CTurtleView, CScrollView)
BEGIN_MESSAGE_MAP(CTurtleView, MvcScrollView_T<CTurtleViewport>)
//{{AFX_MSG_MAP(CTurtleView)
ON_COMMAND(ID_BITMAP_PRINT, OnBitmapPrint)
ON_COMMAND(ID_BITMAP_ACTIVEAREA, OnBitmapActivearea)
ON_COMMAND(ID_BITMAP_LOAD, OnBitmapLoad)
ON_COMMAND(ID_BITMAP_NEW, OnBitmapNew)
ON_COMMAND(ID_BITMAP_SAVE, OnBitmapSave)
ON_COMMAND(ID_BITMAP_SAVEAS, OnBitmapSaveas)
ON_COMMAND(ID_SET_FLOODCOLOR, OnSetFloodcolor)
ON_COMMAND(ID_SET_FONT, OnSetFont)
ON_COMMAND(ID_SET_PENCOLOR, OnSetPencolor)
ON_COMMAND(ID_SET_SCREENCOLOR, OnSetScreencolor)
ON_COMMAND(ID_ZOOM_IN, OnZoomIn)
ON_COMMAND(ID_ZOOM_NORMAL, OnZoomNormal)
ON_COMMAND(ID_ZOOM_OUT, OnZoomOut)
ON_COMMAND(ID_TURTLESTATUS_SHOW, OnTurtlestatusShow)
ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
ON_COMMAND(ID_PRINT_SETUP, OnPrintSetup)
ON_COMMAND(ID_VIEW_CENTER, OnViewCenter)
ON_COMMAND(ID_SET_PENMINUS, OnSetPenminus)
ON_COMMAND(ID_SET_PENPLUS, OnSetPenplus)
	ON_COMMAND(ID_SELECT_TURTLE, OnSelectTurtle)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_COPIA, OnEditCopy)
	ON_COMMAND(ID_EDIT_TAGLIA, OnEditCut)
	ON_COMMAND(ID_EDIT_BUTTA, OnEditDelete)
	ON_COMMAND(ID_EDIT_INCOLLA, OnEditPaste)
	ON_COMMAND(ID_SET_PALETTE, OnSetPalette)
	ON_COMMAND(ID_EDIT_RIEMPI, OnEditFill)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_COMMAND(ID_EDIT_SELECTAREA, OnEditSelectarea)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINTSETUP, OnFilePrintsetup)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BUTTA, OnUpdateEditButta)
	ON_UPDATE_COMMAND_UI(ID_EDIT_TAGLIA, OnUpdateEditTaglia)
	ON_UPDATE_COMMAND_UI(ID_EDIT_RIEMPI, OnUpdateEditRiempi)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPIA, OnUpdateEditCopia)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(101, 1000, OnMenuButton)
	ON_UPDATE_COMMAND_UI_RANGE(101, 1000, OnUpdateMenuButton)
END_MESSAGE_MAP()

void CTurtleView::OnUpdateMenuButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CTurtleView::OnMenuButton(UINT uID)
{
	node sons = GetSons(m_dlgEntry);
	DlgEntry menuEntry;
	UINT id;
	node setup;
	while (sons) {
		menuEntry = car (sons);
		ASSERT(menuEntry);
		sons = cdr (sons);
		id = GetId(menuEntry);
		if (id == uID) {
			setup = GetSetup (menuEntry);
			if (setup != NULLP) {
				NestedExec (setup);
			}
			return;
		}
	}
	TRACE("invalid menu Id");
}

/////////////////////////////////////////////////////////////////////////////
// CTurtleView drawing

void QueryDisplayCaps (CDC* pDC)
{
	theApp.driverversion = pDC->GetDeviceCaps(DRIVERVERSION);
	theApp.technology = pDC->GetDeviceCaps(TECHNOLOGY);

	theApp.rastercaps = pDC->GetDeviceCaps(RASTERCAPS);
	if (theApp.rastercaps & RC_PALETTE) {
		theApp.sizepalette = pDC->GetDeviceCaps(SIZEPALETTE);
		theApp.numreserved = pDC->GetDeviceCaps(NUMRESERVED);
		theApp.colorres = pDC->GetDeviceCaps(COLORRES);
	}

	theApp.curvecaps = pDC->GetDeviceCaps(CURVECAPS);
	theApp.linecaps = pDC->GetDeviceCaps(LINECAPS);
	theApp.polygonalcaps = pDC->GetDeviceCaps(POLYGONALCAPS);
	theApp.textcaps = pDC->GetDeviceCaps(TEXTCAPS);
	theApp.clipcaps = pDC->GetDeviceCaps(CLIPCAPS);

	theApp.horzsize = pDC->GetDeviceCaps(HORZSIZE);
	theApp.vertsize = pDC->GetDeviceCaps(VERTSIZE);
	theApp.horzres = pDC->GetDeviceCaps(HORZRES);
	theApp.vertres = pDC->GetDeviceCaps(VERTRES);
	theApp.logpixelsx = pDC->GetDeviceCaps(LOGPIXELSX);
	theApp.logpixelsy = pDC->GetDeviceCaps(LOGPIXELSY);
	theApp.bitspixel = pDC->GetDeviceCaps(BITSPIXEL);
	theApp.planes = pDC->GetDeviceCaps(PLANES);
	theApp.numbrushes = pDC->GetDeviceCaps(NUMBRUSHES);
	theApp.numpens = pDC->GetDeviceCaps(NUMPENS);
	theApp.numfonts = pDC->GetDeviceCaps(NUMFONTS);
	theApp.numcolors = pDC->GetDeviceCaps(NUMCOLORS);
	theApp.aspectx = pDC->GetDeviceCaps(ASPECTX);
	theApp.aspecty = pDC->GetDeviceCaps(ASPECTY);
	theApp.aspectxy = pDC->GetDeviceCaps(ASPECTXY);
	theApp.pdevicesize = pDC->GetDeviceCaps(PDEVICESIZE);
}

BOOL CTurtleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style |= WS_CLIPCHILDREN;

	return MvcScrollView_T<CTurtleViewport>::PreCreateWindow(cs);
}  

void CTurtleView::OnInitialUpdate()
{
	// TODO: calculate the total size of this view
	// Assegno come dimensioni la client area del padre 							   
	CSize frameSizeTotal, viewSizeTotal;
	RECT frameClientArea, viewClientArea;
	GetParent()->GetClientRect(&frameClientArea);
	frameSizeTotal.cx = frameClientArea.right - frameClientArea.left;
	frameSizeTotal.cy = frameClientArea.bottom - frameClientArea.top;
	BOOL isScroll = (globalPixelX != -1) && (globalPixelY != -1);
	if (globalPixelX == -1) globalPixelX = frameSizeTotal.cx;
	if (globalPixelY == -1) globalPixelY = frameSizeTotal.cy;
	CSize virtualSize;
	virtualSize.cx = globalPixelX;
	virtualSize.cy = globalPixelY;

#ifdef OVs
	if (m_isOVs) {
		CTurtleDoc* pDoc = GetDocument();
		m_pModel = pDoc->GetModel();
		MvcScrollView_T<CTurtleViewport>::OnInitialUpdate();
	}
#endif // OVs

#ifdef OVs
	if (! m_isOVs) {
#endif // OVs

		if (isScroll)
			SetScrollSizes(MM_TEXT, virtualSize);
#ifdef OVs
	}
#endif // OVs
	
	GetClientRect(&viewClientArea); 	// nuovo
	viewSizeTotal.cx = viewClientArea.right - viewClientArea.left;
	viewSizeTotal.cy = viewClientArea.bottom - viewClientArea.top;	

	m_dosPalette = 0;
	m_dosPenColor = -1;
	m_dosScreenColor = -1;
	m_dosFloodColor = -1;
	m_dosTextBkColor = -1;
	m_screenColor = RGB(255,255,255);
	m_floodColor = RGB(255,0,0);
	m_penColor = RGB(0,0,0);
	m_textBkColor = RGB(0,0,0);
	m_textBkMode = TRANSPARENT;
	m_penStyle = PS_SOLID;
	m_penWidth = 1;
	m_drawMode = R2_COPYPEN;
	m_bPenTransparent = FALSE;
	m_bFloodTransparent = FALSE;
	m_eraseFlag = FALSE;	// 980911
	m_xDim = viewSizeTotal.cx;
	m_yDim = viewSizeTotal.cy;
	m_mousePos.x = 0;
	m_mousePos.y = 0;
	m_mouseState = FALSE;
	m_keybState = FALSE;
	m_key = 0;
	m_refresh = TRUE;
	SetModify (FALSE);		// 980903
	m_hCursor = 0;			// 031030

#ifdef NEW_FONT
	// Default font ..
	::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	// .. possibly to be replaced from custom font
	int n =
		GetPrivateProfileString(turtle_name, "Font", "",
					TurtleFont, MAX_CHARS, ini_name);
	if (n > 0) {
		node FontParList = _parse (TurtleFont);
		ListToFont (FontParList, &m_logFont);
	}
	// Create CFont object
	m_pLogFont = &m_logFont;
	m_pFont = new(CFont);
	m_pFont->CreateFontIndirect(&m_logFont);
#else // NEW_FONT
	::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	m_pLogFont = &m_logFont;
	m_pFont = new(CFont);
	m_pFont->CreateFontIndirect(&m_logFont);
#endif // NEW_FONT

	// Get DC for View client area and its capabilities	
	CDC *pDC = GetDC();
	ASSERT(pDC);
	int BitsPerPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int NumColors = 1 << BitsPerPixel;
	int ColortableColors = pDC->GetDeviceCaps(NUMCOLORS);
	int rasterCaps = pDC->GetDeviceCaps(RASTERCAPS);
	int curveCaps = pDC->GetDeviceCaps(CURVECAPS);
	int lineCaps = pDC->GetDeviceCaps(LINECAPS);
	int polygonalCaps = pDC->GetDeviceCaps(POLYGONALCAPS);
	int textCaps = pDC->GetDeviceCaps(TEXTCAPS);

	QueryDisplayCaps (pDC);

	// inizializzaqzione palette in oggetto Iperlogo (theApp)
	if (theApp.rastercaps & RC_PALETTE) {	// 980407: agg. test
		if (theApp.m_pPalette == NULL)
			ClearPalette ();
	}		

#ifdef OVs
	if (! m_isOVs) {
#endif // OVs
		// We draw only on the canvas bitmap  

		// Create canvas bitmap for long-term store of image	
		m_pCanvas = new(CBitmap);
		int result =
			m_pCanvas->CreateCompatibleBitmap(pDC, virtualSize.cx, virtualSize.cy);
		ASSERT(result);
		// Create DC for canvas bitmap
		CDC canvasDC;
		CBitmap* pOldBit;
		canvasDC.CreateCompatibleDC(pDC);
		pOldBit = canvasDC.SelectObject(m_pCanvas);
		CBrush newBrush;
		newBrush.CreateSolidBrush(m_screenColor);
		/// canvasDC.FillRect(CRect(0, 0, m_xDim, m_yDim), &newBrush);
		canvasDC.FillRect(CRect(0, 0, virtualSize.cx, virtualSize.cy), &newBrush);
		canvasDC.SelectObject(pOldBit);
		canvasDC.DeleteDC();
	
		m_nDC = pDC->SaveDC();
		ReleaseDC (pDC);
#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnSetFocus(CWnd* pOldWnd) 
{
#ifdef OVs
	if (m_isOVs) {
		MvcScrollView_T<CTurtleViewport>::OnSetFocus(pOldWnd);
	}
	else {
#endif // OVs
		CScrollView::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here

		if (theApp.m_pPalette) {
			CDC* pDC = GetDC();
			pDC->SelectPalette(theApp.m_pPalette, FALSE);	   
			pDC->RealizePalette();
		}
#ifdef OVs
	}
#endif // OVs
}

#ifdef OVs

BOOL CTurtleView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	

	if (m_isOVs) {
		// Don't erase the background!
		pDC;	// unused
		return TRUE;
	}
	else {
//		return CScrollView::OnEraseBkgnd(pDC);
		CBrush br (GetSysColor (COLOR_APPWORKSPACE)); 
		FillOutsideRect( pDC, &br );
		return TRUE;
	}
}

BOOL CTurtleView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	BOOL bHandled = FALSE;
	if (m_isOVs) {
		bHandled = MvcScrollView_T<CTurtleViewport>::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	else {
		bHandled = CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
#if 0
	// first pump through the normal channels. This allow you to override
	// the components default handling inside the view class.
	bHandled = CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	// then pump through the viewport
	if ((! bHandled) && m_isOVs) {
////	if (m_pDocument != NULL)
///		bHandled = m_vpDiagram.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		bHandled = CTurtleViewport::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
#endif // 0
	return bHandled;
}

BOOL CTurtleView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bHandled = FALSE;
	// Give the controller a chance to manage the message (v. Simple sample)
	if (m_isOVs) {
		if (m_pCtlr)
			bHandled = ((CTurtleController*) m_pCtlr)->OnWndMsg(message, wParam, lParam, pResult);
		if (! bHandled)
			bHandled = MvcScrollView_T<CTurtleViewport>::OnWndMsg(message, wParam, lParam, pResult);
	}
	else
		bHandled = CScrollView::OnWndMsg(message, wParam, lParam, pResult);
	return bHandled;
}

void CTurtleView::OnFileNew() 
{
	// TODO: Add your command handler code here
	
	if (m_isOVs) 
	{
		ClearScreen (TRUE);
	}
}

// da OnBitmapLoad - da riutilizzare anche per OnBitmapLoad
BOOL CTurtleView::FileLoad(BOOL bOVsMode, CString strFileName)
{
	CString PathName = "";

	if (strFileName.GetLength() != 0) {
		PathName = strFileName;
	}
	else {
		static char BASED_CODE lpszFilter[] =
			"File Bitmap (*.bmp)|*.bmp|Tutti i file (*.*)|*.*||";
		LPCSTR lpszFileName = NULL;
		LPCSTR lpszDefExt = NULL;
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		CWnd* pParentWnd = m_frame;
		BOOL bOpenFileDialog = TRUE;
		CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	
		int dialogResult = FileDialog.DoModal();
		if (dialogResult == IDOK)
			PathName = FileDialog.GetPathName();
		else
			return FALSE;
	}

	BOOL loadResult = TRUE;
	if (bOVsMode)
		loadResult = GetDocument()->OnOpenDocument(PathName);
	else {
		int handle = _open (PathName, _O_RDONLY | _O_RAW, _S_IWRITE | _S_IREAD);
		if (handle == 0)
			loadResult = FALSE;
		else {
//			_BITLOAD (m_nView, handle, 1);
			_TURTLELOAD (m_nView, handle, 1); // 040123
			_close (handle);
		}
	}

	if (loadResult) {
		strcpy (m_saveFile, PathName);
		SetModify (FALSE);
	}
	return loadResult;
}

// da OnBitmapSaveAs - da riutilizzare anche per OnbitmapSave/SaveAs
BOOL CTurtleView::FileSave(BOOL bOVsMode, BOOL bSaveAs, CString strFileName)
{
	CString PathName = "";

	if (strFileName.GetLength() != 0) {
		PathName = strFileName;
	}
	else if (bSaveAs || (! m_saveFile[0])) {
		static char BASED_CODE lpszFilter[] =
			"File Bitmap (*.bmp)|*.bmp|Tutti i file (*.*)|*.*||";
		LPCSTR lpszFileName = NULL;
		LPCSTR lpszDefExt= "bmp";		// corretto
		if (m_saveFile[0] != 0) 		// nuovo
			lpszFileName = m_saveFile;
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		CWnd* pParentWnd = m_frame;
		BOOL bOpenFileDialog = FALSE;
		CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	
		int dialogResult = FileDialog.DoModal();
		if (dialogResult == IDOK)
			PathName = FileDialog.GetPathName();
		else
			return FALSE;
	}
	else
		PathName = m_saveFile;

	BOOL saveResult = TRUE;
	if (bOVsMode)
		saveResult = GetDocument()->OnSaveDocument(PathName);
	else {
		int handle = _open (PathName, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD);
		if (handle == 0)
			saveResult = FALSE;
		else {
//			_BITSAVE (m_nView, handle, 0);
			_TURTLESAVE (m_nView, handle, 0); // 040123
			_close (handle);
		}
	}

	if (saveResult) {
		strcpy (m_saveFile, PathName);
		SetModify (FALSE);
	}
	return saveResult;
}

#ifdef ELIMINATO	// spostato in turtleCtrl
void CTurtleView::OnFileOpen()
{
	// TODO: Add your command handler code here

	FileLoad(TRUE, "");
}

void CTurtleView::OnFileSave()
{
	// TODO: Add your command handler code here
	
	FileSave(TRUE, FALSE, "");
}

void CTurtleView::OnFileSaveAs()
{
	// TODO: Add your command handler code here
	
	FileSave(TRUE, TRUE, "");
}
#endif // ELIMINATO

/////////////////////////////////////////////////////////////////////////////
// CShowcaseView printing

BOOL CTurtleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Prepare the CPrintInfo object to print the diagram.
	if (!GetViewport()->PreparePrinting(pInfo))
		return FALSE;
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTurtleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTurtleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTurtleView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add specialized OnPrint() handling

	// Print the diagram
	GetViewport()->Print(pDC, pInfo);
}

#endif // OVs

/////////////////////////////////////////////////////////////////////////////
// CTurtleView diagnostics

#ifdef _DEBUG
void CTurtleView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTurtleView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTurtleDoc* CTurtleView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTurtleDoc)));
	return (CTurtleDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////

int CTurtleView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
#ifdef OVs
	if (m_isOVs) {
		CTurtleDoc* pDoc = GetDocument();
		CTurtleModel* pModel = pDoc->GetModel();
		CTurtleViewport::SetModel(pModel);
	}
#endif // OVs
	return 0;
}

void CTurtleView::OnSize(UINT nType, int cx, int cy) 
{
#ifdef OVs
	if (m_isOVs) {
#endif // OVs
		MvcScrollView_T<CTurtleViewport>::OnSize(nType, cx, cy);
#ifdef OVs
	}
	else
#endif // OVs
		CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void CTurtleView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
#ifdef OVs
	if (m_isOVs) {
#endif // OVs
		MvcScrollView_T<CTurtleViewport>::OnPrepareDC(pDC, pInfo);
#ifdef OVs
	}
	else
		CScrollView::OnPrepareDC(pDC, pInfo);
#endif // OVs
}

void CTurtleView::OnDraw(CDC* pDC)
{
	//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

#ifdef OVs
	if (m_isOVs) {
		CTurtleDoc* pDoc = (CTurtleDoc*) GetDocument();
		ASSERT_VALID(pDoc);

		// Update the window with the contents of the viewport buffer.
		MvcScrollView_T<CTurtleViewport>::OnDraw(pDC);
	}
	else {
#endif // OVs

	// Redraw from the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	BOOL result;
	if (m_refresh) {
		if (theApp.m_pPalette) {
			pDC->SelectPalette(theApp.m_pPalette, FALSE);	   
			pDC->RealizePalette();
		}
		//	Create memory DC for canvas bitmap
		result = canvasDC.CreateCompatibleDC(NULL);
		pOldBit = canvasDC.SelectObject(m_pCanvas);
		result = canvasDC.RestoreDC(m_nDC);
		//	Copy it to the device DC
#ifdef NEW_ZOOM
		result =
			pDC->StretchBlt(0, 0, m_xScroll, m_yScroll, &canvasDC, 0, 0, m_xPixel, m_yPixel, SRCCOPY);
#endif // NEW_ZOOM
		VERIFY(result);
		canvasDC.SelectObject(pOldBit);
		canvasDC.DeleteDC();
	}
#ifdef OVs
	}
#endif // OVs
}


/////////////////////////////////////////////////////////////////////////////
// CTurtleView message handlers

CPoint CTurtleView::ViewToCanvas (CPoint point)
{

	CPoint scrollPos = GetScrollPosition();
	point.Offset(scrollPos);

#ifdef NEW_ZOOM
	point.x = (int) (point.x / m_zoomFactor);
	point.y = (int) (point.y / m_zoomFactor);
#endif // NEW_ZOOM

	return point;
}

void CTurtleView::ShowSelectArea ()
{
	if(! m_isSelectArea)
		return;
	CPoint TopLeft = m_rectSelectArea.TopLeft();
	CPoint BottomRight = m_rectSelectArea.BottomRight();
	int xLeft = TopLeft.x;
	int yTop = TopLeft.y;
	int xRight = BottomRight.x;
	int yBottom = BottomRight.y;

	// save and set drawing parameters
	int PenWidth = GetPenWidth ();
	int DrawMode = GetDrawMode ();
	SetPenWidth (1);
	SetDrawMode (R2_NOT);			// R2_XORPEN R2_COPYPEN
	
	// draws rectangle border with 4 lines
	MoveTo(xLeft, yBottom);
	LineTo(xRight, yBottom);
	MoveTo(xRight, yBottom);
	LineTo(xRight, yTop);
	MoveTo(xRight, yTop);
	LineTo(xLeft, yTop);
	MoveTo(xLeft, yTop);
	LineTo(xLeft, yBottom);
	
	// restaures current drawing parameters
	SetPenWidth (PenWidth);
	SetDrawMode (DrawMode);
}

void CTurtleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

#ifdef OVs
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnLButtonDown(nFlags, point);
	else {
#endif // OVs

	m_mousePos = ViewToCanvas (point);
	m_buttonDownPos = m_mousePos;
	DlgEntry entry;
	node callBack;
	if (m_mouseState) {
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (1, callBack));
			NestedExec(callBack);
		}
		ASSERT(entry);
	}
	else if (m_isSelectArea) {
		ShowSelectArea ();
		m_isSelectArea = FALSE;
		TurtleMenuItemEnable (m_nView, ID_EDIT_TAGLIA, FALSE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_COPIA, FALSE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_BUTTA, FALSE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_RIEMPI, FALSE);
	}
	else if (m_isSelectionOn) {
		m_isDrag = TRUE;	// start cursor dragging
		// clip cursor into client area
		RECT ClientArea;
		m_frame->GetClientRect(&ClientArea);
		ClientToScreen(&ClientArea);
		ClipCursor(&ClientArea);
		DragDetect(m_hWnd, point);
	}

#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

#ifdef OVs
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnLButtonUp(nFlags, point);
	else {
#endif // OVs

	m_mousePos = ViewToCanvas (point);
	m_buttonUpPos = m_mousePos;
	DlgEntry entry;
	node callBack;
	if (m_mouseState) {
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (2, callBack));
			NestedExec(callBack);
		}
		ASSERT(entry);
	}	
	else if (m_isDrag) {
		TurtleMenuItemEnable (m_nView, ID_EDIT_TAGLIA, TRUE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_COPIA, TRUE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_BUTTA, TRUE);
		TurtleMenuItemEnable (m_nView, ID_EDIT_RIEMPI, TRUE);
		m_isDrag = FALSE;	// end cursor dragging
		ClipCursor(NULL);	// free cursor movement
	}

#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

#ifdef OVs
	if (m_isOVs) {
//		MvcScrollView_T<CTurtleViewport>::OnRButtonDown(nFlags, point);
	}
	else {
#endif // OVs

	m_mousePos = ViewToCanvas (point);
	DlgEntry entry;
	node callBack;
	if (m_mouseState) {
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (3, callBack));
			NestedExec(callBack);
		}
		ASSERT(entry);
	}	

#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

#ifdef OVs
	if (m_isOVs) {
//		MvcScrollView_T<CTurtleViewport>::OnRButtonUp(nFlags, point);
	}
	else {
#endif // OVs

	m_mousePos = ViewToCanvas (point);
	DlgEntry entry;
	node callBack;
	if (m_mouseState) {
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (4, callBack));
			NestedExec(callBack);
		}
		ASSERT(entry);
	}	

#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

#ifdef OVs
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnMouseMove(nFlags, point);
	else {
#endif // OVs

	m_mousePos = ViewToCanvas (point);
	DlgEntry entry;
	node callBack;
	if (m_mouseState) {
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (5, callBack));
			NestedExec(callBack);
		}
		ASSERT(entry);
	}	
	else {
		// show cursor movement
		if (m_isDrag) {
			if (m_isSelectArea)	// SI: lasciare test !!!
				ShowSelectArea ();
			m_rectSelectArea = CRect (m_buttonDownPos, m_mousePos);
			m_rectSelectArea.NormalizeRect();
			m_isSelectArea = TRUE;
			ShowSelectArea ();
		}
	}

#ifdef OVs
	}
#endif // OVs
}

int CTurtleView::GetPenStyle()
{
	return m_penStyle;
}
void CTurtleView::SetPenStyle(int style)
{
	m_penStyle = style;
}

int CTurtleView::GetPenWidth()
{
	return m_penWidth;
}
void CTurtleView::SetPenWidth(int width)
{
	m_penWidth = width;
}

COLORREF CTurtleView::GetPenColor()
{
	return m_penColor;
}
void CTurtleView::SetPenColor(COLORREF color)
{
	CDC *pDC = GetDC();
	ColorInPalette (pDC, color, &m_penColor, /*tolerance*/ 0);
	ReleaseDC (pDC);
}

int CTurtleView::GetDosPalette()
{
	return m_dosPalette;
}
void CTurtleView::SetDosPalette(int iPal)
{
	m_dosPalette = iPal;
}

int CTurtleView::GetDosPenColor()
{
	return m_dosPenColor;
}
void CTurtleView::SetDosPenColor(int iCol)
{
	m_dosPenColor = iCol;
	if (iCol >= 0) {
		COLORREF color;
		if (m_dosPalette == 2) {
			iCol = iCol % 16;
			color = dosColors[iCol+16];
		}
		else
		if (iCol == 0)
			color = GetBkColor();
		else
			color = dosColors[dosPalettes [m_dosPalette][iCol]];
		SetPenColor(color);
	}
}

int CTurtleView::GetDosBkColor()
{
	return m_dosScreenColor;
}
void CTurtleView::SetDosBkColor(int iCol)
{
	m_dosScreenColor = iCol;
	if (iCol >= 0) {
		COLORREF color;
		color = dosColors[iCol];
		if (m_dosPalette == 2) {
			iCol = iCol % 16;
			color = dosColors[iCol+16];
		}
		SetBkColor(color);
	}
}

int CTurtleView::GetDosFloodColor()
{
	return m_dosFloodColor;
}
void CTurtleView::SetDosFloodColor(int iCol)
{
	m_dosFloodColor = iCol;
	if (iCol >= 0) {
		COLORREF color;
		color = dosColors[iCol];
		if (m_dosPalette == 2) {
			iCol = iCol % 16;
			color = dosColors[iCol+16];
		}
		SetFloodColor(color);
	}
}

int CTurtleView::GetTextBkMode()
{
	return m_textBkMode;
}
void CTurtleView::SetTextBkMode(int mode)
{
	m_textBkMode = mode;
}

COLORREF CTurtleView::GetTextBkColor()
{
	return m_textBkColor;
}
void CTurtleView::SetTextBkColor(COLORREF color)
{
	CDC *pDC = GetDC();
	ColorInPalette (pDC, color, &m_textBkColor, /*tolerance*/ 0);
	ReleaseDC (pDC);
}

COLORREF CTurtleView::GetBkColor()
{
	return m_screenColor;
}
void CTurtleView::SetBkColor(COLORREF color)
{
#ifdef OVs
	if (m_isOVs) {
		m_screenColor = color;
		m_pModel->SetBkgndColor(color);
	}
	else  {
#endif OVs
		CDC *pDC = GetDC();
		pDC->RestoreDC(m_nDC);

		WORD wPaletteIndex =
			ColorInPalette (pDC, color, &m_screenColor, /*tolerance*/ 0);

		pDC->SetBkColor(ColorOrIndex(m_screenColor));

		m_nDC = pDC->SaveDC();
		ReleaseDC (pDC);
#ifdef OVs
	}
#endif OVs
}

COLORREF CTurtleView::GetFloodColor()
{
	return m_floodColor;
}
void CTurtleView::SetFloodColor(COLORREF color)
{
	CDC *pDC = GetDC();
	ColorInPalette (pDC, color, &m_floodColor, /*tolerance*/ 0);
	ReleaseDC (pDC);
}

int CTurtleView::GetDrawMode()
{
	return m_drawMode;
}
void CTurtleView::SetDrawMode(int DrawMode)
{
	m_drawMode = DrawMode;
}

LOGFONT *CTurtleView::GetTextFont()
{
	if (! m_pLogFont) {
		//		inizializzare esplicitamente turtle con font default ?
		return NULL;
	}
	return m_pLogFont;
}
BOOL CTurtleView::SetTextFont(LOGFONT *lpLogFont)
{
	if (m_pFont) delete(m_pFont);
	m_pLogFont = lpLogFont;
	m_pFont = new(CFont);
	return m_pFont->CreateFontIndirect(lpLogFont);
}

void CTurtleView::SetRefresh(BOOL noyes)
{
	m_refresh = noyes;
	if (noyes)
#ifdef OVs
		if (m_isOVs)
			UpdateAll();
		else
#endif // OVs

//			InvalidateRect(NULL);		// 990311
			MapClipInvalidateRect (		// 990317
				yLogoToView (m_yHighArea),	//	int top
				xLogoToView (m_xLowArea),	//	int left
				yLogoToView (m_yLowArea),	//	int bottom
				xLogoToView (m_xHighArea));	//	int right
}

COLORREF CTurtleView::GetPixel(int x, int y)
{
	CDC* pDC = GetDC();
	
	CDC canvasDC;
	CBitmap* pOldBit;
	int result = canvasDC.CreateCompatibleDC(pDC);
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	COLORREF crColor = canvasDC.GetPixel(x, y);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
	
	ReleaseDC (pDC);
	return crColor;
}

void CTurtleView::MapClipInvalidateRect (int top, int left, int bottom, int right)
{
// take into account Zoom
	left = (int) (left * m_zoomFactor);
	top = (int) (top * m_zoomFactor);
	right = (int) (right * m_zoomFactor);
	bottom = (int) (bottom * m_zoomFactor);
// map coordinates to view and clip them inside the client area
	CPoint scrollPos = GetScrollPosition();
	RECT clientArea;
	m_frame->GetClientRect(&clientArea);
	left = max(left - scrollPos.x, clientArea.left); 
	top = max(top - scrollPos.y, clientArea.top); 
	right = min(right - scrollPos.x, clientArea.right); 
	bottom = min(bottom - scrollPos.y, clientArea.bottom); 
// build rectangle and invalidate it
	CRect rect = CRect (left, top, right, bottom);
	CScrollView::InvalidateRect (&rect);
}

int CTurtleView::SetPixel(int x, int y, COLORREF color)
{
	int result;
	color = ColorOrIndex(color);
	CDC* pDC = GetDC();

#ifdef SAVE
	// We also draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	result = canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980313	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	result = canvasDC.RestoreDC(m_nDC);
	result = canvasDC.SetPixelV(x, y, color);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE
	
	ReleaseDC (pDC);

#ifdef DRAW_INDIRECT
#ifdef NEW_ZOOM
	int top = y - 1;
	int left = x - 1;
	int bottom = y + 1;
	int right = x + 1;
// map to view rect and normalize the client coordinates
	MapClipInvalidateRect (top, left, bottom, right);
#endif // NEW_ZOOM
#endif // DRAW_INDIRECT
	
	return 0;
}

int CTurtleView::Point(int x, int y)
{
	CPoint scrollPos = GetScrollPosition();
	int result;
	CPoint oldPoint;
	CPen newPen;
	CPen* oldPen;
	result = newPen.CreatePen(m_penStyle, m_penWidth,
							ColorOrIndex(writeColor));
	CDC* pDC = GetDC();

#ifdef SAVE
	// We also draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	result = canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980313	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	result = canvasDC.RestoreDC(m_nDC);
	oldPen = canvasDC.SelectObject(&newPen);
	result = canvasDC.SetROP2(m_drawMode);
	oldPoint = canvasDC.MoveTo(x, y);
	result = canvasDC.LineTo(x+1, y+1);
	canvasDC.SelectObject(oldPen);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE
	
	ReleaseDC (pDC);

#ifdef DRAW_INDIRECT
#ifdef NEW_ZOOM
	int top = y - m_penWidth;
	int left = x - m_penWidth;
	int bottom = y + m_penWidth;
	int right = x + m_penWidth;
// map to view rect and normalize the client coordinates
	MapClipInvalidateRect (top, left, bottom, right);
#endif // NEW_ZOOM
#endif // DRAW_INDIRECT
	
	return 0;
}

#ifdef OVs

void TurtleGoHome (int nview)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	turtleView->GoHome();
}

void CTurtleView::GoHome()
{
	m_logoView->pos_x_t = 0.0;
	m_logoView->pos_y_t = 0.0;
	m_xPos = m_logoView->xOrg;
	m_yPos = m_logoView->yOrg;
	asdir_ (BCD_0);		// rivedere
}

void TurtleClearScreen (int nview, BOOL home)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	turtleView->ClearScreen (home);
}

void CTurtleView::ClearScreen (BOOL home)
{
	if (m_isOVs) {
//		m_pModel->Initialize();
		m_pModel->Clear();
	}
	else {
		DWORD bk_colors;
		bk_colors = GetBkColor ();
		FillRect (m_logoView->min_x, m_logoView->min_y, m_logoView->max_x-1, m_logoView->max_y-1, &bk_colors);
		SetModify(TRUE);
	}
	if (home)
		GoHome();
	if (m_isOVs) {
		CreateCursor ();
//		MoveCursor (m_xPos, m_yPos, m_logoView->alfa);
	}
}

CODLineComponent* TurtleCloseLine (int nview)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	return turtleView->CloseLine();
}

CODLineComponent* CTurtleView::CloseLine()
{
	CODLineComponent* pLine = NULL;
	if (m_pLine) {
		pLine = m_pLine;
		m_pLine = NULL;
		m_isPolyline = FALSE;
	}
	return pLine;
}

void TurtleCreateCursor (int nview)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	turtleView->CreateCursor ();
}

#define CURSOR_CENTERED
// #define CURSOR_CENTERED_OFFSET
// #define CURSOR_ALIGNED

void CTurtleView::CreateCursor ()
{
	if (! m_isOVs)
		return;
	CODPointArray codPoints;
	codPoints.SetSize(4);
#ifdef CURSOR_CENTERED
	codPoints.SetAt(0, CPoint (0, CursorSide/4));             
	codPoints.SetAt(1, CPoint (- CursorSide/2, CursorSide/4));             
	codPoints.SetAt(2, CPoint (0, - CursorSide/4));             
	codPoints.SetAt(3, CPoint (CursorSide/2, CursorSide/4));             
#endif // CURSOR_CENTERED
#ifdef CURSOR_CENTERED_OFFSET
	codPoints.SetAt(0, CPoint (m_xPos, m_yPos + CursorSide/4));             
	codPoints.SetAt(1, CPoint (m_xPos - CursorSide/2, m_yPos + CursorSide/4));             
	codPoints.SetAt(2, CPoint (m_xPos, m_yPos - CursorSide/4));             
	codPoints.SetAt(3, CPoint (m_xPos + CursorSide/2, m_yPos + CursorSide/4));             
#endif // CURSOR_CENTERED_OFFSET
#ifdef CURSOR_ALIGNED
	codPoints.SetAt(0, CPoint (CursorSide/2, CursorSide/2));             
	codPoints.SetAt(1, CPoint (0, CursorSide/2));             
	codPoints.SetAt(2, CPoint (CursorSide/2, 0));             
	codPoints.SetAt(3, CPoint (CursorSide, CursorSide/2));             
#endif // CURSOR_ALIGNED
	m_pCursor = new(CODPolygonComponent);
	BOOL cursorCreated = ((CODPolygonComponent*) m_pCursor)->Create(&codPoints);
	BOOL cursorAppended = m_pModel->AppendChild(m_pCursor);
	m_pCursor->SetValue(OD_PROP_EDIT + OD_EDIT_READONLY, TRUE);
	m_pCursor->SetValue(OD_PROP_EDIT + OD_EDIT_CANSELECT, FALSE);
	MoveCursor (m_xPos, m_yPos, m_logoView->alfa);
}

void TurtleShowCursor (int nview, BOOL show)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	turtleView->ShowCursor (show);
}

void CTurtleView::ShowCursor (BOOL show)
{
	if (m_pCursor) {
		m_pCursor->SetVisible(show);
		if (show)
			MoveCursor (m_xPos, m_yPos, m_logoView->alfa);
		else {
			CTurtleModel* pModel = (CTurtleModel*) GetModel();
			pModel->UpdateComponent(m_pCursor, OD_UPDATE_CURRENT_POS);
		}
	}
}

void TurtleMoveCursor (int nview, int x, int y, double alfa)
{
	CTurtleView* turtleView = theApp.m_turtleViews[nview];
	turtleView->MoveCursor (x, y, alfa);
}

void CTurtleView::MoveCursor (int x, int y, double alfa)
{
	if (! m_pCursor->IsVisible())
		return;

#ifdef CURSOR_ALIGNED
	x -= CursorSide / 2;
	y -= CursorSide / 4;
#endif // CURSOR_ALIGNED
	int nOffsetX, nOffsetY;
	float fScaleX, fScaleY;
	int nRotation;
	if (m_pCursor) {
		CTurtleModel* pModel = (CTurtleModel*) CTurtleViewport::GetModel();
#ifdef CURSOR_CENTERED
		m_pCursor->CenterOnPoint(CPoint (x, y));
//		if (m_pCursor->IsVisible())
			pModel->UpdateComponent(m_pCursor, OD_UPDATE_OLD_AND_NEW_POS);
#endif // CURSOR_CENTERED
		CODTransform* pXform = m_pCursor->GetTransform();
		if (! pXform)
			pXform = new(CODTransform);
		pXform->GetTranslation(nOffsetX, nOffsetY);
#ifdef NON_USARE // ha un side-effect sull'aggiornamento dello schermo
///		pXform->SetTranslation(x, y);
///		m_pCursor->SetTransform(pXform);
#endif // NON_USARE
		nRotation = 0;
		pXform->GetScaling(fScaleX, fScaleY);
		BOOL isFlipped = (fScaleX < 0.0) || (fScaleY < 0.0);
		if (pXform->IsRotated()) {
			nRotation = pXform->GetRotation();
			if (isFlipped)
				nRotation += 1800;
		}
		else if (isFlipped)
			nRotation = 1800;
		int neededRotation = ((int) alfa * 10) - nRotation;
		if (neededRotation != 0)  {
//			m_pCursor->Rotate(neededRotation, (float) x, (float) y);
			pXform->Rotate((float) neededRotation, (float) x, (float) y);
#ifdef _DEBUG
			nRotation = pXform->GetRotation();
			pXform->GetScaling(fScaleX, fScaleY); 
#endif // _DEBUG
			m_pCursor->SetTransform(pXform);
//			if (m_pCursor->IsVisible())
				pModel->UpdateComponent(m_pCursor, OD_UPDATE_OLD_AND_NEW_POS);
		}
//		pModel->UpdateComponent(m_pCursor, OD_UPDATE_ALL);
	}
}

UINT OVsToolbarIDs [N_OVS_TOOLBARS] = {
	IDR_OD_DRAWING,
	IDR_OD_ROTATE,
	IDR_OD_STRUCTURE,
	IDR_OD_NUDGE,
	IDR_OD_ALIGN,
	IDR_OD_ZOOM_PAN,
	IDR_OD_LINK,
};
char* OVsToolbarTitles [N_OVS_TOOLBARS] = {
	"Disegno",
	"Rotazione",
	"Struttura",
	"Colpetti",
	"Allineamento",
	"Zoom e carrellata",
	"Collegamento",
};

// int TurtleCreateToolbar (int nview, UINT toolbarId, char* toolbarTitle, UINT toolarStyle, UINT toolbarDocking)
// {
// 	CTurtleView* turtleView = theApp.m_turtleViews[nview];
// 	return turtleView->CreateToolbar (toolbarId, toolbarTitle, toolarStyle, toolbarDocking);
// }

int CTurtleView::OVsCreateToolbar (UINT toolbarId, char* toolbarTitle, UINT toolbarStyle, UINT toolbarDocking)
{
	CTurtleFrame* pFrame = (CTurtleFrame*) m_frame;
	// cerca posto vuoto e controlla duplicati
	int i = 0;
	while (pFrame->m_pToolbar[i] != NULL) {
		if (pFrame->m_OVsToolbarIDs[i] == toolbarId)
			return -1;	// duplicato
		i++;
	};
	if (i >= N_OVS_TOOLBARS)
		return -1;	// non c'e' posto vuoto
	// crea e inizializza toolbar, senza dockarlo
	CToolBar* pToolbar = new(CToolBar);
//	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP;	// default
	DWORD dwStyle = WS_CHILD | CBRS_TOP;				// modificato
	if (   (! pToolbar->Create(pFrame, dwStyle))
		|| (! pToolbar->LoadToolBar(toolbarId))) {
		TRACE0("Failed to create drawing toolbar\n");
		return -1;
	}
	pToolbar->SetBarStyle (pToolbar->GetBarStyle() | toolbarStyle);
	pToolbar->SetWindowText(toolbarTitle);
	pToolbar->EnableDocking(toolbarDocking);
	pFrame->EnableDocking (toolbarDocking);
	// inserisce info su toolbar in tabella
	pFrame->m_pToolbar[i] = pToolbar;
	pFrame->m_OVsToolbarIDs[i] = toolbarId;
	pFrame->m_toolbarShown[i] = FALSE;
	return i;
}

// implementazione incompleta:
// per ora aggiunge solo
int CTurtleView::OVsShowToolbar (CToolBar* pToolbar, int show)
{
	CTurtleFrame* pFrame = (CTurtleFrame*) m_frame;
	// cerca toolbar in questione
	int i = 0;
	while (pFrame->m_pToolbar[i] != pToolbar) {
		i++;
		if (i >= N_OVS_TOOLBARS)
			return -1;	// riporta errore
	}
	int shown = pFrame->m_toolbarShown[i] ? 1 : 0;
	if (show == -1)
		return shown;		// solo inquiry
	if (   ((show == 1) && (shown == 1))
		|| ((show == 0) && (shown == 0)))
		return shown;		// gia' ok
	// cerca ultimo mostrato a cui eventualmente accodare
	// (da migliorare, riordinando quando si fa undock)
	int j = N_OVS_TOOLBARS;
	while ((j > 0) && (! pFrame->m_toolbarShown[j-1]))
		j--;
	// mette per primo o accoda
	if (j == 0)
		pFrame->DockControlBar (pToolbar);
	else
		CODGlobal::DockControlBarLeftOf (pFrame, pToolbar, pFrame->m_pToolbar[j-1]);
	pFrame->m_toolbarShown[i] = TRUE;

	return shown;
}

int CTurtleView::OVsProps (int nextProps)
{
	int previousProps = 0;

	return previousProps;
}

// nextTools codifica bit per bit i toolbar da mostrare o nascondere
// -1 per convenzione significa solo inquiry
int CTurtleView::OVsTools (int nextTools)
{
	CTurtleFrame* pFrame = (CTurtleFrame*) m_frame;
	int previousTools = 0;
	int show;
	if (nextTools == -1) {
		show = -1;
		nextTools = 0;
	}
	CToolBar* pToolbar;
	int i;
	int shown;
	for (i=0; i < N_OVS_TOOLBARS; i++) {
		pToolbar = pFrame->m_pToolbar[i];

		if (show != -1)
			show = nextTools & 1;
		nextTools = nextTools >> 1;

		shown = OVsShowToolbar (pToolbar, show);

		previousTools = previousTools << 1;
		if (shown != -1)
			previousTools = previousTools | shown;
	}

	return previousTools;
}

#endif // OVs

int CTurtleView::LineTo(int x, int y)
{
#ifdef OVs
	if (m_isOVs) {
///	CTurtleModel* pModel = (CTurtleModel*) m_vpDiagram.GetModel();
	CTurtleModel* pModel = (CTurtleModel*) CTurtleViewport::GetModel();
	if (m_isPolyline && m_pLine)
		m_pLine->AddVertex(CPoint (x, y));
	else {
		BOOL ok;
		CODPointArray codPoints;
		codPoints.SetSize(2);
		codPoints.SetAt(0, CPoint (m_xPos, m_yPos));             
		codPoints.SetAt(1, CPoint (x, y));             
		m_pLine = new(CODLineComponent);
		ok = m_pLine->Create(&codPoints);
		if (m_penWidth != 1)
			ok = m_pLine->SetValue(OD_PROP_LINE + OD_LINE_WIDTH, m_penWidth);
		if (m_penStyle != PS_SOLID)
			ok = m_pLine->SetValue(OD_PROP_LINE + OD_LINE_STYLE, m_penStyle);
		if (m_penColor != RGB(0,0,0))
			ok = m_pLine->SetValue(OD_PROP_LINE + OD_LINE_COLOR, m_penColor);
		ok = pModel->AppendChild(m_pLine);
	}
	if (m_refresh)
		pModel->UpdateComponent(m_pLine, OD_UPDATE_CURRENT_POS);
	m_xPos = x;
	m_yPos = y;
	}
	else {
#endif // OVs

	CPoint scrollPos = GetScrollPosition();
	int result;
	CPoint oldPoint;
	CPen newPen;
	CPen* oldPen;
	result = newPen.CreatePen(m_penStyle, m_penWidth,
//							ColorOrIndex(m_penColor));
							ColorOrIndex(writeColor));
	CDC* pDC = GetDC();

#ifdef SAVE
	// We also (only) draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	result = canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980313	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	result = canvasDC.RestoreDC(m_nDC);
	oldPen = canvasDC.SelectObject(&newPen);
	result = canvasDC.SetROP2(m_drawMode);
	oldPoint = canvasDC.MoveTo(m_xPos, m_yPos);
	result = canvasDC.LineTo(x, y);
	canvasDC.SelectObject(oldPen);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE
	
#ifdef DRAW_INDIRECT
	int top = min(m_yPos, y) - m_penWidth;
	int left = min(m_xPos, x) - m_penWidth;
	int bottom = max(m_yPos, y) + m_penWidth;
	int right = max(m_xPos, x) + m_penWidth;
#ifdef NEW_ZOOM
// map to view rect and normalize the client coordinates
// (see doc of InvalidateRect)
	MapClipInvalidateRect (top, left, bottom, right);
#endif // NEW_ZOOM
#endif // DRAW_INDIRECT
	
	ReleaseDC (pDC);

#ifdef OVs
	}
#endif // OVs

	return 0;
}

int CTurtleView::Arc (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{	 
	CPoint scrollPos = GetScrollPosition();
	int result;
	CPoint oldPoint;
	CPen newPen;
	CPen* oldPen;
	result = newPen.CreatePen(m_penStyle, m_penWidth,
//							ColorOrIndex(m_penColor));
							ColorOrIndex(writeColor));
	CDC* pDC = GetDC();

#ifdef SAVE
	// We also draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980313	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	canvasDC.RestoreDC(m_nDC);
	oldPen = canvasDC.SelectObject(&newPen);
	canvasDC.SetROP2(m_drawMode);
	canvasDC.Arc(x1, y1, x2, y2, x3, y3, x4, y4);
	canvasDC.SelectObject(oldPen);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE
	
#ifdef DRAW_INDIRECT
#ifdef NEW_ZOOM
	int top = min(y1, y2) - m_penWidth;
	int bottom = max(y1, y2) + m_penWidth;
	int left = x1 - m_penWidth;
	int right = x2 + m_penWidth;
// map to view rect and normalize the client coordinates
	MapClipInvalidateRect (top, left, bottom, right);
#endif // NEW_ZOOM
#endif // DRAW_INDIRECT
	
	ReleaseDC (pDC);

	return 0;
}

CSize textExtent;	// 990222: reso globale (v. PR_LABELDIM)
// se ((x == -1) && (y == -1)) calcola solo textExtent
int CTurtleView::TextOut(int x, int y, char *s)
{
	int strCount = strlen(s);
	int result;
	CFont* oldFont;
	CDC* pDC = GetDC();
	result = pDC->RestoreDC(m_nDC);

	if (m_pFont)
		oldFont = (CFont*) pDC->SelectObject(m_pFont); 
	else
		oldFont = (CFont*) pDC->SelectStockObject(SYSTEM_FONT);

#ifdef SAVE
	// We also draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980313	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	canvasDC.RestoreDC(m_nDC);
	if (m_pFont)
		oldFont = (CFont*) canvasDC.SelectObject(m_pFont); 
	else
		oldFont = (CFont*) canvasDC.SelectStockObject(SYSTEM_FONT);
	canvasDC.SetTextColor(ColorOrIndex(writeColor));
//>980722
	if (m_textBkMode == OPAQUE) {
		canvasDC.SetBkMode(OPAQUE);
		canvasDC.SetBkColor(m_textBkColor);
	} else
		canvasDC.SetBkMode(TRANSPARENT);
//<

//>990222
//	result = canvasDC.TextOut(x, y, s, strCount);
//	CSize textExtent = pDC->GetTextExtent(s, strCount);
	if ((x != -1) || (y != -1))
		result = canvasDC.TextOut(x, y, s, strCount);
	textExtent = pDC->GetTextExtent(s, strCount);
//<

	canvasDC.SelectObject(oldFont);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE

#ifdef DRAW_INDIRECT
	if ((x != -1) || (y != -1)) {	// 990222
#ifdef NEW_ZOOM
		int top = y;
		int left = x;
		int bottom = top + textExtent.cy;
		int right = left + textExtent.cx;
// map to view rect and normalize the client coordinates
		MapClipInvalidateRect (top, left, bottom, right);
#endif // NEW_ZOOM
	}	// 990222
#endif // DRAW_INDIRECT

	ReleaseDC (pDC);	
	
	return 0;
}

int CTurtleView::MoveTo(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	return 0;
}

BOOL CTurtleView::FloodFill (int x, int y)
{
	int result;
	COLORREF crColor;
	CBrush *oldBrush;
	
	CDC *pDC = GetDC();
	result = pDC->RestoreDC(m_nDC);
	m_nDC = pDC->SaveDC();
	
	// We draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	result = canvasDC.CreateCompatibleDC(pDC);
	result = canvasDC.RestoreDC(m_nDC); 	// ???
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	crColor = canvasDC.GetPixel(x, y);

///	CBrush memBrush;
///	result = memBrush.CreateSolidBrush(m_floodColor);
///	oldBrush = canvasDC.SelectObject(&memBrush);
	CBrush* pBrush =
		BrushFromColorAndPalette (pDC, m_floodColor);
	if (pBrush != NULL) {

		oldBrush = canvasDC.SelectObject(pBrush);

///		WORD wPaletteIndex =								// 980327
///			ColorInPalette (pDC, crColor, NULL, 0);				//
///		result =
///			canvasDC.ExtFloodFill(x, y, PALETTEINDEX(wPaletteIndex), FLOODFILLSURFACE);
		result =
			canvasDC.ExtFloodFill(x, y, ColorOrIndex(crColor/*m_penColor*/), FLOODFILLSURFACE);

		canvasDC.SelectObject(oldBrush);
		pBrush->DeleteObject();
		delete(pBrush);
	}
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
	
	ReleaseDC (pDC);
	CScrollView::InvalidateRect (NULL);
	
	return result;
}

#ifndef OLD_FILL

// a imitazione del metodo DrawRect di CPaletteDialog,
// per utilizzare un brush senza limitazioni di colore,
// facendo merge con interfaccia di vecchio metodo FillRect

//>980430
//void CTurtleView::FillRect (int x1, int y1, int x2, int y2, COLORREF* pColor)
void CTurtleView::FillRect (int left, int top, int right, int bottom, COLORREF* pColor)
//<
{
	CDC *pDC = GetDC();
	pDC->RestoreDC(m_nDC);

	CBrush* pBrush =
		BrushFromColorAndPalette (pDC, *pColor);
	if (pBrush != NULL) {

		// We draw on the canvas bitmap  
		CDC canvasDC;
		CBitmap* pOldBit;
		canvasDC.CreateCompatibleDC(pDC);
		pOldBit = canvasDC.SelectObject(m_pCanvas);
		canvasDC.RestoreDC(m_nDC);
		if (theApp.m_pPalette)
			canvasDC.SelectPalette(theApp.m_pPalette, FALSE);
		CBrush* pOldBrush = canvasDC.SelectObject(pBrush);
//>see doc: "When filling the specified rectangle, FillRect
// does not include the rectangle's right and bottom sides.
//		canvasDC.FillRect(CRect(x1, y1, x2, y2), pBrush);
		canvasDC.FillRect(CRect(left, top, right+1, bottom+1), pBrush);
//>980430
		canvasDC.SelectObject(pOldBrush);
		canvasDC.SelectObject(pOldBit);
		canvasDC.DeleteDC();

		pBrush->DeleteObject();
		delete(pBrush);
	}

	m_nDC = pDC->SaveDC();
	ReleaseDC (pDC);
	CScrollView::InvalidateRect (NULL);
}

#else // OLD_FILL

void CTurtleView::FillRect (int x1, int y1, int x2, int y2, COLORREF *pColor)
{
	CBrush newBrush;
	CDC *pDC = GetDC();
	pDC->RestoreDC(m_nDC);

//	newBrush.CreateSolidBrush(*pColor);					// 980313
///	WORD wPaletteIndex =								// 
///		ColorInPalette (pDC, *pColor, NULL, 0);			//
///	newBrush.CreateSolidBrush(PALETTEINDEX(wPaletteIndex));

//	pDC->FillRect(CRect(x1, y1, x2, y2), &newBrush);	// 971223: non serve ?
	m_nDC = pDC->SaveDC();
	
#ifdef SAVE
	// We also draw on the canvas bitmap  
	CDC canvasDC;
	CBitmap* pOldBit;
	canvasDC.CreateCompatibleDC(pDC);
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	canvasDC.RestoreDC(m_nDC);
//>980303 (ma non servono a niente !) 980313: ora dovrebbe servire
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);
	CBrush* pOldBrush = canvasDC.SelectObject(&newBrush);
//	canvasDC.RealizePalette();
//<980303

	//>971223 aggiunto, per approssimare il colore; FUNZIONA: NON BUTTARE
	//	COLORREF crColor = canvasDC.GetPixel(x1, y1);
	//	COLORREF proxColor = canvasDC.SetPixel(x1, y1, *pColor);
	//	canvasDC.SetPixel(x1, y1, crColor);
	//	newBrush.CreateSolidBrush(proxColor);
	//<

//>see doc: "When filling the specified rectangle, FillRect
// does not include the rectangle's right and bottom sides. GDI fills a rectangle up to, but does not include, the 
//	canvasDC.FillRect(CRect(x1, y1, x2, y2), &newBrush);
	canvasDC.FillRect(CRect(x1, y1, x2+1, y2+1), &newBrush);
//>980429
	canvasDC.SelectObject(pOldBrush);
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
#endif // SAVE
	
	ReleaseDC (pDC);
	
	CScrollView::InvalidateRect (NULL);
}

#endif // OLD_FILL

int CTurtleView::Zoom (double factor, int xPos, int yPos, int xDim, int yDim)
{
	int result;
	CDC *pDC;
	
	// restore current DC
	pDC = GetDC();
	result = pDC->RestoreDC(m_nDC);
	int ableToStretch = pDC->GetDeviceCaps(RASTERCAPS) & RC_STRETCHBLT;
	ReleaseDC (pDC);
	
	if (! ableToStretch) {
		MessageBox("Questo schermo non sa fare Zoom");
		return ableToStretch;
	}

#ifdef NEW_ZOOM

//>991001
	CSize clientSize;
	RECT ClientArea;
	int horzMinPos, horzMaxPos, vertMinPos, vertMaxPos;
	CPoint scrollPos;
	int xCenter, yCenter;
	CScrollBar* pHorzBar = NULL;
	CScrollBar* pVertBar = NULL;

	// get size of client area
	m_frame->GetClientRect(&ClientArea);
	// aggiorno attributi che servono a convertire input
	m_xDim = ClientArea.right - ClientArea.left;
	m_yDim = ClientArea.bottom - ClientArea.top;  
	// verifico che esistano scrollbar
	GetScrollRange(SB_HORZ, &horzMinPos, &horzMaxPos);
	GetScrollRange(SB_VERT, &vertMinPos, &vertMaxPos);
//<
	// recupero posizione scroll e centro nella scala corrente
	// corrispondente alla "dimensione del documento" dichiarata
	// (vecchio zoom factor: nuovo diviso fattore incrementale)
	scrollPos = GetScrollPosition();
//>991001
	// se non c'e' scroll, ho perso memoria del centro e lo pongo al "centro"
	// del canvas (eventualmente in scala ridotta)
//	if (! horzMaxPos)
	if (m_xScroll <= m_xDim)
		xCenter = m_xScroll / 2;
	else
//<
		xCenter = scrollPos.x + m_xDim/2;
//>991001
	// se non c'e' scroll, ho perso memoria del centro e lo pongo al "centro"
	// del canvas (eventualmente in scala ridotta)
//	if (! vertMaxPos)
	if (m_yScroll <= m_yDim)
		yCenter = m_yScroll / 2;
	else
//<
		yCenter = scrollPos.y + m_yDim/2;

	// calcolo "dimensione del documento" nella nuova scala
	m_xScroll = (int) (m_zoomFactor * m_xPixel);
	m_yScroll = (int) (m_zoomFactor * m_yPixel);

	// dichiaro "dimensione del documento" nella nuova scala
	CSize scrollSize  = CSize(m_xScroll, m_yScroll);
	SetScrollSizes(MM_TEXT, scrollSize);

//>991001
	// get size of client area
	m_frame->GetClientRect(&ClientArea);
	// aggiorno attributi che servono a convertire input
	m_xDim = ClientArea.right - ClientArea.left;
	m_yDim = ClientArea.bottom - ClientArea.top;  
	// verifico che esistano scrollbar
	// ma non riporta un risultato aggiornato !!!
	GetScrollRange(SB_HORZ, &horzMinPos, &horzMaxPos);
	GetScrollRange(SB_VERT, &vertMinPos, &vertMaxPos);

	// calcolo il centro e la posizione di scroll nella nuova scala
	scrollPos.x = 0;
	xCenter = (int) (factor * xCenter);
///	if (horzMaxPos) {
	if (m_xScroll > m_xDim)
		scrollPos.x = xCenter - m_xDim/2;

	scrollPos.y = 0;
	yCenter = (int) (factor * yCenter);
///	if (vertMaxPos) {
	if (m_yScroll > m_yDim)
		scrollPos.y = yCenter - m_yDim/2;

		ScrollToPosition (scrollPos);
#endif // NEW_ZOOM

	// comunque produco un aggiornamento della finestra
	CScrollView::InvalidateRect (NULL);

	return ableToStretch;
}

int CTurtleView::Resize (int xPos, int yPos, int xDim, int yDim)
{
	BOOL result;
	int ableToStretch;
	CDC *pDC;				// current DC
	CDC canvasDC;			// source DC in stretch operation; dest. DC in save operation	
	CBitmap* pOldBit;
	
	// restore current DC
	pDC = GetDC();
	pDC->RestoreDC(m_nDC);
	
	//	create compatible DC and select for it old canvas bitmap	
	canvasDC.CreateCompatibleDC(pDC);
	
	// resize Frame Window
	CRect frameRect;
	m_frame->GetWindowRect(&frameRect);
	CPoint frameOrigin = frameRect.TopLeft();
	
	int x = (xPos != -1) ? xPos : frameOrigin.x;
	int y = (yPos != -1) ? yPos : frameOrigin.y;
	int cx = (xDim != -1) ? xDim : frameRect.Height();
	int cy = (yDim != -1) ? yDim : frameRect.Width();
	
#ifndef PROVA
	m_frame->MoveWindow(x, y, cx, cy, TRUE);
#else // PROVA
	m_frame->SetWindowPos(&wndTopMost, x, y, cx, cy, SWP_SHOWWINDOW);
#endif // PROVA
	
	// get new size of client area
	CSize clientSize;
	RECT ClientArea;
	m_frame->GetClientRect(&ClientArea);
	clientSize.cx = ClientArea.right - ClientArea.left;
	clientSize.cy = ClientArea.bottom - ClientArea.top;  
	
	// assign new dimensions to this TurtleView
	SetScrollSizes(MM_TEXT, clientSize);
	// update dim values in C structures
	*m_pMaxX = clientSize.cx;
	*m_pMaxY = clientSize.cy;				
	
	// if capable, stretch content of old canvas to this TurtleView
	ableToStretch = pDC->GetDeviceCaps(RASTERCAPS) & RC_STRETCHBLT;
	if (ableToStretch) {
		pOldBit = canvasDC.SelectObject(m_pCanvas);
		pDC->SetStretchBltMode(STRETCH_DELETESCANS);
		result =
			pDC->StretchBlt(0, 0, clientSize.cx, clientSize.cy, &canvasDC, 0, 0, m_xDim, m_yDim, SRCCOPY);
		ASSERT(result);
		canvasDC.SelectObject(pOldBit);
	}
	// declare new dimensions for this TurtleView
	m_xDim = clientSize.cx;
	m_yDim = clientSize.cy;
	
	// create new canvas bitmap
	delete(m_pCanvas);
	m_pCanvas = new(CBitmap);
	result =
		m_pCanvas->CreateCompatibleBitmap(pDC, m_xDim, m_yDim);
	ASSERT(result);
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	if (ableToStretch) {
		result =
			canvasDC.BitBlt(0, 0, m_xDim, m_yDim, pDC, 0, 0, SRCCOPY);
		ASSERT(result);
	} else {
		CBrush newBrush;
		newBrush.CreateSolidBrush(m_screenColor);
		canvasDC.FillRect(CRect(0, 0, m_xDim, m_yDim), &newBrush);
	}
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
	
	// save and release current DC
	m_nDC = pDC->SaveDC();
	ReleaseDC (pDC);
#ifdef OVs	
	CScrollView::InvalidateRect (NULL);
#endif OVs	
	
	return ableToStretch;
}

void CTurtleView::SetMouseState (int state)
{
	m_mouseState = state;
}

CPoint CTurtleView::GetMousePos ()
{
	return m_mousePos;
}

#ifdef OVs

CODImageComponent* ImageLoadFromFile (CString strImagePath)
{
	CODImageComponent* pImage = new(CODImageComponent);
	if (! pImage->Create(strImagePath)) {
		free (pImage);
		pImage = NULL;
	}
	return pImage;
}

int CTurtleView::BitLoadFromFile (CString strImagePath, int mode)
{
	CODImageComponent* pImage = ImageLoadFromFile (strImagePath);
	if (pImage == NULL)
		return 1;
	SECImage* pSECImage = pImage->GetImage();
    DWORD dwWidth = pSECImage->m_dwWidth;
    DWORD dwHeight = pSECImage->m_dwHeight;
	if (mode == 0)
//		pImage->MoveTo(CPoint (m_xPos, m_yPos));
		pImage->MoveTo(CPoint (m_xPos, m_yPos-dwHeight+1));
	else
		pImage->MoveTo(CPoint (0, 0));
	if (! m_pModel->AppendChild(pImage))
		return 2;
	if (m_refresh)
		m_pModel->UpdateComponent(pImage, OD_UPDATE_CURRENT_POS);
	return 0;
}

#endif // OVs

//>040123
//int CTurtleView::BitLoad (int handle, int mode)
int CTurtleView::TurtleLoad (int handle, int mode)
//<040123
{
	int result;

	CBitmap* bitMap;
	CDC* pDC = GetDC();
	bitMap =	// ha side-effect sulla palette !!!
		LoadBitMapFromFile(handle, pDC, theApp.m_pPalette);
	if (bitMap) {
		// We attach the bitmap to an auxiliary memory DC  
		CDC memDC;
		result = memDC.CreateCompatibleDC(NULL);
		CBitmap* pOldMemBit = memDC.SelectObject(bitMap);
		
		// We copy it to the canvas bitmap	
		CDC canvasDC;
		result = canvasDC.CreateCompatibleDC(NULL);
		CBitmap* pOldBit = canvasDC.SelectObject(m_pCanvas);
		
		CSize bitSize = bitMap->GetBitmapDimension();
		/*> 971223*/
		//		result = canvasDC.BitBlt(0, 0, bitSize.cx, bitSize.cy, &memDC, 0, 0, SRCCOPY);
		
		int nView = theApp.GetTurtleViewIndex (this);
		int x, y, x2, y2, cx, cy, x1, y1;
		x1 = 0;	// fattorizzato: 981016
		// modo BITLOAD (vecchio, corretto):
		// da pos. tarta, per coordinate logiche crescenti
		if (mode == 0) {
			GetIntPos (nView, &x, &y);
			x2 = x;	// fattorizzato per mode 0: 981016
//			if (bitSize.cx <= m_xPixel) {
//				cx = bitSize.cx;
//			} else {
//				cx = m_xDim - x;
//			}
			cx = __min (bitSize.cx, m_xPixel-x);// corretto 981016
//			if (bitSize.cy <= m_xPixel) {		// corretto 981016
//				y2 = y - bitSize.cy + 1;
//				cy = bitSize.cy;
//				y1 = 0;
//			} else {
//				y2 = 0;
//				cy = y;
//				y1 = bitSize.cy - y;
//			}
			cy = y + 1;		// dim. utile in canvas
			if (bitSize.cy <= cy) {
				y2 = y - bitSize.cy;
				cy = bitSize.cy;
				y1 = 0;
			} else {
				y2 = 0;
				y1 = bitSize.cy - cy;
			}
		}
		// modo RECUPERA_DIS (nuovo)
		// da origine canvas (TopLeft), per coord. fisiche crescenti
		else {
			x2 = 0;
			y1 = 0;
			y2 = 0;
			cx = __min (bitSize.cx, m_xPixel);
			cy = __min (bitSize.cy, m_yPixel);
		}
		result = canvasDC.BitBlt(x2, y2, cx, cy, &memDC, x1, y1, SRCCOPY);
		
		canvasDC.SelectObject(pOldBit);
		canvasDC.DeleteDC();
		
		memDC.SelectObject(pOldMemBit);
		memDC.DeleteDC();
		delete (bitMap);
		
		result = pDC->RealizePalette();	// 980224
#ifdef OVs
		CScrollView::InvalidateRect(NULL);
#endif OVs
		result = TRUE;
	}
	else {
		result = FALSE;
	}

	// release current DC and return
	ReleaseDC (pDC);	// 980407: aggiunto
	return result;
}

//>040123
//int CTurtleView::BitSave (int hFile /*handle*/, int mode)
int CTurtleView::TurtleSave (int hFile /*handle*/, int mode)
//<040123
{
#define PROVA_BITSAVE

	BOOL result;
	int xLowView, xHighView, yLowView, yHighView;
	int nView = theApp.GetTurtleViewIndex (this);
//	GetIntActiveArea (nView, &xLowView, &xHighView, &yLowView, &yHighView);
	GetIntActiveArea (nView, &xLowView, &yLowView, &xHighView, &yHighView);
	/*> 971223*/
	//	return SaveBitMaptoFile(handle, m_pCanvas);
	
	int cx = xHighView - xLowView;
	int cy = yLowView - yHighView;
	
	// attach to a memory DC an auxiliary bitmap,
	// same size than the active area,	
	CDC memDC;
	result = memDC.CreateCompatibleDC(NULL);
	CBitmap* pOldMemBit = memDC.SelectObject(m_pCanvas); // TEMPORANEO MA IMPORTANTE
	CBitmap* pNewBitmap = new(CBitmap);
	result =
		pNewBitmap->CreateCompatibleBitmap(&memDC, cx, cy);
	ASSERT(result);
	memDC.SelectObject(pNewBitmap);
	
	// create compatible context for canvas bitmap
	CDC canvasDC;
	result = canvasDC.CreateCompatibleDC(NULL);
	CBitmap* pOldBit = canvasDC.SelectObject(m_pCanvas);
	
	// copy its active area to the auxiliary bitmap
	result =
		memDC.BitBlt(0, 0, cx, cy, &canvasDC, xLowView, yHighView, SRCCOPY);
	ASSERT(result);
	
#ifdef PROVA_BITSAVE
	// get display DC
	CDC *pDC = GetDC();
	if (theApp.m_pPalette)
		pDC->SelectPalette(theApp.m_pPalette, FALSE);   

	// Convert auxiliary bitmap to Dib													 
	HBITMAP hBitmap =
		(HBITMAP) pNewBitmap->m_hObject;
	HGLOBAL hDib = BitmapToSharedDib (pDC, hBitmap);
	LPBITMAPINFOHEADER pDib;
	pDib = (BITMAPINFOHEADER *) GlobalLock(hDib);

	// Compute some sizes and offsets (da BitmapToSharedDib)													 
	UINT nNumColors = DibExplicitInfo (pDib);
	char * pDibTable = (char *) pDib + offsetof(BITMAPINFO, bmiColors);
	UINT sizeDibTable = 0;		// 980403: aggiunto
	if (nNumColors <= 256)		//
		sizeDibTable = nNumColors * sizeof(RGBQUAD);
	char * pImage = (char *) pDibTable + sizeDibTable;
	UINT sizeImage = ((BITMAPINFOHEADER *) pDib)->biSizeImage;

	// Prepare file header
	BITMAPFILEHEADER bmfhFileHeader;
	memset(&bmfhFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bmfhFileHeader.bfType = 0x4d42;   // BM
	bmfhFileHeader.bfSize = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER) + sizeDibTable + sizeImage;
	bmfhFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) +
			sizeof(BITMAPINFOHEADER) + sizeDibTable;

	UINT outLength;
	/* write the BITMAPFILEHEADER */
	outLength = _write (hFile, (LPSTR) &bmfhFileHeader, sizeof (BITMAPFILEHEADER));
	/* write the BITMAPINFOHEADER */
	outLength = _write (hFile, (LPSTR) pDib, sizeof(BITMAPINFOHEADER));
	/* write the COLORTABLE */ 
	if (sizeDibTable)	// 980403: aggiunto
		_write (hFile, (LPSTR) pDibTable, sizeDibTable);
	/* write the IMAGE */ 
	_write (hFile, (LPSTR) pImage, sizeImage);

	// Release shared mem and release display DC
	GlobalUnlock(hDib);				// completa BitmapToSharedDib!!
	ReleaseDC (pDC);
	return TRUE;

#else // PROVA_BITSAVE
	// save to file
	result = SaveBitMaptoFile(handle, pNewBitmap);

#endif // PROVA_BITSAVE
	
	//	delete DCs after unselecting respective bitmap	
	memDC.SelectObject(pOldMemBit);
	memDC.DeleteDC();
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();
	
	delete(pNewBitmap); 
	return result;
	/*< 971223*/
}

// Non usata ?
void CTurtleView::ActivatePalette (CDC *pInDC)
{
	CDC *pDC = pInDC;
	if (pInDC == NULL) {
		pDC = GetDC();
		pDC->RestoreDC(m_nDC);
	}
	pDC->SelectPalette(theApp.m_pPalette, FALSE);
	pDC->RealizePalette();
	if (pInDC == NULL) {
		m_nDC = pDC->SaveDC();
		ReleaseDC (pDC);
	}
}

void CTurtleView::ClearPalette ()
{

	CDC *pDC = GetDC();
	if (theApp.rastercaps & RC_PALETTE) {
		if (theApp.m_pPalette)
			delete(theApp.m_pPalette);
		theApp.m_pPalette = CPaletteFromStock();
		CPalette* pOldPalette =
			pDC->SelectPalette(theApp.m_pPalette, /*bForceBackground*/ FALSE);
		int result = pDC->RealizePalette();
	}
}

HBITMAP CTurtleView::GetClipboardData ()
{
	BOOL result;
	UINT uFormat = CF_BITMAP;	// clipboard format
	HWND hWndNewOwner = m_hWnd;	// handle to window opening clipboard
	HANDLE hData;				// handle of a clipboard object
	result = ::OpenClipboard(hWndNewOwner);
	hData = ::GetClipboardData(uFormat);
	if (! hData)
		result = ::CloseClipboard();
	return (HBITMAP) hData;			// ATTENZIONE: NON CHIUDE !!!!
}

//BOOL CTurtleView::SetClipboardData(HBITMAP hBitmap) // data handle
//BOOL CTurtleView::SetClipboardData(HGLOBAL hDib) // data handle
BOOL CTurtleView::SetClipboardData(HBITMAP hBitmap, HPALETTE hPalette, HGLOBAL hDib) // data handle
{
	BOOL result;
	HWND hWndNewOwner = m_hWnd;	// handle to window opening clipboard
//	HANDLE hDataBitmap;			// handle of a clipboard object
	HANDLE hDataDib;			// handle of a clipboard object
//	HANDLE hDataPalette;		// handle of a clipboard object
	result = ::OpenClipboard(hWndNewOwner);
	result = ::EmptyClipboard();

#ifdef DOC
	Da Reference - Platform SDK - Win32 Functions - SetClipboardData :

	When copying bitmaps, it is best to place only the CF_DIB format
	on the clipboard.
	This is because the colors in a device-dependent
	bitmap (CF_BITMAP) are relative to the system palette,
	which may change before the bitmap is pasted.
	If only the CF_DIB format is on the clipboard
	and a window requests the CF_BITMAP format,
	the system renders the device-dependent bitmap
	using the current palette at that time. 

	If you place the CF_BITMAP format on the clipboard
	(and not CF_DIB), the system renders the CF_DIB clipboard format
	as soon as the clipboard is closed.
	This ensures that the correct palette is used to generate
	the device-independent bitmap (DIB).
	Conversions between other clipboard formats occur upon demand. 
#endif // DOC

	// Se l'immagine e' in formato CF_BITMAP:
	// - se c'e' palette su ClipBoard, OK
	// - se manca palette su ClipBoard,i colori non sono corretti
//	hDataPalette = ::SetClipboardData(CF_PALETTE, hPalette);
//	VERIFY(result = (hDataPalette == hPalette));

	hDataDib = ::SetClipboardData(CF_DIB, hDib);
	VERIFY(result = (hDataDib == hDib));

	// Manca la fascia supeiore !!
//	hDataBitmap = ::SetClipboardData(CF_BITMAP, hBitmap);
//	VERIFY(result = (hDataBitmap == hBitmap));

	VERIFY(result = CloseClipboard());
	return result;
}

int CTurtleView::BitCutCopy (int bitIndex, int is_cut, int x, int y, int width, int height)
{
	CBrush newBrush;
#ifdef NEW_ZOOM
	int top, left, bottom, right;
#endif NEW_ZOOM
	
	// restore current DC
	CDC *pDC = GetDC();
	pDC->RestoreDC(m_nDC);
	if (theApp.m_pPalette)
		pDC->SelectPalette(theApp.m_pPalette, FALSE);	// 980226

#ifndef NEW_ZOOM
	// clear area on screen
	if (is_cut) {
		CPoint scrollPos = GetScrollPosition();
		newBrush.CreateSolidBrush(m_screenColor);
		pDC->FillRect(CRect(x - scrollPos.x, y - scrollPos.y -height+1,
			x - scrollPos.x +width, y - scrollPos.y+1), &newBrush);
	}
#endif // NEW_ZOOM
	
	// create compatible context for canvas bitmap
	CDC canvasDC;
	CBitmap* pOldBit;
	canvasDC.CreateCompatibleDC(pDC);
	if (theApp.m_pPalette)
		canvasDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980226	   
	pOldBit = canvasDC.SelectObject(m_pCanvas);

	if (bitIndex >= 0) {	// 980203
		// delete old indexed bitmap
		if (theApp.m_pBitmap[bitIndex]) delete(theApp.m_pBitmap[bitIndex]);
		
		// create compatible context for new indexed bitmap
		CDC indexedDC;
		CBitmap* pOldIndexedBitmap;
		indexedDC.CreateCompatibleDC(&canvasDC);
		if (theApp.m_pPalette)
			indexedDC.SelectPalette(theApp.m_pPalette, FALSE);	// 980226	   
		
		// create and select into context new indexed bitmap
		theApp.m_pBitmap[bitIndex] = new(CBitmap);
		BOOL result =
			theApp.m_pBitmap[bitIndex]->CreateCompatibleBitmap(&canvasDC, width, height);
		ASSERT(result);
		pOldIndexedBitmap = indexedDC.SelectObject(theApp.m_pBitmap[bitIndex]);
		
		// copy rectangle from canvas bitmap to indexed bitmap
		result =
			indexedDC.BitBlt(0, 0, width, height, &canvasDC, x, y-height+1, SRCCOPY);
		ASSERT(result);
		
		if (bitIndex == 0) {
			HBITMAP hBitmap =
				//			HBITMAP(m_pBitmap[bitIndex]);	// ESEMPIO di OPERATORE ??
				(HBITMAP) theApp.m_pBitmap[bitIndex]->m_hObject;

			HPALETTE hPalette = NULL;
			if (theApp.m_pPalette)
				hPalette = (HPALETTE) theApp.m_pPalette->m_hObject;
			//		HGLOBAL hDib = BitmapToSharedDib (hBitmap);
			HGLOBAL hDib = BitmapToSharedDib (pDC, hBitmap); // 980226
			result =
				//			SetClipboardData(hBitmap);		// Bitmap data handle
				//			SetClipboardData(hDib);			// DIB data handle
				SetClipboardData(hBitmap,
				hPalette,
				hDib);
			GlobalUnlock(hDib);				// completa BitmapToSharedDib!!
			theApp.m_pBitmap[bitIndex] = NULL;
		}
		// restore and release indexed bitmap
		indexedDC.SelectObject(pOldIndexedBitmap);
		indexedDC.DeleteDC();

		
	}	// 980203 // 990903: anticipato

	// clear area in canvas bitmap
	if (is_cut) {
			//		canvasDC.FillRect(CRect(x, y, x+width, y-height+1), &newBrush);
#ifdef NEW_ZOOM
		top = y-height+1;
		left = x;
		bottom = y;
		right = x+width-1;
// see doc: "When filling the specified rectangle, FillRect 
// does not include the rectangle's right and bottom sides"
		canvasDC.FillRect(CRect(left, top, right+1, bottom+1), &newBrush);
#endif // NEW_ZOOM
	}
		
///	}	// 980203 // 990903: anticipato
	
	// restore and release canvas DC
	canvasDC.SelectObject(pOldBit);
	canvasDC.DeleteDC();

	// save and release current DC
	m_nDC = pDC->SaveDC();
	ReleaseDC (pDC);

#ifdef NEW_ZOOM
///	if ((bitIndex >= 0) && is_cut)	// 990903
	if (is_cut)						// 990903
		MapClipInvalidateRect (top-1, left-1, bottom+1, right+1);
#endif // NEW_ZOOM

	return 0;
}

/*
long bitModes[16] = {
	0x00CC0020L, // SRCCOPY
		0x00EE0086L, // SRCPAINT
		0x008800C6L, // SRCAND
		0x00660046L, // SRCINVERT
		0x00440328L, // SRCERASE
		0x00330008L, // NOTSRCCOPY
		0x001100A6L, // NOTSRCERASE
		0x00C000CAL, // MERGECOPY
		0x00BB0226L, // MERGEPAINT
		0x00F00021L, // PATCOPY
		0x00FB0A09L, // PATPAINT
		0x005A0049L, // PATINVERT
		0x00550009L, // DSTINVERT
		0x00000042L, // BLACKNESS
		0x00FF0062L, // WHITENESS
};
*/

DWORD bitModes[16] = {	// (tra parentesi estensioni a Mswlogo)

SRCCOPY,	// (0)
SRCCOPY,	// 1	Copies the source bitmap to the destination bitmap.
SRCPAINT,	// 2	Combines pixels of the destination and source bitmaps using the Boolean OR operator.
SRCAND,		// 3	Combines pixels of the destination and source bitmaps using the Boolean AND operator.
SRCINVERT,	// 4	Combines pixels of the destination and source bitmaps using the Boolean XOR operator.
SRCERASE,	// 5	Inverts the desination bitmap and combines the result with the source bitmap using the Boolean AND operator.
NOTSRCCOPY,	// 6	Copies the inverted source bitmap to the destination.
NOTSRCERASE,// 7	Inverts the result of combining the destination and source bitmaps using the Boolean OR operator.
MERGEPAINT,	// 8	Combines the inverted source bitmap with the destination bitmap using the Boolean OR operator.
DSTINVERT,	// 9	Inverts the destination bitmap.
BLACKNESS,	// (10)	Turns all output black.
WHITENESS,	// (11)	Turns all output white.
SRCCOPY,
SRCCOPY,
SRCCOPY,
SRCCOPY,

};

int CTurtleView::BitPaste (int bitIndex, int bitMode, int x, int y)
{
	BOOL result;
	int width;
	int height;
#ifdef NEW_ZOOM
	int top, left, bottom, right;
#endif NEW_ZOOM

	if (bitIndex == 0) {
		theApp.m_pBitmap[0] = NULL;
		CBitmap ClipboardBitmap;
		CBitmap* pClipboardBitmap;
		HBITMAP hClipboardBitmap =
			GetClipboardData ();			// APRE CLIPBOARD !!!

		//	gets a temporary CBitmap object from the Clipboard Bitmap 	
		if (hClipboardBitmap) {
			pClipboardBitmap =
				ClipboardBitmap.FromHandle(hClipboardBitmap);
			theApp.m_pBitmap[0] = pClipboardBitmap;
		}
		else {
			result = ::CloseClipboard();	// CHIUDE CLIPBOARD !!!
			return -1;
		}
	}
	// test existence of indexed bitmap
	if (! theApp.m_pBitmap[bitIndex])
			return 0;
	
	// retrieve dimension of the indexed bitmap
	BITMAP bmpDescriptor;
	long lResult = theApp.m_pBitmap[bitIndex]->GetObject(sizeof(bmpDescriptor), &bmpDescriptor);
	width = bmpDescriptor.bmWidth;
	height = bmpDescriptor.bmHeight;
	CPoint scrollPos = GetScrollPosition();

	// restore current DC
	CDC *pDC = GetDC();
	pDC->RestoreDC(m_nDC);
	
	// create compatible context for indexed bitmap
	CDC indexedDC;
	CBitmap* pOldIndexedBitmap;
	indexedDC.CreateCompatibleDC(pDC);
	
	// select into context the indexed bitmap
	pOldIndexedBitmap = indexedDC.SelectObject(theApp.m_pBitmap[bitIndex]);
	
	// save and release current DC
	m_nDC = pDC->SaveDC();
	ReleaseDC (pDC);
	
	// create compatible context for canvas bitmap
	CDC canvasDC;
	CBitmap* pOldBit;
	canvasDC.CreateCompatibleDC(&indexedDC);
	pOldBit = canvasDC.SelectObject(m_pCanvas);
	
	// copy rectangle from indexed bitmap to canvas bitmap
#ifdef NEW_ZOOM
	top = y-height+1;
	left = x;
	bottom = y;
	right = x+width-1;
	result =
		canvasDC.BitBlt(left, top, width, height, &indexedDC, 0, 0, bitModes[bitMode]);
#endif // NEW_ZOOM
	ASSERT(result);
	
	// restore and release canvas DC
	canvasDC.SelectObject(pOldBit); 				  
	canvasDC.DeleteDC();
	
	// restore and release indexed bitmap
	indexedDC.SelectObject(pOldIndexedBitmap);
	indexedDC.DeleteDC();

	if (bitIndex == 0) {
		result = ::CloseClipboard();	// CHIUDE CLIPBOARD !!!
		theApp.m_pBitmap[0] = NULL;
	}

#ifdef NEW_ZOOM
	if (bitIndex >= 0)
		MapClipInvalidateRect (top-1, left-1, bottom+1, right+1);
#endif // NEW_ZOOM

	return 0;
}

int CTurtleView::BitFit (int bitIndex, int width, int height)
{
	BOOL result;
	int inWidth;
	int inHeight;

	// test existence of indexed bitmap
	if (! theApp.m_pBitmap[bitIndex])
			return 0;

	// restore current DC
	CDC *pDC = GetDC();
	pDC->RestoreDC(m_nDC);

	// create compatible context for in bitmap
	CDC inDC;
	CBitmap* pOldInBitmap;
	inDC.CreateCompatibleDC(pDC);
	
	// retrieve dimension of the in bitmap
	BITMAP inBitmap;
	long lResult =
		theApp.m_pBitmap[bitIndex]->GetObject(sizeof(inBitmap), &inBitmap);
	inWidth = inBitmap.bmWidth;
	inHeight = inBitmap.bmHeight;

	// create compatible context for out bitmap
	CDC outDC;
	CBitmap* pOldOutBitmap;
	VERIFY(result = outDC.CreateCompatibleDC(pDC));
	// provisionally select into it the in bitmap as template !!!
	pOldOutBitmap = outDC.SelectObject(theApp.m_pBitmap[bitIndex]);
	
	// create out bitmap
	CBitmap* pOutBitmap = new(CBitmap);
	result =
	pOutBitmap->CreateCompatibleBitmap(&outDC, width, height);
	ASSERT(result);
	
	// select into each context the right bitmap
	outDC.SelectObject(pOldOutBitmap);
	pOldInBitmap = inDC.SelectObject(theApp.m_pBitmap[bitIndex]);
	pOldOutBitmap = outDC.SelectObject(pOutBitmap);

	// fit
	outDC.SetStretchBltMode(HALFTONE);
	result =
	outDC.StretchBlt(0, 0, width, height, &inDC, 0, 0, inWidth, inHeight, SRCCOPY);
	ASSERT(result);

	// destroy in bitmap
	inDC.SelectObject(pOldInBitmap);
	delete(theApp.m_pBitmap[bitIndex]);	
	// remember out bitmap
	outDC.SelectObject(pOldOutBitmap);
	theApp.m_pBitmap[bitIndex] = pOutBitmap;

	// save and release current DC
	m_nDC = pDC->SaveDC();
	ReleaseDC (pDC);

	return 0;
}

// stampa finestra Tarta, sensibile all'area attiva:
// (sostituisce BitPrint)
int CTurtleView::TurtlePrint ()
{
#ifdef OVs
	if (m_isOVs) {
		OnFilePrint();
		return 0; 
	}
#endif // OVs
	int xLowView, xHighView, yLowView, yHighView;
	int nView = theApp.GetTurtleViewIndex (this);
	GetIntActiveArea (nView, &xLowView, &yLowView, &xHighView, &yHighView);
	return BitmapPrint (xLowView, yLowView, xHighView, yHighView);
}

// int CTurtleView::BitPrint ()
int CTurtleView::BitmapPrint (int xLowView, int yLowView, int xHighView, int yHighView)
{
	CDC *pDC, memDC, printerDC, *ppDC;
	HANDLE hDevModeIn = 0, hDevModeOut;
	DEVMODE *pDevMode;
	int ColorMode;
	int rasterCaps, printerPlanes;
	int printMethod = 0;

	CBitmap* pOldBit;
	int xSrc, ySrc, nSrcWidth, nSrcHeight;
	int nWidth, nHeight;

//	int xLowView, xHighView, yLowView, yHighView;
//	int nView = theApp.GetTurtleViewIndex (this);
//	GetIntActiveArea (nView, &xLowView, &yLowView, &xHighView, &yHighView);

	HBITMAP hBitmap;
	HGLOBAL hDib;
	LPBITMAPINFOHEADER pDib;
	UINT nNumColors, sizeDibTable;
	char * pImage;

	BOOL bPrintSetupOnly = FALSE;
	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;
	CWnd* pParentWnd = (CWnd*) m_frame;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	if (theApp.m_turtlePrinterData != NULL) {
		printDialog.m_pd.hDevMode = theApp.m_turtlePrinterData;
	}
	if (theApp.m_turtlePrinterData != NULL) {
		HGLOBAL hDevModeIn = GlobalAlloc(GMEM_MOVEABLE, sizeof(DEVMODE));
		pDevMode = (DEVMODE *) GlobalLock(hDevModeIn);
		memcpy(pDevMode, &(theApp.m_turtlePrinterMode), sizeof(DEVMODE));
		printDialog.m_pd.hDevMode = hDevModeIn;
	}
	int result = printDialog.DoModal();
	if (result == IDOK) {
		hDevModeOut = printDialog.m_pd.hDevMode;
		if (hDevModeOut != NULL) {
			theApp.m_turtlePrinterData = hDevModeOut;
			pDevMode = (DEVMODE *) GlobalLock(hDevModeOut);
			memcpy(&(theApp.m_turtlePrinterMode), pDevMode, sizeof(DEVMODE));
			GlobalUnlock(hDevModeOut);
		}
		HDC hDC = printDialog.GetPrinterDC ();
		ppDC = printerDC.FromHandle (hDC);

		printerPlanes = ppDC->GetDeviceCaps(PLANES);
		if ((hDevModeOut != NULL) && (printerPlanes > 1)) {
			ColorMode = theApp.m_turtlePrinterMode.dmColor;
			if (ColorMode == DMCOLOR_COLOR)
				MessageBox("Iperlogo: stampa a colori");
			else
				MessageBox("Iperlogo: stampa in B/N");
		}
				
		rasterCaps = ppDC->GetDeviceCaps(RASTERCAPS);
		/* 0x2A19 per Epson EPL-7500 */
		/* 0x229f per HP DeskJet 850C */
		// sceglie il miglior metodo di stampa
		if (rasterCaps & RC_STRETCHDIB)
			printMethod = RC_STRETCHDIB;
		else if (rasterCaps & RC_STRETCHBLT)
			printMethod = RC_STRETCHBLT;
		else if (rasterCaps & RC_BITBLT)
			printMethod = RC_BITBLT;
		else if (rasterCaps & RC_DIBTODEV)
			printMethod = RC_DIBTODEV;
		
		//	CSize bitmapSize = m_pCanvas->GetBitmapDimension ();
		int cxDIB = *m_pMaxX; 
		int cyDIB = *m_pMaxY;
		
		/* FROM OnDraw di DIBVIEW.CPP (MFC samples) */
		
		// get size of printer page (in pixels)
		int cxPage = ppDC->GetDeviceCaps(HORZRES);
		int cyPage = ppDC->GetDeviceCaps(VERTRES);
		// get printer pixels per inch
		int cxInch = ppDC->GetDeviceCaps(LOGPIXELSX);
		int cyInch = ppDC->GetDeviceCaps(LOGPIXELSY);
		
		int x = 0, y = 0;

		xSrc = xLowView;
		nSrcWidth = xHighView-xLowView;
		nSrcHeight = yLowView-yHighView; //+1; 980331

		switch (printMethod) {	// 980610
		case RC_BITBLT:
		case RC_STRETCHBLT:
			ySrc = yHighView;
		break;
		case RC_DIBTODEV:
		case RC_STRETCHDIB:
			ySrc = cyDIB - yLowView;
		break;
		default:
			;
		}
		//
		// Best Fit case -- create a rectangle which preserves
		// the DIB's aspect ratio, and fills the page horizontally.
		//
		// The formula in the "->bottom" field below calculates the Y
		// position of the printed bitmap, based on the size of the
		// bitmap, the width of the page, and the relative size of
		// a printed pixel (cyInch / cxInch).
		//
		if (((double) nSrcWidth / (double) nSrcHeight) >
			((double) cxPage / (double) cyPage)) { 
			nWidth = cxPage;
			nHeight = (int)(((double)nSrcHeight * nWidth * cyInch) /
							((double)nSrcWidth * cxInch));
		}
		else {
			nHeight = cyPage;
			nWidth =  (int)(((double)nSrcWidth * nHeight * cxInch) /
							((double)nSrcHeight * cyInch));
		}

		DOCINFO DocInfo; LPDOCINFO lpDocInfo = &DocInfo;
		DocInfo.lpszDocName = "ProvaStampa";
		DocInfo.lpszOutput = NULL;
		DocInfo.cbSize = sizeof(int) + 2 * sizeof(LPCSTR);
		result = ppDC->StartDoc(lpDocInfo);
		result = ppDC->StartPage();
		
#ifdef STAMPA_ONDRAW
		OnDraw (ppDC);
#endif // STAMPA_ONDRAW

		switch (printMethod) {	// 980610
		case RC_STRETCHDIB:
			// restore current DC
			pDC = GetDC();
			if (theApp.m_pPalette)
				pDC->SelectPalette(theApp.m_pPalette, FALSE);	   
			// Convert canvas bitmap to Dib													 
			hBitmap = (HBITMAP) m_pCanvas->m_hObject;
			hDib = BitmapToSharedDib (pDC, hBitmap);
			pDib = (BITMAPINFOHEADER *) GlobalLock(hDib);
			// Compute some sizes and offsets (da BitmapToSharedDib)													 
			nNumColors = pDib->biClrUsed;
			if (nNumColors == 0)
				nNumColors = 1 << (pDib->biPlanes * pDib->biBitCount);
			sizeDibTable = 0;		// 980403: aggiunto
			if (nNumColors <= 256)		//
				sizeDibTable = nNumColors * sizeof(RGBQUAD);
			pImage =
				(char *) pDib + offsetof(BITMAPINFO, bmiColors) + sizeDibTable;
			// Stretch Dib to printer DC
			result = ::StretchDIBits(
				ppDC->GetSafeHdc(),
				0, 0,						// destination origin
				nWidth, nHeight,			// destination size
				xSrc, ySrc,					// source origin
				nSrcWidth, nSrcHeight,		// source size
				pImage,						// address of bitmap bits
				(BITMAPINFO*) pDib,			// address of bitmap data
				DIB_RGB_COLORS,
				SRCCOPY);
			if (! result) 
				MessageBox("Iperlogo: potrebbe esserci un problema di stampa");
			GlobalUnlock(hDib);				// completa BitmapToSharedDib!!
			// save and release current DC
			ReleaseDC (pDC);
		break;
		case RC_STRETCHBLT:
			result = memDC.CreateCompatibleDC(NULL);
			pOldBit = memDC.SelectObject(m_pCanvas);
			result = ppDC->StretchBlt(0, 0, nWidth, nHeight, &memDC, xSrc, ySrc, nSrcWidth, nSrcHeight, SRCCOPY);
			if (! result) 
				MessageBox("Iperlogo: potrebbe esserci un problema di stampa");
			memDC.SelectObject(pOldBit);
		break;
		case RC_BITBLT:
			nWidth = /*cxPage*/ nSrcWidth;
			nHeight = (int)(((double)nSrcHeight * nWidth * cyInch)/ ((double)nSrcWidth * cxInch));
			result = memDC.CreateCompatibleDC(NULL);
			pOldBit = memDC.SelectObject(m_pCanvas);
			result = ppDC->BitBlt(0, 0, nSrcWidth, nSrcHeight, &memDC, 0, 0, SRCCOPY /*BLACKNESS*/);
			if (! result) 
				MessageBox("Iperlogo: potrebbe esserci un problema di stampa");
			memDC.SelectObject(pOldBit);
		break;
		default:
			MessageBox("Iperlogo: la stampante non ha le capacità grafiche richieste");
		}
		
		result = ppDC->EndPage();
		result = ppDC->EndDoc();
	}
	if (hDevModeIn != NULL)
		GlobalFree(hDevModeIn);  
	return 0;
}


void CTurtleView::OnBitmapPrint()
{
//	BitPrint ();
	TurtlePrint ();
}

// void CTurtleView::SetActiveArea (double xLowArea, double xHighArea, double yLowArea, double yHighArea)
void CTurtleView::SetActiveArea (double xLowArea, double yLowArea, double xHighArea, double yHighArea)
{		   
	m_xLowArea = xLowArea; m_xHighArea = xHighArea;
	m_yLowArea = yLowArea; m_yHighArea = yHighArea;
}

//void CTurtleView::GetActiveArea (double *xLowArea, double *xHighArea, double *yLowArea, double *yHighArea)
void CTurtleView::GetActiveArea (double *xLowArea, double *yLowArea, double *xHighArea, double *yHighArea)
{		   
	*xLowArea = m_xLowArea; *xHighArea = m_xHighArea;
	*yLowArea = m_yLowArea; *yHighArea = m_yHighArea;
}

double xViewToLogo (int x);
double yViewToLogo (int y);
int CTurtleView::SelectionToActiveArea ()
{
	if (m_isSelectArea) {
		struct dview* save_pt = pt; pt = m_logoView;
		m_xLowArea = xViewToLogo(m_rectSelectArea.TopLeft().x);
		m_xHighArea = xViewToLogo(m_rectSelectArea.BottomRight().x);
		m_yLowArea = yViewToLogo(m_rectSelectArea.BottomRight().y);
		m_yHighArea = yViewToLogo(m_rectSelectArea.TopLeft().y);
		pt = save_pt;
		return TRUE;
	}
	else
		return FALSE;
}

void _ResetActiveArea (int handle);
void CTurtleView::ResetActiveArea ()
{		   
	int handle = theApp.GetTurtleViewIndex (this);
	_ResetActiveArea (handle);
}

BOOL CTurtleView::GetModify()
{
	return m_bModified;
}

void CTurtleView::SetModify(BOOL bModified)
{		   
	m_bModified = bModified;
}

int CTurtleView::ConfirmExit()
{
	CString msgCaption = "Iperlogo - Tarta";
	CString msgText =
		"L'immagine modificata non è stata salvata. Vuoi procedere ugualmente ?";
	if (GetModify())
		return (MessageBox(msgText, msgCaption, MB_OKCANCEL) == IDOK);
	else
		return TRUE;
}

void CTurtleView::OnFileClose()
{
	// TODO: Add your command handler code here
	//	CWnd::OnClose ();
	int handle = theApp.GetTurtleViewIndex (this);
	//	int dev = handle2dev (handle, O_TARTA);
	if (ConfirmExit ())			// 980903
		chiudi_tarta (handle);
}

void CTurtleView::OnBitmapActivearea()
{
	// TODO: Add your command handler code here
	CActiveArea AreaDialog (m_frame);
	
	//	GetActiveArea ( &AreaDialog.m_areaXlow, &AreaDialog.m_areaXhigh,
	//					&AreaDialog.m_areaYlow, &AreaDialog.m_areaYhigh );
	
	BOOL result = AreaDialog.DoModal ();
}

void CTurtleView::OnBitmapLoad()
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		//		"File Bitmap (*.bmp) | *.bmp | Tutti i file (*.*) | *.* ||";
		"File Bitmap (*.bmp)|*.bmp|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt = NULL;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = TRUE;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	
	int result = FileDialog.DoModal();
	if (result == IDOK) {
		UINT nOpenFlags = CFile::modeRead;
		CString PathName = FileDialog.GetPathName();
#ifdef OVs
		if (m_isOVs) 
		{
			strcpy (m_saveFile, PathName);
			BitLoadFromFile (PathName, 1);
		}
		else {
#endif // OVs
			int handle;
			if ((handle = _open (PathName, _O_RDONLY | _O_RAW, _S_IWRITE | _S_IREAD)) == 0)
				;
			else {
				strcpy (m_saveFile, PathName);
//				_BITLOAD (m_nView, handle, 1);
				_TURTLELOAD (m_nView, handle, 1); // 040123
				_close (handle);
			}
#ifdef OVs
		}
#endif // OVs
	} else {
		/* file not found */
	}
}


void CTurtleView::BitmapNew()
{
	OnBitmapNew();
}

void CTurtleView::OnBitmapNew()
{
	// TODO: Add your command handler code here

#ifdef OVs
	if (m_isOVs) 
	{
		ClearScreen (TRUE);
	}
	else {
#endif // OVs
		int save_i_tarta = i_tarta; i_tarta = m_nView;
		int save_nv = nview; nview = m_nView;
		struct dview* save_pt = pt; pt = m_logoView;
		PULISCISCHERMO ();
		pt = save_pt;
		nview = save_nv;
		i_tarta = save_i_tarta;
#ifdef OVs
	}
#endif // OVs
}

void CTurtleView::OnBitmapSaveas()
{
	// TODO: Add your command handler code here
	
	static char BASED_CODE lpszFilter[] =
		//		"File Bitmap (*.bmp) | *.bmp | Tutti i file (*.*) | *.* ||";
		"File Bitmap (*.bmp)|*.bmp|Tutti i file (*.*)|*.*||";
	LPCSTR lpszFileName = NULL;
	LPCSTR lpszDefExt= "bmp";		// corretto
	if (m_saveFile[0] != 0) 		// nuovo
		lpszFileName = m_saveFile;
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd* pParentWnd = m_frame;
	BOOL bOpenFileDialog = FALSE;
	int handle;
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	
	int result = FileDialog.DoModal();
	if (result == IDOK) {
		UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite;
		CString PathName = FileDialog.GetPathName();
		if ((handle = _open (PathName, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) == 0)
			;
		else {
			strcpy (m_saveFile, PathName);
//			_BITSAVE (m_nView, handle, 0);
			_TURTLESAVE (m_nView, handle, 0); // 040123
			_close (handle);
		}
		SetModify (FALSE);	// 980903
	} else {
		/* file not found */
	}
}

void CTurtleView::OnBitmapSave()
{
	// TODO: Add your command handler code here
	
	int handle;
	if (m_saveFile[0] == 0)
		OnBitmapSaveas ();
	else {
		if ((handle = _open (m_saveFile, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) == 0)
			;
		else {
			//> 971223
			//			BitSave (handle);
//			_BITSAVE (m_nView, handle, 0);
			_TURTLESAVE (m_nView, handle, 0); // 040123
			//<
			_close (handle);
			SetModify (FALSE);	// 980903
		}
	}
}

void CTurtleView::OnUpdateEditCopia(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_isSelectArea);
}
void CTurtleView::OnEditCopy() 
{
	// TODO: Add your command handler code here

	if (m_isSelectArea) {
		_BITCUTCOPY (m_nView, 0, FALSE,
			m_rectSelectArea.TopLeft().x,
			m_rectSelectArea.BottomRight().y,
			m_rectSelectArea.Width() + 1,
			m_rectSelectArea.Height() + 1);
	}
}

void CTurtleView::OnUpdateEditTaglia(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_isSelectArea);
}
void CTurtleView::OnEditCut() 
{
	// TODO: Add your command handler code here

	if (m_isSelectArea) {
		_BITCUTCOPY (m_nView, 0, TRUE,
			m_rectSelectArea.TopLeft().x,
			m_rectSelectArea.BottomRight().y,
			m_rectSelectArea.Width() + 1,
			m_rectSelectArea.Height() + 1);
	}
}

void CTurtleView::OnUpdateEditButta(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_isSelectArea);
}
void CTurtleView::OnEditDelete() 
{
	// TODO: Add your command handler code here
	
	if (m_isSelectArea) {
		_BITCUTCOPY (m_nView, -1, TRUE,
			m_rectSelectArea.TopLeft().x,
			m_rectSelectArea.BottomRight().y,
			m_rectSelectArea.Width() + 1,
			m_rectSelectArea.Height() + 1);
	}
}

void CTurtleView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	
//	BitPaste (/*bitIndex*/ 0, /*bitMode*/ 0, /*x*/ m_xPos, /*y*/ m_yPos);
	_BITPASTE (m_nView, 0);
}

void CTurtleView::OnUpdateEditRiempi(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_isSelectArea);
}
void CTurtleView::OnEditFill() 
{
	// TODO: Add your command handler code here
	
	if (m_isSelectArea) {
		_BITBLOCK (m_nView,
				m_rectSelectArea.TopLeft().x,
				m_rectSelectArea.BottomRight().y,
				m_rectSelectArea.Width() + 1,
				m_rectSelectArea.Height() + 1);
		}
}

void CTurtleView::OnEditSelectall() 
{
	// TODO: Add your command handler code here
	
	if (m_isSelectArea)	// SI: lasciare test !!!
		ShowSelectArea ();
	m_rectSelectArea =
		CRect(0, 0,	m_xPixel-1, m_yPixel-1);
	m_rectSelectArea.NormalizeRect();
	m_isSelectArea = TRUE;
	ShowSelectArea ();
}

void CTurtleView::OnEditSelectarea() 
{
	// TODO: Add your command handler code here
	
	double	areaXhigh, areaXlow, areaYhigh, areaYlow;
//	GetActiveArea (&areaXlow, &areaXhigh, &areaYlow, &areaYhigh);
	GetActiveArea (&areaXlow, &areaYlow, &areaXhigh, &areaYhigh);
	if (m_isSelectArea)	// SI: lasciare test !!!
		ShowSelectArea ();
	struct dview* save_pt = pt;
	pt = view [m_nView];
	m_rectSelectArea = CRect(
		xLogoToView (areaXlow),
		yLogoToView (areaYhigh),
		xLogoToView (areaXhigh),
		yLogoToView (areaYlow));
	pt = save_pt;
	m_rectSelectArea.NormalizeRect();
	m_isSelectArea = TRUE;
	ShowSelectArea ();
}

#ifdef NEW_CENTER

// x e y sono coordinate fisiche del canvas
void CTurtleView::Center(int x, int y)
{
	if (x == -1)
		x = m_xPixel / 2;
	if (y == -1)
		y = m_yPixel / 2;

#ifdef OVs
	if(m_isOVs) {
		MvcScrollView_T<CTurtleViewport>::ScrollToPosition (CPoint(x, y));
		return;
	}
#endif // OVs

	if ((m_xScroll <= m_xDim) || (m_yScroll <= m_yDim))
		return;

	CPoint scrollPos;

	x *= m_zoomFactor;
	scrollPos.x = x - m_xDim / 2;

	y *= m_zoomFactor;
	scrollPos.y = y - m_yDim / 2;

	ScrollToPosition (scrollPos);
}

#else // NEW_CENTER

void CTurtleView::Center(int x, int y)
{
	CRect winRect;
	GetWindowRect(&winRect);
	int cx = winRect.right-winRect.left;
	int cy = winRect.bottom-winRect.top;
	int horzMinPos, horzMaxPos, vertMinPos, vertMaxPos;
	GetScrollRange(SB_HORZ, &horzMinPos, &horzMaxPos);
	GetScrollRange(SB_VERT, &vertMinPos, &vertMaxPos);

	if (x == -1) x = 0;
	if (horzMaxPos) {
		x = x + (m_xPixel - cx) / 2;
		x = min(x, cx);
	}

	if (y == -1) y = 0;
	if (vertMaxPos) {
		y = y + (m_yPixel - cy) / 2;
		y = min(y, cy);
	}

	if ((vertMaxPos) || (horzMaxPos)) {
#ifdef OVs
		if(m_isOVs) {
			MvcScrollView_T<CTurtleViewport>::ScrollToPosition (CPoint(x, y));
//			UpdateAll();	// CODViewport:: .. MvcVisualPart::Invalidate
		}
		else
			ScrollToPosition (CPoint(x, y));
#endif // OVs
	}
}

#endif // NEW_CENTER

void CTurtleView::OnViewCenter()
{
	// TODO: Add your command handler code here
	Center (-1, -1);	
}

void CTurtleView::OnSetFont()
{
	// TODO: Add your command handler code here
	// get current font description
	
	CWnd* pParentWnd = m_frame;
	//	LOGFONT LogFont;
	if (m_pFont != NULL)
		m_pFont->GetObject(sizeof(LOGFONT), &m_logFont);
	else
		::GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &m_logFont);
	
	CFontDialog FontDialog(&m_logFont, /*CF_EFFECTS|*/ CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT, NULL, pParentWnd);
	if (FontDialog.DoModal() == IDOK)
	{
		if (m_pFont != NULL) delete(m_pFont);
		m_pFont = new(CFont);
		m_pFont->CreateFontIndirect(&m_logFont);
	}
	m_pLogFont = &m_logFont;
}

void CTurtleView::OnSetPalette() 
{
	// TODO: Add your command handler code here
	
//	UINT nIDTemplate = IDD_PALETTE;
// 	CWnd* pParentWnd = NULL;
// 	CWnd* pParentWnd = this;
	if (theApp.rastercaps & RC_PALETTE) {
 		CWnd* pParentWnd = m_frame;
		CPaletteDialog paletteDialog (pParentWnd);
		int result = paletteDialog.DoModal();
	}
}

void CTurtleView::OnSetPenminus()
{
	// TODO: Add your command handler code here
	
	SetPenWidth (min(1, GetPenWidth () - 1));
}

void CTurtleView::OnSetPenplus()
{
	// TODO: Add your command handler code here
	
	SetPenWidth (GetPenWidth () + 1);
}

void CTurtleView::OnSetPencolor()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	CColorDialog ColorDialog(m_penColor, 0, pParentWnd);
	CColorDialogInitialize (&ColorDialog);

	int result = ColorDialog.DoModal();
	if (result == IDOK) {
		CColorDialogFinalize (&ColorDialog);
		m_penColor = ColorDialog.GetColor ();
//> aggiunto 980327:
		CDC *pDC = GetDC();
		ColorInPalette (pDC, m_penColor, &m_penColor, /*tolerance*/ 0);
		ReleaseDC (pDC);
//<
	}
}

void CTurtleView::OnSetFloodcolor()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	CColorDialog ColorDialog(m_floodColor, 0, pParentWnd);
	CColorDialogInitialize (&ColorDialog);
	
	int result = ColorDialog.DoModal();
	if (result == IDOK) {
		CColorDialogFinalize (&ColorDialog);
		m_floodColor = ColorDialog.GetColor ();
//> aggiunto 980327:
		CDC *pDC = GetDC();
		ColorInPalette (pDC, m_floodColor, &m_floodColor, /*tolerance*/ 0);
		ReleaseDC (pDC);
//<
	}
}

void CTurtleView::OnSetScreencolor()
{
	// TODO: Add your command handler code here
	
	CWnd* pParentWnd = m_frame;
	CColorDialog ColorDialog(m_screenColor, 0, pParentWnd);
	CColorDialogInitialize (&ColorDialog);
	
	int result = ColorDialog.DoModal();
	if (result == IDOK) {
		CColorDialogFinalize (&ColorDialog);
		m_screenColor = ColorDialog.GetColor ();
#ifdef OVs
		if (m_isOVs) {
			m_pModel->SetBkgndColor(m_screenColor);
		}
		else  {
#endif OVs
//> aggiunto 980327:
			CDC *pDC = GetDC();
			ColorInPalette (pDC, m_screenColor, &m_screenColor, /*tolerance*/ 0);
			ReleaseDC (pDC);
//<
			int save_i_tarta = i_tarta; i_tarta = m_nView;
			int save_nv = nview; nview = m_nView;
			struct dview* save_pt = pt; pt = m_logoView;
			PULISCI ();
			pt = save_pt;
			nview = save_nv;
			i_tarta = save_i_tarta;
		}
#ifdef OVs
	}
#endif OVs
}

extern "C" void _ZOOM (double factor);

void CTurtleView::OnZoomIn()
{
	// TODO: Add your command handler code here
	//	_ZOOM (2.0);					// 971223
	//	m_zoomFactor *= 2.0;			//	
	_ZOOM (m_zoomFactor * 2.0); 	//
	OnTurtlestatusRefresh();
}

void CTurtleView::OnZoomNormal()
{
	// TODO: Add your command handler code here
	if (m_zoomFactor != 1.0) {
		//		_ZOOM (1.0 / m_zoomFactor); // 971223
		//		m_zoomFactor = 1.0; 		//	
		_ZOOM (1.0);				//
		OnTurtlestatusRefresh();
	}
}

void CTurtleView::OnZoomOut()
{
	// TODO: Add your command handler code here
	//	_ZOOM (0.5);					// 971223
	//	m_zoomFactor *= 0.5;			//	
	_ZOOM (m_zoomFactor * 0.5); 	//
	OnTurtlestatusRefresh();
}

void CTurtleView::OnTurtlestatusRefresh()
{
	// TODO: Add your command handler code here
	
	if (m_status != NULL) {
		char s[16];

		char name [MAX_CHARS];
		GetName (name);
		((CStatusDialog *) m_status)->m_name = name;
		
		CRect winRect;
		m_frame->GetWindowRect(&winRect);
		((CStatusDialog *) m_status)->m_winPosX = winRect.left;
		((CStatusDialog *) m_status)->m_winPosY = winRect.top;
		((CStatusDialog *) m_status)->m_winDimX = winRect.right-winRect.left;
		((CStatusDialog *) m_status)->m_winDimY = winRect.bottom-winRect.top;
		
		((CStatusDialog *) m_status)->m_viewDimX = m_xPixel;
		((CStatusDialog *) m_status)->m_viewDimY = m_yPixel;

		((CStatusDialog *) m_status)->m_viewTop = m_logoView->max_y_t;
		((CStatusDialog *) m_status)->m_viewBottom = m_logoView->min_y_t;
		((CStatusDialog *) m_status)->m_viewLeft = m_logoView->min_x_t;
		((CStatusDialog *) m_status)->m_viewRight = m_logoView->max_x_t;
		
		((CStatusDialog *) m_status)->m_fontName = m_pLogFont->lfFaceName;
		((CStatusDialog *) m_status)->m_fontWeight = m_pLogFont->lfWeight;
		((CStatusDialog *) m_status)->m_fontSize = m_pLogFont->lfHeight;
		getnam (s, m_logoView->penupdown);
		((CStatusDialog *) m_status)->m_penContact = s;
		((CStatusDialog *) m_status)->m_penStyle = m_penStyle;
		((CStatusDialog *) m_status)->m_turtleHeading = m_logoView->alfa;
		((CStatusDialog *) m_status)->m_turtleVisibility = m_logoView->fltarta ? "Visibile" : "Nascosta";
		((CStatusDialog *) m_status)->m_penWidth = m_penWidth;
		((CStatusDialog *) m_status)->m_colorFloodB = ((unsigned char *) &m_floodColor) [2];
		((CStatusDialog *) m_status)->m_colorFloodG = ((unsigned char *) &m_floodColor) [1];
		((CStatusDialog *) m_status)->m_colorFloodR = ((unsigned char *) &m_floodColor) [0];
		((CStatusDialog *) m_status)->m_colorScreenB = ((unsigned char *) &m_screenColor) [2];
		((CStatusDialog *) m_status)->m_colorScreenG = ((unsigned char *) &m_screenColor) [1];
		((CStatusDialog *) m_status)->m_colorScreenR = ((unsigned char *) &m_screenColor) [0];
		((CStatusDialog *) m_status)->m_colorPenB = ((unsigned char *) &m_penColor) [2];
		((CStatusDialog *) m_status)->m_colorPenG = ((unsigned char *) &m_penColor) [1];
		((CStatusDialog *) m_status)->m_colorPenR = ((unsigned char *) &m_penColor) [0];
		((CStatusDialog *) m_status)->m_turtlePosX = m_logoView->pos_x_t;
		((CStatusDialog *) m_status)->m_turtlePosY = m_logoView->pos_y_t;
		
		((CStatusDialog *) m_status)->UpdateData(FALSE);	
		//		GetDlgItem(IDC_...)->Set...();	
	}
}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
// vedia apri_tarta per simmetria
unsigned long CTurtleView::GetAttr ()
{
//>000628
	struct dview* save_pt = pt; pt = m_logoView;
//<
	node lista = NULLP;
//> 991116
	node selezione = logval (m_isSelectionOn);
	lista = rassoc (pc_SELEZIONE, selezione, lista);
//<
	double or_x = - pt->min_x_t;
	double or_y = - pt->min_y_t;
//>990930
//	double scala_x = (double) (pt->max_x_t - pt->min_x_t) / m_xPixel;
//	double scala_y = (double) (pt->max_y_t - pt->min_y_t) / m_yPixel;
	double scala_x = pt->max_x_t - pt->min_x_t;
	double scala_y = pt->max_y_t - pt->min_y_t;
//<
	double dir_x = 1;
	double dir_y = -1;

//>000102
#ifdef OVs
	int turtleMode = (m_isOVs) ? 1 : 0;
	node mode = int_num (turtleMode);
	lista = rassoc (pc_MODO, mode, lista);
#endif // OVs
//<

	RECT frameClientArea;
	GetParent()->GetClientRect(&frameClientArea);
	int cx = frameClientArea.right;
	int cy = frameClientArea.bottom;
	lista = rassoc2 (pc_VISTA,
		consreal (cx), consreal (cy), lista);

	lista = rassoc2 (pc_SCALA,
		consreal (scala_x), consreal (scala_y), lista);

	lista = rassoc2 (pc_DIR,
		consreal (dir_x), consreal (dir_y), lista);

	lista = rassoc2 (pc_OR,
		consreal (or_x), consreal (or_y), lista);

	lista = rassoc2 (pc_PIXEL,
		consreal (m_xPixel), consreal (m_yPixel), lista);

	CRect winRect;
	m_frame->GetWindowRect(&winRect);

	lista = rassoc2 (pc_DIM,
		consreal (winRect.right - winRect.left),
		consreal (winRect.bottom - winRect.top), lista);

	lista = rassoc2 (pc_POS,
		consreal (winRect.left), consreal (winRect.top), lista);

	CMenu* pMenu = m_frame->GetMenu ();
	node menu = (pMenu == NULL) ? pc_FALSO : pc_VERO;
	lista = rassoc (pc_MENU, menu, lista);

	char sTitolo [MAXCHARS];
	int nTitolo = m_frame->GetWindowText(sTitolo, MAXCHARS);
/*>990311
	node titolo = (nTitolo == 0) ? pc_FALSO : _parse (sTitolo);
*/
	BOOL isTitolo = ((m_frame->GetStyle() & WS_CAPTION) != 0);
	node titolo = (isTitolo) ? _parse (sTitolo) : pc_FALSO;
/*<*/
	lista = rassoc (pc_TITLE, titolo, lista);

//>000628
	pt = save_pt;
//<
	return lista;
}

node KwAssoc (node tail, node kWord);
void CTurtleView::SetAttr (node attrList)
{
	CRect winRect;
	m_frame->GetWindowRect(&winRect);
	int xPos = winRect.left;
	int yPos = winRect.top;
	int xDim = winRect.right - winRect.left;
	int yDim = winRect.bottom - winRect.top;

//> 991116
	node selezione = KwAssoc (attrList, pc_SELEZIONE);
	if (selezione != NULLP) {
		selezione = car (cdr (selezione));
		if (MatchKw (selezione, pc_FALSO))
			m_isSelectionOn = FALSE;
		else if (MatchKw (selezione, pc_VERO))
			m_isSelectionOn = TRUE;
	}
//<
	char sTitolo [MAX_CHARS];
	node title = KwAssoc (attrList, pc_TITLE);
	if (title != NULLP) {
		title = car (cdr (title));
		BOOL isFalso = MatchKw (title, pc_FALSO);
		BOOL isVero = MatchKw (title, pc_VERO);
		((CTurtleFrame*) m_frame)->SetTitleBar (! isFalso);
		if ((! isFalso) && (! isVero)) {
			MemScrivi (sTitolo, title);
			m_frame->SetWindowText(sTitolo);
		}
	}
	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		if (MatchKw (menu, pc_FALSO))
			((CTurtleFrame*) m_frame)->SetMenuBar (FALSE);
		else if (MatchKw (menu, pc_VERO))
			((CTurtleFrame*) m_frame)->SetMenuBar (TRUE);
	}
	node pos = KwAssoc (attrList, pc_POS);
	if (pos != NULLP) {
		pos = cdr (pos);
		xPos = rnum_int (car (pos));
		yPos = rnum_int (car (cdr (pos)));
	}
	node dim = KwAssoc (attrList, pc_DIM);
	if (dim != NULLP) {
		dim = cdr (dim);
		xDim = rnum_int (car (dim));
		yDim = rnum_int (car (cdr (dim)));
	}
	if ((pos != NULLP) || (dim != NULLP)) {
		m_frame->MoveWindow(xPos, yPos, xDim, yDim, TRUE);
	}
}

CMenu* CTurtleView::MenuGetItem (UINT iPos)
{
	CMenu* turtleMenu = GetParentFrame()->GetMenu ();
	if (! turtleMenu) return NULL;		// 980717
	return turtleMenu->GetSubMenu (iPos);
}


void CTurtleView::OnTurtlestatusHide()
{
	// TODO: Add your command handler code here
	
	if (m_status != NULL) {
		m_status->DestroyWindow();
		delete (m_status);
		m_status = NULL;
		CMenu* windowPopup = MenuGetItem (2);
		windowPopup->CheckMenuItem (ID_TURTLESTATUS_SHOW, MF_BYCOMMAND | MF_UNCHECKED); 
	}
}

void CTurtleView::OnTurtlestatusShow()
{
	// TODO: Add your command handler code here
	
	if (m_status == NULL) {
		CWnd* pParentWnd = m_frame;
		CStatusDialog *pStatusDialog;
		pStatusDialog = new(CStatusDialog);
		m_status = (CWnd*) pStatusDialog;
		
		BOOL result = pStatusDialog->CreateDir (IDD_TURTLE_STATUS, pParentWnd);
		OnTurtlestatusRefresh();
		CMenu* windowPopup = MenuGetItem (2);
		windowPopup->CheckMenuItem (ID_TURTLESTATUS_SHOW, MF_BYCOMMAND | MF_CHECKED); 
	} else {
		m_status->DestroyWindow();
		OnTurtlestatusHide();
	}
}

void CTurtleView::OnSelectTurtle() 
{
	// TODO: Add your command handler code here
	int handle = theApp.GetTurtleViewIndex (this);
	switch_tarta (handle);
}

void CTurtleView::OnPrintSetup()
{
	// TODO: Add your command handler code here 	 PRINTDLG LPDEVMODE NPDEVMODE  PDEVMODE
	HANDLE hDevModeIn = NULL, hDevModeOut;
	DEVMODE *pDevMode;
	BOOL bPrintSetupOnly = TRUE;
	DWORD dwFlags = 0;
	CWnd* pParentWnd = (CWnd*) m_frame;
	CPrintDialog printDialog (bPrintSetupOnly, dwFlags, pParentWnd);
	if (theApp.m_turtlePrinterData != NULL) {
		HGLOBAL hDevModeIn = GlobalAlloc(GMEM_MOVEABLE, sizeof(DEVMODE));
		pDevMode = (DEVMODE *) GlobalLock(hDevModeIn);
		memcpy(pDevMode, &(theApp.m_turtlePrinterMode), sizeof(DEVMODE));
		printDialog.m_pd.hDevMode = hDevModeIn;
	}
	int result = printDialog.DoModal();
	if (result == IDOK) {
		//		theApp.m_turtlePrinterData = printDialog.GetDevMode ();
		hDevModeOut = printDialog.m_pd.hDevMode;
		if (hDevModeOut != NULL) {
			theApp.m_turtlePrinterData = hDevModeOut;
			pDevMode = (DEVMODE *) GlobalLock(hDevModeOut);
			memcpy(&(theApp.m_turtlePrinterMode), pDevMode, sizeof(DEVMODE));
			GlobalUnlock(hDevModeOut);
		}
	}
	if (hDevModeIn != NULL)
		GlobalFree(hDevModeIn);  
}

#ifdef HELP

void CTurtleView::OnHelpIndex()
{
	// TODO: Add your command handler code here
	
	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_CONTENTS; 		/* type of help */
	DWORD dwData = 0L;						/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CTurtleView::OnHelpTopic()
{
	// TODO: Add your command handler code here
	
	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_PARTIALKEY;		/* type of help */
	DWORD dwData = (DWORD) "";				/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

void CTurtleView::OnHelpTurtle()
{
	// TODO: Add your command handler code here
	
#define HID_TURTLE	0x30001
#define HID_EDITOR	0x30002
	
	HWND hwnd = this->m_hWnd;				/* handle of window requesting help */
	LPCSTR lpszHelpFile = "iperlogo.hlp";	/* address of directory-path string */
	UINT fuCommand = HELP_CONTEXT;			/* type of help */
	DWORD dwData = HID_TURTLE;				/* additional data	*/
	BOOL result = ::WinHelp(hwnd, lpszHelpFile, fuCommand, dwData);
}

#endif // HELP

// ====================================================================

void ASSCHERMO ();
void CENTRA ();
void _PULISCI ();

#include <sys\stat.h>
#include "windows.h"

#define INTROUND(x) ((int) floor(x + 0.5))

#define DRAW_LINE 0
#define FILL_BOX 1
#define EMPTY_BOX 2

#define DFLT -1
#define OR_PEL 0
#define XOR_PEL 1


#define DEFAULT_X_RATIO 1.0
#define DEFAULT_Y_RATIO 1.0
#define VERY_SMALL_REAL 0.0000001

int x_dir = 1,
y_dir = -1;


/* ------------------------------------------------------------------------ */
void ini_view ()
{
	int i;
	for (i=0; i<N_VIEW; i++) view [i] = NULL;	/* azzera punt. a descrit. */
	nview = -1;
}

/* -------------------------------------------------------------
   Crea una nuova "view" nella vecchia terminologia di iperlogo;
   la struttura costituisce ora un descrittore di finestra TARTA,
   per quanto attiene alle coordinate del mondo e agli attributi
   espressi in linguaggio LOGO;
   l'astrazione "view" serve anche a gestire lo stack delle
   finestre TARTA: in cima e' quella selezionata come corrente
-------------------------------------------------------------- */
int apri_view ()
{
	int nv;				// indice in view[N_VIEW]
	struct dview *pv;	// elemento di view[N_VIEW]

	for (nv=0; nv<N_VIEW; ++nv)	/* alloca elemento di indice */
		if (view [nv] == NULL) break;	// successo
	if (nv == N_VIEW) return -1;	// fallimento
								/* descrittore di view TARTA */
	pv = (struct dview *) calloc (1, sizeof (struct dview));
	if (pv == NULL) {			/* problemi con calloc ? */
		//980129	print_err (2);
		return -1;					// fallimento
	}
#ifndef GFX // 000628
	pv->min_x = 0;
	pv->min_y = 0;
#endif
	view [nv] = pv;
	return nv;
}

/* -------------------------------------------------------------
   Accoda in doppia lista una "view" gia' creata:
   - nview diventa l'indice alla cima dello stack (fine della coda)
   - pview diventa il puntatore al coriispondente descrittore
-------------------------------------------------------------- */
void ins_view (int nv)
{
	struct dview *pv;	// elemento di view[N_VIEW]
	pv = view [nv];

	/* accoda view in doppia lista */
	if (nview != -1) pview->dopo = nv;
	pv->prima = nview;
	pv->dopo = -1;
	
	/* rende NV view corrente */
	pview = view [nv];
	nview = nv;
}

/* --------------------------------------------------------
seleziona la viewport NV
ATTENZIONE:  non aggiorna le variabili in cui sono annotate
foglio corrente e tarta corrente
-------------------------------------------------------- */
void sel_view (int nv)
{
	int prima, dopo;
	struct dview *pv;
	if (nv == nview) return;
	pv = view [nv];
	
				/* sfila NV dallo stack */
	dopo = pv->dopo;
	if ((prima = pv->prima) != -1) view[prima]->dopo = dopo;
	if (dopo != -1) view[dopo]->prima = prima;
				/* e la reinserisce in cima */
	pview->dopo = nv;
	pv->prima = nview;
	pv->dopo = -1;
	/* rende NV viewport corrente */
	nview = nv;
	pview = pv;
}

void free_view (int nv)
{
	struct dview *pv = view [nv];
	int prima = pv->prima, dopo = pv->dopo; 
	/* accorcia la catena */
	if (prima != -1)
		view[prima]->dopo = dopo;
	if (dopo != -1)
		view[dopo]->prima = prima;
	/* cede descrittore */
	free ((char *) pv);
	/* cede elemento di indice */
	view [nv] = NULL;
}

int TurtleWindows (node *result)
{
	int l;
	node list = NULLP;
	int nv = nview, prima;
	struct dview *pv = pview;
	int dev;
	if (nv != -1) {
		while  ((prima = pv->prima) != -1) {	// va in testa
			nv = prima;
			pv = view[nv];
		}
		while  (nv != -1) {						// scorre in avanti
			dev = handle2dev (nv, O_TARTA);
			if (dev != -1)
				list = cons (_nome [dev], list, LIST);
			pv = view[nv];
			nv = pv->dopo;
		}
	}
	l = lstlen (list);
	if (result != NULL) *result = list;
	return l;
}

int view_to_devtarta (int view)
{
	int i;
	for (i=0; i<MAX_DEV; i++)
		if ((_file[i] == view) && (_fstato[i] & O_TARTA)) break;
		return i;
}

int widthLogoToView (double cx)
{
	//	return	(int) (cx * (pt->max_x-pt->min_x+1) / (pt->max_x_t-pt->min_x_t));
	return	INTROUND(cx * pt->max_x / (pt->max_x_t-pt->min_x_t));
}

int heightLogoToView (double cy)
{
	//	return	(int) ( cy * (pt->max_y-pt->min_y+1) / (pt->max_y_t-pt->min_y_t));
	return	INTROUND( cy * pt->max_y / (pt->max_y_t-pt->min_y_t));
}

//>991224
double widthViewToLogo (int cx)
{
	double x_scale = pt->max_x_t-pt->min_x_t;
	return cx * (x_scale / pt->max_x);
}

double heightViewToLogo (int cy)
{
	double y_scale = pt->max_y_t-pt->min_y_t;
	return cy * (y_scale / pt->max_y);
}
//<

#ifdef NEW_SCALE

int xLogoToView (double x)
{
	double x_scale = pt->max_x_t - pt->min_x_t;
	return	INTROUND(((x_dir > 0) ? x : -x) * (pt->max_x / x_scale) + pt->xOrg);
}

int yLogoToView (double y)
{
	double y_scale = pt->max_y_t - pt->min_y_t;
	return	INTROUND(((y_dir > 0) ? y : -y) * (pt->max_y / y_scale) + pt->yOrg);
}

double xViewToLogo (int x)
{
	double x_scale = pt->max_x_t-pt->min_x_t;
	return (x - pt->xOrg) * x_dir * (x_scale / pt->max_x);
}

double yViewToLogo (int y)
{
	double y_scale = pt->max_y_t-pt->min_y_t;
	return (y - pt->yOrg) * y_dir * (y_scale / pt->max_y);
}

#else // NEW_SCALE

int xLogoToView (double x)
{
	//	int view_width = pt->max_x-pt->min_x+1;
	int view_width = pt->max_x;
	int x_org = view_width / 2;
	double x_scale = pt->max_x_t-pt->min_x_t;
	//	return	(int) ((x_dir > 0) ? x : -x) * (view_width / x_scale) + x_org;
	return	INTROUND(((x_dir > 0) ? x : -x) * (view_width / x_scale) + x_org);
}

int yLogoToView (double y)
{
	//	int view_height = pt->max_y-pt->min_y+1;
	int view_height = pt->max_y;
	int y_org = view_height / 2;
	double y_scale = pt->max_y_t-pt->min_y_t;
	//	return	(int) ((y_dir > 0) ? y : -y) * (view_height / y_scale) + y_org;
	return	INTROUND(((y_dir > 0) ? y : -y) * (view_height / y_scale) + y_org);
}

double xViewToLogo (int x)
{
	//	int view_width = pt->max_x-pt->min_x+1;
	int view_width = pt->max_x;
	int x_org = view_width / 2;
	double x_scale = pt->max_x_t-pt->min_x_t;
	return (x - x_org) * x_dir * (x_scale / view_width);
}

double yViewToLogo (int y)
{
	//	int view_height = pt->max_y-pt->min_y+1;
	int view_height = pt->max_y;
	int y_org = view_height / 2;
	double y_scale = pt->max_y_t-pt->min_y_t;
	return (y - y_org) * y_dir * (y_scale / view_height);
}

#endif // NEW_SCALE

void GetIntPos (int nView, int *x, int *y)
{
	struct dview* save_pt = pt;
	pt = view [nView];
	*x = xLogoToView(pt->pos_x_t);
	*y = yLogoToView(pt->pos_y_t);
	pt = save_pt;
}

//void GetIntActiveArea (int nView, int *xLowView, int *xHighView, int *yLowView, int *yHighView)
void GetIntActiveArea (int nView, int *xLowView, int *yLowView, int *xHighView, int *yHighView)
{
	double xLowLogo, xHighLogo, yLowLogo, yHighLogo;
	struct dview* save_pt = pt;
	pt = view [nView];
//	TurtleGetActiveArea (nView, &xLowLogo, &xHighLogo, &yLowLogo, &yHighLogo);
	TurtleGetActiveArea (nView, &xLowLogo, &yLowLogo, &xHighLogo, &yHighLogo);
	*xLowView = xLogoToView(xLowLogo); *xHighView = xLogoToView(xHighLogo);
	*yLowView = yLogoToView(yLowLogo); *yHighView = yLogoToView(yHighLogo);
	pt = save_pt;
}

void pset (double x, double y)
{
	TurtlePoint (nview, xLogoToView (x), yLogoToView (y));
}

void line (double x1, double y1, double x2, double y2, int type)
{
	TurtleLine (nview, xLogoToView (x1), yLogoToView (y1), xLogoToView (x2), yLogoToView (y2), type);
}

int TripleToRgb (unsigned char * colors, node triple)
{
	int i;
	for (i=0; i<3; i++) {
		colors[i] = (unsigned char) node2real (car (triple));
		triple = cdr (triple);
	}
	return TRUE;
}

node RgbToTriple (unsigned char * colors)
{
	node triple = NULLP;
	int i;
	for (i=2; i>=0; i--)
		triple = cons (consreal ((double) colors[i]), triple, LIST); 
	return triple;
}

BOOL GetXYFromProp (node name, node propName, int *pX, int *pY)
{
	node val;
	if (   ((val = _prop (name, propName)) != NULLVALUE)
		&& (typ (car (val)) == LBCD)
		&& (typ (car (cdr (val))) == LBCD))
	{
		*pX = rnum_int (car (val));
		*pY = rnum_int (car (cdr (val)));
		return TRUE;
	} else return FALSE;
}

/* ---------------------------------------------------------------------------
Ritorna per indirizzo gli elementi controllati e convertiti di una coppia
di coordinate reali; in caso di errore assume il valore FALSE.
--------------------------------------------------------------------------- */
//> 000115
// int coordinate_double (double *x, double *y)
int coordinate_double (node arg, double *x, double *y)
//<
{
	return (   (lstlen (arg) == 2)
		&& conv_num_double (car (arg), x)
		&& conv_num_double (car (cdr (arg)), y)
		);
}

/* ---------------------------------------------------------------------------
Ritorna per indirizzo gli elementi controllati e convertiti di una coppia
di coordinate; in caso di errore assume il valore FALSE.
--------------------------------------------------------------------------- */
int coordinate_node (node *x, node *y)
{
	return (   (lstlen (arg_1) == 2)
		&& conv_num_node (car (arg_1), x)
		&& conv_num_node (car (cdr (arg_1)), y)
		);
}

/********************************************
PROCEDURE NUOVE (FUORI ORDINE ALFABETICO)
********************************************/

#define PI 3.1415926535

double sin2 (double x)
{
	if ((x == 0.0) || (x == 180.0)) return 0.0;
	else if (x == 90.0) return 1.0;
	else if (x == 270.0) return -1.0;
	else return sin (x * (PI/180.0));  
}

double cos2 (double x)
{
	if ((x == 90.0) || (x == 270.0)) return 0.0;
	else if (x == 0.0) return 1.0;
	else if (x == 180.0) return -1.0;
	else return cos (x * (PI/180.0));  
}

/* ---------------------------------------------------------------------------
Assegna a TARTA la direzione normalizzandola nel range (0, 360)
--------------------------------------------------------------------------- */
void asdir_ (node angolo)
{
	double c_alfa, s_alfa, mezzo_lato;
	while (_segno (angolo)) {
		angolo = addnum (angolo, BCD_360);
	}
	while (cmpnum (angolo, BCD_360) >= 0) {
		angolo = subnum (angolo, BCD_360);
	}
	pt->alfa = node2real (angolo);
	/*>980108
	pt->senalfa = s_alfa = sin (pt->alfa * (PI/180.0));
	pt->cosalfa = c_alfa = cos (pt->alfa * (PI/180.0));
	*/
	pt->senalfa = s_alfa = sin2 (pt->alfa);
	pt->cosalfa = c_alfa = cos2 (pt->alfa);
	/*<*/
	if (pt->fltarta == 1) { /* se tarta visibile fa calcoli per disegno */
		mezzo_lato = lato_tarta / 2.0;
		pt->lx_a = -mezzo_lato * c_alfa;	/* calcola le quantita' da */
		pt->ly_a =	mezzo_lato * s_alfa;	/* sommare alle coordinate */
		pt->lx_b =	lato_tarta * s_alfa;	/* attuali per avere i tre */
		pt->ly_b =	lato_tarta * c_alfa;	/* vertici della tartaruga */
		pt->lx_c =	mezzo_lato * c_alfa;	/* NB: e' ottimizzabile */
		pt->ly_c = -mezzo_lato * s_alfa;
	};
}

/* ---------------------------------------------------------------------------
Procedura per la scrittura su video del simbolo grafico rappresentante la
tartaruga. L'algoritmo si appoggia sulle routines di spostamento gia' 
esistenti all'interno del modulo grafico.
Attualmente il simbolo grafico creato e' un triangolo equilatero ma lo si
puo' modificare agendo sul tracciamento dei suoi contorni.
--------------------------------------------------------------------------- */
void scrivitarta ()
{
	double xa, ya, xb, yb, xc, yc;
	int DrawMode, PenWidth;
	int EraseFlag;	// 980911
	
	if (pt->fltarta == 0) return ;
	/* tarta e' MOSTRATA */
	xa = pt->pos_x_t + pt->lx_a;		/* calcola le coordinate dei*/
	ya = pt->pos_y_t + pt->ly_a;		/* tre vertici		*/
	xb = pt->pos_x_t + pt->lx_b;
	yb = pt->pos_y_t + pt->ly_b;
	xc = pt->pos_x_t + pt->lx_c;
	yc = pt->pos_y_t + pt->ly_c;
	/* Traccia il cursore (tarta) con 4 linee */
	PenWidth = TurtleGetPenWidth (nview);
	DrawMode = TurtleGetDrawMode (nview);
	EraseFlag = TurtleGetEraseFlag (nview);	// 980911
	TurtleSetPenWidth (nview, 1);
	TurtleSetDrawMode (nview, R2_NOT);	// R2_XORPEN R2_COPYPEN
	
	line (pt->pos_x_t, pt->pos_y_t, xa, ya, DRAW_LINE);
	line (xa, ya, xb, yb, DRAW_LINE);
	line (xb, yb, xc, yc, DRAW_LINE);
	line (xc, yc, pt->pos_x_t, pt->pos_y_t, DRAW_LINE);
	
	TurtleSetPenWidth (nview, PenWidth);
	TurtleSetDrawMode (nview, DrawMode);
	TurtleSetEraseFlag (nview, EraseFlag);	// 980911
}

#ifdef OVs
void riscrivitarta ()
{
	TurtleMoveCursor (i_tarta,
		xLogoToView(pt->pos_x_t), yLogoToView(pt->pos_y_t),
		pt->alfa);
}
#endif // OVs

void scriviarea ()	// 980402
{
	TurtleShowSelectArea (i_tarta /*nview*/);
}

/* ---------------------------------------------------------------------------
Seleziona la finestra di tarta e cancella eventuale cursore grafico (TARTA);
calcola posizione di TARTA arrotondata a reticolo di testo
per comandi di scrittura su dispositivo TARTA
--------------------------------------------------------------------------- */
void intarta () 			/* RIVEDERE */
{
#ifdef OVs
	if (theApp.m_turtleViews[nview]->m_isOVs) {
	}
	else {
#endif // OVs
		scrivitarta (); 	/* Cancella tarta	  */
		scriviarea ();
#ifdef OVs
	}
#endif // OVs
}

/* ---------------------------------------------------------------------------
Riscrive eventuale cursore grafico (TARTA) e ripristina le finestre
se necessario.
--------------------------------------------------------------------------- */
void outtarta ()			/* RIVEDERE */
{
#ifdef OVs
	if (theApp.m_turtleViews[nview]->m_isOVs) {
		riscrivitarta (); 	/* riscrive tarta in nuova posizione*/
	}
	else {
#endif // OVs
		scrivitarta (); 	/* riscrive tarta */
		scriviarea ();
#ifdef OVs
	}
#endif // OVs
}

/* ---------------------------------------------------------------------------
Comando per rendere TARTA visibile
--------------------------------------------------------------------------- */
void MOSTARTA ()
{
#ifdef OVs
	if (theApp.m_turtleViews[nview]->m_isOVs) {
		TurtleShowCursor (i_tarta, TRUE);
		pt->fltarta = 1;
	}
	else  {
#endif // OVs
		if (pt->fltarta == 0) {
			intarta (); 		/* toglie eventuali comandi (schermo misto) */
			pt->fltarta = 1;	/* Aggiorna flag di tarta */
			asdir_ (consreal (pt->alfa)); /* fa asdir fittizio per ricalcolo 
			dati tarta */
			outtarta ();		/* Disegna tarta e rimette comandi */
		}
#ifdef OVs
	}
#endif // OVs
}

/* ---------------------------------------------------------------------------
apre una finestra tipo TARTA con il nome specificato;
se non ci riesce ritorna -1, altrimenti ritorna l' indice i dopo aver
assegnato a _file [i] l' handle di tarta e a _nome [i] il nome
--------------------------------------------------------------------------- */
void TurtleCheck (int nv)
{
	int dev;
	char name[MAXCHARS];
	for (dev = 0; dev <= MAX_DEV; ++dev)
		if ((_file [dev] != -1) && (_fstato [dev] & O_TARTA)) {
			pacstr (name, nome (_nome [dev]));
			MainMenuCheckWindowItem (1, name, (nv == _file[dev]));	
			TurtleMenuCheckSelect (_file[dev], (nv == _file[dev]));
			TurtleMenuActivateSelect (_file[dev], (nv != _file[dev]));
		};
}

int apri_tarta (node lnome, const char *name,
				int x1, int y1, int dimx, int dimy,
				int pixel_x, int	pixel_y,
				double x_left, double x_right, double y_bottom, double y_top)
{
	node dim, pos, pixel;
	int nv /*, handle */;
	struct dview *pv;
	CTurtleView* pTurtle;	// 000628
	
	int i /*, j*/;
	node scala, or, dir;
	double scala_x, scala_y, or_x, or_y;
	int dir_x, dir_y;
	node title;
	node menu; int is_menu = -1;
	char nameString[MAXCHARS], titleString [MAXCHARS], *pTitle;
	
	if (lnome == NULLP) {
		lnome = metti_sc (name);
		arg_1 = NULLP;	// 980717: aggiunto per compatibilità con KwAttr
	}
	menu = KwAttr (lnome, arg_1, pc_MENU);
	if (menu)
		is_menu = MatchKw (menu = car (menu), pc_VERO);
	title = KwAttr (lnome, arg_1, pc_TITLE);
	if ((title) && (MatchKw (title = car (title), pc_FALSO)))
		pTitle = (char *) 0L;
	else {
		if (! title)
			title = lnome;
		LogoToText (title, titleString);
		pTitle = &titleString[0];
	}
	if ((i = nuovo_dev ()) == 0)		/* cerca posto per nuovo device */
		return -1;
	
	nv = apri_view ();	// crea descrittore di lista
	if (nv == -1) return -1;
	pv = view[nv];
	ins_view (nv);		// accoda in lista come view corrente

	if (x1 == -1) {
		pos = KwAttr (lnome, arg_1, pc_POS);
		if (! pos) {
			x1 = -1; y1 = -1;
		} else {
			x1 = rnum_int (car (pos));
			y1 = rnum_int (car (cdr (pos)));
		};
	}
	
	if (dimx == -1) {
		dim = KwAttr (lnome, arg_1, pc_DIM);
		if (! dim) {
			dimx = -1; dimy = -1;
		} else {
			dimx = rnum_int (car (dim));
			dimy = rnum_int (car (cdr (dim)));
		};
	}
	
	if (pixel_x == -1) {
		pixel = KwAttr (lnome, arg_1, pc_PIXEL);
		if (! pixel) {
			if (is_menu || (pTitle != NULL)) {
//>000628
//				pixel_x = 800; pixel_y = 800;
				pixel_x = CanvasSize.cx; pixel_y = CanvasSize.cy;
//<
			} else {
				pixel_x = -1; pixel_y = -1;
			}
		} else {
			pixel_x = rnum_int (car (pixel));
			pixel_y = rnum_int (car (cdr (pixel)));
		};
	}

#ifdef OVs
	node mode = KwAttr (lnome, arg_1, pc_MODO);
	if (mode)
		turtleMode = rnum_int (car (mode));
	else
		turtleMode = defaultTurtleMode;
#endif // OVs
	
#ifdef GFX // 000628
	handle =
		TurtleOpenView (lnome, nv, view[nv], x1, y1, dimx, dimy, pixel_x, pixel_y, &(view[nv]->max_x), &(view[nv]->max_y), pTitle, is_menu);
	
	if (handle >= 0) pv->view = handle;
	else {
		free_view (nv);
		return -1;					/* FALLIMENTO: esce con errore */
	};
#else // GFX
	pTurtle =
		TurtleOpenView (lnome, nv, view[nv], x1, y1, dimx, dimy, pixel_x, pixel_y, &(view[nv]->max_x), &(view[nv]->max_y), pTitle, is_menu);
#endif // GFX

	i_tarta = nv;		// ricorda indice di tarta corrente
	pt = view [nv];		// ricorda pointer a descrittore corrente

	// nota: pt->max_x coincide con pTurtle->m_xPixel (stesso per y)
	scala = KwAttr (lnome, arg_1, pc_SCALA);
	if (! scala) {
		scala_x = (double) (pt->max_x) * DEFAULT_X_RATIO;
		scala_y = (double) (pt->max_y) * DEFAULT_Y_RATIO;
	} else {
#ifdef NEW_SCALE
		scala_x = node2real (car (scala));
		scala_y = node2real (car (cdr (scala)));
#else // NEW_SCALE
		scala_x = num_long (car (scala));
		scala_y = num_long (car (cdr (scala)));
#endif // NEW_SCALE
	};
	or = KwAttr (lnome, arg_1, pc_OR);
	if (! or) {
		or_x = scala_x/2; or_y = scala_y/2;
	}
	else {
#ifdef NEW_SCALE
		// modifica irrilevante ?
		or_x = node2real (car (or)); or_y = node2real (car (cdr (or))); 
#else // NEW_SCALE
		or_x = num_long (car (or)); or_y = num_long (car (cdr (or))); 
#endif // NEW_SCALE
	};
	dir = KwAttr (lnome, arg_1, pc_DIR);
	if (! dir) {
		dir_x = 1; dir_y = -1;
	}
	else {
		dir_x = _segno (car (dir)); dir_y = _segno (car (cdr (dir)));
	};

	if (x_left != -1.0) {
		// capita solo creando tarta da dialogo TurtleNew
		pt->min_x_t = x_left;			/* min. val. logico di x */
		pt->max_x_t = x_right;			/* max. val. logico di x */
		pt->min_y_t = y_bottom; 		/* min. val. logico di y */
		pt->max_y_t = y_top;			/* max. val. logico di y */
	} else {
		pt->min_x_t = - or_x;			/* min. val. logico di x */
		pt->max_x_t = scala_x - or_x;	/* max. val. logico di x */
		pt->min_y_t = - or_y;			/* min. val. logico di y */
		pt->max_y_t = scala_y - or_y;	/* max. val. logico di y */
	}
#ifdef NEW_SCALE
//> 000628
//	pt->xOrg = (- pt->min_x_t) * pixel_x / scala_x;
//	pt->yOrg = pt->max_y_t * pixel_y / scala_y;
	pt->xOrg = (- pt->min_x_t) * pTurtle->m_xPixel / scala_x;
	pt->yOrg = pt->max_y_t * pTurtle->m_yPixel / scala_y;
//<
#endif // NEW_SCALE

	pt->penupdown = pc_GIU; 			/* Default penna abbassata GIU' */
	pt->penmode = pc_PAINT;
	pt->flvideo = pc_CIRCOL;		/* Video in modalita' CIRCOLARE */
	pt->fltarta = 0;				/* Tartaruga non visibile */
	pt->pos_x_t = 0;				/* coordinate iniziali */
	pt->pos_y_t = 0;
	asdir_ (BCD_0); 				/* Direzione nord */
	pt->bitIndex = 1;
	pt->bitMode = 0;
	
	TurtlePostOpenView (nview);
#ifdef OVs
#ifdef GFX // 000628
	CTurtleView* pTurtle = theApp.m_turtleViews[i_tarta];
#endif // GFX
	if (pTurtle->m_isOVs) {
		UINT toolbarStyle = CBRS_TOOLTIPS | CBRS_FLYBY |CBRS_SIZE_DYNAMIC;
		UINT toolbarDocking = CBRS_ALIGN_ANY;
		int i;
		for (i=0; i<N_OVS_TOOLBARS; i++) {
//			TurtleCreateToolbar (i_tarta,
			pTurtle->OVsCreateToolbar (
				OVsToolbarIDs [i], OVsToolbarTitles [i],
				toolbarStyle, toolbarDocking);
		}
	}
	else
		((CTurtleFrame*)pTurtle->m_frame)->OVsMenuCollapse ();
#ifndef NEW_SCALE
	pt->xOrg = pt->max_x / 2;
	pt->yOrg = pt->max_y / 2;
#endif // NEW_SCALE
	TurtleGoHome (i_tarta);
	TurtleCreateCursor (i_tarta);
#endif // OVs
	CENTRA ();		// aggiunto
	MOSTARTA ();	// aggiunto
	
	TurtleSetActiveArea (/*nview*/ i_tarta, pt->min_x_t, pt->min_y_t, pt->max_x_t, pt->max_y_t);

//> 991116
	node selezione = KwAssoc (arg_1, pc_SELEZIONE);
	if (selezione != NULLP) {
		selezione = car (cdr (selezione));
		if (MatchKw (selezione, pc_FALSO))
			pTurtle->m_isSelectionOn = FALSE;
		else if (MatchKw (selezione, pc_VERO))
			pTurtle->m_isSelectionOn = TRUE;
	}
//<

	_nome [i] = lnome;					/* nome del device */
#ifdef GFX // 000628
	_file [i] = /*j*/ handle;			/* handle del device */
#else // GFX
	_file [i] = nv;						/* handle del device */
#endif // GFX
	_fstato [i] = O_FINESTRA | O_TARTA; /* stato del device */
	
	pacstr (nameString, nome (lnome));	/* aggiunge voce a menu WINDOWS */
	MainMenuAppendWindowItem (nameString);
	TurtleCheck (nv);	/* opera in menu Main-Finestre */
	
	WaitMessage ();
	EventCheck ();
	
	++newTurtleCounter;  
	
	return i;						/* riporta indice device */
}

/* ---------------------------------------------------------------------------
Cambia la viewport corrente di TARTA.
Non controlla che il parametro sia l'indice di una view di tipo tarta 
e non di tipo foglio.
--------------------------------------------------------------------------- */
void switch_tarta (int i)
{
	if (i_tarta == i)			/* se gia' tarta corrente */
		return ;				/* esco senza far nulla */
	if (i != -1) {
		sel_view (i);		// aggiunto, perche' ora non e' chiamata
							// solo da inc_cambia_tarta 
		pt = view [i];	/* prende puntatore a descrittore */
		i_tarta = i;		/* ricorda indice di tarta corrente */
		TurtleCheck (i);	/* opera in menu Main-Finestre */
	}
	else {
		nview = -1;		// aggiunti 980612
		i_tarta = -1;	//
		pview = NULL;	//
		pt = NULL;
	}
}

/* ---------------------------------------------------------------------------
Se e' stata nascosta o chiusa la finestra attuale di tarta cerca la
piu' recente (quella usata piu' recentemente) finestra di tarta per
metterla come finestra attuale per tarta.
--------------------------------------------------------------------------- */
void inc_cambia_tarta (int i)
{
	int nv;
	if (i_tarta == i) { /* se ha lasciato tarta corrente */
						/* cerca la tarta piu' recente */
		if ((nv = view [i]->dopo) == -1)
			nv = view [i]->prima;
		switch_tarta (nv);
	};
}

void chiudi_tarta (int i)
{
	int dev;
	char nameString[MAXCHARS];
	
//	TurtleCloseView (i);	// 980717: posposto !!! serve ancora !!!
	inc_cambia_tarta (i);		/* cambio tarta attuale se necessario */
	TurtleCloseView (i);
	free_view (i);				/* libera descrittore */
	dev = handle2dev (i, O_TARTA);
	pacstr (nameString, nome (_nome [dev]));
	MainMenuRemoveWindowItem (nameString);
	_file [dev] = -1;
}

void nas_tarta (int i)
{
}

/*****************************************************************************
PROCEDURE E FUNZIONI IN ORDINE ALFABETICO
*****************************************************************************/

/* ---------------------------------------------------------------------------
Trasforma l'angolo pt->alfa (direzione di tarta) in un angolo espresso in
radianti secondo riferimento standard ( 0 = asse x, senso antiorario )
Non e' normalizzato (riporta -5/4 PI  invece di 3/4 PI).
------------------------------------------------------------------------- */
double angolo_stand ()
{
	return ( (PI/2) - (pt->alfa * (PI/180.0)) );
}

/* ---------------------------------------------------------------------------
Riporta coord. normalizzata rispetto al suo range campo circolare.
cmax e cmin sono il max e il min per la coordinata.
--------------------------------------------------------------------------- */
/* RIVEDERE forse si puo' usare fmod*/
double _dentro (double coord, double cmin, double cmax)
{
	double delta;
	delta = cmax - cmin;
	while (coord < cmin) coord += delta;
	while (coord > cmax) coord -= delta;
	return coord;
}


/* ---------------------------------------------------------------------------
Funzione che riporta la direzione che dovrebbe assumere TARTA per muoversi
dalla posizione attuale verso il punto di coordinate specificate.
RIPORTA: un angolo tra 0 e 360 o riporta 400 in caso di errore
--------------------------------------------------------------------------- */
double _verso (double x, double y)
{
	double dx, dy, temp;
	dx = x - pt->pos_x_t;
	dy = y - pt->pos_y_t;
	if (dx == 0) {
		if (dy == 0) return 400.0;
		else return (dy < 0) ? 180.0 : 0.0;
	};
	temp = 90.0 - ((180.0/PI) * atan2 (dy, dx));
	if (temp < 0) return (temp + 360.0);
	return temp;
}

int fuori_x,	/* X e' fuori schermo fisico			*/
fuori_y;	/* Y e' fuori schermo fisico			*/

			/* ---------------------------------------------------------------------------
			Riporta TRUE se le coordinate sono fuori dal range virtuale;
--------------------------------------------------------------------------- */
int _fuori (double x, double y)
{
	return (fuori_x = ((x < pt->min_x_t) || (x > pt->max_x_t)))  ||
		(fuori_y = ((y < pt->min_y_t) || (y > pt->max_y_t)));
}

/* ---------------------------------------------------------------------------
Traccia una retta verticale lunga dy che esce dal video con 
schermo circolare
------------------------------------------------------------------------- */
void retta_verticale (double dy)
{
	double y_temp, passo_y;
	while ( _fuori (pt->pos_x_t, pt->pos_y_t + dy) ) {
		y_temp = (dy < 0) ? pt->min_y_t : pt->max_y_t; /* vado a max o min y */
		passo_y = y_temp - pt->pos_y_t;
		dy -= passo_y;
		line (pt->pos_x_t, pt->pos_y_t, pt->pos_x_t,  y_temp, DRAW_LINE);
		pt->pos_y_t = (dy < 0) ? pt->max_y_t : pt->min_y_t;/* lo riporto giu'/su*/
	};
				/* disegna ultimo pezzo che non esce */
	line (pt->pos_x_t, pt->pos_y_t,
		pt->pos_x_t, pt->pos_y_t + dy, DRAW_LINE);
	pt->pos_y_t += dy;
}	 

/* ---------------------------------------------------------------------------
Traccia una linea che fuoriesce dallo schermo con schermo CIRCOLARE; 
dalle coordinate correnti incrementandole di quanto specificato; 
--------------------------------------------------------------------------- */
void spostati_circolare (double dx, double dy)
{
	double x_temp, y_temp, passo_x, passo_y, alfad, tan_alfa_stand;
	double angolo;
	int prima_x;
	if (dx == 0) {		/* CASO di retta verticale (tan = infinito) */
		retta_verticale (dy); 
		return;
	};
	alfad = pt->alfa;			/* copia locale di alfa */
	tan_alfa_stand = tan (angolo_stand ()); /* calcola tan in notaz. standard */
	if (_segno (arg_1)) {	/* se l'argomento era negativo cambia alfa localmente*/
		alfad += (alfad < 180) ? 180.0 : -180.0;
	};
	while ( _fuori (pt->pos_x_t + dx, pt->pos_y_t + dy) ) {
		/* determina su quale asse esce per primo */
		prima_x = FALSE;		/* default: esce prima lungo l'asse y */
		if (alfad < 90.0) { 		/* 1" quadrante */
			angolo = _verso (pt->max_x_t, pt->max_y_t);
			if (angolo < alfad) prima_x = TRUE;
		} 
		else {
			if (alfad < 180.0) {		/* 2" quadrante */
				angolo = _verso (pt->max_x_t, pt->min_y_t);
				if (angolo > alfad) prima_x = TRUE;
			} 
			else {
				if (alfad < 270.0) {		/* 3" quadrante */
					angolo = _verso (pt->min_x_t, pt->min_y_t);
					if (angolo < alfad) prima_x = TRUE;
				} 
				else {				/* 4" quadrante */
					angolo = _verso (pt->min_x_t, pt->max_y_t);
					if (angolo > alfad) prima_x = TRUE;
				}; /* 3" */
			}; /* 2" */
		}; /* 1" */
		   if (prima_x) { /* esce prima con x. Calcola ed esegue gli 
			   spostamenti parziali */
			   x_temp = (dx < 0) ? pt->min_x_t : pt->max_x_t; /* vado a max o min x */
			   passo_x = x_temp - pt->pos_x_t;
			   dx -= passo_x;
			   y_temp = pt->pos_y_t + (passo_y = passo_x * tan_alfa_stand);
			   dy -= passo_y;
			   line (pt->pos_x_t, pt->pos_y_t, x_temp, y_temp, DRAW_LINE);
			   /* lo riporto a dx (sin) */
			   pt->pos_x_t = (dx < 0) ? pt->max_x_t : pt->min_x_t;
			   pt->pos_y_t = y_temp;
		   }
		   else { /* esce prima con y. Calcola ed esegue gli spostamenti parziali */
			   y_temp = (dy < 0) ? pt->min_y_t : pt->max_y_t; /* vado a max o min y */
			   passo_y = y_temp - pt->pos_y_t;
			   dy -= passo_y;
			   x_temp = pt->pos_x_t + (passo_x = passo_y / tan_alfa_stand);
			   dx -= passo_x;
			   line (pt->pos_x_t, pt->pos_y_t, x_temp, y_temp, DRAW_LINE);
			   /* lo riporto giu' (su) */
			   pt->pos_y_t = (dy < 0) ? pt->max_y_t : pt->min_y_t;
			   pt->pos_x_t = x_temp;
		   };
	};	/* fine  while (_fuori) */
				/* traccio l'ultimo pezzo (che non va fuori) */
	line (pt->pos_x_t, pt->pos_y_t,
		pt->pos_x_t + dx, pt->pos_y_t + dy, DRAW_LINE);
	pt->pos_x_t += dx;
	pt->pos_y_t += dy;
}

/* ---------------------------------------------------------------------------
Traccia una linea dalle coordinate correnti incrementandole di quanto
specificato; effettua anche il controllo sui rangds in caso di campo chiuso
(E' separata in 6 casi distinti. Notare che dopo ognuno c'e' un return).
--------------------------------------------------------------------------- */
void spostati (double dx, double dy)
{
	double x, y;
	int fuori;
	/*A*/
	if ((dx == 0) && (dy == 0)) return ;
	/*B*/
	if (   (fuori = _fuori (x = pt->pos_x_t + dx, y = pt->pos_y_t + dy))
		&& (MatchKw (pt->flvideo, pc_CHIUSO))) {
		err1 (45);
		return ;
	};
	/*C*/
	if (MatchKw (pt->penupdown, pc_SU) && MatchKw (pt->flvideo, pc_CIRCOL)) {
		pt->pos_x_t = _dentro (x, pt->min_x_t, pt->max_x_t);
		pt->pos_y_t = _dentro (y, pt->min_y_t, pt->max_y_t);
		return ;
	};
	/*D*/
	if (MatchKw (pt->penupdown, pc_SU)) {	/* campo CHIUSO (e ! fuori) o APERTO */
		pt->pos_x_t = x;
		pt->pos_y_t = y;
		return ;
	};
	/*E*/
	if (fuori && MatchKw (pt->flvideo, pc_CIRCOL)) {
		spostati_circolare (dx, dy);
		return ;
	};
	/*F*/
	line (pt->pos_x_t, pt->pos_y_t, x, y, DRAW_LINE);
	if (fabs(x) < VERY_SMALL_REAL) x = 0.0;
	if (fabs(y) < VERY_SMALL_REAL) y = 0.0;
	pt->pos_x_t = x;
	pt->pos_y_t = y;
}

/* ---------------------------------------------------------------------------
Primitiva per assegnare la modalita' del campo a una delle tre previste:
circolare (wrap), chiuso (fence), aperto (window).
L' assegnazione avviene mediante l'uso di parole chiavi.
--------------------------------------------------------------------------- */
void ASCAMPO ()
{
	if ((! MatchKw (arg_1, pc_CHIUSO)) && (! MatchKw (arg_1, pc_APERTO)) && (! MatchKw (arg_1, pc_CIRCOL)))
		err_arg1 ();
	else { /*A*/
		if (_fuori (pt->pos_x_t, pt->pos_y_t) && MatchKw (arg_1, pc_CHIUSO))
			err1 (45);
		else { /*B*/
			intarta ();
			pt->flvideo = arg_1;
			if (MatchKw (pt->flvideo, pc_CIRCOL)) { /*C*/
				if (fuori_x)
					pt->pos_x_t = _dentro (pt->pos_x_t, pt->min_x_t, pt->max_x_t);
				if (fuori_y) { /*D*/
					pt->pos_y_t = _dentro (pt->pos_y_t, pt->min_y_t, pt->max_y_t);
				} /*D*/
			} /*C*/
			outtarta ();
		}; /*B*/
	} /*A*/
}


// extern "C" 
void WRAP ()
{
	arg_1 = pc_CIRCOL;
	ASCAMPO ();
}

// extern "C" 
void WINDOW ()
{
	arg_1 = pc_APERTO;
	ASCAMPO ();
}

// extern "C" 
void FENCE ()
{
	arg_1 = pc_CHIUSO;
	ASCAMPO ();
}

int convColor (node arg, int *pCol, unsigned char *pColors)
{
	int tipo = typ (arg);
	if (tipo == LIST) {
		TripleToRgb (pColors, arg);
		return 2;
	} else if (tipo == LBCD) {
		int iCol = num_int (arg);
//>000113
//		if ((iCol < 0) || (iCol > 3))
//			return -1;
		if (iCol < 0)
			return -1;
		iCol = iCol % 16;
//>
		*pCol = iCol;
		return 1;
	} else
		return -1;
}

/* ---------------------------------------------------------------------------
Comando per assegnare il colore della penna:
4 possibilita' (0 sta per il colore dello sfondo)
--------------------------------------------------------------------------- */
void ASCOL ()				/* RIVEDERE per EGA, VGA e HG */
{
	int formColor;
	int iColor;
	unsigned char colors[4];

	if ((formColor = convColor (arg_1, &iColor, &colors[0])) < 0) {
		err2 (5, arg_1);
		return;
	};

	intarta (); 		/* Cancella tarta	  */

#ifdef VECCHIO
	if (tipo_1 == LIST) {
		TripleToRgb (colors, arg_1);
		TurtleSetPenColor (nview, (DWORD *) &colors[0]);
	} else if (tipo_1 == LBCD) {
		int iCol = num_int (arg_1);
		if (iCol > 3) {
			err2 (5, arg_1);
			return;
		};
		TurtleSetDosPenColor (nview, iCol);
	}
#endif // VECCHIO

	if (formColor == 1)
		TurtleSetDosPenColor (nview, iColor);
	else {
		TurtleSetDosPenColor (nview, -1);
		TurtleSetPenColor (nview, (DWORD *) &colors[0]);
	}
	outtarta ();			/* Scrive tarta 	  */
}

/* ---------------------------------------------------------------------------
Comando per assegnare la direzione a TARTA
--------------------------------------------------------------------------- */
void ASDIR ()
{
	intarta ();
	asdir_ (arg_1);
	outtarta ();
}

/* ---------------------------------------------------------------------------
Comando per assegnare il colore dello sfondo   (16 possibilita').
--------------------------------------------------------------------------- */
void ASSFONDO ()			/* RIVEDERE per EGA, VGA e HG */
{
	/* Seleziona il colore sfondo */
	unsigned char bk_colors[4];
	intarta ();
	if (tipo_1 == LIST) {
		TripleToRgb (bk_colors, arg_1);
		TurtleSetBkColor (nview, (DWORD *) &bk_colors[0]);
		TurtleSetDosBkColor (nview, -1);
	} else if (tipo_1 == LBCD) {
		int iCol = num_int (arg_1);
//>000114
//		if (iCol > 15) {
//			err2 (5, arg_1);
//			return;
//		};
		iCol = iCol % 16;
//
		TurtleSetDosBkColor (nview, iCol);
	}
	/*> 971223
	unsigned char flood_colors[4];
	TurtleGetFloodColor (nview,  (DWORD *) &flood_colors[0]);
	TurtleSetFloodColor (nview,  (DWORD *) &bk_colors[0]);
	TurtleFloodFill (nview, xLogoToView (0.0), yLogoToView (0.0));
	TurtleSetFloodColor (nview,  (DWORD *) &flood_colors[0]);
	*/
#ifdef OVs
	if (! theApp.m_turtleViews[nview]->m_isOVs) {
#endif OVs
		_PULISCI ();
#ifdef OVs
	}
#endif OVs
	/*<*/
	outtarta ();
	TurtleSetModify(TRUE);
}

/* --------------------------------------------------------------------
Comando per assegnare il colore dello sfondo del testo su schermo TARTA
----------------------------------------------------------------------- */
void ASSFSCRIVI ()
{
	unsigned char colors[4];
	if (tipo_1 == LIST) {
		TripleToRgb (colors, arg_1);
		TurtleSetTextBkColor (nview, (DWORD *) &colors[0]);
		TurtleSetTextBkMode (nview, OPAQUE);
	} else
		TurtleSetTextBkMode (nview, TRANSPARENT);
}
void SFSCRIVI ()
{
	unsigned char colors[4];
	TurtleGetTextBkColor (nview, (DWORD *) &colors[0]);
	risultato = RgbToTriple (colors);
}

/* ---------------------------------------------------------------------------
Primitiva per far muovere in "avanti", cioe' nella direzione corrente,
di n_passi la tartaruga modificando le coordinate correnti e tenendo conto
dei stati della penna, del video, ecc.
--------------------------------------------------------------------------- */
void AVANTI ()
{
	intarta ();
	spostati (node2real (arg_1) * pt->senalfa,
		node2real (arg_1) * pt->cosalfa);
	outtarta ();
	TurtleSetModify(TRUE);
}

/* ---------------------------------------------------------------------------
Funzione per dare in output il valore corrente della modalita' di campo.
--------------------------------------------------------------------------- */
void CAMPO ()
{
	risultato = pt->flvideo;
}


int convPenMode (node arg)
{
	int mode = -1;
	if (MatchKw (arg, pc_PAINT))
		mode = 1;
	else if (MatchKw (arg, pc_INVERP))
		mode = 2;
	else if (MatchKw (arg, pc_CANCEP))
		mode = 3;
	return mode;
}

void applyPenMode (int mode)
{
	switch (mode) {
	case 1:		// paint
		TurtleSetDrawMode (nview, R2_COPYPEN);
		TurtleSetEraseFlag (nview, FALSE);
		break;
	case 2:		// reverse
		TurtleSetDrawMode (nview, R2_NOT);
		TurtleSetEraseFlag (nview, FALSE);
		break;
	case 3:		// erase
		TurtleSetDrawMode (nview, R2_COPYPEN);
		TurtleSetEraseFlag (nview, TRUE);
		break;
	}
}

void PR_SETPENMODE ()
{
	int mode = convPenMode (arg_1);
	if (mode < 0)
		err_arg1 ();
	else {
		pt->penmode = arg_1;
		applyPenMode (mode);
	}
}

// void PENPAINT ()				   /* Tarta "scrivente"    */
void PR_PENPAINT ()
{
//> 980911
//	intarta ();
//	pt->penmode = pc_PAINT;
//	TurtleSetDrawMode (nview, R2_COPYPEN);
//	outtarta ();
	pt->penupdown = pc_GIU;
	arg_1 = pc_PAINT;
	PR_SETPENMODE ();
//<
}

/* ---------------------------------------------------------------------------
pt->tipopen  viene ORato con il colore attuale (pt->colpen) per dire al GFX
se deve disegnare in XOR o no
--------------------------------------------------------------------------- */
// void INVERPENNA ()					 /* Tarta "invertitrice"   */
void PR_PENREVERSE ()
{
//> 980911
//	intarta ();
	pt->penupdown = pc_GIU;
//	pt->penmode = pc_INVERP;
//	TurtleSetDrawMode (nview, R2_NOT);
//	outtarta ();
	arg_1 = pc_INVERP;
	PR_SETPENMODE ();
//<
}

// void CANCEPENNA ()					 /* Tarta "cancellante"    */
void PR_PENERASE ()
{
//> 980911
//	unsigned char bk_colors[4];
//	intarta ();
	pt->penupdown = pc_GIU;
//	pt->penmode = pc_CANCEP;
//	TurtleSetDrawMode (nview, R2_COPYPEN);
//	TurtleGetBkColor (nview, (DWORD *) &bk_colors[0]);
//	TurtleSetPenColor (nview, (DWORD *) &bk_colors[0]);
//	outtarta ();
	arg_1 = pc_CANCEP;
	PR_SETPENMODE ();
//<
}

//void PENMODE ()
void PR_PENMODE ()
{
	risultato = pt->penmode;
}

/* ---------------------------------------------------------------------------
Funzione che riporta il colore corrente della penna
--------------------------------------------------------------------------- */
void COLORE ()
{
	int iCol = TurtleGetDosPenColor(nview);
	if (iCol >= 0) {
		risultato = int_num (iCol);
	} else {
		unsigned char colors[4];
		TurtleGetPenColor (nview, (DWORD *) &colors[0]);
		risultato = RgbToTriple (colors);
	}
}

/* ---------------------------------------------------------------------------
Comando per ruotare TARTA a destra (senso orario)
--------------------------------------------------------------------------- */
void DESTRA ()
{
	intarta ();
	asdir_ (consreal (pt->alfa + node2real (arg_1)));
	outtarta ();
}

/* ---------------------------------------------------------------------------
Funzione che riporta la direzione di TARTA
--------------------------------------------------------------------------- */
void DIR ()
{
	risultato = consreal (pt->alfa);
}

/* ---------------------------------------------------------------------------
Funzione che riporta la posizione corrente di TARTA sotto forma di lista
di ascissa e ordinata
--------------------------------------------------------------------------- */
void DOVE ()
{
	risultato = cons (consreal (pt->pos_x_t), 
		cons (consreal (pt->pos_y_t), NULLP, LIST), 
		LIST);
}

/* ---------------------------------------------------------------------------
pt->tipopen  viene ORato con il colore attuale (pt->colpen) per dire al GFX
se deve disegnare in XOR o no
--------------------------------------------------------------------------- */
void GIU () 						 /* Tarta "tracciante"	   */
{
	intarta ();
	pt->penupdown = pc_GIU;
	outtarta ();
}

/* ---------------------------------------------------------------------------
Primitiva per far muovere in "indietro", cioe' nella direzione opposta
a quella corrente, di n_passi la tartaruga modificando le coordinate 
correnti e tenendo conto dei stati della penna, del video, ecc.
--------------------------------------------------------------------------- */
void INDIETRO ()
{
	arg_1 = consreal( - node2real (arg_1));
	AVANTI ();
}


/* ---------------------------------------------------------------------------
Procedura per l'inizializzazione del mondo di tarta con dei valori e dei stati
defaults. E' eseguita una sola volta all'ingresso di LOGO nel modulo ILMAIN.C
--------------------------------------------------------------------------- */
void ini_tarta ()
{
	nview = -1;
	lato_tarta = CursorSide;		/* di tarta 	*/
	BCD_0 = consreal (0.0);
	BCD_1 = consreal (1.0);
	BCD_360 = consreal (360.0);
	if (DefaultTurtle[0] != '\0') {
		arg_1 = NULLP;	// 980717: aggiunto per compatibilità con KwAttr
		//	apri_tarta (mettip (logstr (DefaultTurtle)));
		apri_tarta (mettip (logstr (DefaultTurtle)), NULL, -1, -1, -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0);
	}
}

void TurtleExit () {
	int nv = nview, prima;
	struct dview *pv = pview;
	int dev;
	if (nv != -1) {
		while  ((prima = pv->prima) != -1) {	// va in testa
			nv = prima;
			pv = view[nv];
		}
		while  (nv != -1) {						// scorre in avanti
			dev = handle2dev (nv, O_TARTA);
			pv = view[nv];
			nv = pv->dopo;
			if (dev != -1)
				/* cede descrittore */
				free ((char *) pv);
		
		}
	}
}

/* ---------------------------------------------------------------------------
Funzione che riporta il colore del punto specificato mediante la lista
delle coordinate.
--------------------------------------------------------------------------- */
void LEGGIPUNTO ()			/* TESTARE */
{
	unsigned char colors[4];
	double x, y;
	if (! coordinate_double (arg_1, &x, &y)) {
		err_arg1 ();
		return ;
	};
	if (_fuori (x, y)) {
		if (MatchKw (pt->flvideo, pc_CHIUSO))
			err_arg1 ();
		else
			if (MatchKw (pt->flvideo, pc_CIRCOL)) {
				x = _dentro (x, pt->min_x_t, pt->max_x_t);
				y = _dentro (y, pt->min_y_t, pt->max_y_t);
			};
	};
	intarta ();
	TurtleGetPixel (i_tarta, xLogoToView (x), yLogoToView (y), (DWORD *) &colors[0]);
	risultato = RgbToTriple (colors);
	outtarta ();
}

/* ---------------------------------------------------------------------------
Comando per rendere TARTA (il simbolo) invisibile.
--------------------------------------------------------------------------- */
void NASTARTA ()
{
#ifdef OVs
	if (theApp.m_turtleViews[nview]->m_isOVs) {
		TurtleShowCursor (i_tarta, FALSE);
		pt->fltarta = 0;
	}
	else  {
#endif // OVs
		if (pt->fltarta == 1) {
			intarta (); 	/* Cancella tarta e eventuali comandi */
			pt->fltarta = 0;	/* Aggiorna flag di tarta */
			outtarta ();		/* rimette eventuali comandi */
		}
#ifdef OVs
	}
#endif // OVs
}

void _PULISCI ()
{
	unsigned char bk_colors[4];
	TurtleGetBkColor (nview, (DWORD *) &bk_colors[0]);
	TurtleFillRect (nview, pt->min_x, pt->min_y, pt->max_x-1, pt->max_y-1, (DWORD *) &bk_colors[0]);
	TurtleSetModify(TRUE);
}

/* ---------------------------------------------------------------------------
Comando per reinizializzare il solo contenuto dello schermo grafico,
TARTA esclusa.
--------------------------------------------------------------------------- */
void PULISCI ()
{
	intarta ();
#ifdef OVs
	TurtleClearScreen (i_tarta, FALSE);
#else // OVs
	_PULISCI ();
#endif // OVs
	outtarta ();
}

/* ---------------------------------------------------------------------------
Comando per reinizializzare il contenuto e il cursore (TARTA) dello
schermo grafico.
--------------------------------------------------------------------------- */
void PULISCISCHERMO ()		/* abr. PS */
{
	intarta ();
#ifdef OVs
	TurtleClearScreen (i_tarta, TRUE);
#else // OVs
	pt->pos_x_t = 0;				/* Coordinate (0,0) centro del video	 */
	pt->pos_y_t = 0;
	asdir_ (BCD_0); 				/* Riporta tarta orientata a nord		 */
	_PULISCI ();
#endif // OVs
	outtarta ();
}

/* ---------------------------------------------------------------------------
Comando che scrive il punto specificato mediante la lista delle coordinate;
tiene conto dello stato del campo (chiuso, aperto ecc.) ma non dello
stato della penna (scrive e/o cancella sempre).
La posizione di tarta non viene modificata.
--------------------------------------------------------------------------- */
void PUNTO ()			/* TESTARE */
{
	double x, y;
	if (! coordinate_double (arg_1, &x, &y)) err_arg1 ();
	else {
		if (_fuori (x, y)) {
			if (MatchKw (pt->flvideo, pc_CHIUSO)) {
				err_arg1 ();
				return ;
			};
			/* ELSE */
			if (MatchKw (pt->flvideo, pc_APERTO))
				return;
			/* ELSE */
			/* caso di video CIRCOLARE */
			x = _dentro (x, pt->min_x_t, pt->max_x_t);
			y = _dentro (y, pt->min_y_t, pt->max_y_t);
		};
		intarta ();
		pset (x, y);
		outtarta ();
	};
	TurtleSetModify(TRUE);
}

// Scrive un pixel del colore specificato nella posizione corrente
void PR_SETPIXEL ()
{
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];

	int iColor;
	COLORREF color;
	int formColor = convColor (arg_1, &iColor, (unsigned char *) &color);
	if (formColor < 0)
		{ err2 (5, arg_1); return; };
	if (formColor == 1) {
		if (turtle->m_dosPalette == 2)
			color = dosColors[iColor+16];
		else if (iColor == 0)
			color = turtle->GetBkColor();
		else
			color = dosColors[dosPalettes [turtle->m_dosPalette][iColor]];
	}

	intarta ();
	turtle->SetPixel(xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), color);
	outtarta ();
	TurtleSetModify(TRUE);
}

/* ---------------------------------------------------------------------------
Comando per colorare lo spazio intorno a TARTA delimitato da punti
di colore diverso rispetto a quello su cui si trova TARTA stessa.
MODIFICA: riempe con colore DFLT una figura con bordi di uguale colore.
NON considera dei confini i pixel di colore diverso da DFLT e ci
passa sopra ricoprendoli !!
--------------------------------------------------------------------------- */
	   void RIEMPI ()	/* viene dato una errore se il punto ha lo stesso colore
	   del limite - togliere msg errore del gfx dopo il debug */
	   /* viene sempre dato l'errore PAINT OVERFLOW.  Tenere
	   disattivati gli errori */
{
	intarta ();
	TurtleFloodFill (nview, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t));
	outtarta ();
	TurtleSetModify(TRUE);
}

/* ---------------------------------------------------------------------------
Funzione che riporta il colore corrente dello sfondo
--------------------------------------------------------------------------- */
void SFONDO ()
{
	int iCol = TurtleGetDosBkColor(nview);
	if (iCol >= 0) {
		risultato = int_num (iCol);
	} else {
		unsigned char colors[4];
		TurtleGetBkColor (nview, (DWORD *) &colors[0]);
		risultato = RgbToTriple (colors);
	}
}

/* ---------------------------------------------------------------------------
Comando per ruotare TARTA a sinistra (senso antiorario)
--------------------------------------------------------------------------- */
void SINISTRA ()
{
	intarta ();
	asdir_ (consreal (pt->alfa - node2real (arg_1)));
	outtarta ();
}

void SU ()							 /* Tarta "non tracciante" */
{
	intarta ();
	pt->penupdown = pc_SU;
	outtarta ();
}

/* ---------------------------------------------------------------------------
Predicato VERO se (finestra) TARTA e' visibile
--------------------------------------------------------------------------- */
void PR_TARTA_ ()
{
//	risultato = logval (pt->fltarta);
	int dev = currentWindow (O_TARTA);
	risultato = logval (dev != -1);
}

/* ---------------------------------------------------------------------------
Si sposta nella posizione di coordinate specificate mettendo tempora-
neamente la direzione della tartaruga verso il punto voluto.
(cio' e' necessario per "spostati_circolare" )
--------------------------------------------------------------------------- */
void _vaxy (double x, double y)
{
	double direz, temp;
	if ((direz = _verso (x, y)) == 400.0) return ;
	intarta ();
	temp = pt->alfa;
	pt->alfa = direz;
	spostati (x - pt->pos_x_t, y - pt->pos_y_t);
	pt->pos_x_t = x;			/* assegna in modo prec. le coord */
	pt->pos_y_t = y;
	pt->alfa = temp;
	outtarta ();
	TurtleSetModify(TRUE);
}

/* ---------------------------------------------------------------------------
Primitaiva che riporta la tartaruga alla posizione "tana" cioe' alle coordinate
[0 0] (centro dello schermo) e in direzione nord (0 gradi).
--------------------------------------------------------------------------- */
void VATANA ()
{
	intarta ();
	asdir_ (BCD_0);
	outtarta ();
	_vaxy (0.0, 0.0);
}

/* ---------------------------------------------------------------------------
Comando per muovere TARTA parallelamente all' asse X all' ascissa specificata
--------------------------------------------------------------------------- */
void VAX ()
{
	_vaxy (node2real (arg_1), pt->pos_y_t);
}

/* ---------------------------------------------------------------------------
Comando per muovere TARTA parallelamente all' asse Y all' ordinata 
specificata
------------------------------------------------------------------------- */
void VAY ()
{
	_vaxy (pt->pos_x_t, node2real (arg_1));
}

/* ---------------------------------------------------------------------------
Comando per portare TARTA nella posizione di coordinate specificate
--------------------------------------------------------------------------- */
// void VAXY ()
void PR_VAPOS ()
{
	double x, y;
	if (! coordinate_double (arg_1, &x, &y)) {err_arg1 (); return; };
	_vaxy (x, y); 
}

/* ---------------------------------------------------------------------------
Comando per portare TARTA nella posizione di coordinate specificate
--------------------------------------------------------------------------- */
void VAXY ()
{
	_vaxy (node2real (arg_1), node2real (arg_2)); 
}

/* ---------------------------------------------------------------------------
Funzione che riporta la direzione che dovrebbe assumere TARTA per muoversi
verso il punto le cui coordinate sono specificate nella lista argomento
--------------------------------------------------------------------------- */
void VERSO ()
{
	double x, y, temp;
	if (! coordinate_double (arg_1, &x, &y))
		err_arg1 ();
	else {
		if ((temp = _verso (x,y)) == 400.0) err_arg1 ();
		else risultato = consreal (temp);
	};
}

/* ---------------------------------------------------------------------------
Funzione che riporta l' ascissa di TARTA
--------------------------------------------------------------------------- */
void XCOR ()
{
	risultato = consreal (pt->pos_x_t);
}

/* ---------------------------------------------------------------------------
Funzione che riporta l' ordinata di TARTA
--------------------------------------------------------------------------- */
void YCOR ()
{
	risultato = consreal (pt->pos_y_t);
}

void tputs (int dev, char *s)
{
	int nView = _file [dev];
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
	TurtleTextOut (i_tarta, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), s); 
	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
}

void LABEL ()
{
	char text[MAXCHARS];
//	if (types[0] == LIST) MemScrivi (text, args[0]);
//	else getnam (text, args[0]);
	LogoToText (args[0], text);
	intarta ();
	TurtleTextOut (i_tarta, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), text); 
	outtarta ();
	TurtleSetModify(TRUE);
}

void PR_LABELDIM ()
{
	char text[MAXCHARS];
//	if (types[0] == LIST) MemScrivi (text, args[0]);
//	else getnam (text, args[0]);
	LogoToText (args[0], text);
	TurtleTextOut (i_tarta, -1, -1, text); 
	risultato = cons (consreal (textExtent.cx), 
		cons (consreal (textExtent.cy), NULLP, LIST), 
		LIST);
}

void PR_LABELDIMLOG ()
{
	char text[MAXCHARS];
	LogoToText (args[0], text);
	TurtleTextOut (i_tarta, -1, -1, text); 
	risultato = cons (consreal (widthViewToLogo (textExtent.cx)), 
		cons (consreal (heightViewToLogo (textExtent.cy)), NULLP, LIST), 
		LIST);
}

extern int horzRes, vertRes;
BOOL bScaledFont = FALSE;

int scaledFontHeight (double height)
{
	int scaledHeight;
//	if (bScaledFont && (horzRes != 1024))
//		scaledHeight = (scaledHeight * horzRes) / 1024;
	if (bScaledFont)
		scaledHeight = heightLogoToView (height);
	else
		scaledHeight = INTROUND (height);
	bScaledFont = FALSE;
	return scaledHeight;
}

int ListToFont (node tail, LOGFONT* lpLogFont)
{
	char FaceName[MAXCHARS];
	int n = lstlen (tail);
	if (n < 1) return FALSE;
	memset (lpLogFont, 0, sizeof(LOGFONT));

	MemScrivi (FaceName, car (tail));
	if (strlen (FaceName) >= LF_FACESIZE) return FALSE;
	strcpy (lpLogFont->lfFaceName, FaceName);
						tail = cdr (tail); if (! tail) return n;	
//> 991223
//	lpLogFont->lfHeight = num_int (car (tail));
	lpLogFont->lfHeight = scaledFontHeight (node2real (car (tail)));
//<
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfWidth = num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfOrientation = num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfWeight = num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfItalic = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfUnderline = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfStrikeOut = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfCharSet = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfOutPrecision = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfClipPrecision = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfQuality = (BYTE) num_int (car (tail));
						tail = cdr (tail); if (! tail) return n;	
	lpLogFont->lfPitchAndFamily = (BYTE) num_int (car (tail));
						return n;
}

node FontToList (LOGFONT* lpLogFont)
{
	if (! lpLogFont)
		return NULLP;
	return
		cons (_parse (lpLogFont->lfFaceName), 
		cons (int_num (lpLogFont->lfHeight),
		cons (int_num (lpLogFont->lfWidth),
		cons (int_num (lpLogFont->lfOrientation),
		cons (int_num (lpLogFont->lfWeight),
		cons (int_num (lpLogFont->lfItalic),
		cons (int_num (lpLogFont->lfUnderline),
		cons (int_num (lpLogFont->lfStrikeOut),
		cons (int_num (lpLogFont->lfCharSet),
		cons (int_num (lpLogFont->lfOutPrecision),
		cons (int_num (lpLogFont->lfClipPrecision),
		cons (int_num (lpLogFont->lfQuality),
		cons (int_num (lpLogFont->lfPitchAndFamily), NULLP,
		LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST);
}

void SETTEXTFONT ()
{
	LOGFONT *lpLogFont = (LOGFONT *) calloc(1, sizeof(LOGFONT));

	bScaledFont = FALSE;
	if (! ListToFont (arg_1, lpLogFont))
		{ free(lpLogFont); err2 (5, arg_1); return; }

	if (! TurtleSetTextFont (/*nview*/ i_tarta, lpLogFont)) {
		free(lpLogFont);
		err1 (38);
	}
}

void PR_SETTEXTFONTLOG ()
{
	LOGFONT *lpLogFont = (LOGFONT *) calloc(1, sizeof(LOGFONT));

	bScaledFont = TRUE;
	if (! ListToFont (arg_1, lpLogFont))
		{ free(lpLogFont); err2 (5, arg_1); return; }

	if (! TurtleSetTextFont (/*nview*/ i_tarta, lpLogFont)) {
		free(lpLogFont);
		err1 (38);
	}
}

void TEXTFONT ()
{
	risultato = FontToList(TurtleGetTextFont (/*nview*/ i_tarta));
}

void TEXTSCREEN ()
{
	arg_1 = pc_COMANDI;
	ASSCHERMO ();
}

void FULLSCREEN ()
{
	arg_1 = pc_TARTA;
	ASSCHERMO ();
}

void SPLITSCREEN ()
{
	arg_1 = pc_MISTO;
	ASSCHERMO ();
}

void REFRESH ()
{
	TurtleSetRefresh (/*nview*/ i_tarta, TRUE);
}

void NOREFRESH ()
{
	TurtleSetRefresh (/*nview*/ i_tarta, FALSE);
}

void _ZOOM (double new_factor)
{
	double current_factor, factor;

	current_factor = TurtleGetZoomFactor (i_tarta);
	if (new_factor == current_factor) return;
	factor = new_factor / current_factor;
	TurtleSetZoomFactor (i_tarta, new_factor);	// nuovo: 971223

#ifndef NEW_ZOOM
#define INTROUND(x) ((int) floor(x + 0.5))
	int xDim, yDim;
	int xPos, yPos;
	double smallFactor, xScale, yScale;
	
	smallFactor = (factor < 1.0) ? factor : (1.0 / factor);
	xScale = pt->max_x_t - pt->min_x_t;
	yScale = pt->max_y_t - pt->min_y_t;
#endif // NEW_ZOOM
	
	intarta ();
	
#ifdef NEW_ZOOM
	TurtleZoom (/*nview*/ i_tarta, factor, 0, 0, 0, 0);
#endif // NEW_ZOOM
	
	outtarta ();
}

int CTurtleView::OVsZoom (double factor) {

	CTurtleController* pController = (CTurtleController*) GetCtlr();

	if (factor <= 0) {
		int function = (int) -factor;
		pController->SpecialZoom(function);
	}
	else {
		int xPer, yPer;
		xPer = yPer = (int) (factor * 100);
		SetMagnification(xPer, yPer);
		CTurtleViewport::Invalidate();
	}
	return 0;
}

void ZOOM ()
{
	double new_factor = node2real (arg_1);
#ifdef OVs
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (pView->m_isOVs)
		pView->OVsZoom (new_factor);
	else
#endif OVs
		_ZOOM (new_factor);
}

void RESIZE ()
{
	node val, turtleName = _nome [handle2dev (/*nview*/ i_tarta, O_TARTA)];
	int xPos = -1, yPos = -1;
	int xDim = -1, yDim = -1;
	int xOrigin = -1, yOrigin = -1;
	double xScale = pt->max_x_t - pt->min_x_t + 1,	yScale = pt->max_y_t - pt->min_y_t + 1;
	
	GetXYFromProp (turtleName, pc_POS, &xPos, &yPos);
	GetXYFromProp (turtleName, pc_DIM, &xDim, &yDim);
	GetXYFromProp (turtleName, pc_OR, &xOrigin, &yOrigin);
	if (   ((val = _prop (turtleName, pc_SCALA)) != NULLVALUE)
		&& (typ (car (val)) == LBCD)
		&& (typ (car (cdr (val))) == LBCD))
	{
		xScale = node2real (car (val));
		yScale = node2real (car (cdr (val)));
	}
	
	TurtleResize (/*nview*/ i_tarta, xPos, yPos, xDim, yDim);
	
	if (xOrigin != -1) pt->min_x_t = - xOrigin;
	if (yOrigin != -1) pt->min_y_t = - yOrigin;
	
	pt->max_x_t = pt->min_x_t + xScale - 1;
	pt->max_y_t = pt->min_y_t + yScale - 1;
}

void SETFLOODCOLOR ()
{
	/* Seleziona il colore di riempimento */
	unsigned char flood_colors[4];
	if (tipo_1 == LIST) {
		if (arg_1 == NULLP)
			TurtleSetFloodTransparent (nview, TRUE);
		else {
			TurtleSetFloodTransparent (nview, FALSE);
			TripleToRgb (flood_colors, arg_1);
			TurtleSetFloodColor (/*nview*/ i_tarta, (DWORD *) &flood_colors[0]);
			TurtleSetDosFloodColor (nview, -1);
		}
	} else if (tipo_1 == LBCD) {
		int iCol = num_int (arg_1);
		iCol = iCol % 16;
		TurtleSetDosFloodColor (nview, iCol);
	}
}

void SETPENSIZE ()
{
	int size = rnum_int(_pri (arg_1));
	TurtleSetPenWidth (/*nview*/ i_tarta, size);
}

void SETPENPATTERN ()
{
#define PS_SOLID		0
#define PS_DASH 			1
#define PS_DOT				2
#define PS_DASHDOT			3
#define PS_DASHDOTDOT		4
#define PS_NULL 		5
#define PS_INSIDEFRAME		6
	int style = rnum_int(arg_1);
	TurtleSetPenStyle (/*nview*/ i_tarta, style);
}

void PENDOWNP ()
{
	risultato = logval (MatchKw (pt->penupdown, pc_GIU));
}

void PENCOLOR ()
{
	unsigned char colors[4];
	TurtleGetPenColor (/*nview*/ i_tarta, (DWORD *) &colors[0]);
	risultato = RgbToTriple (colors);
}

void FLOODCOLOR ()
{
	unsigned char colors[4];
	TurtleGetFloodColor (/*nview*/ i_tarta, (DWORD *) &colors[0]);
	risultato = RgbToTriple (colors);
}

void PENSIZE ()
{
	risultato = int_num (TurtleGetPenWidth (/*nview*/ i_tarta));
	risultato = cons (risultato, cons (risultato, NULLP, LIST), LIST);
}

void PENPATTERN ()
{
	risultato = int_num (TurtleGetPenStyle (/*nview*/ i_tarta));
}

void MOUSEON()
{
	TurtleMouseOn (/*nview*/ i_tarta, args[0], args[1], args[2], args[3], args[4]); 
}

void MOUSEOFF()
{
	TurtleMouseOff (/*nview*/ i_tarta); 
}

void MOUSEPOS()
{
	int xPos, yPos;
	TurtleGetMousePos (/*nview*/ i_tarta, &xPos, &yPos); 
	risultato = cons (consreal (xViewToLogo (xPos)), 
		cons (consreal (yViewToLogo (yPos)), NULLP, LIST), 
		LIST);
}

#ifdef OVs

// int _BITLOAD (int nView, int handle, int mode)
int _TURTLELOAD (int nView, int handle, int mode) // 040123
{
	int result;
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();

	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
//>040123
//	result = turtle->BitLoad (handle, mode);
	result = turtle->TurtleLoad (handle, mode);
//<040123

	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
	return result;
}

#else // OVs

// int _BITLOAD (int nView, int handle, int mode)
int _TURTLELOAD (int nView, int handle, int mode) // 040123
{
	int result;
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
//	result = TurtleBitLoad (nView, handle, mode);
	result = TurtleTurtleLoad (nView, handle, mode); // 040123
	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
	return result;
}

#endif // OVs

// int _BITSAVE (int nView, int handle, int mode)
int _TURTLESAVE (int nView, int handle, int mode) // 040123
{
	int result;
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
//	result = TurtleBitSave (nView, handle, mode);
	result = TurtleTurtleSave (nView, handle, mode); // 040123
	outtarta ();
	TurtleSetModify(FALSE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
	return result;
}

#ifdef OVs

// void BITLOAD()
void PR_TURTLELOAD() // 040123
{
	intarta ();
//	int error = TurtleBitLoad (i_tarta, arg_1, 0);
	int error = TurtleTurtleLoad (i_tarta, arg_1, 0); // 040123
	outtarta ();
	if (error)
		err2 (31, arg_1);
}

#else // OVs

// void BITLOAD()
void PR_TURTLELOAD() // 040123
{
	int dev;
	if ((dev = f_apri (arg_1, _O_RDONLY | _O_RAW, _S_IREAD)) <= 0)
		err2 (31, arg_1);
	else {
//		if (! _BITLOAD (i_tarta, _file [dev], 0)) /* errore */
		if (! _TURTLELOAD (i_tarta, _file [dev], 0)) /* errore */ // 040123
			err1 (38);
		f_chiudi (dev); 
	}
}

#endif // OVs

// void BITSAVE()
void PR_TURTLESAVE() // 040123
{
	int dev;
	if ((dev = f_apri (arg_1, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) < 0) {
		if (dev == -1) err2 (31, arg_1);
		else err1 (32);
	}
	else {
//		if (! _BITSAVE (i_tarta, _file [dev], 0)) /* errore */
		if (! _TURTLESAVE (i_tarta, _file [dev], 0)) /* errore */ // 040123
			err1 (38);
		f_chiudi (dev); 
	}
}

// come BITLOAD, ma con mode 1 anziche' 0
void RECU_DISEGNO ()
{
	int dev;
	if ((dev = f_apri (arg_1, _O_RDONLY | _O_RAW, _S_IREAD)) <= 0)
		err2 (31, arg_1);
	else {
//		if (! _BITLOAD (i_tarta, _file [dev], 1)) /* errore */
		if (! _TURTLELOAD (i_tarta, _file [dev], 1)) /* errore */ // 040123
			err1 (38);
		f_chiudi (dev); 
	}
}

// come BITSAVE, ma con mode 1 anziche' 0
void CONS_DISEGNO ()
{
	int dev;
	if ((dev = f_apri (arg_1, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) < 0) {
		if (dev == -1) err2 (31, arg_1);
		else err1 (32);
	}
	else {
//		if (! _BITSAVE (i_tarta, _file [dev], 1)) /* errore */ 
		if (! _TURTLESAVE (i_tarta, _file [dev], 1)) /* errore */ // 040123
			err1 (38);
		f_chiudi (dev); 
	}
}

//>040123 040124
int CTurtleView::BitLoad (int bitIndex, char *filename, DWORD imagetype)
{
	int result;
	// load the image
	CxImage image;
	result = image.Load(filename, imagetype);
	if (! result)
		return 0;
	// access the DIB
	BITMAPINFOHEADER* pDib = (BITMAPINFOHEADER*) image.GetDIB();
	// create the bitmap
	CDC* pDC = GetDC();
	// save reference in global array
	CBitmap* pBitmap = DibToBitmap (pDib, pDC, theApp.m_pPalette);
	theApp.m_pBitmap[bitIndex] = pBitmap;

	// da CTurtleView::BitCutCopy
	if (bitIndex == 0) {
		HBITMAP hBitmap =
			//			HBITMAP(m_pBitmap[bitIndex]);	// ESEMPIO di OPERATORE ??
			(HBITMAP) theApp.m_pBitmap[bitIndex]->m_hObject;

		HPALETTE hPalette = NULL;
		if (theApp.m_pPalette)
			hPalette = (HPALETTE) theApp.m_pPalette->m_hObject;
		//		HGLOBAL hDib = BitmapToSharedDib (hBitmap);
		HGLOBAL hDib = BitmapToSharedDib (pDC, hBitmap); // 980226
		result =
			//			SetClipboardData(hBitmap);		// Bitmap data handle
			//			SetClipboardData(hDib);			// DIB data handle
			SetClipboardData(hBitmap,
			hPalette,
			hDib);
		GlobalUnlock(hDib);				// completa BitmapToSharedDib!!
		theApp.m_pBitmap[bitIndex] = NULL;
	}

	return 1;
}

int TurtleBitLoad (int nview, int bitIndex, char *filename, DWORD imagetype)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->BitLoad (bitIndex, filename, imagetype);
}

void PR_BITLOAD()
{
	char filename [MAXCHARS], format[MAXCHARS];
	getnam (filename, arg_1);
	CString csFilename, csFormat;
	csFilename = CString(filename);
	if (n_argomenti == 1) {
		int dotPos = csFilename.Find('.');
		if (dotPos <= 0) { err_arg1 (); return; }
		csFormat = csFilename.Mid(dotPos+1);
	}
	else {
		getnam (format, arg_2);
		csFormat = CString(format);
	}
	// encode the image type
	DWORD imagetype = 0;
	if (csFormat.CompareNoCase ("bmp") == 0)
		imagetype = CXIMAGE_FORMAT_BMP;
	else if (csFormat.CompareNoCase ("gif") == 0)
		imagetype = CXIMAGE_FORMAT_GIF;
	else if ((csFormat.CompareNoCase ("jpg") == 0) || ( csFormat.CompareNoCase ("jpeg") == 0))
		imagetype = CXIMAGE_FORMAT_JPG;
	else {
		if (n_argomenti == 1) err_arg1 (); else err_arg2 ();
		return;
	};
	// load image from file
	int result = TurtleBitLoad (/*nview*/ i_tarta, pt->bitIndex, filename, imagetype); 
	if (! result)
		err2 (31, arg_1);

}

//>040130
int CTurtleView::BitMultiSave (char *filename, DWORD imagetype, int nLoops, node bitIndexList, node delayList)
{
#define FRAME_DELAY 100*1/6 // centesimi di secondo
	FILE* hFile = fopen(filename, "w+b");
	if (! hFile)
		return 0;
	// create the destination image
	CxImageGIF multiimage;
	multiimage.SetLoops(nLoops);
	CxImage *pimage[16];
	int n = lstlen(bitIndexList);
	int result;
	int i = 0, bitIndex, frameDelay = 0;
	// loop on the source images
	while (bitIndexList) {
		bitIndex = rnum_int(car(bitIndexList)); // da verificare limite ?
		if (delayList)
			frameDelay = rnum_int(car(delayList)); // verificare range ?
		if (bitIndex == 0) {
			theApp.m_pBitmap[0] = NULL;
			CBitmap ClipboardBitmap;
			CBitmap* pClipboardBitmap;
			HBITMAP hClipboardBitmap =
				GetClipboardData ();			// APRE CLIPBOARD !!!
			//	gets a temporary CBitmap object from the Clipboard Bitmap 	
			if (hClipboardBitmap) {
				pClipboardBitmap =
					ClipboardBitmap.FromHandle(hClipboardBitmap);
				theApp.m_pBitmap[0] = pClipboardBitmap;
			}
			else {
				BOOL result = ::CloseClipboard();	// CHIUDE CLIPBOARD !!!
				return -1;
			}
		}
		// test existence of indexed bitmap
		if (! theApp.m_pBitmap[bitIndex])
				return 0;
		// get info on the image
		CBitmap *pBitmap = theApp.m_pBitmap[bitIndex];
		theApp.m_pBitmap[0] = NULL;
		struct tagBITMAP bitmap;
		result = pBitmap->GetBitmap(&bitmap);
		BYTE bmBitsPixel = bitmap.bmBitsPixel;
		HBITMAP hBitmap =
			(HBITMAP) pBitmap->m_hObject;
		HPALETTE hPalette = NULL;
		if (theApp.m_pPalette)
			hPalette = (HPALETTE) theApp.m_pPalette->m_hObject;
		// create one source image
		pimage[i] = new(CxImage);
		pimage[i]->SetFrameDelay(frameDelay);
		pimage[i]->CreateFromHBITMAP(hBitmap, hPalette);
		int newBitsPixel = (bmBitsPixel < 8) ? bmBitsPixel : 8;
		if ((imagetype == CXIMAGE_FORMAT_GIF) && (bmBitsPixel < 24))
			pimage[i]->DecreaseBpp(newBitsPixel, 0, NULL);
		bitIndexList = cdr(bitIndexList);
		if (delayList)
			delayList = cdr(delayList);
		++i;
	}
	// create and save the multiple image
	multiimage.SetEncodeOption(2); // 0 = RLE (default), 2 = LZW
	result = multiimage.Encode(hFile, pimage, n);
	if (! result)
		return 0;
	fclose(hFile);
	return 1;
}
//<040130

int TurtleBitMultiSave (int nview, char *filename, DWORD imagetype, int nLoops, node bitIndexList, node delayList)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->BitMultiSave (filename, imagetype, nLoops, bitIndexList, delayList);
}

int CTurtleView::BitSave (int bitIndex, char *filename, DWORD imagetype)
{
	int result;
	// da CTurtleView::BitPaste
	if (bitIndex == 0) {
		theApp.m_pBitmap[0] = NULL;
		CBitmap ClipboardBitmap;
		CBitmap* pClipboardBitmap;
		HBITMAP hClipboardBitmap =
			GetClipboardData ();			// APRE CLIPBOARD !!!
		//	gets a temporary CBitmap object from the Clipboard Bitmap 	
		if (hClipboardBitmap) {
			pClipboardBitmap =
				ClipboardBitmap.FromHandle(hClipboardBitmap);
			theApp.m_pBitmap[0] = pClipboardBitmap;
		}
		else {
			BOOL result = ::CloseClipboard();	// CHIUDE CLIPBOARD !!!
			return -1;
		}
	}
	// test existence of indexed bitmap
	if (! theApp.m_pBitmap[bitIndex])
			return 0;
	// get info on the image
	CBitmap *pBitmap = theApp.m_pBitmap[bitIndex];
	theApp.m_pBitmap[0] = NULL;
	struct tagBITMAP bitmap;
	result = pBitmap->GetBitmap(&bitmap);
	BYTE bmBitsPixel = bitmap.bmBitsPixel;
	HBITMAP hBitmap =
		(HBITMAP) pBitmap->m_hObject;
	HPALETTE hPalette = NULL;
	if (theApp.m_pPalette)
		hPalette = (HPALETTE) theApp.m_pPalette->m_hObject;
	// create the image
	CxImage image;
//	CxImageGIF image;
	image.CreateFromHBITMAP(hBitmap, hPalette);
	int newBitsPixel = (bmBitsPixel < 8) ? bmBitsPixel : 8;
	if ((imagetype == CXIMAGE_FORMAT_GIF) && (bmBitsPixel < 24))
		image.DecreaseBpp(newBitsPixel, 0, NULL);
	// save the image
	if (imagetype == CXIMAGE_FORMAT_GIF)
		image.SetEncodeOption(2); // 0 = RLE (default), 2 = LZW
/*>
	FILE* hFile = fopen(filename, "w+b");
	if (! hFile)
		return 0;
	result = image.Encode(hFile);
	fclose(hFile);
*/
	result = image.Save(filename, imagetype);
/*<*/
	if (! result)
		return 0;

	return 1;
}

int TurtleBitSave (int nview, int bitIndex, char *filename, DWORD imagetype)
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	return turtle->BitSave (bitIndex, filename, imagetype);
}

void PR_BITSAVE()
{
	char filename [MAXCHARS], format[MAXCHARS];
	int nLoops = 0;
	node indexList = NULLP, delayList = NULLP;
	getnam (filename, arg_1);
	CString csFilename, csFormat;
	csFilename = CString(filename);
	if (n_argomenti == 1) {
		int dotPos = csFilename.Find('.');
		if (dotPos <= 0) { err_arg1 (); return; }
		csFormat = csFilename.Mid(dotPos+1);
	}
	else {
		getnam (format, arg_2);
		csFormat = CString(format);
	}
	// encode the image type
	DWORD imagetype = 0;
	if (csFormat.CompareNoCase ("bmp") == 0)
		imagetype = CXIMAGE_FORMAT_BMP;
	else if (csFormat.CompareNoCase ("gif") == 0)
		imagetype = CXIMAGE_FORMAT_GIF;
	else if ((csFormat.CompareNoCase ("jpg") == 0) || ( csFormat.CompareNoCase ("jpeg") == 0))
		imagetype = CXIMAGE_FORMAT_JPG;
	else {
		if (n_argomenti == 1) err_arg1 (); else err_arg2 ();
		return;
	};
//>040130
//	int result = TurtleBitSave (/*nview*/ i_tarta, pt->bitIndex, filename, imagetype); 
	int result;
	if (n_argomenti <= 2)
		result = TurtleBitSave (/*nview*/ i_tarta, pt->bitIndex, filename, imagetype);
	else { // multi-immagine
		if (imagetype != CXIMAGE_FORMAT_GIF) // solo un GIF puo' essere multiimmagine
			{ err1 (38); return; }
		// terzo argomento: loop count
		nLoops = rnum_int(arg_3);
		// quarto argomento: lista degli indici di immagine
		indexList = args[3];
		// controlla numero indici
		int nIndex = lstlen(indexList);
		if ((nIndex < 2) || (nIndex > 16))
			{ errore (5, funzione, args[2]); return; }
		// controlla valore degli indici
		node index = indexList;
		while (index) {
			if (rnum_int(car(index)) > 16) // da verificare limite
				{ errore (5, funzione, args[3]); return; }
			index = cdr(index);
		};
		if (n_argomenti > 4) {
			// quinto argomento: lista dei delay delle immagini
			delayList = args[4];
			int nDelay = lstlen(delayList);
			if (nDelay != nIndex)
				{ errore (5, funzione, args[4]); return; }
		};
		result = TurtleBitMultiSave (/*nview*/ i_tarta, filename, imagetype, nLoops, indexList, delayList);
	};
//<040130
	if (! result)
		err1 (38);

}
//<040123 040124

void CLEARPALETTE()
{
	TurtleClearPalette (/*nview*/ i_tarta); 
}

void TURTLEWINDOW()
{
	int dev;
	if (i_tarta == -1)
		risultato = NULLP;
	else {
		dev = handle2dev (i_tarta, O_TARTA); 
		risultato = _nome [dev];
	}
}

void _BITCUTCOPY (int nView, int bitIndex, int isCut, int x, int y, int width, int height)
{
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
	TurtleBitCutCopy (nview, bitIndex, isCut, x, y, width, height); 
	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
}

void BITCUT()
{
	int width = widthLogoToView (node2real (arg_1));
	int height = heightLogoToView (node2real (arg_2));
	intarta ();
	TurtleBitCutCopy (/*nview*/ i_tarta, pt->bitIndex, TRUE, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), width, height); 
	outtarta ();
}

void BITCOPY()
{
	int width = widthLogoToView (node2real (arg_1));
	int height = heightLogoToView (node2real (arg_2));
	intarta ();
	TurtleBitCutCopy (/*nview*/ i_tarta, pt->bitIndex, FALSE, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), width, height); 
	outtarta ();
}

void _BITPASTE (int nView, int bitIndex)
{
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
	TurtleBitPaste (nView, bitIndex, pt->bitMode, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t)); 
	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
}

void BITPASTE()
{
	intarta ();
	TurtleBitPaste (/*nview*/ i_tarta, pt->bitIndex, pt->bitMode, xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t)); 
	outtarta ();
}

void BITFIT()
{
	int width = widthLogoToView (node2real (arg_1));
	int height = heightLogoToView (node2real (arg_2));
	intarta ();
	TurtleBitFit (/*nview*/ i_tarta, pt->bitIndex, width, height); 
	outtarta ();
}

void SETBITINDEX()
{
	pt->bitIndex = rnum_int (arg_1);
}

void BITINDEX()
{
	risultato = consreal (pt->bitIndex);
}

void SETBITMODE()
{
	pt->bitMode = rnum_int (arg_1);
}

void BITMODE()
{
	risultato = consreal (pt->bitMode);
}

void _BITBLOCK (int nView, int x, int y, int width, int height)
{
	unsigned char flood_colors[4];
	int save_i_tarta = i_tarta; i_tarta = nView;
	int save_nv = nview; nview = nView;
	struct dview* save_pt = pt; pt = view[nView];
	intarta ();
	TurtleGetFloodColor (nView, (DWORD *) &flood_colors[0]);
//>
//	TurtleFillRect (nView, x, y, x+width-1, y-height+1, (DWORD *) &flood_colors[0]); 
	TurtleFillRect (nView, x, y-height+1, x+width-1, y, (DWORD *) &flood_colors[0]); 
//<
	outtarta ();
	TurtleSetModify(TRUE);
	pt = save_pt;
	nview = save_nv;
	i_tarta = save_i_tarta; 
}

void BITBLOCK()
{
//	unsigned char flood_colors[4];
	int width = widthLogoToView (node2real (arg_1));
	int height = heightLogoToView (node2real (arg_2));
	int x1 = xLogoToView (pt->pos_x_t); 
	int y1 = yLogoToView (pt->pos_y_t); 
//	intarta ();
//	TurtleGetFloodColor (/*nview*/ i_tarta, (DWORD *) &flood_colors[0]);
//	TurtleFillRect (/*nview*/ i_tarta, x1, y1, x1+width-1, y1-height+1, (DWORD *) &flood_colors[0]); 
//	outtarta ();
//>980508
//	_BITBLOCK (/*nview*/ i_tarta, x1, y1, x1+width-1, y1-height+1);
	_BITBLOCK (/*nview*/ i_tarta, x1, y1, width, height);
//<
}

// stampa schermo tarta, limitatamente all'area attiva
void PR_TURTLEPRINT()
{
//	TurtleBitPrint (i_tarta);
	TurtlePrint (i_tarta);
}

// stampa blocco di dimensioni specificate, con origine in pos. tarta
// (gia' stampa di schermo tarta)
void BITPRINT()
{
//	TurtleBitPrint (i_tarta);
	int width = widthLogoToView (node2real (arg_1));
	int height = heightLogoToView (node2real (arg_2));
	intarta ();
	TurtleBitPrint (i_tarta,
		xLogoToView (pt->pos_x_t), yLogoToView (pt->pos_y_t), width, height); 
	outtarta ();
}

void _ResetActiveArea (int nView)
{
	struct dview* pView = view [nView];
	TurtleSetActiveArea (nView, pView->min_x_t, pView->min_y_t, pView->max_x_t, pView->max_y_t);
}

void RESETACTIVEAREA ()
{
	_ResetActiveArea (i_tarta);
}

void SETACTIVEAREA()
{
	node val;
	double xLowArea, yLowArea, xHighArea, yHighArea;
	if (lstlen (val = arg_1) != 4) err_arg1 ();
	xLowArea = node2real (car (val)); val = cdr (val);
	yLowArea = node2real (car (val)); val = cdr (val);
	xHighArea = node2real (car (val)); val = cdr (val);
//	yLowArea = node2real (car (val)); val = cdr (val);
	yHighArea = node2real (car (val));
//	TurtleSetActiveArea (/*nview*/ i_tarta, xLowArea, xHighArea, yLowArea, yHighArea);
	TurtleSetActiveArea (/*nview*/ i_tarta, xLowArea, yLowArea, xHighArea, yHighArea);
}

void ACTIVEAREA()
{
	double xLowArea, yLowArea, xHighArea, yHighArea;
//	TurtleGetActiveArea (/*nview*/ i_tarta, &xLowArea, &xHighArea, &yLowArea, &yHighArea);
	TurtleGetActiveArea (/*nview*/ i_tarta, &xLowArea, &yLowArea, &xHighArea, &yHighArea);
	risultato = cons (consreal (xLowArea), 
		cons (consreal (yLowArea), 
		cons (consreal (xHighArea), 
//		cons (consreal (yLowArea), 
		cons (consreal (yHighArea),
		NULLP, LIST), LIST), LIST), LIST);
}

void ARC()
{
	double delta = node2real (arg_1);
	double radius = node2real (arg_2);
	double x1, y1, x2, y2, x3, y3, x4, y4;
	double x, y; 
	double alfa;
	node updown; 
	
	intarta ();
	updown = pt->penupdown;
	alfa = pt->alfa;
	pt->penupdown = pc_SU;
	
	x = pt->pos_x_t; y = pt->pos_y_t;
	
	x1 = pt->pos_x_t - radius;
	y1 = pt->pos_y_t - radius;
	x2 = pt->pos_x_t + radius;
	y2 = pt->pos_y_t + radius;
	
	spostati (radius * (- pt->senalfa),
		radius * (- pt->cosalfa));
	x4 = pt->pos_x_t;
	y4 = pt->pos_y_t;
	
	pt->pos_x_t = x; pt->pos_y_t = y;
	asdir_ (consreal (pt->alfa + delta));
	spostati (radius * (- pt->senalfa),
		radius * (- pt->cosalfa));
	x3 = pt->pos_x_t;
	y3 = pt->pos_y_t;
	
	TurtleArc (/*nview*/ i_tarta, xLogoToView (x1), yLogoToView (y1), xLogoToView (x2), yLogoToView (y2),
		xLogoToView (x3), yLogoToView (y3), xLogoToView (x4), yLogoToView (y4));
	
	pt->penupdown = updown;
	pt->pos_x_t = x; pt->pos_y_t = y;
	asdir_ (consreal (alfa));
	outtarta ();
	TurtleSetModify(TRUE);
}

void CENTRA ()
{
#ifdef NEW_CENTER
	TurtleCenter (i_tarta, -1, -1);
#else // NEW_CENTER
	TurtleCenter (i_tarta, 0, 0);
#endif // NEW_CENTER
}

void CTurtleView::SetKeybState (int state)
{
	m_keybState = state;
}

void TurtleKeybOn (int nview, node keyDown, node keyUp)
{ 
	CTurtleView* turtle;
	DlgEntry entry;
	turtle = theApp.m_turtleViews[nview];
	turtle->SetKeybState(TRUE);
	if (entry = WinToEntry (Dialogs, (CWnd *)turtle)) {
		node setup = GetSetup (entry);
		_setitem (setup, 6, keyDown);
		_setitem (setup, 7, keyUp);
		PutSetup(entry, setup);
	}
	ASSERT(entry);
}

void TurtleKeybOff (int nview)
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	turtle->SetKeybState(FALSE);
}

unsigned char vKeyToDos0 [], vKeyToDos1 [], vKeyToDos2 [];

UINT CTurtleView::GetKey ()
{
	BYTE KeybState[256];	// array to receive status data
	int result;
	result = GetKeyboardState(&KeybState[0]);
	ASSERT(result);
	UINT uScanCode = m_keybFlags & 0xFF;
	WORD ascii;
	UINT uFlags = 1;
	UINT key = 0;
	result = ToAscii(
		m_key	,		// virtual-key code
		uScanCode,		// scan code
		&KeybState[0],	// key-state array
		&ascii,			// buffer for translated key
		uFlags			// active-menu flag
	);
	if (result > 0)
		key = ascii;
	else {	// emulazione Superlogo
//>990513
//		key = m_key;
		BOOL isShift = KeybState[VK_SHIFT] & 0x80;
		BOOL isControl = KeybState[VK_CONTROL] & 0x80;
		int dosKey;
		if (isShift)
			dosKey = vKeyToDos1 [m_key];
		else if (isControl)
			dosKey = vKeyToDos2 [m_key];
		else
			dosKey = vKeyToDos0 [m_key];
		if (dosKey == 0)
			key = m_key;
		else
			key = 256 + dosKey;
//<
	}
	return key;
}

UINT TurtleGetKey (int nview)
{ 
	CTurtleView* turtle;
	turtle = theApp.m_turtleViews[nview];
	return turtle->GetKey();
}

void PR_KBON()
{
	TurtleKeybOn (i_tarta, args[0], /*args[1]*/ NULLP); 
}

void PR_KBOFF()
{
	TurtleKeybOff (i_tarta); 
}

void PR_KBVAL()
{
	UINT key = TurtleGetKey (i_tarta);
//	*pRisultato = consreal ((double) key);
	risultato = consreal ((double) key);
}

/*	ILTARTA.C  */

void CTurtleView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	DlgEntry entry;
	node callBack;
	m_key = nChar;
	m_keybFlags = nFlags;
//>990513
	node KeyVal;
//	if (m_keybState) {
	if (m_keybState && (nChar != VK_SHIFT) && (nChar != VK_CONTROL)) {
//<
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (6, callBack));
			if (callBack)
				NestedExec(callBack);
		}
		ASSERT(entry);
	}
//>990513-14 : tasti programmati
	else if (   (nChar != VK_SHIFT) && (nChar != VK_CONTROL)
			 && ((KeyVal = KeyProg (GetKey ())) != S_VUOTA)
			 && (typ (KeyVal) == LIST))
		NestedExec (KeyVal);
//<
	else
//>990513
		if (nChar != VK_F1)
//<
			CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTurtleView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
		
	DlgEntry entry;
	node callBack;
	m_key = nChar;
	m_keybFlags = nFlags;
//>990513
//	if (m_keybState) {
	if (m_keybState && (nChar != VK_SHIFT) && (nChar != VK_CONTROL))
	{
//<
		if (entry = WinToEntry (Dialogs, this)) {
			callBack = GetSetup (entry);
			callBack = (_ele (7, callBack));
			if (callBack)
				NestedExec(callBack);
		}
		ASSERT(entry);
	}
	else
//>990513
		if (nChar != VK_F1)
//<
			CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void TAVOLOZZA ()
{
	risultato = int_num (TurtleGetDosPalette (nview));
}
 
int convDosPalette (node arg) {
	int iPal = rnum_int (arg);
//>000112
//	if ((iPal < 0) || (iPal > 1))
	if ((iPal < 0) || (iPal > 2))	// palette 2 significa compatibilità MswLogo
//<
		iPal = -1;
	return iPal;
}

void ASTAVOLOZZA ()
{
//	int iPal = rnum_int (arg_1);
//	if (iPal > 1)
	int iPal = convDosPalette (arg_1);
	if (iPal < 0)
		err2 (5, arg_1);	// non funziona controllo generale ?
	else
		TurtleSetDosPalette (nview, iPal);
}

void PENNA ()
{
	COLORE ();
	node colPenna = risultato;
	TAVOLOZZA ();
	node dosPalette = risultato;
	risultato =
		cons (pt->penupdown,
			cons (colPenna,
				cons (dosPalette,
					cons (pt->penmode, NULLP, LIST),
				LIST),
			LIST),
		LIST);
}

node convUpDown (node arg)
{
	if (arg == pc_GIU_SENZA)		/* sinonimo   */
		return pc_GIU;
	else if ((arg == pc_SU) || (arg == pc_GIU))
		return arg;
	else return NULL;
}

void ASPENNA ()
{
	int n_pars = lstlen (arg_1);			/* Lung.lista */
	node UpDown;
	int formColor, iCol, iPal;
	int penMode;
	unsigned char colors[4];

	if ((n_pars < 1) || (n_pars > 4)) {
		err_arg1 ();
		return;
	}

#define pen(i) (_ele(i, arg_1))

	if (	((UpDown = convUpDown (pen(1))) == NULL)
		||	((n_pars > 1) && ((formColor = convColor (pen(2), &iCol, &colors[0]))) < 0)
		||	((n_pars > 2) && ((iPal = convDosPalette (pen(3))) < 0))
		||	((n_pars > 3) && ((penMode = convPenMode (pen(4))) < 0))
	) {
		err_arg1 ();
		return;
	}
	else {
		intarta ();							/* Cancella TARTA */
		pt->penupdown = UpDown;				/* 1" el: UP/DOWN  */
		if (n_pars >= 3)					/* 3" el: ev. palette DOS */
			TurtleSetDosPalette (nview, iPal);
		if (n_pars >= 2) {					/* 2" el: colore penna */
			if (formColor == 1)
				TurtleSetDosPenColor (nview, iCol);
			else
				TurtleSetPenColor (nview, (DWORD *) &colors[0]);
		}
		if (n_pars >= 4) {					/* 4" el: modo penna */
			pt->penmode = pen(4);
			applyPenMode (penMode);
		}
		outtarta ();						/* Riscrive TARTA */
	}
}

void PR_CREATARTA ()
{

	node lDevName = arg_1;
	int dev = nome2dev (lDevName);
	if (dev >= MIN_DEV)
		err2 (23, lDevName);		// .. gia' aperto
	else {
	arg_1 = arg_2;
		if (apri_tarta (lDevName, NULL,
				-1, -1, -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0) == -1)
		err2 (31, lDevName);		// non riesco ad aprire ..
	}
}

void PR_TARTA ()
{
	if (i_tarta == -1)
		risultato = NULLP;
	else
		risultato = dev2nome (view_to_devtarta (i_tarta));
}

void PR_CHIUDITARTA ()
{
	if (i_tarta == -1) {
	}
	else
		chiudi_tarta (i_tarta);
}

// come menu Composizione-Riempi Selezione
void PR_RIEMPISELEZIONE ()
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->OnEditFill ();
}

// come menu Composizione-Seleziona-Area Attiva (Tarta)
void PR_SELEZIONAREATTIVA () 
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->OnEditSelectarea ();
}

// come menu File-Area Attiva.. Da Selezione (Tarta)
void PR_ATTIVASELEZIONE ()
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->SelectionToActiveArea ();
}

// come menu Vista-Centra (Tarta)
void PR_CENTRA ()
{
	CTurtleView* turtle = theApp.m_turtleViews[nview];
	turtle->OnViewCenter ();
}


unsigned char vKeyToDos0 [] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 00
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 10
 0,73,81,79,71,75,72,77,80, 0, 0, 0, 0,82,83, 0,	// 20: page,end,frecce,ins,del,home
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 30
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 40
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 50
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 60
59,60,61,62,63,64,65,66,67,68, 0, 0, 0, 0, 0, 0,	// 70: F1-F10
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 80
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 90
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// A0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// B0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// C0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// D0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// E0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// F0
};

unsigned char vKeyToDos1 [] = {		// con Shift key
 0, 0, 0, 0, 0, 0, 0, 0, 0,15, 0, 0, 0, 0, 0, 0,	// 00: Shift Tab
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 10
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 20
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 30
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 40
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 50
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 60
84,85,86,87,88,89,90,91,92,93, 0, 0, 0, 0, 0, 0,	// 70: Shift F1-F10
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 80
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 90
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// A0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// B0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// C0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// D0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// E0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// F0
};

unsigned char vKeyToDos2 [] = {		// con Control key
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 00
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 10
0,132,118,117,119,115,72,116,0,0,0,0,0, 0, 0, 0,	// 20: page,end,frecce,home
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 30
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 40
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 50
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 60
94,95,96,97,98,99,100,101,102,103,0,0,0,0, 0, 0,// 70: Ctrl F1-F10
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 80
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 90
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// A0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// B0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// C0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// D0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// E0
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// F0
};


// ---------
#ifdef OVs
BOOL isValidComp (CODComponent* pComp, int testChildCount)
{
	if (! pComp)
		return FALSE;
	if (! AfxIsValidAddress((void*) pComp, 8, TRUE))
		return FALSE;
	ODComponentId lID = pComp->GetID();
	int childCount = pComp->GetChildCount();
	if (   (testChildCount != -1)
		&& (testChildCount != childCount))
		return FALSE;
	CString strType = pComp->GetType();
	CString strName = pComp->GetName();
	return TRUE;
}

node CODComponentSet2LogoList (CODComponentSet* pSet)
{
	node lista = NULLP;
	int nComps = pSet->GetSize();
	CODComponent* pComp;
	ODComponentId lID;
	int i;
	for (i=nComps-1; i>=0; i--) {
		pComp = pSet->GetAt(i);
		lID = pComp->GetID();
		lista = cons (long_num(lID), lista, LIST);
	}
	return lista;
}

void UpdateParent (CODComponent* pComp, CTurtleModel* pModel)
{
	return;
	CODComponent* pParent = pComp->GetParent();
	if (pParent) {
		ODComponentId lID = pParent->GetID();
		if (pModel->GetComponent(lID))
			pModel->UpdateComponent(pParent, OD_UPDATE_OLD_AND_NEW_POS);
		else
			UpdateParent (pParent, pModel);
	}
}

#ifdef NEW_PROP

// quando il componente non è visualizzato (aggiunto al modello)
// sembra non possedere ancora le proprietà in questione !!
void CTurtleView::SetComponentProperties (CODComponent* pComp)
{
	pComp->AddRef();
	BOOL ok;

	CODLineProperties* pPropLine = (CODLineProperties*) pComp->GetProperty(OD_PROP_LINE);
	if (pPropLine != NULL) {
		ok = pComp->SetValue(OD_PROP_LINE + OD_LINE_WIDTH, m_penWidth);
		ok = pComp->SetValue(OD_PROP_LINE + OD_LINE_STYLE, m_penStyle);
		ok = pComp->SetValue(OD_PROP_LINE + OD_LINE_COLOR, m_penColor);
		ok = pComp->SetValue(OD_PROP_LINE + OD_LINE_TRANSPARENT, (m_penWidth == 0));
	}

	CODFillProperties* pPropFill = (CODFillProperties*) pComp->GetProperty(OD_PROP_FILL);
	if (pPropFill != NULL) {
		ok = pComp->SetValue(OD_PROP_FILL + OD_FILL_COLOR, m_floodColor);
		ok = pComp->SetValue(OD_PROP_FILL + OD_FILL_TRANSPARENT, m_bFloodTransparent);
	}

	CODEditProperties* pPropEdit = (CODEditProperties*) pComp->GetProperty(OD_PROP_EDIT);
	if (pPropEdit != NULL) {
		ok = pComp->SetValue(OD_PROP_EDIT + OD_EDIT_CANSELECT, m_isSelectionOn);
	}
}

#else // NEW_PROP

void CTurtleView::SetComponentProperties (CODComponent* pComp)
{
	pComp->AddRef(); 
	CODLineProperties lineProp;
	BOOL hasLineProperties = pComp->GetProperty(OD_PROP_LINE, lineProp);
	lineProp.SetWidth(m_penWidth);
	lineProp.SetStyle(m_penStyle);
	lineProp.SetColor(m_penColor);
	lineProp.SetTransparent(m_penWidth == 0);
	pComp->SetProperty(&lineProp);

	CODFillProperties fillProp;
	BOOL hasFillProperties = pComp->GetProperty(OD_PROP_FILL, fillProp);
	fillProp.SetColor(m_floodColor);
	fillProp.SetTransparent(m_bFloodTransparent);
	pComp->SetProperty(&fillProp);

//>000428 - spostato in PR_GO_DRAW (posposto)
//	CODEditProperties editProp;
//	BOOL hasEditProperties = pComp->GetProperty(OD_PROP_EDIT, editProp);
//	editProp.SetCanSelect(m_isSelectionOn);
//	pComp->SetProperty(&editProp);
//< 000428
}

#endif // NEW_PROP

// usato per CODTextComponent e CODLabelComponent
// come in Tarta classica, il colore del font è COLPENNA !! (ripristinare ASCOLTESTO ?)
// dato che è usato anche per il bordo, se non si vuole il bordo porre TRATTOPENNA a 0
// a differenza di Tarta classica, lo sfondo è COLRIEMPI (vedi SetComponentProperties) !!
void CTurtleView::SetTextProperties (CODTextComponent* pText)
{
	LOGFONT *pLogFont = GetTextFont();
	CODFontProperties fontProp;
	BOOL hasFontProperties = pText->GetProperty(OD_PROP_FONT, fontProp);
	fontProp.SetColor(m_penColor);
	fontProp.SetFaceName(pLogFont->lfFaceName);
	fontProp.SetHeight(pLogFont->lfHeight);
	fontProp.SetWeight(pLogFont->lfWeight);
	fontProp.SetItalic(pLogFont->lfItalic);
	fontProp.SetUnderline(pLogFont->lfUnderline);
	fontProp.SetStrikeout(pLogFont->lfStrikeOut);
	pText->SetProperty(&fontProp);
}

void PR_GO_SETTYPE ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODComponent* pComp = (CODComponent*) num_long (arg_1);
	char text[MAX_CHARS];
	MemScrivi (text, arg_2);
	pComp->SetType(CString(text)); 
}

// appende a Modello (default) o a Composto (arg.2), componente
// passato per pointer; nel caso di Modello, riporta lo Id
void PR_GO_APPEND ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	CODComponent* pChild = (CODComponent*) num_long (arg_1);
	if (n_argomenti == 1) {
		BOOL appended = pModel->AppendChild(pChild);
		if (appended) {
			if (pView->m_refresh)
				pModel->UpdateComponent(pChild, OD_UPDATE_CURRENT_POS);
			risultato = long_num(pChild->GetID());
		}
	}
	else {
		CODComponent* pParent = (CODComponent*) num_long (arg_2);
		BOOL appended = pParent->AppendChild(pChild);
		/* risultato = NULLP */
	}
}

// visualizza un oggetto "appendendolo" al modello,
// dopo aver efettuato la translazione necessaria
// per portare il control point sul target point
// arg_1: componente passato per pointer
// arg_2: target point (default: POS)
// arg_3: indice del control point (default: origine relativa)
// risultato: id che distingue l'oggetto nel modello
void PR_GO_DRAW ()
{
//> 000125
//	PR_GO_APPEND ();
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	CODComponent* pComp = (CODComponent*) num_long (arg_1);
	// pointer inverosimile ?
	if (! isValidComp (pComp, -1))
		err_arg1();
	// già appartiene al modello
	if (pModel->GetComponent(pComp->GetID()))
		err_arg1();
	// posizione target default (tarta)
	double xCor = pt->pos_x_t, yCor = pt->pos_y_t;
	// posizione target esplicita
	if (n_argomenti > 1)
		if (! coordinate_double (arg_2, &xCor, &yCor))
		{ err_arg2 (); return ; };
	// posizione target fisica
	int xTarget = xLogoToView (xCor), yTarget = yLogoToView (yCor);
	// control point default (0,0)
	CPoint controlPoint = CPoint(0,0);
	// control point esplicito
	if (n_argomenti > 2) {
		int nControlPoint = num_int (arg_3);
		controlPoint = pComp->GetControlPoint((OD_CONTROL_POINT) nControlPoint);
	}
	// esegue translazione necessaria
	pComp->Translate(xTarget-controlPoint.x, yTarget-controlPoint.y);
	// cerca di aggiungerlo al modello; insuccesso ?
	if (! pModel->AppendChild(pComp))
		err_arg1();

//>000428 - spostato qui da SetComponentProperties
	CODEditProperties editProp;
	BOOL hasEditProperties = pComp->GetProperty(OD_PROP_EDIT, editProp);
	editProp.SetCanSelect(pView->m_isSelectionOn);
	pComp->SetProperty(&editProp);
//< 000428

	if (pView->m_refresh)
		pModel->UpdateComponent(pComp, OD_UPDATE_CURRENT_POS);
	risultato = long_num(pComp->GetID());
//< 000125
}

// rimuove uno o più elementi da Modello (default) o da Composto (arg.2)
void PR_GO_REMOVE ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	pController->ClearSelection();
	BOOL removed = FALSE;
	if (n_argomenti == 1) {
		ODComponentId lID;
		if (tipo_1 == LBCD) {
			lID = num_long (arg_1);
			CODComponent* pComp = pModel->GetComponent(lID);
			if (pComp)
				removed |= pModel->CompRemove(pComp);
		}
		else while (arg_1 != NULLP) {
			lID = num_long (car (arg_1));
			CODComponent* pComp = pModel->GetComponent(lID);
			if (pComp)
				removed |= pModel->CompRemove(pComp);
			arg_1 = cdr (arg_1);
		}
	}
	else {		// (n_argomenti == 2)
	}
	if (removed)
		pView->UpdateAll();
}

// ora accetta Id o Pointer
// e riporta sempre Pointer
void PR_GO_DUPLICATE ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

//	e' Id o Pointer ?
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp)
		pComp = (CODComponent*) ref;

	CODComponent* pNewComp = pComp->Dup();

	pNewComp->AddRef(); 
	risultato = long_num((long) pNewComp);
}

void PR_GO_SELECTION ()
{
	risultato = NULLP;
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	CODComponentSet* pSet = pController->GetSelection();

	risultato = CODComponentSet2LogoList (pSet);
}

// seleziona l'elemento o gli elementi specificati
// dopo aver deselezionato tutto
void PR_GO_SELECT()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	pController->ClearSelection();
	if ((n_argomenti == 0) || (arg_1 == NULLP))
		return;
	ODComponentId lID;
	CODComponentSet compSet;
	node ref;
	if (tipo_1 == LBCD) {
		ref = num_long (arg_1);
		lID = (ODComponentId) ref;
		CODComponent* pComp = pModel->GetComponent(lID);
		if (! pComp)
			pComp = (CODComponent*) ref;
		int index = compSet.Add(pComp);
	}
	else {
		int nComp = lstlen(arg_1);
		compSet.SetSize(nComp);
		int i; for (i=0; i<nComp; i++) {
			ref = num_long (car (arg_1));
			arg_1 = cdr (arg_1);
			lID = (ODComponentId) ref;
			CODComponent* pComp = pModel->GetComponent(lID);
			if (! pComp)
				pComp = (CODComponent*) ref;
			compSet.SetAt(i, pComp);
		}
	}
	pController->AddToSelection (&compSet);
}

// ora accetta Id o Pointer e riporta sempre Pointer
void PR_GO_GROUP()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	if (arg_1 == NULLP)
		return;

	CODComponent* pGroup = new(CODComponent);
	int nChilds = lstlen (arg_1);
	CODComponentSet compSet;
	compSet.SetSize(nChilds);
	ODComponentId lID;
	CODComponent* pComp;
	int i; for (i=0; i<nChilds; i++) {
		// e' Id o Pointer ?
		node ref = num_long (car (arg_1));
		lID = (ODComponentId) ref;
		pComp = pModel->GetComponent(lID);
		if (! pComp)
			pComp = (CODComponent*) ref;
		compSet.SetAt(i, pComp);
		arg_1 = cdr (arg_1);
	}
	BOOL created = pGroup->Create(&compSet);
	pGroup->AddRef(); 
	risultato = long_num((long) pGroup);
}

void PR_GO_ELEMENTS()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lID = num_long (arg_1);
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) return;

	CODComponentSet* pSet = pComp->GetComponents();
	risultato = CODComponentSet2LogoList (pSet);
}

void PR_GO_UNGROUP()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lID = num_long (arg_1);
	CODComponent* pGroup = pModel->GetComponent(lID);
	if (! pGroup)
		return;
	int componentCount = pGroup->GetChildCount();
	if (componentCount == 0)
		return;

	CODIndexMap* pIndexMap = NULL;	// che significa ?
	pModel->UngroupComponent(pGroup, pIndexMap);
	pView->UpdateAll();
}

// crea un simbolo da una lista di elementi; riporta pointer
void PR_GO_SYMBOL ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODSymbolComponent* pSymbol = new(CODSymbolComponent);
	if (n_argomenti == 0) {
		BOOL created = pSymbol->Create();
	}
	else {
		CODComponent* pComp;
		CODComponentSet* pCompSet = new(CODComponentSet);
		int index = -1;
		while (arg_1 != NULLP) {
			pComp = (CODComponent*) num_long (car (arg_1));	// !
			index = pCompSet->Add(pComp);
			arg_1 = cdr (arg_1);
		}
		BOOL created = pSymbol->Create(pCompSet);

	}
	pSymbol->AddRef(); 
	risultato = long_num((long) pSymbol);	// !
}

// crea una spezzata con posizionamento relativo
void PR_GO_POLYLINE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	node lista = arg_1;
	node point;
	double xCor, yCor;
	int x, y;

	int nPoints = lstlen (lista);
	CODPointArray codPoints;
	codPoints.SetSize(nPoints);
	int i;
	for (i=0; i<nPoints; i++) {
		point = car (lista);
		lista = cdr (lista);
		xCor = node2real(car (point));
		yCor = node2real(car (cdr (point)));
		x = widthLogoToView(xCor);
		y = -heightLogoToView(yCor);
		codPoints.SetAt(i, CPoint (x, y));             
	}
	CODLineComponent* pLine = new(CODLineComponent);
	BOOL created = pLine->Create(&codPoints);
	if (created) {
		pView->SetComponentProperties (pLine);
		risultato = long_num((long) pLine);	// !
	}
}

void PR_GO_POLYGON()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	node lista = arg_1;
	node point;
	double xCor, yCor;
	int x, y;

	int nPoints = lstlen (lista);
	CODPointArray codPoints;
	codPoints.SetSize(nPoints);
	int i;
	for (i=0; i<nPoints; i++) {
		point = car (lista);
		lista = cdr (lista);
		xCor = node2real(car (point));
		yCor = node2real(car (cdr (point)));
		x = widthLogoToView(xCor);
		y = -heightLogoToView(yCor);
		codPoints.SetAt(i, CPoint (x, y));             
	}
	CODPolygonComponent* pPoly = new(CODPolygonComponent);
	BOOL created = pPoly->Create(&codPoints);
	if (created) {
		pView->SetComponentProperties (pPoly);
		risultato = long_num((long) pPoly);	// !
	}
}

// crea rettangolo con vertice bottom-left sull'origine
// e riporta pointer
void PR_GO_RECT()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	double cx = node2real(arg_1);
	double cy = node2real(arg_2);
	int left = 0;
	int right = widthLogoToView(cx);
	int top = - heightLogoToView(cy);
	int bottom = 0;

	CODRectComponent* pRect = new(CODRectComponent);
	BOOL created = pRect->Create(left, top, right, bottom);
	if (created) {
		pView->SetComponentProperties (pRect);
		risultato = long_num((long) pRect);	// !
	}
}

// crea ellisse inscritto in rettangolo con vertice bottom-left sull'origine
// e riporta pointer
void PR_GO_ELLIPSE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	double cx = node2real(arg_1);
	double cy = node2real(arg_2);
	// rettangolo
	int left = 0;
	int right = widthLogoToView(cx);
	int top = - heightLogoToView(cy);
	int bottom = 0;
	// ellisse inscritto nel rettangolo
	CODEllipseComponent* pEllipse = new(CODEllipseComponent);
	BOOL created = pEllipse->Create(CRect(left, top, right, bottom));
	if (created) {
		pView->SetComponentProperties (pEllipse);
		risultato = long_num((long) pEllipse);	// !
	}
}

void pointsListToArray (node list, CODPointArray* pCodPoints)
{
	node point;
	double xCor, yCor;
	int x, y;

	int nPoints = lstlen (list);
	pCodPoints->SetSize(nPoints);
	int i;
	for (i=0; i<nPoints; i++) {
		point = car (list);
		list = cdr (list);
		xCor = node2real(car (point));
		yCor = node2real(car (cdr (point)));
		x = widthLogoToView(xCor);
		y = -heightLogoToView(yCor);
		pCodPoints->SetAt(i, CPoint (x, y));             
	}
}

void PR_GO_CURVE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODPointArray codPoints;
	pointsListToArray (arg_1, &codPoints);

	CODCurveComponent* pCurve = new(CODCurveComponent);
	BOOL created = pCurve->Create(&codPoints);
	if (created) {
		pView->SetComponentProperties (pCurve);
		risultato = long_num((long) pCurve);	// !
	}
}

void PR_GO_CLOSEDCURVE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODPointArray codPoints;
	pointsListToArray (arg_1, &codPoints);

	CODClosedCurveComponent* pClosedCurve = new(CODClosedCurveComponent);
	BOOL created = pClosedCurve->Create(&codPoints);
	if (created) {
		pView->SetComponentProperties (pClosedCurve);
		risultato = long_num((long) pClosedCurve);	// !
	}
}

// 000501: ripreso argomenti da PR_GO_LABEL e aggiunto font
// arg_1 : testo (può essere vuoto)
// arg_2 :	1 numero : lato box a cui ancorare il testo in autosize
//				(0=OD_TOP_LEFT, 8=OD_CENTER)
//			lista di 2 (o 3) numeri : dimensioni box (+ ancoraggio)
// arg_3 : lista di fino a 3 numeri:
//			multiline (1) | wordbreak (2)
//			allineam. orizz. (0=DT_LEFT, 1=DT_CENTER, 2=DT_RIGHT)
//			allineam. vert.  (0=DT_TOP, 1=DT_VCENTER, 2=DT_BOTTOM)
void PR_GO_TEXT()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODTextComponent* pText = new(CODTextComponent);
	BOOL created = pText->Create(CPoint(0, 0), pView->GetDC());
	if (! created)
		return;
	pView->SetComponentProperties (pText);
	pView->SetTextProperties (pText);

	char text[MAX_CHARS];
	LogoToText (arg_1, text);
	pText->SetText(text);

	node props;
	int val;
	if (n_argomenti > 2) {
		props = arg_3;
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			if (val & 1)
				pText->SetValue(OD_PROP_MULTILINE, TRUE);
			if (val & 2)
				pText->SetValue(OD_PROP_WORDBREAK, TRUE);
		}
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			pText->SetValue(OD_PROP_HORZ_ALIGNMENT, val);
		}
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			pText->SetValue(OD_PROP_VERT_ALIGNMENT, val * 4);
		}
	}

	OD_CONTROL_POINT ctlPtLabel;
	if ((n_argomenti > 1) && (typ(arg_2) == LIST)) {
		int l = lstlen(arg_2);
		if ((l < 2) || (l > 3)) { err_arg2 (); return; }
		double cx = node2real(car (arg_2));
		double cy = node2real(car (cdr (arg_2)));
		pText->SetTextBox(CRect(0,0, widthLogoToView(cx), heightLogoToView(cy)));
		if (l > 2) {
			ctlPtLabel = (OD_CONTROL_POINT) num_int (car (cdr (cdr (arg_2))));
			pText->GrowToText(ctlPtLabel, pView->GetDC());
		}
	}
	else {
		ctlPtLabel = OD_CENTER;
		if ((n_argomenti > 1) && (typ(arg_2) == LBCD))
			ctlPtLabel = (OD_CONTROL_POINT) num_int (arg_2);
		pText->SizeToText(ctlPtLabel, pView->GetDC());
	}

	risultato = long_num((long) pText);	// !
}

// crea oggetto immagine con origine sul vertice inferiore sinistro
// arg_1 : nome del file immagine
// risultato: pointer (!) a immagine non aggiunta a modello
void PR_GO_IMAGE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	char strImagePath [MAXCHARS];
	getnam (strImagePath, arg_1);
	CString strFileName = strImagePath;

	CODImageComponent* pImage = ImageLoadFromFile (strFileName);
	if (pImage == NULL)
	{ err2 (31, arg_1); return; }

	SECImage* pSECImage = pImage->GetImage();
    DWORD dwWidth = pSECImage->m_dwWidth;
    DWORD dwHeight = pSECImage->m_dwHeight;
	pImage->Translate(0, -dwHeight+1);

	risultato = long_num((long) pImage);	// !
}

// riporta sotto forma di lista le dimensioni in pixel di un'immagine
void PR_GO_IMAGE_DIM()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODImageComponent* pImage = (CODImageComponent*) pModel->GetComponent(lID);
	if (! pImage) {
		pImage = (CODImageComponent*) ref;
		if (! isValidComp (pImage, -1))
			err_arg1();
	}

	SECImage* pSECImage = pImage->GetImage();
    DWORD dwWidth = pSECImage->m_dwWidth;
    DWORD dwHeight = pSECImage->m_dwHeight;

	risultato = cons (consreal (dwWidth), 
		cons (consreal (dwHeight), NULLP, LIST), 
		LIST);
}

CODSymbolComponent* SymbolLoad (LPCSTR lpszPath);

// arg_1 : nome del file .sym
// risultato: pointer (!) a simbolo non aggiunto a modello
void PR_SYMBOLLOAD()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	char strImagePath [MAXCHARS];
	getnam (strImagePath, arg_1);

	CString strFileName = strImagePath;
	CODSymbolComponent* pSymbol = SymbolLoad (strFileName);
	if (pSymbol)
		risultato = long_num((long) pSymbol);	// !
	else
		err2 (31, arg_1);
}

// vedi OnCapacitor() in CircuitCtlr.cpp
// vedi OnInsertSymbol() in OdController.cpp
// vedi SymbolLoad() in turtleMdl.cpp
// vedi PR_NODORECUPERA() (obsoleto ?) in turtleVw.cpp

void EventCheck ();

// come PR_NODEINSERT, ma senza duplicazione !!
// arg_1 : pointer (!) a simbolo campione non aggiunto a modello
// risulato: lista di id aggiunto a modello e punto (coppia x-y)
void PR_SYMBOLINSERT()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	CODSymbolComponent* pSymbol = (CODSymbolComponent*) num_long (arg_1);	// !

	pController->m_bInserted = FALSE;
	pController->OnInsertSymbol(pSymbol);
	while (! pController->m_bInserted)
		EventCheck ();
	ODComponentId lID = pSymbol->GetID();
	CPoint insertPoint = pController->m_ptInsertPoint;
	node logoPoint = cons (int_num (xViewToLogo (insertPoint.x)),
					 cons (int_num (yViewToLogo (insertPoint.y)), NULLP, LIST), LIST);
	risultato = cons (long_num (lID), logoPoint, LIST);
}

// arg_1 : pointer (!) a simbolo campione non aggiunto a modello
// risulato: lista di id aggiunto a modello e punto (coppia x-y)
// NOTA: da aggiornare perché riporti un Pointer
// chiamando direttamente pComp->Dup() (vedi OGDUPLICA !!)
void PR_NODEINSERT()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	CODComponent* pComp = (CODComponent*) num_long (arg_1);	// !
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->CompDup (pComp);

	pController->m_bInserted = FALSE;
	pController->OnInsertSymbol(pSymbol);
	while (! pController->m_bInserted)
		EventCheck ();
	ODComponentId lID = pSymbol->GetID();
	CPoint insertPoint = pController->m_ptInsertPoint;
	node logoPoint = cons (int_num (xViewToLogo (insertPoint.x)),
					 cons (int_num (yViewToLogo (insertPoint.y)), NULLP, LIST), LIST);
	risultato = cons (long_num (lID), logoPoint, LIST);
}

// arg_1 : simbolo campione (non aggiunto a modello)
// risulato: id di nodo (simbolo istanziato, aggiunto a modello)
// NOTA: da abolire e sostituire con OGDISEGNA OGDUPLICA !!!
void PR_NODECREATE()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	CODComponent* pComp = (CODComponent*) num_long (arg_1);	// !
	if (! pComp) return;
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->CompDup (pComp);
	if (! pSymbol) return;
	risultato = long_num(pSymbol->GetID());
}

void PR_NODICONNETTI()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lSourceID = num_long (arg_1);
	ODComponentId lTargetID = num_long (arg_2);
	CODComponent* pSourceComp = pModel->GetComponent(lSourceID);
	CODComponent* pTargetComp = pModel->GetComponent(lTargetID);

	if (   pSourceComp && pTargetComp
//		&& (pSourceComp->GetType() == ..) && (pTargetComp->GetType() == ..)
	) {
		CODLinkComponent* pLinkComp =
			pModel->NodesConnect((CODSymbolComponent*) pSourceComp, (CODSymbolComponent*) pTargetComp);
		if (pLinkComp) {
			pModel->UpdateComponent(pLinkComp, OD_UPDATE_CURRENT_POS);
			if (pView->m_refresh)
				risultato = long_num(pLinkComp->GetID());
		}
	}
}

void PR_ARCOINVERTI()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lID = num_long (arg_1);
	CODComponent* pComp = pModel->GetComponent(lID);
	CString strType = pComp->GetType();
	//	if (strType != ...)

	pModel->EdgeInvert((CODLinkComponent*) pComp);
}

// crea un label e riporta pointer
// arg_1 : testo (può essere vuoto)
// arg_2 :	1 numero : lato box a cui ancorare il testo in autosize
//				(0=OD_TOP_LEFT, 8=OD_CENTER)
//			lista di 2 (o 3) numeri : dimensioni box (+ ancoraggio)
// arg_3 : lista di fino a 3 numeri:
//			multiline (1) | wordbreak (2)
//			allineam. orizz. (0=DT_LEFT, 1=DT_CENTER, 2=DT_RIGHT)
//			allineam. vert.  (0=DT_TOP, 1=DT_VCENTER, 2=DT_BOTTOM)
void PR_GO_LABEL()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	CODLabelComponent* pLabel = new(CODLabelComponent);
	BOOL created = pLabel->Create(NULL);
	if (! created)
		return;
	pView->SetComponentProperties (pLabel);
	pView->SetTextProperties (pLabel);

	char text[MAX_CHARS];
	LogoToText (arg_1, text);
	pLabel->SetText(text);

	node props;
	int val;
	if (n_argomenti > 2) {
		props = arg_3;
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			if (val & 1)
				pLabel->SetValue(OD_PROP_MULTILINE, TRUE);
			if (val & 2)
				pLabel->SetValue(OD_PROP_WORDBREAK, TRUE);
		}
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			pLabel->SetValue(OD_PROP_HORZ_ALIGNMENT, val);
		}
		if (props != NULLP) {
			val = num_int (car (props)); props = cdr (props);
			pLabel->SetValue(OD_PROP_VERT_ALIGNMENT, val * 4);
		}
	}

	OD_CONTROL_POINT ctlPtLabel;
	if ((n_argomenti > 1) && (typ(arg_2) == LIST)) {
		int l = lstlen(arg_2);
		if ((l < 2) || (l > 3)) { err_arg2 (); return; }
		double cx = node2real(car (arg_2));
		double cy = node2real(car (cdr (arg_2)));
		pLabel->SetTextBox(CRect(0,0, widthLogoToView(cx), heightLogoToView(cy)));
		if (l > 2) {
			ctlPtLabel = (OD_CONTROL_POINT) num_int (car (cdr (cdr (arg_2))));
			pLabel->GrowToText(ctlPtLabel, pView->GetDC());
		}
	}
	else {
		ctlPtLabel = OD_CENTER;
		if ((n_argomenti > 1) && (typ(arg_2) == LBCD))
			ctlPtLabel = (OD_CONTROL_POINT) num_int (arg_2);
		pLabel->SizeToText(ctlPtLabel, pView->GetDC());
	}

	risultato = long_num((long) pLabel);	// !
}

// arg_1 : id del simbolo
// risultato : numero di label
void PR_SYMBOL_LABELNUMBER ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

//	e' Id o Pointer ?
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->GetComponent(lID);
	if (! pSymbol)
		pSymbol = (CODSymbolComponent*) ref;

	int nLabels = pSymbol->GetNumLabels();
	risultato = int_num(nLabels);
}

// arg_1 : id del simbolo
// arg_2 : indice del label
void PR_SYMBOL_REMOVELABEL ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

//	e' Id o Pointer ?
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->GetComponent(lID);
	if (! pSymbol)
		pSymbol = (CODSymbolComponent*) ref;

	int nLabelIdx = num_int (arg_2);
	CODLabelComponent* pLabel = pSymbol->GetLabel(nLabelIdx);
	if (! pLabel) { err_arg2 (); return; }
	if (pLabel) {
		BOOL removed1 = pSymbol->RemoveLabel(nLabelIdx);
		if (! removed1) { err_arg2 (); return; }
//		BOOL removed2 = pModel->RemoveChild(pLabel);
//		if (! removed2) { err_arg2 (); return; }
		pSymbol->OnMove();
		if (pView->m_refresh)
			pModel->UpdateComponent(pSymbol, OD_UPDATE_OLD_AND_NEW_POS);
//		delete (pLabel);
	}
}

// arg_1 : id del simbolo
// arg_2 : indice
// arg_3 : (num. o lista di max 2 num.) : centratura label e testo, se non default
// arg_4 : testo
// risultato: Pointer al label creato (già Id)
void PR_SYMBOL_INSERTLABEL ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

//	e' Id o Pointer ?
	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->GetComponent(lID);
	if (! pSymbol) {
		pSymbol = (CODSymbolComponent*) ref;
		isId = FALSE;
	}

	int nLabelIdx = num_int (arg_2);
	char text[MAX_CHARS];
	OD_CONTROL_POINT ctlPtSymbol = OD_CENTER;
	OD_CONTROL_POINT ctlPtLabel = OD_CENTER;
	if (typ (arg_3) == LIST) {
		if (arg_3 != NULLP) {
			ctlPtSymbol = (OD_CONTROL_POINT) num_int (car (arg_3));
			if (cdr (arg_2) != NULLP) {
				ctlPtLabel = (OD_CONTROL_POINT) num_int (car (cdr (arg_3)));
			}
		}
	}
	else
		ctlPtSymbol = (OD_CONTROL_POINT) num_int (arg_3);
	LogoToText (args[3], text);
	CODLabelComponent* pLabel = new(CODLabelComponent);
	BOOL created = pLabel->Create(NULL);
	if (created) {
		BOOL inserted = pSymbol->InsertLabel(pLabel, nLabelIdx);
		if (inserted) {
			pLabel->SetOrientation(ctlPtSymbol);
			pLabel->SetText(text);
			// ATTENZIONE: SizeToText supporta solo OD_TOP_LEFT (0) e OD_CENTER (8)
			pLabel->SizeToText(ctlPtLabel, pView->GetDC()); 
			BOOL set = pLabel->SetValue(OD_PROP_LINE + OD_LINE_TRANSPARENT, TRUE);
			set = pLabel->SetValue(OD_PROP_FILL + OD_FILL_TRANSPARENT, TRUE);
			set = pLabel->SetValue(OD_PROP_EDIT + OD_EDIT_CANSELECT, FALSE);
//			pSymbol->OnMove();
//			pModel->UpdateComponent(pSymbol, OD_UPDATE_OLD_AND_NEW_POS);
//			risultato = long_num(pLabel->GetID());
			if (isId) {
				pSymbol->OnMove();
				if (pView->m_refresh)
					pModel->UpdateComponent(pSymbol, OD_UPDATE_OLD_AND_NEW_POS);
			}
			risultato = long_num((long) pLabel);
		}
	}
}

// in realtà ora ASSEGNA un label : generalizzato arg_4
// arg_1 : Id o Pointer al simbolo
// arg_2 : Pointer al label da aggiungere
// arg_3 : orientation del label nel simbolo + eventuale displacement
// arg_4 : indice del label nel simbolo (insert), oppure .. (add)
// risultato: indice del label nel simbolo
void PR_SYMBOL_SETLABEL ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

//	rif. al simbolo è Id o Pointer ?
	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODSymbolComponent* pSymbol = (CODSymbolComponent*) pModel->GetComponent(lID);
	if (! pSymbol) {
		isId = FALSE;
		pSymbol = (CODSymbolComponent*) ref;
		if (! isValidComp (pSymbol, -1))
			err_arg1();
	}

	CODLabelComponent* pLabel =
		(CODLabelComponent*) num_long (arg_2);
	if (! isValidComp (pLabel, -1))
		err_arg2();

	OD_CONTROL_POINT ctlPtSymbol = OD_CENTER;
	node pos = NULLP;
	if (n_argomenti > 2)
		pos = args[2];
	if (pos == NULLP) 
		pLabel->SetOrientation(ctlPtSymbol);
	else {
		ctlPtSymbol = (OD_CONTROL_POINT) num_int (car (pos));
		pos = cdr (pos);
		if (pos == NULLP)
			pLabel->SetOrientation(ctlPtSymbol);
		else {
			double dx = node2real(car (pos));
			double dy = node2real(car (cdr (pos)));
			CODOrientationProperties orientProp;
			orientProp.SetControlPoint(ctlPtSymbol);
			if (dx != 0)
				orientProp.SetHorizontalSpacing(widthLogoToView(dx));
			if (dy != 0)
				orientProp.SetVerticalSpacing(heightLogoToView(dy));
			pLabel->SetProperty(&orientProp);
		}
	}

	int nLabelIdx = -1;
	if (n_argomenti > 3)
		nLabelIdx = num_int (args[3]);
	int nLabels = pSymbol->GetNumLabels();
	if ((nLabelIdx == -1) || (nLabelIdx > nLabels))
		nLabelIdx = nLabels;

	BOOL inserted = pSymbol->InsertLabel(pLabel, nLabelIdx);
	if (! inserted) { err1 (38); return; }

	pLabel->SetValue(OD_PROP_EDIT + OD_EDIT_CANSELECT, FALSE);
	if (isId) {
		pSymbol->OnMove();
		if (pView->m_refresh)
			pModel->UpdateComponent(pSymbol, OD_UPDATE_OLD_AND_NEW_POS);
	}
	risultato = int_num(nLabelIdx);
}

// CPoint GetComponentCenter(CODComponent* pComp);	// in TurtleMdl.cpp
void PR_GO_POS()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}
	int nControlPoint = OD_BOTTOM_LEFT;
	if (n_argomenti > 1) 
		nControlPoint = num_int (arg_2);
	CPoint controlPoint =
		pComp->GetControlPoint((OD_CONTROL_POINT) nControlPoint);
	risultato = cons(consreal(xViewToLogo(controlPoint.x)),
				cons(consreal(yViewToLogo(controlPoint.y)), NULLP, LIST), LIST);
}

// ispirato a CODSymbolComponent::GetEdges (OdSymbolComp.cpp)
void AdjustLinksIfSymbol (CODComponent* pComp)
{
	IODNode* pINode;
	HRESULT hResult = pComp->QueryInterface(IID_IODNode, (void **) &pINode);
	if (pComp->QueryInterface(IID_IODNode, (void **) &pINode) == S_OK)
	{
		CODSymbolComponent* pSymbol = (CODSymbolComponent*) pComp;
		CODConnectionSet setConnections;
		pSymbol->GetAllConnections(setConnections);
		for (int nConnectIdx = 0; nConnectIdx < setConnections.GetSize(); nConnectIdx++)
		{
			CODConnection* pConnection = setConnections.GetAt(nConnectIdx);
			CODPortComponent* pDepPort = pSymbol->GetDependentPortOnConnection(pConnection);
			if (pDepPort != NULL)
			{
				CODLinkComponent* pLink = (CODLinkComponent*) pDepPort->GetOwner();
				ASSERT_VALID(pLink);
				IODEdge* pIEdge;
				if (pLink->QueryInterface(IID_IODEdge, (void **) &pIEdge) == S_OK)
					pLink->OnMove();
			}
		}
	}
}

// obsoleta ?
void PR_GO_CENTER()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		pComp = (CODComponent*) ref;
		isId = FALSE;
	}

	double x, y;
	if (! coordinate_double (arg_2, &x, &y))
		{ err_arg2 (); return; }

	pComp->CenterOnPoint(CPoint (xLogoToView (x), yLogoToView (y)));
	AdjustLinksIfSymbol (pComp);
	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
		else
			UpdateParent (pComp, pModel);
}

// arg_1 : id del componente, o direttamente pointer al componente !!
// arg_2 : lista di 2 numeri che rappresentano la posizione del centro in coordinate Logo
void PR_GO_SETPOS()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}

	double x, y;
	if (! coordinate_double (arg_2, &x, &y))
		{ err_arg2 (); return; }

	int iControlPoint = OD_BOTTOM_LEFT;	// SudOvest (6)
	if (n_argomenti > 2)
		iControlPoint = num_int (arg_3);

	CPoint ptDest(xLogoToView (x), yLogoToView (y));
	CPoint ptOrigin = pComp->GetControlPoint ((OD_CONTROL_POINT) iControlPoint);
	// da CODComponent::MoveTo(CPoint point)
	CPoint ptTranslate(ptDest - ptOrigin);
	pComp->Translate(ptTranslate.x, ptTranslate.y);
	AdjustLinksIfSymbol (pComp);

	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
		else
			UpdateParent (pComp, pModel);
}

// arg_1 : id del componente, o direttamente pointer al componente !!
// arg_2 : lista di 2 numeri che rappresentano la traslazione in coordinate Logo
void PR_COMPTRANSLA()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}

	double cx, cy;
	if (! coordinate_double (arg_2, &cx, &cy))
		{ err_arg2 (); return; }

	pComp->Translate(widthLogoToView(cx), -heightLogoToView(cy));
	AdjustLinksIfSymbol (pComp);

	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
		else
			UpdateParent (pComp, pModel);
}

float GetComponentDirection(CODComponent* pComp);	// in TurtleMdl.cpp
void PR_COMPDIR()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}
	double alfa = GetComponentDirection(pComp);
	risultato = consreal(alfa);
}

void SetComponentDirection(CODComponent* pComp, float alfa, CPoint origin);	// in TurtleMdl.cpp
// void SetComponentDirection(CODComponent* pComp, float alfa, int iOrigin);	// in TurtleMdl.cpp
void PR_COMPASDIR()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}
	double alfa = node2real(arg_2);
	CPoint origin = pComp->GetControlPoint(OD_CENTER);
	if (n_argomenti > 2) {
		if (tipo_3 == LBCD)
			origin = pComp->GetControlPoint((OD_CONTROL_POINT) num_int (arg_3));
		else
			origin =
				CPoint (xLogoToView (node2real (car (arg_3))),
						yLogoToView (node2real (car (cdr (arg_3)))));
	}
	SetComponentDirection (pComp, alfa, origin);
	AdjustLinksIfSymbol (pComp);

	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
}

// arg_1 : Id o Pointer al simbolo
// arg_2 : delta = angolo di rotazione
// arg_3 : codice o coordinate centro di rotazione
void ComponentRotate (CODComponent* pComp, float delta, CPoint origin);	// in TurtleMdl.cpp
void ComponentRotate(CODComponent* pComp, float alfa, int iOrigin);	// in TurtleMdl.cpp
void PR_COMPRUOTA()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	double delta = node2real(arg_2);

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}

	CPoint origin = pComp->GetControlPoint(OD_CENTER);
	if (n_argomenti > 2) {
		if (tipo_3 == LBCD)
			origin = pComp->GetControlPoint((OD_CONTROL_POINT) num_int (arg_3));
		else {
			double xReal = node2real(car (arg_3));
			double yReal = node2real(car (cdr (arg_3)));
			int x, y;
			if (isId) {
				x = xLogoToView (xReal);
				y = yLogoToView (yReal);
			}
			else {
				x = widthLogoToView(xReal);
				y = -heightLogoToView(yReal);
			}
			origin = CPoint(x,y);
		}
	}
	ComponentRotate (pComp, delta, origin);
	AdjustLinksIfSymbol (pComp);

	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
		else
			UpdateParent (pComp, pModel);
}

void GetComponentScale (CODComponent* pComp, float& fScaleX, float& fScaleY);	// in TurtleMdl.cpp
void PR_COMPSCALA()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}

	float fScaleX, fScaleY;
	GetComponentScale(pComp, fScaleX, fScaleY);
	risultato = cons (consreal (fScaleX), cons (consreal (fScaleY), NULLP, LIST), LIST);
}

void SetComponentScale (CODComponent* pComp, float fScaleX, float fScaleY, CPoint origin);	// in TurtleMdl.cpp
// void SetComponentScale (CODComponent* pComp, float fScaleX, float fScaleY, int iOrigin);	// in TurtleMdl.cpp
void PR_COMPASSCALA()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	BOOL isId = TRUE;
	node ref = num_long (arg_1);
	ODComponentId lID = (ODComponentId) ref;
	CODComponent* pComp = pModel->GetComponent(lID);
	if (! pComp) {
		isId = FALSE;
		pComp = (CODComponent*) ref;
		if (! isValidComp (pComp, -1))
			err_arg1();
	}

	float fScaleX = node2real (car (arg_2));
	float fScaleY = node2real (car (cdr (arg_2)));
	CPoint origin = pComp->GetControlPoint(OD_CENTER);
	if (n_argomenti == 3) {
		if (tipo_3 == LBCD)
			origin = pComp->GetControlPoint((OD_CONTROL_POINT) num_int (arg_3));
		else
			origin =
				CPoint (xLogoToView (node2real (car (arg_3))),
						yLogoToView (node2real (car (cdr (arg_3)))));
	}
	SetComponentScale (pComp, fScaleX, fScaleY, origin);
	AdjustLinksIfSymbol (pComp);

	if (pView->m_refresh)
		if (isId)
			pModel->UpdateComponent(pComp, OD_UPDATE_OLD_AND_NEW_POS);
}
*/

void PR_TURTLEPROPS ()
{
	risultato = int_num (0);

	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	int newProps = -1;
	if (n_argomenti > 0)
		newProps = num_int (arg_1);
	int previousProps = pView->OVsProps (newProps);
	risultato = int_num (previousProps);
}

void PR_TURTLETOOLS ()
{
	risultato = int_num (0);

	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	int nextTools = -1;
	if (n_argomenti > 0)
		nextTools = num_int (arg_1);
	int previousTools = pView->OVsTools (nextTools);
	risultato = int_num (previousTools);
}
#endif OVs

#ifdef DOC
ID_FILE_PRINT:   Standard printing of the current document
Note   You must connect this to your CView-derived class's message map to enable this functionality.
This command prints the current document, or more correctly, starts the printing process,
which involves invoking the standard print dialog and running the print engine.
CView::OnFilePrint implements this command and the main print loop.
...
Only in the case of an application that uses the printer for non-page oriented output,
should you find the need to replace the ID_FILE_PRINT implementation.
#endif // DOC

void CTurtleView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	
#ifdef OVs
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnFilePrint();
	else
#endif // OVs
	 CView::OnFilePrint();
}

#ifdef DOC
ID_FILE_PRINT_PREVIEW:   Enter print-preview mode for the current document.
Note   You must connect this to your CView-derived class's message map to enable this functionality.
CView::OnFilePrintPreview starts the print preview mode by calling the documented
helper function CView::DoPrintPreview.
CView::DoPrintPreview is the main engine for the print preview loop,
just as OnFilePrint is the main engine for the printing loop.
The print preview operation can be customized in a variety of ways by passing
different parameters to DoPrintPreview.
Please refer to Technical Note 30, which discusses some of the details of print preview
and how to customize it.
#endif // DOC

void CTurtleView::OnFilePrintPreview() 
{
	// TODO: Add your command handler code here

	// finge che Turtle Frame sia il Main Frame dell'applicazione ..
	m_pOldMain = theApp.m_pMainWnd;
	theApp.m_pMainWnd = m_frame;

#ifdef OVs
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnFilePrintPreview();
	else
#endif // OVs
		CView::OnFilePrintPreview();
}

#ifdef OVs
void CTurtleView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)
{
	if (m_isOVs)
		MvcScrollView_T<CTurtleViewport>::OnEndPrintPreview(pDC, pInfo, point, pView);
	else
		CView::OnEndPrintPreview(pDC, pInfo, point, pView);

	// .. ripristina il Main Frame dell'applicazione
	theApp.m_pMainWnd = m_pOldMain;
}
#endif // OVs

#ifdef DOC
ID_FILE_PRINT_SETUP:   Invoke the standard Print Setup dialog.
Note   You must connect this to your CWinApp-derived class's message map to enable this functionality.
This command invokes the standard print setup dialog that allows the user to customize
the printer and print settings for at least this document or at most all the documents
in this application. You must use the Control Panel to change the default printer settings
for the entire system.
CWinApp::OnFilePrintSetup has a very simple implementation creating a CPrintDialog object
and calling the CWinApp::DoPrintDialog implementation function.
This sets the application default printer setup.
The common need for customizing this command is to allow for per-document printer settings,
which should be stored with the document when saved.
To do this you should add a message-map handler in your CDocument class that creates
a CPrintDialog object, initializes it with the appropriate printer attributes
(usually hDevMode and hDevNames), call the CPrintDialog::DoModal,
and save the changed printer settings.
For a robust implementation, you should look at the implementation of CWinApp::DoPrintDialog
for detecting errors and CWinApp::UpdatePrinterSelection for dealing with sensible defaults
and tracking system-wide printer changes.
#endif // DOC

void CTurtleView::OnFilePrintsetup() 
{
	// TODO: Add your command handler code here
	
}

void CTurtleView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable(TRUE);
}

void CTurtleView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
	pCmdUI->Enable(TRUE);
}

void CTurtleView::PrintPreview ()
{
	OnFilePrintPreview();
}

void PR_TURTLEPRINTPREVIEW ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	pView->PrintPreview();
}

#ifdef OVs
// riporta alcune proprietà generali della viewport:
// 1. divisione di pagina è visibile
// 2. griglia è visibile
// 3. oggetti sono ancorati alla griglia (in modalià interattiva)
// .. (da completare)
void PR_GETTURTLEPROPS ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	BOOL bPagesVisible = pView->GetPagesVisible();
	BOOL bGridVisible = pView->GetGridVisible();
	BOOL bSnapToGrid = pView->GetSnapToGrid();
	COLORREF crGridColor = pView->GetGridColor();
	float fGridSpacingX, fGridSpacingY;
	pView->GetHorizontalGridSpacing(fGridSpacingX);
	pView->GetVerticalGridSpacing(fGridSpacingY);
	node gridSpacing = cons (consreal(fGridSpacingX), cons (consreal(fGridSpacingY), NULLP, LIST), LIST);

	CPoint cursorPoint(xLogoToView(pt->pos_x_t), yLogoToView(pt->pos_y_t));
	CPoint nearPoint = pView->FindClosestGridPoint(cursorPoint);
	OD_SELECTION_MODE selMode = pView->GetSelectionMode();

	node lista = NULLP;
	lista = cons (gridSpacing, lista, LIST);
	lista = cons (RgbToTriple ((unsigned char*) &crGridColor), lista, LIST);
	lista = cons (int_num((bSnapToGrid) ? 1 : 0), lista, LIST);
	lista = cons (int_num((bGridVisible) ? 1 : 0), lista, LIST);
	lista = cons (int_num((bPagesVisible) ? 1 : 0), lista, LIST);
	risultato = lista;
}

// assegna alcune proprietà generali della viewport
// (vedi PR_GETTURTLEPROPS)
void PR_SETTURTLEPROPS ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	node props = arg_1;
	BOOL bPagesVisible = FALSE;
	BOOL bGridVisible = FALSE, bSnapToGrid = FALSE;
	float fGridSpacingX, fGridSpacingY;
	COLORREF crGridColor = RGB(0,0,0);

	// pages visible
	if (props != NULLP) {
		bPagesVisible = (num_int (car (props)) != 0);
		props = cdr (props);
	}
	// grid visible
	if (props != NULLP) {
		bGridVisible = (num_int (car (props)) != 0);
		props = cdr (props);
	}
	// snap to grid
	if (props != NULLP) {
		bSnapToGrid = (num_int (car (props)) != 0);
		props = cdr (props);
	}
	// horz grid spacing
	if (props != NULLP) {
		fGridSpacingX = node2real (car (props));
		props = cdr (props);
	}
	// vert grid spacing
	if (props != NULLP) {
		fGridSpacingY = node2real (car (props));
		props = cdr (props);
	}
	// grid color
	if (props != NULLP) {
		props = cdr (props);
	}

	pView->SetPagesVisible(bPagesVisible);
	pView->SetGridVisible(bGridVisible);
	pView->SetSnapToGrid(bSnapToGrid);
//	pView->SetHorizontalGridSpacing(fGridSpacingX);
//	pView->SetVerticalGridSpacing(fGridSpacingY);
//	pView->SetGridColor(crGridColor);

	pView->UpdateAll();
}

void PR_GO_SETPANSELECT ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	if (bool_ (arg_1))
		pController->OnPan();
	else
		pController->OnDrawSelect();
}
*/

#endif // OVs

BOOL CTurtleView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

//>031030
	if ((m_hCursor != 0) && (nHitTest == HTCLIENT)) {
		HCURSOR cursor = SetCursor (m_hCursor);
		return TRUE;
	}
#ifdef // OVs
	else
//<031030
		return MvcScrollView_T<CTurtleViewport>::OnSetCursor(pWnd, nHitTest, message);
#endif // OVs
}

// casi per l'argomento:
// 1. ID di risorsa
// 2. nome di risorsa (non implementato)
// 3. nome di file
void PR_SETCURSOR ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	char fileName [MAXCHARS];
	getnam (fileName, arg_1);
	node num;
	UINT nIDResource;
	UINT n_car = edi_num (fileName, &num);
	HCURSOR hCursor;
	if (n_car > 0) {
		nIDResource = (UINT) node2real(num);
		hCursor = LoadCursor (NULL, (LPCTSTR) nIDResource);
	}
	else
		hCursor = LoadCursorFromFile((LPCTSTR) fileName);
	if (hCursor)
		pView->m_hCursor = hCursor;
	else {	
		pView->m_hCursor = 0;
		err2 (5, arg_1);
	}
}
