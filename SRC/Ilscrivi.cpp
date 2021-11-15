/*ILSCRIVI.C	G. Toffoli

000511	definito MAX_LINE e sost. MAXCHARS con MAX_LINE in scri_atomo
991012	aggiunto costrutto simmetrico di quote (letteralizzazione) per stringhe
980904	ripreso e completato scrittura di array
980828	preparazione per conversione a C++; corretto pop_sc (??)
980204	corretto disattivazione modifica (#define OLD) in _scrivi
971221	modificato scri_atomo; ritoccato a_capo
971205	Riattivato e modificato gestione di comando HALT (ex  BREAK ta tastiera) 
960601	mod. a_capo
960410	prima vers. Windows da Iperlogo del 15.11.88
*/

// #define OLD
#include "stdafx.h"

#define CMODULE
#define ILWIN
#define NEW_QUOTE

#include "ilcomune.cpp"
#include "ilfunz.h"

#ifdef NEW_QUOTE
int isPrintQuoted = FALSE;
#endif // NEW_QUOTE

#define MAX_LINE (4 * K)

void EventCheck ();

/*-------------------------------------------------------------
scrive il carattere delimitatore di linea su device di output
-------------------------------------------------------------*/
void a_capo ()
{
#ifdef ILWIN
	if (_fstato [dev_out] & O_FOGLIO)
		_prints ("\r\n", 2);
	else 
#endif // ILWIN
		_prints ("\r", 1);
}

/*----------------------------------------------
scrive il carattere spazio su device di output
----------------------------------------------*/
void spazio ()
{ 
	_prints (" ", 1);
}

/*--------------------------------------------------------------------
in preparazione a eventuale scrittura su device TARTALFA :
- cancella eventuale finestra comandi in schermo MISTO
- cancella cursore di TARTA
- inizializza cursore (riga e colonna) alfanumerico in schermo TARTA
--------------------------------------------------------------------*/
void inscrivi ()
{
#ifdef ILDOS
	if (dev_scrivi == TARTALFA)
		intarta ();
#endif // ILDOS
}

/*--------------------------------------------------------------------
all' uscita da eventuale scrittura su device TARTALFA :
- ripristina cursore di TARTA
- ripristina eventuale finestra comandi in schermo MISTO
--------------------------------------------------------------------*/
void outscrivi ()
{
#ifdef ILDOS
	if (dev_scrivi == TARTALFA)
		outtarta ();
#endif // ILDOS
}

/*---------------------------------------------------------
scrive su device di output un oggetto atomico (non lista)
---------------------------------------------------------*/
void scri_atomo (node oggetto)
{
	node stringa;
	int ltot, l;
	char s_locale [MAX_LINE+1];
//	tstbreak ();		/* testa eventuale richiesta di break */
	switch (typ (oggetto)) {
	case LSTR :
	case LWORD:
#ifdef ILWIN
		if (oggetto == DEL_FINELINEA) {
			a_capo ();
			break;
		}
#endif // ILWIN
	case LVAR :
	case UFUN :
	case SFUN :
	case PLIST:
		ltot = wrdlen (stringa = nome (oggetto));
		if (ltot == 0) spazio ();
		else
		{
#ifdef NEW_QUOTE
			if (isPrintQuoted && (typ (oggetto) == LSTR))
				_prints ("«", 1);
#endif // NEW_QUOTE
//#define PROVA_PACSTR
#ifndef PROVA_PACSTR
			irdstr (stringa);
			do {
				/// 		_prints (s_locale, l = bufstr (s_locale, ltot, MAXCHARS));
				l = bufstr (s_locale, ltot, MAX_LINE);
				_prints (s_locale, l);
			}
			while (l >= MAX_LINE);
#else // PROVA_PACSTR
			l = pacstr (s_locale, stringa);
			_prints (s_locale, l);
#endif // PROVA_PACSTR
#ifdef NEW_QUOTE
			if (isPrintQuoted && (typ (oggetto) == LSTR))
				_prints ("»", 1);
#endif // NEW_QUOTE
		};
		break;
	case LBCD :
		_prints (s_locale, edo_num (oggetto, s_locale));
		break;
	};
	no_spazio = ((oggetto >= DEL_FINELINEA) && (oggetto <= DEL_PARSINISTRA));
}

/*----------------------------------------------------------------------
scrive su device di output un generico oggetto, togliendo le parentesi
esterne in caso di lista e senza aggiungere LINEDEL
----------------------------------------------------------------------*/
#ifndef OLD

