/*ILANALIS.C		G. Toffoli

000511	spostato qui, da ilcomune.cpp: s_input, s_token, s_linea
		sostituito MAXCHARS con MAX_INPUT e MAX_LINE
000113	modificato RemoveComment
991012	aggiunto costrutto simmetrico di quote (letteralizzazione)
		riutilizzando stato 3, eliminato stato 4 (entrambi inattivi da 10 anni !!)
990430	aggiunto pc_END come sinonimo di pc_FINE
		per ovviare a lacuna in traduzione automatica
990429	modificato gestione di TILDE per compatibilità MswLogo:
		aggiunto 1 classe (colonna) a matrice lessicale
980904	analisi di array
980831	interfaccia _parse, cputs_ : char * -> const char *
980828	preparazione per conversione a C++
		sostituito min e max con __min e __max
980210	iniziato pulizia per WIN32; persa compatib. DOS e WIN16
		modificato _parse per gestire linee multiple
980107	mod. LEGGILISTA (ma perche' dev_in non e' OK ?)
971221	messo sollecita sotto controllo di is_prompt (nuova variabile)
970316	tentato recupero update del 02.10.96 (normalizzazione)
960723	in leggi_linea tolto eco a cgets_
960507	deivato parse da analisi
960409	primo port sotto Windows da Vers. DOS del 10.02.89
*/

#include "stdafx.h"

#define ILWIN
#define NEW_QUOTE

#define ATOMO		0
#define NUOVALINEA	1
#define NUOVALISTA	2
#define NUOVOARRAY	3	// 980904

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

extern int is_prompt;
#include "assert.h"
extern int uno;
#define trap(exp) \
( (exp) ? (void) 0 : uno/0 )
#define VALID(n) trap((n >= 0L) && (n < tot_nodi))

int MatchKw (node, node);

#define TILDE	'~'
#define COMMENT ';'

#define pc_END		kw [80]

//>990429
	
#ifdef NEW_QUOTE
// #define LQUOTE	'«'
// #define RQUOTE	'»'
#define LQUOTE	171		// '«'
#define RQUOTE	187		// '»'

int isQuoted;

#define N_STATI		5
#define N_CLASSI	9

#else // NEW_QUOTE

#define N_STATI		6
#define N_CLASSI	7

#endif // NEW_QUOTE

#define SIZE_MAT_CELL 2
#define SIZE_MAT_ROW (SIZE_MAT_CELL * N_CLASSI)

//> 000511
#define MAX_INPUT	(64 * K - 2)
#define MAX_TOKEN (4 * K)
char	s_input [MAX_INPUT+1],	/* stringa da input MLOGO */
		s_linea [MAX_INPUT+1],	/* ultima linea digitata */
		s_token [MAX_TOKEN+1];	/* stringa di token */
//<

int is_tilde;
//<

#ifdef NUOVOARRAY
int	stk2_analisi [20];	/* 2' stack di camm. att. in albero di analisi */
#endif // NUOVOARRAY

