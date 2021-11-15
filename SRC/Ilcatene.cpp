/*ILCATENE.C		G. Toffoli

  primitive interne per la gestione di oggetti implementati
  mediante catene di celle cons: liste, parole, stringhe

000206	mod. member (usato in set_intersection e set_difference)
000102	ridefinito pc_CASEIGNOREDP
991102	aggiunto Duplicate (copia di oggetti con ricorsione sulle liste)
990331	corretto valore riportato da rdstr e rdstr2
990323	in listainv modificato R con R2
980831	interfaccia logstr, match_sc, match_sc_case
		char * -> const char *
		CONTROLLARE MATCH_SC !!!!!
980828	preparazione per conversione a C++
		sostituito min e max con __min e __max
980205	test di rdstr
971205	definito macro tonormal e cambiato convenzione su normalizzazione (lowercase)
970316	tentato recupero update del 02.10.96 (CASEIGNOREDP)
960925	sdoppiato match e match_sc in due versioni, case-sensitive e no
960409	prima vers. Windows da Iperlogo del 22.06.89
	
*/

#include "stdafx.h"
#include <ctype.h>

#define ILWIN
#define NORMALIZE

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#ifdef ILWIN

#ifdef NORMALIZE
// int case_sensitive = FALSE;
int MatchKw (node, node);
// #define pc_CASEIGNOREDP pc_MODO
#define pc_CASEIGNOREDP kw[84]
#define case_sensitive (MatchKw (car (cercav (pc_CASEIGNOREDP, LVAR)), pc_FALSO))
//#define tonormal(x) toupper(x)
#define tonormal(x) tolower(x)

int is_casesensitive () {
	return case_sensitive;
}
#endif // NORMALIZE

#endif // ILWIN

int match_sc (const char *s, node str2);		// 980831
int match_sc_case (const char *s, node str2);	//
int match_sc_nocase (const char *s, node str2);		//

/********************************************
PRIMITIVE CHE OPERANO SU LISTE
********************************************/

/*------------------------------------------
ritorna il numero di elementi di una lista
------------------------------------------*/
int lstlen (node lista)
{
	int i = 0;
	while (lista != NULLP) {
		++i;
		lista = cdr (lista);
	};
	return i;
}

/*---------------------------------------------------------------------------
ritorna l' ultimo elemento di una lista; ritorna NULLP se la lista e' vuota
---------------------------------------------------------------------------*/
node lastel (node lista)
{
	node elemento = lista;
	while (lista != NULLP) {
		elemento = lista;
		lista = cdr (lista);
	};
	return elemento;
}

/*------------------------------------------------------
concatena lista2 a lista1 e ritorna la lista risultato
------------------------------------------------------*/
node lstcat (node lista1, node lista2)
{
	if (lista1 == NULLP) lista1 = lista2;
	else putcdr (lastel (lista1), lista2);
	return lista1;
}

/* -------------------------------------------------------------
Crea la coppia contenente i due nodi argomento.
------------------------------------------------------------- */
node coppia (node nodo1, node nodo2)
{
	return cons (nodo1, cons (nodo2, NULLP, LIST), LIST);
}

/*-------------------------------------------------------------
riporta l' intersezione di 2 insiemi rappresentati come liste
-------------------------------------------------------------*/
node set_intersection (node lista1, node lista2)
{
	node ele, riporta = NULLP;
	int n;
	if ((lista1 == NULLP) || (lista2 == NULLP)) return NULLP;
	else {
		lista1 = listainv (lista1);
		/*<29.05.89*/
		if (   (buff_pic != NULL)
			&& (n = list2array (lista2, (node *) buff_pic, /* 8*K */ DIM_BPIC / 4 )))
			while (lista1 != NULLP) {
				if (nodeinarray (ele = car (lista1), (node *) buff_pic, n))
					riporta = cons (ele, riporta, LIST);
				lista1 = cdr (lista1);
			}
			else
				/*>*/
				while (lista1 != NULLP) {
					if (member (ele = car (lista1), lista2))
						riporta = cons (ele, riporta, LIST);
					lista1 = cdr (lista1);
				};
				return riporta;
	};
}

