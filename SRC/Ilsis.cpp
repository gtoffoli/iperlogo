/* ILSIS.C		G. Toffoli

040228	aggiunto PR_FTP_CATALOGO, PR_FTP_ALBERO e PR_FTP_ATTR (vuoto)
040222	aggiunto PR_FTP_CANCEARC, PR_FTP_RINOMINAARC,PR_FTP_CARTELLA, PR_FTP_CAMBIACARTELLA, PR_FTP_CREACARTELLA, PR_FTP_RIMUOVICARTELLA
040221	aggiunto PR_FTPAPRI, PR_FTPCHIUDI, PR_FTPGET, PR_FTPPUT
040205	in PR_SIGNATURE aggiunto 4-to argomento
040130	in PR_BITSAVE aggiunto altri 3 argom. opzionali per GIF (e TIF) multipli
040126	in PR_BITLOAD e PR_BITSAVE: da 1 (default) a 2 argomenti
040123	rinominato BITLOAD -> PR_TURTLELOAD e BITSAVE -> PR_TURTLESAVE
		introdotto nuove PR_BITLOAD e PR_BITSAVE dummy
		(nuovi BLOCCORECUPERA e BLOCCOCONSERVA, non piu' sinonimi di TARTARECUPERA e TARTACONSERVA)

031030	aggiunto PR_SETCURSOR (al posto di ASCURSORE di Iperlogo-DOS)
020531	tolto _W ovunque non necessario
010606	PR_SHELL_COMMANDLINE dopo PR_SHELL_GETFOLDERPATH
010602	portato maxarg: in CREALIB 2 -> 3; in LIBERA 0 -> 1
010526	aggiunto PR_SIGNATURE dopo PR_VERSION
010213	aggiunto PR_ORDINA2 dopo ORDINA
010207	allineato con IL32 (mod. a CREALIB del 001218) 
010128	PR_VOCE_DIALOGO
010122	PR_CONF_BUFFER
010108	agg. argomento a PR_CONF_VIDEOAPRI, PR_CONF_VIDEOCHIUDI, PR_CONF_DATIAPRI e PR_SPEECH_ENABLE
001218	aggiunto argomento a CREALIB (libcode)
000822	aggiunto primitive INTEGRA
000816	aggiunto 3 primitiva INTEGRA sotto omonimo switch di precompilation
000511	aggiunto 7-imo argomento a EDITBOXCREATE (CASELLATESTOCREA)
000508	aggiunto PR_GO_IMAGE e PR_GO_IMAGE_DIM
000502	in PR_GO_LABEL riportato a 1 N.arg.standard; mod. PR_GO_TEXT
000313	aggiunto PR_GO_SETPANSELECT
000310	aggiunto PR_SHELL_GETFOLDERPATH
000221	aggiunto PR_SHELL_LINKCREATE
000202	aggiunto LISTBOXRESET
000131	aggiunto PR_DRIVETYPES e PR_DRIVEREADY (dummy)
000129	mod. specifiche PR_SYMBOL_SETLABEL (già PR_SYMBOL_ADDLABEL)
000125	mod. numero argomenti a primitive di posiz., direz. e scala
000123	OG_DRAW a 1-3 argomenti, OG_APPEND a 2 argomenti
000119	aggiunto campo N_MASSIMO in 3 macro e in sis_tab
000118	aggiunto PR_CREACARTELLA, PR_RIMUOVICARTELLA, PR_RISALICARTELLA, PR_CARTELLE, PR_ARCHIVI
000113	aggiunto PR_SETPIXEL (ASPIXEL) e PR_LOCALMAKE (ASLOCALE)
		rilassato controlli su colori
991224	aggiunto PR_GO_TEXT
991223	aggiunto PR_SETTEXTFONTLOG e PR_LABELDIMLOG
991218	ripristinato CREALIB e LIB
991217	aggiunto ALBERO dopo CATALOGO
991215	aggiunto argomento a DIALOGFILEOPEN e DIALOGFILESAVE
991214	aggiunto PR_DAOEXECUTE (DATABASEESEGUI) dopo PR_DAOQUERY
991213	esteso STRINGA ad argomenti qualsiasi
991212	inserito DIALOGSETFOCUS dopo DIALOGDELETE
991209	esteso SETITEM a liste (e parole/stringhe : da implementare)
991208	aggiunto PR_GO_LABEL e PR_SYMBOL_ADDLABEL
991202	aggiunto PR_SYMBOLINSERT, simile a PR_NODEINSERT, ma senza copia
991130	agg. PR_GO_APPEND, PR_GO_DRAW, PR_GO_SYMBOL, PR_GO_SETTYPE
991124	abilitato primitive di PLAYER; aggiunto PR_PLAYERMCI
991116	agg. PR_MIDIOUTNUMDEVS (MIDICONTA) prima di MIDIAPRI; spostato PR_MIDI (MIDI)
991105	inserito PR_DIALOGHI prima di WINDOWCREATE
991104	inserito 6 primitive dopo PR_MENUCREATE
991102	inserito PR_DUPLICATE prima di PR_COMBINE
991028	PR_SYMBOL_INSERTLABEL (già ADD), PR_SYMBOL_REMOVELABEL e PR_SYMBOL_LABELNUMBER
991027	inserito PR_SETTURTLEPROPS
991026	mod. specs di PR_NODORECUPERA, PR_NODOINSERISCI, PR_SYMBOL_ADDLABEL; agg.PR_NODOCREA
991021	agg. PR_GO_SELECT, PR_GO_RECT e altre 5
991014	aggiunto PR_PAGESETUP e PR_TURTLEPRINTPREVIEW
991012	inserito 4 primitive per EDITBOX prima di COMBOBOX; PR_PRINT_QUOTED
991011	aggiunto PR_GO_ID
991007	modificato numero argomenti in WINDOWCREATE
991005	aggiunto PR_ROOT (RADICE)
990927	duplicato ASSOC !!! (a 2 e 3 argomenti)
990925	aggiunto PR_VERSION
990924	inserito PR_SETASSOC
990923	inserito PR_SYMBOL_INSERT
990922	corretto argomento di PR_RECUFOGLIO e PR_CONSFOGLIO),
990920	inserito PR_GO_ON (OGACCENDI) e PR_GO_OFF (OGSPEGNI)
990917	inserito PR_TOOLBARSETPOS (PULSANTIERAASPOS)
		PR_TURTLEPROPS (TARTAPROP) e PR_TURTLETOOLS (TARTATOOL) 
990916	aggiunto PR_MENUCREATE
990915	aggiunto numerose primitive del tipo PR_COMP...
990913	aggiunto primitive per diagrammi e oggetti grafici
990728	aggiunto PR_PROCESSOCREA
990714	modificato impl. di PR_QUOTED (LETTERALE)
		riciclato per PR_QUOTED codice primit. PMCI (non usato)
990707	GLOSSARY e SHOWGLOSSARY passano da 3 a 6 argomenti
990630	aggiunto (dummy) COPIAARC, RINOMINAARC, MIDI
990616	definito 8 argomenti per TOOLBARCREATE
990526	aggiunto nuovo BLOCCOSTAMPA (BITPRINT ora = TARTASTAMPA)
990523	sostituito implementazione di FOGLIOESEGUI
		FOGLIOIMPOSTAFONT al posto di FOGLIOASSEGNAFONT
990418	aggiunto FOGLIOIMPOSTASTAMPANTE, FOGLIOSTAMPA,
		FOGLIOTAGLIA,FOGLIOCOPIA, FOGLIOINCOLLA,
		FOGLIOASSEGNAFONT, FOGLIOESEGUISELEZIONE, FOGLIOSELEZIONABLOCCO
		RIEMPISELEZIONE, SELEZIONAREATTIVA, ATTIVASELEZIONE,CENTRA
990510	riattivato primitiva SCHERMO
990415	in LOCALE aggiunto sintassi 2 (n. argom. illimitato)
990413	aggiunto CONTACARATTERI (_CONTAC) e _CONTALINEE (_CONTAL)
990408	recupero di molte primitive di Foglio
990401	aggiunto PR_GLOSSARY (GLOSSARIO), PR_SHOWGLOSSARY (GLOSSARIOMOSTRA)
990318	aggiunto PR_SHOWHELP (AIUTOMOSTRA) e PR_DOCLIST (LISTADOC)
990314	aggiunto PR_DAOQUERY
990304	aggiunto PR_WINDOWSHOW (FINESTRAMOSTRA)
990225	riattivato NONREGISTRARE (NODRIBBLE)
		aggiunto PR_TOOLBARCREATE e PR_TOOLBARDELETE
990222	PR_LABELDIM (ETICHETTADIM)
990115	PR_BROWSER, PR_CHIUDIBROWSER
		ripristinato vecchio PULISCITESTO (non CLEARTEXT)
990112	PR_FOGLIO, PR_RECUFOGLIO, PR_CONSFOGLIO, PR_EXECFOGLIO, 
981209	PR_CREATARTA, FOGLIO, BROWSER, PLAYER
		PR_CHIUDITARTA, FOGLIO, BROWSER, PLAYER
981121	spostato campi in descrittore di primitiva
981119	riattivato ATTR e ASATTR
981016	riattivato COPIADEF e _RINOMINA (gia' _rinomina ?)
		esteso CANCEPRIM da lista a singola primitiva
		riattivato CONS_DISEGNO e RECU_DISEGNO (dummy)
980914	attivato PR_COMBINE, PR_LASTS, PR_BUTLASTS
980911	aggiunto ASMODOPENNA (PR_SETPENMODE); ritoccato nomi
		attivato PR_SIGN e PR_REMOVE
980908	VAXY cambia specifiche (come SETXY di MswLogo) !!!!!!!!
		aggiunto VAPOS come precedente VAXY (SETPOS di MswLogo)
		aggiunto (dummy) LASTS, BUTLASTS, COMBINE, REMOVE, ..
		.. REMDUP, QUOTED, SIGN, GENSYM
980902	reimplementato PENNA e ASPENNA; riattivato VAI
980728	reimplementato TARTA?, FOGLIO? e CANCEFOGLIO
		aggiunto PAROLAVUOTA e LISTAVUOTA
980721	reimplementato ASSFSCRIVI (ASSFONCAR) per TARTA
980717	modificato controllo argomento in APRI per consentire attributi
980714	riabilitato ASTASTO e TASTO
980713	aggiunto primitive ICON, KB, FOCUS, SCROLL, ..
980709	inserito primitiva MCI (1 argomento)
980503	inserito primitive MIDI
971210	riattivato SDEF (SPROC)
970808	iniziato pulizia (EDITA, CDOS, ..) e allineamento bilingue
970602	EDITA anche a 0 argomenti
970402	aggiunto EDITA
961001	aggiunto RADSIN, RADCOS, RADTAN, RADARCTAN e REVERSE
		esteso FPUT (MPRI) e LPUT (MULT) a parole/stringhe
960930	attivato funzioni logiche bit a bit
960910	aggiunto CURDIR e XOR; aggiunto argomento a RIACASO
960517	reinserito primitiva persa (AFINELINEA); riabilitato prim. disco e file
960410	prima vers. Windows da ILSIS.ASM del 15.09.89
*/

