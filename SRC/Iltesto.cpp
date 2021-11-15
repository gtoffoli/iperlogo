/* ILTESTO.C		G. Toffoli

	990126	Gestione del browser corrente
	980828	preparazione per conversione a C++
	980714	riattivato ASTASTO e TASTO; aggiunto KeyProg
	980202	Ripulitura per WIN32: persa compatibilità DOS e WIN16
			separato SELEZIONA fin. (corrente) da MOSTRA (fai emergere)
			eliminato riferimenti a Q_TARTA
	980107	Gestione del foglio corrente
			modif. chiamata a sel_foglio in SELEZIONA e SelectWindowByName
	970911	completata selezione fisica di finestre TARTA e FOGLIO 
	970908	per compatibilita' VC++5.0: bool -> bool_ 
	970503	recupero update del 02.10.96 (normalizzazione)
	960507	disattivato cputs_ (ridefinito in IPERLVW.CPP)
	960423	prima vers. Windows da Iperlogo del 19.09.89
*/

#include "stdafx.h"

#define ILWIN

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

int MatchKw (node, node);

#include <stdio.h>

extern int cgetc_ ();

extern void HideCommander ();
extern void ShowCommander ();
void sel_browser (int dev);

unsigned char case_char (unsigned char c)
//unsigned char c;
{
	if (is_maiuscole) c = toupper (c);
	return c;
}

void case_string (char* s)
// char* s;
{
	unsigned char c;
	if (is_maiuscole)
		while ((c = *s) != '\0')
			*s++ = toupper(c);
}

void LOWERCASE ()
{
	int c;
	if ((A = nome (arg_1)) != S_VUOTA) {
		irdstr (A);
		R = iwtstr ();
		while ((c = rdstr ()) != EOS) {
			libera_1 ();
			R = wtstr (tolower((char) c));
		};
		risultato = str_par (R, typ (arg_1));
	}
}

void UPPERCASE ()
{
	int c;
	if ((A = nome (arg_1)) != S_VUOTA) {
		irdstr (A);
		R = iwtstr ();
		while ((c = rdstr ()) != EOS) {
			libera_1 ();
			R = wtstr (toupper((char) c));
		};
		risultato = str_par (R, typ (arg_1));
	}
}

/*----------------------------------------------
inizializza le finestre di console
----------------------------------------------*/
void ini_con ()
{
	_schermo = pc_COMANDI;
	
}

/*-------------------------------------------------------------
inizializzazione e reinizializzazione delle finestra di testo
-------------------------------------------------------------*/
void ini_testo ()
{
}

// void MainMenuShowWindow (char *);
void TurtleBringToTop (int nview); 
void EditorBringToTop (int nview); 

#ifdef OLD_SELECT

void SelectWindowByName (char *name)
{
	node lname = metti_sc (name);
	int dev = nome2dev (lname);
	int handle = _file [dev];
	if (_fstato [dev] & O_TARTA) {				/* .. tipo TARTA */
		switch_tarta (handle);
		TurtleBringToTop (handle); 
	}
	else if (_fstato [dev] & O_FOGLIO) {			/* .. tipo FOGLIO */
		sel_foglio (/*handle*/ dev);
		EditorBringToTop (handle);
	}
}

#endif // OLD_SELECT

/* ------------------------------------------------------------------------
Seleziona la viewport corrente.
Si rifiuta di selezionare: i device, i "file" non aperti, i "file" che
non sono delle finestre.
------------------------------------------------------------------------ */
void SELEZIONA ()
{
	int dev, handle;
	dev = nome2dev (arg_1);
	if (	(dev < MIN_DEV) 				/* device o sconosc. */
		|| (_fstato [dev] & O_ARCHIVIO))		/* archivio aperto */
		err2 (29, arg_1);
	else if (_fstato [dev] & O_FINESTRA) {		/* finestra .. */
		handle = _file [dev];
		if (_fstato [dev] & O_TARTA) {				/* .. tipo TARTA */
			switch_tarta (handle);
		}
		else if (_fstato [dev] & O_FOGLIO) {		/* .. tipo FOGLIO */
			sel_foglio (dev);
		}
		else if (_fstato [dev] & O_BROWSER) {		/* .. tipo BROWSER */
			sel_browser (dev);
		}
	}
}

void ASMAIUSCOLE ()
{
	is_maiuscole = bool_ (arg_1) ? 0xFF : 0x00;
}


