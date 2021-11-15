/* ILSPAZIO.C		G. Toffoli

991218	spostato CREALIB e LIB in ilnodi.cpp
981123	in KwAttr fattorizzato KwAssoc (in ILPARLIS.CPP)
980828	preparazione per conversione a C++
980721	modificato KwAttr secondo vera sintassi di lista associativa
980717	aggiunto KwAttr per associare/reperie attributi di oggetti
		quali finestre e dispositivi di i/o, anche mediante "associazioni"
980205	ritoccato _sdef
980107	gestione del foglio corrente; modif. EDITA e EditAllProcs
971217	ritoccato CONSERVA
971210	supporto a editing di procedure nuove
971112	aggiunto IniProc, NextProc e EraseProcByName per supporto a edit/erase interattivo
971110	scorporato SaveToDev da CONSERVA
970618	eliminato mediazione di ILVIEW.C in chiamate a Editor
970603	ripristino provv, di dev. FOGLIO; a_capo dopo pc_FINE in _sdef 
970512	continuato implementazione di Editor
970503	continuato recupero update del 02.10.96 (normalizzazione)
970402	aggiunto EDITA
970315	tentato recupero update del 02.10.96 (normalizzazione)
		eliminato BIT_COMPILA (obsoleto, sost.da BIT_RISERVA)
960409	prima vers. Windows da Iperlogo del 24.04.89
*/	

#include "stdafx.h"

#define ILWIN
#define NORMALIZE

#define CMODULE

#include "ilcomune.cpp"
#include "ilfunz.h"

#ifdef NORMALIZE
node NormalKw (node);
int MatchKw (node, node);
#endif // NORMALIZE

#ifdef ILWIN

#include <sys\stat.h>
#include <io.h>

void EditorResetRead (int nview);	// 980831
int openDefaultEditor ();

#endif // ILWIN

