/*ILDIZION.C	G. Toffoli

020603-020604-020605 in libera_n sostituito FatalExit con THROW di eccezione (gestita in main)
020531	aumentato costante nodi in libera_1 e libera_2
010602	LIBERA con argomento per liberare posizione libreria in file di protezione
000509	corretto NormalKw
991230	FatalExit in libera_n
981119	modificato cercav se chiamata da cercat
		per evitare duplicazioni
980831	interfaccia cerca_sc, metti_sc, match_sc_case:
		char * -> const char *
980828	preparazione per conversione a C++
		sostituito min e max con __min e __max
980122	modificato get_sf
971205	cambiato convenzione su normalizzazione (lowercase)
970316	continuato recupero
970315	tentato recupero update del 02.10.96 (normalizzazione)
960926	modificato cercap, cerca_sc, mettio e cercav per conservare case delle parole
		disattivato cercao (non usato)
960925	modificato (poco) in_indice
960409	prima vers. per Windows da Iperlogo del 31.05.89
*/

#include "stdafx.h"

#include "Afxwin.h"	// 020603

#define ILWIN
#define NORMALIZE
  
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include "ilsfun.h"		// 981121
extern struct primitiva descr_sf;

#ifdef ILWIN

#define MAX_TIMER 31
extern node Dialogs, TimerCallback[];
char fname[MAXCHARS];

//>020604
int sogliaNodi[4] = { 0, K/2, K, 2*K };	// 020604
void abbassaSogliaNodi()
{
	int i;
	for (i=1; i<=3; i++)
		sogliaNodi[i] /= 2;
}
//<020604

int match_case (node, node);
int match_nocase (node, node);
// int match_sc_case (char *, node);
int match_sc_case (const char *, node);

int IsNormalUpper = FALSE; //TRUE;

extern int uno;
#define trap(exp) \
	( (exp) ? (void) 0 : uno/0 )
#define normalize_string(s) \
	while ((c = *s) != '\0') *s++ = tolower((unsigned char) c);
//	while ((c = *s) != '\0') *s++ = toupper((unsigned char) c);

#endif // ILWIN

void marca_ws ()
{
  int i, nval;
  node ele;
  set_tipo (NULLVALUE, BIT_LIBERA);	/* non e' compreso in dizionario */
  ele = dizionario;			/* marca elementi del dizionario */
  do set_tipo (ele, BIT_LIBERA);
  while ((ele = cdr (ele)) != NULLP);
  ele = dizionario;			/* marca nodi connessi a dizionario */
  do marca_rete (car (ele));
  while ((ele = cdr (ele)) != NULLP);
  marca_rete (linea_com);	/* marca nodi connessi a linea di comando */
  marca_rete (ult_err);		/* marca nodi di descrittore ultimo errore */
  marca_rete (l_musica);	/* marca nodi di lista note da suonare */
				/* marca nodi connessi a elem. stack valori */
  nval = ini_p_sv - p_sv;	/* numero elementi su stack dei valori */
  for (i = 1; i <= nval; ++i) marca_rete (get_sv (i));
				/* marca costanti bcd e liste programmate */
#ifdef ILDOS
  for (i = 0; i < DIM_BCD+DIM_CODICI+1; ++i) marca_rete (bcd [i]);
				/* marca nodi connessi comandi sospesi */
  for (i = 1; i <= liv_contesto; ++i)
    marca_rete ((contesti [i-1]).linea_com);
				/* marca le liste di glosse */
  for (i = 0; i < N_VIEW; ++i)
    if (view [i] != NULL) marca_rete (view [i]->glosse);
#endif // ILDOS
#ifdef ILWIN
  for (i = 0; i < DIM_BCD; ++i) marca_rete (bcd [i]);
  for (i = 0; i < DIM_CODICI; ++i) marca_rete (tab_prog [i]);
				/* marca nodi connessi comandi sospesi */
  for (i = 1; i <= liv_contesto; ++i)
    marca_rete ((contesti [i])._linea_com);	// contesto 0 e' riservato
				/* marca nodi connessi a gerarchia di dialoghi e finestre tarta */
  marca_rete (Dialogs);
				/* marca nodi connessi a callbacks dei Timer */
  for (i = 0; i <= MAX_TIMER; ++i)
    marca_rete (TimerCallback [i]);
#endif // ILWIN
}

