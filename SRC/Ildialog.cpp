// ildialog.cpp
//
/* ILDIALOG.CPP	G. Toffoli

	990623	rinominato GetParent -> DialogGetParent
			stesso per GetWindow e PutWindow
	990622	rinominato RemoveEntry -> RemoveEntryByName
			ridefinito RemoveEntry
*/

#include "stdafx.h"

#include "ilcpp.h"
#include "ildialog.h"

extern DlgList Dialogs;

#define X		1
#define Y		2
#define WIDTH	3
#define HEIGHT	4

Geometry CreateGeometry(int x, int y, int width, int height) {
	return (Geometry) cons(consreal((double) x), cons(consreal((double) y),
			cons(consreal((double) width), cons(consreal((double) height), NULLP, LIST), LIST), LIST), LIST);  
}

int GetX(Geometry geometry) {
	return (int) (node2real (_ele (X, (node) geometry)));
}

int GetY(Geometry geometry) {
	return (int) (node2real (_ele (Y, (node) geometry)));
}

int GetWidth(Geometry geometry) {
	return (int) (node2real (_ele (WIDTH, (node) geometry)));
}

int GetHeight(Geometry geometry) {
	return (int) (node2real (_ele (HEIGHT, (node) geometry)));
}


#define WINDOW		1
#define TYPE		2
#define ID			3
#define PARENT		4
#define TITLE		5
#define GEOMETRY	6
#define SETUP		7
#define SONS		8

DlgEntry CreateDlgEntry(CWnd* window, int type, int id, node parent, node title, Geometry geometry, node setup, node sons) {
	return cons (consptr ((char *) window), cons(consreal((double) type), cons(consreal((double) id), cons(parent, cons(title,
		cons((node) geometry, cons(setup, cons(sons, NULLP, LIST), LIST), LIST), LIST), LIST), LIST), LIST), LIST);
}

CWnd* DialogGetWindow(DlgEntry entry) {
	return (CWnd*) (node2ptr (_ele (WINDOW, entry)));
}

void DialogPutWindow(DlgEntry entry, CWnd* window) {
	_setitem (entry, WINDOW, consptr ((char *) window));
}

int GetType(DlgEntry entry) {
	return (int) (node2real (_ele (TYPE, entry)));
}

int GetId(DlgEntry entry) {
	return (int) (node2real (_ele (ID, entry)));
}

//node GetParent(DlgEntry entry) {
node DialogGetParent(DlgEntry entry) {
	return (_ele (PARENT, entry));
}

node GetTitle(DlgEntry entry) {
	return (_ele (TITLE, entry));
}

node GetSetup(DlgEntry entry) {
	return (_ele (SETUP, entry));
}

void PutSetup(DlgEntry entry, node setup) {
	_setitem (entry, SETUP, setup);
}

Geometry GetGeometry(DlgEntry entry) {
	return ((Geometry) _ele (GEOMETRY, entry));
}

node GetSons(DlgEntry entry) {
	return (_ele (SONS, entry));
}

void PutSons(DlgEntry entry, node sons) {
	_setitem (entry, SONS, sons);
}

void AddEntry(DlgList *pList, DlgEntry entry, node name) {
	*pList = cons (cons (name, entry, LIST), *pList, LIST);
}

DlgEntry GetEntry(DlgList list, node name) {
	return (DlgEntry) _menpri (_assoc (name, list)); 
}

void RemoveEntry(DlgList *pList, DlgEntry entry) {
	node tail = *pList, previous;
	while (tail != NULLP) {
		if (cdr (car (tail)) == entry) break;
		previous = tail; tail = cdr (tail);
	}
	if (tail == *pList) *pList = cdr (tail);
	else putcdr (previous, cdr (tail));
}

//void RemoveEntry(DlgList *pList, node name) {
void RemoveEntryByName(DlgList *pList, node name) {
	node tail = *pList, previous;
	while (tail != NULLP) {
		if (car (car (tail)) == name) break;
		previous = tail; tail = cdr (tail);
	}
	if (tail == *pList) *pList = cdr (tail);
	else putcdr (previous, cdr (tail));
}


DlgEntry NameToEntry (DlgList dialogs, node name, int ControlType) {
	node couple;
	DlgEntry entry;
	int type;
	if (   (couple = _assoc (name, dialogs))
	    && (entry = (DlgEntry) _menpri (couple))
	    && (   (ControlType == ANY)  
	        || ((type = GetType (entry)) == ControlType)
//	        || ((ControlType == DIALOG) && (type > DIALOG))
	        || ((ControlType == DIALOG) && ((type == MODAL) || (type == MODELESS)))
	       )
	  )
		return entry;
	else return NULLP; 
}

DlgEntry IdToEntry (DlgList dialogs, int id) {
	DlgEntry entry;
	while (dialogs) {
		entry = cdr (car (dialogs));
		if (id == GetId (entry)) return entry;
		dialogs = cdr (dialogs);
	}
	TRACE("invalid CtrlID");
	return NULLP; 
}

DlgEntry WinToEntry (DlgList dialogs, CWnd *win) {
	DlgEntry entry;
	while (dialogs) {
		entry = cdr (car (dialogs));
		if (win == DialogGetWindow (entry)) return entry;
		dialogs = cdr (dialogs);
	}
	TRACE("invalid CtrlID");
	return NULLP; 
}

void PR_DIALOGHI ()
{
	risultato = Dialogs;
}