/*-------------------------------------------------------------
riporta la differenza di 2 insiemi rappresentati come liste,
cioe' gli elementi di LISTA1 non presenti in LISTA2
-------------------------------------------------------------*/
node set_difference (node lista1, node lista2)
{
	node ele, riporta = NULLP;
	int n;
	if (lista2 == NULLP) return lista1;
	else {
		lista1 = listainv (lista1);
		/*<29.05.89*/
		if (   (buff_pic != NULL)
			&& (n = list2array (lista2, (node *) buff_pic, /* 8*K */ DIM_BPIC / 4 )))
			while (lista1 != NULLP) {
				if (! nodeinarray (ele = car (lista1), (node *) buff_pic, n))
					riporta = cons (ele, riporta, LIST);
				lista1 = cdr (lista1);
			}
			else
				/*>*/
				while (lista1 != NULLP) {
					if (! member (ele = car (lista1), lista2))
						riporta = cons (ele, riporta, LIST);
					lista1 = cdr (lista1);
				};
				return riporta;
	};
}

/*-------------------------------------------------------------
riporta l' unione di 2 insiemi rappresentati come liste,
cioe' LISTA1 concatenata alla differenza di LISTA2 e LISTA1
-------------------------------------------------------------*/
node set_union (node lista1, node lista2)
{
	return lstcat (lstdup (lista1), set_difference (lista2, lista1));
}

/*------------------------------------------------------------
riporta TRUE se ELE coincide con uno degli elementi di LISTA
(non basta che sia isomorfo)
000206: ora basta che sia isomorfo
------------------------------------------------------------*/
int member (node ele, node lista)
{
	while (lista != NULLP) {
//> 000206
//		if (ele == car (lista)) return TRUE;
		if (_uguale (ele, car (lista))) return TRUE;
//<
		lista = cdr (lista);
	};
	return FALSE;
}

/*---------------------------------------------------------------
cancella da una lista la cella di car specificato (ele);
eventualmente modifica la variabile lista passata per indirizzo;
se l' elemento cercato non viene trovato non fa niente;
---------------------------------------------------------------*/
void RemoveEle (node *lista, node ele)
{
	node previous, tail = *lista;
	if (ele == car (tail)) *lista = cdr (tail);
	else do {
		previous = tail; tail = cdr (tail);
		if (ele == car (tail)) {
			putcdr (previous, cdr (tail));
			return;
		}
	} while (tail != NULLP);
}

/*-----------------------------------------------------------------------
cancella un elemento da una lista senza fare controllo di appartenenza;
ritorna l' elemento successivo della lista;
modifica la variabile lista passata per indirizzo
----------------------------------------------------------------------*/
node lstdel (node *lista, node elemento)
{
	node vecchio_el, nuovo_el;
	if ((nuovo_el = *lista) == elemento) return (*lista = cdr (elemento));
	else {
		while ((nuovo_el = cdr (vecchio_el = nuovo_el)) != elemento) {};
		return putcdr (vecchio_el, cdr (nuovo_el));
	}
}

/*----------------------------------------------------
crea e ritorna un duplicato di una lista o di un bcd
----------------------------------------------------*/
node lstdup (node lista)
{
	node nuova_lista, el;
	if (lista == NULLP) return NULLP;
	else {
		nuova_lista = el = cons (car (lista), NULLP, typ (lista));
		while ((lista = cdr (lista)) != NULLP)
			el = putcdr (el, cons (car (lista), NULLP, typ (lista)));
		return nuova_lista;
	};
}

node Duplicate (node object);
/*------------------------------------------
crea e ritorna una copia di un oggetto,
duplicando ricorsivamente liste e sottoliste
--------------------------------------------*/
node Duplicate (node object)
{
	node new_object, item;
	if (typ (object) != LIST)
		return object;
	else if (object == NULLP) return NULLP;
	else {
		new_object = item = cons (Duplicate (car (object)), NULLP, LIST);
		while ((object = cdr (object)) != NULLP)
			item = putcdr (item, cons (Duplicate (car (object)), NULLP, LIST));
		return new_object;
	};
}

/*------------------------------------------------------------------
ritorna la lista ottenuta dalla lista data invertendo gli elementi
------------------------------------------------------------------*/
node listainv (node lista)
{
	R2 = NULLP;
	while (lista != NULLP) {
		libera_1 ();
		R2 = cons (car (lista), R2, LIST);
		lista = cdr (lista);
	};
	return R2;
}

