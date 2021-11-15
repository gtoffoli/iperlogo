/* ILGUI.CPP	G. Toffoli

000511	in EditBoxCreate aggiunto scroll-bar se altezza >= 20
000204	LISTBOXGETSELECT e COMBOBOXGETTEXT riportano stringa anziché lista !!
000202	aggiunto LISTBOXRESET
991215	in DialogFileOpenClose (DIALOGFILEOPEN e DIALOGFILESAVE) agg. titolo
991212	aggiunto DIALOGSETFOCUS
991128	in DialogCreate gestione di is_nestedExec (vedi ilmain.cpp)
991012	mod. stile in ComboBox; aggiunto controllo EditBox (GETTEXT riporta stringa)
		mod. stile in Dialogo; posto owner di Dialogo a NULL
991011	Quasi ovunque mod. 5^ parametro in AddControl
991008	in DialogCreate tolto font da template
		aggiunto controllo a ListBoxDeleteString e ComboBoxDeleteString
		simulato dialogo MODAL con dialogo MODELESS e ciclo di attesa
991007	in WINDOWCREATE: modific. num. argomenti
		in DialogCreate: agg. WS_CLIPCHILDREN
		corretto dialogo modale: non usava il template
		riempito DIALOGDELETE (era vuoto)
990623	rinominato GetParent -> DialogGetParent
		stesso per GetWindow e PutWindow
990622	rinominato RemoveEntry -> RemoveEntryByName
990617	spostato definizioni IDC_.. in ildialog.h
990126	IDC_LISTBOX da 1000 a 9000 per conflitto con IDs in resource.h
990110	completato DIALOGFILEOPEN e DIALOGFILESAVE
981124	iniziato DIALOGFILEOPEN e DIALOGFILESAVE
980831	eliminato extern "C" per conversione a C++ dei moduli C
		eliminato il case EDIT: da DeleteSons (non usato ?)
		eliminato err_pochi () : SOSTITUIRE !!!!!!!!!!!!!!!!!!
980505	ritoccato LISTBOX
970908	per compatibilita' VC++5.0: bool -> bool_, _fstrcpy -> strcpy, _fmemcpy -> memcpy,
		WIN32: DLGTEMPLATE e' predefinito; WIN16: modificato nomi campi in DLGTEMPLATE
		eliminato _far (obsoleto) 
970503	recupero update del 02.10.96 (normalizzazione)
960531	prima versione
*/

#include "stdafx.h"
#include "Iperlogo.h"
#include "Modeless.h"

#include "ilcpp.h"
#include "ildialog.h"

int MatchKw (node, node);

extern node risultato;
extern int n_argomenti;
extern int is_nestedExec;

DlgList Dialogs = NULLP;

int MessBox (char *s)
{
	CString msg = s;
	return AfxMessageBox (msg, MB_OK, 0);
}

void ArgsToBox (int i, DLGTEMPLATE *pTemplate)
{
	pTemplate->x = (int) node2real (args[i]); 
	pTemplate->y = (int) node2real (args[i+1]); 
	pTemplate->cx = (int) node2real (args[i+2]); 
	pTemplate->cy = (int) node2real (args[i+3]); 
}

void ArgsToRect (CRect *lpRect, CDialog* dialog, int i)
{
#ifdef PIXEL_UNITS
	DWORD lDlgBaseUnits = GetDialogBaseUnits();
	int xpos = (int) (node2real (args[i]) / LOWORD(lDlgBaseUnits) * 4) ; 
	int ypos = (int) (node2real (args[i+1]) / HIWORD(lDlgBaseUnits) * 8); 
	int width = (int) (node2real (args[i+2]) / LOWORD(lDlgBaseUnits) * 4); 
	int height = (int) (node2real (args[i+3]) / HIWORD(lDlgBaseUnits) * 8); 
	lpRect->SetRect(xpos, ypos, xpos+width-1, ypos+height-1);
#else // PIXEL_UNITS
	int xpos = (int) node2real (args[i]); 
	int ypos = (int) node2real (args[i+1]); 
	int width = (int) node2real (args[i+2]); 
	int height = (int) node2real (args[i+3]); 
	lpRect->SetRect(xpos, ypos, xpos+width-1, ypos+height-1);
	dialog->MapDialogRect(lpRect);
#endif // PIXEL_UNITS
}

DlgEntry AddControl (DlgEntry parent, CWnd* control, int ControlType, int id, node label, node setup) {
	DlgEntry entry = CreateDlgEntry(control, ControlType, id, args[0], label, NULLP, setup, NULLP);
	AddEntry(&Dialogs, entry, args[1]);
	PutSons (parent, cons (args[1], GetSons (parent), LIST));
	return entry;
}