node	lista_parsed;	/* lista di token ottenuta da analisi di linea */
int 	is_tok_delimitatore;
char	*p_linea;	/* puntatore corrente a stringa linea di input */


					/*--------------------------------------------------------------------------
					matrice di transizione ed esecuzione dell' automa lessicale : l' elemento
					corrispondente allo stato corrente e alla classe del carattere in ingresso
					contiene numero della routine semantica e nuovo stato (negativo se finale)
--------------------------------------------------------------------------*/
char mat_lessicale [N_STATI * N_CLASSI * SIZE_MAT_CELL] = {

	/* ingresso :		  spazio   altro	 D		 "       \     linea	tilde	«		»	*/
	/* stato : */
	
#ifdef NEW_QUOTE
	
	/* 0 : inizio */		2, 0,	4, 1,	3,10,	5, 2,	2, 4,	3,10,	7,10,	8,3,	4,1,
	
	/* 1 : parola */		0,10,	4, 1,	0,10,	0,10,	2, 4,	0,10,	0,10,	4,1,	4,1,
	
	/* 2 : par. lett.  */	6, 0,	4, 1,	4, 1,	4, 1,	2, 4,	6, 0,	4,1,	4,1,	4,1,
	
	/* 3 : str. lett.  */	4, 3,	4, 3,	4, 3,	4, 3,	4, 3,	4,3,	4,3,	4,3,	2,10,
	
	/* 4 : \ in parola */	4, 1,	4, 1,	4, 1,	4, 1,	4, 1,	0,10,	4,1,	4,1,	4,1,

#else // NEW_QUOTE
	
	/* 0 : inizio */		2, 0,	4, 1,	3,10,	5, 2,	2, 5,	3,10,	7,10,
	
	/* 1 : parola */		0,10,	4, 1,	0,10,	0,10,	2, 5,	0,10,	0,10,
	
	/* 2 : par. lett.  */	6, 0,	4, 1,	4, 1,	4, 1,	2, 5,	6, 0,	4,1,
	
	/* 3 : str. lett.  */	4, 3,	4, 3,	4, 3,	4, 3,	2, 4,	0,10,	7,10,
	
	/* 4 : secondo \   */	0,10,	0,10,	0,10,	0,10,	4, 3,	0,10,	7,10,
	
	/* 5 : \ in parola */	4, 1,	4, 1,	4, 1,	4, 1,	4, 1,	0,10,	4,1

#endif // NEW_QUOTE
};

#ifdef OLD_COMMENT

void RemoveComment (char *s, int *sLength)
{
	while (*s != '\0') {
		if (*s == COMMENT) {
			*s = '\0';
			*sLength = strlen (s);
			break;
		}
		++s;
	}	
}

#else // OLD_COMMENT

void RemoveComment (char *s, int *sLength)
{
	int isComment = FALSE;
	char c;
	while (((c = *s) != '\0') && (c != '\r') && (c != '\r')) {
		if (isComment)
			*s = ' ';
		else
			if (*s == COMMENT) {
				*s = ' ';
				isComment = TRUE;
			}
		++s;
	}	
}

#endif // OLD_COMMENT

