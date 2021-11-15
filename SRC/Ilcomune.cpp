/* ILCOMUNE.C		G. Toffoli

000511	spostato s_input, s_linea, s_token in ilanalisi.cpp
981121	spostato descr_sf in ILSIS.CPP
981118	spostato pagine_base in ILNODI.CPP
980831	aggiunto "#ifdef CMODULE .. extern .. #endif" a buf_c, _schermo, resto_pn
		reinserito array lstack (da ilstack.cpp)
		disattivato qui_top e top_p
980127	spostato definizioni su contesto a CONTESTI.H
960926	aggiunto descrittore di tipo LREF
960423	eliminato ss_alfa, ss_graf
		aggiunto PI (pigreca)
  960409	1^ vers. per Windows da vers. Iperlogo del 12.09.89		
*/

#include "stdafx.h"

#include "LOGO.H"
#include "ILCOMUNE.INC"	


#ifdef CMODULE
extern
#endif
node lstack [DIM_LSTACK];	/* stack logo */

#ifndef ILDOS

#ifdef CMODULE
extern
#endif
node args[MAX_ARGS];
#ifdef CMODULE
extern
#endif
int types[MAX_ARGS];

#endif ILDOS
				/* variabili per comunicaz. tra funzioni */
#ifdef CMODULE
extern
#endif
node	nuovo_el,
	vecchio_el;
#ifdef CMODULE
extern
#endif
int	i_indice;

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< non spostare : */
				/* variabili veramente globali */
#ifdef CMODULE
extern
#endif
long int n_parole;
#ifdef CMODULE
extern
#endif
int	n_indice
#ifndef CMODULE
	= 16
#endif
;
#ifdef CMODULE
extern
#endif
int	conto_diz [DIM_INDICE];	/* riempimento delle sez. del dizionario */
			/********** indici a nodi del work-space *************/
#ifdef CMODULE
extern
#endif
node	indice_diz [DIM_INDICE],/* indice delle sezioni del dizionario */
	kw [DIM_KW],		/* indice delle parole chiave */
	_nome [MAX_DEV+1],	/* indice dei nomi dei device LOGO */
	bcd [DIM_BCD],		/* indice delle costanti numeriche bcd */
	tab_prog [DIM_CODICI],	/* tabella di programmazione tasti */
	prog_touch
#ifndef CMODULE
	 = 0L
#endif
	 ;	/* lista con cui e' programmato il TS */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> non spostare */

#ifdef CMODULE
extern
#endif
int	_file [MAX_DEV+1],	/* indice dei  file C assoc. a device LOGO */
	_fstato [MAX_DEV+1];	/* stato LOGO file C assoc. a device LOGO */

#ifdef IL89		// aggiunta condizione 980123

		/* interfaccia per funzioni DOS FIND_FIRST e FIND_NEXT */
#ifdef CMODULE
extern
#endif
struct dta filefind;

#endif IL89

			/********** indici nell' array dev ********/
#ifdef CMODULE
extern
#endif
int	dev_leggi,	/* device per lettura */
	dev_scrivi,	/* device per scrittura */
	dev_in,		/* device di input corrente MLOGO */
	dev_out,	/* device di output corrente MLOGO */
	dev_dribble,	/* device per dribble */
	dev_tarta,	/* device attivo come TARTA */
	dev_foglio;	/* device attivo come FOGLIO */


#ifdef CMODULE
extern
#endif
char	delimitatore [2]
#ifndef CMODULE
	= " "		/* valore event. token del. durante analisi */
#endif
	,
	prompt [3];	/* stringa di prompt per input interattivo */
//> 000511
//	s_input [MAXCHARS+1],/* stringa da input MLOGO */
//	s_token [64],	/* stringa di token */
//	s_linea [128];	/* ultima linea digitata */
//<

#ifdef CMODULE
extern
#endif
node	p_cella; 	/* puntatore a ultima cella creata durante analisi*/
#ifdef CMODULE
extern
#endif
int	tipo_cella, 	/* tipo cella creata durante analisi */
	is_lib,		/* versione con libreria associata */
	is_debug,	/* e' attiva la modalita' debug */
	is_demo,	/* e' attiva la modalita' DEMO */
	is_gia,		/* gia' passato per top_level */
	is_ciao,	/* eseguito comando "ciao" */
	is_break,	/* e' attiva richiesta di break */
	is_conin,	/* attesa di input da console */
	is_eco,		/* eco abilitato */
	is_locale,	/* e' in corso dichiarazione di variabile locale */
	is_acchiappa,	/* appena incontrato comando ACCHIAPPA */
	is_lancia,	/* appena incontrato comando LANCIA    */
	is_pausa,	/* appena incontrato comando PAUSA     */
	is_continua,	/* appena incontrato comando CONTINUA  */
	is_recupera,	/* appena incontrato comando RECUPERA  */
	is_vai;		/* appena incontrato comando VAI       */
