/* MENU.CPP

000308	corretto PR_MENUELEMENTREPLACE
991105	corretto RemoveMenuEntry
991104	nuove primitive
991027	gestione di popup ricorsivi (di livello inferiore)
990921	implementazione comandi in modo simile a toolbar e dialoghi:
		AddMenuEntry, RemoveMenuEntry
990917	prima versione - vedi anche ToolBar.cpp
*/

#include "stdafx.h"
#include "iperlogo.h"
#include "ilcpp.h"
#include "ildialog.h"
#include "turtlevw.h"
// #include "/il32/resource.h"
#include "resource.h"

extern int dev2handle (int dev);
extern CWnd* TurtleHandle2Frame (int handle);

CMenu* IlMenuCreate (node menuList, CTurtleView* turtleView);

// Ripreso da ToolFrame (vedi anche AddGoEntry in turtleCtrl.cpp)
// ATTENZIONE ! E' diverso da AddControl !! la lista dei figli
// non contiene i nomi ma direttamente gli entry !!
node AddMenuEntry (DlgEntry turtleEntry, UINT nID, node setup)
{
	DlgEntry menuEntry = CreateDlgEntry (NULL, 0, nID, turtleEntry, NULLP, NULLP, setup, NULLP);
	node name = NULLP;
//	AddEntry(&Dialogs, turtleEntry, name);

	PutSons (turtleEntry, cons (menuEntry, GetSons (turtleEntry), LIST));
	return menuEntry;
}

// (vedi anche ToolFrame e AddGoEntry in turtleCtrl.cpp)
// ATTENZIONE ! E' diverso da AddControl !! la lista dei figli
// non contiene i nomi ma direttamente gli entry !!
BOOL RemoveMenuEntry (DlgEntry turtleEntry, UINT nID)
{
	node sons = GetSons(turtleEntry);
	DlgEntry menuEntry;
	int id;
	node tail = sons;
	while (tail) {
		menuEntry = car (tail);
		ASSERT(menuEntry);
		id = GetId (menuEntry);
		if (id == nID) {
// attenzione: eliminazione di cella di lista, senza controllo
// di esistenza e con modifica della lista stessa !!
			lstdel (&sons, tail);
			PutSons (turtleEntry, sons);
			return TRUE;
		}
		tail = cdr (tail);
	}
	return FALSE;
} 

BOOL IlMenuPopupAppend (CMenu* pMenu, HMENU hMenu, LPCTSTR lpszNewItem) {
	UINT nFlags = MF_POPUP;
	UINT nIDNewItem = (UINT) hMenu;
	if (! lpszNewItem)
		nFlags = MF_SEPARATOR;
	BOOL appended =
		pMenu->AppendMenu(nFlags, nIDNewItem, lpszNewItem);
	return appended;
}

CMenu* IlMenuPopupCreate (node popupList, DlgEntry turtleEntry)
{
	CMenu* pMenu = new(CMenu);
	BOOL created = pMenu->CreatePopupMenu();
	if (! created)
		return NULL;
	node itemList;
	int nIDNewItem;
	node setup;
	char szItemName [MAX_CHARS];
	while (popupList != NULLP) {
		itemList = car (popupList);
		popupList = cdr (popupList);

		nIDNewItem = num_int (car (itemList));
		if (nIDNewItem == -1)
			pMenu->AppendMenu(MF_SEPARATOR);
		else {
			itemList = cdr (itemList);
			MemScrivi (szItemName, car (itemList));
			itemList = cdr (itemList);
			setup = car (itemList);

			if (nIDNewItem == 0) {	// popup menu di livello inferiore
				CMenu* pMenuPopup = IlMenuPopupCreate (setup, turtleEntry);
				if (! pMenuPopup)
					return NULL;
				HMENU hMenu = pMenuPopup->GetSafeHmenu();
				BOOL appended =
					pMenu->AppendMenu(MF_STRING | MF_POPUP, (UINT) hMenu, szItemName);
			}
			else {					// elemento normale di popup menu
				pMenu->AppendMenu(MF_STRING, (UINT) nIDNewItem, szItemName);
				AddMenuEntry (turtleEntry, (UINT) nIDNewItem, setup);
			}
		}
	}
	return pMenu;
}