void RemoveControl (DlgList *pDialogs, DlgEntry entry, node name) {
	node sons;
	node parentname = DialogGetParent (entry);
	DlgEntry parent = NameToEntry (Dialogs, parentname, DIALOG); 
	sons = GetSons (parent);
	RemoveEle (&sons, name);
	PutSons (parent, sons);
//	RemoveEntry(pDialogs, name);
	RemoveEntryByName(pDialogs, name);
}

void DialogCreate (int controlType)
{
#define pc_FALSO	kw [2]

	int result;
	char captionText [MAX_CHARS];
	CWnd *pParent;
	CDialog *pDialog;
	CModelessDialog *pModelessDialog;
	pDialog = pModelessDialog =
		new(CModelessDialog);

	// buffer dimensionato per eccesso, che inizia con un Dialog template
	char Template [MAX_CHARS];
	memset(Template, 0, MAX_CHARS);
	DLGTEMPLATE* pTemp = (DLGTEMPLATE*) &Template[0];

	DlgEntry entry;
	int id = 0;
	node parent = args[0];
	node name = args[1];
	node title = args[2];

 	if (entry = NameToEntry (Dialogs, parent, TURTLE)) {
    	pParent = DialogGetWindow(entry);
//		pTemp->style = WS_CHILD | WS_VISIBLE | DS_SETFONT;
		pTemp->style = WS_POPUP | WS_VISIBLE | DS_SETFONT; // 991012 ?
	}
 	else {
//		pParent = CWnd::GetDesktopWindow();
		pParent = NULL;					// 991008
		pTemp->style = WS_POPUP | WS_VISIBLE | DS_SETFONT;
		pTemp->style |= WS_EX_TOPMOST;	// 991012
	}
	pTemp->style |= WS_CLIPCHILDREN | DS_CENTERMOUSE;	// 991012
//	pTemp->style |= DS_CONTROL;							// 991012

	char* szBoxCaption;
	if (! MatchKw (title, pc_FALSO)) {
		pTemp->style |= WS_CAPTION;

		if (types[2] == LIST) MemScrivi (captionText, title);
		else getnam (captionText, title);
		szBoxCaption = (captionText[0]) ? &captionText[0] : "Dialogo";
	}

	Geometry geometry = NULLP;
//	node setup = args[7];
	node setup = (n_argomenti > 7) ? args[7] : NULLP;	// 991007
	node sons = NULLP;

	pTemp->cdit = 0;	// count of dialog items
	ArgsToBox (3, pTemp);
                  
	// complete the DLGTEMPLATE structure
	BYTE* pdest = (BYTE*) pTemp + sizeof(DLGTEMPLATE);
	// no menu
	pdest += sizeof(WORD);
	// use default window class
	pdest += sizeof(WORD);
#ifdef OLD_TITLE
	// dialog title
	strcpy ((char*) pdest, szBoxCaption);
	pdest += strlen (szBoxCaption)+1;
#else // OLD_TITLE
	if (MatchKw (title, pc_FALSO))
		// no title
		pdest += sizeof(WORD);
	else {
		UINT CodePage = CP_ACP;					// code page
		DWORD dwFlags = MB_PRECOMPOSED;			// character-type options
		LPCSTR lpMultiByteStr = szBoxCaption;	// address of string to map
		int cchMultiByte = strlen (szBoxCaption);// number of bytes in string
		LPWSTR lpWideCharStr = (LPWSTR) pdest;	// address of wide-character buffer
		int cchWideChar = MAX_CHARS / 2;        // size of buffer

		int nWideChars = MultiByteToWideChar(
			CodePage, dwFlags, lpMultiByteStr, cchMultiByte, lpWideCharStr, cchWideChar);
		pdest += (nWideChars + 1) * sizeof(WORD);
	}
#endif // OLD_TITLE

#ifdef SET_FONT
	// Informazioni relative ai fonts
	// font size
	*(WORD*) pdest = 8;
	pdest += sizeof(WORD);
	// font name
	char *szFontName = "ARIAL";	
	strcpy ((char*) pdest, szFontName);
#else // SET_FONT
	// no font
	pdest += sizeof(WORD);
#endif // SET_FONT

	entry = CreateDlgEntry(pDialog, controlType, 0, parent, title, geometry, setup, sons);
	AddEntry(&Dialogs, entry, name);

#ifdef OLD_MODAL
	if (controlType == MODAL) {
		// aggiunto 991007
		result = pModelessDialog->InitModalIndirect(pTemp, pParent);
		// vedi override di OnInitDialog in modeless.cpp !
		result = pModelessDialog->DoModal();
	}
	else {
		result = pModelessDialog->CreateIndirect(pTemp, pParent);
		if (setup) NestedExec (setup);
    }
#else // OLD_MODAL
	result = pModelessDialog->CreateIndirect(pTemp, pParent);
	pModelessDialog->SetOwner(NULL);	// 991012
	if (setup) {
		int was_nestedExec = is_nestedExec;	// 991128
		is_nestedExec = FALSE;				//
		NestedExec (setup);
		is_nestedExec = was_nestedExec;		//
	}
	pModelessDialog->SetFocus();

	if (controlType == MODAL) {
//		pModelessDialog->SetCapture();
		int was_nestedExec = is_nestedExec;	// 991128
		is_nestedExec = FALSE;				//
		while (! pModelessDialog->m_bCanceled) {
			EventCheck ();
		}
		is_nestedExec = was_nestedExec;		//
//		ReleaseCapture();
		if (pParent)
			pParent->SetFocus();
		else
			theApp.m_pMainWnd->SetFocus();

		pModelessDialog->DestroyWindow();
		delete (pModelessDialog);
	}
#endif // OLD_MODAL
}

