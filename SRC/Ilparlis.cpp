/*ILPARLIS.C	. Toffoli

020605	libera_2 () al posto di TEST_LIBERA_..
010213	versione generalizzata di ORDINA (non protetta da memory underflow !)
000301	corretto ORDINA per poter orinare lista mista di parole e stringhe
991213	esteso STRINGA ad argomento lista
991209	esteso SETITEM (ASELE) a liste
991102	aggiunto PR_DUPLICATE (copia di oggetti con ricorsione sulle liste)
991015	in ASSOC e SETASSOC abolito condiz.che lista associat. contenga solo liste
990924	aggiunto PR_SETASSOC (ASSOCIA) 
990714	corretto (?) implementazione di PR_QUOTED (LETTERALE) 
990325	reimplementato sorted_insert
		completato MPRI (INPRI) e MULT (INULT) per parole/stringhe
990324	sost. libera_1 con macro TEST_LIBERA(128)
990323	protezione ris.parziali in ORDINA e sorted_insert
981208	aggiunto rassoc2
980924	aggiunto rassoc
980923	aggiunto KwAssoc
980915	aggiunto RemoveCell, RemoveEqual e RemoveDuplicate;
		attivato PR_REMDUP
980914	aggiunto CatString e attivato PR_COMBINE
980911	aggiunto PosEle e iniziato PR_REMOVE
980908	aggiunto PR_LASTS e PR_BUTLASTS (dummy)
980828	preparazione per conversione a C++
980728	aggiunto primitive PAROLAVUOTA e LISTAVUOTA (EMPTYWORD e EMPTYLIST)
980525	introdotto AssocWord (funziona come ASSOC, anziche' _assoc)
980525 - corretto PRI (ARAY)
970331 - reimplementato funzione REVERSE
970207 - reintrodotto (dummy) funzione REVERSE da versione 961002
960503 - introdotto tipo ARAY (= array)
960409 - prima vers. Windows da Iperlogo del 18.04.89
modulo che implementa le primitive MLOGO operanti su parole e liste
*/

#include "stdafx.h"

#define ILWIN

#define CMODULE
#include "ilcomune.cpp"
#include "ilsfun.h"
#include "ilfunz.h"

#define TEST_LIBERA_NULLP(n) \
	if(nodi<n){LIBERA();if(nodi<n){err1(41);return NULLP;}}
#define TEST_LIBERA_VOID(n) \
	if(nodi<n){LIBERA();if(nodi<n){err1(41);return;}}

int MatchKw (node parola, node kw);
void RemoveEle (node *lista, node ele);
void MemScrivi (char *s, node oggetto);

#ifdef ILWIN

void _ARRAY ()
{
	int i, size = rnum_int (arg_1);
	node cell = NULLP;
	for (i = size; i >= 1; i--) {
		cell = cons (NULLP, cell, LIST);
	}
	if (n_argomenti == 1) arg_2 = consreal ((double) 1);	 
	risultato = cons (cons (arg_1, cons (arg_2, NULLP, LIST), LIST), cell, ARAY); 
} 

void _setitem (node tail, int i, node val)
{
	while (--i > 0) tail = cdr (tail);
	putcar (tail, val);
}

// ancora da implementare per parole/stringhe
void SETITEM ()
{
	int i = rnum_int (arg_1);
	int size;
	if (typ (arg_2) == LIST) {
		size = lstlen (arg_2);
		if (i > size) err_pochi (arg_2);
		else _setitem (arg_2, i, arg_3);
	}
	else if (typ (arg_2) == ARAY) {

		size = num_int (car (car (arg_2)));
		node cell = cdr (arg_2);
		i = i - num_int (car (cdr (car (arg_2)))) + 1;
		if (i > size) err_pochi (arg_2);
		else {
			while (--i > 0) cell = cdr (cell);
			putcar (cell, arg_3);
		}
	}
}

void ARRAYP ()
{
	risultato = logval (tipo_1 == ARAY);
}

void PR_FIRSTS ()
{
	node tail, sublist, el;
	risultato = NULLP;
	if (arg_1 != NULLP) {
		tail = arg_1;
		if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST)) { 
			risultato = el = cons (car (sublist), NULLP, LIST);
			while ((tail = cdr (tail)) != NULLP) {
				if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST))
					el = putcdr (el, cons (car (sublist), NULLP, LIST)); 
				else { err_arg1 (); return; }
			}
		} else err_arg1 ();
	}
}

