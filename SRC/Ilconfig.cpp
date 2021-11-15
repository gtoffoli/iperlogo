/* ILCONFIG.CPP	G. Toffoli

040228	aggiunto FTP_CATALOGO, FTP_ALBERO e FTP_ATTR (vuoto)
040222	aggiunto FTP_CANCEARC, FTP_RINOMINAARC,"FTP_CARTELLA, FTP_CAMBIACARTELLA, FTP_CREACARTELLA, FTP_RIMUOVICARTELLA
040221	aggiunto FTP_APRI, FTP_CHIUDI, FTP_GET, FTP_PUT
040123	nuovi BLOCCORECUPERA e BLOCCOCONSERVA, non piu' sinonimi di TARTARECUPERA e TARTACONSERVA

020208	aggiunto parola chiave CONFERMA (dialoghi di uscita)
010606	COMANDOSHELL dopo CARTELLASHELL
010526	aggiunto FIRMA (SIGNATURE) dopo VERSIONE
010213	aggiunto ORDINA2 dopo ORDINA
010128	VOCEDIALOGO
010122	agg. CONFDATIRICEVUTI; VOCEACCETTA -> VOCESCRIVI
000901/20
000822/25	aggiunto primitive INTEGRA
000705	agg. parola chiave CONSERVA (per PLAYER)
000508	aggiunto OGIMMAGINE (PR_GO_IMAGE) e OGIMMAGINEDIM (PR_GO_IMAGE_DIM)
000313	aggiunto OG_ASPANNING (PR_GO_SETPANSELECT) dopo ASPIXEL
000310	aggiunto CARTELLASHELL (PR_SHELL_GETFOLDERPATH)
000221	aggiunto CREACOLLEGAMENTO (PR_SHELL_LINKCREATE)
000202	aggiunto LISTASCELTAAZZERA (LISTBOXRESET); corretto LISTASCELTASCEGLI -> LISTASCELTASELEZIONE
000131	aggiunto DISCHI (PR_DRIVETYPES) e DISCOPRONTO? (PR_DRIVEREADY)
000130	mod. nomi primitive etichette e link
000119	modificato messaggio di errore 11
000118	aggiunto CREACARTELLA, RIMUOVICARTELLA, RISALICARTELLA, CARTELLE, ARCHIVI
000113	aggiunto ASPIXEL e ASLOCALE
000102	ripristinato parola chiave MODO e spostato IGNORACASE?
991228	aggiunto sinonimi con punto per ASELEMENTO e ASSOCIA
		ritoccato nomi delle primitive di CASELLATESTO
991224	aggiunto OGTESTO
991223	aggiunto ASFONTESTOLOG dopo ASFONTESTO e ETICHETTADIMLOG dopo ETICHETTADIM
991217	aggiunto CATALOGO2 (gia' ALBERO) dopo CATALOGO
991214	aggiunto DATABASEESEGUI dopo DATABASEINTERROGA
>	991213	recupero da ilsis.cpp:
991212	inserito DIALOGOASFUOCO dopo DIALOGOCANCELLA
991208	aggiunto OGLABEL e OGAGGIUNGILABEL
991202	aggiunto SIMBOLOINSERISCI, simile a NODOINSERISCI, ma senza copia
991130	agg. OGAPPENDI, OGDISEGNA, OGSIMBOLO, OGASTIPO
<
991124	abilitato primitive di PLAYER; aggiunto PR_PLAYERMCI
991116	agg. parola chiave SELEZIONE;
		agg. funzione MIDICONTA prima di MIDIAPRI; spostato MIDI
991105	inserito DIALOGHI prima di FINESTRACREA
991104	inserito 6 primitive dopo MENUCREA (PR_MENUCREATE)
991102	inserito DUPLICA (PR_DUPLICATE) prima di COMBINA (PR_COMBINE)
991028	OG(INSERISCI)LABEL, OGRIMUOVILABEL e OGCONTALABEL
991027	inserito OGASPROP (PR_SETTURTLEPROPS)
991025	inserito NODOCREA
991021	agg. OGSELEZIONA, OGRETTANGOLO, OGLABEL e altre 5
991014	aggiunto IMPOSTAPAGINA e TARTAANTEPRIMA
991012	inserito 4 primitive per EDITBOX (CASELLATESTO) prima di COMBOBOX
		inserito SCRILETT (SCRIVILETTERALE, cioè PR_PRINT_QUOTED) dopo scrivi
991011	aggiunto OGID
991005	ASELEMENTO sinonimo di ASELE, RIP sinonimo di CONTARIPETIZIONI
		INULT sinonimo di INULTIMO, INPRI sinonimo di INPRIMO
		primitiva RADICE
990927	aggiunto ASSOCELE (ASSOC a 3 argomenti)
990925	aggiunto PR_VERSION (VERSIONE)
990924	inserito PR_SETASSOC (ASSOCIA)
990923	inserito NODOINSERISCI (PR_SYMBOL_INSERT)
990920	aggiunto OGACCENDI e OGSPEGNI
990917	inserito PULSANTIERAASPOS, TARTAPROP e TARTATOOL
990916	aggiunto MENUCREA
990915	aggiunto numerose primitive del tipo OG...
990913	aggiunto primitive per diagrammi e oggetti grafici
990728	aggiunto PROCESSOCREA
990722	aggiunto parole chiave RADICE e ATTR
990714	aggiunto sinonimo LETT a LETTERALE
990707	modificato terminologia relativa a GLOSSARIO
990702	aggiunto COPIAARC, RINOMINAARC, MIDI
		SCORRIARITMETICO e SCORRILOGICO al posto di ASHIFT e LSHIFT
990629	rinominato ESEGUIRISULTATO -> RISULTATODIESEGUI
990621	PULSANTIERACREA,PULSANTIERACANCELLA <- STRUMENTICREA,STRUMENTICHIUDI
990615	FOGLIOESEGUI diventa sinonimo di FOGLIOESEGUITUTTO
990604	aggiunto TARTARECUPERA e TARTACONSERVA come sinonimi
		di BLOCCORECUPERA e BLOCCOCONSERVA, da differenziare
990526	aggiunto nuovo BLOCCOSTAMPA (BITPRINT ora = TARTASTAMPA)
990523	FOGLIOIMPOSTAFONT al posto di FOGLIOASSEGNAFONT
990521	aggiunto terminologia italiana (vedi rel.notes vers. 3.06m)
990518	aggiunto FOGLIOIMPOSTASTAMPANTE, FOGLIOSTAMPA,
		FOGLIOTAGLIA,FOGLIOCOPIA, FOGLIOINCOLLA,
		FOGLIOASSEGNAFONT, FOGLIOESEGUISELEZIONE, FOGLIOSELEZIONABLOCCO
		RIEMPISELEZIONE, SELEZIONAREATTIVA, ATTIVASELEZIONE,CENTRA
990430	aggiunto pc_END per lacuna in traduzione automatica
990416	italianizzato controlli di Windows e grafica raster (bit..)
		reso DIR e ASDIR sinonimi di DIREZIONE e ASDIREZIONE
990413	aggiunto CONTACARATTERI (_CONTAC) e _CONTALINEE (_CONTAL)
990408	recupero di molte primitive di Foglio
990401	aggiunto GLOSSARIO, GLOSSARIOMOSTRA
990318	aggiunto parola chiave AIUTO
		aggiunto MOSTRADOC / SHOWDOC (PR_HELP)
		aggiunto LISTADOC / DOCLIST (PR_DOCLIST)
990314	aggiunto DAOQUERY (PR_DAOQUERY)
990304	agg. pr. FINESTRAMOSTRA (WINDOWSHOW)
990225	agg. pr. STRUMENTICREA e STRUMENTICHIUDI
990222	agg. pr. ETICHETTADIM (LABELDIM)
990129	allocato messaggio E39 e p.c. STRUMENTI (kw[78])
990115	agg. pr. BROWSER, BROWSERCHIUDI
		ripristinato PULISCITESTO originario di MLOGO
990112	agg. pr. TARTA, FOGLIO,
		FOGLIORECUPERA, FOGLIOCONSERVA, FOGLIOESEGUI
981228	aggiunto p.c. IPERLOGO (kw[77])
981224	aggiunto p.c. VAI (kw[76]); allocato messaggio E30
981220	aggiunto p.c. SCHERMO (kw[75])
981218	aggiunto p.c. URL (kw[74])
981209	aggiunto CREA e CHIUDI di: TARTA, FOGLIO, BROWSER, PLAYER
		aggiunto p.c. BROWSER al posto di STILE (STYLE)
		aggiunto p.c. VISTA (VIEW) al posto di CENTRATO (CENTERED)
980911	aggiunto ASMODOPENNA (SETPENMODE)
		aggiunto alias a primitive GIU (GIULAPENNA) e SU (SULAPENNA)
980908	VAXY cambia specifiche (come SETXY di MswLogo) !!!!!!!!
		aggiunto VAPOS come precedente VAXY (SETPOS di MswLogo)
		aggiunto (dummy) LASTS, BUTLASTS, COMBINE, REMOVE, ..
		.. REMDUP, QUOTED, SIGN, GENSYM
		aggiunto alias a PRI, MENPRI, ULT, MENULT, ELE, ELE? ..
		.. DOVE, PUNTO, SCRIPUNTO, ..
980908	conversione a .CPP
980904	modificatto messaggi 3 e 4
980728	ritoccato errore 25,
		rinominato QUOZIENTE in QUOTO (chiesto da G. Lariccia, 980727)
		aggiunto QUOZIENTE come sinonimo di DIVISIONE (rivedere inglese)
		aggiunto PAROLAVUOTA e LISTAVUOTA (EMPTYWORD e EMPTYLIST)
970921	corretto KEYBOARDVAL in KEYBOARDVALUE
970913	aggiunto primitive ICON.., ..FOCUS, KEYBOARD.., SCROLL..
970910	aggiunto parole chiave da APRI a RIPRODUCI (PLAY)
		ASCOLSFONDO sinonimo di ASSFONDO e COLSFONDO di SFONDO (primitive)
		PLAYER parola chiave al posto di BOTTONE
		ritoccato errori 22 e 23
971020	modificato DIR(ECTORY) in DIRECTORY per evitare collisione con DIR(EZIONE) 
970908	modifiche per compatibilita' VC++5.0 
970808	pulizia parziale (EDITA, CDOS, ..) e allineamento bilingue
970804	sostituito pc GLOSSE con pc MENU
970402	aggiunto pc EDITA e primitiva EDIT/EDITA
961002	prima versione originale
*/