CMenu* IlMenuCreate (node menuList, CTurtleView* turtleView)
{

	CMenu* pMenu = new(CMenu);
	BOOL created = pMenu->CreateMenu();
	if (! created)
		return NULL;

	CMenu* pMenuPopup;
	HMENU hMenu;
	node popupName, popupList;
	char szPopupName [MAX_CHARS];
	BOOL appended = TRUE;
	while (appended && (menuList != NULLP)) {
		popupName = car (menuList);
		menuList = cdr (menuList);
		popupList = car (menuList);
		menuList = cdr (menuList);

		MemScrivi (szPopupName, popupName);
		pMenuPopup = IlMenuPopupCreate (popupList, turtleView->m_dlgEntry);
		if (! pMenuPopup)
			return NULL;
		hMenu = pMenuPopup->GetSafeHmenu();
		appended = IlMenuPopupAppend (pMenu, hMenu, szPopupName);
	}
	return pMenu;
}

#ifdef DOC
typedef struct tagMENUITEMINFO {
    UINT    cbSize; 
    UINT    fMask; 
    UINT    fType; 
    UINT    fState; 
    UINT    wID; 
    HMENU   hSubMenu; 
    HBITMAP hbmpChecked; 
    HBITMAP hbmpUnchecked; 
    DWORD   dwItemData; 
    LPTSTR  dwTypeData; 
    UINT    cch; 
} MENUITEMINFO, FAR *LPMENUITEMINFO;
#endif // DOC