#include "stdafx.h"

#define ILWIN
#define CON_NMASSIMO

// #define CMODULE
// #include "ilcomune.cpp"
#include "logo.h"
#include "ilsfun.h"
#include "ilfunz.h"

#ifdef ILWIN

void _ARRAY();
void SETITEM();
void ARRAYP();

void PR_FIRSTS ();
void PR_BUTFIRSTS();

void REVERSE();

void LOWERCASE();
void UPPERCASE();

void PARSE();
void RUNPARSE();

void READCHARS();

void MINUS();

void RADSIN();
void RADCOS();
void RADTAN();
void RADARCTAN();

void XOR();
void BITAND();
void BITOR();
void BITXOR();
void BITNOT();
void ASHIFT();
void LSHIFT();

void ARC();

void MOUSEON();
void MOUSEOFF();
void MOUSEPOS();
// void CLEARTEXT();

void WRAP();
void WINDOW();
void FENCE();

void LABEL();
void SETTEXTFONT();
void PR_SETTEXTFONTLOG();
void TEXTFONT();
void TEXTSCREEN();
void FULLSCREEN();
void SPLITSCREEN();
void REFRESH();
void NOREFRESH();

void FINESTRE();
void TURTLEWINDOW();
void ZOOM();
void RESIZE();

void SETFLOODCOLOR();
void SETPENSIZE();
void SETPENPATTERN();
void CLEARPALETTE();
void PENDOWNP();
void PENCOLOR();
void FLOODCOLOR();
void PENSIZE();
void PENPATTERN();

void RUNRESULT();
void REPCOUNT();
void IFELSE();
void YIELD();
void NOYIELD();
// void EVENTCHECK();

void CHDIR();
void CURDIR();

void BITCUT();
void BITCOPY();
void BITPASTE();
void BITFIT();
void SETBITINDEX();
void BITINDEX();
void SETBITMODE();
void BITMODE();
void BITBLOCK();
void BITPRINT();
//>040123
// void BITLOAD();
// void BITSAVE();
void PR_BITLOAD(); // dummy
void PR_BITSAVE(); // dummy
void PR_TURTLELOAD();
void PR_TURTLESAVE();
//<040123
void SETACTIVEAREA();
void ACTIVEAREA();

void SETTIMER();
void CLEARTIMER();

void PR_DIALOGHI();

void WINDOWCREATE();
void WINDOWDELETE();

void DIALOGCREATE();
void DIALOGDELETE();
void DIALOGSETFOCUS();

void LISTBOXCREATE();
void LISTBOXDELETE();
void LISTBOXGETSELECT();
void LISTBOXADDSTRING();
void LISTBOXDELETESTRING();
void LISTBOXGETINDEX();
void LISTBOXCREATERAW();
void LISTBOXRESET();

void EDITBOXCREATE();
void EDITBOXDELETE();
void EDITBOXGETTEXT();
void EDITBOXSETTEXT();

void COMBOBOXCREATE();
void COMBOBOXDELETE();
void COMBOBOXGETTEXT();
void COMBOBOXSETTEXT();
void COMBOBOXADDSTRING();
void COMBOBOXDELETESTRING();

void SCROLLBARCREATE();
void SCROLLBARDELETE();
void SCROLLBARGET();
void SCROLLBARSET();

void BUTTONCREATE();
void BUTTONDELETE();

void STATICCREATE();
void STATICDELETE();
void STATICUPDATE();

void GROUPBOXCREATE();
void GROUPBOXDELETE();

void CHECKBOXCREATE();
void CHECKBOXDELETE();
void CHECKBOXGET();
void CHECKBOXSET();

void RADIOBUTTONCREATE();
void RADIOBUTTONDELETE();
void RADIOBUTTONGET();
void RADIOBUTTONSET();

void DEBUGWINDOWS();
void MESSAGEBOX();
void DIALOGFILEOPEN();
void DIALOGFILESAVE();
void WINDOWFILEEDIT();

void SOUND();

void EDITA();

void PR_MIDIOUTNUMDEVS();
void MIDIOPEN();
void MIDICLOSE();
void PR_MIDI();
void MIDIMESSAGE();
void PLAYWAVE();
void MCI();

void PR_MINIMIZE();
void PR_MAXIMIZE();
void PR_NORMALIZE();
void PR_RESTORE();
void PR_SETFOCUS();
void PR_GETFOCUS();

void PR_KBON();
void PR_KBOFF();
void PR_KBVAL();

void PR_TARTA_();
void PR_FOGLIO_();
void PR_CANCEFOGLIO();

void PR_EMPTYWORD();
void PR_EMPTYLIST();

void PR_VAPOS();

void PR_LASTS ();
void PR_BUTLASTS();

void PR_DUPLICATE();
void PR_COMBINE();
void PR_REMOVE();
void PR_REMDUP();
void PR_QUOTED();

void PR_SIGN();

void PR_GENSYM();

void PR_PENPAINT();
void PR_PENREVERSE();
void PR_PENERASE();
void PR_SETPENMODE();
void PR_PENMODE();

void PR_CREATARTA();
void PR_CREAFOGLIO();
void PR_CREABROWSER();
void PR_CREAPLAYER();

void PR_CHIUDITARTA();
void PR_CHIUDIFOGLIO();
void PR_CHIUDIBROWSER();
void PR_CHIUDIPLAYER();

void PR_TARTA();
void PR_FOGLIO();
void PR_BROWSER();
void PR_PLAYER();

void PR_RECUFOGLIO();
void PR_CONSFOGLIO();
void PR_EXECFOGLIO();

void PR_LABELDIM();
void PR_LABELDIMLOG();

void PR_TOOLBARCREATE();
void PR_TOOLBARDELETE();
void PR_TOOLBARSETPOS();

void PR_WINDOWSHOW();

void PR_DAOQUERY();
void PR_DAOEXECUTE();

void PR_SHOWHELP();
void PR_DOCLIST();
void PR_GLOSSARY();
void PR_SHOWGLOSSARY();

void CONTACARATTERI();
void CONTALINEE();

void FOGLIOIMPOSTASTAMPANTE();
void FOGLIOSTAMPA();
void FOGLIOTAGLIA();
void FOGLIOCOPIA();
void FOGLIOINCOLLA();
void FOGLIOIMPOSTAFONT();
void FOGLIOESEGUISELEZIONE();
void FOGLIOESEGUITUTTO();
void FOGLIOSELEZIONABLOCCO();

void PR_RIEMPISELEZIONE();
void PR_SELEZIONAREATTIVA();
void PR_ATTIVASELEZIONE();
void PR_CENTRA();

void PR_TURTLEPRINT();

void PR_COPIAARC();
void PR_RINOMINAARC();
void PR_PROCESSOCREA();

// ---------

void PR_CONSUMA();

void PR_SETTURTLEPROPS();
// void PR_TURTLEPROPS();
void PR_GETTURTLEPROPS();
void PR_TURTLETOOLS();
void PR_GO_SELECTION();
void PR_GO_REMOVE();
void PR_GO_DUPLICATE();
void PR_GO_CENTER();
void PR_SYMBOLLOAD();
void PR_NODECREATE();
void PR_NODEINSERT();
void PR_NODICONNETTI();
void PR_ARCOINVERTI();
void PR_GO_POS();
void PR_GO_SETPOS();
void PR_COMPTRANSLA();
void PR_COMPDIR();
void PR_COMPASDIR();
void PR_COMPRUOTA();
void PR_COMPSCALA();
void PR_COMPASSCALA();
void PR_GO_GROUP();
void PR_GO_ELEMENTS();
void PR_GO_UNGROUP();
void PR_GO_ON();
void PR_GO_OFF();
void PR_GO_ID();

void PR_MENUCREATE();
void PR_MENUDELETE();
void PR_MENU();
void PR_MENUELEMENT();
void PR_MENUELEMENTSET();
void PR_MENUELEMENTREMOVE();
void PR_MENUELEMENTREPLACE();

void PR_SETASSOC();
void PR_VERSION();
void PR_SIGNATURE();	// 010526
void PR_ROOT();
void PR_PRINT_QUOTED();
void PR_PAGESETUP();
void PR_TURTLEPRINTPREVIEW();

void PR_GO_SELECT();
void PR_GO_POLYLINE();
void PR_GO_POLYGON();
void PR_GO_CURVE();
void PR_GO_CLOSEDCURVE();
void PR_GO_RECT();
void PR_GO_ELLIPSE();
void PR_SYMBOL_INSERTLABEL();
void PR_SYMBOL_REMOVELABEL();
void PR_SYMBOL_LABELNUMBER();

void PR_PLAYERCOMMAND();

void PR_GO_APPEND();
void PR_GO_DRAW();
void PR_GO_SYMBOL();
void PR_GO_SETTYPE();
void PR_SYMBOLINSERT();

void PR_GO_LABEL();
void PR_SYMBOL_SETLABEL();
void PR_GO_TEXT();
void PR_GO_IMAGE();
void PR_GO_IMAGE_DIM();

void PR_GO_SETPANSELECT();

void PR_CATALOGO();
void PR_ALBERO();
void PR_ORDINA2();	// 010213

void PR_SETPIXEL();
void PR_LOCALMAKE();