#ifdef CMODULE
extern
#endif
node	dove_vai;	/* valore parola associata a comando VAI */
#ifdef CMODULE
extern
#endif
int	is_traccia,	/* e' attivo tracciamento di variabile o procedura */
	is_finito,	/* finito esecuzione di lista di istruzioni */
	is_stop,	/* incontrata fine di procedura */
	is_riporta,	/* procedura termina con RIPORTA */
	is_per,		/* in corso definizione di procedura */
	is_leggiparola,	/* e' in esecuzione primitiva LEGGIPAROLA */
	is_letterale;	/* scrivere parola/stringa in modo letterale */
#ifdef CMODULE
extern
#endif
node	lista_prog;	/* eventuale lista programmata digitata */
#ifdef CMODULE
extern
#endif
int	is_interprete,	/* input e' richiesto da interprete dei comandi */
	is_analisi,	/* input e' richiesto tramite analisi */
	is_prima_linea,	/* e' prima linea di input */
	liv_analisi;	/* livello di discesa dell' albero di analisi */
#ifdef CMODULE
extern
#endif
node	dizionario;	/* dizionario degli oggetti MLOGO */
#ifdef CMODULE
extern
#endif
int	mod_parola;	/* modalita' di esecuzione di una parola MLOGO */
#ifdef CMODULE
extern
#endif
node	blocco;		/* lista che rappresenta un blocco di comandi */
#ifdef CMODULE
extern
#endif
int	ha_blocco_valore,/*il blocco deve riportare un valore a SE, SEV, SEF */
	liv_contesto;	/* livello di nidificazione dei contesti */

#ifdef ILDOS

/* <--------------------------------- attenzione : NON MODIFICARE */
#ifdef CMODULE
extern
#endif
node	id_contesto;	/* identificatore del motivo di attivazione contesto */
#ifdef CMODULE
extern
#endif
int	dev_recupera,	/* device per comando recupera */
	liv_procedura,	/* livello di nest delle procedure LOGO */
	in_liv_proc,    /* valore di liv_procedura a inizio PAUSA/RECUPERA */
	liv_funzione,	/* livello di profondita' delle funzioni */
	in_liv_funzione;/* valore di liv_funzione a inizio PAUSA/RECUPERA */
#ifdef CMODULE
extern
#endif
node	funzione;	/* indir. funzione (primitiva o procedura) corrente */
#ifdef CMODULE
extern
#endif
int	liv_esecuzione,	/* livello di discesa albero di esecuzione (blocchi) */
	val_verifica,	/* valore corrente del predicato argomento di "se" */
	conto_esegui;	/* contatore delle iterazioni di un blocco */
#ifdef CMODULE
extern
#endif
node  token,		/* elemento della lista di comando */
	ini_token;    	/* token iniziale */
#ifdef CMODULE
extern
#endif
int	n_arg_attesi,	/* numero argomenti attesi */
	n_arg_trovati,	/* numero argomenti incontrati */
	parentesi,      /* */
	conto_parentesi,/* conto algebrico parentesi in valut. espressione */
	p_sc,		/* pointer allo stack di controllo */
	p_sv,		/* pointer allo stack dei valori */
	ini_p_sv;	/* pointer iniziale allo stack dei valori in contesto*/
#ifdef CMODULE
extern
#endif
node	linea_com;	/* lista che rappresenta una linea di comando parsed */
/* ---------------------------------------------------> fin qui */

#endif // ILDOS

#ifdef ILWIN

// batteria di #define relative a campi di contesti[0]
// spostata in "contesti.h"

#endif // ILWIN