int FreeLibParam (int libserial);
void LIBERA ()
{
//>010602
	if (n_argomenti == 1)
		FreeLibParam (num_int (arg_1));
	else {
//<010602
/*<08.03.89 non serve piu' ?
  smarca_n ();			** smarca tutti i nodi **
>*/
		marca_ws ();			/* marca nodi appartenenti a work-space */
		marca_ll ();			/* marca nodi appartenenti a lista libera */
		pacca_n ();			/* inserisce in lista libera nodi non marcati*/
	}
}

void FatalExit (char* s);
// char* MsgNodeSpace = "Errore di sistema (ho esaurito la memoria assegnata)";
char* MsgNodeSpace = "Ho esaurito la memoria assegnata";

/*--------------------------------
  libera se ci sono meno di n nodi
  --------------------------------*/
void libera_n (int n)
// int n;
{
  if (nodi < n) {
    LIBERA ();
    if (nodi < n) {
//>991230
//		errfat (9);
//>020603-020605
//		FatalExit (MsgNodeSpace);
		int i; for (i=3; i<=9; i++) bcd[i] = NULLP; // variabili a protezione di risultati parziali
		AfxMessageBox(MsgNodeSpace);
		AfxThrowUserException();
//<020603-020605
//>
	}
  }
}

/*-----------------------------------------
  si assicura un numero sufficiente di nodi
  per primitive numeriche iterative e altre
  -----------------------------------------*/
void n_libera ()
{
//	libera_n (100);
  libera_n (sogliaNodi[3]);
}

/*---------------------------------
  libera se ci sono meno di 10 nodi
  ---------------------------------*/
void libera_1 ()
{
//  libera_n (10);
  libera_n (sogliaNodi[1]);
}

/*----------------------------------
  libera se ci sono meno di 100 nodi
  ----------------------------------*/
void libera_2 ()
{
//  libera_n (128);
  libera_n (sogliaNodi[2]);
}

/*-------------------------------------------------------------------
  ritorna VERO se l' oggetto o una sua foglia (nel caso di una lista)
  coincide con la parola data
  -------------------------------------------------------------------*/
int fogliadi (node parola, node oggetto)
// node parola, oggetto;
{
  if (typ (oggetto) != LIST) return (oggetto == parola);
  while (oggetto != NULLP) {
    if (fogliadi (parola, car (oggetto))) return TRUE;
    oggetto = cdr (oggetto);
  };
  return FALSE;
}

/*-----------------------------------------------------------------------
  ritorna la parola LOGO di nome (avente come valore la stringa)
  specificato; se non c' e' ritorna NULLP; modifica vecchio_el e nuovo_el,
  gli elementi del dizionario tra cui inserire la nuova parola
  -----------------------------------------------------------------------*/
node cercap (node nome)
// node nome;
{
  int differenza;
  i_indice = differenza = 1;
  while (   (i_indice < n_indice-1)	/* ricerca su indice */
#ifdef ILDOS
         && (match (nome, car (car (indice_diz [i_indice]))) > 0)
#endif // ILDOS
#ifdef ILWIN
         && (match_case (nome, car (car (indice_diz [i_indice]))) > 0)
#endif // ILWIN
        ) ++i_indice;
  vecchio_el = indice_diz [--i_indice];	/* ricerca su segmento di dizionario */
  while (   ((nuovo_el = cdr (vecchio_el)) != NULLP)
#ifdef ILDOS
         && ((differenza = match (nome, car (car (nuovo_el)))) > 0)
#endif // ILDOS
#ifdef ILWIN
         && ((differenza = match_case (nome, car (car (nuovo_el)))) > 0)
#endif // ILWIN
        )
    vecchio_el = nuovo_el;
  if (differenza == 0) return car (nuovo_el);
  else return NULLP;
}
 
/*---------------------------------------------------------------------------
  richiude virtualmente la catena degli oggetti associati ad una parola del
  dizionario, catena intrecciata con il dizionario stesso per l' introduzione
  delle primitive PREC e SUCC
  ---------------------------------------------------------------------------*/
node richiudi (node oggetto)
// node oggetto;
{
  return (typ (oggetto) == LIST) ? car (cdr (oggetto)) : oggetto;
}

/*----------------------------------------
  ritorna la parola precedente quella data
  ----------------------------------------*/
node paroprec (node oggetto)
// node oggetto;
{
  while (typ (oggetto = cdr (oggetto)) != LIST);
  return car (oggetto);
}

/*------------------------------------------
  ritorna la parola successiva a quella data
  ------------------------------------------*/
node parosucc (node oggetto)
// node oggetto;
{
  while (typ (oggetto = cdr (oggetto)) != LIST);
  return car (cdr (cdr (oggetto)));
}

