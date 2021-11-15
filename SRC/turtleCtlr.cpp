// turtleCtlr.cpp : implementation of the CTurtleController class
//
/*	turtleCtrl.cpp

000713	disattivato OnRButtonDown/Up (provvisoriamente)
000421	aggiunto OnFileOpen, OnFileSave, OnFileSaveAs
		sostituito CTurtleController::FileExportPrompt con ::FileOpenSavePrompt
000228	aggiunto AddToSelection
991105	spostato gestione menu da turtleCtlr a turtleVw
991029	aggiunto SpecialZoom
991018	agg. messaggi di mouse in MESSAGE_MAP (v. OdController.cpp)
991014	esteso range di ON_UPDATE_COMMAND_UI_RANGE e poi ripristinato
		mediato OnPageSetup
991011	completato OgDoubleClicked
990923	ovverride di OnInsertSymbol e InsertSymbol
990922	integrato menu dinamici
990920  override di OnLButtonDblClk e StartTextEdit
990909  override e aggiunte per estensione di CODController::OnFileExport
*/

#include "stdafx.h"

#include "iperlogo.h"
#include "ilcpp.h"
#include "ildialog.h"

#include "turtlevw.h"
#include "turtleCtlr.h"
#include "turtleDoc.h"
#include "turtleMdl.h"
#include "turtleVp.h"
// #include "/il32/resource.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern DlgList Dialogs;

/////////////////////////////////////////////////////////////////////////////
// CTurtleController

CTurtleController::CTurtleController(CTurtleViewport* pViewport) :
	CODController(pViewport)
{
}

CTurtleController::~CTurtleController()
{
}

BEGIN_MESSAGE_MAP(CTurtleController, CODController)
	//{{AFX_MSG_MAP(CTurtleController)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_OD_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_IL_SYMBOL_LOAD, OnSymbolLoad)
	ON_COMMAND(ID_IL_EDGE_CREATE, OnEdgeCreate)
	ON_COMMAND(ID_IL_EDGE_INVERT, OnEdgeInvert)
//	ON_COMMAND_RANGE(101, 1000, OnMenuButton)
//	ON_UPDATE_COMMAND_UI_RANGE(101, 1000, OnUpdateMenuButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef SPOSTATO_A_VIEW

