/*ILCONTRO.C	G. Toffoli

020605	in sf_call aggiunto catch di eccezioni
020531	ripristinato contr_conv: fatto invece pulizia di _w in ilsis.cpp
011228	corretto contr_conv (conversione non richiesta di stringa a parola)
000125	corretto numero argomenti in contr_conv
991125	in modalità DEBUG trap provoca overflow
990913	aggiunto CONSUMA
990415	in LOCALE aggiunto sintassi 2 (n. argom. illimitato)
990325	in contr_conv complet. contr. su lista di parole/numeri
990323	azzeramento di variabili globali bcd[i] in sf_call
990311	ritoccato tracciamento di definizione procedura (PER)
990310	ridefinita macro trap, corretto tracciamento
		rivisto controllo in uf_ret
		in parenout controllo numero "argomenti" trovati
990309	in uf_call / uf_ret, controllo residuo in stack dei valori 
990112	corretto RECUPERA per foglio
981125	predisposto limitazione di accesso a classi di primitive
980902	modificato blk_out e _vai per far funzionare VAI (e' corretto ?)
980828	preparazione per conversione a C++; corretto pop_args()
		sostituito min e max con __min e __max
980525	Corretto controllo su argomento array (rivedere tutto)
980505	Completato controllo su argomento array
980107	Modificato controllo su finestra TARTA/FOGLIO non esistente 
971229	controlli su pausa da menu
971217	Aggiunto controllo su finestra TARTA/FOGLIO non esistente 
971205	Riattivato e modificato gestione di comando HALT (ex  BREAK ta tastiera) 
971110	aggiunto rinfresco di pannello stato di TARTA 
970908	per compatibilita' VC++5.0: bool -> bool_ 
970603	ripristino di FOGLIO, implementazione di editor 
970422	recupero update del 02.10.96 (normalizzazione)
960507	sostituito MOVMEM e SETMEM
960410	disattivato RECUPERA
960409	prima vers. Windows da Iperlogo del 20.02.89
		VEDERE versione precedente per memoria di LBCD
*/

#include "stdafx.h"
#include "iperlogo.h"
 
#define ILWIN
#define NORMALIZE

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include "ilsfun.h"		// 981121
extern struct primitiva descr_sf;
extern int Passpartout;

#ifdef NORMALIZE
int MatchKw (node, node);
#endif // NORMALIZE

#ifdef ILWIN

#include <sys\stat.h>
#include <io.h>
#include "assert.h"

void BreakOnDebug();
void FatalExit (char* s);
char* MsgInvalidObject = "Errore di sistema (oggetto non valido)";

#ifdef _DEBUG
// #define trap(exp) \
//	( (exp) ? (void) 0 : uno/0 )
#define trap(exp) \
	if (! (exp)) BreakOnDebug()
#else // _DEBUG
#define trap(exp) \
	if (! (exp)) FatalExit (MsgInvalidObject)
#endif // _DEBUG

#define VALID(n) trap((n >= 0L) && (n < tot_nodi))

			/* codifica dei tipi di contesto (id_contesto) */
#define CT_EVENT	3L	/* contesto attivato da richiesta asincrona */
#define NULL_DEV	-1

void RUNRESULT ();
void LabelPause (char *label);
//int QueryWindows (int mask, node *result);
int currentWindow (int mask);

int idRun = 0;
#define ID_RUN			1
#define ID_RUNRESULT	2

#define REP_COUNT
#ifdef REP_COUNT
int is_ripeti = 0;
//int RepTotal = 0, RepCount = 0;
#endif // REP_COUNT

// int EditorResetRead (int nview);
void EditorResetRead (int nview);	// 980831

void AssertContesto()
{
	VALID(id_contesto);			/* identificatore del motivo di attivazione contesto */
// int	dev_recupera,			/* device per comando recupera */
	trap(liv_procedura >= 0);	/* livello di nest delle procedure LOGO */
	trap(in_liv_proc >= 0);		/* valore di liv_procedura a inizio PAUSA/RECUPERA */
	trap(liv_funzione >= 0);	/* livello di profondita' delle funzioni */
	trap(in_liv_funzione >= 0);	/* valore di liv_funzione a inizio PAUSA/RECUPERA */
	VALID(funzione);			/* indir. funzione (primitiva o procedura) corrente */
	trap(liv_esecuzione >= 0);	/* livello di discesa albero di esecuzione (blocchi) */
	trap(val_verifica >= 0);	/* valore corrente del predicato argomento di "se" */
#ifdef REP_COUNT
	trap(RepTotal >= 0);
	trap(RepCount >= 0);
#endif // REP_COUNT
	trap(conto_esegui >= 0);	/* contatore delle iterazioni di un blocco */
	VALID(token);				/* elemento della lista di comando */
	VALID(ini_token);    		/* token iniziale */
	trap(n_arg_attesi >= 0);	/* numero argomenti attesi */
	trap(n_arg_trovati >= 0);	/* numero argomenti incontrati */
	trap(parentesi >= -1);		/* */
	trap(conto_parentesi >= 0);	/* conto algebrico parentesi in valut. espressione */
	trap((p_sc >= 0) && (p_sc < p_sv));		/* pointer allo stack di controllo */
	trap(p_sv < DIM_LSTACK);				/* pointer allo stack dei valori */
	trap(ini_p_sv < DIM_LSTACK);			/* pointer iniziale allo stack dei valori in contesto*/
	VALID(linea_com);			/* lista che rappresenta una linea di comando parsed */
}

#endif // ILWIN

int	i_range;	/* codice di controllo di range numerico */
int	range [2*11] = 	/* tabella dei range numerici */
{
	0, 0,			/* non usato */
	0, 32767,		/* 1 */
	0, 1,			/* 2 */
	0, 3,			/* 3 */
	0, 15,			/* 4 , gia' 0-31 */
	0, 25,			/* 5 */
	0, 127,			/* 6 */
	0, 255,			/* 7 */
	0, 388,			/* 8 */
	0, 0,			/* 9 */
	-10000, 10000		/* 10 */
};
 
/*---------------------------------------------------------------
  dichiarazione di errore in controllo centralizzato di argomento
  ---------------------------------------------------------------*/