/*------------------------------------------
derivata da analisi() per versione Windows
--------------------------------------------*/
node _parse (const char *s)	// 980831
{
#define MULTILINE

#ifdef MULTILINE
	char lastChar;
#endif // MULTILINE

	char *f_linea;
	int l_linea, esito_bcd, is_ini_linea, is_fine;
	node bcd;
//	node parola;
	
	int save_p_sv = p_sv;	// Non viene svuotato bene lo stack dei valori ?
	
	is_analisi = TRUE;
	is_prima_linea = TRUE;
	is_fine = FALSE;
	liv_analisi = 0;
	lista_parsed = p_cella = NULLP;
	tipo_cella = NUOVALINEA;

#ifndef MULTILINE
	strncpy (s_input, s, MAX_INPUT);
#else // MULTILINE
	do {
		p_linea = &s_input [0];
		while (((lastChar = *(s++)) != NULLC) && (lastChar != '\n'))
			*(p_linea++) = lastChar;
		*p_linea = NULLC;
#endif // MULTILINE

		l_linea = __min(MAX_INPUT, strlen (s_input));
		RemoveComment (s_input, &l_linea);
		
		p_linea = &s_input [0];
		f_linea = p_linea + l_linea;
		*(p_linea-1) = NULLC;
		
		is_prima_linea = FALSE;
		is_ini_linea = TRUE;
		delimitatore [0] = 0;
		while (p_linea < f_linea) { 	// processa tutti i tokens
			libera_2 ();
//			while ((*p_linea == ' ') || (*p_linea == TAB) || (*p_linea == TILDE)) ++p_linea;
			while ((*p_linea == ' ') || (*p_linea == TAB))
				++p_linea;
			if (   ((! IS_OPERATORE(*p_linea)) || (IS_INI_ESPR(*(p_linea-1))))
				&& ((esito_bcd = edi_num (p_linea, &bcd)) > 0)
				) {
				p_linea += (esito_bcd - 1);
				nuovo_token ();
				putcar (p_cella, bcd);
			}
			else {
				isola_token ();
				if (is_tok_delimitatore)
				{
					if (delimitatore [0] == '[') {
						if (! is_leggiparola) {
							nuovo_token ();
							stk_analisi [liv_analisi++] = p_cella;
							tipo_cella = NUOVALISTA;
						};
					}
					else if (delimitatore [0] == ']') {
						if (! is_leggiparola) {
							if (   (liv_analisi == 0)
#ifdef NUOVOARRAY
								|| (typ (stk2_analisi [liv_analisi]) != LIST)
#endif // NUOVOARRAY
								) msg (4);
							else {
								p_cella = stk_analisi [--liv_analisi];
								tipo_cella = ATOMO;
							};
						};
					}
#ifdef NUOVOARRAY
					else if (delimitatore [0] == '{') {
						if (! is_leggiparola) {
							nuovo_token ();
							stk_analisi [liv_analisi++] = p_cella;
							tipo_cella = NUOVOARRAY;
						};
					}
					else if (delimitatore [0] == '}') {
						if (! is_leggiparola) {
							node array = stk2_analisi [liv_analisi];
							if (   (liv_analisi == 0)
								|| (typ (array) != ARAY)
								) msg (4);
							else {
								node size = consreal ((double) lstlen (cdr (array)));
								putcar (car (array), size);
								p_cella = stk_analisi [--liv_analisi];
								tipo_cella = ATOMO;
							};
						};
					}
#endif // NUOVOARRAY
				else {				/* ":()+-* /<=>^ */
						nuovo_token ();
						putcar (p_cella, (node) strcspn ((char *)&s_del,
							(char *)&delimitatore));
					}
				}
				else {			/* token e' parola non delimitatore */
					nuovo_token ();
#ifdef NEW_QUOTE
					if (isQuoted) {
						node stringa = logstr (s_token);
						putcar	(p_cella, stringa);
					}
					else {
						node parola;
#endif // NEW_QUOTE
//>990430
//					is_fine = MatchKw ((parola = metti_sc (s_token)), pc_FINE) && is_ini_linea;
					parola = metti_sc (s_token);
					is_fine = is_ini_linea &&
							(MatchKw (parola, pc_FINE) || MatchKw (parola, pc_END));
//<
					putcar	(p_cella, parola);
#ifdef NEW_QUOTE
					}
#endif // NEW_QUOTE
				}
				is_ini_linea = FALSE;
			}
			if (is_per && is_fine) {
				is_per = FALSE;
				break;
			}
		}
		
#ifndef MULTILINE
#else // MULTILINE
	} while (lastChar != NULLC);	
#endif // MULTILINE
	
	p_sv = save_p_sv;	// ripristina lo stack dei valori
	//	is_analisi = FALSE;
	VALID(lista_parsed);
	return lista_parsed;
}