/*************************************************************************
PRIMITIVE CHE OPERANO SU STRINGHE

  S_VUOTA = 1 rappresenta una stringa vuota e il cdr dell' ultimo nodo pieno
  della stringa;
  EOS	  =-1 rappresenta lo stato di fine stringa in lettura
*************************************************************************/

/*-----------------------------------------------------------------
inizializza lettura di stringa MLOGO (apre la stringa in lettura)
-----------------------------------------------------------------*/
void irdstr (node nome)
{
	int lung1 = sizeof(val_rd_c);
	int lung2 = sizeof(val_rd_c.value);
	if ((rd_cell = nome) == S_VUOTA) rd_cell = NULLP;
	else {
		i_rd_c = -1;	/* indice in rd_cell del carattere corrente */
		getnode (rd_cell, (char *) &val_rd_c);
		l_rd_c = val_rd_c.length;	/* intero tra 0 e 7 */
	};
}

/*---------------------------------------------------------------
ritorna carattere successivo di stringa MLOGO aperta in lettura
---------------------------------------------------------------*/
unsigned rdstr ()
{
	itera:
	if (rd_cell == NULLP) return EOS;
	/* punta al prossimo carattere da leggere (0 .. 7) */
	else if (++i_rd_c == l_rd_c) {
		if ((val_rd_c.value.cc.cdr == NULLP) || (l_rd_c != 4))
			return EOS;
		else {	/* passa a cella successiva e itera */
			irdstr (val_rd_c.value.cc.cdr);
		goto itera;
		};
//	} else return (unsigned) val_rd_c.value.string [i_rd_c]);
	} else return (unsigned) (0xFF & val_rd_c.value.string [i_rd_c]);
}

/*-----------------------------------------------------------------
inizializza lettura di stringa MLOGO (apre la stringa in lettura)
-----------------------------------------------------------------*/
void irdstr2 (node nome)
// node nome;
{
	if ((rd_cell2 = nome) == S_VUOTA) rd_cell2 = NULLP;
	else {
		i_rd_c2 = -1;	/* indice in rd_cell del carattere corrente */
		getnode (rd_cell2, (char *) &val_rd_c2);
		l_rd_c2 = val_rd_c2.length; /* intero tra 0 e 7 */
	};
}

/*---------------------------------------------------------------
ritorna carattere successivo di stringa MLOGO aperta in lettura
---------------------------------------------------------------*/
unsigned rdstr2 ()
{
	itera2:
	if (rd_cell2 == NULLP) return EOS;
	/* punta al prossimo carattere da leggere (0 .. 7) */
	else if (++i_rd_c2 == l_rd_c2) {
		if ((val_rd_c2.value.cc.cdr == NULLP) || (l_rd_c2 != 4))
			return EOS;
		else {	/* passa a cella successiva e itera */
			irdstr2 (val_rd_c2.value.cc.cdr);
		goto itera2;
		};
//	} else return (unsigned) val_rd_c2.value.string [i_rd_c2];
	} else return (unsigned) (0xFF & val_rd_c2.value.string [i_rd_c2]);
} 

/*------------------------------------------------------------------
inizializza scritt. di stringa MLOGO (apre la stringa in scritt.);
ritorna S_VUOTA
------------------------------------------------------------------*/
node iwtstr ()
{
	return (wt_cell = S_VUOTA);
}

/*-----------------------------------------------------------------
appende carattere a stringa MLOGO (aperta con IWTSTR)
-----------------------------------------------------------------*/
node wtstr (char c)
{
	static node stringa;
//	node new;
	node newcell;
	if (wt_cell == S_VUOTA) {
		i_wt_c = 0;
		stringa = wt_cell = consstr (1, &c);
	} else if (++i_wt_c == 8) {
	/*<05.12.88
	i_wt_c = 0;
		*/
		i_wt_c = 4; 	/* nella cella NEW ci saranno 5 caratteri */
		/*>*/
		/* il cdr di wt_cell sono i 4 caratteri che vanno trasferiti all'inizio
		di NEW (cioe' nel car);
		(node) c e' il primo carat. nel cdr di new
		(5 << 8) e' il numero di carat. (5) che contiene il nodo NEW */
		newcell = cons (cdr (wt_cell), (node) c, LSTR | (5 << 8));
		puttyp (wt_cell, LSTR | (4 << 8));	/* wt_cell e' tipo LSTR e lung. 4 */
		putcdr (wt_cell, newcell);
		wt_cell = newcell;
	} else {
		put_char (wt_cell, c);
	};
	return stringa;
}