/*----------------------------------------------------------------------
  se l' indice delle parole del dizionario e' sbilanciato piu' di un tot
  ristruttura l' indice stesso
  ----------------------------------------------------------------------*/
void in_indice ()
{
  node elemento;
  int media, i, j, k;
int old_n_indice = n_indice, old_media = n_parole/n_indice;	// aggiunto
  if (++conto_diz [i_indice] > ((media = ++n_parole/n_indice) + n_indice)) {
//    n_indice = (media < DIM_INDICE) ?
//      media : DIM_INDICE;
n_indice = __min(media, DIM_INDICE);		// riscritto
n_indice = __max(n_indice, old_n_indice);	// aggiunto
    media = n_parole/n_indice;
//    media = n_parole/n_indice -1;		// e' meglio ?
    elemento = dizionario;
    i = j = 0;
    while (i < n_indice) {
      indice_diz [i] = elemento;
      conto_diz [i] = media;
      k = j + media;
      while (++j < k) elemento = cdr (elemento);
      ++i;
    }
  }
}

node nuovap (node nome)
//node nome;
{
  node cella, parola, succ, locale;
  putcdr (vecchio_el,
       cella = cons (parola = cons (nome, vecchio_el, LWORD), nuovo_el, LIST));
  in_indice ();
  succ = car (nuovo_el);
  while (typ (locale = cdr (succ)) != LIST) succ = locale;
  putcdr (succ, cella);
  return parola;
}

/*-----------------------------------------------------------------------
  mette una parola di nome specificato nel dizionario degli oggetti MLOGO
  se non c' e'; ritorna comunque la parola messa o preesistente;
  l' argomento o e' una stringa o e' gia' una parola e allora viene ritornata
  -----------------------------------------------------------------------*/
node mettip (node nome)
// node nome;
{
  return ((typ (nome) == LWORD) ? nome
                                : ((cercap (nome) != NULLP) ? car (nuovo_el)
                                                            : nuovap (nome)));
}

/*-----------------------------------------------------------------------
  ritorna la parola MLOGO di nome (avente come valore la stringa)
  specificato; se non c' e' ritorna NULLP; modifica vecchio_el e nuovo_el
  -----------------------------------------------------------------------*/
// node cerca_sc (char *stringa_C)
node cerca_sc (const char *stringa_C)
// char *stringa_C;
{
  int differenza;
  i_indice = differenza = 1;
  while (   (i_indice < n_indice-1)
#ifdef ILDOS
         && (match_sc (stringa_C, car (car (indice_diz [i_indice]))) > 0)
#endif // ILDOS
#ifdef ILWIN
         && (match_sc_case (stringa_C, car (car (indice_diz [i_indice]))) > 0)
#endif // ILWIN
        )
          ++i_indice;
  vecchio_el = indice_diz [--i_indice];
  while (   ((nuovo_el = cdr (vecchio_el)) != NULLP)
#ifdef ILDOS
         && ((differenza = match_sc (stringa_C, car (car (nuovo_el)))) > 0)
#endif // ILDOS
#ifdef ILWIN
         && ((differenza = match_sc_case (stringa_C, car (car (nuovo_el)))) > 0)
#endif // ILWIN
        )
    vecchio_el = nuovo_el;
  if (differenza == 0) return car (nuovo_el);
  else return NULLP;
}
 
/*-----------------------------------------------------------------------
  mette una parola di nome specificato nel dizionario degli oggetti MLOGO
  se non c' e'; ritorna comunque la parola messa o preesistente
  -----------------------------------------------------------------------*/
// node metti_sc (char *stringa_C)
node metti_sc (const char *stringa_C)
// char *stringa_C;
{
  return ((cerca_sc (stringa_C) != NULLP) ? car (nuovo_el)
                                          : nuovap (logstr (stringa_C)));
}

int isCercaTipo = FALSE;	// 981119
/*-----------------------------------------------------------------------
  ritorna l' oggetto di tipo specificato associato a parola, oppure NULLP
  -----------------------------------------------------------------------*/