/*-------------------------------------------------------
legge una linea di comando da console, ne fa l' analisi
e costruisce la lista di token per EXEC o altro master
---------------------------------------------------------*/
node analisi ()
{
	char *f_linea;
	int l_linea, resto, n, esito_bcd, is_ini_linea, is_fine;
	node bcd;
//	node parola;
	
	is_analisi = TRUE;
	is_prima_linea = TRUE;
	is_fine = FALSE;
	liv_analisi = 0;
	lista_parsed = p_cella = NULLP;
	tipo_cella = NUOVALINEA;
	do {
		p_linea = &s_input [0];
		*(p_linea-1) = NULLC;
		f_linea = (l_linea = leggi_linea (s_input, MAX_INPUT, 0)) + p_linea;
		is_prima_linea = FALSE;
		is_ini_linea = TRUE;
//>990429
		is_tilde = FALSE;
//<
		if ((lista_prog != NULLP) || (l_linea == 0)) break;
		delimitatore [0] = 0;
		while (p_linea < f_linea) { /* processa tutti i tokens */
			libera_2 ();
//			while ((*p_linea == ' ') || (*p_linea == TAB) || (*p_linea == TILDE)) ++p_linea;
			while ((*p_linea == ' ') || (*p_linea == TAB))
				++p_linea;
			if (   ((! IS_OPERATORE(*p_linea)) || (IS_INI_ESPR(*(p_linea-1))))
				&& ((esito_bcd = edi_num (p_linea, &bcd)) > 0)
				) {
				p_linea += (esito_bcd - 1);
				nuovo_token ();
				putcar (p_cella, bcd);
			}
			else {
				isola_token ();
//>990429
//<
				if (is_tilde)
					p_linea = f_linea;
				else
//<
				if (is_tok_delimitatore)
				{
					if (delimitatore [0] == '[') {
						if (! is_leggiparola) {
							nuovo_token ();
							stk_analisi [liv_analisi++] = p_cella;
							tipo_cella = NUOVALISTA;
						};
					}
					else if (delimitatore [0] == ']') {
						if (! is_leggiparola) {
							if (   (liv_analisi == 0)
#ifdef NUOVOARRAY
								|| (typ (stk2_analisi [liv_analisi]) != LIST)
#endif // NUOVOARRAY
								) msg (4);
							else {
								p_cella = stk_analisi [--liv_analisi];
								tipo_cella = ATOMO;
							};
						};
					}
#ifdef NUOVOARRAY
					else if (delimitatore [0] == '{') {
						if (! is_leggiparola) {
							nuovo_token ();
							stk_analisi [liv_analisi++] = p_cella;
							tipo_cella = NUOVOARRAY;
						};
					}
					else if (delimitatore [0] == '}') {
						if (! is_leggiparola) {
							node array = stk2_analisi [liv_analisi];
							if (   (liv_analisi == 0)
								|| (typ (array) != ARAY)
								) msg (4);
							else {
								node size = consreal ((double) lstlen (cdr (array)));
								putcar (car (array), size);
								p_cella = stk_analisi [--liv_analisi];
								tipo_cella = ATOMO;
							};
						};
					}
#endif // NUOVOARRAY
					else {				/* ":()+-* /<=>^ */
						nuovo_token ();
						putcar (p_cella, (node) strcspn ((char *)&s_del,
							(char *)&delimitatore));
					}
				}
				else {			/* token e' parola non delimitatore */
					nuovo_token ();
#ifdef NEW_QUOTE
					if (isQuoted) {
						node stringa = logstr (s_token);
						putcar	(p_cella, stringa);
					}
					else {
						node parola;
#endif // NEW_QUOTE
//>990430
//					is_fine = MatchKw ((parola = metti_sc (s_token)), pc_FINE) && is_ini_linea;
					parola = metti_sc (s_token);
					is_fine = is_ini_linea &&
							(MatchKw (parola, pc_FINE) || MatchKw (parola, pc_END));
//<
					putcar	(p_cella, parola);
#ifdef NEW_QUOTE
					}
#endif // NEW_QUOTE
				};
			};
			is_ini_linea = FALSE;
			if (   (l_linea == MAX_INPUT)
				&& (!IS_DEL_LINEA(*(f_linea-1)))
				&& ((n = p_linea - &s_input [0]) > MAX_INPUT/2)
				) {
				memcpy (&s_input [0] - 1, p_linea - 1, (resto = MAX_INPUT - n) + 1);
				f_linea =	(p_linea = &s_input [0])
					+ (l_linea = resto + leggi_linea (f_linea - n, n, 1));
			};
		};
		if (is_per && is_fine) {
			is_per = FALSE;
			break;
		};
//>990429
//	} while (liv_analisi > 0);
	} while ((liv_analisi > 0) || is_tilde);
//<
	if (lista_parsed != NULLP) pop_sv ();
	else if (lista_prog != NULLP) {
		lista_parsed = lista_prog;
		lista_prog = NULLP;
	};
	is_analisi = FALSE;
	VALID(lista_parsed);
	return lista_parsed;
}