void PR_BUTFIRSTS ()
{
	node tail, sublist, el;
	risultato = NULLP;
	if (arg_1 != NULLP) {
		tail = arg_1;
		if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST)) { 
			risultato = el = cons (cdr (sublist), NULLP, LIST);
			while ((tail = cdr (tail)) != NULLP) {
				if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST))
					el = putcdr (el, cons (cdr (sublist), NULLP, LIST)); 
				else { err_arg1 (); return; }
			}
		} else err_arg1 ();
	}
}

#endif // ILWIN

void err_pochi (node arg)
{
#ifdef ILDOS
  err (7, arg);
#endif // ILDOS
#ifdef ILWIN
  err2 (7, arg);
#endif // ILWIN
}

/*-----------------------------------------------------------------
  ritorna una lista contenente in ordine gli n elementi sul v_stack
  -----------------------------------------------------------------*/
node _lista (int n)
{
  node lista;
  lista = NULLP;
  while (n-- != 0) lista = cons (pop_sv (), lista, LIST);
  return lista;
}

/*---------------------------------------------------------------------
  ritorna come valore il numero di elementi o caratteri dell' argomento
  ---------------------------------------------------------------------*/
int _conto (node arg)
{
  return (typ (arg) == LIST) ?  lstlen (arg) :		/* lista */
  				wrdlen (nome (arg));	/* parola o stringa */
} 
 
/*-------------------------------------------------------------------
  ritorna  un intero pari al numero di elementi o caratteri di  arg_1
  -------------------------------------------------------------------*/
/*-------------------------------------------------------------------
  ritorna in risultato il primo elemento di una lista o di una parola
  -------------------------------------------------------------------*/
node _pri (node arg)
{
  int tipo;
  node s_da;
  if (arg == NULLP) err_arg1 ();				/* lista vuota */
  else {
    if ((tipo = typ (arg)) == LIST) return car (arg);		/* lista */
    else {						/* parola o stringa */
      if ((s_da = nome (arg)) == S_VUOTA) err_arg1 ();
      else {
        iwtstr ();
        irdstr (s_da);
        return str_par (wtstr ((char) rdstr ()), tipo);
      }
    }
  }
	return NULL;	// 980828: per motivi sintattici
}
 
/*-------------------------------------------------------
  ritorna l' ultimo elemento di una lista o di una parola
  -------------------------------------------------------*/
node _ult (node arg)
{
  int tipo, c, c_temp;
  node s_da;
  if (arg == NULLP) err_arg1 ();				/* lista vuota */
  else {
    if ((tipo = typ (arg)) == LIST) return car (lastel (arg));/* lista */
    else {						/* parola o stringa */
      if ((s_da = nome (arg)) == S_VUOTA) err_arg1 ();
      else {
        irdstr (s_da);
        while ((c_temp = rdstr ()) != EOS) c = c_temp;
        iwtstr ();
        return str_par (wtstr ((char) c), tipo);
      }
    }
  }
	return NULL;	// 980828: per motivi sintattici
} 
 
/*--------------------------------------------------------------------------
  crea una nuova lista = arg_2 con in piu' arg_1 appeso come ultimo elemento
  --------------------------------------------------------------------------*/
node _mult (node arg1, node arg2)
{
  F = arg1; libera_n (lstlen (T = arg2));
  return lstcat (lstdup (arg2), cons (arg1, NULLP, LIST));
} 

/*-----------------------------------------------------------------------
  crea una nuova stringa dalla stringa data escludendo il primo carattere
  o crea una nuova parola dalla parola data escludendo il primo carattere
  o ritorna una sottolista della lista data escludendo il primo elemento
  -----------------------------------------------------------------------*/
node _menpri (node arg)
{
  int tipo, c;
  if (arg == NULLP) err_arg1 ();				/* lista vuota */
  else {
    if ((tipo = typ (arg)) == LIST) return cdr (arg);	/* lista */
    else { 						/* parola  o stringa */
      if ((A = nome (arg)) == S_VUOTA) err_arg1 ();
      else {
        irdstr (A); rdstr ();
        R = iwtstr ();
        while ((c = rdstr ()) != EOS) {
//        libera_1 ();
//>020605
//		  TEST_LIBERA_NULLP(128)
          libera_2 ();
//<020605
          R = wtstr ((char) c);
        };
        return str_par (R, tipo);
      }
    }
  }
	return NULL;	// 980828: per motivi sintattici
} 