#ifdef CMODULE
extern
#endif
#ifdef ILDOS
struct contesto contesti [MAX_CONTESTO];/* stack dei contesti */
#else // ILDOS
struct contesto contesti [MAX_CONTESTO+1];/* stack dei contesti */
#endif // ILDOS
#ifdef CMODULE
extern
#endif
node /* int */ stk_analisi [20];/* stack di camm. att. in albero di analisi */
#ifdef CMODULE
extern
#endif
int	stk_livelli [MAX_PROC];	/* stack di camm. att. dur.esecuz. (blocchi) ?? */
#ifdef CMODULE
extern
#endif
node stk_funzioni [MAX_PROC];/* stack di nest procedure attive dur. esecuzione */
#ifdef CMODULE
extern
#endif
int	is_funzione;	/* la primitiva e' comando/funzione */
#ifdef CMODULE
extern
#endif
node	lista_arg,  /* lista dei nomi degli argomenti di una procedura */
	corpo;		/* corpo di procedura */
#ifdef CMODULE
extern
#endif
int	n_locali,	/* numero variabili locali nella procedura */
	n_argomenti;	/* numero argomenti della procedura */

#ifdef ILDOS
#ifdef CMODULE
extern
#endif
node arg_1,		/* primo argomento */
	arg_2,		/* secondo argomento */
	arg_3;		/* terzo argomento */
#endif // ILDOS

#ifdef CMODULE
extern
#endif
node risultato;	/* risultato della primitiva corrente */

#ifdef ILDOS
#ifdef CMODULE
extern
#endif
int	tipo_1,		/* tipo del primo argomento */
	tipo_2,		/* tipo del secondo argomento */
	tipo_3;		/* tipo del terzo argomento */
#endif // ILDOS

#ifdef CMODULE
extern
#endif
int	tipo_token;	/* tipo del token corrente */
#ifdef CMODULE
extern
#endif
node	val_token,	/* valore del token corrente */
	next_token,	/* eventuale token successivo */
	next_val,	/* valore di eventuale token succesivo */
	prev_token,	/* eventuale token precedente */
	err_token,	/* token iniziale di scope di errore */
	oggetto;	/* oggetto corrente */
#ifdef CMODULE
extern
#endif
unsigned code_seg,	/* base del code-segment */
	data_seg,	/* base del data-segment */
	seg_max,	/* ultimo segmento di RAM + 1 */
	seg_nodi;	/* primo segmento dello spazio dei nodi */

#ifdef CMODULE
extern
#endif
int	is_fatal,	/* si e' verificato errore fatale (disco/dischetto) */
	is_errore;	/* incontrato e ancora non gestito errore */
#ifdef CMODULE
extern
#endif
node	ult_err;	/* descrittore di ultimo errore */

#ifdef CMODULE
extern
#endif
int	semaforo,	/* semaforo per gestione cursori grafici */
	i_leggi_foglio,	/* posizione corrente di lettura da device FOGLIO */
	is_edita,	/* e' attivo ambiente di editor */
	is_ins,		/* flag di controllo della modalita' di editing */
	is_esc,		/* e' temporaneamente disattivo ambiente editor */
	n_messaggi,	/* numero messaggi informativi */
	i_s_stringhe,	/* primo carattere libero in s_stringhe */
	i_tab_stringhe,	/* primo entry libero in tab_stringhe */
	tab_stringhe [DIM_MESSAGGI];/* tabella indice messaggi in s_stringhe */

#ifdef CMODULE
extern
#endif
char	s_stringhe [DIM_STRINGHE];/* super-stringa delle stringhe C */

#ifdef CMODULE	// 980831
extern
#endif
node	_schermo;	/* parola chiave: mix su schermo tra tarta e comandi */

#ifdef CMODULE
extern
#endif
int	schermi,	/* 1 o 2 (inizialmente 1) */
	righe_comandi,	/* righe della finestra COMANDI */
	righe_foglio,	/* righe della finestra FOGLIO */
	righe_servizio,	/* righe della finestra SERVIZIO */
	righe_tarta,	/* righe della "finestra" TARTA */
	pri_lin;	/* prima linea grafica di COMANDI in schermo MISTO */

#ifdef IL89
#ifdef CMODULE
extern
#endif
struct ss ss_alfa;	/* copia stato schermo alfanumerico */
#ifdef CMODULE
extern
#endif
struct ss ss_graf;	/* copia stato schermo grafico */
#endif // IL89

/* da MLTESTO.C */

#ifdef IL89
char	descmodi [8]	/* descrizione dei modi video */
			/* BIT7 = colore; 40/80 = colonne; 0 = non valido */
			= {40, BIT7+40, 80, BIT7+80, BIT7+40, 0, 0, 80};
