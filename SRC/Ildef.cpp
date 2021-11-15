/*ILDEF.C		G. Toffoli

000112	aggiunto PR_LOCALMAKE (ASLOCALE), da collaudare
990429	variante compatibile con MswLogo in sintassi di DEFINISCI
981016	riattivato COPIADEF e RINOMINA
		esteso CANCEPRIM da lista a singola primitiva
980828	preparazione per conversione a C++
980210	iniziato pulizia per WIN32; persa compatib. DOS e WIN16
970315	disattivato rif. a BIT_COMPILA (obsoleto)
960410	prima vers. Windows da Iperlogo del 15.11.88
*/

#include "stdafx.h"

#define ILWIN
#define NORMALIZER

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

/*------------------------------------------------------
come ASsegna <nome> <valore>, preceduto da LOCALE <nome>
--------------------------------------------------------*/
void PR_LOCALMAKE ()
{
	_locale ();
	AS ();
}

/*-------------------------------------------------------
assegna al contenitore di nome arg_1 il contenuto arg_2
-------------------------------------------------------*/
void AS ()
{
	node variabile;
	if ((variabile = cercav (arg_1, LVAR)) == NULLP)
		variabile = mettio (arg_1, arg_2, LVAR);
	else putcar (variabile, arg_2);
	recu_pac ();
}

/*-----------------------------------------------------
pone il contenuto arg_1 nel contenitore di nome arg_2
-----------------------------------------------------*/
void CHIAMA ()
{
	node locale;
	locale = arg_1; arg_1 = arg_2; arg_2 = locale;
	AS ();
}

/*-------------------------------------------------------
accetta la definizione linea per linea di una procedura
-------------------------------------------------------*/
void PER ()
{
	node  lista_arg, oggetto, locale;
	gettok ();
	token = cdr (token);
	if (val_token == DEL_VIRGOLETTE) {
		gettok ();
		token = cdr (token);
	};
	if (   (! tipoalfa (arg_1 = val_token))
		|| (IS_RIS((arg_1=mettip (arg_1))))
		) err_arg1 ();
	else if (cercav (arg_1, SFUN) != NULLP) err2 (6, arg_1);
	else if (_coperto (oggetto = cercav (arg_1, UFUN))) err2 (9, arg_1);
	else {
		lista_arg = NULLP;
		while (token != NULLP) {/* ricerca fine linea di comando */
			gettok ();
			token = cdr (token);
			if (val_token == DEL_FINELINEA) break;
			else if (val_token != DEL_DUEPUNTI) {
				err2 (5, val_token);
				break;
			};
			gettok ();
			locale = token;
			token = cdr (token);
			if (! tipoalfa (val_token)) {
				err2 (5, val_token);
				break;
			};
			putcdr (locale, NULLP);
			lista_arg = lstcat (lista_arg, locale);
		};
	};

	if (! is_errore) {			/* R = corpo della procedura */
		is_per = TRUE;
		R = NULLP;
		dev_in = dev_recupera;
		while (! _fineleggi (dev_in)) {
			token = analisi ();
			if (! is_per) {
				if (liv_analisi != 0) err2 (18, arg_1); /* [ e ] non bilanciate */
				token = cdr (token);
				break;
			};
			R = lstcat (R, token);
			token = NULLP;
		};
		if (! is_errore) {
			_def (lista_arg, R);
			if (dev_recupera == CONSOLE) msg (10, arg_1);
		};
		dev_in = dev_leggi;
	};
	is_per = FALSE;
}

void DEFINISCI ()
{
	node lista_arg, corpo;
	if (cercav (arg_1, SFUN) != NULLP)
		err2 (6, arg_1);
	else if (_coperto (cercav (arg_1, UFUN)))
		err2 (9, arg_1);
	else if (	(lstlen (arg_2) == 2)
			 && (typ (lista_arg = car (arg_2)) == LIST)
			 && (typ (corpo = car (cdr (arg_2))) == LIST))
		_def (lista_arg, _mult (DEL_FINELINEA, corpo));
//> 990429
	else if (	(lstlen (arg_2) >= 1)
			 && (typ (lista_arg = car (arg_2)) == LIST))
	{
		corpo = cdr (arg_2);
		_def (lista_arg, _mult (DEL_FINELINEA, corpo));
	}
//<
	else err_arg2 ();
}


/*************************************************************
crea o modifica la definizione di una procedura di nome arg_1
*************************************************************/
void _def (node lista_arg, node corpo)
// node lista_arg, corpo;
{
	node definizione, procedura;
	if (! is_demo) {
		definizione = makeuf (lista_arg, corpo);
		if ((procedura = cercav (arg_1, UFUN)) == NULLP)
			mettio (arg_1, definizione, UFUN);
		else putcar (procedura, definizione);
		recu_pac ();
	};
}

void TESTO ()
{
	if ((risultato = cercav (arg_1, UFUN)) == NULLP)
		err2 (2, arg_1);
	else risultato = car (risultato);
}

void COPIADEF ()
{
	if (cercav (arg_1, SFUN) != NULLP)
		err2 (6, arg_1);
	else if (_coperto (cercav (arg_1, UFUN)))
		err2 (9, arg_1);
	else if ((oggetto = cercav (arg_2, SFUN)) != NULLP)
		mettio (arg_1, car (oggetto), SFUN);
	else if ((oggetto = cercav (arg_2, UFUN)) != NULLP) {
//		if (bit_tipo (oggetto, BIT_COMPILA))
//			err2 (50, arg_1);
//		else
			mettio (arg_1, car (oggetto), UFUN);
	}
	else err2 (2, arg_2);
}
 

void _RINOMINA ()
{
	COPIADEF ();
	if (! is_errore)
		canceo (oggetto);
}

/*-------------------------------------------------------------------------
se la definizione (di variabile o procedura) e' eseguita come conseguenza
di recupera, non da procedura e non in una lista di comandi, e RECUPERA
aveva il 2" argomento (nome di pacchetto), mette la parola nel pacchetto
-------------------------------------------------------------------------*/
void recu_pac ()
{
	if (   (liv_esecuzione == 0)			/* non siamo in ACCHIAPPA */
		&& (liv_procedura == in_liv_proc)		/* def puo' der. da CONSERVA */
		&& (id_contesto != NULLP)			/* era specificato pacchetto */
		&& (id_contesto != CT_PAUSA)		/* non siamo in PAUSA */
		) _pac (arg_1, id_contesto);
}

/*---------------------------------------------------------------------
cancella dallo spazio di lavoro le primitive i cui nomi sono in arg_1
---------------------------------------------------------------------*/
void CANCEPRIM ()
{
	node oggetto;
	if (tipo_1 == LWORD) {
		if ((oggetto = cercav (mettip (arg_1), SFUN)) != NULLP)
			canceo (oggetto);
		else
			err2 (2, arg_1);
	}
	else while (arg_1 != NULLP) {
		if ((oggetto = cercav (mettip (car (arg_1)), SFUN)) != NULLP)
			canceo (oggetto);
		arg_1 = cdr (arg_1);
	}
}

/* ILDEF.C */
