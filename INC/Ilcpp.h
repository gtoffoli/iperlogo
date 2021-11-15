// ilcpp.h : excerpt of logo.h for benefit of .cpp modules of IperLogo for Windows 

#ifdef WIN32

#include "logo.h"

#else // WIN32

			/* codifica dei tipi di token / oggetto Logo */
#define	LIST     1
#define	LWORD    2
#define	SFUN     3
#define	UFUN     4
#define	LVAR     5
#define	LSTR     7
#define LBCD	 10
#define	PLIST    11
#define	ARAY    12


typedef unsigned long node;
#define	NULLP	0L
#define	funzione _funzione()

#endif // WIN32

#ifdef MISTO

extern "C" {
int typ (node); 
node car(node);
node cdr(node);
void putcar(node, node);
void putcdr(node, node);
node cons(node, node, int);
node consptr(const char *);
char *node2ptr (node);
node consreal(double);
double node2real(node);
int lstlen (node);
node _pri(node);
node _menpri(node);
node _ele(int, node);
void _setitem (node, int, node);
node lstdel (node *, node);
void RemoveEle (node *, node);
node _assoc (node, node);
node logstr (const char *);
int pacstr (char *s, node nome);
node metti_sc (const char *);
node mettip (node lstring);
node getnam (char *, node);
node nome (node oggetto);
int bool_ (node);
node logval (int);
int num_int(node);
int rnum_int(node);
node int_num(int);
node _prop (node parola, node nome);
node _funzione ();
void err1 (int);
void err2 (int, node);
void err_arg1 ();
void err_pochi ();
node _parse (const char *);
void ExecOnEvent (node);
void NestedExec (node);
int f_apri (node ml_nome, int mode, int pmode);
int f_chiudi (int dev);
}

extern "C" {
node kw [];		/* indice delle parole chiave */
// node funzione;
// node risultato;	/* risultato della primitiva corrente */
node args[];
int types[];
}

#else // MISTO

#define ILWIN
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#define MAX_CHARS 128

void err_pochi ();
node consptr(const char *);
char *node2ptr (node);
node _assoc (node, node);
node _parse (const char *);
void execute (const char *s);
void _setitem (node, int, node);
void ExecOnEvent (node);
void NestedExec (node);
void EventCheck ();
extern void MemScrivi (char *, node);
void RemoveEle (node *, node);
int MatchKw (node parola, node kw);
node KwAttr (node lName, node tail, node kWord);

#endif // MISTO