void CTurtleController::OnUpdateMenuButton(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CTurtleController::OnMenuButton(UINT uID)
{
	CTurtleView* pView = (CTurtleView*) GetVp();
	DlgEntry turtleEntry = pView->m_dlgEntry;
	node sons = GetSons(turtleEntry);
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

#endif // SPOSTATO_A_VIEW

/////////////////////////////////////////////////////////////////////////////
// CTurtleController message handlers

void CTurtleController::OnMouseEvent(int iEvent, CPoint point)
{
	CTurtleView* pView = (CTurtleView*) GetVp();
	pView->m_buttonDownPos = point;
	if (pView->m_mouseState) {
		DlgEntry entry = WinToEntry (Dialogs, pView);
		ASSERT(entry);
		if (entry) {
			node callBack = GetSetup (entry);
			callBack = (_ele (iEvent, callBack));
			NestedExec(callBack);
		}
	}
}
	
afx_msg void CTurtleController::OnLButtonDown(UINT nFlags, CPoint point)
{
	CODController::OnLButtonDown(nFlags, point);
	OnMouseEvent (1, point);
}

afx_msg void CTurtleController::OnLButtonUp(UINT nFlags, CPoint point)
{
	CODController::OnLButtonUp(nFlags, point);
	OnMouseEvent (2, point);
}

afx_msg void CTurtleController::OnRButtonDown(UINT nFlags, CPoint point)
{
//	CODController::OnRButtonDown(nFlags, point);
//	OnMouseEvent (3, point);
}

afx_msg void CTurtleController::OnRButtonUp(UINT nFlags, CPoint point)
{
//	CODController::OnRButtonUp(nFlags, point);
//	OnMouseEvent (4, point);
}

afx_msg void CTurtleController::OnMouseMove(UINT nFlags, CPoint point)
{
	CODController::OnMouseMove(nFlags, point);
	OnMouseEvent (5, point);
}

/////////////////////////////////////////////////////////////////////////////
// personalizzazioni

#include "secjpeg.h"

BOOL CTurtleController::SaveJpeg(LPCTSTR lpszFileName)
{
	// Save as Jpeg file

	if (GetVp() == NULL)
		return FALSE;

	SECJpeg jpeg;
	GetVp()->DrawImage(&jpeg);
	return jpeg.SaveImage(lpszFileName);
}

// BOOL CTurtleController::FileExportPrompt(CString& strFileName, DWORD* pFilterIdx, CString strTitle, CString strFilter)
BOOL CTurtleController::FileOpenSavePrompt(BOOL bOpenFileDialog, CString& strFileName, DWORD* pFilterIdx, CString strTitle, CString strFilter)
{
	DWORD dwFlags = OFN_OVERWRITEPROMPT;

//	CFileDialog dlgFile(FALSE,				// save dialog
	CFileDialog dlgFile(bOpenFileDialog,	// open or save dialog
	                    NULL,				// default extension
	                    NULL,				// initial filename
	                    dwFlags,			// flags
	                    strFilter,			// filter
	                    GetWnd());			// parent window

	dlgFile.m_ofn.lpstrTitle = strTitle;

	if (dlgFile.DoModal() == IDOK)
	{
		strFileName = dlgFile.GetPathName();

		if (pFilterIdx != NULL)
		{
			*pFilterIdx = dlgFile.m_ofn.nFilterIndex;
		}

		if (dlgFile.GetFileExt().IsEmpty())
		{
			switch (dlgFile.m_ofn.nFilterIndex)
			{
			case 1:
				strFileName += _T(".bmp");
				break;
			case 2:
				strFileName += _T(".jpg");
				break;
			case 3:
				strFileName += _T(".emf");
				break;
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CTurtleController::OnFileExport()
{
//	CODController::OnFileExport();

	CString strFileName;
	DWORD dwFilterIdx;
	CString strTitle = "Esporta Come";
	CString strFilter =
"File Bitmap (*.bmp)|*.bmp|File Jpeg (*.jpg)|*.jpg|Enhanced Metafile (*.emf)|*.emf||";

//	if (FileExportPrompt(strFileName, &dwFilterIdx, strTitle, strFilter))
	if (FileOpenSavePrompt(FALSE, strFileName, &dwFilterIdx, strTitle, strFilter))
	{
		switch (dwFilterIdx)
		{
		case 1:
			SaveDib(strFileName);
			break;

		case 2:
			SaveJpeg(strFileName);
			break;

		case 3:
			SaveMetafile(strFileName);
			break;
		}
	}
}


BOOL CTurtleController::SymbolLoadPrompt(CString& strFileName, CString strTitle, CString strFilter)
{
	DWORD dwFlags = 0;

	CFileDialog dlgFile(TRUE,                 // load dialog
	                    "sym",                // default extension
	                    NULL,                 // initial filename
	                    dwFlags,              // flags
	                    strFilter,            // filter
	                    GetWnd());            // parent window

	dlgFile.m_ofn.lpstrTitle = strTitle;

	if (dlgFile.DoModal() == IDOK)
	{
		strFileName = dlgFile.GetPathName();
		return TRUE;
	}

	return FALSE;
}

void CTurtleController::OnSymbolLoad()
{
	CString strFileName;
	CString strTitle = "Carica Simbolo";
	CString strFilter = "File Simbolo (*.sym)|*.sym||";

	if (! SymbolLoadPrompt(strFileName, strTitle, strFilter))
		return;
	CTurtleViewport* pVp = (CTurtleViewport*) GetVp();
	if (! pVp)
		return;
	CTurtleModel* pModel = (CTurtleModel*) GetModel();
	if (! pModel)
		return;
	CODSymbolComponent* pSymbol = pModel->SymbolLoad (strFileName);
	pModel->UpdateComponent(pSymbol, OD_UPDATE_CURRENT_POS);
}


// vedi anche ShowcaseCtlr.cpp
void CTurtleController::OnEdgeCreate()
{
	CTurtleModel* pModel = (CTurtleModel*) GetModel();
	if (! pModel) return;
	CODComponentSet* pSet = GetSelection();
	int nComps = pSet->GetSize();
	if (nComps != 2) return;
	CODSymbolComponent* pSourceComp = (CODSymbolComponent*) pSet->GetAt(0);
	CODSymbolComponent* pTargetComp = (CODSymbolComponent*) pSet->GetAt(1);

//	CODPortComponent* pSourcePort = pSourceComp->GetCenterPort();
//	CODPortComponent* pTargetPort = pTargetComp->GetCenterPort();
//	CODLinkComponent* pLinkComp = new(CODLinkComponent);
//	BOOL created = pLinkComp->Create(pSourcePort, pTargetPort);
//	pLinkComp->SetTargetEndpoint(new CODArrowEndpoint());
//	if (created) {
//		BOOL appended = pModel->AppendChild(pLinkComp);
//		CODConnection* pSourceConnection = pLinkComp->ConnectTail(pSourcePort);
//		CODConnection* pTargetConnection = pLinkComp->ConnectHead(pTargetPort);
//		pModel->UpdateComponent(pLinkComp, OD_UPDATE_CURRENT_POS);
//	}

	CODLinkComponent* pLinkComp =
		pModel->NodesConnect(pSourceComp, pTargetComp);
}

void CTurtleController::OnEdgeInvert()
{
	CTurtleModel* pModel = (CTurtleModel*) GetModel();
	if (! pModel) return;
	CODComponentSet* pSet = GetSelection();
	int nComps = pSet->GetSize();
	if (nComps != 1) return;
	CODLinkComponent* pLinkComp = (CODLinkComponent*) pSet->GetAt(0);

	pModel->EdgeInvert(pLinkComp);

//	CODSymbolComponent* pSourceComp = (CODSymbolComponent*) pLinkComp->GetTailComponent();
//	CODSymbolComponent* pTargetComp = (CODSymbolComponent*) pLinkComp->GetHeadComponent();
//	CODPortComponent* pSourcePort = pSourceComp->GetCenterPort();
//	CODPortComponent* pTargetPort = pTargetComp->GetCenterPort();

//	pLinkComp->DisconnectTail();
//	pLinkComp->DisconnectHead();

//	CODConnection* pSourceConnection = pLinkComp->ConnectTail(pTargetPort);
//	CODConnection* pTargetConnection = pLinkComp->ConnectHead(pSourcePort);

//	pModel->UpdateComponent(pLinkComp, OD_UPDATE_OLD_AND_NEW_POS);
}

#ifdef DOC // da CODController (OdController.cpp)

//@mfunc Prompts the user for a filename and format to export diagram into.
//@rdesc TRUE if user selected a filename, FALSE if the user cancelled.
//@parm Output parameter for filename.
//@parm Returns the index of the file filter selected.
//@parm String resource ID for dialog title.
//@parm String resource ID for file filter.
//@comm This function displays a file save dialog that allows the user to
// select a filename to export to.
BOOL CODController::FileExportPrompt(CString& strFileName, DWORD* pFilterIdx, UINT nTitleID, UINT nFilterID)
{
	CString title;
	VERIFY(title.LoadString(nTitleID));
	CString strFilter;
	VERIFY(strFilter.LoadString(nFilterID));

	DWORD dwFlags = OFN_OVERWRITEPROMPT;

	CFileDialog dlgFile(FALSE,                // save dialog
	                    NULL,                 // default extension
	                    NULL,                 // initial filename
	                    dwFlags,              // flags
	                    strFilter,            // filter
	                    GetWnd());            // parent window

	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal() == IDOK)
	{
		strFileName = dlgFile.GetPathName();

		if (pFilterIdx != NULL)
		{
			*pFilterIdx = dlgFile.m_ofn.nFilterIndex;
		}

		if (dlgFile.GetFileExt().IsEmpty())
		{
			if (dlgFile.m_ofn.nFilterIndex == 2)
			{
				strFileName += _T(".emf");
			}
			else
			{
				strFileName += _T(".bmp");
			}
		}

		return TRUE;
	}

	return FALSE;
}

//@mfunc Exports the viewport to a selected file format.
//@rdesc void
//@comm Handles the ID_OD_FILE_EXPORT command message by prompting the
// user for a filename and file format. The <mf CODController::FileExportPrompt>
// function returns an index into the file filter combo box on the
// file save dialog. That index is used to determine which file export
// function to call. This function will call either <mf CODController::SaveDib>
// or <mf CODController::SaveMetafile> as a result.
void CODController::OnFileExport()
{
	CString strFileName;
	DWORD dwFilterIdx;

	if (FileExportPrompt(strFileName, &dwFilterIdx))
	{
		switch (dwFilterIdx)
		{
		case 1:
			SaveDib(strFileName);
			break;

		case 2:
			SaveMetafile(strFileName);
			break;
		}
	}
}

//@mfunc Saves contents of the viewport to a Windows bitmap.
//@rdesc TRUE if successfully saved; otherwise FALSE.
//@parm Name of file to create.
//@comm This function creates a Windows bitmap (DIB) file and saves
// the contents of the viewport to it.
BOOL CODController::SaveDib(LPCTSTR lpszFileName)
{
	// Save as Windows bitmap

	if (GetVp() == NULL)
		return FALSE;

	SECDib dib;
	GetVp()->DrawImage(&dib);
	return dib.SaveImage(lpszFileName);
}

//@mfunc Saves the contents of the viewport to an enhanced metafile.
//@rdesc TRUE if viewport successfully saved; otherwise FALSE.
//@parm Name of file to create.
//@comm This function creates an enhanced metafile and renders the
// viewport into it.
BOOL CODController::SaveMetafile(LPCTSTR lpszFileName)
{
	// Save as enhanced metafile

	if (GetVp() == NULL)
		return FALSE;

	CFile outFile(lpszFileName, CFile::modeCreate | CFile::modeWrite);
	return GetVp()->WriteMetafile(outFile);
}

//@mfunc Toggles debug drawing mode. Debug drawing shows the regions
// and bounding boxes of all components in the diagram.
//@rdesc void
//@comm Debug drawing is only available in the debug versions of the library.
void CODController::OnDebugDraw()
{
#ifdef _DEBUG
	CODGlobal::SetDebugDraw(!CODGlobal::GetDebugDraw());
	GetVp()->UpdateAll();
#endif
}

#endif // DOC

// non sembra necessario modificarla:
// conviene fare override di StartTextEdit (a più basso livello) 
void CTurtleController::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CODController::OnLButtonDblClk(nFlags, point);

}

// Ripreso in parte da ToolFrame
BOOL OgDoubleClicked(CTurtleView* pView, ODComponentId lID)
{
	DlgEntry turtleEntry = pView->m_dlgEntry;
	node sons = GetSons(turtleEntry);
	DlgEntry goEntry;
	int id;
	node setup;
	pView->m_ogID = lID;
	while (sons) {
		goEntry = car (sons);
		ASSERT(goEntry);
		sons = cdr (sons);
		id = GetId(goEntry);
		if (id == lID) {
			setup = GetSetup (goEntry);
			if (setup != NULLP) {
				int oldHandle = i_tarta;
				int newHandle = pView->m_nView;
				if (newHandle != oldHandle)
					switch_tarta (newHandle);
				NestedExec (setup);
				if (newHandle != oldHandle)
					switch_tarta (oldHandle);
			}
			return TRUE;
		}
	}
	TRACE("invalid ODComponentId");
	return FALSE;
}

BOOL CTurtleController::StartTextEdit(UINT nFlags, CPoint ptDev)
{
	nFlags; // unused

	CPoint ptLog = ptDev;
	VpDPtoLP(&ptLog);

	m_pCompHit = GetVp()->ComponentHitTest(ptLog);

	// test da completare
	if (m_pCompHit != NULL) {
		ODComponentId lID;
		lID = m_pCompHit->GetID();
		CTurtleView* pView = (CTurtleView*) GetVp();
		BOOL ok = OgDoubleClicked(pView, lID);
		if (ok)
			return FALSE;	// non deve eseguire il comportamento standard
	}

	return CODController::StartTextEdit(nFlags, ptDev);
}

// Ripreso da ToolFrame
// ATTENZIONE ! E' diverso da AddControl !! la lista dei figli
// non contiene i nomi ma direttamente gli entry !!
node AddGoEntry (DlgEntry turtleEntry, ODComponentId lID, node setup)
{
	DlgEntry goEntry = CreateDlgEntry (NULL, 0, lID, turtleEntry, NULLP, NULLP, setup, NULLP);
	node name = NULLP;
//	AddEntry(&Dialogs, goEntry, name);

	PutSons (turtleEntry, cons (goEntry, GetSons (turtleEntry), LIST));
	return goEntry;
}

// Ripreso da ToolFrame
// ATTENZIONE ! E' diverso da AddControl !! la lista dei figli
// non contiene i nomi ma direttamente gli entry !!
BOOL RemoveGoEntry (DlgEntry turtleEntry, ODComponentId lID)
{
	BOOL ok = FALSE;
	node sons = GetSons(turtleEntry);
	DlgEntry goEntry;
	int id;
	while (sons) {
		goEntry = car (sons);
		ASSERT(goEntry);
		sons = cdr (sons);
		id = GetId (goEntry);
		if (id == lID) {
//			RemoveEntry (&Dialogs, goEntry);
			lstdel (&sons, goEntry);	// attenzione: sostituzione senza controllo
			PutSons (turtleEntry, sons);
			ok = TRUE;
		}
	}
	return ok;
} 

// Accende (programma) un oggetto grafico
void PR_GO_ON ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lID = num_long (args[0]);
	CODComponent* pComp = pView->GetModel()->GetComponent(lID);
	if (! pComp) return;

	node goSetup = args[1];
	if (goSetup != NULLP)
		DlgEntry goEntry = AddGoEntry (pView->m_dlgEntry, lID, goSetup);
}

// Spegne (sprogramma) un oggetto grafico
void PR_GO_OFF ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleModel* pModel = (CTurtleModel*) pView->GetModel();

	ODComponentId lID = num_long (arg_1);
	CODComponent* pComp = pView->GetModel()->GetComponent(lID);
	if (! pComp) return;

	BOOL ok = RemoveGoEntry (pView->m_dlgEntry, lID);
}

// Riporta ID di oggetto grafico doppiocliccato
void PR_GO_ID ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;

	if (pView->m_ogID >= 0)
		risultato = long_num (pView->m_ogID);
}

// overridden per renderlo pubblico
void CTurtleController::OnInsertSymbol (CODSymbolComponent* pSymbol)
{
	CODController::OnInsertSymbol(pSymbol);
}

// overridden per renderlo pubblico e prendere coordinate
void CTurtleController::InsertSymbol(UINT nFlags, CPoint ptDev)
{
	CODController::InsertSymbol(nFlags, ptDev);

	// Convert to logical coordinates
	CPoint ptLog = ptDev;
	VpDPtoLP(&ptLog);

	m_ptInsertPoint = ptLog;
	m_bInserted = TRUE;
}


void CTurtleController::PageSetup()
{
	CODController::OnPageSetup();
}

void PR_PAGESETUP ()
{
	CTurtleView* pView = theApp.m_turtleViews[i_tarta];
	if (! pView->m_isOVs) return;
	CTurtleController* pController = (CTurtleController*) pView->GetCtlr();

	pController->PageSetup();
}

void CTurtleController::SpecialZoom (int function)
{
	if (function == 0)
		OnZoomNormal();
	else if (function == 1)
		OnZoomFit();
	else if (function == 2)
		OnZoomSelection();
}

// 
// da CODController::SelectAll()
void CTurtleController::AddToSelection (CODComponentSet* pComps)
{
	CODComponentIterator i(pComps);
	CODComponent* pComp = NULL;
	for (pComp = i.GetFirst(); pComp != NULL; pComp = i.GetNext())
	{
		m_setSelection.Add(pComp);
	}
	//Perform the selection
	OnSelectionChange(pComps);
	// Reset the state
	m_state = OD_SELECT_READY;
}

// overridden per renderlo pubblico
void CTurtleController::OnPan ()
{
	CODController::OnPan ();
}

// overridden per renderlo pubblico
void CTurtleController::OnDrawSelect ()
{
	CODController::OnDrawSelect ();
}


BOOL CTurtleController::FileLoad(CString PathName)
{
	CTurtleModel* pModel = (CTurtleModel*) GetModel();
	CTurtleView* pView = (CTurtleView*) GetVp();

	BOOL loadResult = FALSE;
	CString strFileName;
	DWORD dwFilterIdx = 0;
	CString strTitle = "Recupera";
	CString strFilter =
"Modello di Iperlogo - Tarta a oggetti (*.ilm)|*.ilm||";

	if (PathName.GetLength() != 0) {
		strFileName = PathName;
		dwFilterIdx = 1;
	}
	else if (! FileOpenSavePrompt(TRUE, strFileName, &dwFilterIdx, strTitle, strFilter))
		return FALSE;

	switch (dwFilterIdx)
	{
	case 1:
		int result;
		UINT nOpenFlags = CFile::modeRead;
		CFile file;
		result = file.Open (strFileName, nOpenFlags, NULL);
		if (result) {
			UINT nMode = CArchive::load;
			int nBufSize = 512;
			void FAR* lpBuf = NULL;
			CArchive archive (&file, nMode, nBufSize, lpBuf);
			pModel->Serialize(archive);
			archive.Close ();	
			file.Close ();
			loadResult = TRUE;
		}
		break;
	}
	if (loadResult) {
		strcpy (pView->m_saveFile, PathName);
	}
	return loadResult;
}

BOOL CTurtleController::FileSave(CString PathName, BOOL bSaveAs)
{
	CTurtleModel* pModel = (CTurtleModel*) GetModel();
	CTurtleView* pView = (CTurtleView*) GetVp();

	BOOL saveResult = FALSE;
	CString strFileName;
	DWORD dwFilterIdx = 0;
	CString strTitle = "Salva Come";
	CString strFilter =
"Modello di Iperlogo - Tarta a oggetti (*.ilm)|*.ilm||";

	if (PathName.GetLength() != 0) {
		strFileName = PathName;
		dwFilterIdx = 1;
	}
	else if (bSaveAs || (! pView->m_saveFile[0]))
		if (! FileOpenSavePrompt(FALSE, strFileName, &dwFilterIdx, strTitle, strFilter))
			return FALSE;

	switch (dwFilterIdx)
	{
	case 1:
		int result;
		UINT nOpenFlags = CFile::modeCreate | CFile::modeWrite;
		CFile file;
		result = file.Open (strFileName, nOpenFlags, NULL);
		if (result) {
			UINT nMode = CArchive::store;
			int nBufSize = 512;
			void FAR* lpBuf = NULL;
			CArchive archive (&file, nMode, nBufSize, lpBuf);
			pModel->Serialize(archive);
			archive.Close ();	
			file.Close ();
			saveResult = TRUE;
		}
		break;
	}
	if (saveResult) {
		strcpy (pView->m_saveFile, PathName);
		pView->SetModify (FALSE);
	}
	return saveResult;
}

void CTurtleController::OnFileOpen()
{
	FileLoad("");
}

void CTurtleController::OnFileSave()
{
	FileSave("", FALSE);
}

void CTurtleController::OnFileSaveAs()
{
	FileSave("", TRUE);
}
