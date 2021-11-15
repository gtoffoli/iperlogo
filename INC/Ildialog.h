// ildialog.h

// typedef unsigned long node;

#define ANY			-1 
#define BUTTON		0
#define EDITBOX		1	// 980831 (non usato ?), 991012
#define STATIC		2
#define LISTBOX		3
#define SCROLLBAR	4
#define COMBOBOX	5
#define GROUPBOX	6
#define CHECKBOX	7
#define RADIOBUTTON	8
#define DIALOG	9

#define MODAL		10
#define MODELESS	11 
#define TURTLE		12 
#define TOOLBAR		13 

#define IDC_SCROLLBAR		2100
#define IDC_GROUPBOX		2200
#define IDC_CHECKBOX		2300
#define IDC_RADIOBUTTON		2400
#define IDC_COMBOBOX		2500
#define IDC_LISTBOX			2600
#define IDC_BUTTON			3000
#define IDC_STAT			4000
#define IDC_EDITBOX			5000
#define IDC_TOOLBAR			6000
#define RANGE_TOOLBAR 4000	// range massimo

typedef node Geometry;
typedef node DlgEntry;
typedef node DlgList;

Geometry CreateGeometry(int x, int y, int width, int height);
int GetX(Geometry geometry);
int GetY(Geometry geometry);
int GetWidth(Geometry geometry);
int GetHeight(Geometry geometry);

DlgEntry CreateDlgEntry(CWnd* window, int type, int id, node parent, node title, Geometry geometry, node setup, node sons);
CWnd* DialogGetWindow(DlgEntry entry);
void DialogPutWindow(DlgEntry entry, CWnd* window);
int GetType(DlgEntry entry);
int GetId(DlgEntry entry);
node DialogGetParent(DlgEntry entry);
node GetTitle(DlgEntry entry);
node GetSetup(DlgEntry entry);
void PutSetup(DlgEntry entry, node setup);
Geometry GetGeometry(DlgEntry entry);
node GetSons(DlgEntry entry);
void PutSons(DlgEntry entry, node sons);
DlgEntry NameToEntry (DlgList dialogs, node name, int ControlType);
DlgEntry IdToEntry (DlgList dialogs, int id);
DlgEntry WinToEntry (DlgList dialogs, CWnd *win);

void AddEntry(DlgList *pList, DlgEntry entry, node name);
DlgEntry GetEntry(DlgList list, node name);
void RemoveEntry(DlgList *pList, DlgEntry entry);
// void RemoveEntry(DlgList *pList, node name);
void RemoveEntryByName(DlgList *pList, node name);
void DeleteSons (node sons);