/*---------------------------------------------------
ritorna il numero di caratteri di una stringa MLOGO
---------------------------------------------------*/
int wrdlen (node nome)
{
	int i = 0;
	irdstr (nome);
	while (rdstr () != EOS) ++i;
	return i;
}

/*------------------------------------------------------------------------
confronta due stringhe LOGO: riporta un numero maggiore, uguale, minore
di 0 a seconda che la prima stringa sia maggiore, uguale o minore della
seconda nell' ordinamento alfabetico
------------------------------------------------------------------------*/
// int match (str1, str2)

// versione originaria, case-sensitive
int match_case (node str1, node str2)
{
	int n1, n2, n, i, delta;
	char s1 [12], s2 [12];
	if (str1 == S_VUOTA) str1 = NULLP; if (str2 == S_VUOTA) str2 = NULLP;
match_loop:
	if (str1 == NULLP) return (str2 == NULLP) - 1;
	if (str2 == NULLP) return 1;
	n = __min (n1 = node2str (str1, s1), n2 = node2str (str2, s2));
	for (i = 0; i < n; ++i) {
		if ((delta = s1 [i] - s2 [i]) != 0) return delta;
	};
	if ((n1 == 4) && (n2 > 4)) {	/* caso asimmetrico 1 */
		if ((str1 = cdr (str1)) == NULLP) return (-1); /* str1 e' piu' corta */
		node2str (str1, &s1 [4]); /* ci sono almeno altri 5 carat. in str1 */
		for (i = 4; i < n2; ++i) {
			if ((delta = s1 [i] - s2 [i]) != 0) return delta;
		};
		return 1;				/* str1 e' solo piu' lunga di str2 */
	};
	if ((n2 == 4) && (n1 > 4)) {	/* caso asimmetrico 2 */
		if ((str2 = cdr (str2)) == NULLP) return 1; /*(-1); str2 e' piu' corta */
		node2str (str2, &s2 [4]); /* ci sono almeno altri 5 carat. in str2 */
		for (i = 4; i < n1; ++i) {
			if ((delta = s1 [i] - s2 [i]) != 0) return delta;
		};
		return -1;			/* str2 e' solo piu' lunga di str1 */
	};
	if ((n1 != 4) || (n2 != 4)) return n1 - n2; /* non continuano */
	str1 = cdr (str1); str2 = cdr (str2);	/* potrebbero cont. entrambi */
	goto match_loop;
}

// versione nuova, case-insensitive
int match_nocase (node str1, node str2)
{
	int n1, n2, n, i, delta;
	char s1 [12], s2 [12];
	if (str1 == S_VUOTA) str1 = NULLP; if (str2 == S_VUOTA) str2 = NULLP;
match_loop:
	if (str1 == NULLP) return (str2 == NULLP) - 1;
	if (str2 == NULLP) return 1;
	n = __min (n1 = node2str (str1, s1), n2 = node2str (str2, s2));
	for (i = 0; i < n; ++i) {
		if ((delta = tonormal(s1 [i]) - tonormal(s2 [i])) != 0) return delta;
	};
	if ((n1 == 4) && (n2 > 4)) {	/* caso asimmetrico 1 */
		if ((str1 = cdr (str1)) == NULLP) return (-1); /* str1 e' piu' corta */
		node2str (str1, &s1 [4]); /* ci sono almeno altri 5 carat. in str1 */
		for (i = 4; i < n2; ++i) {
			if ((delta = tonormal(s1 [i]) - tonormal(s2 [i])) != 0) return delta;
		};
		return 1;				/* str1 e' solo piu' lunga di str2 */
	};
	if ((n2 == 4) && (n1 > 4)) {	/* caso asimmetrico 2 */
		if ((str2 = cdr (str2)) == NULLP) return 1; /*(-1); str2 e' piu' corta */
		node2str (str2, &s2 [4]); /* ci sono almeno altri 5 carat. in str2 */
		for (i = 4; i < n1; ++i) {
			if ((delta = tonormal(s1 [i]) - tonormal(s2 [i])) != 0) return delta;
		};
		return -1;			/* str2 e' solo piu' lunga di str1 */
	};
	if ((n1 != 4) || (n2 != 4)) return n1 - n2; /* non continuano */
	str1 = cdr (str1); str2 = cdr (str2);	/* potrebbero cont. entrambi */
	goto match_loop;
}