void _scrivi (node oggetto)
{
	int livello = 0;
	int finito = FALSE;
	no_spazio = TRUE;
	node save_p_sc = p_sc;
	push_sc (oggetto);
	if (typ (oggetto) == ARAY) {
			oggetto = cdr (oggetto);
			_prints ("{", 1);
	}
	while ((! finito) && (! is_errore)) {
		// caso 1 : scende a elemento di lista
		if ((oggetto != NULLP) && (typ (oggetto) == LIST)) {
			push_sc (oggetto);			// salva elemento
			oggetto = car (oggetto);	// prende valore elemento
			++livello;					// (ri)scende di livello
			if (typ (oggetto) == LIST) {
				push_sc (oggetto);			// salva tipo struttura
				if (! no_spazio) spazio ();
				no_spazio = TRUE;
				_prints ("[", 1);
			} else if (typ (oggetto) == ARAY) {
				push_sc (oggetto);			// salva tipo struttura
				oggetto = cdr (oggetto);	// punta a lista valori
				if (! no_spazio) spazio ();
				no_spazio = TRUE;
				_prints ("{", 1);
			};
		}
		// caso 2 : altrimenti
		else {
			// azione 21 : scrittura atomo se <> da NULLP
			if (oggetto != NULLP) {
				if (   (! no_spazio) 
					&& (oggetto != DEL_FINELINEA)
					&& (oggetto != DEL_PARDESTRA)
					) spazio ();
				scri_atomo (oggetto);
				EventCheck ();			// 971205
				if (is_break)			//
					return;				//
			};
			// azione 22
			do {
				// caso 221 : finito
				if (livello == 0) finito = TRUE;
				// caso 222 : non finito: avanzamento con eventuale risalita
				else {
					// caso 2221 : eventuale chiusura con risalita
					if (oggetto == NULLP) {
//						node temp = pop_sc ();
						int tipo = typ (pop_sc ());
//						push_sc (temp);
						if (tipo == LIST) {
							_prints ("]", 1);
							no_spazio = FALSE;
						} else if (tipo == ARAY) {
							_prints ("}", 1);
							no_spazio = FALSE;
						}
					}
					--livello;
					oggetto = cdr (pop_sc (/*oggetto*/));	// 980828
				}
			} while ((! finito) && (oggetto == NULLP));
		}
	}
	oggetto = pop_sc ();
	if (typ (oggetto) == ARAY) {
		_prints ("}", 1);
		no_spazio = FALSE;
	}
	p_sc = save_p_sc;
}

#else //OLD

void _scrivi (node oggetto)
{
	int livello, finito;
	livello = 0;
	finito = FALSE;
	no_spazio = TRUE;
	while ((! finito) && (! is_errore)) {
		if ((oggetto != NULLP) && (typ (oggetto) == LIST)) {
			push_sc (oggetto);
			oggetto = car (oggetto);
			++livello;
			if (typ (oggetto) == LIST) {
				if (! no_spazio) spazio ();
				no_spazio = TRUE;
				_prints ("[", 1);
			};
		}
		else {
			if (oggetto != NULLP) {
				if (   (! no_spazio) 
					&& (oggetto != DEL_FINELINEA)
					&& (oggetto != DEL_PARDESTRA)
					) spazio ();
				scri_atomo (oggetto);
				EventCheck ();			// 971205
				if (is_break)			//
					return;				//
			};
			do {
				if (livello == 0) finito = TRUE;
				else {
					if (oggetto == NULLP) {
						_prints ("]", 1);
						no_spazio = FALSE;
					}
					--livello;
					oggetto = cdr (pop_sc (/*oggetto*/));	// 980828
				}
			} while ((! finito) && (oggetto == NULLP));
		}
	}
}

#endif //OLD

void _mostra (node oggetto)
{
	int tipo;
	if ((tipo = typ (oggetto)) == LIST) _prints ("[", 1);
	_scrivi (oggetto);
	if (tipo == LIST) _prints ("]", 1);
}

void STAMPA ()
{
	int i;
	inscrivi ();
	for (i = n_argomenti; i > 1; --i) {
		_scrivi (get_sv (i));
		spazio ();
	};
	_scrivi (get_sv (1));
	a_capo ();
	pop_args ();
	outscrivi ();
} 

void MOSTRA ()
{
	int i;
	inscrivi ();
	for (i = n_argomenti; i > 1; --i) {
		_mostra (get_sv (i));
		spazio ();
	};
	_mostra (get_sv (1));
	a_capo ();
	pop_args ();
	outscrivi ();
} 

void SCRIVI ()
{
	int i;
	inscrivi ();
	for (i = n_argomenti; i >= 1; --i) _scrivi (get_sv (i));
	pop_args ();
	outscrivi ();
} 

#ifdef NEW_QUOTE
void PR_PRINT_QUOTED ()
{
	isPrintQuoted = TRUE;
	SCRIVI ();
	isPrintQuoted = FALSE;
} 
#endif // NEW_QUOTE

/*---------------------------------------------
scrive carattere generico su device di output
---------------------------------------------*/
void _scricar (char c)
{
	char s_locale [2];
	s_locale [0] = c;
	_prints (s_locale, 1);
}

/*---------------------------------------
scrive il primo carattere di una parola
---------------------------------------*/
void SCRICAR ()
{
	inscrivi ();
	irdstr (nome (arg_1));
	_scricar (rdstr ());
	outscrivi ();
}

/* ILSCRIVI.C */