void err_arg ()
{
#ifdef ILDOS
	err (5, val_arg);
#endif // ILDOS
#ifdef ILWIN
	err2 (5, val_arg);
#endif // ILWIN
}

/*----------------------------------------------------------------------------
  pop degli argomenti dopo esecuzione di primitiva con n. argomenti illimitato
  ---------------------------------------------------------------------------*/
void pop_args ()
{
	int i;
//> 980828
//	for (i = 1; i <= n_argomenti; ++i) pop_sv (i);
	for (i = 1; i <= n_argomenti; ++i) pop_sv ();
//>
}

/*-----------------------------------------------------------------
  se non e' atteso un valore da' errore,
  altrimenti pone un valore sul v_stack e incrementa n_arg_trovati;
  -----------------------------------------------------------------*/
void push_arg (node arg)
// node arg;
{
#ifdef ILDOS
  if (   (n_arg_trovati < n_arg_attesi)
      || is_riporta
      || ((typ (funzione) == SFUN) && get_sf (funzione) && (N_ILLIMITATO))
      || ((liv_procedura == in_liv_proc) && (liv_esecuzione == 0))
  ) {
    push_sv (arg);
    ++n_arg_trovati;
  } else err (12, arg);
#endif // ILDOS
#ifdef ILWIN
//  if (   (n_arg_trovati < n_arg_attesi)
//      || is_riporta
//      || ((typ (funzione) == SFUN) && get_sf (funzione) && (N_ILLIMITATO))
//      || ((liv_procedura == in_liv_proc) && (liv_esecuzione == 0))
//  ) {
    push_sv (arg);
    ++n_arg_trovati;
//  } else err2 (12, arg);
#endif // ILWIN
}

void _pausa ()
{
	msg (6, stk_funzioni [liv_procedura]);
	is_pausa = FALSE;		/* resetta flag settato da PAUSA */
	pausa_recupera (CT_PAUSA);
	dev_recupera = CONSOLE;	/* redirige input comandi su CONSOLE */
//> 971012
	LabelPause ("Continua");
//< 971012
}
 
void _continua ()
{
	--is_continua;
	while (is_continua != liv_contesto) pop_contesto ();
	is_continua = FALSE;
	err_token = token;
//> 971012
	LabelPause ("Pausa");
//< 971012
}

/*--------------------------------------------------------------
  NOTA: viene annullato eventuale ACCHIAPPA omonimo preesistente
  NOTA: e' annullata la nota precedente (09.09.85)
  --------------------------------------------------------------*/
void _acchiappa ()
{
  push_contesto (arg_1);
  blocco = arg_2;
  conto_esegui = 1;
  _esegui (arg_1);
  is_acchiappa = FALSE;
}

/*-----------------------------------------------------------------------------
  se nello spazio delle parole non esiste una variabile di nome specificato la
  crea e considera che il suo vecchio valore sia NULLVALUE (valore invalido);
  comunque mette il valore specificato nella variabile e salva su stack dei
  valori (l' indirizzo del)la variabile e il vecchio valore
  ---------------------------------------------------------------------------*/
void pshloc (node parola, node nuovo_val)
// node parola, nuovo_val;
{
  node oggetto, vecchio_val;
  if ((oggetto = cercav (parola, LVAR)) != NULLP) vecchio_val = car (oggetto);
  else oggetto = mettio (parola, (vecchio_val = NULLVALUE), LVAR);
  putcar (oggetto, nuovo_val);
  push_sv (oggetto);
  push_sv (vecchio_val);
}
 
/*------------------------------------------------------------------------
  ripristina i vecchi valori di n variabili; in cima al vstack si trovano
  n coppie (variabile, vecchio-valore); se il vecchio valore era NULLVALUE
  (valore non assegnato) la variabile NON viene piu' cancellata dallo spazio
  delle parole
  ------------------------------------------------------------------------*/
void poploc (int n)
// int     n;
{
  int i;
  node risultato, oggetto, valore;
  if (n_arg_trovati != 0) risultato = pop_sv ();
  for (i = 1; i <= n; ++i) {
    valore = pop_sv ();
    oggetto = pop_sv ();
    putcar (oggetto, valore);
  };
  if (n_arg_trovati != 0) push_sv (risultato);
}
 