/*---------------------------------------------------------------
alloca una cella cons, aggancia la cella alla lista
di token costruita da readline; a seconda che la cella
rappresenti il primo token di una nuova sottolista (NUOVALISTA)
oppure un nuovo elemento della lista corrente (regular_token)
l' aggancio avviene in modi diversi
---------------------------------------------------------------*/
void nuovo_token ()
{
	node locale;
#ifdef NUOVOARRAY
	node array, size, start;
#endif // NUOVOARRAY
	locale = cons (NULLP, NULLP, LIST);	/*alloca nuova cella di tipo default*/
	switch (tipo_cella) {
	case ATOMO:							/* seguito della lista corrente */
		putcdr (p_cella, locale);
		break;
	case NUOVALINEA:					/* inizio parsed_list */
		push_sv (lista_parsed = locale);
		break;
	case NUOVALISTA:					/* inizio di lista */
		putcar (p_cella, locale);
#ifdef NUOVOARRAY
		stk2_analisi [liv_analisi] = locale;
		break;
	case NUOVOARRAY:					/* inizio di array */
		size = consreal ((double) 0);
		start = consreal ((double) 1);
		array =
			cons (cons (size, cons (start, NULLP, LIST), LIST), locale, ARAY); 
		putcar (p_cella, array);
		stk2_analisi [liv_analisi] = array;
		break;
	default: ;							/* AGGIUNGERE GESTIONE ERRORE */
#endif // NUOVOARRAY
	}
	p_cella = locale;					/* cella corrente = nuova cella */
	tipo_cella = ATOMO; 				/* per default */
}

/*---------------------------------------------------
riconosce un token nel buffer di lettura s_input
e lo trasferisce carattere per carattere in s_token;
ritorna la lunghezza del token
---------------------------------------------------*/
void isola_token ()
{
#ifdef NEW_QUOTE
	isQuoted = FALSE;
#endif // NEW_QUOTE
	unsigned char c, *p_token;
	int i, stato, azione;
	is_tok_delimitatore = FALSE;
	stato = 0;
//> 980828
//	p_token = &s_token [0];
	p_token = (unsigned char *) &s_token [0];
//<
	while (stato <= 5) {
//>990429
//		azione = mat_lessicale [(i=stato*12 + (classe_lessicale (c=*p_linea) << 1))];
		i = stato * SIZE_MAT_ROW + (classe_lessicale (c = *p_linea)) * SIZE_MAT_CELL;
		azione = mat_lessicale [i];
//<
		stato =  mat_lessicale [i + 1];
		switch (azione) {
		case 0 :			/* non fa niente */
			break;
		case 1 :			/* inizio stringa letterale : */
		case 2 :			/* scarta carattere */
			++p_linea;
			break;
		case 3 :			/* riconosciuto delimitatore : */
			is_tok_delimitatore = TRUE;
			delimitatore [0] = c;
		case 4 :			/* copia carattere */
			*(p_token++) = c;
			++p_linea;
			break;
		case 5 :			/* consuma '"' e crea delimitatore */
			nuovo_token ();
			if (c == '"') putcar (p_cella, DEL_VIRGOLETTE);
			else putcar (p_cella, DEL_DUEPUNTI);
			++p_linea;
			break;
		case 6 :			/* consuma ' ' e crea parola vuota */
			nuovo_token ();
			putcar (p_cella, mettip (S_VUOTA));
			break;
//>990429
		case 7 :			/* consuma resto di linea e finelinea */
			is_tilde = TRUE;
			break;
//<
#ifdef NEW_QUOTE
		case 8 :			/* consuma « e crea .. */
			++p_linea;
			isQuoted = TRUE;
			break;
#endif // NEW_QUOTE
		};
	};
	*p_token = NULLC;
}

/*--------------------------------------------------------------------
classifica un carattere ai fini dell' analisi lessicale utilizzando
alcuni codici di confronto "cablati" localmente e la classificazione
piu' generale dei caratteri contenuta in MLCODICI.ASM
--------------------------------------------------------------------*/
int classe_lessicale (int c)
{
	int classe;
	c = c & 0xFF;
	switch (c) {
	case ' ' :		/* spazio */
	case TAB :		/* TAB */
//	case TILDE :		/* ~ */	990429
		classe = 0;
		break;
	case '"' :		/* inizio di parola letterale */
	case ':' :		/* inizio di nome di variabile */
		classe = 3;
		break;
	case '\\' : 	/* "\" = inizio e fine di stringa letterale */
		classe = 4;
		break;
	case LINEDEL :	/* delimitatore di linea */
	case COMMENT :	/* semicolon */
		classe = 5;
		break;
//>990429
	case TILDE :		/* ~ */
		classe = 6;
		break;
//<
#ifdef NEW_QUOTE
	case LQUOTE :		/* « */
		classe = 7;
		break;
	case RQUOTE :		/* » */
		classe = 8;
		break;
#endif // NEW_QUOTE
	default :
		if (IS_DELIMITATORE(c)) classe = 2; 	/* delimitatore */
		else classe = 1;				/* tutti gli altri caratteri */
	};
	return classe;
}