#include <windows.h>

#define ILWIN
// #define ENGLISH

#ifdef WIN32
#include "../inc/ilconfig.h"
#endif // WIN32

extern char * parole[];

#ifndef WIN32
int FAR PASCAL LibMain (HANDLE hInstance, WORD wDataSeg, WORD wHeapSize, LPSTR lpszCmdLine)
{
	if (wHeapSize > 0) UnlockData (0);
 	return 1;
}
#else // WIN32
//	Unlike Windows 3.x DLLs, Windows 95 and Windows NT call one
//	function, DllMain, for both initialization and termination.
//	It also makes calls on both a per-process and per-thread basis,
//	so several initialization calls can be made if a process is multithreaded.
//	The function is optional; if you don't provide it in source code,
//	the compiler links its own version, which does nothing but return TRUE.
#endif // WIN32

#ifndef WIN32
int FAR PASCAL _export WEP (int nParam)
{
	return 1;
}
#endif // WIN32

#ifndef WIN32
char FAR * FAR PASCAL _export ConfigGet (int i)
#else // WIN32
DllExport char * __stdcall ConfigGet (int i)
#endif // WIN32
{
	return parole[i]; 
}

char* parole [] = {

#ifdef ITALIANO
	
	"VUOTA",
	"VERO",
	"FALSO",
	"CONSOLE",
	"STAMPANTE",
	"PER",
	"FINE",
	"AS",
	"PAC",
	"COPERTO",
	"ERRORE",
	"GIU'",
	"SU",
	"CANCEPENNA",
	"INVERPENNA",
	"AT",
	"FOGLIO",
	"QUI",
	"STOP",
	"CHIUSO",
	"APERTO",
	"CIRCOLARE",
	"COMANDI",
	"TARTA",
	"MISTO",
	"RECUPERA",
	"COM1",
	"COM2",
	"GIU",
	"AP",
	"VAR",
	"PROC",
	"PULISCI",
	"XOFF",				// v. 1
	"FULL",				// v. 1
	"TXLF",				// v. 1
	"RXLF",				// v. 1
	"BINARIO",
	"APPENDI",			//	gia'	"FINESTRA",
	"MAIUSCOLE",		//	gia'	"TESTO",
	"POS",
	"DIM",
	"SFONDO",
	"STATO",
	"DIMBUF",
	"COLTESTO",
	"CURSORE",				// v. 1
	"ALP",
	"AND",
	"OR",
	"XOR",
	"PLAYER", 			// gia' BOTTONE 980710
	"BROWSER",			// gia' STILE 981209
	"VISTA",			// gia' CENTRATO 981209
	"ACCAPO",			// v. 1
	"ARCHIVIO",
	"MENU",				// gia' GLOSSE
	"FONT",
	"TIPO",
	"SCALA",
	"DIR",
	"PIXEL",
	"TITOLO",			// gia' EGA
	"DIPINGI",			// gia' CGA
	"PAUSA",			// gia' VGA
	"CONTINUA",			// gia' HERC
	"MODO",				// ripristinato	000102
	"EDITA",
//>980710
	"APRI",
	"CHIUDI",
	"ALIAS",
	"INFO",
	"CAPACITA",
	"RIPRODUCI",
//<980710
	"URL",
	"SCHERMO",
	"VAI",
	"IPERLOGO",
	"STRUMENTI",		// 78
	"AIUTO",			// 79
	"END",				// 80
	"RADICE",			// 81
	"ATTR",				// 82
	"SELEZIONE",		// 83	991116
	"IGNORACASE?",		// 84	000102
	"CONSERVA",			// 85	000705
	"CONFERMA",			// 86	020208
	"#",

#endif // ITALIANO

#ifdef ENGLISH

	"EMPTY",
	"TRUE", //
	"FALSE", //
	"CONSOLE",
	"PRINTER",
	"TO", //
	"END", //
	"MAKE", //
	"PAC",
	"BURIED",
	"ERROR", //
	"PENDOWN",
	"PENUP",
	"ERASE",		// già PENERASE
	"REVERSE",		// già PENREVERSE
	"AT",
	"EDITOR",		// FOGLIO
	"HERE",
	"STOP",
	"FENCE",
	"WINDOW",
	"WRAP",
	"COMMANDER",
	"TURTLE",
	"MIXED",
	"LOAD",
	"TRACE",				//	gia'	"COM1",
	"UNTRACE",				//	gia'	"COM2",
	"DOWN",
	"AP",
	"NAME",
	"PROCEDURE",
	"CLEAR",
	"XOFF",
	"FULL",
	"TXLF",
	"RXLF",
	"BINARY",
	"APPEND",			//	gia' FINESTRA
	"UPPERCASE",		//	gia' TESTO
	"POS",
	"DIM",
	"SCREENCOLOR",
	"STATUS",
	"DIMBUF",
	"TEXTCOLOR",
	"CURSOR",
	"ALP",
	"AND",
	"OR",
	"XOR",
	"PLAYER", 			// gia' BUTTON 980710
	"BROWSER",			// gia' STYLE 981209
	"VIEW",				// gia' CENTERED 981209
	"ACCAPO",
	"FILE",
	"MENU",				// gia' GLOSSE
	"FONT",
	"TYPE",
	"SCALE",
	"HEADING",
	"PIXEL",
	"TITLE",			// gia' EGA
	"PAINT",			// gia' CGA
	"PAUSE",			// gia' VGA
	"CONTINUE",			// gia' HERC
	"MODE",				// ripristinato	000102
	"EDIT",
//>980710
	"OPEN",				// APRI
	"CLOSE",			// CHIUDI
	"ALIAS",			// ALIAS
	"INFO",				// INFO
	"CAPABILITY",		// CAPACITA
	"PLAY",				// RIPRODUCI
//<980710
	"URL",
	"DISPLAY",
	"GO",
	"IPERLOGO",
	"TOOLBAR",			// 78
	"HELP",				// 79
	"END",				// 80 !!!
	"ROOT",				// 81
	"SELECTION",		// 83		991116
	"CASEIGNOREDP",		// 84		000102
	"CONFIRMATION",		// 86	020208
	"#",

#endif // ENGLISH

/* primitive di sistema */

#ifdef ITALIANO

//	"_NUMSM",
//	"_SM",
//	"_ISM",
//	"_ASM",
//	"_GLOSSA",
	"ZITTO",
	"YCOR",
	"XCOR",
	"VUOTO?",
	"VERSO",
	"VERIFICA",
	"VAY",
	"VAXY",
	"VAX",
//	"_UR",
//	"_UC",
	"VATANA",
	",TANA",
//	"_PR",
//	"_PC",
	"_VAL",
	"_IZP",
	"_IZL",
	"_IZF",
	"_IZB",
	"VAI",
	"_FIP",
	"_FIL",
	"_FIF",
	"_FIB",
	"_VAC",
	"UNIONE",
	"ULTLINEA",
	"ULT",
	",ULTIMO",	// 980908
	"UGUALE?",
	"_TRONCA",
	"TRACCIA",
	"TOUCH?",
	"TOUCH",
	"TESTO",
	"TEMPO",
	"TAVOLOZZA",
	"TASTO?",
	"TASTO",
	"TASTIERA",
	"TARTA?",
	"TAN",
	"SVAR_IN",
	"SUONA",
//	"_GIUF",
	"SUCHESCRIVI",
	"SUCC",
	"SU",
	",SULAPENNA",	// 980911
	"STUTTO_IN",
	"STRINGA",
	"STOP",
	"STITOLI",
	"STATO",
	",PULSANTE?",
	"STAMPA",
	",ST",
	"SPROC_IN",
	"_SPOSTA",
	"SPROC",
	"SOMMA",
	"SLISTEPROP",
	"SINISTRA",
	",S",
	"SFONDO",
//>980710
	",COLSFONDO",
//<980710
	"SE_VERO",
	",SEV",
	"SEN",
	"SELEZIONA",
	",SEL",
	"SE_FALSO",
	",SEF",
	",ALTRIMENTI",
	"SE",
	"SCRIVI_SU",
	"SCRIVI_A",
	"SCRIVI",
	",SC",
	"SCRILETT",		// 991012: PRINTQUOTED
	"SCRICAR",
	"SCOPRI",
	"SCI",
	"SCHIA",
	"SCHERMO",                     
	"SCALAFIN",
	"RIPORTA",
	"RIPETI",
	"RIGHE",
	"RIEMPI",
	"RIACASO",
	"RESTO",
	"REGISTRA",
	"RECUPERA_TESTO",			// v. 1
	",RTES",					// v. 1
	"RECUPERA_QUADRO",			// v. 1
	"RECUPERA_DIS",				// v. 1
	"RECUPERA",
	"RADQ",
	"RADIANTI",
	"QUOTO",		// gia' QUOZIENTE !!! (richiesto da G. Lariccia 980727)
	"_QIB",
	"_QFB",
	"QUI",
	"SCRIPUNTO",
	",SCRIPIXEL",	// 980908
	"PULISCITESTO",
	",PT",
	"PULISCISCHERMO",
	",PS",
	"PULISCI",
	"PROP",
	"PRODOTTO",
	"PRIMITIVA?",
	"PRI",
	",PRIMO",	// 980908
	"PREC",
	"POTENZA",
	"PI",
	"PARTE",
	"PER",
	"PENNA",
	"PAUSA",
	"PAROLA?",
	"PAROLA",
	"PACTUTTO",
	"PAC",
	"ORDINA",
	"ORDINA2",	// 010213
	"OPPURE",
	",O",
	"NUMERO?",
	"NONTRACCIARE",
	",NTRA",
	"NONREGISTRARE",
	",NREG",
	"NON",
	"NODI",
	"NASTARTA",
	",NT",
	"NASFOGLIO",
	"NASCONDI",
	"INULTIMO",
	",INULT",
	"INPRIMO",
	",INPRI",
	"_MS",
	"MOSTRA",
	"MOSTARTA",
	",MT",
	"MOSFOGLIO",
	"MODO",
	"_MODC",
	"_ML",
	"MISURA",
	"MENULT",
	",MENULTIMO",	// 980908
	"MENPRI",
	",MENPRIMO",	// 980908
	"MENLISTA",
	"_MC",
//	"_MARCAL",
//	"_MARCAC",
//	"_MARCAB",
	"MAIUSCOLE?",
	"LUNGARC",
	"LOG10",
	"LOCALE",
	"LN",
	"LISTAPROP",
	"LISTARIF",
	"LISTA?",
	"LISTA",
	"LIBERA",
	"LIB",				// v. 1
	"LEGGISTRINGA",
	",LS",
	"PUNTO",
	",PIXEL",	// 980908
	"LEGGIPAROLA",
	",LP",
	"LEGGILISTA",
	",LL",
	"LEGGI_DA",
	"LEGGICAR",
	",LC",
	"LEGGI_A",
	"LANCIA",
//	"_IR",
	"_IP",
	"INVERPENNA",
	"INTERSEZIONE",
	",IS",
	"INT",
	"INDIETRO",
	",I",
	"INDICE",
	"_IL",
	"_IC",
	"GRADI",
//	"_SUF",
	"GIU'",
	",GIU",
	",GIULAPENNA",	// 980911
	"FRASE",
	"FOGLIO?",
	"FINESTRE",
	"FINESCRIVI?",
	"FINELEGGI?",
	"ESP",
	"ESEGUI",
	"ERRORE",
	"ELE?",
	",ELEMENTO?",	// 980908
	"ELE",
	",ELEMENTO",	// 980908
	"ASECO",
	"_P",
	"DOVESCRIVI",
	"DOVELEGGI",
	"_NUML",
	"POSELE",
	"_CURSORE",		// 990526
	",_NUMC",
	"DOVE",
	",POS",			// 980908
	"DOS",			// v. 1
//	"_M",
	"_L",
	"DIVISIONE",
	",QUOZIENTE",		// 980728: aggiunto sinonimo !!!!
	"DITO",				// v. 1
	"DISCO",
	"DIREZIONE",
	",DIR",
	"DIFFERENZA",
	"DESTRA",
	",D",
	"DEFINISCI",
	",DF",
	"DEF?",
	"DEC",
	"_C",
	"_B",
	"DACHELEGGI",
	"CURSORE",					// v. 1
	"_CP",
	"CREALIB",					// v. 1
	"COSA?",
	"COSA",
	"COS",
	"COPRI",
	"COPIADEF",
	"_COPIA",
	"CONTINUA",
	"CONTA",
	"CONSERVA_TESTO",			// v. 1
	",CTES",					// v. 1
	"CONSERVA_QUADRO",			// v. 1
	"CONSERVA_DIS",				// v. 1
	"CONSERVA",
	"COMPRESS",					// v. 1
	"COMPILA",					// v. 1
	"COLTESTO",					// v. 1
	"COLORE",
	",COLPENNA",
	"_CL",
	"CIFRE",
	"CIAO",
	"CHIUDITUTTO",
	"CHIUDI",
	"CHIAMA",
	"_CERCA",
	"_CC",
	"_CB",
	"CATALOGO",
	",CAT",
	"CATALOGO2",				// 991217
	"CARATTERE",
	",CAR",
	"CANCEVAR_IN",
	"CANCETUTTO_IN",
	"CANCEPROP_IN",
	"CANCEPROP",
	"CANCEPROC_IN",
	"CANCEPRIM",
	"CANCEPENNA",
	"CANCEVAR",
	"CANCEFOGLIO",			// v. 1
	"CANCEARC",
	"CANCEPROC",
	"CAMPO",
	"_CAMBIA",
	"BREAK",
	"AVANTI",
	",A",
	"ATTR",					// v. 1
	"ASTOUCH",				// v. 1
	"ASTEMPO",
	"ASTAVOLOZZA",			// v. 1
	"ASTASTO",				// v. 1
	",AT",
	"ASSOC",
	"ASSOCELE",				// 990927
	"ASSOCIA",				// 990924
	",.ASSOCIA",			// 991228
	"ASSO",
	"ASSFONCAR",			// v. 1
	"ASSFONDO",
//>980710
	",ASCOLSFONDO",
//>980710
	"ASSCHIA",
	"ASSCHERMO",
	"ASSCALAFIN",			// v. 1
	"ASRIGHE",				// v. 1
	"ASPROP",
	",AP",
	"ASPETTA",
	"ASPENNA",				// v. 1
	"ASMODO",				// v. 1
	"ASMAIUSCOLE",
	"ASLISTAPROP",
	",ALP",
	"_ASINS",
	"ASDISCO",
	"ASDIREZIONE",
	",ASDIR",
	"ASCURSORE",			// v. 1
	"ASCOM",				// v. 1
	"ASCOLTESTO",			// v. 1
	"ASCOL",
	",ASCOLPENNA",
	"ASCII",
	"ASCIFRE",
	"ASCAMPO",
	"ASATTR",				// v. 1
	"AS",
	"ARRO",
	"ARCTAN",
	"ARCHIVIO?",
	"ARCAPERTI",
//	"_AR",
	"APRI",
	"_AP",
	"AMB",
	"_AL",
	"_AGGIORNA",
	"ACCHIAPPA",
	"ACASO",
	"_AC",
//	".SEGDOS",
//	".SEGPROG",
//	".SEGDATI",
	"RINOMINA",			// v. 1
//	".PORTA",
//	".MEM",
//	".ESEGUI",
//	".ASSEG",
//	".ASPORTA",
//	".ASMEM",

#endif // ITALIANO

#ifdef ENGLISH

//	"_NUMSM",
//	"_SM",
//	"_ISM",
//	"_ASM",
//	"_GLOSSA",
	"ZITTO",
	"YCOR", //
	"XCOR", //
	"EMPTYP", //
	"TOWARDS", //
	"TEST", //
	"SETY", //
	"SETXY", // VAXY
	"SETX", //
//	"_UR",
//	"_UC",
	"HOME", //
//	"_PR",
//	"_PC",
	"_VAL",
	"_IZP",
	"_IZL",
	"_IZF",
	"_IZB",
	"GO",	// VAI
	"_FIP",
	"_FIL",
	"_FIF",
	"_FIB",
	"_VAC",
	"UNIONE",
	"ULTLINEA",
	"LAST", //
	"EQUALP",
	"_TRONCA",
	"TRACE", // TRACCIA
	"TOUCH?",
	"TOUCH",
	"TEXT", // 
	"TIME", //
	"TAVOLOZZA",
	"KEYP", //
	"TASTO",
	"TASTIERA",
	"TARTA?",
	"TAN",
	"SVAR_IN",
	"SOUND", // SUONA
//	"_GIUF",
	"WRITER", //
	"SUCC",
	"PENUP", //
	",PU",
	"STUTTO_IN",
	"STRING", // STRINGA
	"STOP", //
	"STITOLI",
	"STATO",
	",PULSANTE?",
	"SPROC_IN",
	"_SPOSTA",
	"SPROC",
	"SUM", //
	"SLISTEPROP",
	"LEFT", //
	",LT",
	"SCREENCOLOR", //
	"IFTRUE", //
	",IFT",
	"SIN", //
	"SELECT", // SELEZIONA
	"IFFALSE", //
	",IFF",
	"IF", //
	"SETWRITE", //
	"SETWRITEPOS", //
	"TYPE", //
	"PRINT", //
	",PR",
	"PRINTQUOTED",		// 991012: SCRILETT
	"SCRICAR",
	"SCOPRI", // UNBURY e' procedura
	"SCI",
	"SCHIA",
	"SCREEN", // SCHERMO                     
	"SCALAFIN",
	"OUTPUT", // RIPORTA
	",OP",
	"REPEAT", //
	"RIGHE",
	"FILL", //
	"RERANDOM", //
	"REMAINDER", //
	"DRIBBLE", //
	"RECUPERA_TESTO",			// v. 1
	"RECUPERA_QUADRO",			// v. 1
	"RECUPERA_DIS",				// v. 1
	"LOAD", //
	"SQRT", //
	"RADIANTI",
	"QUOTIENT", //
	"_QIB",
	"_QFB",
	"QUI",
	"SCRIPUNTO",
//>
//	"PULISCITESTO",
	"CLEARTEXT",
	",CT",
//<
	"CLEARSCREEN", //
	",CS",
	"CLEAR", //
	"GPROP", //
	"PRODUCT", //
	"PRIMITIVEP", //
	"FIRST", //
	"PREC",
	"POWER", //
	"PI",
	"PARTE",
	"TO", //
	"_", //	già PENNA
	"PAUSE", //
	"WORDP", //
	"WORD",  //
	"PACTUTTO",
	"PAC",
	"ORDINA",
	"ORDINA2",	// 010213
	"OR", //
	"NUMBERP", //
	"UNTRACE", // NONTRACCIARE
	"NODRIBBLE", //
	"NOT", // NON
	"NODES", // NODI
	"HIDETURTLE", //
	",HT",
	"NASFOGLIO",
	"NASCONDI",
	"LPUT", //
	"FPUT", //
	"_MS",
	"SHOW", //
	"SHOWTURTLE", //
	",ST",
	"MOSFOGLIO",
	"MODO",
	"_MODC",
	"_ML",
	"MISURA",
	"BUTLAST", //
	",BL",
	"BUTFIRST", //
	",BF",
	"MENLISTA",
	"_MC",
//	"_MARCAL",
//	"_MARCAC",
//	"_MARCAB",
	"MAIUSCOLE?",
	"LUNGARC",
	"LOG10", //
	"LOCAL", //
	"LN", //
	"PLIST", //
	"LISTARIF",
	"LISTP", //
	"LIST", //
	"FREE", // LIBERA
	"LIB",					// v. 1
	"LEGGISTRINGA",
	"PIXEL", //
	"READWORD", //
	",RW", //
	"READLIST", //
	",RL", //
	"SETREADER", // LEGGI_DA (gia' READER, per errore)
	"READCHAR", //
	",RC", //
	"SETREADPOS", //
	"THROW", //
//	"_IR",
	"_IP",
	"PENREVERSE", //
	",PX",
	"INTERSEZIONE",
	"INT",
	"BACK", //
	",BK", //
	"INDEX",
	"_IL",
	"_IC",
	"GRADI",
//	"_SUF",
	"PENDOWN", //
	",PD", //
	"SENTENCE", //
	",SE",
	"FOGLIO?",
	"WINDOWS", // FINESTRE
	"FINESCRIVI?",
	"EOFP", //
	"EXP", //
	"RUN", //
	"ERROR", //
	"MEMBERP", //
	"ITEM", //
	"SETECHO", // ASECO
	"_P",
	"WRITEPOS", //
	"READPOS", //
	"_NUML",
	"POSELE",
	"_NUMC",
	"POS", //
	"DOS",					// v. 1
//	"_M",
	"_L",
	"DIVISIONE",
	"DITO",					// v. 1
	"DISK", // DISK
	"HEADING", //
	"DIFFERENCE", //
	"RIGHT", //
	",RT", //
	"DEFINE", //
	"DEFINEDP", //
	"DEC",
	"_C",
	"_B",
	"READER", // DACHELEGGI
	"CURSORE",				// v. 1
	"_CP",
	"CREALIB",				// v. 1
	"NAMEP", //
	"THING", //
	"COS", //
	"COPRI", // BURY e' procedura
	"COPYDEF", //
	"_COPIA",
	"CONTINUE", //
	",CO",
	"COUNT", //
	"CONSERVA_TESTO",
	"CONSERVA_QUADRO",
	"CONSERVA_DIS",
	"SAVE", //
	"COMPRESS",				// v. 1
	"COMPILA",				// v. 1
	"COLTESTO",				// v. 1
	"PENCOLOR", //
	"_CL",
	"CIFRE",
	"BYE", //
	"CLOSEALL", //
	"CLOSE", //
	"NAME", //
	"_CERCA",
	"_CC",
	"_CB",
	"DIR", //
	"DIR2",					// 991217
	"CHAR", //
	"ERNS_IN", // CANCEVAR_IN
	"ERALL_IN", // CANCETUTTO_IN
	"ERPLS_IN", // CANCEPROP_IN
	"REMPROP", //
	"ERPS_IN", // CANCEPROC_IN
	"CANCEPRIM",
	"PENERASE", //
	",PE",
	"CANCEVAR",
	"CANCEFOGLIO",			// v. 1
	"ERASEFILE", //
	",ERF",
	"CANCEPROC",
	"CAMPO",
	"_CAMBIA",
	"BREAK", //
	"FORWARD", //
	",FD", //
	"ATTR",					// v. 1
	"ASTOUCH",				// v. 1
	"SETTIME", //
	"ASTAVOLOZZA",			// v. 1
	"ASTASTO",				// v. 1
	"ASSOC",
	"ASSOCELE",				// 990927
	"SETASSOC",				// 990924
	"ASSO",
	"ASSFONCAR",			// v. 1
	"SETSCREENCOLOR", //
	",SETSC",
	"ASSCHIA",
	"SETSCREEN", 			// ASSCHERMO
	"SETSCREENSCALE",		// v. 1
	"ASRIGHE",				// v. 1
    "PPROP", //
    "WAIT", //
	"_", // già ASPENNA
	"ASMODO",				// v. 1
	"ASMAIUSCOLE",
	"ASLISTAPROP",
	"_ASINS",
	"SETDISK", // ASDISCO
	"SETHEADING", // ASDIR
	",SETH",
	"ASCURSORE",			// v. 1
	"ASCOM",				// v. 1
	"ASCOLTESTO",			// v. 1
	"SETPENCOLOR", //
	",SETPC",
	"ASCII", //
	"SETDIGITS",
	"SETFIELD",
	"ASATTR",				// v. 1
	"MAKE", //
	"ROUND", //
	"ARCTAN", //
	"ARCHIVIO?",
	"ALLOPEN", //
//	"_AR",
	"OPEN", //
	"_AP",
	"AND", //
	"_AL",
	"_AGGIORNA",
	"CATCH", //
	"RANDOM", //
	"_AC",
//	".SEGDOS",
//	".SEGPROG",
//	".SEGDATI",
	"RINOMINA",			// v. 1
//	".PORTA",
//	".MEM",
//	".ESEGUI",
//	".ASSEG",
//	".ASPORTA",
//	".ASMEM",

#endif // ENGLISH

	"+",
	"-",
	"*",
	"/",
	"^",
	"=",
	">",
	"<",

#ifdef ILWIN

#ifdef ITALIANO

	"MAGGIORE?",
	"MINORE?",

	"ARRAY",
	"ARRAY?",
	"ASELEMENTO",
	",ASELE",
	",.ASELEMENTO",		// 991228
	",.ASELE",			// 991228

	"PRIMI",
	"MENPRIMI",

	"ROVESCIA",

	"MINUSCOLO",
	"MAIUSCOLO",

	"MOUSEON",
	",MOUSEACCENDI",	// 990521
	"MOUSEOFF",
	",MOUSESPEGNI",		// 990521
	"MOUSEPOS",
//	"PULISCITESTO",

	"ANALIZZA",
	"ANALIZZAESEGUI",

	"LEGGICARATTERI",

	"MENO",

	"RADSEN",
	"RADCOS",
	"RADTAN",
	"RADARCTAN",

	"XOR",
	"BITAND",
	"BITOR",
	"BITXOR",
	"BITNOT",
	"SCORRIARITMETICO",	// 990702
	",ASHIFT",
	"SCORRILOGICO",		// 990702
	",LSHIFT",

	"ARCO",

	"CIRCOLARE",
	"FINESTRA",
	"MURO",

	"ETICHETTA",
	"ASFONTESTO",
	"ASFONTESTOLOG",	// 991223
	"FONTESTO",

	"SCHERMOTESTO",		// v.3
	"SCHERMOPIENO",		// v.3
	"SCHERMOMISTO",		// v.3
	"RINFRESCA",		// v.3
	"NONRINFRESCARE",	// v.3

	"FINESTRATARTA",	// fattorizzare con foglio 
	"ZOOM",				//
	"RIDIMENSIONA",

	"DIPINGIPENNA",		// o PENNADIPINGI ?
	"ASCOLRIEMPI",		// anziche' ASCOLORERIEMPI
	"ASTRATTOPENNA",
	"ASMOTIVOPENNA",
	"CANCETAVOLOZZA",
	"PENNAGIU?",
	"MODOPENNA",
	"COLRIEMPI",		// anziche' COLORERIEMPI
	"TRATTOPENNA",
	"MOTIVOPENNA",

	"RISULTATODIESEGUI",
	",ESEGUIRISULTATO",
	"CONTARIPETIZIONI",	//
	",RIP",				// 991005
	"SE_ALTRIMENTI",
	"PASSA",			// o CEDI ?
	"NONPASSARE",		// o NONCEDERE ?
	"CONTROLLAEVENTO",
	"APPLICA",
	".MACRO",
	".DEFMACRO",
	"MACROP",
    
	"CAMBIADIR",
	",CAMBIACARTELLA",
	"DIRECTORY",		// gia' DIR, che collide con DIR(EZIONE)
	",CARTELLA",

#endif // ITALIANO

#ifdef ENGLISH

	"GREATERP",
	"LESSP",

	"ARRAY",
	"ARRAYP",
	"SETITEM",

	"FIRSTS",
	"BUTFIRSTS",
	",BFS",

	"REVERSE",

	"LOWERCASE",
	"UPPERCASE",

	"MOUSEON",
	"MOUSEOFF",
	"MOUSEPOS",
//	"CLEARTEXT",	gia' esisteva PULISCITESTO in MLOGO
//	",CT",

	"PARSE",
	"RUNPARSE",

	"READCHARS",
	",RCS",

	"MINUS",

	"RADSIN",
	"RADCOS",
	"RADTAN",
	"RADARCTAN",

	"XOR",
	"BITAND",
	"BITOR",
	"BITXOR",
	"BITNOT",
	"ASHIFT",
	"LSHIFT",

	"ARC",

	"WRAP",
	"WINDOW",
	"FENCE",

	"LABEL",
	"SETTEXTFONT",
	"SETTEXTFONTLOG",	// 991223
	"TEXTFONT",

	"TEXTSCREEN",
	"FULLSCREEN",
	"SPLITSCREEN",
	"REFRESH",
	"NOREFRESH",

	"TURTLEWINDOW",
	",TW",
	"ZOOM",
	"RESIZE",

	"PENPAINT",
	"SETFLOODCOLOR",
	"SETPENSIZE",
	"SETPENPATTERN",
	"CLEARPALETTE",
	"PENDOWNP",
	"PENMODE",
	"FLOODCOLOR",
	"PENSIZE",
	"PENPATTERN",

	"RUNRESULT",
	"REPCOUNT",
	"IFELSE",
	"YIELD",
	"NOYIELD",
	"EVENTCHECK",
	"APPLY",
	".MACRO",
	".DEFMACRO",
	"MACROP",
    
	"CHDIR",
	"DIR",

#endif // ENGLISH

#ifdef ITALIANO

	"DIALOGHI",					// 991105

	"FINESTRACREA",		//
	",WINDOWCREATE",			// 990521
	"FINESTRACANCELLA",	//
	",FINESTRACANCE",	//
	",WINDOWDELETE",			// 990521

	"DIALOGOCREA",		//
	",DIALOGCREATE",			// 990521
	"DIALOGOCANCELLA",	//
	",DIALOGOCANCE",	//
	",DIALOGDELETE",			// 990521
	"DIALOGOASFUOCO",			// 991213

	"LISTASCELTACREA",			// 990521
	",LISTBOXCREATE",
	"LISTASCELTAGREZZACREA",	// 990521
	",LISTBOXCREATERAW",
	"LISTASCELTACANCELLA",		// 990521
	",LISTBOXDELETE",
	"LISTASCELTASELEZIONE",		// 000202
	",LISTASCELTASCEGLI",		// 990521	da togliere !!
	",LISTBOXGETSELECT",
	"LISTASCELTAAGGIUNGI",		// 990521
	",LISTBOXADDSTRING",
	"LISTASCELTATOGLI",			// 990521
	",LISTBOXDELETESTRING",
	"LISTASCELTAINDICE",		// 990521
	",LISTBOXGETINDEX",
	"LISTASCELTAAZZERA",		// 000202

	"CASELLATESTOCREA",			// 991012
	"CASELLATESTOCANCELLA",		// 991012
	"CASELLATESTOVALORE",		// 991012
	"CASELLATESTOASSEGNA",		// 991012

	"COMBOCREA",				// 990521
	",COMBOBOXCREATE",
	"COMBOCANCELLA",			// 990521
	",COMBOBOXDELETE",
	"COMBOTESTO",				// 990521
	",COMBOBOXGETTEXT",
	"COMBOASSEGNATESTO",		// 990521
	",COMBOBOXSETTEXT",
	"COMBOAGGIUNGI",		// 990521
	",COMBOBOXADDSTRING",
	"COMBOTOGLI",			// 990521
	",COMBOBOXDELETESTRING",

	"BARRASCORRIMENTOCREA",		// 990521
	",SCROLLBARCREATE",
	"BARRASCORRIMENTOCANCELLA",	// 990521
	",SCROLLBARDELETE",
	"BARRASCORRIMENTOASSEGNA",	// 990521
	",SCROLLBARGET",
	"BARRASCORRIMENTOVALORE",	// 990521
	",SCROLLBARSET",

	"PULSANTECREA",				// 990522
	",BOTTONECREA",		//
	",BUTTONCREATE",			// 990521
	"PULSANTECANCELLA",			// 990522
	",BOTTONECANCELLA",	//
	",BOTTONECANCE",	//
	",BUTTONDELETE",			// 990521

	"ETICHETTACREA",			// 990521
	",STATICCREATE",
	"ETICHETTACANCELLA",		// 990521
	",STATICDELETE",
	"ETICHETTAAGGIORNA",		// 990521
	",STATICUPDATE",

	"GRUPPOCREA",				// 990521
	",GROUPBOXCREATE",
	"GRUPPOCANCELLA",			// 990521
	",GROUPBOXDELETE",

	"CASELLACREA",				// 990521
	",CHECKBOXCREATE",
	"CASELLACANCELLA",			// 990521
	",CHECKBOXDELETE",
	"CASELLAVALORE",			// 990521
	",CHECKBOXGET",
	"CASELLAASSEGNA",			// 990521
	",CHECKBOXSET",

	"PULSANTERADIOCREA",		// 990522
	",RADIOBUTTONCREATE",
	"PULSANTERADIOCANCELLA",	// 990522
	",RADIOBUTTONDELETE",
	"PULSANTERADIOVALORE",		// 990522
	",RADIOBUTTONGET",
	"PULSANTERADIOASSEGNA",		// 990522
	",RADIOBUTTONSET",

	"FINESTRADEBUG",			// 990521
	",DEBUGWINDOWS",
	"MESSAGGIO",				// 990521
	",MESSAGEBOX",				//
	"DIALOGOAPRIFILE",			// 990521
	",DIALOGFILEOPEN",
	"DIALOGOSALVAFILE",			// 990521
	",DIALOGFILESAVE",
	"FINESTRAMODIFICAFILE",		// 990522
	",WINDOWFILEEDIT",

	"BLOCCOTAGLIA",			//
	",BITCUT",				// 990521
	"BLOCCOCOPIA",			//
	",BITCOPY",				// 990521
	"BLOCCOINCOLLA",		//
	",BITPASTE",			// 990521
	"BLOCCOADATTA",			//
	",BITFIT",				// 990521
	"BLOCCOINDICEASSEGNA",	//
	",ASBLOCCOINDICE",		//
	",SETBITINDEX",			// 990521
	"BLOCCOINDICE",			//
	",BITINDEX",			// 990521
	"BLOCCOMODOASSEGNA",	//
	",ASBLOCCOMODO",		//
	",SETBITMODE",			// 990521
	"BLOCCOMODO",			//
	",BITMODE",				// 990521
	"BLOCCODISEGNA",		//
	",BLOCCO",				// 990521
	",BITBLOCK",			// 990521
//	"BLOCCOSTAMPA",			//
	"TARTASTAMPA",			// 990526
	",BITPRINT",			// 990521
	"BLOCCORECUPERA",		// 040123
	",BITLOAD",				// 990521
//>040123
//	",TARTARECUPERA",		// 990604
	"TARTARECUPERA",
//<040123
	"BLOCCOCONSERVA",		// 040123
	",BITSAVE",				// 990521
//>040123
//	",TARTACONSERVA",		// 990604
	"TARTACONSERVA",		// 990604
//<040123
	"ASSEGNAAREAATTIVA",	// 990522
	",AREATTIVASSEGNA",		//
	",ASAREATTIVA",			//
	",SETACTIVAREA",		// 990521
	"AREAATTIVA",			// 990521
	",AREATTIVA",			//
	",ACTIVAREA",			// 990521

#endif // ITALIANO

#ifdef ENGLISH

	"DIALOGS",					// 991105

	"WINDOWCREATE",
	"WINDOWDELETE",

	"DIALOGCREATE",
	"DIALOGDELETE",
	"DIALOGSETFOCUS",			// 991213

	"LISTBOXCREATE",
	"LISTBOXCREATERAW",
	"LISTBOXDELETE",
	"LISTBOXGETSELECT",
	"LISTBOXADDSTRING",
	"LISTBOXDELETESTRING",
	"LISTBOXGETINDEX",
	"LISTBOXRESET",				// 000202

	"EDITBOXCREATE",			// 991012
	"EDITBOXDELETE",			// 991012
	"EDITBOXGETTEXT",			// 991012
	"EDITBOXSETTEXT",			// 991012

	"COMBOBOXCREATE",
	"COMBOBOXDELETE",
	"COMBOBOXGETTEXT",
	"COMBOBOXSETTEXT",
	"COMBOBOXADDSTRING",
	"COMBOBOXDELETESTRING",

	"SCROLLBARCREATE",
	"SCROLLBARDELETE",
	"SCROLLBARGET",
	"SCROLLBARSET",

	"BUTTONCREATE",
	"BUTTONDELETE",

	"STATICCREATE",
	"STATICDELETE",
	"STATICUPDATE",

	"GROUPBOXCREATE",
	"GROUPBOXDELETE",

	"CHECKBOXCREATE",
	"CHECKBOXDELETE",
	"CHECKBOXGET",
	"CHECKBOXSET",

	"RADIOBUTTONCREATE",
	"RADIOBUTTONDELETE",
	"RADIOBUTTONGET",
	"RADIOBUTTONSET",

	"DEBUGWINDOWS",
	"MESSAGEBOX",
	"DIALOGFILEOPEN",
	"DIALOGFILESAVE",
	"WINDOWFILEEDIT",

	"BITCUT",
	"BITCOPY",
	"BITPASTE",
	"BITFIT",
	"SETBITINDEX",
	"BITINDEX",
	"SETBITMODE",
	"BITMODE",
	"BITBLOCK",
	"BITPRINT",
	"BITLOAD",
	"BITSAVE",
	"SETACTIVEAREA",
	"ACTIVEAREA",

#endif // ENGLISH

#ifdef ITALIANO

	"TIMERASSEGNA",			// 990521
	",ASTIMER",
	"TIMERCANCELLA",		// 990521
	",CANCETIMER",

	"MIDICONTA",			// 991116
	"MIDIAPRI",				// v.3
	"MIDICHIUDI",			// v.3
	"MIDI",					// 990701 991116
	"MIDIMESSAGGIO",		// v.3
	"SUONAWAVE",			// v.3
	"MCI",					// v.3

	"EDITA",
	",ED",

	"MINIMIZZA",
	"MASSIMIZZA",
	"NORMALIZZA",
	"RIPRISTINA",
	"ASFUOCO",
	"FUOCO",

	"TASTIERAACCENDI",		// 990521
	",KEYBOARDON",
	"TASTIERASPEGNI",		// 990521
	",KEYBOARDOFF",
	"TASTIERAVALORE",		// 990521
	",KEYBOARDVALUE",

	"SCROLLAX",
	"SCROLLAY",

	"PAROLAVUOTA",
	"LISTAVUOTA",

	"VAPOS",

	"ULTIMI",
	"MENULTIMI",

	"DUPLICA",				// 991102
	"COMBINA",
	"RIMUOVI",
	"RIMUOVIDOPPI",
	"LETTERALE",
	",LETT",
	",QUOTED",

	"SEGNO",
	"GENSIM",

	"ASMODOPENNA",
//>981209
	"TARTACREA",
	"FOGLIOCREA",
	"BROWSERCREA",
	"PLAYERCREA",

	"TARTACHIUDI",
	"FOGLIOCHIUDI",
	"BROWSERCHIUDI",
	"PLAYERCHIUDI",
//<
//>990115
	"TARTA",
	"FOGLIO",
	"BROWSER",
	"PLAYER",
//<
//>990112
//	"FOGLIO",
	"FOGLIORECUPERA",
	"FOGLIOCONSERVA",
	"FOGLIOESEGUITUTTO",
	",FOGLIOESEGUI",
//	"TARTA",
//<
	"ETICHETTADIM",
	"ETICHETTADIMLOG",		// 991223

	"PULSANTIERACREA",
	"PULSANTIERACANCELLA",
	"PULSANTIERAASPOS",		// 990917

	"FINESTRAMOSTRA",

	"DATABASEINTERROGA",
	",DBINT",
	"DATABASEESEGUI",		// 991214
	",DBESEGUI",			// 991214

	"GLOSSARIOVOCI",		// gia' INFORMAZIONE
	",VOCI",				// gia' INFO
	"GLOSSARIOVOCIMOSTRA",	// gia' INFORMAZIONEMOSTRA
	",VOCIMOSTRA",			// gia' INFOMOSTRA
	"GLOSSARIOINDICE",
	",GLOSSARIO",
	"GLOSSARIOINDICEMOSTRA",
	",GLOSSARIOMOSTRA",

	"_CONTAC",
	"_CONTAL",

	"FOGLIOIMPOSTASTAMPANTE",
	"FOGLIOSTAMPA",
	"FOGLIOTAGLIA",
	"FOGLIOCOPIA",
	"FOGLIOINCOLLA",
	"FOGLIOIMPOSTAFONT",
	"FOGLIOESEGUISELEZIONE",
	"FOGLIOSELEZIONABLOCCO",

	"RIEMPISELEZIONE",
	"SELEZIONAAREAATTIVA",	// 990521
	"ATTIVASELEZIONE",
	"CENTRA",

	"BLOCCOSTAMPA",			// 990526

	"COPIAARC",				// 990701
	"RINOMINAARC",			// 990701
//	"MIDI",					// 990701
	"PROCESSOCREA",			// 990728

	"MENUCREA",				// 990916
	"MENUCANCELLA",			// 991104
	"MENU",					// 991104
	"MENUELEMENTO",			// 991104
	"MENUELEMENTOIMPOSTA",	// 991104
	"MENUELEMENTORIMUOVI",	// 991104
	"MENUELEMENTOASSEGNA",	// 991103

// ---------

	"SCARTA",

	"OGASPROP",			// 991028
//	"TARTAPROP",
//	"TARTATOOL",
	"OGPROP",			// 000130
	"OGTOOL",			// 000130

	"OGSELEZIONE",
	"OGRIMUOVI",
	"OGDUPLICA",
	"OGCENTRA",
//	"NODORECUPERA",
	"OGNODORECUPERA",	// 000130
	"NODOCREA",			// 991025 obsoleta ?
	"NODOINSERISCI",	//  obsoleta ?
//	"NODICONNETTI",
	"OGLINK",			// 000130
//	"ARCOINVERTI",
	"OGLINKINVERTI",	// 000130
	"OGPOS",
	"OGASPOS",
	"OGTRANSLA",
	"OGDIR",
	"OGASDIR",
	"OGRUOTA",
	"OGSCALA",
	"OGASSCALA",
	"OGCOMPONI",
	"OGELEMENTI",
	"OGSCOMPONI",
	"OGACCENDI",
	"OGSPEGNI",
	"OGID",				// 991011

	"VERSIONE",			// 990925
	"FIRMA",			// 010526
	"RADICE",			// 991005

	"IMPOSTAPAGINA",	// 991014
	"TARTAANTEPRIMA",	// 991014

	"OGSELEZIONA",		// 991021
	"OGSPEZZATA",		// 991021
	"OGPOLIGONO",		// 991021
	"OGCURVA",			// 991021
	"OGCURVACHIUSA",	// 991021
	"OGRETTANGOLO",		// 991021
	"OGELLISSE",		// 991021

	"OGINSERISCILABEL",	// 991028	obsoleta ?
	"OGRIMUOVIETICHETTA",// 991028 000130
	"OGCONTAETICHETTE",	// 991028 000130

	"PLAYERCOMANDO",	// 991124

	"OGAPPENDI",		// 991213
	"OGDISEGNA",		// 991213
	"OGSIMBOLO",		// 991213
	"OGASTIPO",			// 991213
//	"SIMBOLOINSERISCI",	// 991213
	"OGNODOINSERISCI",	// 991213 000130

	"OGETICHETTA",		// 991213 000130
	"OGASETICHETTA",	// 991213 000130
	"OGTESTO",			// 991214
	"OGIMMAGINE",		// 000508
	"OGIMMAGINEDIM",	// 000508

	"ASPIXEL",			// 000113
	"OGASPANNING",		// 000313

	"ASLOCALE",			// 000113

	"CREACARTELLA",		// 000118
	"RIMUOVICARTELLA",	// 000118
	"RISALICARTELLA",	// 000118
	"CARTELLE",			// 000118
	"ARCHIVI",			// 000118

	"DISCHI",			// 000131
	"DISCOPRONTO?",		// 000131

	"CREACOLLEGAMENTO",	// 000221
	"CARTELLASHELL",	// 000310
	"COMANDOSHELL",		// 010606

	"FTP_APRI",			// 040221
	"FTP_CHIUDI",		// 040221
	"FTP_GET",			// 040221
	"FTP_PUT",			// 040221
	"FTP_CANCEARC",		// 040222
	"FTP_RINOMINAARC",	// 040222
	"FTP_CARTELLA",		// 040222
	"FTP_CAMBIACARTELLA", // 040222
	"FTP_CREACARTELLA",	// 040222
	"FTP_RIMUOVICARTELLA", // 040222
	"FTP_CATALOGO",		// 040228
	"FTP_ALBERO",		// 040228
	"FTP_ATTR",			// 040228

#ifdef INTEGRA
	"CONFSTATO",		// 000822
	"CONFINIZIA",		// 000822
	"CONFTERMINA",		// 000822
	"CONFCREA",			// 000825
	"CONFCHIAMA",		// 000822
	"CONFVIDEOAPRI",	// 000822
	"CONFVIDEOCHIUDI",	// 000822
	"CONFDATIAPRI",		// 000822
	"CONFDATICHIUDI",	// 000822
	"CONFDATIINVIA",	// 000822
	"CONFBUFFER",	// 010122

	"VOCEACCENDI",		// 000901
	"VOCESCRIVI",		// 000920-010122
	"VOCEDIALOGO",		// 010128
#endif // INTEGRA

#endif // ITALIANO

#ifdef ENGLISH

	"SETTIMER",
	"CLEARTIMER",

	"MIDICOUNT",		// 991116
	"MIDIOPEN",
	"MIDICLOSE",
	"MIDI",				// 990701 991116
	"MIDIMESSAGE",
	"PLAYWAVE",
	"MCI",

	"EDIT",
	",ED",

	"ICON",			// MINIMIZZA
	"MAXIMIZE",		// MASSIMIZZA
	"NORMALIZE",	// NORMALIZZA
	"UNICON",		// RIPRISTINA
	"SETFOCUS",
	"GETFOCUS",

	"KEYBOARDON",
	"KEYBOARDOFF",
	"KEYBOARDVALUE",

	"SCROLLX",
	"SCROLLY",

	"EMPTYWORD",
	"EMPTYLIST",

	"SETPOS",

	"LASTS",
	"BUTLASTS",

	"DUPLICATE",			// 991102
	"COMBINE",
	"REMOVE",
	"REMOVEDUPLICATE",
	"QUOTED",

	"SIGN",
	"GENSYM",

	"SETPENMODE",
//>981209
	"TURTLECREATE",
	"EDITORCREATE",
	"BROWSERCREATE",
	"PLAYERCREATE",

	"TURTLEDELETE",
	"EDITORDELETE",
	"BROWSERDELETE",
	"PLAYERDELETE",
//<
//>990115
	"TURTLE",
	"EDITOR",
	"BROWSER",
	"PLAYER",
//<
//>990112
//	"EDITOR",
	"EDITORLOAD",
	"EDITORSAVE",
	"EDITOREXEC",
//	"TURTLE",
//<
	"LABELDIM",
	"LABELDIMLOG",			// 991223

	"TOOLBARCREATE",
	"TOOLBARDELETE",
	"TOOLBARSETPOS",		// 990917

	"WINDOWSHOW",

	"DATABASEQUERY",
	",DBQUERY",
	"DATABASEEXECUTE",		// 991214
	",DBEXEC",				// 991214

	"INFORMATION",
	",INFO",
	"INFORMATIONSHOW",
	",INFOSHOW",
	"GLOSSARY",
	"GLOSSARYSHOW",

	"_CONTAC",
	"_CONTAL",

	"FOGLIOIMPOSTASTAMPANTE",
	"FOGLIOSTAMPA",
	"FOGLIOTAGLIA",
	"FOGLIOCOPIA",
	"FOGLIOINCOLLA",
	"FOGLIOIMPOSTAFONT",
	"FOGLIOESEGUISELEZIONE",
	"FOGLIOSELEZIONABLOCCO",

	"RIEMPISELEZIONE",
	"SELEZIONAAREAATTIVA",
	"ATTIVASELEZIONE",
	"CENTRA",

	"BLOCCOSTAMPA",			// 990526

	"FILECOPY",				// 990701
	"FILERENAME",			// 990701
//	"MIDI",					// 990701
	"PROCESSCREATE",		// 990728

	"MENUCREATE",			// 990916
	"MENUDELETE",			// 991104
	"MENU",					// 991104
	"MENUELEMENT",			// 991104
	"MENUELEMENTSETSTATE",	// 991104
	"MENUELEMENTREMOVE",	// 991104
	"MENUELEMENTSET",		// 991103

// ---------

	"DISCARD",

	"GOSETPROPS",			// 991028
	"TURTLEPROPS",
	"TURTLETOOLS",

	"SELECTION",
	"GOREMOVE",
	"GOSETCENTER",
	"NODELOAD",
	"NODECREATE",		// 991025
	"NODEINSERT",
	"NODEDUPLICATE",
	"NODESCONNECT",
	"EDGEINVERT",
	"GOPOS",
	"GOSETPOS",
	"GOTRANSLATE",
	"GODIR",
	"GOSETDIR",
	"GOROTATE",
	"GOSCALE",
	"GOSETSCALE",
	"GOGROUP",
	"GOELEMENTS",
	"GOUNGROUP",
	"GOON",
	"GOOFF",
	"GOID",				// 991011

	"VERSION",			// 990925
	"SIGNATURE",		// 010526
	"ROOT",				// 991005

	"PAGESETUP",		// 991014
	"TURTLEPREVIEW",	// 991014

	"GOSELECT",			// 991021
	"GOPOLYLINE",		// 991021
	"GOPOLYGON",		// 991021
	"GOCURVE",			// 991021
	"GOCLOSEDCURVE",	// 991021
	"GORECT",			// 991021
	"GOELLIPSE",		// 991021

	"GOLABELINSERT",	// 991028
	"GOLABELREMOVE",	// 991028
	"GOLABELCOUNT",		// 991028

	"PLAYERCOMMAND",	// 991124

	"GOAPPEND",			// 991213
	"GODRAW",			// 991213
	"GOSYMBOL",			// 991213
	"GOSETTYPE",		// 991213
	"SYMBOLINSERT",		// 991213

	"GOLABEL",			// 991213
//	"GOLABELADD",		// 991213
	"GOLABELSET",		// 000129
	"GOTEXT",			// 991214
	"GOIMAGE",			// 000508
	"GOIMAGEDIM",		// 000508

	"SETPIXEL",			// 000113
	"GOSETPANNING",		// 000313
	"LOCALMAKE",		// 000113

	"MKDIR",			// 000118
	"RMDIR",			// 000118
	"POPDIR",			// 000118
	"DIRECTORIES",		// 000118
	"FILES",			// 000118

	"DRIVES",			// 000131
	"DRIVEREADYP?",		// 000131

	"SHELLLINKCREATE",	// 000221
	"SHELLFOLDER",		// 000310
	"SHELLCOMMAND",		// 010606

#ifdef INTEGRA
	"CONFSTATO",		// 000822
	"CONFINIZIA",		// 000822
	"CONFTERMINA",		// 000822
	"CONFCREA",			// 000825
	"CONFCHIAMA",		// 000822
	"CONFVIDEOAPRI",	// 000822
	"CONFVIDEOCHIUDI",	// 000822
	"CONFDATIAPRI",		// 000822
	"CONFDATICHIUDI",	// 000822
	"CONFDATIINVIA",	// 000822
	"CONFBUFFER",		// 010122

	"VOCEACCENDI",		// 000901
	"VOCESCRIVI",		// 000920-010122
	"VOCEDIALOGO",		// 010128
#endif // INTEGRA

#endif // ENGLISH

#endif // ILWIN

	"#",

/* messaggi informativi */
	
	"stack insufficiente: rivedere configurazione di IPERLOGO",
	"non riesco ad aprire l' archivio $1",
//	"ci sono troppe [",
	"[ o { senza corrispondente ] o }",
//	"ci sono piu' ] che [",
	"] o } non in corrispondenza con [ o {",
	"interrotto !",
	"pausa in $1",
	"$1 termina",
	"$1 riporta $2",
	"ho finito i nodi",
	"fine della definizione di $1",
	"qualcosa non va sul disco",
	"il foglio non basta",
	"non trovo $1",
	"$1 e'troppo lunga",
	"non ho cancellato niente",
	"INSERISCO",
	"MODIFICO ",
	"errore riscontrato in :",
	"cercavo di eseguire : ",
	"#",

/* messaggi di errore */

	"errore in IPERLOGO",						//	0

/* 1- 9	errori in uso primitiva o in valore argomenti */

	"$2non ha un valore",						//	1
	"non so fare$2",							//	2
	"posso fare$1solo in una procedura",		//	3
	"non posso fare$1in una procedura",			//	4
	"a$1non piace$2come ingresso",				//	5
	"$2e' una primitiva",						//	6
	"$2non ha abbastanza elementi",				//	7
	"non posso dividere per 0",					//	8
	"$2e' in pacchetto coperto",				//	9

/* 11-19	errore di sintassi */

	"E10",
//	"troppo pochi ingressi a$1",				//	11
	"troppo pochi o troppi ingressi a$1",		//	11
	"non so che fare con$2",					//	12
	"E13",										//	13
	"ci sono piu' ( che )",						//	14
	"non posso fare$1",							//	15
	"non trovo l' etichetta$2",					//	16
	"non trovo ACCHIAPPA$2",					//	17
	"nella definizione di$2ci sono piu' [ che ]",//	18
	"$1non riporta un valore",					//	19

/* 21-29	errore logico in archivio o device */

	"questo video non e' adatto a$2",			//	20
	"non trovo l' archivio$2",					//	21
//	"l' archivio$2non e' aperto",		//	22
//	"l' archivio$2e' gia' aperto",		//	23
	"l' archivio o dispositivo$2non e' aperto",	//	22
	"l' archivio o dispositivo$2e' gia' aperto",//	23
	"non posso cancellare$2perche' e' aperto",	//	24
//	"non posso leggere da$2",				//	25
	"non posso leggere da / scrivere su$2",		//	25
	"non posso posizionare$2",					//	26
	"posizione oltre la fine",					//	27
	"in FOGLIO non e' definito un blocco",		//	28
	"non posso selezionare$2",					//	29

//	"E30",
	"$2 non puo' essere chiuso in programma lanciato dal suo interno",
 
 /* 31-39	errore fisico in archivio o device */
	"non riesco ad aprire$2",					//	31
	"qualcosa non va sul disco",				//	32
	"qualcosa non va sul disco : forse e' pieno",//	33
	"qualcosa non va sulla STAMPANTE",			//	34
	"errore su linea di comunicazione",			//	35
	"non riesco a chiudere $2",					//	36
	"non riesco a chiudere l'archivio ??? ",	//	37 da rivedere (CHIUDITUTTO)
	"non riesco a fare$1",						//	38
//	"E39",
	"$2richiede la presenza di Internet Explorer 4.0",

/* 41-  errore dipendente da limiti della implementazione */

	"numero troppo grande",						//	40
	"ho finito i nodi",							//	41
	"ricorsione troppo profonda",				//	42
	"troppi RECUPERA/PAUSA/ACCHIAPPA non terminati",//	43
	"a$1non piace la precisione$2",				//	44
	"TARTA contro il MURO",						//	45
	"il foglio  non basta",						//	46
	"non posso fare$1in questa modalita' di schermo",//	47
	"FOGLIO e TARTA non stanno bene insieme",	//	48
	"non posso fare$1perche' manca FOGLIO",		//	49
	"$2e' una procedura compilata",				//	50

#ifdef ILWIN

/* 51-  errore in gestione finestre di dialogo */

	"non posso creare la finestra$2",			//	51
	"la finestra$2 non esiste o non e' adatta per$1",	//	52

#endif // ILWIN

	"#",

/* valori in formato BCD-ASCII per le costanti numeriche */
	"0",
	"1",
	"360",
	"#",
    
	">>"
};
 