void MAIUSCOLE ()
{
	risultato = logval (is_maiuscole);
}

/*-----------------------------------------------------------------------
riporta VERO se e' stato battuto un tasto e il suo valore non e' ancora
stato letto
-----------------------------------------------------------------------*/
void TASTO_ ()
{
	int c;
	int ris = TRUE;
	if (buf_c == NULLC) {
		if ((c = getc(stdin)) != EOF) ungetc (c, stdin);
		else ris = FALSE;
	};
	risultato = logval (ris);
}

/*-------------------------------------------------------------------
programma il tasto di codice arg_1 con la parola o la stringa arg_2
-------------------------------------------------------------------*/
void ASTASTO ()
{
	tab_prog [num_int (arg_1)] = (typ (arg_2) == LIST) ? arg_2 : nome (arg_2);
}

/*-------------------------------------------------
riporta il valore con cui e' programmato un tasto
-------------------------------------------------*/
void TASTO ()
{
	risultato = tab_prog [num_int (arg_1)];
}

node KeyProg (unsigned int nChar) 
{
	return tab_prog [nChar];
}

/*-------------------------------------
pulisce buffer di input della console
-------------------------------------*/
void clr_con ()
{
	buf_c = NULLC;		// ?
	j_tasti = i_tasti = 0;//?
	fflush (stdin);
}

/*----------------------------------------------------
ritorna un carattere: lo prende dal buffer se c' e',
altrimenti lo legge da console con lettura senza eco
----------------------------------------------------*/
int getch_ ()
{
	int c;
//	if (is_break) vai_top ();	?? e' possibile ?
	c = cgetc_ ();
	return (ultimo_c = c);
}

/*-------------------------------------------------------------------------
ritorna un codice di carattere su 2 byte; lo prende dal buffer s_tasti;
se questo e' vuoto prima lo rimbocca chiamando 1 o 2 volte getch_
e sostituendo eventualmente il codice con la stringa programmata
-------------------------------------------------------------------------*/
int gettasto ()
{
	int c;
	node s;
	if (j_tasti == i_tasti) {
		if ((c = getch_ ()) == 0) c = 256 + getch_ ();
//		if (is_break) vai_top ();	e' possibile ?
		if (   ((s = tab_prog [c]) == S_VUOTA)
			|| (s == NULLP)
			|| (   (typ (s) == LIST)
			&& ((is_per) || (! is_analisi) || (! is_prima_linea) || (fin_comando != ini_comando))
			)
			) return c;
		if (typ (s) == LIST)		/* lista programmata */
		{lista_prog = s; return DUMMY;}
		else unread (s);			/* parola programmata */
	};
	return (int) s_tasti [j_tasti++];
}

/*-------------------------------------------------------------------
accoda una stringa di caratteri al buffer di lettura della tastiera
-------------------------------------------------------------------*/
void unread (node s)
// node s;
{
	int c, i;
	char s_locale [128];
	pacstr (s_locale, s);
	i = 0; while  ((c = s_locale [i++]) != NULLC) s_tasti [i_tasti++] = c;
}

/*-------------------------------------------------------------------------
ASSCHERMO "TARTA  /  ASSCHERMO "MISTO  /  ASSCHERMO "COMANDI
se schermo e' MONO (1) dichiara l' esistenza di schermo colore per TARTA;
e abilita TARTA nella risoluzione grafica a colori 320 X 200;
se schermo e' COLO :
(1) assegna tutto schermo a TARTA nascondendo finestra COMANDI
(2) assegna tutto schermo a TARTA mostrando sopra parte di fin. COMANDI
(3) assegna tutto schermo a COMANDI nascondendo TARTA
------------------------------------------------------------------------*/
void ASSCHERMO ()
{
	if (arg_1 == _schermo) return;
	if (MatchKw (arg_1, pc_TARTA)) {
		_schermo = arg_1;
		HideCommander ();
	}
	else if (MatchKw (arg_1, pc_COMANDI)) { /***** COMANDI *****/
		_schermo = arg_1;
		ShowCommander ();
	}
	else if (MatchKw (arg_1, pc_MISTO)) { /***** MISTO ***/
		_schermo = arg_1;
		ShowCommander ();
	}
	else err_arg1 ();
}

void SCHERMO ()
{
	risultato = _schermo;
}

/* ILTESTO.C */