/*------------------------------------------------------
costruisce la stringa di prompt e la scrive su console
------------------------------------------------------*/
void sollecita ()
{
	int i;
	char s_locale [512];
	if (is_prompt) {
		s_locale [0] = NULLC;
		for (i = 1; i <= liv_contesto; ++i) {
			if ((contesti [i])._id_contesto == CT_PAUSA) {	/* test su id del contesto (0 e' ris.) */
				pacstr (&(s_locale [strlen (s_locale)]),
					nome (stk_funzioni [(contesti [i])._liv_procedura]) /* proc. interr. */
					);
				strcat (s_locale, " ");
			};
		};
		strcat
			(s_locale, 
			((liv_funzione > in_liv_funzione) || (liv_analisi > 0)) ? ">" : "?"
			);
		cputs_ (s_locale);
	}
}

/*--------------------------------------------------------------------------
mette una linea di input in s gestendo il prompt e l' eventuale eco;
ritorna il numero di caratteri letti ( = 0 se fine file );
gestisce anche la bufferizzazione nel caso di linea piu' lunga di n :
se linea non e' finita legge n caratteri;
se i > 0 e' richiesta lettura di continuazione della linea;
--------------------------------------------------------------------------*/
int leggi_linea (char *s, int n, int i)
{
	int l_linea, locale, dev;
	if (dev_in == CONSOLE) {
		sollecita ();
		l_linea = cgets_ (s, n);
		if (lista_prog != NULLP) {
			locale = dev_out;
			dev_out = CONSOLE;
			if (is_eco) {
				_scrivi (lista_prog);
				a_capo ();
			};
			dev_out = locale;
		}
		else if (l_linea > 0) {
			strcpy (s_linea, s);
			s_linea [strlen (s_linea) - 1] = NULLC;
		};
	} else {
		
		dev = dev_in;			/* equipara fogli a FOGLIO */
		if (_fstato [dev] & O_FOGLIO) dev = FOGLIO;
		switch (dev) {
			
		case FOGLIO:
			l_linea = bgets (dev_in, s, n);
			break;
#ifdef ILDOS
		case COM1:
		case COM2:
			l_linea = comgets (s, dev_in - COM1, n);
			break;
#endif // ILDOS
			
		default:				/* da file */
			l_linea = fgets_ (s, n, _file [dev_in], _fstato [dev_in]);
		};
		if ((is_eco) && (l_linea != 0)) {
			if (i != 0) sollecita ();
			cputs_ (s);
		};
	};
	RemoveComment (s, &l_linea);
	return l_linea;
}

void LEGGILISTA ()
{
	/*>980107*/
	int save_dev = dev_in;
	dev_in = dev_leggi;
	/*<*/
	if ((risultato = analisi ()) != NULLP)
		if (_ult (risultato) == DEL_FINELINEA)
			risultato = _menult (risultato);
		/*>980107*/
		dev_in = save_dev;
		/*<*/
}

void LEGGIPAROLA ()
{
	is_leggiparola = TRUE;
	LEGGILISTA ();
	is_leggiparola = FALSE;
	if (risultato == NULLP) risultato = mettip (S_VUOTA);
	else risultato = car (risultato);
}

void PARSE ()
{
	char s[MAX_INPUT];
	getnam (s, arg_1);
	risultato = _parse (s); 
}

void RUNPARSE ()
{
	if (typ (arg_1) == LIST) {
		risultato = NULLP;
	} else {
		PARSE ();
	}
}

void ULTLINEA ()
{
	risultato = logstr (s_linea);
}

/* ILANALIS.C */