#endif // IL89

#ifdef CMODULE
extern
#endif
char	s_tasti [256];	/* coda circolare dei codici battuti da tastiera */
#ifdef CMODULE
extern
#endif
char	i_tasti,	/* indice 1" carattere vuoto in s_stasti */
	j_tasti;	/* indice 1" carattere da leggere in s_tasti */

#ifdef CMODULE
extern
#endif
int ultimo_c;

#ifdef CMODULE	// 980831
extern
#endif
char buf_c;

#ifdef CMODULE
extern
#endif
int	ini_comando,	/* indice iniziale in s_comandi durante cgets */
	fin_comando,	/* indice finale   in s_comandi durante cgets */
	max_comando,	/* indice massimo  in s_comandi durante cgets */
	dim_scorri;
#ifdef CMODULE
extern
#endif
int	is_maiuscole
#ifndef CMODULE
	= TRUE
#endif
	;
#ifdef CMODULE
extern
#endif
int	col_testo,
	col_commento;

/* da MLEDITA.C */

#ifdef CMODULE
extern
#endif
int	n_c,		/* numero di caratteri compresi fra due indici dati */
  	fine;		/* indice temp di fine car, parola, linea o blocco */
 

/* da ILTARTA.C */

#ifdef CMODULE
extern
#endif
int     /* last_bkg,*/	/* ultimo valore di background usato in open */
	sfondo_video
#ifndef CMODULE
	 = 0
#endif
	 ,	/* sfondo dello schermo (non dei viewport) */
	colore_def,		/* colore da usare per default (1 o 10) */
        tipo_video,             /* tipo del video richiesto (screen) */
        lato_tarta;             /* lung. del lato triangolo tarta */

#ifdef CMODULE
extern
#endif
int	nview;			/* numero view selezionata fisicamente */
#ifdef CMODULE
extern
#endif
struct	dview	*pview,		/* puntat. a view selezion.fisicamente */
		*pt,		/* puntatore descrittore di tarta corrente */
		*view [N_VIEW];	/* puntatori ai descrittori di viewport */
#ifdef CMODULE
extern
#endif
int	no_rebuild,		/* funz. REBUILD e' sospesa */
	need_rebuild;		/* funz. REBUILD e' in agenda */
#ifdef CMODULE
extern
#endif
int     savepic;                /* file per gestione di view sovrapposte */

#ifdef CMODULE
extern
#endif
struct	dbott	*bott [N_BOTT];	/* puntatori ai descrittori dei bottoni */

#ifdef CMODULE
extern
#endif
int    /* i_base_t, */	/* indice in view [] di tarta di base (view 1) */
	i_tarta;	/* indice in view [] di tarta attuale		*/
//	fuori_x,	/* X e' fuori schermo fisico			*/
//	fuori_y;	/* Y e' fuori schermo fisico			*/

#ifdef IL89

struct	font	fonts [16] = {
	{8, 8},		/* default (ROM_8*8) */
	{8, 8},		/* ROM_8*8 */
	{8,14},		/* ROM_8*14 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8},		/* ROM_8*8 */
	{8, 8}		/* ROM_8*8 */
};

#endif IL89

#ifdef CMODULE	// 980831
extern
#endif
node	resto_pn;		/* resto di (lista di) parole o numeri */

#ifdef IL89		// aggiunta condizione 980123

#ifdef CMODULE
extern
#endif
int	set_mode,	/* Copia del registro di controllo	???*/
	modalita,	/* Modalita' di schermo			???*/
	rege_seg
#ifndef CMODULE
	 = 0xb800
#endif
	 ,/* Indirizzo del regen buffer		   */
	dim_crt,	/* Dimensione del regen buffer		  */
	ind_6845;       /* Indirizzo base del controller grafico 6845   */

#ifdef CMODULE
extern
#endif

#ifdef CMODULE
extern
#endif
int	is_cursore,	/* vale 1 durante scrittura di cursore grafico */
        offcur;         /* offset corrente: ex BX */
#ifdef CMODULE
extern
#endif
union	u_rc pos;	/* pos. corrente (riga e colonna): ex DX */