void PR_CREACARTELLA();
void PR_RIMUOVICARTELLA();
void PR_RISALICARTELLA();
void PR_CARTELLE();
void PR_ARCHIVI();
void PR_DRIVETYPES();
void PR_DRIVEREADY();

void PR_SHELL_LINKCREATE();
void PR_SHELL_GETFOLDERPATH();
void PR_SHELL_COMMANDLINE();

void PR_SETCURSOR();

void PR_FTP_APRI();
void PR_FTP_CHIUDI();
void PR_FTP_GET();
void PR_FTP_PUT();
void PR_FTP_CANCEARC();
void PR_FTP_RINOMINAARC();
void PR_FTP_CARTELLA();
void PR_FTP_CAMBIACARTELLA();
void PR_FTP_CREACARTELLA();
void PR_FTP_RIMUOVICARTELLA();
void PR_FTP_CATALOGO();
void PR_FTP_ALBERO(); 
void PR_FTP_ATTR();

#ifdef INTEGRA
void PR_CONF_STATO();
void PR_CONF_INIZIA();
void PR_CONF_TERMINA();
void PR_CONF_CONFCREA();
void PR_CONF_CHIAMA();
void PR_CONF_VIDEOAPRI();
void PR_CONF_VIDEOCHIUDI();
void PR_CONF_DATIAPRI();
void PR_CONF_DATICHIUDI();
void PR_CONF_DATIINVIA();
void PR_CONF_BUFFER();

void PR_VOCE_ACCENDI();
void PR_VOCE_SCRIVI();
void PR_VOCE_DIALOGO();
#endif // INTEGRA

#endif // ILWIN

/* codifica di descrittore di primitiva */

#define	o		0
#define	f		1		// Funzione (primitiva che riporta)
#define	i		1		// numero argomenti Illimitato
#define	r		0x400	// shift-left 10 bits per codice di range
#define	n		r * 1	// codice di range per numero naturale
#define	t		0

/* aggregazioni di bit in descrittore di argomento a SFUN */

#define	null		0
#define	A_L			A_LISTA
#define	A_S			A_STRINGA
#define	A_S_L		A_STRINGA + A_LISTA

#ifdef ILWIN
#define	A_A			A_ARRAY 
#define	A_S_L_A		A_STRINGA + A_LISTA + A_ARRAY
#endif // ILWIN

#define	A_B			A_VEROFALSO
#define	A_N			A_NUMERO
#define	A_LW		A_LISTAPAR
#define	A_LW_L		A_LISTAPAR + A_LISTA
#define	A_LW_S		A_LISTAPAR + A_STRINGA
#define	A_LW_S_L	A_LISTAPAR + A_STRINGA + A_LISTA
#define	A_LN		A_LISTANUM
#define	A_LN_L		A_LISTANUM + A_LISTA
#define	A_LN_N_L	A_LISTANUM + A_NUMERO + A_LISTA  
#define	A_LN_LW_L	A_LISTANUM + A_LISTAPAR + A_LISTA
#define	A_W			A_PAROLA
#define	A_W_L		A_PAROLA + A_LISTA
#define	A_W_S		A_PAROLA + A_STRINGA
#define	A_W_S_L		A_PAROLA + A_STRINGA + A_LISTA
#define	A_W_LW_S_L	A_PAROLA + A_LISTAPAR + A_STRINGA + A_LISTA
#define	A_F_S   	A_NOMEARC + A_STRINGA
#define	A_F_S_L		A_NOMEARC + A_STRINGA + A_LISTA
#define	A_F_LW_S_L	A_NOMEARC + A_LISTAPAR + A_STRINGA + A_LISTA

/* aggregazioni di bit in descrittore di argomento a SFUN */


#ifdef ILDOS
#define PR(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg + nmin*4 + ill*16 + fun*32 + prec*64, classe, a1, a2, a3, cfun} 
#define up(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg + nmin*4 + ill*16 + fun*32 + prec*64, classe, a1, a2, a3, nullf} 
#endif // ILDOS

#ifdef ILWIN
// #define PR(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4, classe | ill<<6 | fun<<7, {a1, a2, a3, null, null, null, null, null}, cfun} 
// #define P8(prec,narg,nmin,ill,fun, classe, a1, a2, a3, a4, a5, a6, a7, a8, cfun) {narg | nmin<<4, classe | ill<<6 | fun<<7, {a1, a2, a3, a4, a5, a6, a7, a8}, cfun} 
// #define up(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4, classe | ill<<6 | fun<<7, {a1, a2, a3, null, null, null, null, null}, nullf} 

#ifndef CON_NMASSIMO

#define PR(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4 | ill<<12 | fun<<13, classe, {a1, a2, a3, null, null, null, null, null}, cfun} 
#define P8(prec,narg,nmin,ill,fun, classe, a1, a2, a3, a4, a5, a6, a7, a8, cfun) {narg | nmin<<4 | ill<<12 | fun<<13, classe, {a1, a2, a3, a4, a5, a6, a7, a8}, cfun} 
#define up(prec,narg,nmin,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4 | ill<<12 | fun<<13, classe, {a1, a2, a3, null, null, null, null, null}, nullf} 

#else // CON_NMASSIMO

#define PR(prec,narg,nmin,nmax,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4 | nmax<<8 | ill<<12 | fun<<13, classe, {a1, a2, a3, null, null, null, null, null}, cfun} 
#define P8(prec,narg,nmin,nmax,ill,fun, classe, a1, a2, a3, a4, a5, a6, a7, a8, cfun) {narg | nmin<<4 | nmax<<8 | ill<<12 | fun<<13, classe, {a1, a2, a3, a4, a5, a6, a7, a8}, cfun} 
#define up(prec,narg,nmin,nmax,ill,fun, classe, a1, a2, a3, cfun) {narg | nmin<<4 | nmax<<8 | ill<<12 | fun<<13, classe, {a1, a2, a3, null, null, null, null, null}, nullf} 

#endif // CON_NMASSIMO

#endif // ILWIN

extern void nullf () {
}

struct primitiva descr_sf;	/* copia locale descrizione primitiva */

/* 
   array di descrittori delle primitive MLOGO :
   contiene le informazioni che e' possibile
   codificare in forma sistematica come dati da
   interpretare anziche' come istruzioni
*/

