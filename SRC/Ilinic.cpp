/* ILINIC.C	G. Toffoli

001227
001218	spostato caricamento libreria in ilnodi.cpp
001211	ritoccato nome_lib
001123	pulizia (ILCONFIG, NORMALIZE)
001122	continuato configurazione con libreria
000102	ridefinito pc_CASEIGNOREDP
980831	modificato dichiaraz. ConfigGet in ilconfig.h
980828	preparazione per conversione a C++
980728	ripetuto in ini_sis inizializzazione di pc_VUOTA (in ini_diz)
971205	is_maiuscole (primitive MAIUSCOLE e ASMAIUSCOLE) falso per default
970806	esecuzione di file LOGO di partenza: LoadFileList
970316	continuato recupero
970315	tentato recupero update del 02.10.96 (NORMALIZE)
970207	tentato recupero update del 02.10.96 (ILCONFIG)
960517	riabilitato chiamata a ini_flussi
960423	altre disattivazioni temporanee
960410	disattivazioni definitive e provvisorie
960409	prima vers. per Windows da Iperlogo del 06.06.89
*/

#include "stdafx.h"

#define TEMP	// per disattivazioni temporanee

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

void ini_stack ();
void ini_timer ();
#define NODI_PAGINA 1638

// extern char nome_lib[];	// nome libreria da cercare
int LeggiLibreria();

#include <windows.h>
#ifndef WIN32
char FAR * FAR PASCAL ConfigGet (int i);
#else // WIN32
#include "ilconfig.h"
#endif // WIN32

char *NormalSc (char *);
// #define pc_CASEIGNOREDP pc_MODO
#define pc_CASEIGNOREDP kw[84]

void ini_main ()
{
  
	int pagine_lette = 0;
	is_maiuscole = FALSE;

	if ((pagine = prepara_mem ()) == -1) {	/* occupa memoria espansa */
		printf_null ("Non ho trovato spazio per i nodi.");
		exit(2);
	}
	tot_nodi = pagine * (long) NODI_PAGINA;
	frelst = NODI_RIS;				/* escludo da catena nodi riservati */
	nodi = tot_nodi - NODI_RIS;

//	if (nome_lib [0])				/* e' specificato nome di libreria ? */
	if (is_lib)						/* versione con libreria ? */
		pagine_lette = LeggiLibreria ();	/* carica libreria */

	inifre (pagine, pagine_lette);	/* (completa) iniz. lista dei nodi liberi */
	putcdr (tot_nodi-1, NULLP);		/* tappo a ultimo nodo della catena */
	if (! is_lib) ini_diz ();		/* inizializza dizionario delle parole */
	ini_sis ();						/* legge parole di sistema e messaggi */

	pc_VUOTA = mettip (S_VUOTA);	// 980728: ripristino (forse alterata da ini_sis ?)
						/* assegna valori default a variabili globali */ 
	mettio (pc_CASEIGNOREDP, pc_VERO, LVAR);

	schermi = 1;		/* numero schermi per default */
	ini_stack ();
	ini_timer ();
	ini_view ();		/* inizializza gestione viewport */
	ini_con ();			/* inizializza i/o di console in finestra 0 */
	ini_testo ();		/* inizializza finestre di testo */
	ini_flussi ();		/* inizializza flussi */
}

/* da MLDIZION.C */

/*-----------------------------------------------------------------------------
  riserva primi 2 nodi : indir. assegnati per convenz. a lista e stringa vuota;
  inizializza il dizionario MLOGO con 2 parole speciali, la piu' piccola e la
  piu' grande in ordine alfabetico, usate come "tappi" in operazioni di ricerca
  ---------------------------------------------------------------------------*/