#ifdef CMODULE
extern
#endif
int	nquadro;	/* usata solo in ILQUADRO.C */
struct	dview	*pquadro,	/* pointer quadro corrente */
		*pq_foglio,	/* pointer a quadro di FOGLIO corrente */
	quadro0 /* = {0, MODO_LINEA,  0x01, 0, 0,0, DIM_COMANDI} */,/* comandi */
	quadro1 /* = {1, MODO_FOGLIO, 0x01, 0, 0,0, 1024} */,	/* foglio */
	quadro2 /* = {2, MODO_LINEA,  0x02, 0, 0,0, 128} */ ,	/* servizio */
	quadro3;						/* tarta */

#ifdef CMODULE
extern
#endif
int	nq_linea,	/* numero quadro associato a cprintf */
	nq_foglio;	/* numero quadro associato a FOGLIO corrente */

#endif // IL89

/* da MLNUMERI.C */

#ifndef ILDOS
#define PI 3.1415926535
#endif // ILDOS


#ifdef CMODULE
extern
#endif
int	acaso_i,
	precisione
#ifndef CMODULE
	 = 7
#endif
	 ;
/* char    primi [10] = {1, 3, 5, 7, 11, 13, 17, 19, 23, 29}; */

/* da MLANALIS.C  */

/*
definizione della corrispondenza tra i simboli dei delimitatori,
la loro precedenza relativa,
il numero (indirizzo) di nodo riservato alla parola corrispondente
*/
#ifdef CMODULE
extern
#endif
char	s_del [18]
#ifndef CMODULE
	= "   \r\":()+-*/^<=> "
#endif
	,
	p_del [18]
#ifndef CMODULE
	 = {0,0,0,0,0,0,0,0,2,2,3,3,4,1,1,1,0}
#endif
	 ;


#ifndef ILDOS
extern
char *stpchr ();
#endif // ILDOS

#ifdef CMODULE
extern
#endif
int	dev_temp;
#ifdef CMODULE
extern
#endif
node val_arg;

/* da ILFLUSSI.C */

#ifdef CMODULE
extern
#endif
char	crlf [3]
#ifndef CMODULE
	= "\r\n"
#endif
	;
#ifdef CMODULE
extern
#endif
int	i_stampante;

#ifdef CMODULE
extern
#endif
char command [16]
#ifndef CMODULE
	= "C:\\COMMAND"
#endif
;
#ifdef CMODULE
extern
#endif
char riga []
#ifndef CMODULE
	= "\2WS\r"
#endif
	;

/* da ILSPAZIO.C */

		/* variabili comuni alle liste proprieta'*/
#ifdef CMODULE
extern
#endif
node    o_l_p,		/* oggetto di tipo PLIST */
	lista_p,	/* lista-di-proprieta = valore di o_l_p */
	ultima_p;	/* ultima proprieta' */
		/* variabili comuni alle due procedure (ini_sel, nuovo_sel) */
#ifdef CMODULE
extern
#endif
int     tipo_sel,		/* tipo usato come criterio di selezione */
	conto_sel;		/* conto oggetti selezionati */
#ifdef CMODULE
extern
#endif
node	cond_sel,		/* condizione di selezione */
   	el_sel,			/* elemento sotto prova */
	o_sel;			/* oggetto */
#ifdef CMODULE
extern
#endif
int	no_spazio;
 
/* da MLMONDO.C */

#ifdef CMODULE
extern
#endif
int	is_tick,	/* semaforo per servizio tick */
	is_EMM,		/* semaforo per chiamata sel_EMM (memoria estesa) */
	_tempo,		/* tempo M_LOGO contato in tick di sistema */
	is_musica,	/* e' in corso la musica */
	conto_musica;	/* contatore a calare per durata nota */
#ifdef CMODULE
extern
#endif
long int osc
#ifndef CMODULE
	= 1193180
#endif
	;	/* frequenza dell' oscillatore */

#ifdef CMODULE
extern
#endif
node	l_musica;	/* lista delle note (coppie frequenza-durata) */

#ifdef CMODULE
extern
#endif
int	buff_touch
#ifndef CMODULE
	= FALSE
#endif
 ,	/* flag: VERO se c'e' una toccata pronta */
	riga_touch
#ifndef CMODULE
	 = 0
#endif
	 ,		/* riga dell'ultima toccata (0-24) */
	colonna_touch
#ifndef CMODULE
	 = 0
#endif
	 ,	/* colonna dell'ultima toccata (0-79) */
	is_hp
#ifndef CMODULE
	 = FALSE
#endif
	 ;		/* macchina e' HP VECTRA (con touch screen)
			
				   (usato per il touch HP) */