void WindowDelete (node name, int controlType)
{
	// controlla che il dialogo esista e sia del tipo giusto
	DlgEntry entry = NameToEntry (Dialogs, name, DIALOG);
	if ((! entry) || (GetType(entry) != controlType)) {
		err2 (52, name); return;
	}
	// rimuove i controlli dalla lista degli entry
	node sons = GetSons (entry);
	DeleteSons (sons);
	// rimuove il dialogo dalla lista degli entry
	RemoveEntryByName(&Dialogs, args[0]);
	// rimuove il dialogo
	CModelessDialog* Dialog = (CModelessDialog*) DialogGetWindow (entry);

#ifdef OLD_MODAL
	if (controlType == MODELESS)
		Dialog->DestroyWindow();
	else	// MODAL
		Dialog->DialogCancel();		// richiama CDialog::OnCancel
	delete (Dialog);
#else // OLD_MODAL
	if (controlType == MODAL)
		Dialog->DialogCancel();		// richiama CDialog::OnCancel
	else {	// MODELESS
		Dialog->DestroyWindow();
		delete (Dialog);
	}
#endif // OLD_MODAL
}


void WINDOWCREATE ()
{
	DialogCreate (MODELESS);
}

void WINDOWDELETE ()
{
	WindowDelete (args[0], MODELESS);
}

void DIALOGCREATE ()
{
	DialogCreate (MODAL);
}

void DIALOGDELETE ()
{
	WindowDelete (args[0], MODAL);
}

void DIALOGSETFOCUS ()
{
	DlgEntry control;
	if (control = NameToEntry (Dialogs, args[0], ANY)) {
		int type = GetType (control);
		int nID = GetId (control);
		CWnd* pWndCtrl = DialogGetWindow (control);
		node dialogName = DialogGetParent(control);
		DlgEntry dialog = NameToEntry (Dialogs, dialogName, DIALOG);
		CDialog* pDialog = (CDialog*) DialogGetWindow (dialog);
		if (type == BUTTON)
			pDialog->SetDefID(nID);
		else
			pDialog->GotoDlgCtrl(pWndCtrl);
	}
	else err2 (52, args[0]);
}

void ListBoxCreate (DWORD dwStyle)
{
	CListBox *ListBox;
	static UINT nID = IDC_LISTBOX;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 2); 
		ListBox = new (CListBox);
		if (ListBox->Create(dwStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, ListBox, LISTBOX, nID, NULLP, NULLP);
			entry = AddControl (parent, ListBox, LISTBOX, nID, args[1], NULLP);
		else err2 (51, args[1]);
	}
}
void LISTBOXCREATE ()
{
//	ListBoxCreate (WS_CHILD | WS_VISIBLE | LBS_STANDARD);
	ListBoxCreate (WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_GROUP | WS_TABSTOP);
}
void LISTBOXCREATERAW ()
{
	ListBoxCreate (WS_CHILD | WS_VISIBLE | (LBS_STANDARD ^ LBS_SORT));
}

void ListBoxDelete (DlgEntry entry)
{
	CListBox *ListBox = (CListBox *) DialogGetWindow (entry);
	ListBox->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void LISTBOXDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], LISTBOX))
		ListBoxDelete (entry);
	else err2 (52, args[0]);
}