struct	primitiva sis_tab [] =  {

// up(0,0,0,0,o,f, EDIT,      null,     null,  null, numsm), // _NUMSM
// up(0,0,0,0,o,f, EDIT,      null,     null,  null, dammi_sm), // _SM
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, indietro_sm), // _ISM
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, avanti_sm), // _ASM
// up(0,0,0,0,o,f, EDIT,      null,     null,  null, glossa), // _GLOSSA
up(0,0,0,0,o,o, null,      null,     null,  null, ZITTO),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, YCOR),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, XCOR),
PR(0,1,1,1,o,f, null,     A_S_L,     null,  null, VUOTO),
PR(0,1,1,1,o,f, TURT,    A_LN_L,     null,  null, VERSO),
PR(0,1,1,1,o,o, null,       A_B,     null,  null, VERIFICA),
PR(0,1,1,1,o,o, TURT,   t + A_N,     null,  null, VAY),
// PR(0,1,1,1,o,o, TURT,    A_LN_L,     null,  null, VAXY),
PR(0,2,2,2,o,o, TURT,   t + A_N,  t + A_N,  null, VAXY),
PR(0,1,1,1,o,o, TURT,   t + A_N,     null,  null, VAX),
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, afinefinestra), //  VAUR
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, afineriga), //   VAUC
PR(0,0,0,0,o,o, TURT,      null,     null,  null, VATANA),
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, ainiziofinestra), // VAPR
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, ainizioriga), //  VAPC
PR(0,1,1,1,o,o, EDIT,   n + A_N,     null,  null, VAILINEA), //  VAL
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AINIZIOPAROLA), //   VAIP
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AINIZIOLINEA), // VAIL
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AINIZIOTESTO), // VAIF
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AINIZIOBLOCCO), //   VAIB
PR(0,1,1,1,o,o, PROC_,       A_S,     null,  null, VAI),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AFINEPAROLA), //   VAFP
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AFINELINEA), //   VAFL
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AFINETESTO), //   VAFF
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AFINEBLOCCO), //  VAFB
PR(0,1,1,1,o,o, EDIT,     n+A_N,     null,  null, VAICURSORE), //   VAC
PR(0,2,2,2,o,f, null,       A_L,      A_L,  null, UNIONE),
PR(0,0,0,0,o,f, null,      null,     null,  null, ULTLINEA),
PR(0,1,1,1,o,f, null,     A_S_L,     null,  null, ULT),
PR(0,2,2,2,o,f, null,      null,     null,  null, UGUALE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, TRONCALINEA),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, TRACCIA),
up(0,0,0,0,o,f, null,      null,     null,  null, touch_ ),
up(0,0,0,0,o,f, null,      null,     null,  null, touch ),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, TESTO),
PR(0,0,0,0,o,f, null,      null,     null,  null, TEMPO),
PR(0,0,0,0,o,f, null,      null,     null,  null, TAVOLOZZA),
PR(0,0,0,0,o,f, null,      null,     null,  null, TASTO_),
PR(0,1,1,1,o,f, null, r*8 + A_N,     null,  null, TASTO),
up(0,1,1,1,o,o, TXOU,       A_S,     null,  null, TASTIERA),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_TARTA_),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, TANGENTE),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, SVAR),
PR(0,1,1,1,o,o, null,    A_LN_L,     null,  null, SOUND),
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, avantifinestra), // SUF
PR(0,0,0,0,o,f, null,      null,     null,  null, SUCHESCRIVI),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, SUCC),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, SU),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, STUTTO),
// PR(0,2,1,2,i,f, null,       A_S,     null,  null, STRINGA),
PR(0,2,1,2,i,f, null,      null,     null,  null, STRINGA),
PR(0,0,0,0,o,o, PROC_,     null,     null,  null, STOP),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, STITOLI),
up(0,1,1,1,o,f, null,      null,     null,  null, stato),
PR(0,1,1,1,i,o, TXOU,      null,     null,  null, STAMPA),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, SPROC),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, SPOSTABLOCCO),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, SDEF), // SP
PR(0,2,1,2,i,f, null,       A_N,     null,  null, SOMMA),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, SLISTEPROP),
PR(0,1,1,1,o,o, TURT,     t+A_N,     null,  null, SINISTRA),
PR(0,0,0,0,o,f, null,      null,     null,  null, SFONDO),
PR(0,1,1,1,o,o, EXEC,       A_L,     null,  null, SEV),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, SENO),
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, SELEZIONA),
PR(0,1,1,1,o,o, EXEC,       A_L,     null,  null, SEF),
PR(0,2,2,2,o,o, EXEC,       A_B,      A_L,  null, SE),
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, SCRIVISU),
PR(0,1,1,1,o,o, null,       A_N,     null,  null, SCRIVIA),
PR(0,1,1,1,i,o, TXOU,      null,     null,  null, SCRIVI),
PR(0,1,1,1,i,o, TXOU,      null,     null,  null, PR_PRINT_QUOTED),
PR(0,1,1,1,o,o, TXOU,       A_S,     null,  null, SCRICAR),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, SCOPRI),
PR(0,3,2,3,o,f, null,       A_N, r*6+A_N,r*6+A_N, SCIENTIFICA), // SCI
up(0,0,0,0,o,f, TURT,      null,     null,  null, schia),
PR(0,0,0,0,o,f, null,      null,     null,  null, SCHERMO),
up(0,0,0,0,o,f, null,      null,     null,  null, scalafin),
PR(0,1,1,1,o,f, PROC_,     null,     null,  null, RIPORTA),
PR(0,2,2,2,o,o, null,     n+A_N,      A_L,  null, RIPETI),
up(0,0,0,0,o,f, null,      null,     null,  null, righe),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, RIEMPI),
PR(0,1,0,1,o,o, null,       A_N,     null,  null, RIACASO),
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, RESTO),
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, REGISTRA),
up(0,1,1,1,o,o, EDIT,   A_F_S_L,     null,  null, recu_testo),
up(0,1,1,1,o,o, null,     A_W_L,     null,  null, recu_quadro),
PR(0,1,1,1,o,o, TURT,     A_W_L,     null,  null, RECU_DISEGNO),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,      A_S,  null, RECUPERA),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADICE2),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADIANTI),
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, QUOZIENTE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, QUIINIZIOBLOCCO), // QUIIB
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, QUIFINEBLOCCO), //   QUIFB
PR(0,1,1,1,o,o, PROC_,       A_S,     null,  null, NIENTE), //qui
PR(0,1,1,1,o,o, TURT,    A_LN_L,     null,  null, PUNTO),
PR(0,0,0,0,o,o, null,      null,     null,  null, PULISCITESTO),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PULISCISCHERMO),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PULISCI),
PR(0,2,2,2,o,f, null,     A_W_S,    A_W_S,  null, PROP),
PR(0,2,1,2,i,f, null,       A_N,     null,  null, PRODOTTO),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, PRIMITIVA),
PR(0,1,1,1,o,f, null,   A_S_L_A,     null,  null, PRI),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, PREC),
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, POTENZA),
PR(0,0,0,0,o,f, null,       A_N,     null,  null, PIGRECA),
PR(0,3,3,3,o,f, null,     n+A_N,  n + A_N,  A_S_L, PEZZO),
PR(0,0,0,0,o,o, TOPL,      null,     null,  null, PER),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PENNA),
PR(0,0,0,0,o,o, PROC_,      null,     null,  null, PAUSA),
PR(0,1,1,1,o,f, null,      null,     null,  null, PAROLA_),
PR(0,2,1,2,i,f, null,       A_S,     null,  null, PAROLA),
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, PACTUTTO),
PR(0,2,2,2,o,o, null,   A_W_S, A_W_LW_S_L,  null, PAC),
PR(0,1,1,1,o,f, null, A_LN_LW_L,     null,  null, ORDINA),
PR(0,1,2,2,o,f, null,       A_L,      A_N,  null, PR_ORDINA2),	// 010213
PR(0,2,2,2,i,f, null,       A_B,     null,  null, OPPURE),
PR(0,1,1,1,o,f, null,      null,     null,  null, NUMERO),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, NONTRACCIARE),
PR(0,0,0,0,o,o, null,      null,     null,  null, NONREGISTRARE),
PR(0,1,1,1,o,f, null,       A_B,     null,  null, NON),
PR(0,0,0,0,o,f, null,      null,     null,  null, NODI_),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, NASTARTA),
up(0,0,0,0,o,o, EDIT,      null,     null,  null, nasfoglio),
up(0,1,1,1,o,o, null,     A_F_S,     null,  null, nascondi),
#ifdef ILDOS
PR(0,2,2,2,o,f, null,      null,      A_L,  null, MULT),
PR(0,2,2,2,o,f, null,      null,      A_L,  null, MPRI),
#endif // ILDOS
#ifdef ILWIN
PR(0,2,2,2,o,f, null,      null,  A_S_L,  null, MULT),	// 020531: tolto W
PR(0,2,2,2,o,f, null,      null,  A_S_L,  null, MPRI),	// 020531: tolto W
#endif // ILWIN
PR(0,1,1,1,o,o, EDIT,       A_S,     null,  null, METTIPAROLA), //   MP
PR(0,1,1,1,i,o, TXOU,      null,     null,  null, MOSTRA),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, MOSTARTA),
up(0,0,0,0,o,o, EDIT,      null,     null,  null, mosfoglio),
up(0,0,0,0,o,f, null,      null,     null,  null, modo),
PR(0,1,1,1,o,o, EDIT,       A_S,     null,  null, MODCARATTERE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, METTILINEA), //    ML
up(0,1,1,1,o,f, null, r*3 + A_N,     null,  null, misura),
PR(0,1,1,1,o,f, null,     A_S_L,     null,  null, MENULT),
PR(0,1,1,1,o,f, null,     A_S_L,     null,  null, MENPRI),
PR(0,2,2,2,o,f, null,       A_L,      A_L,  null, MENLISTA),
PR(0,1,1,1,o,o, EDIT,       A_S,     null,  null, METTICARATTERE), //   MC
// up(0,1,1,1,o,o, EDIT, r*7 + A_N,     null,  null, marcalinea),
// up(0,1,1,1,o,o, EDIT, r*7 + A_N,     null,  null, marcacarattere),
// up(0,1,1,1,o,o, EDIT, r*7 + A_N,     null,  null, marcablocco),
PR(0,0,0,0,o,f, null,      null,     null,  null, MAIUSCOLE),
PR(0,1,1,1,o,f, null,     A_F_S,     null,  null, LUNGARC),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, LOG_10),
//PR(0,1,1,1,o,o, PROC_,A_W_LW_S_L,     null,  null, LOCALE),
PR(0,1,1,1,i,o, PROC_,A_W_LW_S_L,    null,  null, LOCALE),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, LN),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, LISTAPROP),
PR(0,2,2,2,o,f, null,     A_W_S,    A_W_S,  null, LISTARIF),
PR(0,1,1,1,o,f, null,      null,     null,  null, LISTA_),
PR(0,2,1,2,i,f, null,      null,     null,  null, LISTA),
PR(0,0,0,1,o,o, null,       A_N,     null,  null, LIBERA),	// 010602: maxarg 0 -> 1
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, LIB),
PR(0,0,0,0,o,f, null,      null,     null,  null, LEGGISTR),
PR(0,1,1,1,o,f, TURT,    A_LN_L,     null,  null, LEGGIPUNTO),
PR(0,0,0,0,o,f, null,      null,     null,  null, LEGGIPAROLA),
PR(0,0,0,0,o,f, null,      null,     null,  null, LEGGILISTA),
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, LEGGIDA),
PR(0,0,0,0,o,f, null,      null,     null,  null, LEGGICAR),
PR(0,1,1,1,o,o, null,       A_N,     null,  null, LEGGIA),
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, LANCIA),
//up(0,0,0,0,o,o, EDIT,      null,     null,  null, indriga), //     IR
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, INDPAROLA), //    IP
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_PENREVERSE /*INVERPENNA*/),
PR(0,2,2,2,o,f, null,       A_L,      A_L,  null, INTERSEZIONE),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, INTERO), // INT
PR(0,1,1,1,o,o, TURT,   t + A_N,     null,  null, INDIETRO),
PR(0,0,0,0,o,f, null,      null,     null,  null, INDICE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, INDLINEA), //    IL
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, INDCARATTERE), //   IC
PR(0,1,1,1,o,f, null,       A_N,     null,  null, GRADI),
// up(0,0,0,0,o,o, EDIT,      null,     null,  null, indfinestra), //  GIUF
PR(0,0,0,0,o,o, TURT,      null,     null,  null, GIU),
PR(0,2,1,2,i,f, null,      null,     null,  null, FRASE),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_FOGLIO_),
PR(0,1,1,1,o,f, null,     A_F_S,     null,  null, FINESTRE),
PR(0,0,0,0,o,f, null,      null,     null,  null, FINESCRIVI),
PR(0,0,0,0,o,f, null,      null,     null,  null, FINELEGGI),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, ESP),
PR(0,1,1,1,o,o, EXEC,       A_L,     null,  null, ESEGUI),
PR(0,0,0,0,o,f, null,      null,     null,  null, U_ERRORE),
PR(0,2,2,2,o,f, null,      null,    A_S_L,  null, ELE_),

#ifdef ILDOS
PR(0,2,2,2,o,f, null,   n + A_N,    A_S_L,  null, ELE),
#endif // ILDOS
#ifdef ILWIN
PR(0,2,2,2,o,f, null,       A_N,  A_S_L_A,  null, ELE),
#endif // ILWIN