#ifdef CMODULE
extern
#endif
int	is_touch_attivo
#ifndef CMODULE
	= FALSE
#endif
 	,/* per touch screen Philips */
	is_mouse_attivo
#ifndef CMODULE
	= FALSE
#endif
	,/* per mouse Microsoft */
	is_mouse_acceso
#ifndef CMODULE
	= FALSE
#endif
	 ;/* per cursore mouse Microsoft */


/* da ILCATENE.C */

#ifdef CMODULE
extern
#endif
int	i_rd_c, i_rd_c2, i_wt_c,
	l_rd_c, l_rd_c2;
#ifdef CMODULE
extern
#endif
node	rd_cell, rd_cell2, wt_cell;
#ifdef CMODULE
extern
#endif
struct nodo val_rd_c, val_rd_c2;

/* da ILVIEW.C */

#ifdef CMODULE
extern
#endif
int     pixel_x,                /* numero di pixel lungo l'asse x del video */
        pixel_y;                /* numero di pixel lungo l'asse y del video */
#ifdef CMODULE
extern
#endif
int     num_pic_lost;           /* numero delle pic da buttare sul file */
#ifdef CMODULE
extern
#endif
char    *file_pic
#ifndef CMODULE
	= "TEMPPIC.GFX"
#endif
	,  /* nome del file per le finestre */
        *file_temp
#ifndef CMODULE
         = "TEMPPIC2.GFX"
#endif
         ;/* file ausiliario per COMPRESS */
#ifdef CMODULE
extern
#endif
char    *buff_pic;              /* puntatore al blocco ausiliario per */
				/* MOVE_PIC */

/* da MLUTIL.C */

#ifdef CMODULE
extern
#endif
node	lista_aperta;	/* lista su cui opera leggi_ele */

/* da MLNODI.ASM */

#ifdef CMODULE
extern
#endif
char desc_ogg [16]
#ifndef CMODULE
 = {
	0x00,
	0x11,	/*  1 : LIST */
	0x11,	/*  2 : LWORD */
	0x01,	/*  3 : SFUN */
	0x11,	/*  4 : UFUN */
	0x11,	/*  5 : LVAR */
#ifdef ILDOS
	0x00,
#else // ILDOS
	0x11,	/*  6 : LREF */
#endif // ILDOS
	0x00,	/*  7 : LSTR	gia'  0x01 */
	0x00,
	0x00,
	0x00,	/* 10 : LBCD 	modificato per nuovo formato */
	0x11,	/* 11 : PLIST */
#ifdef ILDOS
	0x00,
#else // ILDOS
	0x11,	/* 12 : ARAY */
#endif // ILDOS
	0x00,
	0x01,	/* 14 : INFIX 	?? */
	0x10	/* 15 : LDEL 	?? */
}
#endif
;

/*  da nuovo ILNODI.C (con extended memory manager per Vectra-EMS e file) */

#ifdef CMODULE
extern
#endif
int	nodi_ram_estesa,	/* e' vero se i nodi sono in RAM estesa */
	nodi_in_file;		/* e' vero se i nodi sono su file */
				/* N.B. se entrambi sono falsi i nodi sono
				   in memoria base */
		/* < NON SPOSTARE (vedi CREALIB e LIB) */
#ifdef CMODULE
extern
#endif
node	frelst,			/* nodo iniziale lista libera */
	ult_cons;		/* massimo numero di nodo usato (CONS) */
#ifdef CMODULE
extern
#endif
long int nodi,			/* numero nodi liberi */
	tot_nodi;		/* numero nodi in totale */
#ifdef CMODULE
extern
#endif
int	next_pagina,		/* 1 + massimo numero di pagina usata (CONS) */
		/* > FINO A QUI */
	pagine,			/* numero pagine di mem. base o estesa */
	pagina_attuale;		/* pagina logica attualmente in uso */
#ifdef CMODULE
extern
#endif
unsigned seg_page_frame,		/* segment del page frame del EMM */
	memory_handle;		/* handle usato con EMM */
#ifdef CMODULE
extern
#endif
int	file_handle;		/* handle del file dei nodi */
// #ifdef CMODULE
// extern
// #endif
// char	*pagine_base [MAX_PAG_BASE+1];	/* puntatori alla mem. 
//							base per i nodi */
#ifdef CMODULE
extern
#endif
union vp_os ris;		/* contiene il puntatore al nodo
				attualmente in uso.  NON usarlo mai
				all'esterno di ILNODI  */