void _vai ()
{
  int trovato;
  node locale;
  locale = token;
  token = ini_token;
  trovato = FALSE;
  while (! trovato) {
    if (token != NULLP) {	/* cerca la sequenza ETICHETTA "<parola> */
      gettok ();
#ifndef NORMALIZE
      if ((val_token == pc_ETICHETTA) && (next_val == DEL_VIRGOLETTE)) {
#else // NORMALIZE
//    if (MatchKw (val_token, pc_ETICHETTA) && MatchKw (next_val, DEL_VIRGOLETTE)) {
      if (   MatchKw (val_token, pc_ETICHETTA)
		  && (next_val == DEL_VIRGOLETTE)) {	// 980902
#endif // NORMALIZE
        token = cdr (token);
        gettok ();
        trovato = (next_val == dove_vai);
      };
      token = cdr (token);
    }
    else if (liv_esecuzione > 0) {	/* cerca in blocco esterno */
      blk_out ();
      parenout (conto_parentesi);
      token = ini_token;
    }
    else break;				/* esito negativo della ricerca */
  }
  if (trovato)
    token = cdr (token);
  else {
    token = locale;
#ifdef ILDOS
    err (16, dove_vai);
#endif // ILDOS
#ifdef ILWIN
    err2 (16, dove_vai);
#endif // ILWIN
  };
  is_vai = FALSE;
}
 
/*----------------------------------------------------------
  salvataggio di parte del contesto sullo stack di controllo
  ----------------------------------------------------------*/
void pushco ()
{
  AssertContesto();
  push_sc ((node) ((conto_parentesi << 12) + (n_arg_trovati << 6) + n_arg_attesi));
  push_sc ((node) parentesi);
  push_sc (err_token);
  AssertContesto();
}
 
/*---------------------------------------------------------
  ripristino di parte del contesto dallo stack di controllo
  ---------------------------------------------------------*/
void popco ()
{
  AssertContesto();
  err_token = pop_sc ();
  parentesi = (int) pop_sc ();
  n_arg_attesi = (int) pop_sc ();
  conto_parentesi = (n_arg_attesi >> 12);
  n_arg_trovati = (n_arg_attesi >> 6) & 0x3F;
  n_arg_attesi = n_arg_attesi & 0x3F;
  AssertContesto();
}
 
/*azioni comuni al riconoscimento di un token funzione (sfun o ufun)*/
void f_in ()
{
  AssertContesto();
  push_sc (funzione);
  pushco ();
  funzione = val_token;
  n_arg_trovati = 0;
  ++liv_funzione;
  err_token = prev_token;
  AssertContesto();
}
 
/*azioni comuni al termine della valutazione di sfun e ufun*/
void f_out ()
{
  AssertContesto();
  popco ();
  funzione = pop_sc ();
  VALID(funzione);
  if (is_funzione) push_arg (pop_sv ());
  --liv_funzione;
#ifdef ILDOS
  if (conto_esegui > 0) _esegui (NULLP);
#endif // ILDOS
#ifdef ILWIN
  if (conto_esegui > 0) _esegui ((node) idRun);
  idRun = 0;
#endif // ILWIN
  AssertContesto();
}

#ifdef ILDOS
node _traccia (tipo)
int tipo;
{
  node oggetto;
  if ((oggetto=cercav (arg_1, tipo)) != NULLP) set_tipo (oggetto, BIT_TRACCIA);
  return oggetto;
}
#endif // ILDOS
#ifdef ILWIN
node TraceSym (int tipo, node sym)
// int tipo;
// node sym;
{
  node oggetto;
  if ((oggetto=cercav (sym, tipo)) != NULLP) set_tipo (oggetto, BIT_TRACCIA);
  return oggetto;
}

node UntraceSym (int tipo, node sym)
// int tipo;
// node sym;
{
  node oggetto;
  if ((oggetto=cercav (sym, tipo)) != NULLP) res_tipo (oggetto, BIT_TRACCIA);
  return oggetto;
}
#endif // ILWIN
 
/*----------------------------------------
  fa eventuale tracciamento della funzione
  ----------------------------------------*/
void tr_comando ()
{
  int i;
  node lista;
  if ((bit_tipo (funzione, BIT_TRACCIA)) || is_traccia) {
    lista = NULLP;
    for (i = 1; i <= n_arg_trovati; ++i) lista = cons (
      (typ (funzione) == UFUN) ? get_sc (-i+1) : get_sv (i),
      lista, LIST);
    dev_out = CONSOLE;
    indenta (); _scrivi (funzione); spazio ();
/*>990311*/
	if (cercav (funzione, LWORD) == pc_PER)
		_scrivi (car (token));
/*<*/
	_scrivi (lista); a_capo ();
    dev_out = dev_scrivi;
  };
}
 
/*-------------------------
  uscita da parentesi tonde
  -------------------------*/
void parenout (int n)
// int n;
{
	int i;
	int locale;
	for (i = 1; i <= n; ++i) {
/*>990310*/
		if (n_arg_trovati > 1) {
			err2 (12, get_sv (1));
//	break;
		}
/*<*/
		locale = n_arg_trovati;
		popco ();
		funzione = pop_sc ();
		n_arg_trovati = n_arg_trovati + locale;
	}
}
 
/*----------------------------------------
  azioni comuni all' ingresso in un blocco
  ----------------------------------------*/
void blk_in (int is_arg_atteso)
// int is_arg_atteso;
{
  AssertContesto();
  push_sc (funzione);
  funzione = NULLP;
  push_sc (ini_token);
  err_token = ini_token = token;
  push_sc (token);
  push_sc ((node) conto_esegui);
#ifdef REP_COUNT
  push_sc ((node) RepCount);
  push_sc ((node) RepTotal);
#endif // REP_COUNT
  push_sc ((node) val_verifica);
  pushco ();
  conto_parentesi = 0;
  parentesi = -1;
  n_arg_attesi = is_arg_atteso;
  push_sc ((node) n_locali);
  n_locali = 0;
  ++liv_esecuzione;
#ifdef REP_COUNT
  if (is_ripeti) {
	is_ripeti = 0;
  	RepTotal = conto_esegui;
  	RepCount = 1;
  } else {
  	RepTotal = 0;
  }
#endif // REP_COUNT
  conto_esegui = 0;
  val_verifica = FALSE;
  n_arg_trovati = 0;
  AssertContesto();
}
 
/*azioni comuni all' uscita da un blocco*/
void blk_out ()
{
#ifdef REP_COUNT
  int OldTotal, OldCount;
#endif // REP_COUNT
  int locale;
  node id;
  AssertContesto();
  parenout (conto_parentesi);
  poploc (n_locali);
  n_locali = (int) pop_sc ();
  locale = n_arg_trovati;
  popco ();
  n_arg_trovati = n_arg_trovati + locale;
  val_verifica = (int) pop_sc ();
#ifdef REP_COUNT
  OldTotal = RepTotal;
  OldCount = RepCount;
  RepTotal =  (int) pop_sc ();
  RepCount =  (int) pop_sc ();
#endif // REP_COUNT
  conto_esegui = (int) pop_sc ();
// if (is_vai) conto_esegui = 0;
  if (is_vai)			// 980902
	  conto_esegui = 1;	//
  token = pop_sc ();
  ini_token = pop_sc ();
  funzione = pop_sc ();
  --liv_esecuzione;
  if (--conto_esegui > 0) {
  	blk_in (0);
#ifdef REP_COUNT
	if (OldTotal) {
		RepTotal = OldTotal;
		RepCount = OldCount + 1;
	}
#endif // REP_COUNT
  }
  else {
    token = pop_sc ();
    if (   ((id = pop_sc ()) != NULLP)
        && (id == id_contesto)
       ) pop_contesto ();
#ifdef ILWIN
	else {
		if (id == ID_RUNRESULT) {
			if (n_arg_trovati == 0) { ++n_arg_trovati; push_sv (NULLP); }
			else push_sv (cons (pop_sv(), NULLP, LIST)); 
		}
	} 
#endif // ILWIN
  };
  AssertContesto();
}

void STOP ()
{
  is_stop = TRUE;
}
 
void RIPORTA ()
{
  risultato = arg_1;
  is_stop = TRUE;
  is_riporta = TRUE;
}
 
void VERIFICA ()
{
  val_verifica = bool_ (arg_1);
}
 
void SEV ()
{
	blocco = arg_1;
	conto_esegui = val_verifica;
	ha_blocco_valore = TRUE;
}
 
void SEF ()
{
	blocco = arg_1;
	conto_esegui = ! val_verifica;
	ha_blocco_valore = TRUE;
}
 
void SE ()
{
	conto_esegui = val_verifica = bool_ (arg_1);
	blocco = arg_2;
	ha_blocco_valore = TRUE;
}

#ifdef ILWIN
void IFELSE ()
{
	conto_esegui = 1;
  	blocco = (bool_ (arg_1)) ? arg_2 : arg_3;
	val_verifica = FALSE;
	ha_blocco_valore = TRUE;
}
#endif // ILWIN
 
/*esecuzione iterativa di un blocco di comandi*/
void RIPETI ()
{
	blocco = arg_2;
	conto_esegui = rnum_int (arg_1);
#ifdef REP_COUNT
	is_ripeti = TRUE;
#endif // REP_COUNT
	ha_blocco_valore = FALSE;		// non significativo ?
}
 
/*esecuzione di un blocco di comandi*/
void ESEGUI ()
{
	blocco = arg_1;
	conto_esegui = 1;
	ha_blocco_valore = TRUE;
#ifdef ILWIN
	idRun = ID_RUN;
#endif // ILWIN
}

#ifdef ILWIN
void RUNRESULT ()
{
	blocco = arg_1;
	conto_esegui = 1;
	ha_blocco_valore = TRUE;
	idRun = ID_RUNRESULT;
}

void REPCOUNT ()
{
//	int count = 0;
//	if (liv_esecuzione > 0) count = (int) get_sc(6+8);
//	risultato = int_num (count); 
	risultato = int_num (RepCount); 
}
#endif // ILWIN

void _esegui (node id)
// node id;
{
  push_sc (id);
  push_sc (token);
  token = blocco;
  blk_in (ha_blocco_valore && (n_arg_attesi > 0));
}
 
/*---------------------------
  ingresso in parentesi tonde
  ---------------------------*/
void parenin ()
{
  push_sc (funzione);
  funzione = NULLP;
  pushco ();
  n_arg_attesi = (n_arg_attesi > 0);
  n_arg_trovati = 0;
  parentesi = -1;
  ++conto_parentesi;
}

extern CIperlogoApp theApp;                   
void _letterale ();
/*-------------------------------------------------
  ingresso nella valutazione di una System Function
  -------------------------------------------------*/
void sf_in ()
{
	f_in ();
	get_sf (funzione);
//>990714
// NOTA !! - riciclato per PR_QUOTED codice e macro non utilizzati,
//			gia' introdotti per primitive MCI e MIDI,
//			da rinominare: QUOTED, IS_PR_QUOTED
	if (IS_PR_MM)
		_letterale ();
	else
//<
#ifndef TEMP	// da recuperare compatibilmente con NestedExec e gestione errori
	if (   (liv_funzione != stk_livelli [liv_procedura] + 1)
		&& (! IS_PR_ESEGUI) 
		&& ((! IS_PR_FUNZIONE) || IS_PR_PROC)
	) errore (19, funzione, NULLP);	/* primitiva non riporta ! */
#endif // TEMP
//> 981121
//	if ((IS_PR_GUI) && (theApp.m_regAccess < 3) && (! Passpartout))
//		errore (2, NULLP, funzione);	/* liv. di accesso non sufficiente */
//<
	n_arg_attesi = N_NOMINALE;
}

/*-------------------------------------------------------------
  se non esiste gia' una variabile locale con quel nome la crea
  -------------------------------------------------------------*/
void _locale ()
{
	int i, is_nuova;
	node oggetto, parola;
	is_locale = FALSE;
/*>990415
	resto_pn = arg_1;
*/
	int k;
	for (k = 0; k < n_argomenti; ++k)
		push_sc (pop_sv ());
	for (k = 0; k < n_argomenti; ++k) {
		resto_pn = pop_sc ();
/*<*/
		while ((parola = _parola ()) != NULLP) {
			is_nuova = TRUE;
			oggetto = cercav (parola, LVAR);
			for (i = 1; i <= n_locali; ++i) {
				if (get_sv (2 * i) == oggetto) is_nuova = FALSE;
			};
			if (is_nuova) {
				pshloc (parola, NULLVALUE);
				++n_locali;
			}
		}
/*>990415 */
	}
/*<*/
}

BOOL contr_listapar (node val_arg)
{
	while (val_arg != NULLP) {
		if (! tipoalfa (car (val_arg)))
			return FALSE;
		val_arg = cdr (val_arg);
	};
	return TRUE;
}

BOOL contr_listanum (node val_arg)
{
	while (val_arg != NULLP) {
		if (typ (car (val_arg)) != LBCD)
			return FALSE;
		val_arg = cdr (val_arg);
	};
	return TRUE;
}

/*------------------------------------------------------------------------
  controllo e eventuale conversione di un argomento; il tipo eventualmente
  modificato e' ritornato come valore della funzione; il valore eventualm.
  modificato viene ritornato per indirizzo
  ------------------------------------------------------------------------*/
int contr_conv (node *arg, int descr)
// node *arg;
// int descr;
{
  int   tipo;
  node risposta;
  char	s_locale [128];
  tipo = typ (val_arg = *arg);
  if (descr == 0) return tipo;		/* non e' previsto controllo */

  if (tipo == LIST) {	/* argomento lista */
    if (! (descr & A_LISTA)) err_arg ();
    else {
					/* controllo su lista di parole .. o numeri */
/*>990325
      if (descr & A_LISTAPAR)
		while (val_arg != NULLP) {
			if (! tipoalfa (car (val_arg))) {err_arg (); break;}
			val_arg = cdr (val_arg);
		}
*/
		BOOL isListaParNum =
			(    (! (descr & (A_LISTAPAR | A_LISTANUM)))
			  || ((descr & A_LISTAPAR) && contr_listapar (val_arg))
			  || ((descr & A_LISTANUM) && contr_listanum (val_arg))
			);
		if (! isListaParNum) err_arg ();
/*<*/
    };
    return LIST;
  }
//>980505
//else if (! (descr & (A_STRINGA|A_PAROLA|A_VEROFALSO|A_NUMERO))) err_arg ();
  else if (! (descr & (A_STRINGA|A_PAROLA|A_VEROFALSO|A_NUMERO|A_ARRAY))) err_arg ();
  /* controllo su numero */
//>980525
//if (descr & A_ARRAY) {
//  if (tipo != ARAY)
  if (tipo == ARAY) {
	if (! (descr & A_ARRAY))
//<980525
		err_arg ();
  }
//<980505
  /* controllo su numero */
  if (descr & A_NUMERO) {
    if (tipo != LBCD)
      if (tipoalfa (val_arg)) {
        pacstr (s_locale, nome (val_arg));
        if (edi_num (s_locale, &risposta) > 0) *arg = val_arg = risposta;
        else err_arg ();
      } else err_arg ();
					/* controllo di range su numero */
    if ((! is_errore) && ((i_range = (descr >> BIT_RANGE)) != 0)) {
      risposta = num_int (val_arg = roundnum (val_arg));
    };
    return LBCD;
  };
					/* conversione da numero a stringa */
  if (tipo == LBCD) {
    edo_num (val_arg, s_locale);
    *arg = val_arg = logstr (s_locale);
    tipo = LSTR;
  };
					/* eventuale forzamento a parola */
//>011228 020531
  if (descr & (A_PAROLA | A_VEROFALSO)) *arg = val_arg = mettip (val_arg);
//  if ((descr & (A_PAROLA | A_VEROFALSO)) && (! (descr & A_STRINGA)))
//	  *arg = val_arg = mettip (val_arg);
//>011228 020531
					/* controllo su valore logico */
  if (descr & A_VEROFALSO) {
#ifndef NORMALIZE
    if ((val_arg != pc_VERO) && (val_arg != pc_FALSO)) err_arg ();
#else // NORMALIZE
    if ((! MatchKw (val_arg, pc_VERO)) && (! MatchKw (val_arg, pc_FALSO))) err_arg ();
#endif // NORMALIZE
  }
#ifdef ILDOS
					/* controllo su nome di archivio */
  else if (   (descr & A_NOMEARC)
           && ((! nomearc (nome (val_arg))) || (outregs.h.al != 0))
          ) err_arg ()
#endif // ILDOS
          ;			/* usa side_effect di nomearc */
  return tipo;
}
 
/*-----------------------------------------------
  ingresso nella valutazione di una User Function
  -----------------------------------------------*/
void uf_in ()
{
  f_in ();
  get_uf (&lista_arg, &corpo, funzione);
  n_arg_attesi = lstlen (lista_arg);
}
 
/*---------------------------------------------------------------------
  finalizza l' esecuzione di una procedura LOGO  (User Function RETurn)
  con pop di uno stack-frame
  ---------------------------------------------------------------------*/
void uf_ret ()
{
  node procedura, loc_1, loc_2;
  loc_1 = err_token;
  loc_2 = token;
  is_funzione = is_riporta;
  is_riporta = FALSE;
// /*>990310
  procedura = stk_funzioni [liv_procedura--];
// >*/
  while (liv_esecuzione > 0) {
    if (conto_parentesi > 0) break;
    blk_out ();
  };
  if (conto_parentesi > 0) {
    err_token = loc_1;
    token = loc_2;
    errore (14, NULLP, NULLP);
	BreakOnDebug();
    return;
  };

/*>990309
  int ufun_p_sv =
		(int) pop_sc ()				// 6: p_sv salvato in uf_call
		- (2 * n_locali)			// variabili locali
		- (2 * (int) get_sc (1));	// variabili argomento
  if (p_sv < (ufun_p_sv - n_arg_trovati)) {
	  node locRisultato, locValore;
		if (n_arg_trovati != 0)
			locRisultato = pop_sv ();
		while (p_sv < ufun_p_sv) {	// spurga valori non consumati
			locValore = pop_sv ();
			if (TRUE) {				// introdurre opzione ?
				err2 (12, locValore);	// va in errore
				return;
			}
		}
		if (n_arg_trovati != 0)
			push_sv (locRisultato);
  }

<*/

/*>990310*/
  if (n_arg_trovati > ((is_funzione) ? 1 : 0)) {
	  ++liv_procedura;
	  err2 (12, get_sv (n_arg_trovati));	// va in errore
	  return;
  }
/*<*/

//  procedura = stk_funzioni [liv_procedura--];	// 990310: posposto

  if ((bit_tipo (procedura, BIT_TRACCIA)) || is_traccia) {
    dev_out = CONSOLE;
    indenta ();
/*>990310
    msg ((n_arg_trovati == 0) ? 7 : 8, procedura, get_sv (1));
*/
    msgsenza ((n_arg_trovati == 0) ? 7 : 8, procedura, get_sv (1));
	a_capo ();
/*<*/
    dev_out = dev_scrivi;
  };

  poploc (n_locali);				/* spurgo delle variabili locali a procedura */

  poploc ((int) pop_sc ());			/* 5: spurgo delle variabili argomento */
  n_locali = (int) pop_sc ();		/* 4: numero variabili locali proc. esterna*/
  liv_esecuzione = (int) pop_sc ();	/* 3 */
  val_verifica = (int) pop_sc ();	/* 2 */
#ifdef REP_COUNT
  RepTotal =  (int) pop_sc ();
  RepCount =  (int) pop_sc ();
#endif // REP_COUNT
  conto_esegui = (int) pop_sc ();	/******************************/
  token = pop_sc ();		/* 1" elemento di STACK-FRAME */
  ini_token = pop_sc ();
  is_stop = FALSE;
  f_out ();
}
 
void TRACCIA ()
{
#ifdef ILDOS
  if (n_argomenti == 0) is_traccia = TRUE;
  else if (! (_traccia (UFUN) || _traccia (SFUN)))
    err (2, arg_1);
#endif // ILDOS
#ifdef ILWIN
	node tail, ele;
	if (n_argomenti == 0) is_traccia = TRUE;
	else if (tipo_1 == LIST) {
  		tail = arg_1;
  		while (tail) {
  			ele = car (tail); tail = cdr (tail);
			if (! (TraceSym (UFUN, ele) || TraceSym (SFUN, ele)))
    			err2 (2, ele);
  		}
  	}
	else if (! (TraceSym (UFUN, arg_1) || TraceSym (SFUN, arg_1)))
		err2 (2, arg_1);
#endif // ILWIN
}

void NONTRACCIARE ()
{
#ifdef ILDOS
  is_traccia = FALSE;
  _nontracciare ();
#endif // ILDOS
#ifdef ILWIN
	node tail, ele;
	if (n_argomenti == 0) {
		is_traccia = FALSE;
		_nontracciare ();
	}
	else if (tipo_1 == LIST) {
  		tail = arg_1;
  		while (tail) {
  			ele = car (tail); tail = cdr (tail);
			if (! (UntraceSym (UFUN, ele) || UntraceSym (SFUN, ele)))
    			err2 (2, ele);
  		}
  	}
	else if (! (UntraceSym (UFUN, arg_1) || UntraceSym (SFUN, arg_1)))
		err2 (2, arg_1);
#endif // ILWIN
}

void LOCALE ()
{
  is_locale = TRUE;
}
 
void VAI ()
{
  is_vai = TRUE;
  dove_vai = arg_1;
}

void PAUSA ()
{
  is_pausa = TRUE;
}

int IsFuncRunning () {
	return (liv_funzione > in_liv_funzione);
}

int IsProcRunning () {
	return (liv_procedura > in_liv_proc);
}

int PauseLevel () 
{
  int i, level = 0;
  for (i = liv_contesto; i > 0; --i)
    if ((contesti [i])._id_contesto == CT_PAUSA) {	// contesto 0 è riservato
      level = i;
      break;
    };
    return level;
}
 
void CONTINUA ()
{
  int i;
  for (i = liv_contesto; i > 0; --i)
#ifdef ILDOS
    if ((contesti [i-1]).id == CT_PAUSA) {
#endif // ILDOS
#ifdef ILWIN
    if ((contesti [i])._id_contesto == CT_PAUSA) {	// contesto 0 è riservato
#endif // ILWIN
      is_continua = i;
      break;
    };
#ifdef ILDOS
	if (! is_continua) err (15);
#endif // ILDOS
#ifdef ILWIN
	if (! is_continua) err1 (15);
#endif // ILWIN
}
 
void ACCHIAPPA ()
{
  is_acchiappa = TRUE;
}
 
void LANCIA ()
{
  int i;
  for (i = liv_contesto; i > 0; --i) 
#ifdef ILDOS
    if ((contesti [i-1]).id == arg_1) {
#endif // ILDOS
#ifdef ILWIN
    if ((contesti [i])._id_contesto == arg_1) {	// contesto 0 e' riservato
#endif // ILWIN
      is_lancia = i;
      break;
    };
#ifdef ILDOS
	if (! is_lancia) err (17, arg_1);
#endif // ILDOS
#ifdef ILWIN
	if (! is_lancia) err2 (17, arg_1);
#endif // ILWIN
}
 
void _lancia ()
{
    --is_lancia;
    while (is_lancia != liv_contesto) pop_contesto ();
    is_lancia = FALSE;
}

void indenta ()
{
  int i;
  for (i = 1; i <= liv_procedura; ++i) spazio ();
}

void push_contesto (node id)
// node id;
{
  AssertContesto();
  if (liv_contesto == MAX_CONTESTO-1)
#ifdef ILDOS
	err (43);
#endif // ILDOS
#ifdef ILWIN
	err1 (43);
#endif // ILWIN
  else {
   id_contesto = id;
#ifdef ILDOS
  movmem ((char *)&id_contesto, (char *)&contesti [liv_contesto], DIM_CONTESTO);
#endif // ILDOS
#ifdef ILWIN
  contesti [liv_contesto+1] = contesti [0];
#endif // ILWIN
   ++liv_contesto;
   funzione = NULLP;
  }
  AssertContesto();
}

void pop_contesto ()
{
  int locale;
  AssertContesto();
  locale = dev_recupera;
  trap(liv_contesto > 0);
  --liv_contesto;
#ifdef ILDOS
  movmem ((char *)&contesti [liv_contesto], (char *)&id_contesto, DIM_CONTESTO);
#endif // ILDOS
#ifdef ILWIN
  contesti [0] = contesti [liv_contesto+1];
#endif // ILWIN
#ifdef ILDOS
  id_contesto = (liv_contesto == 0) ? 0 : (contesti [liv_contesto-1]).id;
#endif // ILDOS
#ifdef ILWIN
  id_contesto = (liv_contesto == 0) ? 0 : (contesti [liv_contesto])._id_contesto;	// cont. 0 e' riservato
#endif // ILWIN
#ifdef ILDOS
  linea_com = NULLP;
  if (locale != dev_recupera)
    if (! (_fstato [locale] & O_FINESTRA))
      f_chiudi (locale);
#endif // ILDOS
#ifdef ILWIN
  if (locale != NULL_DEV) {
    linea_com = NULLP;
    if (   (locale != dev_recupera)
        && (! (_fstato [locale] & O_FINESTRA))
       ) f_chiudi (locale);
  }
#endif // ILWIN
  AssertContesto();
}

void RECUPERA ()
{
#ifdef ILDOS
  dev_temp = nome2dev (arg_1);
#endif // ILDOS
#ifdef ILWIN
  if (tipo_1 == LIST) {
  	arg_1 = car (arg_1);
  	arg_2 = car (cdr (arg_1));
  }
  dev_temp = nome2dev (arg_1);
#endif // ILWIN
  if (dev_temp == -1) {
#ifdef ILDOS
    if ((dev_temp = f_apri (arg_1, O_RDONLY | O_RAW)) == 0)
#endif // ILDOS
#ifdef ILWIN
    if ((dev_temp = f_apri (arg_1, _O_RDONLY | _O_RAW, _S_IWRITE | _S_IREAD)) == 0)
#endif // ILWIN
#ifdef ILDOS
		err (31, arg_1);
#endif // ILDOS
#ifdef ILWIN
		err2 (31, arg_1);
#endif // ILWIN
    else if (dev_temp == -1)
#ifdef ILDOS
		err (21, arg_1);
#endif // ILDOS
#ifdef ILWIN
		err2 (21, arg_1);
#endif // ILWIN
  }
  else if (_fstato [dev_temp] & O_FOGLIO) {
		i_leggi_foglio = 0;
#ifdef ILWIN
//		EditorResetRead (0);
		EditorResetRead (_file [dev_temp]);	// 990112
#endif // ILWIN
  }
  else if (   (dev_temp == STAMPANTE)
           || (_fstato [dev_temp] & O_TARTA))
#ifdef ILDOS
		err (25, arg_1);
#endif // ILDOS
#ifdef ILWIN
		err2 (25, arg_1);
#endif // ILWIN
  if (! is_errore) is_recupera = TRUE;
}

/*-------------------------------------------------------------------
  primitiva per la commutazione dell' input sul file specificato.
  Puo' servire per ripristinare un ws salvato con la primitiva conserva
  -------------------------------------------------------------------*/
void _recupera ()
{
  is_recupera = FALSE;
  pausa_recupera (arg_2);
  dev_recupera = dev_temp;
}

/*--------------------------------
  azioni comuni a pausa e recupera
  --------------------------------*/
void pausa_recupera (node causa)
// node causa;
{
  push_contesto (causa);	/* salva il contesto su apposito stack */
  ini_p_sv = p_sv;		/* base di partenza per p_sv */
  token = NULLP;		/* forza ritorno apparente a top-level */
  in_liv_funzione = liv_funzione;/* nest globale delle funzioni */
  in_liv_proc = liv_procedura;/* nest globale delle procedure utente */
  liv_esecuzione = 0;		/* nest dei blocchi in proc. corrente */
  conto_parentesi = 0;
  parentesi = -1;
}

/*-------------------------------------------------------------------------
  attivazione della primitiva di sistema che implementa una System Function
  e ritorno a livello di nest inferiore
  con eventuale incremento di n_arg_trovati
  -------------------------------------------------------------------------*/
void TurtleStatusRefresh (int nview);	// aggiunto
//>020605
extern void abbassaSogliaNodi();
//<020605

void sf_call ()
{
  int i;
			/* garbage collection : */
     					/* resetta le 6 variabili globali A - S */
#ifdef ILDOS
  node locale;
  setmem ((char *)&A, 12, 0);
#endif // ILDOS
#ifdef ILWIN
  memset ((char *)&A, 0, 12);
#ifdef REP_COUNT
	is_ripeti = FALSE;
#endif // REP_COUNT
#endif // ILWIN
  libera_2 ();			/* esegue LIBERA se restano meno di 128 nodi */
  tr_comando ();
#ifdef ILDOS
  arg_1 = arg_2 = arg_3 = NULLP;
#endif // ILDOS
#ifdef ILWIN
/*>990323*/
  for (i = 3; i < DIM_BCD; ++i) bcd [i] = NULLP;
/*<*/
  for (i = 0; i < MAX_ARGS; ++i) args[i] = NULLP;
#endif // ILWIN
  get_sf (funzione);
  is_funzione = IS_PR_FUNZIONE;
//>000125
//n_argomenti = N_NOMINALE;
  n_argomenti = n_arg_trovati;
//><
  if (N_ILLIMITATO) {
    n_argomenti = n_arg_trovati;	//>000125 non serve più ?
    for (i = 1; i <= n_argomenti; ++i) {
      arg_1 = get_sv (i);
#ifdef ILDOS
      tipo_1 = contr_conv (&arg_1, descr_sf.descr_1);
#endif // ILDOS
#ifdef ILWIN
      tipo_1 = contr_conv (&arg_1, descr_sf.descrs[0]);
#endif // ILWIN
      mod_sv (arg_1, i);
    };
  }
  else {
    n_argomenti = __min(n_argomenti,n_arg_trovati);	//>000125 non serve più ?
    if (n_argomenti > 0) {

#ifdef ILDOS 
      arg_1 = pop_sv ();
      if (n_argomenti > 1) {
        arg_2 = arg_1;
        arg_1 = pop_sv (); 
        if (n_argomenti > 2) {
          arg_3 = arg_2;
          arg_2 = arg_1;
          arg_1 = pop_sv ();
          tipo_3 = contr_conv (&arg_3, descr_sf.descr_3);
        }
        tipo_2 = contr_conv (&arg_2, descr_sf.descr_2); 
      }
      tipo_1 = contr_conv (&arg_1, descr_sf.descr_1); 
#endif // ILDOS

#ifdef ILWIN
		for (i = n_argomenti-1; i >= 0; --i) {
			args[i] = pop_sv ();
			types[i] = contr_conv (&(args[i]), descr_sf.descrs[i]);
		};
#endif // ILWIN 
    };
  };
  if ((IS_PR_TOP) && (liv_procedura != in_liv_proc))
#ifdef ILDOS
  		err (4);
#endif // ILDOS
#ifdef ILWIN
  		err1 (4);
#endif // ILWIN
  if ((IS_PR_PROC) && (liv_procedura == in_liv_proc))
#ifdef ILDOS
  		err (3);
#endif // ILDOS
#ifdef ILWIN
  		err1 (3);
#endif // ILWIN
#ifdef ILDOS
  if (IS_PR_FOGLIO && (d_foglio == 0))
  		err (49);
#endif // ILDOS

/*>980107
	if (   ((IS_PR_TARTA)  && (QueryWindows (O_TARTA,  NULL) == 0))
	    || ((IS_PR_FOGLIO) && (QueryWindows (O_FOGLIO, NULL) == 0)))
*/
	if (   ((IS_PR_TARTA)  && (currentWindow (O_TARTA) == -1))
	    || ((IS_PR_FOGLIO) && (currentWindow (O_FOGLIO) == -1)))
/*<*/

		err1(47);
	
  if (! is_errore) {		/*controllo preventivo argomenti O.K.*/
	risultato = 0;
#ifdef ILDOS
	if ((IS_PR_TARTA) || ((dev_scrivi == TARTALFA) && (IS_PR_SCRIVI))) {
        if (_schermo == pc_COMANDI) {	/* entra in ambiente TARTA */
          locale = arg_1;
          arg_1 = pc_MISTO;
          ASSCHERMO ();
          arg_1 = locale;
        };
        (*descr_sf.corpo) ();			/* esecuzione primitiva */
	}
	else
#endif // ILDOS
//>020605
try
{
//<020605
		(*descr_sf.corpo) ();			/* esecuzione primitiva */
//>020605
}
catch( CUserException* e )
{
   // User already notified.
	e->Delete();
	abbassaSogliaNodi();
	errore (41, funzione, NULLP);
}
catch( CException* e )
{
    // For other exception types, notify user here.
	e->Delete();
    AfxMessageBox("Errore di sistema in esecuzione primitiva");
	errore (0, funzione, NULLP);
}
//<020605
#ifdef ILWIN
	if (IS_PR_TARTA) {
		TurtleStatusRefresh (i_tarta);
	};	
#endif // ILWIN
  };
  if (is_errore) is_funzione = FALSE;	/* per non porre risultato su stack */
  else if (is_funzione) push_sv (risultato);
  f_out ();
  if (is_locale) _locale ();
  if (is_vai) _vai ();
/*>971229
  if (is_pausa) _pausa ();
*/
  if ((is_pausa) && (n_arg_attesi == 0) && (conto_parentesi == 0))
  	_pausa ();
/*<*/
  if (is_continua) _continua ();
  if (is_recupera) _recupera ();
  if (is_acchiappa) _acchiappa ();
  if (is_lancia) _lancia ();
//>971205
//  tstbreak ();		/* testa eventuale richiesta di break */
//<971205
}

#ifdef ILWIN
node _funzione ()
{
	return funzione;
}
#endif // ILWIN

/*----------------------------------------------------------------
  inizia l' esecuzione di una procedura LOGO  (User Function CALL)
  con push di uno stack-frame
  ----------------------------------------------------------------*/
void uf_call ()
{
  int i;
  /* node */ int locale;
  get_uf (&lista_arg, &corpo, funzione);
  n_argomenti = lstlen (lista_arg);
                /*riconosce eventuale ricorsione di coda :*/
			/* e proc. da attivare coincide con proc.attiva */
  if ((stk_funzioni [liv_procedura] == funzione)

			/* e livello di procedura diverso da iniziale */
  && (liv_procedura != in_liv_proc)
			/* e chiamata si trova a livello 0 relativo */
  && (stk_livelli [liv_procedura] + 1 == liv_funzione)
			/*e la sua chiamata si trova a livello di blocco 0*/
  && (liv_esecuzione == 0)
                        /* e */
				/* o siamo a fine della procedura chiamante */
  && ((next_token == NULLP)
				/* o segue "stop" */
#ifndef NORMALIZE
       || (next_val == pc_STOP)
#else // NORMALIZE
       || MatchKw (next_val, pc_STOP)
#endif // NORMALIZE
     ))
  {
    				/* appoggia argomenti su stack di controllo */
    for (i = 1; i <= n_argomenti; ++i) push_sc (pop_sv ());
				/* annulla variabili locali vecchia attivaz.*/
    locale = n_arg_trovati;
    n_arg_trovati = 0;
    poploc (n_locali);
    n_arg_trovati = locale;
    n_locali = 0;
				/* attiva argomenti di nuova attivazione */
    for (i = 1; i <= n_argomenti; ++i) {
      putcar (cercav (car (lista_arg), LVAR), pop_sc ());
      lista_arg = cdr (lista_arg);
    };
    tr_comando ();		/* ATTENZIONE : usa oltre top di c_stack */
    is_funzione = FALSE;
    f_out ();			/* compensa f_in */
  }
  else if (liv_procedura == MAX_PROC-1)
#ifdef ILDOS
  		err (42);
#endif // ILDOS
#ifdef ILWIN
  		err1 (42);
#endif // ILWIN
  else {
    push_sc (ini_token);
    push_sc (token);		/* 1" elemento di STACK-FRAME */
    push_sc ((node) conto_esegui);	/******************************/
#ifdef REP_COUNT
	push_sc ((node) RepCount);
	push_sc ((node) RepTotal);
#endif // REP_COUNT
    push_sc ((node) val_verifica);	/* 2 */
    push_sc ((node) liv_esecuzione);	/* 3 */
    push_sc ((node) n_locali);		/* 4: salva conto esterno variabili locali */
    n_locali = 0;       	/* reinizializza conto variabili locali */
    push_sc ((node) n_argomenti);	/* 5: ultimo elemento di STACK-FRAME */
/*>990309
	push_sc ((node) p_sv);			// 6: post-ultimo elemento
<*/
                    /* binding di argomenti con salvataggio vecchio binding */
    for (i = 1; i <= n_argomenti; ++i) push_sc (pop_sv ()); /* uso temp. */
    for (i = 1; i <= n_argomenti; ++i) {
      pshloc (car (lista_arg), pop_sc ());
      lista_arg = cdr (lista_arg);
    };
    tr_comando ();		/* ATTENZIONE : usa oltre top di c_stack */
    stk_funzioni [++liv_procedura] = funzione;
    stk_livelli [liv_procedura] = liv_funzione;
  };
  err_token = ini_token =
  token = corpo; 
  is_stop = FALSE;
  liv_esecuzione = 0;
  n_arg_attesi = n_arg_trovati = 0;
  parentesi = -1;
  conto_parentesi = 0;
  conto_esegui = 0;
#ifdef REP_COUNT
	RepCount = 0;
	RepTotal = 0;
#endif REP_COUNT
  val_verifica = FALSE;
  funzione = NULLP;
}

/*------------------------------------------------------------
  attivazione di funzione di sistema (sfun) o di utente (ufun)
  ------------------------------------------------------------*/
void f_call ()
{
  if (typ (funzione) == UFUN) uf_call (); else /*SFUN*/ sf_call ();
}


void PR_CONSUMA()
{
}

/* ILCONTRO.C */