// input: (sotto)menu, pos / -ID
// riporta: ID [,nome [, n.elementi]]
BOOL MenuItemGetInfo (CMenu* pMenu, int iPos,
					  int* pItemID, char* pItemName, int* pItemCount)
{
	BOOL ByPos = FALSE;
	UINT nIDItem;
	if (iPos < 0)
		nIDItem = (UINT) (-iPos);
	else {
		int itemCount = pMenu->GetMenuItemCount();
		if ((iPos < 1) || (iPos > itemCount))
			return FALSE;
		nIDItem = (UINT) iPos;
		ByPos = TRUE;
	}

	MENUITEMINFO itemInfo;
	itemInfo.cbSize = sizeof(MENUITEMINFO);
	itemInfo.fMask = MIIM_ID | MIIM_STATE | MIIM_TYPE | MIIM_SUBMENU;
	itemInfo.dwTypeData = pItemName;
	itemInfo.cch = MAX_CHARS;
	BOOL success =
		pMenu->GetMenuItemInfo(nIDItem, &itemInfo, ByPos);
	int nID;
	if (! success)
		return FALSE;	// ??

	if (itemInfo.hSubMenu)
		nID = 0;
	else if ((itemInfo.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
		nID = -1;
	else
		nID = itemInfo.wID;
	*pItemID = nID;

	if (! ((itemInfo.fType & MFT_STRING) == MFT_STRING))
		*pItemName = 0;

	if (itemInfo.hSubMenu)
		*pItemCount = GetMenuItemCount(itemInfo.hSubMenu);
	else
		*pItemCount = 0;

	return TRUE;
}

// rimuove elemento di menu ed eventuale discendenza da dialoghi
// eventualmente rimuove elemento di menu dal menu
BOOL MenuItemRemove (node turtleEntry, CMenu* pMenu, int iPos, BOOL remove)
{
	int nItemID, nItemCount;
	char szItemName [MAX_CHARS];
	BOOL success =
	MenuItemGetInfo (pMenu, iPos, &nItemID, &szItemName[0], &nItemCount);
	if (! success)
		return FALSE;

	if (nItemID == -1)		// separator
		return TRUE;
	else if (nItemID > 0)	// normal item
		BOOL removed = RemoveMenuEntry (turtleEntry, (UINT) nItemID);
	else {					// popup
		CMenu* pMenuPopup = pMenu->GetSubMenu(iPos);
		int iItem;
		for (iItem=0; iItem<nItemCount; iItem++)
			success = MenuItemRemove (turtleEntry, pMenuPopup, iItem, FALSE);
	}

	if (remove)
		pMenu->RemoveMenu(iPos, MF_BYPOSITION);
	return TRUE;
}

//----------

void PR_MENUCREATE ()
{
	node parent = args[0];	// nome finestra
	int parentHandle = -1;
	CTurtleView* turtleView;
	CFrameWnd* parentWindow = NULL;
	int parentDev = nome2dev (parent);
	if ((parentDev != -1) && (_fstato [parentDev] & O_TARTA)) {
		parentHandle = dev2handle (parentDev);
		turtleView = theApp.m_turtleViews[parentHandle];
		parentWindow = (CFrameWnd*) turtleView->m_frame;
	}
	else err2 (52, parent);
	node menuList = args[1];
	CMenu* pMenu;

	pMenu = IlMenuCreate (menuList, turtleView);
	if (pMenu && (pMenu->GetMenuItemCount() > 0))
		BOOL set = parentWindow->SetMenu(pMenu);
	else
		err2 (5, args[1]);
}

void PR_MENUDELETE ()
{
}

void PR_MENU ()
{
}

// input: nome finestra, lista di identificazione [pos pos .. pos/-ID]
// output: lista di: ID [nome [numero elementi]]
void PR_MENUELEMENT ()
{
	node parent = args[0];	// nome finestra
	int parentHandle;
	CTurtleView* turtleView;
	CFrameWnd* parentWindow = NULL;
	int parentDev = nome2dev (parent);
	if ((parentDev != -1) && (_fstato [parentDev] & O_TARTA)) {
		parentHandle = dev2handle (parentDev);
		turtleView = theApp.m_turtleViews[parentHandle];
		parentWindow = (CFrameWnd*) turtleView->m_frame;
	}
	else { err2 (52, parent); return; }
	CMenu* pMenu = parentWindow->GetMenu();
	if (! pMenu)
		err2 (52, parent);

	if (args[1] == NULLP)
		{ err2 (5, args[1]); return; }
	node posList = args[1];

// NOTA: l'ultimo accesso avviene fuori dal ciclo perché:
// - non ci interessa l'oggetto ma l'informazione
// - l'identificazione può avvenire per ID anziché per posizione
	UINT itemCount, iPos;
	int posCount = lstlen (posList);
	while (posCount-- > 1) {
		itemCount = pMenu->GetMenuItemCount();
		iPos = num_int (car (posList));
		posList = cdr (posList);

		if ((iPos < 1) || (iPos > itemCount))
			{ err2 (5, args[1]); return; }
		--iPos;
		pMenu = pMenu->GetSubMenu(iPos);
	}

	itemCount = pMenu->GetMenuItemCount();
	iPos = num_int (car (posList));
	if ((itemCount < 1) || ((iPos < 1) || (iPos > itemCount)))
		{ err2 (5, args[1]); return; }
	--iPos;

	int nItemID, nItemCount;
	char szItemName [MAX_CHARS];
	BOOL success =
	MenuItemGetInfo (pMenu, iPos, &nItemID, &szItemName[0], &nItemCount);
	if (! success)
		{ err2 (5, args[1]); return; }
	risultato = NULLP;
	if (nItemID == 0)
		risultato = cons (int_num (nItemCount), risultato, LIST);
	if (strlen(szItemName) > 0)
		risultato = cons (logstr (szItemName), risultato, LIST);
	risultato = cons (int_num (nItemID), risultato, LIST);
}

void PR_MENUELEMENTSET ()
{
}

void PR_MENUELEMENTREMOVE ()
{
	node parent = args[0];	// nome finestra
	int parentHandle;
	CTurtleView* turtleView;
	CFrameWnd* parentWindow = NULL;
	int parentDev = nome2dev (parent);
	if ((parentDev != -1) && (_fstato [parentDev] & O_TARTA)) {
		parentHandle = dev2handle (parentDev);
		turtleView = theApp.m_turtleViews[parentHandle];
		parentWindow = (CFrameWnd*) turtleView->m_frame;
	}
	else { err2 (52, parent); return; }
	CMenu* pMenu = parentWindow->GetMenu();
	if (! pMenu)
		err2 (52, parent);

	if (args[1] == NULLP)
		{ err2 (5, args[1]); return; }
	node posList = args[1];

// NOTA: l'ultimo accesso avviene fuori dal ciclo perché:
// - è necessario conoscere il parent di elemento o popup
	UINT itemCount, iPos;
	int posCount = lstlen (posList);
	while (posCount-- > 1) {
		itemCount = pMenu->GetMenuItemCount();
		iPos = num_int (car (posList));
		posList = cdr (posList);

		if ((iPos < 1) || (iPos > itemCount))
			{ err2 (5, args[1]); return; }
		--iPos;
		pMenu = pMenu->GetSubMenu(iPos);
	}

	itemCount = pMenu->GetMenuItemCount();
	iPos = num_int (car (posList));
	if ((itemCount < 2) || ((iPos < 1) || (iPos > itemCount)))
		{ err2 (5, args[1]); return; }
	--iPos;
	BOOL success =
	MenuItemRemove (turtleView->m_dlgEntry, pMenu, iPos, TRUE);
	parentWindow->DrawMenuBar();
}

// iPos parte da 0 se rappresenta posiz. di item da sostituire
// riporta sempre indice (da 0) di elemento appeso o modificato
int IlMenuItemAddReplace (node turtleEntry, CMenu* pMenu, int iPos, node newItem)
{
	int itemCount = pMenu->GetMenuItemCount();

	if (iPos != -1) {
		// rimozione logica dell'elemento: ID/IDs di elemento semplice/popup)
		BOOL removed =
		MenuItemRemove (turtleEntry, pMenu, iPos, FALSE);
	}

	node id = car (newItem);
	newItem = cdr (newItem);
	if ((typ (id) != LBCD))
		return -1;
	int nIDNewItem = num_int (id);

	BOOL appended, replaced, removed, inserted, infoOk;
	// separator
	if (nIDNewItem == -1) {
		if (iPos == -1)
			appended =
//			pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR) NULL);
			pMenu->AppendMenu(MF_SEPARATOR);
		else
			replaced =
//			pMenu->ModifyMenu(iPos, MF_BYPOSITION | MF_SEPARATOR, 0, (LPCTSTR) NULL);
			pMenu->ModifyMenu(iPos, MF_BYPOSITION | MF_SEPARATOR);
	}
	else {
		char szItemName [MAX_CHARS];
		MemScrivi (szItemName, car (newItem));
		newItem = cdr (newItem);
		// normal menu item (command)
		if (nIDNewItem > 0) {
			node setup = car (newItem);
			AddMenuEntry (turtleEntry, nIDNewItem, setup);
			if (iPos == -1)
				appended =
				pMenu->AppendMenu(MF_BYPOSITION | MF_STRING, (UINT) nIDNewItem, szItemName);
			else {
				// ModifyMenu non sostituisce bene un popup con un elemento semplice ?
				int nOldItemID, nOldItemCount;
				char szOldItemName [MAX_CHARS];
				infoOk = MenuItemGetInfo (pMenu, iPos, &nOldItemID, &szOldItemName[0], &nOldItemCount);
				// menu item vecchio era popup
				if (nOldItemID == 0) {
					removed =
					pMenu->RemoveMenu(iPos, MF_BYPOSITION);
					inserted =
					pMenu->InsertMenu(iPos, MF_BYPOSITION | MF_STRING, (UINT) nIDNewItem, szItemName);
				}
				// menu item vecchio non era popup
				else {
					replaced =
					pMenu->ModifyMenu(iPos, MF_BYPOSITION | MF_STRING, (UINT) nIDNewItem, szItemName);
				}
			}
		}
		// popup
		else if (nIDNewItem == 0) {
			node popupList = car (newItem);
			CMenu* pMenuPopup = IlMenuPopupCreate (popupList, turtleEntry);
			if (! pMenuPopup)
				return -3;
			HMENU hMenu = pMenuPopup->GetSafeHmenu();
			if (iPos == -1)
				appended =
				pMenu->AppendMenu(MF_STRING | MF_POPUP, (UINT) hMenu, szItemName);
			else
				replaced =
				pMenu->ModifyMenu(iPos, MF_BYPOSITION | MF_STRING | MF_POPUP, (UINT) hMenu, szItemName);
		}
	}

	return (iPos == -1) ? pMenu->GetMenuItemCount() - 1 : iPos;
}

// void PR_MENUSETSUBMENU ()
// appende o sostituisce un elemento di menu
// args: finestra tarta, lista posizione, lista nuovo elemento
void PR_MENUELEMENTREPLACE ()
{
	node parent = args[0];	// nome finestra
	int parentHandle;
	CTurtleView* turtleView;
	CFrameWnd* parentWindow = NULL;
	int parentDev = nome2dev (parent);
	if ((parentDev != -1) && (_fstato [parentDev] & O_TARTA)) {
		parentHandle = dev2handle (parentDev);
		turtleView = theApp.m_turtleViews[parentHandle];
		parentWindow = (CFrameWnd*) turtleView->m_frame;
	}
	else { err2 (52, parent); return; }
	CMenu* pMenu = parentWindow->GetMenu();
	if (! pMenu)
		err2 (52, parent);

	if (args[1] == NULLP)
		{ err2 (5, args[1]); return; }
	node posList = args[1];
	CMenu* pMenuPopup = pMenu;
	UINT itemCount, iPos;
	while (posList) {
		pMenu = pMenuPopup;
		itemCount = pMenu->GetMenuItemCount();
		iPos = num_int (car (posList));
		posList = cdr (posList);
//>000308
		if (iPos == (itemCount + 1))
			iPos = -1;
//<
		if (iPos == -1)
			break;
		if ((iPos < 1) || (iPos > itemCount))
			{ err2 (5, args[1]); return; }
		--iPos;
		pMenuPopup = pMenu->GetSubMenu(iPos);
	}

	if (iPos != -1)
		BOOL removed =
		MenuItemRemove (turtleView->m_dlgEntry, pMenu, iPos, FALSE);

	int success =
	IlMenuItemAddReplace (turtleView->m_dlgEntry, pMenu, iPos, args[2]);
	if (success < 0)
		{ err2 (5, args[2]); return; }
}