int ListBoxGetSelect (CListBox *ListBox, char *s)
{
	int i = ListBox->GetCurSel ();
	if (i == LB_ERR) {
		/* errore () */
		return LB_ERR;
	} else {
		if (s) ListBox->GetText (i, s);
		return i;
	}	
}
void LISTBOXGETSELECT ()
{
	CListBox *ListBox;
	int i;
	char text[MAX_CHARS] = "";
	DlgEntry entry = NameToEntry (Dialogs, args[0], LISTBOX);
	if (entry) {
		ListBox = (CListBox *) DialogGetWindow (entry);
		ListBoxGetSelect (ListBox, text);
		if ((i = ListBoxGetSelect (ListBox, text)) == LB_ERR) risultato = NULLP;
//		else risultato = _parse (text);
		else risultato = logstr (text);

	}
	else err2 (52, args[0]);
}

void LISTBOXGETINDEX ()
{
	CListBox *ListBox;
	int i;
	DlgEntry entry = NameToEntry (Dialogs, args[0], LISTBOX);
	if (entry) {
		ListBox = (CListBox *) DialogGetWindow (entry);
		if ((i = ListBoxGetSelect (ListBox, NULL)) == LB_ERR) i = 0;
		else ++i;
		risultato = int_num (i);
	}
	else err2 (52, args[0]);
}

void ListBoxAddString (CListBox *lb, char *s)
{
	lb->AddString (s);
	if (lb->GetCurSel() == LB_ERR)	// 980505
		lb->SetCurSel(0);			//
}
void LISTBOXADDSTRING ()
{
	char text[MAX_CHARS];
	CListBox *ListBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], LISTBOX);
	if (entry) {
		ListBox = (CListBox *) DialogGetWindow (entry);
		if (types[1] == LIST) MemScrivi (text, args[1]);
		else getnam (text, args[1]);
		ListBoxAddString (ListBox, text);
	}
	else err2 (52, args[0]);
}

int ListBoxDeleteString (CListBox *lb, int i)
{
	if (lb->DeleteString (i) == LB_ERR)
		return LB_ERR;
	else
		return i;
}
void LISTBOXDELETESTRING ()
{
	CListBox *ListBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], LISTBOX);
	if (entry) {
		ListBox = (CListBox *) DialogGetWindow (entry);
//		ListBoxDeleteString(ListBox, (int) node2real (args[1]));
		int i = num_int (args[1]);
		if (ListBoxDeleteString(ListBox, i) != i)
			err2 (7, args[0]);	// non ha abbastanza elementi
	}
	else err2 (52, args[0]);	// finestra non adatta
}

void LISTBOXRESET ()
{
	CListBox *ListBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], LISTBOX);
	if (entry) {
		ListBox = (CListBox *) DialogGetWindow (entry);
		ListBox->ResetContent();
	}
	else err2 (52, args[0]);	// finestra non adatta
}

void ComboBoxCreate ()
{
	CComboBox *ComboBox;
	static UINT nID = IDC_COMBOBOX;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
//		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBS_SIMPLE;
//		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_AUTOHSCROLL;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | CBS_AUTOHSCROLL;
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 2); 
		ComboBox = new (CComboBox);
		if (ComboBox->Create(dwStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, ComboBox, COMBOBOX, nID, NULLP, NULLP);
			entry = AddControl (parent, ComboBox, COMBOBOX, nID, args[1], NULLP);
		else err2 (51, args[1]);
	}
}
void COMBOBOXCREATE ()
{
	ComboBoxCreate ();
}

void ComboBoxDelete (DlgEntry entry)
{
	CComboBox *ComboBox = (CComboBox *) DialogGetWindow (entry);
	ComboBox->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void COMBOBOXDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], COMBOBOX))
		ComboBoxDelete (entry);
	else err2 (52, args[0]);
}

void ComboBoxGetText (CComboBox *ComboBox, char *text)
{
	ComboBox->GetWindowText (text, MAX_CHARS);
}
void COMBOBOXGETTEXT ()
{
	char text[MAX_CHARS];
	CComboBox *ComboBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], COMBOBOX);
	if (entry) {
		ComboBox = (CComboBox *) DialogGetWindow (entry);
		ComboBoxGetText(ComboBox, text);
//		risultato = _parse (text);
		risultato = logstr (text);
	}
	else err2 (52, args[0]);
}

void ComboBoxSetText (CComboBox *ComboBox, char *text)
{
	ComboBox->SetWindowText (text);
}
void COMBOBOXSETTEXT ()
{
	CComboBox *ComboBox;
	char text[MAX_CHARS];
	DlgEntry entry = NameToEntry (Dialogs, args[0], COMBOBOX);
	if (entry) {
		ComboBox = (CComboBox *) DialogGetWindow (entry);
		if (types[1] == LIST) MemScrivi (text, args[1]);
		else getnam (text, args[1]);
		ComboBoxSetText(ComboBox, text);
	}
	else err2 (52, args[0]);
}