#ifdef CMODULE
extern
#endif
node	ultimo_nodo,		/* ultimo dei nodi presenti sul file */
	nodo_attuale;		/* indice del nodo att. in uso (file o EMM) */
#ifdef CMODULE
extern
#endif
struct	nodoil nodo_vuoto
#ifndef CMODULE
	= { 0, 0L, 0L }
#endif
	,  /* nodo vuoto per fill (su file)*/
	val_nodo_attuale;	/* buffer di 1 nodo per il file */


#ifdef IL89
    extern int errno;	* numero dell'errore in apertura file */
#endif

/* da MLDEVICE.ASM */

#ifdef CMODULE
extern
#endif
int	brk_off,	/* offset brk_handler originale */
	brk_seg,	/* segmento di brk_handler originale */
	tt_off,		/* offset tick_handler originale */
	tt_seg,		/* segmento tick_handler originale */
	fat_off,	/* offset errore-fatale originale */
	fat_seg,	/* segmento errore-fatale originale */
	gt_off,		/* offset graftabl originale */
	gt_seg,		/* segmento graftabl originale */
	com1_off,	/* offset com1_handler originale */
	com1_seg,	/* segmento di com1_handler originale */
	com2_off,	/* offset com2_handler originale */
	com2_seg,	/* segmento di com2_handler originale */
	com_stat [2],	/* stato corrente di COM1 e COM2 */
	ij_rx [2];	/* indici i e j a coda di COM1 e COM2 */
#ifdef CMODULE
extern
#endif
char	coda_rx	[2*256];/* coda rx per linea 0, coda rx per linea 1 */

/* da C.ASM */

#ifdef CMODULE
extern
#endif
int	d_stampa
#ifndef CMODULE
	= 80
#endif
	;	/* dimensione linea di stampante */
#ifdef CMODULE
extern
#endif
char	partenza [16];	/* nome eventuale archivio da recuperare al via      */

#ifdef CMODULE
extern
#endif
node	_doveele;	/* posizione corrispondente a ultimo ELE? vero */
#ifdef CMODULE
extern
#endif
int	dest,		/* offset di destinazione per .ESEGUI */
	segmento;	/* segmento corrente per .ESEGUI .BYTE e .ASBYTE */
#ifdef CMODULE
extern
#endif
char	modo_ini;	/* modalita' di video originale */

// union	REGS	inregs,	/* registri in ingresso a interrupt 8086 */
// 		outregs;/* registri in uscita  da interrupt 8086 */
// 
// #ifdef CMODULE
// extern
// #endif
//
// struct	SREGS	segregs;

/*< vers. IperLOGO */

#ifdef CMODULE
extern
#endif
struct nodoil *pnodi;	/* spazio dei nodi */

#ifdef CMODULE
extern
#endif
int	top_ip, top_cs,	/* CS:IP di top-level */
	top_sp,	top_ss,	/* SS:SP di top-level */
	top_bp;		/*    BP di top-level */

/* indirizzo di entry-point QUI_TOP (in ILMAIN.C) */
// #ifndef CMODULE
// extern	qui_top ();
// extern void qui_top ();	// 980831
// #endif
//
// #ifdef CMODULE
// extern
// #endif
// int	(*top_p) ()
// #ifndef CMODULE
//	= &qui_top
//#endif
//	;


#ifdef IL89

/* indirizzi dei gestori di interruzione LOGO (in ILDEVICE.ASM) 
   per modifica vettori interruzione */
extern	brk_hnd (), tick_hnd (), fat_hnd (), com1_hnd (), com2_hnd ();
#ifdef CMODULE
extern
#endif
int	(*brk_p) ()
#ifndef CMODULE
	= &brk_hnd
#endif
	,
	(*tick_p) ()
#ifndef CMODULE
	 = &tick_hnd
#endif
	 ,
	(*fat_p) ()
#ifndef CMODULE
	 = &fat_hnd
#endif
	 ,
	(*com1_p) ()
#ifndef CMODULE
	 = &com1_hnd
#endif
	 ,
	(*com2_p) ()
#ifndef CMODULE
	 = &com2_hnd
#endif
	 ;

extern graftabl ();
#ifdef CMODULE
extern
#endif
int	(*gt_p) ()
#ifndef CMODULE
	= &graftabl
#endif
	;

#endif // IL89

/*> vers. IperLOGO */

/* ILCOMUNE.C */