node cercav (node parola, int tipo)
// node parola;
// int tipo;
{
	node oggetto;
#ifdef NORMALIZE
	node ref, normale;
	char c, s[MAXCHARS], *p = &s[0];
	// se parola non e' in forma normale ..
	if ((tipo != LWORD) && (! bit_tipo (parola, BIT_NORMALE))) {
		// .. se esiste riferimento, reperisce forma normalizzata ..
		if (typ (ref = cdr (parola)) == LREF) {
			if (isCercaTipo) return NULL;	// 981119 
			(parola = car (ref));
		}
		// .. altrimenti, cerca forma normalizzata ..
		else {
			getnam (s, parola);
			normalize_string(p);
			normale = cerca_sc (s);
			// .. e, se la trova, crea riferimento
			if (normale != NULLP) {
				set_tipo (normale, BIT_NORMALE);
 				if (normale != parola) {
  					putcdr (parola, cons (normale, cdr (parola), LREF));
					parola = normale;
				}
			}
		}
	}
#endif // NORMALIZE
	oggetto = parola;
	do oggetto = richiudi (cdr (oggetto));
	while ((typ (oggetto) != tipo) && (oggetto != parola));
	return (typ (oggetto) == tipo) ? oggetto : NULLP;
}

#ifdef NOT_USED 
/*---------------------------------------------------------------
  ritorna l' oggetto avente nome e tipo specificati, oppure NULLP
  ---------------------------------------------------------------*/
node cercao (node nome, int tipo)
// node nome;
// int tipo;
{
  node parola;
  if ((parola = cercap (nome)) == NULLP) return NULLP;
  else return cercav (parola, tipo);
}
#endif // NOT_USED 
 
/*--------------------------------------------------------------
  a partire da "da" cerca il primo oggetto del tipo specificato;
  se lo trova ritorna l' oggetto, altrimenti NULLP; comunque
  aggiorna "da" per consentire la prosecuzione della ricerca
  -------------------------------------------------------------*/
// NOTA: "da" e' una lista di parole ?
node cercat (node *da, int tipo)
// node *da;
// int tipo;
{
	isCercaTipo = TRUE;		// 981119
	node el, oggetto;
	el = *da;
	do {
		oggetto = cercav (car (el), tipo);
		el = cdr (el);
	}
	while ((el != NULLP) && (oggetto == NULLP));
	*da = el;
	isCercaTipo = FALSE;	// 981119
	return oggetto;
}

/*--------------------------------------------------------------------------
  ritorna pointer a descrizione di un oggetto di tipo SFUN (system function)
  --------------------------------------------------------------------------*/
int get_sf (node fun)
// node fun;
{
#ifdef ILDOS
	movmem ((char *)&sis_tab [car (fun)], (char *)&descr_sf, 12);
#endif // ILDOS
#ifdef ILWIN
//	memcpy ((char *)&descr_sf, (char *)&sis_tab [car (fun)], 22);
	memcpy ((char *)&descr_sf, (char *)&sis_tab [car (fun)], sizeof(struct primitiva));
	getnam (fname, fun);
#endif // ILWIN
  return TRUE;
}

/*-----------------------------------------------
  costruisce il valore di un oggetto di tipo UFUN
  -----------------------------------------------*/
node makeuf (node lista_arg, node corpo)
// node lista_arg, corpo;
{
  return cons (lista_arg, cons (corpo, NULLP, LIST), LIST);
}

/*-----------------------------------------------------------------
  ritorna la descrizione di un oggetto di tipo UFUN (User Function)
  -----------------------------------------------------------------*/
void get_uf (node *lista_arg, node *corpo, node oggetto)
// node *lista_arg, *corpo, oggetto;
{
  node locale;
  locale = car (oggetto);
  *lista_arg = car (locale);  
  *corpo = car (cdr (locale));
}

/*-------------------------------------------------------------------------
  crea un nuovo oggetto di tipo e valore specificati 
  associato alla parola specificata, che deve gia' esistere nel vocabolario
  NOTA: il nuovo oggetto viene inserito subito dopo la parola in un anello
  -------------------------------------------------------------------------*/
node mettio (node parola, node valore, int tipo)
// node parola, valore;
// int tipo;
{

#ifdef NORMALIZE
	node ref, normale;
	char c, s[MAXCHARS], s2[MAXCHARS], *p = &s[0];
	// se parola non e' in forma normale ..
	if (! bit_tipo (parola, BIT_NORMALE))
		// .. se esiste riferimento, reperisce forma normalizzata
		if (typ (ref = cdr (parola)) == LREF) {
			(parola = car (ref));
trap(typ (parola) == LWORD);
trap(bit_tipo (parola, BIT_NORMALE));
		}
		// .. altrimenti, crea parola normalizzata e inserisce riferimento
		else { 
			getnam (s, parola);
			strcpy (s2, s);
			normalize_string(p);
			normale = metti_sc (s);
			set_tipo (normale, BIT_NORMALE);
			if (normale != parola) {
   				putcdr (parola, cons (normale, cdr (parola), LREF));
				parola = normale;
			}
		} 
	// associa a parola normalizzata oggetto di tipo e valore richiesto
#endif // NORMALIZE

  return putcdr (parola, cons (valore, cdr (parola), tipo));
}

