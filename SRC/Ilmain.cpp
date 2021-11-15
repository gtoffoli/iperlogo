/* ILMAIN.C		G. Toffoli

020603-020604 in main_loop aggiunto catch di eccezioni
001227	commentato "is_lib = FALSE"
000119	iniziato gestione di N_MASSIMO
991221	eliminato breakpoint (BreakOnDebug) 
991206	introdotto ini_liv_contesto locale a main_loop, argomento a valuta_token
991126	ritolto ini_exec da ClearVar e messo solo dove opportuno
991126	simulato breakpoint (BreakOnDebug) in gestione errore: ClearVar
		corretto errore()
991124	controllo di non rientranza e gestione cursore in NestedExec
991102	ritoccato ancora uscita da errore
991013	corretto tracciamento di errore (?)
991009	pulizia; ritoccato return da recupero a NULL_DEV (NestedExec)
		commentato inner_loop; 4 volte agg.cast (int) a n_arg_trovati
990714	aggiunto _letterale per implem. di PR_QUOTED (LETTERALE) 
990310	in uf_call / uf_ret, controllo residuo in stack dei valori
		ritocco in stampa errore
980904	valutazione di array
980903	chiusura morbida, con ConfirmExit, in IperlogoExit
		RIVEDERE IS_CIAO : while (! is_ciao) .. ha piu' senso ?
980902	corretto err_arg1 e err_arg2 (da quando erano sbagliati ?)
980901	in valuta, a fine esecuzione comando, scrittura su CONSOLE
		(anziche' su device corrente) degli argomenti rimasti sullo stack
980831	interfaccia _parse, execute: char * -> const char *
980828	preparazione per conversione a C++
98020	riattivato IperlogoExit da CIAO
980127	in MAIN_LOOP corretto (?) recupero da NULL_DEV
971229	In look-ahead tolto baco introdotto il 971216 (pulizia del codice)
		Disattivato CIAO
971221	Iniziato riordinamento inizializzazioni; commentato LabelExec
		Seria modifica per NestedExec (recupera da NULL_DEV)
971216	Pulizia del codice
971204	Introdotto LabelExec () per commutazione di Esegui <-> Halt
960409	prima vers. Windows da Iperlogo del 06.06.89
*/

#include "stdafx.h"
 
#define ILWIN
 
#define VERBO      1
#define LETTERALE  2
#define VALORE     3 

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include "ilsfun.h"
extern struct primitiva descr_sf;

#include "windows.h"
			/* codifica dei tipi di contesto (id_contesto) */
#define CT_EVENT	3L	/* contesto attivato da richiesta asincrona */
#define NULL_DEV	-1

int is_prompt;			// prompt dei comandi
int is_eco_interactive;	// eco di comandi da tastiera o mouse
int is_yield = TRUE;
int	is_nestedExec = FALSE;	// 991124
node _parse (const char *);	// 980831
void EventCheck ();

extern void IperlogoExit ();
extern int PauseLevel ();
void BreakOnDebug();