node _menult (node arg)
{
  int   tipo, c, c_temp;
  node temp;
  if (arg == NULLP) err_arg1 ();
  else {
    if ((tipo = typ (arg)) == LIST) {			/* lista */
      libera_n (lstlen (A = arg));
      temp = lstdup (arg);
      lstdel (&temp, lastel (temp));
      return temp;
    }
    else {						/* parola o stringa */
      if ((A = nome (arg)) == S_VUOTA) err_arg1 ();
      else {
        R = iwtstr ();
        irdstr (A);
        c = rdstr ();
        while ((c_temp = rdstr ()) != EOS) {
//        libera_1 ();
//>020605
//		  TEST_LIBERA_NULLP(128)
          libera_2 ();
//<020605
          R = wtstr ((char) c);
          c = c_temp;
        };
        return str_par (R, tipo);
      }
    }
  }
	return NULL;	// 980828: per motivi sintattici
} 

/*-----------------------------------------------------------------------
  ritorna un oggetto contenente gli elementi dell' oggetto dato nel range
  da i a f inclusi; se l' oggetto e' una lista ritorna una lista,
  altrimenti ritorna una stringa
  -----------------------------------------------------------------------*/
node _pezzo (int i, int f, node oggetto)
{
  int j, c;
  R = j = 0;
  if (typ (A = oggetto) == LIST) {
    while ((++j <= f) && (A != NULLP)) {
      if (j >= i) R = _mult (car (A), R);
      A = cdr (A);
    };
    if (j <= f) R = NULLP;
  }
  else {
    irdstr (A = nome (oggetto));
    R = iwtstr ();
    while ((++j <= f) && ((c = rdstr ()) != EOS))
      if (j >= i) {
//      libera_1 ();
//>020605
//		TEST_LIBERA_NULLP(128)
        libera_2 ();
//<020605
        R = wtstr ((char) c);
      };
    if (j <= f) R = S_VUOTA;
  };
  return R;
}

/*-----------------------------------------------------------
  ritorna l' elemento di lista o il carattere di indice arg_1
  della lista o parola arg_2
  -----------------------------------------------------------*/
node _ele (int i_ele, node arg)
{
  int tipo, i, c;
  node s_da, s_a;
  if (i_ele == 0) err_arg1 ();
  else {
    if ((tipo = typ (arg)) == LIST)			/* lista */
      if (i_ele > lstlen (arg)) err_pochi (arg);
      else {
        for (i = 2; i <= i_ele; i++) arg = cdr (arg);
        return car (arg);
      }
    else {						/* parola o stringa */
      if (i_ele > wrdlen (s_da = nome (arg))) err_pochi (arg);
      else {
        s_a = iwtstr ();
        irdstr (s_da);
        for (i = 1; i <= i_ele; i++) c = rdstr ();
        s_a = wtstr ((char) c);
        return str_par (s_a, tipo);
      }
    }
  }
	return NULL;	// 980828: per motivi sintattici
} 
 
node str_par (node stringa, int tipo)
{
  return (tipo == LSTR) ? stringa :				/* stringa */
                          mettip (stringa);		/* parola */
}

/*-----------------------------------------------
  ritorna l' oggetto non lista contenuto in resto
  o la successiva parola del resto (di lista)
  -----------------------------------------------*/
node _parola ()
{
  node locale;
  if ((locale = resto_pn) != NULLP)
    if (typ (resto_pn) != LIST)
      resto_pn = NULLP;
    else {
      locale = car (resto_pn);
      resto_pn = cdr (resto_pn);
    };
  return locale;
}

/*------------------------------------------------------------------
  inserisce un NUOVO elemento in una LISTA ordinata, prima del primo
  ELEmento per cui PRED (NUOVO ELE) e' vero
  -----------------------------------------------------------------*/
// reimplementata in modo piu' efficiente :
// modifica lista corrente, anziche' creare lista nuova
node sorted_insert (node nuovo, node lista, int (*pred) (node term1, node term2))
{
/*>990325
	node ele, prima = NULLP;
	while (lista != NULLP) {
	if ((*pred) (nuovo, (ele = car (lista))) < 0) break;
		TEST_LIBERA_NULLP(128)	// 990323
		S =						// 990323
		prima = cons (ele, prima, LIST);
		lista = cdr (lista);
	};
	return lstcat (listainv (prima), cons (nuovo, lista, LIST));
*/
	node cella = cons (nuovo, NULLP, LIST);
	if (lista == NULLP)
		return cella;
	S = lista;
	node elemento, precedente = NULLP, corrente = lista;
	while (corrente != NULLP) {
		if ((*pred) (nuovo, (elemento = car (corrente))) < 0) {
			putcdr (cella, corrente);
			if (precedente == NULLP)
				lista = cella;
			else
				putcdr (precedente, cella);
			return lista;
		}
//>020605
//		TEST_LIBERA_NULLP(128)
        libera_2 ();
//<020605
		precedente = corrente;
		corrente = cdr (corrente);
	};
	putcdr (precedente, cella);
	return lista;
/*<*/
}