PR(0,1,1,1,o,o, null,       A_B,     null,  null, ECO),
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DAMMIPAROLA), //    DP
PR(0,0,0,0,o,f, null,      null,     null,  null, DOVESCRIVI),
PR(0,0,0,0,o,f, null,      null,     null,  null, DOVELEGGI),
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DOVELINEA), //   DOVEL
PR(0,0,0,0,o,f, null,      null,     null,  null, DOVEELE),
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DOVECURSORE), // DOVEC
PR(0,0,0,0,o,f, TURT,      null,     null,  null, DOVE),
up(0,1,1,1,o,f, null,       A_L,     null,  null, cdos),
// up(0,0,0,0,o,f, EDIT,      null,     null,  null, dammimarca), //  DM
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DAMMILINEA), //  DL
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, DIVISIONE),
up(0,0,0,0,o,f, null,      null,     null,  null, dito),
PR(0,0,0,0,o,f, null,      null,     null,  null, DISCO),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, DIR),
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, DIFFERENZA),
PR(0,1,1,1,o,o, TURT,   t + A_N,     null,  null, DESTRA),
PR(0,2,2,2,o,o, null,     A_W_S,      A_L,  null, DEFINISCI),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, DEF),
PR(0,3,2,3,o,f, null,       A_N, r*6+A_N,r*6+A_N, DECIMALE), // DEC
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DAMMICARATTERE), // DC
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, DAMMIBLOCCO), //    DB
PR(0,0,0,0,o,f, null,      null,     null,  null, DACHELEGGI),
up(0,1,1,1,o,f, null,     A_W_S,     null,  null, cursore),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, CANCEPAROLA), //CP
// PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, CREALIB),
PR(0,2,2,3,o,o, null,     A_F_S,      A_N,  null, CREALIB),	// 010602: max 2 -> 3
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, COSA_),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, COSA),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, COSENO),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, COPRI),
PR(0,2,2,2,o,o, null,     A_W_S,    A_W_S,  null, COPIADEF),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, COPIABLOCCO),
PR(0,0,0,0,o,o, TOPL,      null,     null,  null, CONTINUA),
PR(0,1,1,1,o,f, null,     A_S_L,     null,  null, CONTA),
up(0,1,1,1,o,o, EDIT,   A_F_S_L,     null,  null, cons_testo),
up(0,1,1,1,o,o, null,     A_W_L,     null,  null, cons_quadro),
PR(0,1,1,1,o,o, TURT,     A_W_L,     null,  null, CONS_DISEGNO),
PR(0,2,1,2,o,o, null,     A_F_S, A_LW_S_L,  null, CONSERVA),
up(0,1,1,1,o,o, null,       A_N,     null,  null, compress),
up(0,0,0,0,o,o, null,      null,     null,  null, compila),
up(0,0,0,0,o,f, null,      null,     null,  null, coltesto),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, COLORE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, CANCELINEA), //CL
PR(0,0,0,0,o,f, null,      null,     null,  null, CIFRE),
PR(0,0,0,0,o,o, null,      null,     null,  null, CIAO),
PR(0,0,0,0,o,o, null,      null,     null,  null, CHIUDITUTTO),
PR(0,1,1,1,o,o, null,A_F_LW_S_L,     null,  null, CHIUDI),
PR(0,2,2,2,o,o, null,      null,    A_W_S,  null, CHIAMA),
PR(0,1,1,1,o,o, EDIT,       A_S,     null,  null, CERCAS),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, CANCECARATTERE), //CC
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, CANCEBLOCCO), //CB
PR(0,1,0,1,o,f, null,    A_LW_S,     null,  null, PR_CATALOGO),
PR(0,2,0,2,o,f, null,		A_S,    A_S,  null, PR_ALBERO),	// 020531: tolto W
PR(0,1,1,1,o,f, null, r*7 + A_N,     null,  null, CARATTERE),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, C_VAR_IN),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, CANCETUTTO),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, C_PRP_IN),
PR(0,2,2,2,o,o, null,     A_W_S,    A_W_S,  null, C_PRP),
PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, C_PRO_IN),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, CANCEPRIM),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_PENERASE /*CANCEPENNA*/),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, C_VAR),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, PR_CANCEFOGLIO),
PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, CANCEARC),
PR(0,1,1,1,o,o, null,A_W_LW_S_L,     null,  null, C_PRO),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, CAMPO),
PR(0,2,2,2,o,o, EDIT,       A_S,      A_S,  null, CAMBIAS),
PR(0,0,0,0,o,o, null,      null,     null,  null, _BREAK),
PR(0,1,1,1,o,o, TURT,   t + A_N,     null,  null, AVANTI),
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, ATTR),
up(0,1,1,1,o,o, null,     A_S_L,     null,  null, astouch),
PR(0,1,1,1,o,o, null,   n + A_N,     null,  null, ASTEMPO),
PR(0,1,1,1,o,o, null, r*1 + A_N,     null,  null, ASTAVOLOZZA),
PR(0,2,2,2,o,o, null, r*8 + A_N,    A_S_L,  null, ASTASTO),
// PR(0,2,2,2,o,f, null,   A_W_S_L,      A_L,  null, ASSOC),
PR(0,2,2,2,o,f, null,      null,     A_L,   null, ASSOC),			// 990927
PR(0,3,3,3,o,f, null,      null,    A_L, r*8+A_N, ASSOC),			// 990927
PR(0,3,3,3,o,f, null,      null,     null,   A_L, PR_SETASSOC),	// 990924, 991015
PR(0,1,1,1,o,f, null,       A_N,     null,  null, ASSO),
//up(0,1,1,1,o,o, TURT, r*7 + A_N,     null,  null, assfscrivi),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, ASSFSCRIVI),
// PR(0,1,1,1,o,o, TURT,n+A_LN_N_L,     null,  null, ASSFONDO),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, ASSFONDO),
up(0,1,1,1,o,o, TURT,       A_N,     null,  null, asschia),
PR(0,1,1,1,o,o, null,       A_S,     null,  null, ASSCHERMO),
up(0,1,1,1,o,o, null,    A_LN_L,     null,  null, asscalafin),
up(0,1,1,1,o,o, null, r*5 + A_N,     null,  null, asrighe),
PR(0,3,3,3,o,o, null,     A_W_S,    A_W_S,  null, ASPROP),
PR(0,1,1,1,o,o, null,     n+A_N,     null,  null, ASPETTA),
PR(0,1,1,1,o,o, TURT,       A_L,     null,  null, ASPENNA),
up(0,1,1,1,o,o, null,       A_N,     null,  null, asmodo),
PR(0,1,1,1,o,o, null,       A_B,     null,  null, ASMAIUSCOLE),
PR(0,2,2,2,o,o, null,     A_W_S,      A_L,  null, ASLISTAPROP),
PR(0,1,1,1,o,o, EDIT,       A_B,     null,  null, ASINS),
PR(0,1,1,1,o,o, null,       A_S,     null,  null, ASDISCO),
PR(0,1,1,1,o,o, TURT,     t+A_N,     null,  null, ASDIR),
//up(0,1,1,1,o,o, null,    A_LN_L,     null,  null, ascursore),
PR(0,1,1,1,o,o, null,    A_F_S,     null,  null, PR_SETCURSOR), // 031030
up(0,1,1,1,o,o, null,       A_L,     null,  null, ascom),
up(0,1,1,1,o,o, null, r*7 + A_N,     null,  null, ascoltesto),
// PR(0,1,1,1,o,o, TURT,n+A_LN_N_L,     null,  null, ASCOL),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, ASCOL),
PR(0,1,1,1,o,f, null, r*7 + A_S,     null,  null, ASCII),
PR(0,1,1,1,o,o, null, r*6 + A_N,     null,  null, ASCIFRE),
PR(0,1,1,1,o,o, TURT,       A_S,     null,  null, ASCAMPO),
// PR(0,1,1,1,o,o, null,       A_L,     null,  null, ASATTR),
PR(0,2,2,2,o,o, null,     A_W_S,      A_L,  null, ASATTR),
PR(0,2,2,2,o,o, null,     A_W_S,     null,  null, AS),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, ARRO),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, ARCTAN),
PR(0,1,1,1,o,f, null,     A_F_S,     null,  null, ARCHIVIO),
PR(0,0,0,0,o,f, null,      null,     null,  null, ARCAPERTI),
//up(0,0,0,0,o,o, EDIT,      null,     null,  null, avantiriga), //AR
//PR(0,1,1,1,o,o, null,A_F_LW_S_L,     null,  null, APRI),
PR(0,1,1,1,o,o, null,   A_F_S_L,     null,  null, APRI),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AVANTIPAROLA), //AP
PR(0,2,2,2,i,f, null,       A_B,     null,  null, AMB),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AVANTILINEA ), //AL
PR(0,1,1,1,o,o, EDIT,       A_B,     null,  null, AGGIORNA),
PR(0,2,2,2,o,o, null,     A_W_S,      A_L,  null, ACCHIAPPA),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, ACASO),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, AVANTICARATTERE), //AC

// up(0,0,0,0,o,f, null,       A_N,     null,  null, _segdos),
// up(0,0,0,0,o,f, null,       A_N,     null,  null, _segprog),
// up(0,0,0,0,o,f, null,       A_N,     null,  null, _segdati),
PR(0,2,2,2,o,o, null,     A_W_S,    A_W_S,  null, _RINOMINA),
// up(0,1,1,1,o,f, null,       A_N,     null,  null, _porta),
// up(0,1,1,1,o,f, null,       A_N,     null,  null, _mem),
// up(0,1,1,1,o,o, null,       A_N,     null,  null, esegui_),
// up(0,1,1,1,o,o, null,       A_N,     null,  null, _asseg),
// up(0,2,2,2,o,o, null,       A_N,      A_N,  null, _asporta),
// up(0,2,2,2,o,o, null,       A_N,      A_N,  null, _asmem),

/* primitive definite nel modulo ILNUMERI */

PR(2,2,1,2,i,f, null,       A_N,     null,  null, SOMMA),
PR(2,2,1,2,o,f, null,       A_N,      A_N,  null, DIFFERENZA),
PR(3,2,1,2,i,f, null,       A_N,     null,  null, PRODOTTO),
PR(3,2,2,2,o,f, null,       A_N,      A_N,  null, DIVISIONE),
PR(3,2,2,2,o,f, null,       A_N,      A_N,  null, POTENZA),

/* primitive definite nel modulo ILLOGICA */

PR(1,2,2,2,o,f, null,      null,     null,  null, UGUALE),
PR(1,2,2,2,o,f, null,      null,     null,  null, MAG),

#ifdef IL89
ini_ele	label byte
PR(1,2,2,o,f, null,      null,     null,  null, MIN),
fin_ele label byte
#endif // IL89
#ifdef ILWIN
PR(1,2,2,2,o,f, null,      null,     null,  null, MIN),