void ComboBoxAddString (CComboBox *ComboBox, char *s)
{
	ComboBox->AddString (s);
}
void COMBOBOXADDSTRING ()
{
	char text[MAX_CHARS];
	CComboBox *ComboBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], COMBOBOX);
	if (entry) {
		ComboBox = (CComboBox *) DialogGetWindow (entry);
		if (types[1] == LIST) MemScrivi (text, args[1]);
		else getnam (text, args[1]);
		ComboBoxAddString (ComboBox, text);
	}
	else err2 (52, args[0]);
}


int ComboBoxDeleteString (CComboBox *cb, int i)
{
	if (cb->DeleteString (i) == CB_ERR)
		return CB_ERR;
	else
		return i;
}
void COMBOBOXDELETESTRING ()
{
	CComboBox *ComboBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], COMBOBOX);
	if (entry) {
		ComboBox = (CComboBox *) DialogGetWindow (entry);
//		ComboBoxDeleteString(ComboBox, (int) node2real (args[1]));
		int i = num_int (args[1]);
		if (ComboBoxDeleteString(ComboBox,  i) != i)
			err2 (7, args[0]);	// non ha abbastanza elementi
	}
	else err2 (52, args[0]);	// finestra non adatta
}


void EditBoxCreate ()
{
	CEdit *EditBox;
	static UINT nID = IDC_EDITBOX;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | WS_BORDER;
//>000511
//		dwStyle |= ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN;
		node props;
		int val;
		if (n_argomenti <= 6)
			dwStyle |= ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_WANTRETURN;
		else 
		{
			props = args[6];
			if (props != NULLP) {
				val = num_int (car (props)); props = cdr (props);
				if (val & 1)
					dwStyle |= ES_MULTILINE;
				if (val & 2)
					dwStyle |= ES_WANTRETURN;
				if (val & 4)
					dwStyle |= WS_VSCROLL;
			}
			if (props != NULLP) {
				val = num_int (car (props)); props = cdr (props);
				if (val & 1)
					dwStyle |= ES_AUTOHSCROLL;
				if (val & 2)
					dwStyle |= ES_AUTOVSCROLL;
			}
			if (props != NULLP) {
				val = num_int (car (props)); props = cdr (props);
				if (val & 1)
					dwStyle |= ES_CENTER;
				if (val & 2)
					dwStyle |= ES_RIGHT;
			}
			if (props != NULLP) {
				val = num_int (car (props)); props = cdr (props);
				if (val & 1)
					dwStyle |= ES_LOWERCASE;
				if (val & 2)
					dwStyle |= ES_UPPERCASE;
			}
			if (props != NULLP) {
				val = num_int (car (props)); props = cdr (props);
				if (val & 1)
					dwStyle |= ES_READONLY;
				if (val & 1)
					dwStyle |= ES_PASSWORD;
			}
		}
//<
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 2);
//		if (rect.Height() >= 20)
//			dwStyle |= WS_VSCROLL;
		EditBox = new (CEdit);
		if (EditBox->Create(dwStyle, rect, pParentWnd, ++nID))
			entry = AddControl (parent, EditBox, EDITBOX, nID, args[1], NULLP);
		else err2 (51, args[1]);
	}
}
void EDITBOXCREATE ()
{
	EditBoxCreate ();
}

void EditBoxDelete (DlgEntry entry)
{
	CEdit* EditBox = (CEdit*) DialogGetWindow (entry);
	EditBox->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void EDITBOXDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], EDITBOX))
		EditBoxDelete (entry);
	else err2 (52, args[0]);
}

void EditBoxGetText (CEdit* EditBox, char *text)
{
	EditBox->GetWindowText (text, MAX_CHARS);
}
void EDITBOXGETTEXT ()
{
	char text[MAX_CHARS];
	CEdit* EditBox;
	DlgEntry entry = NameToEntry (Dialogs, args[0], EDITBOX);
	if (entry) {
		EditBox = (CEdit *) DialogGetWindow (entry);
		EditBoxGetText(EditBox, text);
		risultato = logstr (text);
	}
	else err2 (52, args[0]);
}

void EditBoxSetText (CEdit* EditBox, char *text)
{
	EditBox->SetWindowText (text);
}
void EDITBOXSETTEXT ()
{
	CEdit* EditBox;
	char text[MAX_CHARS];
	DlgEntry entry = NameToEntry (Dialogs, args[0], EDITBOX);
	if (entry) {
		EditBox = (CEdit *) DialogGetWindow (entry);
		if (types[1] == LIST) MemScrivi (text, args[1]);
		else getnam (text, args[1]);
		EditBoxSetText(EditBox, text);
	}
	else err2 (52, args[0]);
}