/*-------------------------------------------------
  risultato intero <- codice ASCII del 1" carattere
  dell' argomento (parola o stringa)
  -------------------------------------------------*/
void ASCII ()
{
  int c;
  irdstr (nome (arg_1));
  if ((c = rdstr ()) == EOS) err_arg1 ();
  else risultato = int_num (c);
}

#ifdef ILWIN

// riporta lista con l'associazione nome-val aggiunta in testa
node rassoc (node nome, node val, node lista)
{
	node sottolista = cons (nome, cons (val, NULLP, LIST), LIST);
	return cons (sottolista, lista, LIST);
}

// come rassoc, ma con associazione a 2 valoi
node rassoc2 (node nome, node val1, node val2, node lista)
{
	node sottolista =
		cons (nome, cons (val1, cons (val2, NULLP, LIST), LIST), LIST);
	return cons (sottolista, lista, LIST);
}

/*-------------------------------------------------------------
  Riporta la sottolista di tail che ha come primo elemento name
  -------------------------------------------------------------*/
node _assoc (node name, node tail)
{
  node temp;
  while (tail != NULLP) {
    temp = car (tail);
	if (car (temp) == name) {
      return temp;
    };
    tail = cdr (tail);
  };
  return tail;
}

/*---------------------------------------------------------------
  Riporta la sottolista  di tail che ha come primo elemento
  la parola kWord, trattata come parola chiave (case insensitive)
  (vedi anche KwAttr in ILSPAZIO.CPP)
  ---------------------------------------------------------------*/
node KwAssoc (node tail, node kWord)
{
	node item;
	while (tail != NULLP) {
		if (   (typ (item = car (tail)) == LIST)
			&& (MatchKw (car (item), kWord)))
			return item;
		tail = cdr (tail);
	};
	return NULLP;
}

/*-----------------------------------------------------------------------
  Riporta la sottolista (NO! la porzione) di tail
  che ha come primo elemento la parola name
  -----------------------------------------------------------------------*/
node AssocWord (node name, node tail)
{
	while (tail != NULLP) {
		if (car (tail) == name)
			return tail;
		tail = cdr (tail);
	};
	return NULLP;
}

#endif // ILWIN

#ifdef OBSOLETO
/*---------------------------------------------------------------------------
  Riporta la sottolista di arg_2 che ha come primo elemento arg_1
  ---------------------------------------------------------------------------*/
void ASSOC ()
{
	node lista, temp;
	risultato = temp = NULLP;
	lista = arg_2;
	while (lista != NULLP) {
		if (typ (temp = car (lista)) != LIST) {
			err_arg2 (); return ;
		}
		else if (car (temp) == arg_1) {
			risultato = temp; return ;
		};
		lista = cdr (lista);
	};
}
#endif // OBSOLETO

/*----------------------------------------------------------------------
2 argomenti:
  Riporta la sottolista di arg_2 che ha come primo elemento arg_1
3 argomenti:
  Riporta la sottolista di arg_2 che ha come arg_3-esimo elemento arg_1;
  assomiglia a una generalizzazione di RASSOC (reverse assoc)
  ----------------------------------------------------------------------*/
void ASSOC ()
{
	risultato = NULLP;
	int iElemento;
	node tail = arg_2;
	if (n_argomenti > 2) {
		iElemento = rnum_int (arg_3);
		if (iElemento <= 0) {
			errore (5, funzione, arg_3); return;
		}
	}
	node assoc;
	while (tail != NULLP) {
		assoc = car (tail);
//>991015
//		if (typ (assoc) != LIST)
		if (assoc == NULLP)
			{ err_arg2 (); return; }
		if (typ (assoc) == LIST) {
//<
			if (n_argomenti == 2) {
				if (ug_atomo (car (assoc), arg_1)) {
					risultato = assoc;
					return ;
				};
			}
			else {	// n_argomenti == 3
				if (_conto (assoc) < iElemento) {
					err_arg2 (); return ;
				}
				if (ug_atomo (_ele (iElemento, assoc), arg_1)) {
					risultato = assoc;
					return;
				}
			}
//>991015
		}
//<
		tail = cdr (tail);
	};
}

node ConsNull (node car)
{
	return cons (car, NULLP, LIST);
}

// In Logo il CONS del Lisp esiste solo come struttura interna, non è un oggetto.
node DottedPair (node key, node val)
{
	if (typ (val) != LIST)
		val = ConsNull (val);
	return cons (key, val, LIST);
}