PR(0,2,2,2,o,f, null,      null,     null,  null, MAG),
PR(0,2,2,2,o,f, null,      null,     null,  null, MIN),

PR(0,2,1,2,o,f, null,     n+A_N,      A_N,  null, _ARRAY),
PR(0,1,1,1,o,f, null,      null,     null,  null, ARRAYP),
// PR(0,3,3,3,o,o, null,       A_N,      A_A,  null, SETITEM),
PR(0,3,3,3,o,o, null,       A_N,  A_S_L_A,  null, SETITEM),

PR(0,1,1,1,o,f, null,       A_L,     null,  null, PR_FIRSTS),
PR(0,1,1,1,o,f, null,       A_L,     null,  null, PR_BUTFIRSTS),

PR(0,1,1,1,o,f, null,   A_S_L,     null,  null, REVERSE),	// 020531: tolto W

PR(0,1,1,1,o,f, null,     A_S,     null,  null, LOWERCASE),	// 020531: tolto W
PR(0,1,1,1,o,f, null,     A_S,     null,  null, UPPERCASE),	// 020531: tolto W

P8(0,5,5,5,o,o, null, A_L, A_L, A_L, A_L, A_L, null, null, null, MOUSEON),
PR(0,0,0,0,o,o, null,      null,     null,  null, MOUSEOFF),
PR(0,0,0,0,o,f, null,      null,     null,  null, MOUSEPOS),
// PR(0,0,0,0,o,o, null,      null,     null,  null, CLEARTEXT),

PR(0,1,1,1,o,f, null,     A_S,     null,  null, PARSE),		// 020531: tolto W
PR(0,1,1,1,o,f, null,   A_S_L,     null,  null, RUNPARSE),	// 020531: tolto W

PR(0,1,1,1,o,f, null,       A_N,     null,  null, READCHARS),

PR(0,1,1,1,o,f, null,       A_N,     null,  null, MINUS),

PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADSIN),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADCOS),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADTAN),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, RADARCTAN),

PR(0,2,2,2,i,f, null,       A_B,     null,  null, XOR),
PR(2,2,2,2,i,f, null,       A_N,      A_N,  null, BITAND),
PR(2,2,2,2,i,f, null,       A_N,      A_N,  null, BITOR),
PR(2,2,2,2,i,f, null,       A_N,      A_N,  null, BITXOR),
PR(2,1,1,1,o,f, null,       A_N,     null,  null, BITNOT),
PR(2,2,2,2,o,f, null,       A_N,      A_N,  null, ASHIFT),
PR(2,2,2,2,o,f, null,       A_N,      A_N,  null, LSHIFT),

PR(0,2,2,2,o,o, TURT,       A_N,       A_N,  null, ARC),

PR(0,0,0,0,o,o, TURT,      null,     null,  null, WRAP),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, WINDOW),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, FENCE),

PR(0,1,1,1,o,o, TURT,   A_W_S_L,     null,  null, LABEL),
PR(0,1,1,1,o,o, TURT,       A_L,     null,  null, SETTEXTFONT),
PR(0,1,1,1,o,o, TURT,       A_L,     null,  null, PR_SETTEXTFONTLOG),	// 991223
PR(0,0,0,0,o,f, TURT,      null,     null,  null, TEXTFONT),

PR(0,0,0,0,o,o, TURT,      null,     null,  null, TEXTSCREEN),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, FULLSCREEN),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, SPLITSCREEN),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, REFRESH),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, NOREFRESH),

PR(0,0,0,0,o,f, TURT,      null,     null,  null, TURTLEWINDOW),
PR(0,1,1,1,o,o, TURT,      null,     null,  null, ZOOM),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, RESIZE),

PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_PENPAINT /*PENPAINT*/),
// PR(0,1,1,1,o,o, TURT,n+A_LN_N_L,     null,  null, SETFLOODCOLOR),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, SETFLOODCOLOR),
PR(0,1,1,1,o,o, TURT,n+A_LN_N_L,     null,  null, SETPENSIZE),
PR(0,1,1,1,o,o, TURT,     n+A_N,     null,  null, SETPENPATTERN),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, CLEARPALETTE),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PENDOWNP),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PR_PENMODE /*PENMODE*/),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, FLOODCOLOR),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PENSIZE),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PENPATTERN),

PR(0,1,1,1,o,o, EXEC,       A_L,     null,  null, RUNRESULT),
PR(0,0,0,0,o,f, EXEC,      null,     null,  null, REPCOUNT),
PR(0,3,3,3,o,o, EXEC,       A_B,      A_L,   A_L, IFELSE),
PR(0,0,0,0,o,o, EXEC,      null,     null,  null, YIELD),
PR(0,0,0,0,o,o, EXEC,      null,     null,  null, NOYIELD),
// PR(0,0,0,0,o,o, EXEC,      null,     null,  null, EVENTCHECK),
up(0,0,0,0,o,o, EXEC,      null,     null,  null, EVENTCHECK),
up(0,2,2,2,o,f, EXEC,     A_W_S,      A_L,  null, APPLY),
up(0,0,0,0,o,o, TOPL,      null,     null,  null, .MACRO),
up(0,2,2,2,o,o, null,     A_W_S,      A_L,  null, .DEFMACRO),
up(0,1,1,1,o,f, null,     A_W_S,     null,  null, MACROP),

PR(0,1,1,1,o,o, null,       A_S,     null,  null, CHDIR),
PR(0,0,0,0,o,f, null,      null,     null,  null, CURDIR),

PR(0,0,0,0,o,f, PGUI,      null,     null,  null, PR_DIALOGHI),
// P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_W_S_L, n+A_N, n+A_N, n+A_N, n+A_N, A_L, WINDOWCREATE),
P8(0,8,7,8,o,o, PGUI, A_W_S, A_W_S, A_S_L, n+A_N, n+A_N, n+A_N, n+A_N, A_L, WINDOWCREATE),	// 020531: tolto W in 3
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, WINDOWDELETE),

P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_S_L, n+A_N, n+A_N, n+A_N, n+A_N, A_L, DIALOGCREATE),	// 020531: tolto W in 3
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, DIALOGDELETE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, DIALOGSETFOCUS),

P8(0,6,6,6,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, null, null, LISTBOXCREATE),
P8(0,6,6,6,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, null, null, LISTBOXCREATERAW),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, LISTBOXDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, LISTBOXGETSELECT),
PR(0,2,2,2,o,o, PGUI,     A_W_S,  A_S_L,  null, LISTBOXADDSTRING),	// 020531: tolto W in 2
PR(0,2,2,2,o,o, PGUI,     A_W_S,    n+A_N,  null, LISTBOXDELETESTRING),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, LISTBOXGETINDEX),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, LISTBOXRESET),

//P8(0,6,6,6,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, null, null, EDITBOXCREATE),
P8(0,6,6,7,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, A_LN_L, null, EDITBOXCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null,EDITBOXDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, EDITBOXGETTEXT),
PR(0,2,2,2,o,o, PGUI,     A_W_S,  A_S_L,  null, EDITBOXSETTEXT),	// 020531: tolto W in 2

P8(0,6,6,6,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, null, null, COMBOBOXCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, COMBOBOXDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, COMBOBOXGETTEXT),
PR(0,2,2,2,o,o, PGUI,     A_W_S,  A_S_L,  null, COMBOBOXSETTEXT),	// 020531: tolto W in 2
PR(0,2,2,2,o,o, PGUI,     A_W_S,  A_S_L,  null, COMBOBOXADDSTRING),	// 020531: tolto W in 2
PR(0,2,2,2,o,o, PGUI,     A_W_S,    n+A_N,  null, COMBOBOXDELETESTRING),

P8(0,7,7,7,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, A_L, null, SCROLLBARCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, SCROLLBARDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,      A_N,   A_N, SCROLLBARGET),
PR(0,4,4,4,o,o, PGUI,     A_W_S,    A_W_S,  null, SCROLLBARSET),

P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_W_S_L, n+A_N, n+A_N, n+A_N, n+A_N, A_L, BUTTONCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, BUTTONDELETE),

P8(0,7,7,7,o,o, PGUI, A_W_S, A_W_S,   A_W_S_L, n+A_N, n+A_N, n+A_N, n+A_N, null, STATICCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, STATICDELETE),
PR(0,2,2,2,o,o, PGUI,     A_W_S,  A_S_L,  null, STATICUPDATE),	// 020531: tolto W in 2

P8(0,6,6,6,o,o, PGUI, A_W_S, A_W_S, n+A_N, n+A_N, n+A_N, n+A_N, null, null, GROUPBOXCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, GROUPBOXDELETE),

P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_W_S, A_S_L, n+A_N, n+A_N, n+A_N, n+A_N, CHECKBOXCREATE),	// 020531: tolto W in 4
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, CHECKBOXDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, CHECKBOXGET),
PR(0,2,2,2,o,o, PGUI,     A_W_S,    A_W_S,  null, CHECKBOXSET),

P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_W_S, A_S_L, n+A_N, n+A_N, n+A_N, n+A_N, RADIOBUTTONCREATE),	// 020531: tolto W in 4
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, RADIOBUTTONDELETE),
PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, RADIOBUTTONGET),
PR(0,2,2,2,o,o, PGUI,     A_W_S,    A_W_S,  null, RADIOBUTTONSET),

PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, DEBUGWINDOWS),
PR(0,2,2,2,o,o, PGUI,       A_L,      A_L,  null, MESSAGEBOX),
// PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, DIALOGFILEOPEN),
// PR(0,1,1,1,o,f, PGUI,     A_W_S,     null,  null, DIALOGFILESAVE),
PR(0,2,1,2,o,f, PGUI,     A_W_S,     null,  null, DIALOGFILEOPEN),
PR(0,2,1,2,o,f, PGUI,     A_W_S,     null,  null, DIALOGFILESAVE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,      A_L,  null, WINDOWFILEEDIT),

PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, BITCUT),
PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, BITCOPY),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, BITPASTE),
PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, BITFIT),
PR(0,1,1,1,o,o, TURT,       A_N,     null,  null, SETBITINDEX),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, BITINDEX),
PR(0,1,1,1,o,o, TURT,       A_N,     null,  null, SETBITMODE),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, BITMODE),
PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, BITBLOCK),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_TURTLEPRINT), // gia' BITPRINT
//>040123
// PR(0,1,1,1,o,o, TURT,     A_W_S,     null,  null, BITLOAD),
// PR(0,1,1,1,o,o, TURT,     A_W_S,     null,  null, BITSAVE),
PR(0,1,1,2,o,o, TURT,     A_W_S,    A_W_S,  null, PR_BITLOAD),
PR(0,1,1,1,o,o, TURT,     A_W_S,     null,  null, PR_TURTLELOAD),
// PR(0,1,1,3,o,o, TURT,     A_W_S,    A_W_S, A_LN_L, PR_BITSAVE),
P8(0,1,1,5,o,o, TURT,     A_W_S,    A_W_S, n+A_N, A_LN_L, A_LN_L, null, null, null, PR_BITSAVE),	// 040130
PR(0,1,1,1,o,o, TURT,     A_W_S,     null,  null, PR_TURTLESAVE),
//<040123
PR(0,1,1,1,o,o, TURT,n+A_LN_N_L,     null,  null, SETACTIVEAREA),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, ACTIVEAREA),

PR(0,3,3,3,o,o, null,     n+A_N,      A_N,   A_L, SETTIMER),
PR(0,1,1,1,o,o, null,     n+A_N,     null,  null, CLEARTIMER),

PR(0,0,0,0,o,f, null,      null,     null,  null, PR_MIDIOUTNUMDEVS),
PR(0,1,0,1,o,f, null,     n+A_N,     null,  null, MIDIOPEN),
PR(0,0,0,0,o,o, null,      null,     null,  null, MIDICLOSE),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_MIDI),
PR(0,1,1,1,o,o, null,n+A_LN_N_L,     null,  null, MIDIMESSAGE),
PR(0,0,0,0,o,o, null,      null,     null,  null, PLAYWAVE),
// PR(0,1,1,1,o,o, null,       A_L,     null,  null, MCI),
PR(0,1,1,1,o,o, null,   A_S_L,     null,  null, MCI),	// 020531: tolto W

PR(0,1,0,1,o,o, null,A_W_LW_S_L,     null,  null, EDITA),

PR(0,1,1,1,o,o, null,   A_W_S_L,     null,  null, PR_MINIMIZE),
PR(0,1,1,1,o,o, null,   A_W_S_L,     null,  null, PR_MAXIMIZE),
PR(0,1,1,1,o,o, null,   A_W_S_L,     null,  null, PR_NORMALIZE),
PR(0,1,1,1,o,o, null,   A_W_S_L,     null,  null, PR_RESTORE),
PR(0,1,1,1,o,o, null,   A_W_S_L,     null,  null, PR_SETFOCUS),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_GETFOCUS),

PR(0,1,1,1,o,o, null,       A_L,     null,  null, PR_KBON),
PR(0,0,0,0,o,o, null,      null,     null,  null, PR_KBOFF),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_KBVAL),

up(0,1,1,1,o,o, null,     n+A_N,     null,  null, PR_SCROLLX),
up(0,1,1,1,o,o, null,     n+A_N,     null,  null, PR_SCROLLY),

PR(0,0,0,0,o,f, null,      null,     null,  null, PR_EMPTYWORD),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_EMPTYLIST),

PR(0,1,1,1,o,o, TURT,    A_LN_L,     null,  null, PR_VAPOS),

PR(0,1,1,1,o,f, null,       A_L,     null,  null, PR_LASTS),
PR(0,1,1,1,o,f, null,       A_L,     null,  null, PR_BUTLASTS),

PR(0,1,1,1,o,f, null,      null,     null,  null, PR_DUPLICATE),
PR(0,2,2,2,o,f, null,      null,  A_S_L,  null, PR_COMBINE),	// 020531: tolto W
PR(0,2,2,2,o,f, null,      null,  A_S_L,  null, PR_REMOVE),	// 020531: tolto W
PR(0,1,1,1,o,f, null,       A_L,     null,  null, PR_REMDUP),
// PR(0,1,1,1,o,f, null,       A_S,     null,  null, PR_QUOTED),
PR(0,1,1,1,o,f, PMCI,     A_W_S,     null,  null, PR_QUOTED),

PR(0,1,1,1,o,f, null,       A_N,     null,  null, PR_SIGN),

PR(0,0,0,0,o,f, null,      null,     null,  null, PR_GENSYM),

PR(0,1,1,1,o,o, TURT,     A_W_S,     null,  null, PR_SETPENMODE),

PR(0,2,1,2,o,o, null,     A_W_S,     A_L,  null, PR_CREATARTA),
PR(0,2,1,2,o,o, null,     A_W_S,     A_L,  null, PR_CREAFOGLIO),
PR(0,2,1,2,o,o, null,     A_W_S,     A_L,  null, PR_CREABROWSER),
PR(0,2,1,2,o,o, null,     A_W_S,     A_L,  null, PR_CREAPLAYER),

PR(0,0,0,0,o,o, null,      null,    null,  null, PR_CHIUDITARTA),
PR(0,0,0,0,o,o, null,      null,    null,  null, PR_CHIUDIFOGLIO),
PR(0,0,0,0,o,o, null,      null,    null,  null, PR_CHIUDIBROWSER),
PR(0,0,0,0,o,o, null,      null,    null,  null, PR_CHIUDIPLAYER),

PR(0,0,0,0,o,f, null,      null,     null,  null, PR_TARTA),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_FOGLIO),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_BROWSER),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_PLAYER),

// PR(0,1,1,1,o,o, EDIT,     A_W_L,     null,  null, PR_RECUFOGLIO),
PR(0,1,0,1,o,o, EDIT,   A_W_S_L,     null,  null, PR_RECUFOGLIO),
// PR(0,1,1,1,o,o, EDIT,     A_W_L,     null,  null, PR_CONSFOGLIO),
PR(0,1,1,1,o,o, EDIT,     A_W_S,     null,  null, PR_CONSFOGLIO),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOESEGUITUTTO),

PR(0,1,1,1,o,f, TURT,   A_S_L,     null,  null, PR_LABELDIM),	// 020531: tolto W
PR(0,1,1,1,o,f, TURT,   A_S_L,     null,  null, PR_LABELDIMLOG),	// 020531: tolto W

P8(0,8,8,8,o,o, PGUI, A_W_S, A_W_S, A_W_S_L, A_W_S_L, A_LN_N_L, A_LN_L, A_LN_L, A_L, PR_TOOLBARCREATE),
PR(0,1,1,1,o,o, PGUI,     A_W_S,     null,  null, PR_TOOLBARDELETE),
PR(0,2,2,2,o,o, PGUI,     A_W_S, A_LN_N_L,  null, PR_TOOLBARSETPOS),

PR(0,1,1,1,o,o, null,     A_F_S,     null,  null, PR_WINDOWSHOW),

PR(0,2,2,2,o,f, null,   A_W_S_L,  A_S_L,  null, PR_DAOQUERY),	 // 020531: tolto W in 2
PR(0,2,2,2,o,o, null,   A_W_S_L,  A_S_L,  null, PR_DAOEXECUTE),	// 020531: tolto W in 2
PR(0,1,1,1,o,f, null,   A_S_L,     null,  null, PR_DOCLIST),	// 020531: tolto W
PR(0,1,1,1,o,o, null,   A_S_L,     null,  null, PR_SHOWHELP),	// 020531: tolto W
// PR(0,3,0,o,f, null,     A_F_S,    A_F_S, A_F_S, PR_GLOSSARY),
// PR(0,3,0,o,o, null,     A_F_S,    A_F_S, A_F_S, PR_SHOWGLOSSARY),
P8(0,6,0,6,o,f, null, A_S, A_S, A_S, A_B, A_B, A_B, null, null, PR_GLOSSARY),	// 020531: tolto W in 1,2,3
P8(0,6,0,6,o,o, null, A_S, A_S, A_S, A_B, A_B, A_B, null, null, PR_SHOWGLOSSARY),	// 020531: tolto W in 1,2,3

PR(0,0,0,0,o,f, EDIT,      null,     null,  null, CONTACARATTERI),
PR(0,0,0,0,o,f, EDIT,      null,     null,  null, CONTALINEE),

PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOIMPOSTASTAMPANTE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOSTAMPA),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOTAGLIA),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOCOPIA),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOINCOLLA),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOIMPOSTAFONT),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOESEGUISELEZIONE),
PR(0,0,0,0,o,o, EDIT,      null,     null,  null, FOGLIOSELEZIONABLOCCO),

PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_RIEMPISELEZIONE),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_SELEZIONAREATTIVA),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_ATTIVASELEZIONE),
PR(0,0,0,0,o,o, TURT,      null,     null,  null, PR_CENTRA),

PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, BITPRINT),

PR(0,2,2,2,o,f, null,     A_F_S,    A_F_S,  null, PR_COPIAARC),
PR(0,2,2,2,o,f, null,     A_F_S,    A_F_S,  null, PR_RINOMINAARC),
// PR(0,0,0,0,o,f, null,      null,     null,  null, PR_MIDI),
PR(0,1,1,1,i,f, null,     A_W_L,     null,  null, PR_PROCESSOCREA),

PR(0,2,2,2,o,o, null,     A_W_S,     A_L,   null, PR_MENUCREATE),
PR(0,1,1,1,o,o, null,     A_W_S,    null,   null, PR_MENUDELETE),
PR(0,1,1,1,o,f, null,     A_W_S,    null,   null, PR_MENU),
PR(0,2,2,2,o,f, null,     A_W_S,A_LN_N_L,   null, PR_MENUELEMENT),
PR(0,3,3,3,o,o, null,     A_W_S,A_LN_N_L,    A_L, PR_MENUELEMENTSET),
PR(0,2,2,2,o,o, null,     A_W_S,A_LN_N_L,   null, PR_MENUELEMENTREMOVE),
PR(0,3,3,3,o,o, null,     A_W_S,A_LN_N_L,    A_L, PR_MENUELEMENTREPLACE),

// ---------

PR(0,1,1,1,o,o, null,      null,     null,  null, PR_CONSUMA),