void ScrollBarCreate ()
{
	CScrollBar *ScrollBar;
	static UINT nID = IDC_SCROLLBAR;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
		DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 2); 
		DWORD dsbStyle = (rect.Height() > rect.Width()) ? SBS_VERT : SBS_HORZ;
		ScrollBar = new (CScrollBar);
		if (ScrollBar->Create(dwStyle | dsbStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, ScrollBar, SCROLLBAR, nID, NULLP, args[6]);
			entry = AddControl (parent, ScrollBar, SCROLLBAR, nID, args[1], args[6]);
		else err2 (51, args[1]);
	}
}
void SCROLLBARCREATE ()
{
	ScrollBarCreate ();
}

void ScrollBarDelete (DlgEntry entry)
{
	CScrollBar *ScrollBar = (CScrollBar *) DialogGetWindow (entry);
	ScrollBar->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void SCROLLBARDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], SCROLLBAR))
		ScrollBarDelete (entry);
	else err2 (52, args[0]);
}

int ScrollBarGet (CScrollBar *ScrollBar)
{
	return ScrollBar->GetScrollPos();
}
void SCROLLBARGET ()
{
	CScrollBar *ScrollBar;
	DlgEntry entry = NameToEntry (Dialogs, args[0], SCROLLBAR);
	if (entry) {
		ScrollBar = (CScrollBar *) DialogGetWindow (entry);
		risultato = consreal ((double) ScrollBarGet (ScrollBar));
	}
	else err2 (52, args[0]);
}

int ScrollBarSet (CScrollBar *ScrollBar, int low, int high, int pos)
{
	ScrollBar->SetScrollRange(low, high, FALSE);
	return ScrollBar->SetScrollPos(pos, TRUE);
}
void SCROLLBARSET ()
{
	CScrollBar *ScrollBar;
	DlgEntry entry = NameToEntry (Dialogs, args[0], SCROLLBAR);
	if (entry) {
		ScrollBar = (CScrollBar *) DialogGetWindow (entry);
		ScrollBarSet (ScrollBar,
			(int) node2real (args[1]), (int) node2real (args[2]), (int) node2real (args[3])); 
	}
	else err2 (52, args[0]);
}


void ButtonCreate ()
{
//	WS_CAPTION | WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON    
	char label[MAX_CHARS];
	CButton *Button;
	static UINT nID = IDC_BUTTON;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
//		DWORD dwStyle = WS_CHILD | WS_VISIBLE /* | WS_GROUP | WS_TABSTOP */;
//>991212
///		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP;
//<   
		if (types[2] == LIST) MemScrivi (label, args[2]);
		else getnam (label, args[2]);
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 3); 
		Button = new (CButton);
		if (Button->Create((LPCSTR) label, dwStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, Button, BUTTON, nID, args[2], args[7]);
			entry = AddControl (parent, Button, BUTTON, nID, args[1], args[7]);
		else err2 (51, args[1]);
	}
}
void BUTTONCREATE ()
{
	ButtonCreate();
}

void ButtonDelete (DlgEntry entry)
{
	CButton *Button = (CButton *) DialogGetWindow (entry);
	Button->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void BUTTONDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], BUTTON))
		ButtonDelete (entry);
	else err2 (52, args[0]);
}


void StaticCreate ()
{
	char label[MAX_CHARS];
	CStatic *Static;
	static UINT nID = IDC_STAT;
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_LEFT;
		if (types[2] == LIST) MemScrivi (label, args[2]);
		else getnam (label, args[2]);
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 3); 
		Static = new (CStatic);
		if (Static->Create((LPCSTR) label, dwStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, Static, STATIC, nID, args[2], NULLP);
			entry = AddControl (parent, Static, STATIC, nID, args[1], NULLP);
		else err2 (51, args[1]);
	}
}
void STATICCREATE ()
{
	StaticCreate();
}

void StaticDelete (DlgEntry entry)
{
	CStatic *Static = (CStatic *) DialogGetWindow (entry);
	Static->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void STATICDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], STATIC))
		StaticDelete (entry);
	else err2 (52, args[0]);
}

void StaticUpdate (CStatic *Static, char *text)
{
	Static->SetWindowText (text);
}
void STATICUPDATE ()
{
	CStatic *Static;
	char text[MAX_CHARS];
	DlgEntry entry = NameToEntry (Dialogs, args[0], STATIC);
	if (entry) {
		Static = (CStatic *) DialogGetWindow (entry);
		if (types[1] == LIST) MemScrivi (text, args[1]);
		else getnam (text, args[1]);
		StaticUpdate(Static, text);
	}
	else err2 (52, args[0]);
}