// ASASSOC KEY VAL LISTASSOC
// Funzione che, in lista associativa, sostituisce associazione per la chiave data,
// oppure la appende se non c'è già:
// - se VAL è una lista, diventa sottolista dell'associazione (coda)
// - se VAL non è una lista,viene creata una cella CONS con VAL come CAR
// ATTENZIONE: la lista, se già esistente, viene MODIFICATA
void PR_SETASSOC ()
{
	node newAssoc = DottedPair (arg_1, arg_2);
	if (arg_3 == NULLP) {
		risultato = ConsNull (newAssoc);
		return;
	}
	node previousCell, currAssoc;
	node tail = arg_3;
	while (tail != NULLP) {
		currAssoc = car (tail);
//>991015
//		if (typ (currAssoc) != LIST)
		if (currAssoc == NULLP)
//<
			{ errore (5, funzione, arg_3); return; }
//>991015
//		else
//		if (car (currAssoc) == arg_1)
		if ((typ (currAssoc) == LIST) && (car (currAssoc) == arg_1))
//<
			break;
		previousCell = tail;
		tail = cdr (tail);
	}
	if (tail != NULLP)
		putcar (tail, newAssoc);
	else
		putcdr (previousCell, ConsNull (newAssoc));
	risultato = arg_3;
}

/*---------------------------------------------------------------------------
  risultato stringa <- carattere il cui codice ASCII e' l' argomento (intero);
  se carattere coincide con delimitatore il risultato viene promosso a parola
  ---------------------------------------------------------------------------*/
void CARATTERE ()
{
  int locale;
  iwtstr ();
  risultato = wtstr ((char) (locale = rnum_int (arg_1)));
  if (IS_DELIMITATORE(locale)) risultato = mettip (risultato);
}

void PRI ()
{ 

#ifdef ILWIN
	if (typ (arg_1) == ARAY)
//>980525
//		risultato = car (cdr (car (arg_1)));
		risultato = car (cdr (arg_1));
//<
	else
#endif // ILWIN
		risultato = _pri (arg_1);
} 

void ULT ()
{
  risultato = _ult (arg_1);
} 
 
void MENPRI ()
{
  risultato = _menpri (arg_1);
} 
 
void MENULT ()
{
  risultato = _menult (arg_1);
}
 
/*------------------------------------------------------
  la nuova lista contiene come sua parte il 2" argomento
  ------------------------------------------------------*/
void MPRI ()
{
	if (typ (arg_2) == LIST)
		risultato = cons (arg_1, arg_2, LIST);
	else {
		if (typ (arg_1) == LWORD) {
			push_sv (arg_1); push_sv (arg_2);
			PAROLA ();
		}
		else if (typ (arg_1) == LSTR) {
			push_sv (arg_1); push_sv (arg_2);
			STRINGA ();
		}
		else err_arg1 ();
	}
} 
 
/*-------------------------------------------
  la nuova lista viene ricreata completamente
  -------------------------------------------*/
void MULT ()
{
	if (typ (arg_2) == LIST)
		risultato = _mult (arg_1, arg_2);
	else {
		if (typ (arg_1) == LWORD) {
			push_sv (arg_2); push_sv (arg_1);
			PAROLA ();
		}
		else if (typ (arg_1) == LSTR) {
			push_sv (arg_2); push_sv (arg_1);
			STRINGA ();
		}
		else err_arg1 ();
	}
} 
 
/*-------------------------------------------------------------------------
  crea una nuova lista avente come elementi gli argomenti di tipo non-lista
  e gli elementi degli argomenti di tipo lista
  -------------------------------------------------------------------------*/
void FRASE ()
{
	int   i;
	R = NULLP;
	for (i = 1; i <= n_argomenti; i++) { /*per ogni argomento ...*/
		if ((A = pop_sv ()) != NULLP) {
			if (typ (A) == LIST) {		/* arg_1 = lista, e' concatenato */
				libera_n (lstlen (A));
				R = lstcat (lstdup (A), R);
			}
			else {				/* arg_1 <> lista, e' inserito */
//				libera_1 ();
//>020605
//				TEST_LIBERA_VOID(128)
				libera_2 ();
//<020605
				R = cons (A, R, LIST);
			}
		}
	}
	risultato = R;
} 
 
/*---------------------------------------------------------------------
  crea dall' inizio una nuova lista avente come elementi gli argomenti
  ---------------------------------------------------------------------*/
void LISTA ()
{
  risultato = _lista (n_argomenti);
} 
 
void CONTA ()
{
  risultato = int_num (_conto (arg_1));
}
 
void ELE ()
{
#ifdef ILDOS
	risultato = _ele (rnum_int (arg_1), arg_2);
#endif // ILDOS

#ifdef ILWIN
	int	size, i = rnum_int (arg_1);
	node tail = arg_2;
	if (typ (arg_2) == ARAY) {
		size = num_int (car (car (arg_2)));
		i = i - num_int (car (cdr (car (arg_2)))) + 1;
		if (i > size) { err_pochi (arg_2); return; }
		tail = cdr (arg_2);
	}
	risultato = _ele (i, tail);
#endif // ILWIN
}