/*------------------------------------------------------------------
  cancella l' oggetto specificato staccandolo dall' anello di parola
  NOTA: non e' lecito cancellare con canceo oggetti di tipo LWORD
  ------------------------------------------------------------------*/
void canceo (node oggetto)
// node oggetto;
{
  node prima, dopo;
  dopo = cdr (oggetto);
  while ((dopo = richiudi (dopo)) != oggetto) {
    prima = dopo;
    dopo = cdr (dopo);
  };
  putcdr (prima, cdr (oggetto));
}

/*--------------------------------------------------------------------------
  cancella l' oggetto di tipo specificato associato alla parola specificata;
  ritorna l' oggetto cancellato o NULLP
  --------------------------------------------------------------------------*/
node cancet (node parola, int tipo)
// node parola;
// int tipo;
{
  node oggetto;
  if ((oggetto = cercav (parola, tipo)) != NULLP) canceo (oggetto);
  return oggetto;
}

/*---------------------------------------------------
  ritorna il nome (stringa MLOGO) di un oggetto MLOGO
  ---------------------------------------------------*/
node nome (node oggetto)
// node oggetto;
{
  int tipo;
  char s [MAXCHARS];
  if ((tipo = typ (oggetto)) == LSTR) return oggetto;
  else if (tipo == LBCD) {
    edo_num (oggetto, s);
    return logstr (s);
  }
  else return car (cercav (oggetto, LWORD));
}

/*-----------------------------------------------------------------
  converte il nome di un oggetto MLOGO in una stringa C preallocata
  -----------------------------------------------------------------*/
void getnam (char *s, node oggetto)
// char s [];
// node oggetto;
{
  pacstr (s, nome (oggetto));
}

/*--------------------------------------------------
  ritorna TRUE se l' oggetto e' di tipo LWORD o LSTR
  --------------------------------------------------*/
int tipoalfa (node oggetto)
// node oggetto;
{
  int tipo;
  return (((tipo = typ (oggetto)) == LWORD) || (tipo == LSTR));
}

void PREC ()
{
  risultato = paroprec (arg_1);
}

void SUCC ()
{
  risultato = parosucc (arg_1);
}

#ifdef NORMALIZE

/*----------------------------------------------------------------------
  ritorna stringa, o sua forma normale se corrisponde a parola riservata
  ----------------------------------------------------------------------*/
char *NormalSc (char *s)
// char *s;
{
	static char normal[MAXCHARS];
	char *p = &normal[0];
	unsigned char c;
	if (IsNormalUpper)
		while ((c = *s++) != '\0')
			*p++ = toupper(c);
	else
		while ((c = *s++) != '\0')
			*p++ = tolower(c);
	*p = '\0';
	return &normal[0];
}

/*----------------------------------------------------------
  ritorna parola, o sua forma normale se e' parola riservata
  ----------------------------------------------------------*/
node NormalKw (node parola)
// node parola;
{
	node ref, res;
//> 000509
//	if (	(typ (oggetto) == LWORD)
	if (	(typ (parola) == LWORD)
//<
		&&	(! bit_tipo (parola, BIT_NORMALE))
		&&	(typ (ref = cdr (parola)) == LREF)
		&&	(bit_tipo (res = car (ref), BIT_RISERVA)))
		return res;
	else return parola;
}

/*-------------------------------------------------------------------
  ritorna vero se parola corrisponde a parola chiave, a meno del case
  -------------------------------------------------------------------*/
int MatchKw (node parola, node kw)
// node parola, kw;
{
	node ref;
trap ((typ (kw) == LWORD) && bit_tipo (kw, BIT_NORMALE));
	if (parola == kw) return TRUE;
	else if (typ (parola) != LWORD) return FALSE;
	else if (typ (ref = cdr (parola)) == LREF)
		return (car (ref) == kw);
	else if (match_nocase (car (parola), car (kw)) == 0) {
   		putcdr (parola, cons (kw, cdr (parola), LREF));
		return TRUE;
	}
	else return FALSE;
}

#endif // NORMALIZE

/* ILDIZION.C */