int match (node str1, node str2)
{
	return (case_sensitive) ?
		match_case (str1, str2) : match_nocase (str1, str2);
}

/*------------------------------------------------------------------------
confronta stringa C con stringa MLOGO: riporta un numero >, =, <
di 0 a seconda che la prima stringa sia maggiore, uguale o minore della
seconda nell' ordinamento alfabetico
------------------------------------------------------------------------*/

// versione originaria, case-sensitive
// int match_sc_case (char *s, node str2)
int match_sc_case (const char *s, node str2)
{
	int n, i, delta;
	char s2 [8];
	if (str2 == S_VUOTA) return (1 - (*s == NULLC)) ;
	n = node2str (str2, s2);		/* legge i carat. di str2 in s2 */
match_loop2:
	for (i = 0; i < n; ++i, ++s) {
		if (*s == NULLC) return (-1);
		if ((delta = *s - s2 [i]) != 0) return delta;
	};
	if (n != 4) return (1 - (*s == NULLC)); /* str2 non puo' continuare */
	n = node2str (str2 = cdr (str2), s2); /* legge seguito di str2 e ric.*/
	if (str2 == NULLP) return (1 - (*s == NULLC)) ;
	goto match_loop2;
}

// versione nuova, case-insensitive
// int match_sc_nocase (char *s, node str2)
int match_sc_nocase (const char *s, node str2)
{
	int n, i, delta;
	char s2 [8];
	if (str2 == S_VUOTA) return (1 - (*s == NULLC)) ;
	n = node2str (str2, s2);		/* legge i carat. di str2 in s2 */
match_loop2:
	for (i = 0; i < n; ++i, ++s) {
		if (*s == NULLC) return (-1);
		if ((delta = tonormal(*s) - tonormal(s2 [i])) != 0) return delta;
	};
	if (n != 4) return (1 - (*s == NULLC)); /* str2 non puo' continuare */
	n = node2str (str2 = cdr (str2), s2); /* legge seguito di str2 e ric.*/
	if (str2 == NULLP) return (1 - (*s == NULLC)) ;
	goto match_loop2;
}

//int match_sc (char *s, node str2)
int match_sc (const char *s, node str2)
{
	return match_sc_nocase (s, str2);
	return (case_sensitive) ?
		match_sc_case (s, str2) : match_sc_nocase (s, str2);
}

/*--------------------------------------------------------------
riporta una stringa MLOGO costruita a partire da una stringa C
--------------------------------------------------------------*/
// node logstr (char *s)
node logstr (const char *s)	// 980831
{
	int l;
//	node nome, old, new;
	node nome, old, newcell;
	l = strlen (s);
	nome = S_VUOTA;
	while (l != 0) {
		if (l <= 8) {
			newcell = consstr (l, s);
			l = 0;
		} else {
			newcell = consstr (4, s);
			s = s + 4; l = l - 4;
		};
		if (nome == S_VUOTA) nome = newcell;
		else putcdr (old, newcell);
		old = newcell;
	};
	return nome;
}

/*-------------------------------------------------------
converte una stringa MLOGO in una stringa C preallocata
ritorna la lunghezza della stringa C, NULLC compreso
-------------------------------------------------------*/
int pacstr (char *s, node nome)
{
	int l, n;
	l = 0;
	if (nome != S_VUOTA) do {
		l += (n = node2str (nome, s));
		s += n;
	} while ((n == 4) && ((nome = cdr (nome)) != NULLP));
	*s = NULLC;
	return l+1;
}

/*----------------------------------------------------------------------
converte una stringa MLOGO, di lunghezza l, gia' aperta e parzialmente
letta in una stringa C preallocata di lunghezza massima specificata;
ritornava ultimo carattere trasferito (EOF se stringa MLOGO e' finita)
ritorna numero di caratteri trasferiti
----------------------------------------------------------------------*/
int bufstr (char *s, int l, int maxc)
{
	char *p, *maxs;
	int c;
	maxs = s + maxc;
	p = s;
	while ((p != maxs) && ((c = rdstr ()) != EOS)) {
		if (is_letterale && IS_DELIMITATORE(c) && (l > 1))
			*(p++) = '\\';
		*(p++) = c;
	};
	*p = NULLC;
	///  return (int) (p - s);
	return (p - s);
}

/* ILCATENE.C */