/*--------------------------------------------------------------
  crea una nuova stringa concatenando 1 o piu' parole o stringhe
  --------------------------------------------------------------*/
void STRINGA ()
{
	int   i, c;
	node arg;
	int tipo;
	R = iwtstr ();
	for (i = n_argomenti; i >= 1; i--) {
		arg = get_sv (i);
		tipo = typ (arg);
		if ((tipo == LWORD) || (tipo == LSTR) || (tipo == LBCD)) {
			irdstr (nome (arg));
			while ((c = rdstr ()) != EOS) {
//				libera_1 ();
//>020605
//				TEST_LIBERA_VOID(128)
				libera_2 ();
//<020605
				R = wtstr ((char) c);
			};
		}
		else {
			char text[64*K];
			char* pText = &text[0];
			MemScrivi (text, arg);
			while ((c = *(pText++)) != 0) {
//>020605
//				TEST_LIBERA_VOID(128)
				libera_2 ();
//<020605
				R = wtstr ((char) c);
			};
		}
	};
	for (i = 1; i <= n_argomenti; i++) pop_sv ();
	risultato = R;
}
 
/*----------------------------------------------------------------------
  crea una nuova parola concatenando 1 o piu' parole o stringhe
  e filtrando via i caratteri non appartenenti alla classe P (in parola)
  ----------------------------------------------------------------------*/
void PAROLA ()
{
  STRINGA ();
  risultato = mettip (risultato);
}

/*-------
  PAROLA?
  -------*/
void PAROLA_ ()
{
  risultato = logval ((tipoalfa (arg_1)) || (tipo_1 == LBCD));
}
 
/*------
  LISTA?
  ------*/
void LISTA_ ()
{
  risultato = logval (tipo_1 == LIST);
}
 
/*------
  VUOTO?
  ------*/
void VUOTO ()
{
  if (arg_1 == NULLP) risultato = pc_VERO;		/* lista vuota */
  else if (tipo_1 == LIST) risultato = pc_FALSO;	/* lista non vuota */
  else risultato = logval (nome (arg_1) == S_VUOTA);	/* parola */
}
 
void PEZZO ()
{
  int i, f;
  if ((i = rnum_int (arg_1)) < 1) err_arg1 ();
  else if ((f = rnum_int (arg_2)) < i) err_arg2 ();
  else if (   ((risultato = _pezzo (i, f, arg_3)) == NULLP)
           || (risultato == S_VUOTA))
#ifdef ILDOS
    err (7, arg_3);
#endif // ILDOS
#ifdef ILWIN
    err2 (7, arg_3);
#endif // ILWIN
}

// riporta la cella di posizione specificata di una lista,
// cioe' non un elemento, ma una sottolista
node iListCell (int pos, node lista)
{
	int i;
	for (i = 1; i < pos; i++)
		lista = cdr (lista);
	return lista;
}

// modifica una lista, eliminando la cella di posizione specificata,
// che deve esistere
void RemoveCell (node *lista, int pos)
{
	int i;
	node tail = *lista;
	if (pos == 1)
		*lista = cdr (tail);
	else {
		for (i = 2; i < pos ; i++)
			tail = cdr (tail);
		putcdr (tail, cdr (cdr (tail)));
	}
}

/*----------------------------------------------------------------
  riporta eventuale posizione di elemento di arg2 isomorfo ad arg1,
  oppure 0
  ----------------------------------------------------------------*/
int PosEle (node arg1, node arg2)
{
	node elemento = arg1, insieme = arg2;
	int uguale = 0;
	node pezzo;
	int lung, pos;
	pos = 1;
	if (typ (insieme) == LIST) {
		while (   (insieme != NULLP)
			   && (! (uguale = _uguale (elemento, car (insieme))))) {
			insieme = cdr (insieme);
			++pos;
		}
	}
	else if (typ (insieme) != LIST) {		/* parola (compreso numero) */
		lung = wrdlen (elemento = nome (elemento));
		insieme = nome (insieme);
		pos = 1;
		while (   ((pezzo = _pezzo (pos, pos+lung-1, insieme)) != S_VUOTA)
			   && (! (uguale = _uguale (elemento, pezzo)))
          ) ++pos;
	};
	return
		(uguale) ? pos : 0;
}

