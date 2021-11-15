/*ILFOGLIO.C	G. Toffoli

	981224	controllo sulla chiusura
			da parte di codice eseguito dall'interno
	980828	preparazione per conversione a C++; rivedere nomearc !!!!
	980721	Modificato KwAttr secondo vera sintassi di lista associativa
			Introdotto controllo su esistenza del menu
	980717	Iniziato a usare KwAttr per uso di associazioni in lista argomenti
	980107	Gestione del foglio corrente
			aggiunto openDefaultEditor; modif. sel_foglio
	971229	Introdotto ini_foglio
	971217	Modificato apri_foglio
	970904	In apri_foglio/chiudi_foglio aggiunta/tolta voce a menu WINDOWS
	970618	prima versione sotto Windows
*/
 
#define CMODULE
#define ILWIN

// #define pc_MENU pc_GLOSSE	// kw [56]
// #define pc_TITLE pc_EGA		// kw [62]

#include "ilcomune.cpp"
#include "ilfunz.h"

int MatchKw (node, node);
node KwAttr (node lName, node tail, node kWord);
void MemScrivi (char *, node);

int EditorOpenView (int x, int y, int dimx, int dimy, char *pTitle, int isMenu);
int EditorCloseView (int nview);
void EditorClear (int nview);	

int dev_editor = -1;
int newEditorCounter = 1;
char DefaultEditor[64] = "";

int view_to_devfoglio (int view)
{
	int i;
	for (i=0; i<MAX_DEV; i++)
		if ((_file[i] == view) && (_fstato[i] & O_FOGLIO)) break;
	return i;
}

/*------------------------------------------------------
  attiva nuovo foglio senza cancellare quello precedente
  ------------------------------------------------------*/
void MainMenuShowWindow (char *name);

void sel_foglio (/*nview*/ int dev) {
/*>980107
	nq_foglio = nview;
*/
	char name[MAXCHARS];

	dev_editor = dev;
	pacstr (name, nome (_nome [dev]));
	MainMenuShowWindow (name);
/*<*/
}

/* ---------------------------------------------------------------------------
  apre una finestra tipo FOGLIO con il nome specificato;
  se non ci riesce ritorna -1, altrimenti ritorna l' indice i dopo aver
  assegnato a _file [i] l' handle di tarta e a _nome [i] il nome
--------------------------------------------------------------------------- */
void MainMenuAppendWindowItem (char *name);

int apri_foglio (node lnome)
{
	int i, j;
	node title;
	node menu; int is_menu = -1;		// 980721
	char nameString [MAXCHARS], titleString [MAXCHARS], *pTitle;
	node pos, dim;
	int x1, y1;
	int dimx, dimy;

	menu = KwAttr (lnome, arg_1, pc_MENU);
	if (menu)
		is_menu = MatchKw (menu = car (menu), pc_VERO);

	title = KwAttr (lnome, arg_1, pc_TITLE);
	if ((title) && (MatchKw (title = car (title), pc_FALSO)))
		pTitle = (char *) 0L;
	else {
		if (! title)
			title = lnome;
		if (typ (title) == LIST) MemScrivi (titleString, title);
		else getnam (titleString, title);
		pTitle = &titleString[0];
	}
	if ((i = nuovo_dev ()) == 0)		/* cerca posto per nuovo device */
		return -1;
	if (_file [i] == -1) {
		pos = KwAttr (lnome, arg_1, pc_POS);
		if (! pos) {
			x1 = -1; y1 = -1;
		} else {
			x1 = rnum_int (car (pos));
			y1 = rnum_int (car (cdr (pos)));
		};

		dim = KwAttr (lnome, arg_1, pc_DIM);
		if (! dim) {
			dimx = -1; dimy = -1;
		} else {
			dimx = rnum_int (car (dim));
			dimy = rnum_int (car (cdr (dim)));
		};

		j = EditorOpenView (x1, y1, dimx, dimy, pTitle, is_menu);	//
		_file [i] = j;                  	/* handle del device */
	}

	_nome [i] = lnome;              	/* nome del device */
	_fstato [i] = O_FINESTRA | O_FOGLIO;/* stato del device */

	pacstr (nameString, nome (lnome));		/* aggiunge voce a menu WINDOWS */
	MainMenuAppendWindowItem (nameString);

	dev_editor = i;	// NOTA: c'e' qualche ridondanza
	++newEditorCounter;
	return i;                       /* riporta indice device */
}

void MainMenuRemoveWindowItem (char *name);
int currentWindow (int mask);

int chiudi_foglio (int dev)
{
	int result = EditorCloseView (_file [dev]);
	if (result) {
		_file [dev] = -1;
		char nameString[MAXCHARS];
		pacstr (nameString, nome (_nome [dev]));
		MainMenuRemoveWindowItem (nameString);

		if (dev == dev_leggi) dev_leggi = dev_in = CONSOLE;
		else if (dev == dev_scrivi) dev_scrivi = dev_out = CONSOLE;
		dev_editor = -1;
		if ((dev = currentWindow (O_FOGLIO)) != -1)
			sel_foglio (dev);
		return TRUE;
	}
	return result;
}


void ini_foglio ()
{
	arg_1 = NULLP;	// 980717 : per gestione di listra di attributi
	if (DefaultEditor[0] != '\0')
		dev_editor = apri_foglio (mettip (logstr (DefaultEditor)));
}

int openDefaultEditor ()	// spostato qui da MAINFRM.CPP
{
	char sCounter[32] = "";
	char editorName[MAXCHARS];

	if (newEditorCounter > 1)
		sprintf (sCounter, "%d", newEditorCounter);
	if (DefaultEditor[0] != '\0')
		strcpy (editorName, DefaultEditor);
	else
		pacstr (editorName, nome (pc_FOGLIO));
	strcat (editorName, sCounter);
	arg_1 = NULLP;	// 980717 : per gestione di listra di attributi
  	return apri_foglio (mettip (logstr (editorName)));
}

/* -------------------------------------------------
Predicato VERO se (un) FOGLIO e' visibile (esiste ?)
---------------------------------------------------- */
int currentWindow (int mask);
void PR_FOGLIO_ ()
{
	int dev = currentWindow (O_FOGLIO);
	risultato = logval (dev != -1);
}

void cance_foglio ()
{
	int dev = currentWindow (O_FOGLIO);
	if (dev == -1)
		err1 (49);
	else
		EditorClear (_file [dev]);	
}

void PR_CANCEFOGLIO ()
{
	cance_foglio ();
}