PR(0,1,1,1,o,o, TURT,    A_LN_L,     null,  null, PR_SETTURTLEPROPS),
// PR(0,1,0,1,o,f, TURT,       A_N,     null,  null, PR_TURTLEPROPS),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PR_GETTURTLEPROPS),
PR(0,1,0,1,o,f, TURT,       A_N,     null,  null, PR_TURTLETOOLS),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PR_GO_SELECTION),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, PR_GO_REMOVE),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_GO_DUPLICATE),
PR(0,2,2,2,o,o, TURT,       A_N, A_LN_N_L,  null, PR_GO_CENTER),
PR(0,1,1,1,o,f, TURT,     A_F_S,     null,  null, PR_SYMBOLLOAD),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_NODECREATE),
// PR(0,1,1,1,o,f, TURT,     A_F_S,     null,  null, PR_SYMBOL_INSERT),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_NODEINSERT),
PR(0,2,2,2,o,f, TURT,       A_N,      A_N,  null, PR_NODICONNETTI),
PR(0,1,1,1,o,o, TURT,       A_N,     null,  null, PR_ARCOINVERTI),
// PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_GO_POS),
PR(0,1,1,2,o,f, TURT,       A_N,      A_N,  null, PR_GO_POS),
// PR(0,2,2,2,o,o, TURT,       A_N, A_LN_N_L,  null, PR_GO_SETPOS),
PR(0,2,2,3,o,o, TURT,       A_N, A_LN_N_L,   A_N, PR_GO_SETPOS),
PR(0,2,2,2,o,o, TURT,       A_N, A_LN_N_L,  null, PR_COMPTRANSLA),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_COMPDIR),
// PR(0,3,2,3,o,o, TURT,       A_N,   A_N, A_LN_N_L, PR_COMPASDIR),
PR(0,2,2,3,o,o, TURT,       A_N,   A_N, A_LN_N_L, PR_COMPASDIR),
// PR(0,3,2,3,o,o, TURT,       A_N,   A_N, A_LN_N_L, PR_COMPRUOTA),
PR(0,2,2,3,o,o, TURT,       A_N,   A_N, A_LN_N_L, PR_COMPRUOTA),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_COMPSCALA),
// PR(0,3,2,3,o,o, TURT,       A_N,A_LN_N_L,A_LN_N_L, PR_COMPASSCALA),
PR(0,2,2,3,o,o, TURT,       A_N,A_LN_N_L,A_LN_N_L, PR_COMPASSCALA),
PR(0,1,1,1,o,f, TURT,  A_LN_N_L,     null,  null, PR_GO_GROUP),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_GO_ELEMENTS),
PR(0,1,1,1,o,o, TURT,       A_N,     null,  null, PR_GO_UNGROUP),
PR(0,2,2,2,o,o, TURT,       A_N,      A_L,  null, PR_GO_ON),
PR(0,1,1,1,o,o, TURT,       A_N,     null,  null, PR_GO_OFF),
PR(0,0,0,0,o,f, TURT,      null,     null,  null, PR_GO_ID),

PR(0,0,0,0,o,f, null,      null,     null,  null, PR_VERSION),
//>040205
// PR(0,0,0,3,o,f, null,     A_W_S,    A_W_S,   A_N, PR_SIGNATURE),	// 010526
P8(0,0,0,4,o,f, null, A_W_S, A_W_S, A_N, A_N, null,null,null,null, PR_SIGNATURE),
//<040205
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_ROOT),

PR(0,0,0,0,o,o, null,      null,     null,  null, PR_PAGESETUP),
PR(0,0,0,0,o,o, null,      null,     null,  null, PR_TURTLEPRINTPREVIEW),

PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, PR_GO_SELECT),
PR(0,1,1,1,o,f, TURT,       A_L,     null,  null, PR_GO_POLYLINE),
PR(0,1,1,1,o,f, TURT,       A_L,     null,  null, PR_GO_POLYGON),
PR(0,1,1,1,o,f, TURT,       A_L,     null,  null, PR_GO_CURVE),
PR(0,1,1,1,o,f, TURT,       A_L,     null,  null, PR_GO_CLOSEDCURVE),
PR(0,2,2,2,o,f, TURT,       A_N,      A_N,  null, PR_GO_RECT),
PR(0,2,2,2,o,f, TURT,       A_N,      A_N,  null, PR_GO_ELLIPSE),

P8(0,4,4,4,o,f, TURT,A_N,A_N,n+A_LN_N_L,A_S_L, null,null,null,null, PR_SYMBOL_INSERTLABEL),	// 020531: tolto W
PR(0,2,2,2,o,o, TURT,       A_N,      A_N,  null, PR_SYMBOL_REMOVELABEL),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_SYMBOL_LABELNUMBER),

PR(0,1,1,1,o,f, null,   A_S_L,     null,  null, PR_PLAYERCOMMAND),	// 020531: tolto W

PR(0,2,2,2,o,f, TURT,       A_N,      A_N,  null, PR_GO_APPEND),
PR(0,1,1,3,o,f, TURT,       A_N, A_LN_N_L,   A_N, PR_GO_DRAW),
PR(0,1,0,1,o,f, TURT,  A_LN_N_L,     null,  null, PR_GO_SYMBOL),
PR(0,2,2,2,o,o, TURT,       A_N,  A_S_L,  null, PR_GO_SETTYPE),	// 020531: tolto W
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_SYMBOLINSERT),

PR(0,1,1,3,o,f, TURT,   A_S_L, A_LN_N_L,  A_LN_L, PR_GO_LABEL),	// 020531: tolto W
P8(0,2,2,4,o,f, TURT,A_N,A_N,A_LN_N_L,A_N,  null,null,null,null, PR_SYMBOL_SETLABEL),
PR(0,1,1,3,o,f, TURT,   A_W_S_L, A_LN_N_L,  A_LN_L, PR_GO_TEXT),
PR(0,1,1,1,o,f, TURT,     A_F_S,     null,  null, PR_GO_IMAGE),
PR(0,1,1,1,o,f, TURT,       A_N,     null,  null, PR_GO_IMAGE_DIM),
PR(0,1,1,1,o,o, TURT,  A_LN_N_L,     null,  null, PR_SETPIXEL),
PR(0,1,1,1,o,o, TURT,       A_B,     null,  null, PR_GO_SETPANSELECT),

PR(0,2,2,2,o,o, null,     A_W_S,     null,  null, PR_LOCALMAKE),

PR(0,1,1,1,o,o, null,       A_S,     null,  null, PR_CREACARTELLA),
PR(0,1,1,1,o,o, null,       A_S,     null,  null, PR_RIMUOVICARTELLA),
PR(0,0,0,0,o,o, null,      null,     null,  null, PR_RISALICARTELLA),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CARTELLE),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_ARCHIVI),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_DRIVETYPES),
PR(0,1,1,1,o,f, null,       A_S,     null,  null, PR_DRIVEREADY),

P8(0,7,3,7,o,f, null,A_S,A_S,A_S,A_S,A_S,A_S,A_N,null, PR_SHELL_LINKCREATE),
PR(0,2,2,2,o,f, null,       A_N,      A_N,  null, PR_SHELL_GETFOLDERPATH),
PR(0,1,1,2,o,f, null,     A_S,    A_S,  null, PR_SHELL_COMMANDLINE),	// 020531: tolto W in 1,2

PR(0,1,1,3,o,o, null,     A_W_S,    A_W_S,  A_W_S, PR_FTP_APRI), // 040221
PR(0,0,0,0,o,o, null,     A_W_S,    A_W_S,  A_W_S, PR_FTP_CHIUDI), // 040221
P8(0,1,1,5,o,o, null,A_W_S,A_W_S,A_W_S,A_B,A_N,null,null,null, PR_FTP_GET), // 040221
PR(0,1,1,3,o,o, null,     A_W_S,    A_W_S,  A_W_S, PR_FTP_PUT), // 040221
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, PR_FTP_CANCEARC), // 040222
PR(0,2,2,2,o,o, null,     A_W_S,    A_W_S,  null, PR_FTP_RINOMINAARC), // 040222
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_FTP_CARTELLA), // 040222
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, PR_FTP_CAMBIACARTELLA), // 040222
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, PR_FTP_CREACARTELLA), // 040222
PR(0,1,1,1,o,o, null,     A_W_S,     null,  null, PR_FTP_RIMUOVICARTELLA), // 040222
PR(0,1,0,1,o,f, null,    A_LW_S,     null,  null, PR_FTP_CATALOGO), // 040228
PR(0,2,0,2,o,f, null,		A_S,      A_S,  null, PR_FTP_ALBERO), // 040228
PR(0,1,1,1,o,f, null,     A_W_S,     null,  null, PR_FTP_ATTR), // 040228

#ifdef INTEGRA
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CONF_STATO),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CONF_INIZIA),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CONF_TERMINA),
PR(0,1,1,3,o,f, null,       A_N,      A_L,   A_L, PR_CONF_CONFCREA),
P8(0,2,2,4,o,f, null,A_N,A_S,A_L,A_L,null,null,null,null, PR_CONF_CHIAMA),	// 020531: tolto W
PR(0,2,2,2,o,f, null,       A_N,     A_L,  null, PR_CONF_VIDEOAPRI),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, PR_CONF_VIDEOCHIUDI),
PR(0,0,0,1,o,f, null,       A_L,     null,  null, PR_CONF_DATIAPRI),
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CONF_DATICHIUDI),
PR(0,1,1,1,o,f, null,   A_S_L,     null,  null, PR_CONF_DATIINVIA),	// 020531: tolto W
PR(0,0,0,0,o,f, null,      null,     null,  null, PR_CONF_BUFFER),

PR(0,1,1,2,o,f, null,     A_W_S,      A_L,  null, PR_VOCE_ACCENDI),
PR(0,0,0,0,o,f, null,     A_W_S,     null,  null, PR_VOCE_SCRIVI),
PR(0,1,1,1,o,f, null,       A_N,     null,  null, PR_VOCE_DIALOGO),
#endif // INTEGRA

#endif // ILWIN
};

#ifdef IL89

public	fine_sis
fine_sis label byte
 
public	l_el_sis
l_el_sis dw offset fin_ele - offset ini_ele	;/* lunghezza elemento */

void ini (){
  sis_tab
};

#endif // IL89
 