// riporta una lista (nuova se necessario) in cui sono rimossi
// tutti gli elementi della lista originaria isomorfi a ele
node RemoveEqual (node ele, node lista) {
	int finito = FALSE;
	while (! finito) {
		int pos = PosEle (ele, lista);
		if (pos > 0) {
			lista = lstdup (lista);
			RemoveCell (&lista, pos);
		}
		else
			finito = TRUE;
	}
	return lista;
}

// riporta una lista (nuova se necessario) in cui sono rimossi
// tutti gli elementi duplicati (isomorfi) nella lista originaria;
// lascia solo primo elemento in ogni classe di equivalenza
node RemoveDuplicate (node lista)
{
	node ele, tail;
	int nEle, iEle = 1, i, pos;
	while (((nEle = lstlen (lista)) > 1) && (iEle < nEle)) {
		ele = car (lista);
		tail = cdr (lista);
		for (i=1; i<iEle; i++) {
			ele = car (tail);
			tail = cdr (tail);
		}
		pos = PosEle (ele, tail);
		if (pos > 0) {
			lista = lstdup (lista);
			RemoveCell (&lista, iEle+pos);
		}
		else
			++iEle;
	}
	return lista;
}

/*---------------------------------------------------------
  ELE? (riporta VERO se arg_1 e' elemento o parte di arg_2)
  ---------------------------------------------------------*/
void ELE_ ()
{
  node arg1, arg2, pezzo;
  int l1,j;
  j = 1;
  if (typ (arg_2) == LIST) {
    arg2 = arg_2;
    while ((arg2 != NULLP) && (! (risultato = _uguale (arg_1, car (arg2))))) {
      arg2 = cdr (arg2);
      ++j;
    }
  }
  else if (typ (arg_1) != LIST) {		/* parola (compreso numero) */
    l1 = wrdlen (arg1 = nome (arg_1));
    arg2 = nome (arg_2);
    j = 1;
    while (   ((pezzo = _pezzo (j, j+l1-1, arg2)) != S_VUOTA)
           && (! (risultato = _uguale (arg1, pezzo)))
          ) ++j;
  };
  if (risultato) _doveele = j;
  risultato = logval ((int) risultato);
}
 
void DOVEELE ()
{
  risultato = int_num ((int) _doveele);
}

/*----------------------------------------------------------------
  funzione che riporta l' intersezione (insiemistica) di due liste
  ----------------------------------------------------------------*/
void INTERSEZIONE ()
{
  risultato = set_intersection (arg_1, arg_2);
}

/*----------------------------------------------------------------
  funzione che riporta la differenza (insiemistica) di due liste
  ----------------------------------------------------------------*/
void MENLISTA ()
{
  risultato = set_difference (arg_1, arg_2);
}

/*----------------------------------------------------------------
  funzione che riporta l' unione (insiemistica) di due liste
  ----------------------------------------------------------------*/
void UNIONE ()
{
  risultato = set_union (arg_1, arg_2);
}

/*--------------------------------------------------------------------
  funzione che riporta una copia della lista data, ordinata secondo un
  criterio lessicografico o numerico, a seconda del tipo degli elementi
  ---------------------------------------------------------------------*/
void ORDINA ()
{
	int tipo;
	node ele;
	if (arg_1 == NULLP) return;
//>000301
//	tipo = typ (car (arg_1));
	tipo = tipoalfa (car (arg_1));
//<
/*>990323
	risultato = NULLP;
	while (arg_1 != NULLP) {
		if (typ (ele = car (arg_1)) != tipo) {err_arg1 (); return;};
		risultato = sorted_insert (ele, risultato, &confro);
		arg_1 = cdr (arg_1);
	};
*/
	R = NULLP;
	A = arg_1;
	while (A != NULLP) {
//>000301
//		if (typ (ele = car (A)) != tipo) {
		if (tipoalfa (ele = car (A)) != tipo) {
//<
			err_arg1 (); return;
		}
		R = sorted_insert (ele, R, &confro);
		A = cdr (A);
	};
	risultato = R;
/*<*/
}

int confro2 (node term1, node term2);
/*--------------------------------------------------------------------
  funzione che estende ORDINA a liste generalizzate, es
  ---------------------------------------------------------------------*/
node OrdinaRicorsivamente (node lista, int n)
{
	node cella = lista;
	node risultato = NULLP;
	node ele;
	int i = 0;
	while ((cella != NULLP) && (i < n)) {
		++i;
		cella = cdr (cella);
	};
	while (cella != NULLP) {
		ele = car (cella);
		if (typ(ele) == LIST)
			ele = OrdinaRicorsivamente (ele, n);
		risultato = sorted_insert (ele, risultato, &confro2);
		cella = cdr (cella);
	};
	while (i > 0) {
		risultato = cons (_ele (i, lista), risultato, LIST);
		--i;
		lista = cdr (lista);
	};
	return risultato;
}