void ini_diz ()
{
  int j;
  node i, cella, stringa;
  for (i = NULLP; i <= NULLVALUE; ++i) {
    putcar (i, NULLP);
    putcdr (i, NULLP);
  };
  puttyp (NULLP, LIST);		/* nodo 0 sta per lista vuota */
  puttyp (S_VUOTA, LSTR);	/* nodo 1 sta per stringa vuota */
				/* inizializza dizionario con indice */
				/* dizionario iniz. contiene parola vuota */
  putcar (dizionario, pc_VUOTA = cons (S_VUOTA, (dizionario = NULLP), LWORD));
  n_parole = 1;
  for (j = 0; j <= n_indice-1; ++j) // 960423: sost. "i" con "j"
    indice_diz [j] = dizionario;
				/* mette in dizionario parole di indirizzo
				   prescelto corrisp. a simboli di delimitat.*/
  for (i = DEL_FINELINEA; i<= DEL_MAGGIORE; ++i) {
    iwtstr ();
    cercap (stringa = wtstr (s_del [i]));
    puttyp (i, LWORD);
    putcar (i, stringa);
    putcdr (vecchio_el, cella = cons (i, nuovo_el, LIST));
    putcdr (i, vecchio_el);
    putcdr (car (nuovo_el), cella);
    ++n_parole;
  };
  for (i = 0; i < DIM_CODICI; ++i) tab_prog [i] = S_VUOTA;
}

/* da MLFLUSSI.C */

/*--------------------------------------------------------------------
  legge file MLOGO.SIS contenente nella lingua configurata :
  le costanti numeriche che vengono memorizzate in ordine in bcd [];
  le parole chiave, che vengono memorizzate in ordine in kw [];
  le primitive di sistema nello stesso ordine di MLSIS.ASM;
  i messaggi di errore e gli altri messaggi :
  - costruisce la super-stringa dei messaggi 
  - insieme costruisce un vettore in cui ogni elemento contiene
    l' indice del carattere iniziale del messaggio corrispondente
  -------------------------------------------------------------------*/ 

void ini_sis ()
{
	int i_parole;
	char *s;
	node i, locale;
	node parola;
//>001123: spostato internamente
//	if (! is_lib) {		/* interna le parole */
		i_parole = 0;

						/* interna le parole chiave */
    	i = 0;				/* indice di kw in tab. kw */
 		while ((locale = int_sis (ConfigGet (i_parole++))) != NULLP)
		{
if (! is_lib) {
			parola = mettip (locale);
			set_tipo (parola, BIT_NORMALE | BIT_RISERVA);
			kw [i++] = parola;
}
		}
						/* interna le primitive */
		i = 0;					// indice di prim. in tab. ILSIS
 		s = ConfigGet (i_parole++);
    	while ((locale = int_sis (s)) != NULLP) {
if (! is_lib) {
			parola = mettip (locale);
			set_tipo (parola, BIT_NORMALE);
			mettio (parola, i, SFUN);
}
 			s = ConfigGet (i_parole++);
			if (*s == ',')
				++s;				// sinonimo
 			else ++i;				// nuova primitiva
    	}
//	}
//<001123
						/* legge i messaggi */
	i_s_stringhe = i_tab_stringhe = 0;
	n_messaggi = 1;
	while (leggi_msg (ConfigGet (i_parole++)))
		++n_messaggi;
						/* legge i messaggi di errore */
	while (leggi_msg (ConfigGet (i_parole++)));
						/* converte e memorizza costanti bcd */
	if (! is_lib) {
		i = 0;					// indice di costante BCD (??)
#ifndef TEMP // va in errore internando BCD
		while (locale = int_sis (parole [i_parole++]))
			bcd [i++] = locale;
#endif // TEMP
	}
}

/*------------------------------------------------------------------
  ritorna una stringa LOGO, letta sequenzialmente da buffer di dati
  statici visto come buffer del device FOGLIO, una per linea;
  se e' un numero la ritorna in rappresentazione numerica LOGO;
  se incontra una linea di terminazione di un "blocco" ritorna FALSE
  ------------------------------------------------------------------*/

node int_sis (char *s)
{
	if (s [0] == '#') return NULLP;
	else {
//>001123
		if (is_lib)
			return 1;
		else {
//<001123
			if (edi_num (s, &oggetto) == 0) {
				oggetto = logstr (NormalSc (s));
			}
			return oggetto;
		} // 001123
	}
}

int leggi_msg (char *s)
{
  if (s [0] == '#') return FALSE;
  else {
    tab_stringhe [i_tab_stringhe++] = i_s_stringhe;
    strcpy (&s_stringhe [i_s_stringhe], s);
    i_s_stringhe = i_s_stringhe + strlen (s) + 1;
    return TRUE;
  };
}

/* ILINIC.C */