void GroupBoxCreate ()
{
	CButton *GroupBox;
	static UINT nID = IDC_GROUPBOX;
	char *text = "";
	DlgEntry parent, entry;
	if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
	else {
		CWnd* pParentWnd = DialogGetWindow (parent);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX;
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 2); 
		GroupBox = new (CButton);
		if (GroupBox->Create(text, dwStyle, rect, pParentWnd, ++nID))
//			entry = AddControl (parent, GroupBox, GROUPBOX, nID, NULLP, NULLP);
			entry = AddControl (parent, GroupBox, GROUPBOX, nID, args[1], NULLP);
		else err2 (51, args[1]);
	}
}
void GROUPBOXCREATE ()
{
	GroupBoxCreate ();
}

void GroupBoxDelete (DlgEntry entry)
{
	CButton *GroupBox = (CButton *) DialogGetWindow (entry);
	GroupBox->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void GROUPBOXDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], GROUPBOX))
		GroupBoxDelete (entry);
	else err2 (52, args[0]);
}


void CheckBoxCreate ()
{
	DlgEntry parent, group, entry;
	char label[MAX_CHARS];
	CButton *CheckBox;
	static UINT nID = IDC_CHECKBOX;
  if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
  else {
	CWnd* pParentWnd = DialogGetWindow (parent);
	args[0] = args[1]; args[1] = args[2];
	if (! (group = NameToEntry (Dialogs, args[0], GROUPBOX)))
		err2 (52, args[0]);
	else {
		CWnd* pGroupWnd = DialogGetWindow (group);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_GROUP | BS_AUTOCHECKBOX;
		if (types[3] == LIST) MemScrivi (label, args[3]);
		else getnam (label, args[3]);
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 4); 
		CheckBox = new (CButton);
		if (CheckBox->Create((LPCSTR) label, dwStyle, rect, pGroupWnd, ++nID))
//			entry = AddControl (group, CheckBox, CHECKBOX, nID, args[3], NULL);
//		else err2 (51, args[1]);
			entry = AddControl (group, CheckBox, CHECKBOX, nID, args[2], NULL);
		else err2 (51, args[2]);
	}
  }
}
void CHECKBOXCREATE ()
{
	CheckBoxCreate();
}

void CheckBoxDelete (DlgEntry entry)
{
	CButton *CheckBox = (CButton *) DialogGetWindow (entry);
	CheckBox->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void CHECKBOXDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], CHECKBOX))
		CheckBoxDelete (entry);
	else err2 (52, args[0]);
}

int CheckBoxGet(CButton *CheckBox)
{
	return CheckBox->GetCheck();
}
void CHECKBOXGET ()
{
	DlgEntry entry;
	CButton *CheckBox;
	if (entry = NameToEntry (Dialogs, args[0], CHECKBOX)) {
		CheckBox = (CButton *) DialogGetWindow (entry);
		risultato = logval(CheckBoxGet(CheckBox));
	}
	else err2 (52, args[0]);
}

void CheckBoxSet(CButton *CheckBox, int state)
{
	CheckBox->SetCheck(state);
}
void CHECKBOXSET ()
{
	DlgEntry entry;
	CButton *CheckBox;
	if (entry = NameToEntry (Dialogs, args[0], CHECKBOX)) {
		CheckBox = (CButton *) DialogGetWindow (entry);
		CheckBoxSet(CheckBox, bool_ (args[1]));
	}
	else err2 (52, args[0]);
}


void RadioButtonCreate ()
{
	DlgEntry parent, group, entry;
	char label[MAX_CHARS];
	CButton *RadioButton;
	static UINT nID = IDC_RADIOBUTTON;
  if (! (parent = NameToEntry (Dialogs, args[0], DIALOG)))
		err2 (52, args[0]);
  else {
	CWnd* pParentWnd = DialogGetWindow (parent);
	args[0] = args[1]; args[1] = args[2];
	if (! (group = NameToEntry (Dialogs, args[0], GROUPBOX)))
		err2 (52, args[0]);
	else {
		CWnd* pGroupWnd = DialogGetWindow (group);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_RADIOBUTTON;
		if (types[3] == LIST) MemScrivi (label, args[3]);
		else getnam (label, args[3]);
		CRect rect;
		ArgsToRect (&rect, (CDialog *) pParentWnd, 4); 
		RadioButton = new (CButton);
		if (RadioButton->Create((LPCSTR) label, dwStyle, rect, pGroupWnd, ++nID))
//			entry = AddControl (group, RadioButton, RADIOBUTTON, nID, args[3], NULL);
//		else err2 (51, args[1]);
			entry = AddControl (group, RadioButton, RADIOBUTTON, nID, args[2], NULL);
		else err2 (51, args[2]);
	}
  }
}
void RADIOBUTTONCREATE ()
{
	RadioButtonCreate();
}