void PR_ORDINA2 ()
{
	int n = 0;
	if (n_argomenti > 1)
		n = rnum_int (arg_2);
	risultato = OrdinaRicorsivamente (arg_1, n);
}

/* funzione reimplementata : */

void REVERSE ()
{
 	char c_s[8*K], *reverse_s;
 	node logo_s;
 	risultato = arg_1;
 	if (typ (arg_1) == LIST) {
		if (arg_1 != NULLP)
			risultato = listainv (arg_1);
	}
	else {
		logo_s = nome (arg_1);
		if (logo_s != S_VUOTA) {
			pacstr (c_s, logo_s);
			reverse_s = _strrev (c_s);
			risultato = logstr (reverse_s);
			if (typ (arg_1) == LWORD)
				risultato = mettip (risultato);
		}
  }
}

void PR_EMPTYWORD ()
{
	risultato = pc_VUOTA;
}

void PR_EMPTYLIST ()
{
	risultato = NULLP;
}

// copiato e ritoccato PR_FIRSTS
void PR_LASTS ()
{
	node tail, sublist, el;
	risultato = NULLP;
	if (arg_1 != NULLP) {
		tail = arg_1;
		if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST)) { 
//			risultato = el = cons (car (sublist), NULLP, LIST);
			risultato = el = cons (car (lastel (sublist)), NULLP, LIST);
			while ((tail = cdr (tail)) != NULLP) {
				if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST))
//					el = putcdr (el, cons (car (sublist), NULLP, LIST)); 
					el = putcdr (el, cons (car (lastel (sublist)), NULLP, LIST)); 
				else { err_arg1 (); return; }
			}
		} else err_arg1 ();
	}
}

// copiato e ritoccato PR_BUTFIRSTS
void PR_BUTLASTS ()
{
	node tail, sublist, el;
	risultato = NULLP;
	if (arg_1 != NULLP) {
		tail = arg_1;
		if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST)) { 
//>			risultato = el = cons (cdr (sublist), NULLP, LIST);
			risultato = el = cons (_menult (sublist), NULLP, LIST);
//<
			while ((tail = cdr (tail)) != NULLP) {
				if (((sublist = car (tail)) != NULLP) && (typ (sublist) == LIST))
//>					el = putcdr (el, cons (cdr (sublist), NULLP, LIST)); 
					el = putcdr (el, cons (_menult (sublist), NULLP, LIST)); 
//<
				else { err_arg1 (); return; }
			}
		} else err_arg1 ();
	}
}

// ispirato a STRINGA
// richiede inizializzazione e finalizzazione
void CatString (node logoString)
{
	int   c;
	irdstr (logoString);
	while ((c = rdstr ()) != EOS) {
//		libera_1 ();
//>020605
//		TEST_LIBERA_VOID(128)
		libera_2 ();
//<020605
		R = wtstr ((char) c);
	};
}

node Combine2Strings (node logoString1, node logoString2)
{
	R = iwtstr ();
	CatString (logoString1);
	CatString (logoString2);
	return R;
}

//to combine :this :those
//if wordp :those [output word :this :those]
//output fput :this :those
//end
void PR_COMBINE ()
{
 	if (typ (arg_2) == LIST)
		risultato = cons (arg_1, arg_2, LIST);
	else
 		if ((typ (arg_1) == LIST) || (typ (arg_1) == ARAY))
			err_arg1 ();
		else {
			node arg1 = arg_1, arg2 = arg_2;
			int type1 = contr_conv (&arg1, A_STRINGA);
			if (! is_errore) 
				int type2 = contr_conv (&arg2, A_STRINGA);
			if (! is_errore) {
				risultato = Combine2Strings (nome (arg1), nome (arg_2));;
				risultato = mettip (risultato);
			}
		}
}

void PR_REMOVE ()
{
	risultato = RemoveEqual (arg_1, arg_2);
}

node Duplicate (node object);
void PR_DUPLICATE ()
{
	risultato = Duplicate (arg_1);
}

void PR_REMDUP ()
{
	risultato = RemoveDuplicate (arg_1);
}

void PR_QUOTED ()
{
/*>990714
	R = iwtstr ();
	R = wtstr ('"');
	CatString (nome (arg_1));
	risultato = mettip (R);
*/
	risultato = arg_1;
/*<*/
}

void PR_GENSYM ()
{
	static int SymCount = 0;
	char s [MAXCHARS];

	sprintf (s, "gs%d", ++SymCount);
	risultato = mettip (logstr (s));
}

/* ILPARLIS.C */