/*---------------------------------------------------------------------------
riporta la lista di riferimenti alla parola arg_1 dagli oggetti individuati
da arg_2; se arg_2 = VAR o PROC si tratta dei valori di variabile o delle
definizioni di procedura, altrimenti del val. della proprieta' con quel nome
---------------------------------------------------------------------------*/
void LISTARIF ()
{
	node ele, parola;
	risultato = NULLP;
	ele = dizionario;
	do {
		parola = car (ele);
#ifndef NORMALIZE
		if (fogliadi (arg_1, ( (arg_2 == pc_VAR)  ? car (cercav (parola, LVAR))
			: ((arg_2 == pc_PROC) ? car (cercav (parola, UFUN))
#else // NORMALIZE
			if (fogliadi (arg_1, ( MatchKw (arg_2, pc_VAR)	? car (cercav (parola, LVAR))
				:  ( MatchKw (arg_2, pc_PROC) ? car (cercav (parola, UFUN))
#endif // NORMALIZE
				: _prop (parola, arg_2)
				)
				)
				)
				) {
				risultato = cons (parola, risultato, LIST);
				libera_1 ();
			};
	} while ((ele = cdr (ele)) != NULLP);
	risultato = listainv (risultato);
}

void NODI_ ()
{
	risultato = long_num (nodi);
}

/*-----------------------------------------------------------------------------
ritorna una lista contenente tutte le parole contenute nello spazio di lavoro
scrive l' indice di tutte le parole del ws con i tipi associati
---------------------------------------------------------------------------*/
void INDICE ()
{
	risultato = cons (car (dizionario), cdr (dizionario), LIST);
}

/*-------------------------------------------------------------------
ritorna il valore della proprieta' di nome specificato della parola
-------------------------------------------------------------------*/
node _prop (node parola, node nome)
// node parola, nome;
{
	node proprieta, el_p, nome_p, val_p;
	proprieta = NULLVALUE;
#ifdef NORMALIZE
	nome = NormalKw (nome); // se corrisponde a kw sostituisce con forma normale
#endif // NORMALIZE
	if ((parola != NULLP) && (o_l_p = cercav (parola, PLIST)) != NULLP) {
		el_p = lista_p = car (o_l_p);
		do {
			nome_p = car (ultima_p = el_p);
			val_p = car (el_p = cdr (el_p));
			el_p = cdr (el_p);
			if (nome_p == nome) proprieta = val_p;
		} while ((nome_p != nome) && (el_p != NULLP));
	};
	return proprieta;
}

/*----------------------------------------------------------------
primitiva per accedere al valore di una proprieta' di una parola
----------------------------------------------------------------*/
void PROP ()
{
	if ((risultato = _prop (arg_1, arg_2)) == NULLVALUE)
		risultato = NULLP;
}

/*--------------------------------------------------------------------
associa una proprieta', cioe' una coppia (nome, valore) a una parola
--------------------------------------------------------------------*/
void _asprop (node parola, node nome, node valore)
// node parola, nome, valore;
{
	node locale;
#ifdef NORMALIZE
	nome = NormalKw (nome); // se corrisponde a kw sostituisce con forma normale
#endif // NORMALIZE
	if (_prop (parola, nome) != NULLVALUE) {
		ultima_p = cdr (ultima_p);
		putcar (ultima_p, valore);
	}
	else {
		locale = cons (nome, cons (valore, NULLP, LIST), LIST);
		if (o_l_p == NULLP) o_l_p = mettio (parola, locale, PLIST);
		else putcdr (lastel (lista_p), locale);
	}
}

/*-------------------------------------------------------
primitiva MLOGO che associa una proprieta' a una parola
-------------------------------------------------------*/
void ASPROP ()
{
	_asprop (arg_1, arg_2, arg_3);
}

/*----------------------------------------------------------------
primitiva MLOGO che associa una lista di proprieta' a una parola
----------------------------------------------------------------*/
void ASLISTAPROP ()
{
	if ((o_l_p = cercav (arg_1, PLIST)) == NULLP) mettio (arg_1, arg_2, PLIST);
	else putcar (o_l_p, arg_2);
}

/*---------------------------------------------------------------------------
rimuove coppia (proprieta', valore) dalla lista di proprieta' di una parola
---------------------------------------------------------------------------*/
void _canceprop (node parola, node nome) 
// node parola, nome;
{
	if (_prop (parola, nome) != NULLVALUE) {
		lstdel (&lista_p, lstdel (&lista_p, ultima_p));
		if (lista_p == NULLP) canceo (o_l_p);
		else putcar (o_l_p, lista_p);
	}
}

/*---------------------------------------------------------------------------
rimuove coppia (proprieta', valore) dalla lista di proprieta' di una parola
---------------------------------------------------------------------------*/
void C_PRP ()
{
	_canceprop (arg_1, arg_2);
}

/*-----------------------------------------------------
ritorna la lista di proprieta' associata a una parola
-----------------------------------------------------*/
void LISTAPROP ()
{
	risultato = (o_l_p = cercav (arg_1, PLIST)) ? car (o_l_p) : NULLP;
}

/*--------------------------------------------------------
inizializza la ricerca degli oggetti di tipo specificato
in base a condizione relativa ad appartenenza a package
--------------------------------------------------------*/
void ini_sel (int tipo, node cond)
// int tipo;
// node cond;
{
	tipo_sel = tipo;
	cond_sel = cond;
	el_sel = cdr (dizionario);
}

/*----------------------------------------------------
ritorna il successivo oggetto di tipo specificato
che soddisfa la condizione di appartenenza a package
----------------------------------------------------*/
node nuovo_sel ()
{
	node valore, el_cond, pac;
	valore = NULLP;
	while ((el_sel != NULLP) && (valore == NULLP)) {
		if ((o_sel = cercat (&el_sel, tipo_sel)) != NULLP)				/* oggetto trovato ? */
			if (cond_sel == NULLP) {			/* tutto il work-space */
				if (   ((pac = _prop (o_sel, pc_PAC)) == NULLVALUE)
#ifndef NORMALIZE
					|| (_prop (pac, pc_COPERTO) != pc_VERO)    ) valore = o_sel;
#else // NORMALIZE
				|| (! MatchKw (_prop (pac, pc_COPERTO), pc_VERO)) ) valore = o_sel;
#endif // NORMALIZE
			}
			else if (typ (cond_sel) == LWORD) { /* appartiene AL package ? */
				if (_prop (o_sel, pc_PAC) == cond_sel) valore = o_sel;
			}
			else {				/* appartiene a UN package della lista ? */
				el_cond = cond_sel;
				while ((el_cond != NULLP) && (valore == NULLP)) {
					if (_prop (o_sel, pc_PAC) == car (el_cond)) valore = o_sel;
					el_cond = cdr (el_cond);
				} 
			}
	};
	return valore;
}

void IniProc () {
	ini_sel (UFUN, NULLP);
}

char* NextProc () {
	static char name [MAXCHARS];
	node object;
	object = nuovo_sel ();
	if (object == NULLP) return NULL;
	else {
		pacstr (name, nome(object));
		return &name[0];
	}
}

void EraseProcByName (char * name) {
	node parola;
	parola = metti_sc (name);
	cancet (parola, UFUN);
}

void ProcToDevByName (int dev_conserva, char* name) {
	int old_dev;
	node parola, oggetto;
	parola = metti_sc (name);
	oggetto = cercav (parola, UFUN);
	if (oggetto == NULLP) {
		return;
	}
	old_dev = dev_out;
	dev_out = dev_conserva;
	_sdef (oggetto);
	a_capo ();
	dev_out = old_dev;
}

/*--------------------------------------------------------------------------
inserisce un oggetto nel pacchetto specificato se non gia' in un pacchetto
o se il pacchetto a cui appartiene non e' coperto
--------------------------------------------------------------------------*/
void _pac (node oggetto, node pacchetto)
// node oggetto, pacchetto;
{
	if (! _coperto (oggetto)) _asprop (oggetto, pc_PAC, pacchetto);
}

/*--------------------------------------------------------------
ritorna TRUE se l' oggetto esiste gia' in un pacchetto coperto
--------------------------------------------------------------*/
int _coperto (node oggetto)
// node oggetto;
{
	node pacchetto;
	return (   ((pacchetto = _prop (oggetto, pc_PAC)) != NULLVALUE)
#ifndef NORMALIZE
		&& (_prop (pacchetto, pc_COPERTO) == pc_VERO)
#else // NORMALIZE
		&& MatchKw (_prop (pacchetto, pc_COPERTO), pc_VERO)
#endif // NORMALIZE
		);
}

/*-------------------------------------------------------------------------
primitiva che inserisce la/e parola/e specificate nel package specificato
-------------------------------------------------------------------------*/
void PAC ()
{
	if (tipo_2 == LWORD) _pac (arg_2, arg_1);
	else while (arg_2 != NULLP) {
		_pac (car (arg_2), arg_1);
		arg_2 = cdr (arg_2);
	}
}

/*------------------------------------------------------------------------
inserisce tutti gli oggetti del tipo specificato nel package specificato
------------------------------------------------------------------------*/
void _pactutto (int tipo)
// int 	tipo;
{
	node el, oggetto;
	el = cdr (dizionario);
	while (el != NULLP)
		if ((oggetto = cercat (&el, tipo)) != NULLP) _pac (oggetto, arg_1);
}

/*------------------------------------------------------------------------
primitiva che inserisce tutte le proc. e le var. nel package specificato
------------------------------------------------------------------------*/
void PACTUTTO ()
{
	_pactutto (UFUN);
	_pactutto (LVAR);
}

/*------------------------------------------------------------------------- 
primitiva che assegna la proprieta' [COPERTO VERO] al package specificato
-------------------------------------------------------------------------*/
void COPRI ()
{
#ifdef ILDOS
	_asprop (arg_1, pc_COPERTO, pc_VERO);
#endif // ILDOS
#ifdef ILWIN
	if (tipo_1 == LWORD) {			/* parola */
		_asprop (arg_1, pc_COPERTO, pc_VERO);
	}
	else while (arg_1 != NULLP) {			/*lista */
		_asprop (car (arg_1), pc_COPERTO, pc_VERO);
		arg_1 = cdr (arg_1);
	};
#endif // ILWIN
}

/*--------------------------------------------------------------------------
primitiva che assegna la proprieta' [COPERTO FALSO] al package specificato
--------------------------------------------------------------------------*/
void SCOPRI ()
{
#ifdef ILDOS
	_canceprop (arg_1, pc_COPERTO);
#endif // ILDOS
#ifdef ILWIN
	if (tipo_1 == LWORD) {			/* parola */
		_canceprop (arg_1, pc_COPERTO);
	}
	else while (arg_1 != NULLP) {			/*lista */
		_canceprop (car (arg_1), pc_COPERTO);
		arg_1 = cdr (arg_1);
	};
#endif // ILWIN
}

/*--------------------------------------------------------------
primitiva per la cancellazione di tutte le proc. e var. dal ws
--------------------------------------------------------------*/
void CANCETUTTO ()
{
#ifdef ILDOS
	conto_sel = 0;
	cance_cond (UFUN, arg_1);
	cance_cond (LVAR, arg_1);
#endif // ILDOS
#ifdef ILWIN
	node cond = (n_argomenti == 0) ? NULLP : arg_1;
	conto_sel = 0;
	cance_cond (UFUN, cond);
	cance_cond (LVAR, cond);
	cance_cond (PLIST, cond);
#endif ILWIN
	msg_cance ();
}

/*----------------------------------------------------------------------
primitiva che cancella dal ws le liste di propr. selezionate in base ad arg_1
----------------------------------------------------------------------*/
void C_PRP_IN ()
{
	conto_sel = 0;
#ifdef ILDOS
	cance_cond (PLIST, arg_1);
#endif // ILDOS
#ifdef ILWIN
	cance_cond (PLIST, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	msg_cance ();
}

/*----------------------------------------------------------------------
primitiva che cancella dal ws le procedure selezionate in base ad arg_1
----------------------------------------------------------------------*/
void C_PRO_IN ()
{
	conto_sel = 0;
#ifdef ILDOS
	cance_cond (UFUN, arg_1);
#endif // ILDOS
#ifdef ILWIN
	cance_cond (UFUN, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	msg_cance ();
}

/*----------------------------------------------------------------------
primitiva che cancella dal ws le variabili selezionate in base ad arg_1
----------------------------------------------------------------------*/
void C_VAR_IN ()
{
	conto_sel = 0;
#ifdef ILDOS
	cance_cond (LVAR, arg_1);
#endif // ILDOS
#ifdef ILWIN
	cance_cond (LVAR, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	msg_cance ();
}

/*--------------------------------------------------------------------------
elimina dal ws gli oggetti del tipo specificato selezionati in base a cond
--------------------------------------------------------------------------*/
void cance_cond (int tipo, node cond)
// int tipo;
// node cond;
{
	node oggetto, parola, pac, coperto;
	ini_sel (tipo, cond);
	while ((oggetto = nuovo_sel ()) != NULLP) {
		parola = richiudi (cdr (oggetto));
		pac = _prop (oggetto, pc_PAC);
		coperto = _prop (oggetto, pc_COPERTO);
		canceo (oggetto);
		if (pac != NULLVALUE) _asprop (parola, pc_PAC, pac);
		if (coperto != NULLVALUE) _asprop (parola, pc_COPERTO, coperto);
		++conto_sel;
	};
}

void msg_cance ()
{
	if ((conto_sel == 0) && (liv_procedura == in_liv_proc)) msg (15);
}

/*---------------------------------------------------------------
primitiva per la cancellazione di una o piu' procedure dal ws
a seconda che il parametro sia una parola o una lista di parole
---------------------------------------------------------------*/
void C_PRO ()
{
	node locale;
	if ((locale = cance_tipo (UFUN)) != NULLP)
#ifdef ILDOS
		err (2, locale);
#endif // ILDOS
#ifdef ILWIN
	err2 (2, locale);
#endif // ILWIN
}

/*---------------------------------------------------------------
primitiva che cancella una o piu' variabili specificate da arg_1
---------------------------------------------------------------*/
void C_VAR ()
{
	node locale;
	if ((locale = cance_tipo (LVAR)) != NULLP)
#ifdef ILDOS
		err (1, locale);
#endif // ILDOS
#ifdef ILWIN
		err2 (1, locale);
#endif // ILWIN
}

/*------------------------------------------------------------------------
cancella uno o piu' oggetti, del tipo specificato, specificati da arg_1;
riporta NULLP se tutto ok; altrimenti riporta la prima parola corrispon-
dente a procedura non definita / variabile senza valore
----------------------------------------------------------------------*/
node cance_tipo (int tipo)
// int tipo;
{
	node locale;
	if (tipo_1 == LWORD) {			/* parola */
		if (cancet (arg_1, tipo) == NULLP) return arg_1;
	}
	else while (arg_1 != NULLP) {			/*lista */
		if (cancet (locale = car (arg_1), tipo) == NULLP) return locale;
		arg_1 = cdr (arg_1);
	};
	return NULLP;
}

/*-------------------------------------------------------------------
primitiva che scrive le liste di proprieta' definite in tutto il ws
o nella parte di ws specificata da arg_1
-------------------------------------------------------------------*/
void SLISTEPROP ()
{
	node oggetto;
	ini_sel (PLIST, arg_1);
	while ((oggetto = nuovo_sel ()) != NULLP) {
		_scrivi (oggetto);			/* scrive nome */
		_prints (" : ", 3);
		_mostra (car (oggetto));		/* scrive lista */
		a_capo ();
	};
}

/*--------------------------------------------------------
scrive il titolo (linea di definizione) di una procedura
--------------------------------------------------------*/
void s_titolo (node oggetto)
// node oggetto;
{
	node lista_arg;
#ifdef ILDOS
	if ((oggetto != NULLP) && (! bit_tipo (oggetto, BIT_COMPILA))) {
#endif // ILDOS
		_scrivi (pc_PER);
		spazio ();
		_scrivi (oggetto);
		if (typ (oggetto) == UFUN) {	// aggiunto 971210
			lista_arg = car (car (oggetto));
			while (lista_arg != NULLP) {
				spazio ();
				_prints (":", 1);
				_scrivi (car (lista_arg));
				lista_arg = cdr (lista_arg);
			}
		}
		a_capo ();
#ifdef ILDOS
	}
#endif // ILDOS
}

/*-------------------------------------------------------------------------
primitiva che scrive titolo di procedure logo selezionate in base ad arg_1
-------------------------------------------------------------------------*/
void STITOLI ()
{
#ifdef ILDOS
	ini_sel (UFUN, arg_1);
#endif // ILDOS
#ifdef ILWIN
	ini_sel (UFUN, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	while ((oggetto = nuovo_sel ()) != NULLP) s_titolo (oggetto);
}

/*------------------------------------------------------------------------
primitiva che scrive le var., selezionate in base ad arg_1, e loro valori
------------------------------------------------------------------------*/
void SVAR ()
{
	node oggetto;
#ifdef ILDOS
	ini_sel (LVAR, arg_1);
#endif // ILDOS
#ifdef ILWIN
	ini_sel (LVAR, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	while ((oggetto = nuovo_sel ()) != NULLP) _svar (oggetto);
}

void _svar (node oggetto)
// node oggetto;
{
	node valore;
	if ((valore = car (oggetto)) != NULLVALUE) {
		_scrivi (pc_AS); spazio ();
		_prints ("\"", 1);
		is_letterale = TRUE;
		_scrivi (oggetto);
		spazio ();
		if (tipoalfa (valore)) _prints ("\"", 1);
		_mostra (valore);
		is_letterale = FALSE;
		a_capo ();
	};
}

/*--------------------------------------
scrive la definizione di una procedura
--------------------------------------*/
void _sdef (node oggetto)
// node oggetto;
{
#ifdef ILDOS
	if ((oggetto != NULLP) && (! bit_tipo (oggetto, BIT_COMPILA))) {
#endif // ILDOS
		is_letterale = TRUE;
		s_titolo (oggetto);
#ifdef ILWIN
		if (typ(oggetto) == UFUN)
#endif // ILWIN
			_scrivi (car (cdr (car (oggetto))));
//	a_capo ();	// eliminato 980205
		is_letterale = FALSE;
		_scrivi (pc_FINE);
		a_capo ();
		a_capo ();	// aggiunto 980205
#ifdef ILDOS
	}
#endif // ILDOS
}

/*------------------------------------------------------------------------
primitiva che scrive la definiz. di una o piu' proc. specificate da arg_1
------------------------------------------------------------------------*/
void SDEF ()
{
	if (tipo_1 == LWORD) _sdef (cercav (arg_1, UFUN));		/* parola */
	else while (arg_1 != NULLP) {					/* lista */
		_sdef (cercav (car (arg_1), UFUN));
		arg_1 = cdr (arg_1);
	}
}

/*------------------------------------------------------------------------
primitiva che scrive la definiz. delle proc. selezionate in base ad arg_1
------------------------------------------------------------------------*/
void SPROC ()
{
	node oggetto;
#ifdef ILDOS
	ini_sel (UFUN, arg_1);
#endif // ILDOS
#ifdef ILWIN
	ini_sel (UFUN, (n_argomenti == 0) ? NULLP : arg_1);
#endif // ILWIN
	while ((oggetto = nuovo_sel ()) != NULLP) _sdef (oggetto);
}

/*------------------------------------------------------
primitiva che scrive la definizione delle procedure
e il valoredelle variabili selezionate in base ad arg_1
------------------------------------------------------*/
void STUTTO ()
{
	SPROC ();
	SVAR ();
}

void SaveToDev (int dev_conserva)
{
	node oggetto, el_p, nome_p, val_p;
	dev_out = dev_conserva;
	is_letterale = TRUE;
	ini_sel (PLIST, arg_2);
	while ((oggetto = nuovo_sel ()) != NULLP) {
		el_p = car (oggetto);
		if (lstlen (el_p) > 2) {
			_scrivi (pc_ASLISTAPROP); spazio ();
			_prints ("\"", 1); _scrivi (oggetto); spazio ();
			_mostra (el_p); a_capo ();
		} else {
			while (el_p != NULLP) {
				nome_p = car (el_p);
				val_p = car (el_p = cdr (el_p));
				el_p = cdr (el_p);
				_scrivi (pc_ASPROP); spazio ();
				_prints ("\"", 1); _scrivi (oggetto); spazio ();
				_prints ("\"", 1); _scrivi (nome_p); spazio ();
				if (tipoalfa (val_p)) _prints ("\"", 1);
				_mostra (val_p);
				a_capo ();
			};
		};
	};
	ini_sel (UFUN, arg_2);
	while ((oggetto = nuovo_sel ()) != NULLP) {
		_sdef (oggetto);
		a_capo ();
	};
	ini_sel (LVAR, arg_2);
	while ((oggetto = nuovo_sel ()) != NULLP) _svar (oggetto);
	if (dev_conserva > 2) f_chiudi (dev_conserva);
	is_letterale = FALSE;
	dev_out = dev_scrivi;
}

/*-----------------------------------------------------
primitiva che salva sul file/device arg_1 l' intero ws
o la parte del ws specificata da arg2
-----------------------------------------------------*/
void CONSERVA ()
{
	int dev_conserva, locale;
#ifndef NORMALIZE
	if (arg_1 == pc_CONSOLE) dev_conserva = CONSOLE;
	else if (arg_1 == pc_STAMPANTE) dev_conserva = STAMPANTE;
	else if (arg_1 == pc_FOGLIO) dev_conserva = FOGLIO;
#else // NORMALIZE
	if (MatchKw (arg_1, pc_CONSOLE)) dev_conserva = CONSOLE;
	else if (MatchKw (arg_1, pc_STAMPANTE)) dev_conserva = STAMPANTE;
	/*> 971217
	else if (MatchKw (arg_1, pc_FOGLIO)) dev_conserva = FOGLIO;
	*/
	else if (((locale = nome2dev (arg_1)) != -1) && (_fstato [locale] & O_FOGLIO))
		dev_conserva = locale;
	/*>*/
#endif // NORMALIZE
#ifdef ILDOS
	else if ((locale = f_apri (arg_1, O_WRONLY | O_CREAT | O_TRUNC | O_RAW)) > 0)
#endif // ILDOS
#ifdef ILWIN
		else if ((locale = f_apri (arg_1, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_RAW, _S_IWRITE | _S_IREAD)) > 0)
#endif // ILWIN
		dev_conserva = locale;
	else {
#ifdef ILDOS
		if (locale == -1) err (31, arg_1);
#endif // ILDOS
#ifdef ILWIN
		if (locale == -1) err2 (31, arg_1);
#endif // ILWIN
#ifdef ILDOS
		else err (32);
#endif // ILDOS
#ifdef ILWIN
		else err1 (32);
#endif // ILWIN
		dev_conserva = CONSOLE;
	};
	
	/*> 
	node oggetto, el_p, nome_p, val_p;
	
	  dev_out = dev_conserva;
	  is_letterale = TRUE;
	  ini_sel (PLIST, arg_2);
	  while ((oggetto = nuovo_sel ()) != NULLP) {
	  el_p = car (oggetto);
	  if (lstlen (el_p) > 2) {
	  _scrivi (pc_ASLISTAPROP); spazio ();
	  _prints ("\"", 1); _scrivi (oggetto); spazio ();
	  _mostra (el_p); a_capo ();
	  } else {
	  while (el_p != NULLP) {
	  nome_p = car (el_p);
	  val_p = car (el_p = cdr (el_p));
	  el_p = cdr (el_p);
	  _scrivi (pc_ASPROP); spazio ();
	  _prints ("\"", 1); _scrivi (oggetto); spazio ();
	  _prints ("\"", 1); _scrivi (nome_p); spazio ();
	  if (tipoalfa (val_p)) _prints ("\"", 1);
	  _mostra (val_p);
	  a_capo ();
	  };
	  };
	  };
	  ini_sel (UFUN, arg_2);
	  while ((oggetto = nuovo_sel ()) != NULLP) {
	  _sdef (oggetto);
	  a_capo ();
	  };
	  ini_sel (LVAR, arg_2);
	  while ((oggetto = nuovo_sel ()) != NULLP) _svar (oggetto);
	  if (dev_conserva > 2) f_chiudi (dev_conserva);
	  is_letterale = FALSE;
	  dev_out = dev_scrivi;
	*/
	SaveToDev (dev_conserva);
	/*<*/
}

#ifdef ILWIN

extern int dev_editor;

void EDITA ()
{
	int save_dev = dev_out;
	node lword, lfun;
	//	dev_out = FOGLIO;
	//	EditorOpenView (pc_EDITA /*?*/, 0, 0, 30, 600, 500 /* , &(view[nv]->max_x), &(view[nv]->max_y) */, "EDITOR");
	//	n_editor = EditorOpenView (0, 30, 600, 500, "Logo Editor");
	/*>980107
	dev_editor = apri_foglio (pc_FOGLIO);
	*/
	if (dev_editor == -1)
		dev_editor = openDefaultEditor ();
	/*<*/
	dev_out = dev_editor;
	//	if (n_argomenti > 0)
	//		SDEF ();
	if (tipo_1 == LWORD) {						/* parola */
		//		_sdef (cercav (arg_1, UFUN));
		lfun = (cercav (arg_1, UFUN));
		_sdef ((lfun == NULLP) ? arg_1 : lfun);
	}
	else while (arg_1 != NULLP) {				/* lista */
		//		_sdef (cercav (car (arg_1), UFUN));
		lword = car (arg_1);
		lfun = (cercav (lword, UFUN));
		_sdef ((lfun == NULLP) ? lword : lfun);
		arg_1 = cdr (arg_1);
	}
	dev_out = save_dev;
}

void EditAllProcs ()
{
	node oggetto;
	int save_dev = dev_out;
	/*>980107
	dev_editor = apri_foglio (pc_FOGLIO);
	*/
	if (dev_editor == -1)
		dev_editor = openDefaultEditor ();
	/*<*/
	dev_out = dev_editor;
	ini_sel (UFUN, NULLP);
	while ((oggetto = nuovo_sel ()) != NULLP) _sdef (oggetto);
	dev_out = save_dev;
}

void FINE_EDITA ()
{
	dev_temp = FOGLIO;
	is_recupera = TRUE;
	EditorResetRead (_file [dev_editor]);
	//	EditorCloseView (_file [dev_editor]);
}

#endif // ILWIN

// data la lista tail (tipicamente argomento in apertura oggetto/dispositivo)
// riporta l'eventuale valore associato al primo elemento (nome oggetto)
// per mezzo di una associazione (lista associativa) o proprieta' il cui
// nome e' specificato da kWord;
// nel reperire tale valore, il nome dell'oggetto viene considerato
// sia cosi' come e', sia normalizzato (tutto minuscolo o maiuscolo)
// NOTA: _prop gia' gestisce internamente tale equivalenza
node KwAssoc (node tail, node kWord);
node KwAttr (node lName, node tail, node kWord)
{
	node valProp;
//	node item;
//> 981123
//	while (tail != NULLP) {
//		if (   (typ (item = car (tail)) == LIST)
//			&& (MatchKw (car (item), kWord)))
//			return cdr (item);
//		tail = cdr (tail);
//	};
	node valAssoc = KwAssoc (tail, kWord);
	if (valAssoc != NULLP)
			return cdr (valAssoc);
	else
//< 981123
		if ((valProp = _prop (lName, kWord)) != NULLVALUE)
		return valProp;
	else
		return NULLP;
}

/* ILSPAZIO.C */