void EventCheck ()
{
	MSG msg;
	if(PeekMessage(&msg, (HWND) NULL, 0, 0, PM_NOREMOVE))
		if(GetMessage(&msg, (HWND) NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
}

void YIELD ()
{
	is_yield = TRUE;
}

void NOYIELD ()
{
	is_yield = FALSE;
}

void _letterale ()
{
	mod_parola = LETTERALE;		/* parola non preceduta da modificatore*/
}

void ini_valuta ()			// inizializzazione parziale di Commander (NestedExec)
{
	funzione = NULLP;		/* nessuna funzione incontrata */
	n_arg_attesi = 0;		/* numero di parametri atteso dalla funzione corrente*/
	n_arg_trovati = 0;		/* numero di oggetti sullo stack per la fun corrente*/
	conto_parentesi = 0;
 	parentesi = -1;			/* = liv_funzione se sfun corr. e' preceduta da "("*/
	is_stop = FALSE;		/* se vero e' terminata esecuz. procedura corrente */

	mod_parola = VERBO;		/* parola non preceduta da modificatore*/
	is_finito = FALSE;		/* se vero ritorna al toploop di MLOGO*/
}

void ini_exec ()			// inizializzazione quasi totale di Commander
{
// prima parte (da main_ini)
	is_locale = FALSE;
	is_acchiappa = FALSE;
	is_lancia = FALSE;
	is_pausa = FALSE;
	is_continua = FALSE;
	is_recupera = FALSE;
	is_vai = FALSE;

// seconda parte (da main_loop) :
	CHIUDITUTTO ();
	p_sc = 0;
	ini_p_sv = p_sv = DIM_LSTACK-1;
	is_fatal = FALSE;		/* flag di errore fatale (disco/dischetto) */
	is_errore = FALSE;		/* flag di errore */
	is_per = FALSE;			/* "in corso def procedura" */
	is_leggiparola = FALSE;	/* non e' in corso esecuzione di LEGGIPAROLA */
	is_letterale = FALSE;	/* stampa parole senza simbolo car. letter. */
	l_musica = NULLP;		/* musica non attiva */

	val_verifica = FALSE;	/* valore ultima condizione verificata */
	liv_contesto = 0;		/* livello di nest dei contesti */
	id_contesto = 0;		/* contesto "top_level" */
	in_liv_funzione =		/* val. in. liv_funzione in PAUSA/RECUPERA */
	liv_funzione = 0;		/* nest globale delle funzioni */
	in_liv_proc =			/* val. in. liv_procedura in PAUSA/RECUPERA */
	liv_procedura = 0;		/* nest globale delle procedure utente */
	liv_esecuzione = 0;		/* nest dei blocchi in proc. corrente */

	n_locali = 0;			/* n. var. locali definite a livello nest corrente */
	token = NULLP;
	dev_in = dev_leggi = dev_out = dev_scrivi = dev_recupera = CONSOLE;

	ini_valuta ();

// terza parte (aggiunte) :
	liv_analisi = 0;		/* # parentesi non chiuse */

	is_nestedExec = FALSE;	// 991124
}

HCURSOR hWaitCursor;

void main_ini ()		// inizializzazioni specifiche
{
						// inizializzazioni prima mancanti ?
//	is_lib = FALSE;
	is_debug = FALSE;
	is_demo = FALSE;
	is_ciao = FALSE;
	is_eco = FALSE;			// eco di comandi batch
	is_eco_interactive = TRUE;	// nuovo: 971221
	is_prompt = FALSE;		// nuovo: 971221
	is_traccia = FALSE;

	ini_main ();			// inizializzazioni varie in modulo ILINIC
	ini_exec ();

	hWaitCursor = LoadCursor (AfxGetInstanceHandle(), IDC_WAIT);
}

void main_fin ()			// finalizzazioni specifiche
{
	rilascia_mem ();		/* rilascia memoria espansa */
}

/*--------------------------------------------------------------------
  per consentire un ritorno piu' ordinato al livello comandi di MLOGO,
  elimina i valori delle variabili locali;
  971221: ora fa tutte le operazioni necessarie dopo errore o break
  --------------------------------------------------------------------*/
void ClearVar ()
{
	node ult, prec;
	prec = NULLP;
	while (p_sv != ini_p_sv) {
//>991126
//		if (typ (ult = pop_sv ()) == LVAR)
//			putcar (ult, prec);
		ult = pop_sv ();
		int tipo = typ (ult);
		if (tipo == LVAR) {
			putcar (ult, prec);
//			BreakOnDebug();
		}
//<
		prec = ult;
	};
//>991126
//	ini_exec ();		// 971221
//<
}

//>020604
extern void abbassaSogliaNodi();
void main_loop_catch ()
{
	ClearVar ();
	ini_exec ();
	linea_com = NULLP;
	n_argomenti = 0;
    LIBERA ();
}
//<020604

void main_loop (int fatal)
{
//>020603 020604
try
{
//<020603 020604
//>991206
	void valuta_token (int ini_liv_contesto);
	// serve per pop dei contesti in caso di errore
	int ini_liv_contesto = liv_contesto;
	// nel caso di NestedExec, c'è un push di contesto
	// da compensare
	if (id_contesto == CT_EVENT)
		--ini_liv_contesto;
//<991206
static int resume;
	int was_break;

	resume = PauseLevel ();	// FALSE;	971210 
/*>971217
	if (resume) goto resume_exec;
*/
	if (resume && (! is_break)) goto resume_exec;
/*<*/
	is_yield = TRUE;
	was_break = is_break;
	is_break = FALSE;		/* reset di richiesta di break */
	is_conin = FALSE;		/* non e' in corso input di carattere */
	if (was_break) {		/* risposta a break da tastiera */
		msg (5);
		ClearVar ();		// aggiunto 971217
		ini_exec ();		// aggiunto 991126 (tolto da ClearVar)
		return;
	}		/* risposta a break da tastiera */

/*>980127*/
	if (dev_recupera == NULL_DEV)
			goto resume_exec;
/*<*/

	while (! is_ciao) {		/* finche' non esegue il comando "ciao" */
							/* analizza una linea di comando ... */
		if (dev_recupera == NULL_DEV) {	// aggiunto in ILWIN
			pop_contesto ();
//>991009
			is_finito = FALSE;	// certo !
			is_stop = FALSE;	// mi sembra vero
			mod_parola = VERBO;	// sempre vero ?
//<
			return;
		}
		if (_fineleggi (dev_recupera)) {	// 971221: modificato _fineleggi
			pop_contesto ();
			is_finito = FALSE;
			is_stop = FALSE;
			mod_parola = VERBO;
			goto rivaluta;
		};

// inner_loop:		// posticipato qui in ILWIN

		if (token == NULLP) {
			if (linea_com == NULLP) {
				dev_in = dev_recupera;
				if ((dev_in == CONSOLE) || (dev_in == NULL_DEV)) { // inserito in ILWIN
					resume = TRUE;
					return;
				}
				is_interprete = TRUE;
				linea_com = analisi ();

resume_exec:
				resume = FALSE;
				is_interprete = FALSE;
				dev_in = dev_leggi;
			};
    							/* ... e la interpreta */
			err_token = token = linea_com;/* token corrente */
			n_arg_attesi = 0;	/* numero di parametri atteso dalla funzione corrente*/
			n_arg_trovati = 0;	/* numero di oggetti sullo stack per la fun corrente*/
			conto_parentesi = 0;
			parentesi = -1;		/* = liv_funzione se sfun corr. e' preceduta da "("*/
			is_stop = FALSE;	/* se vero e' terminata esecuz. procedura corrente */
		};
    	mod_parola = VERBO;		/* parola non preceduta da modificatore*/
    	is_finito = FALSE;		/* se vero ritorna al toploop di MLOGO*/
		while (! is_finito) {
rivaluta:
//>991206
//			valuta ();			/*eseguito per ogni token*/
			valuta_token (ini_liv_contesto);	// eseguito per ogni token
//<991206
			if (is_errore)  {	// aggiunto 971210
				return;
			}
			if (is_yield) EventCheck ();	// aggiunto 971210
			if (is_break)  {				// aggiunto 971205
				is_break = FALSE;
				msg (5);
				return;
			}
		};
		linea_com = NULLP;
	};
//>020603 020604
}
catch( CUserException* e )
{
   // User already notified.
	e->Delete();
	main_loop_catch();
	abbassaSogliaNodi();
}
catch( CException* e )
{
    // For other exception types, notify user here.
	e->Delete();
	main_loop_catch();
    AfxMessageBox("Errore di sistema nel ciclo di valutazione");
}
//<020603 020604
}

void execute (const char *s)	// 980831
{
	linea_com = _parse (s);
/*> 971221 */
	if (liv_analisi > 0) {	/* parentesi non chiuse */
		msg (3);
		ClearVar ();		// aggiunto 971217
		ini_exec ();		// aggiunto 991126 (tolto da ClearVar)
		return;
	}
/*< */
	if (linea_com != NULLP)
		main_loop (FALSE);
}

void NestedExec (node lista)
{
	if (! lista) return;

	if (is_nestedExec) return;
	is_nestedExec = TRUE;
	HCURSOR cursor = SetCursor (hWaitCursor);

	pausa_recupera (CT_EVENT);	// include push del contesto e (re)inizializzazioni varie
	dev_recupera = NULL_DEV;
	ini_valuta ();
	linea_com = lista;
	main_loop (FALSE);			// include pop del contesto

	SetCursor (cursor);
	is_nestedExec = FALSE;
}

/*---------------
  valuta un token
  ---------------*/
//>991206
// void valuta ()
void valuta_token (int ini_liv_contesto)
//<991206
{
	int i, locale, tipo_funzione;
	node nodoloc, parola, oggetto, id_cont/*esto*/;
	char s_locale [128];
	int dev_locale;	// aggiunto 980901
	int was_errore;	// aggiunto 971210
	if (is_yield) EventCheck ();
	if (is_errore) goto ciclo_di_attivazione;
	if (token == NULLP) {		/* lista di token corrente e' finita */
		if (conto_parentesi > 0) {
			errore (14, 0L, 0L);
//			BreakOnDebug();
		}
		else
			if (((locale = typ (funzione)) == SFUN) || (locale == UFUN))
				goto ciclo_di_attivazione;
			else if (liv_esecuzione > 0) { 		/* fine di blocco */
				blk_out ();
				if (is_errore) goto ciclo_di_attivazione;
			}
			else {				/* fine di linea comandi o procedura */
				if (liv_procedura != in_liv_proc) 	/* fine corpo di procedura */
  					is_stop = TRUE;
				else {					/* fine linea di comando */
					is_finito = TRUE;
					if (n_arg_trovati > 0) {
						dev_locale = dev_out;			// 980901
						dev_out = CONSOLE;				// 980901
						for (i = n_arg_trovati; i >= 1; --i) {
							_mostra (get_sv (i)); a_capo ();
						};
						dev_out = dev_locale;		// 980901
/*>990310
						while (n_arg_trovati-- != 0) pop_sv ();
*/
						while (n_arg_trovati != 0) {
							pop_sv ();
							--n_arg_trovati;
						}
/*<*/
					};
				}
			}
	}
	else {			/*lista di token corrente non e' finita*/
		gettok ();
		switch (tipo_token) {
			case LIST:				/* inizio LISTA */
			case ARAY:				/* 980904: inizio ARRAY */
				token = cdr (token);
				push_arg (val_token);
				mod_parola = VERBO;
			break;  
			case LSTR:			/* stringa MLOGO */
			case LBCD:			/* numero BCD */
				if (mod_parola == VERBO) {
					token = cdr (token);
					push_arg (val_token);
					break;
				}
				else if (tipo_token == LSTR) val_token = mettip (val_token);
				else {
					edo_num (val_token, s_locale);
					val_token = mettip (logstr (s_locale));
				};
 			case LWORD:				/* parola MLOGO */
				token = cdr (token);
				locale = mod_parola;
				mod_parola = VERBO;
				switch (locale) {
					case LETTERALE: 
						push_arg (val_token);
					break;
					case VALORE:			/* parola con valore: var MLOGO */
						risultato = car (oggetto = cercav (val_token, LVAR));
						if ((oggetto == NULLP) || (risultato == NULLVALUE))
							errore (1, 0L, val_token);
 						else push_arg (risultato);
					break;
					case VERBO:			/* parola eseguibile */
						if ((parola = val_token) == DEL_FINELINEA) {
							if (typ (funzione) == SFUN) {		/* primitiva non eseguita */
								get_sf (funzione);
								if ((((unsigned) n_arg_trovati) >= N_MINIMO) && (parentesi != liv_funzione))
									sf_call ();
							}
						}
						else if ((IS_RIS(parola)) && (parola <= DEL_PARDESTRA))
							switch (parola) {
								case (int) DEL_VIRGOLETTE:
									mod_parola = LETTERALE;
								break; 
								case (int) DEL_DUEPUNTI:
									mod_parola = VALORE;
								break; 
								case (int) DEL_PARSINISTRA:
									parenin ();
									if (   (typ (next_val) == LWORD)
										&& (cercav (next_val, SFUN) != NULLP)
										) parentesi = liv_funzione + 1;
								break; 
								case (int) DEL_PARDESTRA:
									if (conto_parentesi == 0) {
										errore (14, 0L, 0L);
//										BreakOnDebug();
									}
									else if (parentesi == liv_funzione) {
										get_sf (funzione);
										if (((unsigned) n_arg_trovati) < N_MINIMO)
											errore (11, funzione, NULLP);
//> 000119
										else if ((! N_ILLIMITATO) && (((unsigned) n_arg_trovati) > N_MASSIMO))
											errore (11, funzione, NULLP);
//<
										else {
											f_call ();
											parenout (1);
										};
									}
									else {
										parenout (1);
									};
								break;
							}
						else if (IS_RIS(parola)) {		/* operatore infisso */
							val_token = cercav (parola, SFUN);
							sf_in ();
						}
						else if ((val_token = cercav (parola, UFUN)) != NULLP) {
							uf_in ();
						}
						else if ((val_token = cercav (parola, SFUN)) != NULLP) {
							sf_in ();
						}
 						else errore (2, 0L, parola);
					break;
				}  /* switch (mod_parola) */
			break;
			} /* switch tipo_token */
		}; /* if lista corrente non e' finita */
ciclo_di_attivazione:
	while ((! is_errore) && (mod_parola == VERBO)) {
		if (is_stop) {			/* fine valutazione ufun */
			uf_ret ();
/*>990310*/
			if (is_errore)
	break;
/*<*/
			next_val = ((next_token = token) == NULLP) ? NULLP : car (token);
		}
		else {				/* look ahead */
			if (   (n_arg_trovati > 0)
				&& (next_val >= DEL_PIU) && (next_val <= DEL_MAGGIORE)
				&& (   (typ (funzione) != SFUN)
					|| (! IS_RIS((parola=cercav(funzione,LWORD))))
					|| (p_del [next_val] > p_del [parola])
					)
				) {
					--n_arg_trovati;
					gettok ();
					token = cdr (token);
					val_token = cercav (val_token, SFUN);
					sf_in ();
					++n_arg_trovati;
					if (parentesi == liv_funzione) parentesi = -1;	/* coordinato con ...*/
				}
			else if (   ((tipo_funzione = typ (funzione)) == SFUN)
					&& get_sf (funzione)			/* sempre vero */
					&& (   (((unsigned) n_arg_trovati) >= N_NOMINALE)
						|| ((token == NULLP) && (((unsigned) n_arg_trovati) >= N_MINIMO))
						)
//> 000119
//					&& (   (! N_ILLIMITATO)
					&& (   ((! N_ILLIMITATO) && (! (((unsigned) n_arg_trovati) < N_MASSIMO)))
//<
						|| (parentesi < liv_funzione)				/* ... questo        */
						)
					) sf_call ();
			else if (   (tipo_funzione == UFUN)
					&& (n_arg_trovati >= n_arg_attesi)
					) uf_call ();
			else if (   (token == NULLP)
					&& ((tipo_funzione == SFUN) || (tipo_funzione == UFUN))
					)
				errore (11, funzione, NULLP);
			else return;
		};
	};
	if (is_errore) {
		was_errore = is_errore;	is_errore = FALSE;	// aggiunto 971210
		id_cont/*esto*/ = 0;
		for (i = liv_contesto; i > 0; --i) {
			if ((id_cont/*esto*/ = (contesti [i])._id_contesto) == CT_PAUSA)
				break;	// contesto 0 è riservato (??)
			else if (id_cont/*esto*/ == pc_ERRORE) {
				--i;
				break;
			};
		};
///>991013
///		spostato avanti
///		while (i != liv_contesto) pop_contesto ();
///<
		if (id_cont/*esto*/ != pc_ERRORE) {
				/* tracciamento dell' errore + break */
			dev_out = CONSOLE;
			_scrivi (car (nodoloc = cdr (ult_err)));
			a_capo ();
			if (liv_procedura != in_liv_proc) {
//				msgsenza (18);
				msgsenza (18, NULLP, NULLP);	// 980831
				for (i = in_liv_proc; i <= liv_procedura; ++i) {
					_scrivi (stk_funzioni [i]);
					spazio ();
				};
				a_capo ();
			};
			if ((liv_procedura != in_liv_proc) || (dev_recupera != CONSOLE)) {
//				msgsenza (19);
/*>990310
				msgsenza (19, NULLP, NULLP);	// 980831
				_scrivi (car (cdr (cdr (nodoloc))));
				a_capo ();
*/
				locale = car (cdr (cdr (nodoloc)));
				if (locale != NULLP) {
					msgsenza (19, NULLP, NULLP);
					_scrivi (locale);
					a_capo ();
				}
/*>990310*/
			};
			ult_err = NULLP;
			is_errore = was_errore;	// aggiunto 971210
			if (id_cont/*esto*/ == CT_PAUSA) {
///>991013
				ClearVar ();
///<
				pop_contesto ();
				dev_recupera = CONSOLE;
				pausa_recupera (CT_PAUSA);
			}
			else  {
//				vai_top_1 ();		// sostituito 971210
///>991013
//				while (liv_contesto > 0) {	// 991013
//>991206
//				if (liv_contesto > 0) {		// 991126
				while (liv_contesto > ini_liv_contesto) {	// 991206
//<991206
///<
					ClearVar ();
///>991013
					pop_contesto ();
				}
///<991013
//>991126
				if (liv_contesto == 0)
					ini_exec();
//<991126
				return;
			}
		};
	};
}

/*----------------------------------------
  analizza un token ed il token successivo
  ----------------------------------------*/
void gettok ()
{
	prev_token = token;
	tipo_token = typ (val_token = car (token));
	next_val = NULLP;
	if ((next_token = token) != NULLP) {
		do next_val = car (next_token = cdr (next_token));
		while (next_val == DEL_FINELINEA);
	}
}

void COSA ()
{
	node oggetto;
	risultato = car (oggetto = cercav (arg_1, LVAR));
	if ((oggetto == NULLP) || (risultato == NULLVALUE))
	errore (1, arg_1, NULLP);
}

void CIAO ()
{
//	main_fin ();
	is_ciao = TRUE;		// non serve ?
	IperlogoExit ();
	is_ciao = FALSE;	// 980903: potrebbe non essere confermata l'uscita
}

void NIENTE () {}

void _BREAK ()
{
  	is_break = TRUE;
//	vai_top_1 ();
	ClearVar ();
	ini_exec ();		// aggiunto 991126 (tolto da ClearVar)
}

void U_ERRORE ()
{
	risultato = ult_err;
	ult_err = NULLP;
}

void err_arg1 ()
{
	errore (5, funzione, arg_1);
}

void err_arg2 ()
{
	errore (5, funzione, arg_2);
}

/*-------------------------------------------------------------------
  memorizzazione di alcuni parametri dell' errore riscontrato
  durante la valutazione di un token o l' esecuzione di una primitiva
  -------------------------------------------------------------------*/
void errore (int codice, node f, node o)
{
	node lista, elemento, locale;

	/* prepara 6 elementi su stack dei valori */
	push_sv (int_num (codice));
	push_sv (msgstr (codice + n_messaggi, f, o));
	push_sv ((liv_procedura == 0) ? NULLP : stk_funzioni [liv_procedura]);
	lista = NULLP;
	elemento = err_token;
	while ((elemento != NULLP) && (elemento != token)) {
		if ((locale = car (elemento)) == DEL_FINELINEA) lista = NULLP;
//>991126
//		else lista = lstcat (lista, cons (locale, NULLP, LIST));
		else {
			node cell = cons (locale, NULLP, LIST);
			if (lista == NULLP)
				lista = cell;
			else
				lista = lstcat (lista, cell);
		}
//<
		elemento = cdr (elemento);
	};
	push_sv (lista);
	push_sv (f);
	push_sv (o);
	ult_err = _lista (6);
	is_errore = TRUE;
}

/*procedura di errore fatale : messaggio e ritorno a toplevel*/
void errfat (int codice)
{
	msg (codice);
	vai_top_1 ();
}

void qui_top () {
	return;		// 980122
/*
__asm {
	pop top_ip
	pop top_cs
	mov top_bp,bp
	mov top_sp,sp
	mov top_ss, ss
	push top_cs
	push top_ip
	}
*/
}

void vai_top () {
//	main_loop (TRUE);
	return;		// 971221 (non funziona)
/*
__asm {
	mov bp,top_bp
	mov ss,top_ss
	mov sp,top_sp
	jmp DWORD PTR top_ip
	}
*/
}

/*-----------------------------------------------------------------------
  esegue un ritorno piu' ordinato di vai_top al livello comandi di MLOGO:
  elimina i valori delle variabili locali;
NOTA: E' CHIAMATA SOLO DA QUESTO MODULO !!!
  -----------------------------------------------------------------------*/
void vai_top_1 ()
{
	ClearVar ();
	ini_exec ();		// aggiunto 991126 (tolto da ClearVar)
	vai_top ();
}

int dimnode = sizeof(node);
int dimnodo = sizeof(struct nodo);
int dimnodoil = sizeof(struct nodoil);
int dimcell = sizeof(union cell);
int dimcarcdr = sizeof(struct car_cdr);


/* ILMAIN.C */