void RadioButtonDelete (DlgEntry entry)
{
	CButton *RadioButton = (CButton *) DialogGetWindow (entry);
	RadioButton->DestroyWindow();
	RemoveControl (&Dialogs, entry, args[0]);
}
void RADIOBUTTONDELETE ()
{
	DlgEntry entry;
	if (entry = NameToEntry (Dialogs, args[0], RADIOBUTTON))
		RadioButtonDelete (entry);
	else err2 (52, args[0]);
}

int RadioButtonGet(CButton *RadioButton)
{
	return RadioButton->GetCheck();
}
void RADIOBUTTONGET ()
{
	DlgEntry entry;
	CButton *RadioButton;
	if (entry = NameToEntry (Dialogs, args[0], RADIOBUTTON)) {
		RadioButton = (CButton *) DialogGetWindow (entry);
		risultato = logval(RadioButtonGet(RadioButton));
	}
	else err2 (52, args[0]);
}

void RadioButtonSet(CButton *RadioButton, int state)
{
	RadioButton->SetCheck(state);
}
void RADIOBUTTONSET ()
{
	DlgEntry entry;
	CButton *RadioButton;
	if (entry = NameToEntry (Dialogs, args[0], RADIOBUTTON)) {
		RadioButton = (CButton *) DialogGetWindow (entry);
		RadioButtonSet(RadioButton, bool_ (args[1]));
	}
	else err2 (52, args[0]);
}

void DEBUGWINDOWS ()
{
}

int CaptionMessageBox (char *caption, char *message)
{
	CWnd mb;
	return mb.MessageBox(message, caption, MB_OK);
}
void MESSAGEBOX ()
{
	char banner[MAX_CHARS],
		body [1024];
	if (types[0] == LIST) MemScrivi (banner, args[0]);
	else getnam (banner, args[0]);
	if (types[1] == LIST) MemScrivi (body, args[1]);
	else getnam (body, args[1]);
	CaptionMessageBox (banner, body);
}

void argToText (int i, char* name);
node DialogFileOpenClose (BOOL bOpenFileDialog)
{
	LPCSTR lpszDefExt = NULL;
	LPCSTR lpszFileName = NULL;
	char fileName[MAX_CHARS];
	if (n_argomenti > 0) {
		getnam (fileName, args[0]);
		lpszFileName = &fileName[0];
	}
	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	LPCSTR lpszFilter = "Tutti i file (*.*)|*.*||";
	CWnd* pParentWnd = NULL;
	
	CFileDialog FileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd);
	char titleString[MAX_CHARS];
	if (n_argomenti > 1) {
		argToText (1, titleString);
		FileDialog.m_ofn.lpstrTitle = &titleString[0];
	}
	int result = FileDialog.DoModal();
	if (result == IDOK) {
		CString pathName = FileDialog.GetPathName ();
		return metti_sc (pathName.GetBuffer (1));
	}
	return NULLP;
}

void DIALOGFILEOPEN ()
{
	risultato = DialogFileOpenClose (TRUE);
	if (risultato != NULLP)
		risultato = cons (risultato, NULLP, LIST);
}

void DIALOGFILESAVE ()
{
	risultato = DialogFileOpenClose (FALSE);
	if (risultato != NULLP)
		risultato = cons (risultato, NULLP, LIST);
}

void WINDOWFILEEDIT ()
{
}

void DeleteSons (node sons)
{
	node name;
	DlgEntry son;
	int ControlType;
	while (sons) {
		name = car (sons);
	    son = NameToEntry (Dialogs, name, ANY);
		CWnd* window = DialogGetWindow (son);
	    ControlType = GetType(son);
	    switch (ControlType) {
	    case BUTTON:
			((CButton *)window)->DestroyWindow();
	    break;
//> 980831, 991212
	    case EDITBOX: 
			((CEdit *)window)->DestroyWindow();
	    break;
//<
	    case STATIC: 
			((CStatic *)window)->DestroyWindow();
	    break;
	    case LISTBOX: 
			((CListBox *)window)->DestroyWindow();
			delete ((CListBox *) window);
	    break;
	    case SCROLLBAR: 
			((CScrollBar *)window)->DestroyWindow();
			delete ((CScrollBar *) window);
	    break;
	    case COMBOBOX: 
			((CComboBox *)window)->DestroyWindow();
			delete ((CComboBox *) window);
	    break;
	    case GROUPBOX: 
			((CButton *)window)->DestroyWindow();
	    break;
	    case CHECKBOX: 
			((CButton *)window)->DestroyWindow();
	    break;
	    case RADIOBUTTON: 
			((CButton *)window)->DestroyWindow();
	    break;
		}
		RemoveControl (&Dialogs, son, name);
		sons = cdr (sons);
	}
} 
