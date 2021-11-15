/* LOGO.H	G. Toffoli

000511	pulizia
990919	uso di #pragma pack per passaggio /Zp1 -> /Zp8 (standard)
990126	portato N_VIEW da 16 a 64 (= MAX_DEV)
981224	aggiunto pc_URL, pc_SCHERMO e pc_VAI
981214	aggiunto O_BROWSER, pc_BROWSER e pc_VISTA
981121	aggiunto classe di primitiva PGUI e PMCI
		spostato campi in descrittore di primitiva
		stralciato ILSFUN:H (struttura primitive)
981118	spostato in ilnodi.cpp definizioni
980831	riciclato alcune parole chiavi (GLOSSE, EGA, CGA)
980721	portato MAX_DEV a 64
980709	esteso descrizione di primitiva: N_MASSIMO !!!
		PC_PLAYER e O_PLAYER al posto di PC_BOTTONE e O_BOTTONE
		portato DIM_KW da 70 a 100
980127	ripulitura maggiore dopo passaggio a WIN32
980122	sostituito int con int16 e unsigned con unsigned int16
		nelle strutture dati di dimesnione cablata
970503	def. nuovo device MEMORY, al posto di COM2 (sovrapposto a COM1)
970402	aggiunto pc_EDITA
970315	tentato recupero update del 02.10.96 (parole riservate)
		introdotto BIT_RISERVA al posto di BIT_COMPILA (obsoleto)
960926	eliminato bit obsoleti in campo tipo; aggiunto BIT_NORMALE
		aggiunto tipo LREF per riferimento a parole normalizzate
960603	introdotto tipo ARAY (ARRAY)
960508	aumentato DIM_STRINGHE
960409	prima versione Windows da Iperlogo.h del 06.10.89
*/

#ifndef _LOGO_
#define _LOGO_

#include "dos.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
// #include "contesti.h"	// spostato in fondo

#define MAX_ARGS 10
#define MAX_DESC 3

#define arg_1 (args[0])
#define arg_2 (args[1])
#define arg_3 (args[2])
#define tipo_1 (types[0])
#define tipo_2 (types[1])
#define tipo_3 (types[2])
#define descr_1 (descrs[0])
#define descr_2 (descrs[1])
#define descr_3 (descrs[2])

		/* codifica valori logici e fine-stringa-C */
#define	TRUE    1
#define FALSE   0
		/* codifica di valori per stringhe C */
#define EOS	-1			// end of string
#define NULLC 0			// null character

		/* codifica valori per semaforo */
#define	VERDE	0
#define	ROSSO	1

#define DUMMY   0
#define	node	unsigned long
#define LNODE   4		/* lungh. di identificatore nodo */
#define	NODE_LENGTH (2+2*LNODE)	/* lungh. di structure NODO o NODOIL */

// #define K	1024
#define K	1024L
#define DIM_SETTORE 512	/* dimensione buffer di settore */
#define N_RIGHE	25	/* righe di schermo video */
#define DIM_ASCII  256	/* dimensione set dei codici ASCII a 8 bit */
#define DIM_ESTESI 133	/* dimensione set dei codici estesi */

#define DIM_INDICE	100	/* (gia' 14) dimensione indice dizionario */
#define	DIM_KW  100	/* dimensione dell' indice delle parole chiave */
#define MAX_DEV 64	/* (gia' 15) massimo codice di device */
#define DIM_RIGHE 60 /*45*//* massimo num. di righe gestite da modulo QUADRO */
#define N_VIEW 64 // 16	/* massimo numero di quadri di schermo */
#define N_BOTT 20	/* massimo numero di bottoni invisibili */
#define	DIM_BCD 10	/* dimensione dell' indice delle costanti numeriche */
#define DIM_CODICI 256+133 /* dimensione set dei codici di tastiera IBM */
#define DIM_ARRAY	(DIM_INDICE+DIM_KW+MAX_DEV+1+DIM_BCD+DIM_CODICI+1)
#define DIM_LIB		((LNODE*DIM_ARRAY) + (2 * (3+DIM_INDICE)))

#define DIM_BPIC ((unsigned) 0xEA00) /* dim. buffer per scroll video */
#define DIM_MESSAGGI 75 /* dimensione dell' indice delle stringhe messaggio */
#define DIM_STRINGHE 2000 // 1600 /* dimensione della super-stringa delle stringhe C */
#define DIM_COMANDI 2000/* dimensione del buffer della finestra comandi */
#define DIM_SERVIZIO 81	/* dimensione del buffer della finestra commento */
#define MAX_PROC 200	/* massimo livello di ricorsione di procedure */
#define	MAS_CHAR 0x7f	/* maschera per input carattere */

			/* codifica dei tipi di contesto (id_contesto) */
#define CT_TOP		0L	/* contesto iniziale (top_level) */
#define CT_PAUSA	1L	/* contesto attivato da PAUSA    */
#define CT_RECUPERA	2L	/* contesto attivato da RECUPERA */

			/* codici ASCII*/
#define NUL	0	
#define BS	8 
#define TAB	9 
#define LF	10 
#define CR	13 
#define X_ON	0x11
#define X_OFF	0x13
#define CTRL_Z	0x1A
#define ESC	27
#define SPACE	32
#define LINEDEL	CR	/* gia' LF */
			/* codici estesi di tastiera IBM-PC */
#define LEFT	256+75
#define	RIGHT	256+77
#define INS	256+82
#define	DEL	256+83

#define MAXCHARS    256
#define MAXLEVELS   10

			/*rappresentaz. convenzionale di alcuni valori*/
#define NODI_RIS	16 /* nodi riservati per indirizzi convenzionali */
#define MASC_RIS	0xFFFFFFF0L /* maschera per riconoscere i nodi riservati */
#define IS_RIS(p)	((p & MASC_RIS) == 0) /* parola e' nodo riservato */
#define NULLP		0L /* null pointer, lista vuota */
#define S_VUOTA		1L /* stringa vuota, cdr di ult. nodo pieno di stringa*/
#define NULLVALUE	2L /* null value */
#define DEL_FINELINEA	3L /* parola delimitatore di linea */
#define DEL_VIRGOLETTE	4L /* parola '"' */
#define DEL_DUEPUNTI	5L /* parola ':' */
#define DEL_PARSINISTRA	6L /* parola '(' */
#define DEL_PARDESTRA	7L /* parola ')' */
#define DEL_PIU		8L /* parola '+' */
#define DEL_MENO	9L /* parola '-' */
#define DEL_PER		10L/* parola '*' */
#define DEL_DIVISO	11L/* parola '/' */
#define DEL_ESP		12L/* parola '^' */
#define DEL_MINORE	13L/* parola '<' */
#define DEL_UGUALE	14L/* parola '=' */
#define DEL_MAGGIORE	15L/* parola '>' */

			/*codifica dei tipi di token / oggetto*/
#define	LIST     1
#define	LWORD    2
#define	SFUN     3
#define	UFUN     4
#define	LVAR     5
#define	LSTR     7
#define LBCD	 10
#define	PLIST    11

#ifdef ILWIN
#define	LREF	6	//	reference to other word in the dictionnary
#define	ARAY    12
#endif // ILWIN

	/* codifica di bit in byte o word */
#define BIT7	0x80
#define BIT6	0x40

#ifdef IN_ILSFUN
	/* codifica di bit in descrittore di argomento a SFUN */
 #define A_LISTA		1
 #define A_STRINGA	2
 #define A_VEROFALSO	4
 #define A_NUMERO	8
 #define A_LISTAPAR	16
 #define A_LISTANUM	32
 #define	A_ARRAY 	64 

 #define A_NOMEARC	256
 #define A_PAROLA	512
 #define BIT_RANGE	10
#endif // IN_ILSFUN

		/* codifica dei device MLOGO */
#define MIN_DEV 6 /*4*/	/* minimo codice di device non preallocato */
#define	CONSOLE	0	/* codice di console */
#define	STAMPANTE 1	/* codice di stampante */
#define	FOGLIO	2	/* codice di device del foglio di editor */
#define TARTALFA 3	/* codice di schermo TARTA usato per output alfanum. */
#define COM1 4		/* codice di porta comunicazione n. 1 */

#ifdef ILDOS
#define COM2 5		/* codice di porta comunicazione n. 2 */
#endif // ILDOS
#ifdef ILWIN
#define COM2 COM1		/* codice di porta comunicazione n. 2 */
#define MEMORY 5		/* codice di buffer di memoria */
#endif // ILWIN

		/* codifica dei bit di stato MLOGO (_stato) per file C */
#define O_BINARIO	1	/* file aperto in modalita' binario */
		/*	2	riservato futuri usi per file */
		/*	4	riservato futuri usi per file */
#define O_FINESTRA	8	/* device corrispondente a viewport del GFX */
#define O_TARTA		16	/* finestra di tipo tarta */
#define O_FOGLIO	32	/* finestra di tipo foglio */
#define O_ARCHIVIO	64	/* device di tipo archivio */
//> 980709
//#define	O_BOTTONE	128	/* bottone invisibile */
#define	O_PLAYER	128	/* player MCI */
//< 980709
#define	O_BROWSER	256	/* browser HTML */


		/* maschere che identificano campi in byte di tipo */
#define CAMPO_TIPO	0x0f
// #define CAMPO_LUNG	0x30	// obsoleto ?
// #define BIT_COMPILA	0x10	// obsoleto ?
#define BIT_RISERVA	0x10		// nuovo, per parole di uso riservato
#define BIT_NORMALE	0x20		// nuovo, per normalizzazione nomi
#define BIT_TRACCIA	0x40
// #define BIT_CONTINUA	0x40	// obsoleto ?
#define BIT_LIBERA	0x80

		/* codifica delle costanti bcd tramite indice */
#define	BCD_0		bcd [0]
#define	BCD_1		bcd [1]
#define	BCD_360		bcd [2]

/*<03.03.89 - cambiato numerazione : */
/*<960410 - tolto lo zero dall' indice */
		/* variabili locali a primitive aritmetiche
				   iterative proteggibili durante LIBERA */
#define A		bcd [/*0*/3]	/* argomento 1 */
#define B		bcd [/*0*/4]	/* argomento 2 */
#define F		bcd [/*0*/5]	/* fattore */
#define R		bcd [/*0*/6]	/* risultato parziale */
#define S		bcd [/*0*/7]	/* subtotale */
#define T		bcd [/*0*/8]	/* termine */
#define R2		bcd [/*0*/9]	/* risultato parziale 2 */

		/* variabili globali relative a TARTA */

#define VIDEO_X		320	/* (int) risoluzione logica asse x */
#define VIDEO_Y		200	/* (int) risoluzione logica asse y */
#define VIDEO_X_F	320.0	/* (float) risoluzione logica asse x */
#define VIDEO_Y_F	200.0	/* (float) risoluzione logica asse y */
#define DIM_TARTA	10	/* lunghezza lato triangolo di tarta */

		/* codifica delle parole chiave in dizionario */
#define pc_VUOTA	kw [0]
#define	pc_VERO		kw [1]
#define pc_FALSO	kw [2]
#define pc_CONSOLE	kw [3]
#define pc_STAMPANTE	kw [4]
#define pc_PER		kw [5]
#define pc_FINE		kw [6]
#define pc_AS		kw [7]
#define pc_PAC		kw [8]
#define pc_COPERTO	kw [9]
#define pc_ERRORE	kw [10]
#define pc_GIU		kw [11]	/* GIU' con accento */
#define pc_SU		kw [12]
#define pc_CANCEP	kw [13]
#define pc_INVERP	kw [14]
#define pc_PROGTASTO	kw [15]
#define pc_FOGLIO	kw [16]
#define pc_ETICHETTA	kw [17]
#define pc_STOP		kw [18]
#define pc_CHIUSO	kw [19]
#define pc_APERTO	kw [20]
#define pc_CIRCOL	kw [21]
#define pc_COMANDI	kw [22]
#define pc_TARTA	kw [23]
#define pc_MISTO	kw [24]
#define pc_RECUPERA	kw [25]
#define pc_COM1		kw [26]
#define pc_COM2		kw [27]
#define pc_GIU_SENZA	kw [28]	/* GIU senza accento */
#define pc_ASPROP	kw [29]
#define pc_VAR		kw [30]
#define pc_PROC		kw [31]
#define pc_PULISCI	kw [32]
#define pc_XOFF		kw [33]
#define pc_FULL		kw [34]
#define pc_TXLF		kw [35]
#define pc_RXLF		kw [36]
#define pc_BINARIO	kw [37]
#define pc_APPENDI	kw [38]
#define pc_MAIUSCOLE	kw [39]
#define pc_POS  	kw [40]
#define pc_DIM		kw [41]
#define pc_SFONDO	kw [42]
#define pc_STATO	kw [43]
#define pc_DIMBUF	kw [44]
#define pc_COLTESTO	kw [45]
#define pc_CURSORE	kw [46]
#define pc_ASLISTAPROP	kw [47]
#define pc_AND		kw [48]
#define pc_OR		kw [49]
#define pc_XOR		kw [50]
//>980709
//#define pc_BOTTONE	kw [51]
#define pc_PLAYER	kw [51]
//<980709
//#define pc_STILE	kw [52]
#define pc_BROWSER	kw [52]
//#define pc_CENTRATO	kw [53]
#define pc_VISTA	kw [53]
#define pc_ACCAPO	kw [54]
#define pc_ARCHIVIO	kw [55]
//#define pc_GLOSSE	kw [56]
#define pc_MENU		kw [56]	// gia' pc_GLOSSE
#define pc_FONT		kw [57]
#define pc_TIPO         kw [58]
#define pc_SCALA	kw [59]
#define pc_DIR		kw [60]
#define pc_PIXEL	kw [61]
//#define pc_EGA	kw [62]
#define pc_TITLE	kw [62]	// gia' pc_EGA
//#define pc_CGA	kw [63]
#define pc_PAINT	kw [63]	// gia' pc_CGA
#define pc_VGA          kw [64]
#define pc_HERC         kw [65]
#define pc_MODO         kw [66]
#define pc_EDITA		kw [67]

#define pc_APRI			kw [68]
#define pc_CHIUDI		kw [69]
#define pc_ALIAS		kw [70]
#define pc_INFO			kw [71]
#define pc_CAPACITA		kw [72]
#define pc_RIPRODUCI	kw [73]
#define pc_URL			kw [74]
#define pc_SCHERMO		kw [75]
#define pc_VAI			kw [76]

#ifdef IN_ILSFUN

	/* elemento descrittore di primitiva di sistema */
/*
  descr :	num. argom. nominale	bit 0-1
		num. argom. facoltativi	bit 2-3
		"num. argomenti illim."	bit 4
		comando/funzione	bit 5
		precedenza		bit 6-7

  descr_i	lista			bit 0
		parola			bit 1
		vero/falso		bit 2
		intero			bit 3
		lista di parole		bit 4
		lista di numeri		bit 5
		meglio parola		bit 6
		meglio numero		bit 7

  classi	tarta			bit 0
		foglio			bit 1
*/	

#define N_NOMINALE	(descr_sf.descr & 0x0F)
#define N_MINIMO	((descr_sf.descr >> 4) & 0x0F)
#define N_MASSIMO	((descr_sf.descr >> 8) & 0x0F)	// 980709
#define N_ILLIMITATO	(descr_sf.classi & 0x40)
#define IS_PR_FUNZIONE	(descr_sf.classi & 0x80)

/* codifica di classi di primitiva */

#define	TURT	1	// IS_PR_TARTA
#define	EDIT	2	// IS_PR_FOGLIO
#define	TOPL	4	// IS_PR_TOP
#define	PROC_	8	// IS_PR_PROC
#define	TXOU	16	// IS_PR_SCRIVI
#define	EXEC	32	// IS_PR_ESEGUI
#define	PGUI	64	// IS_PR_GUI
#define	PMCI	128	// IS_PR_MM

#define IS_PR_TARTA	(descr_sf.classi & TURT)	/* app. a classe TARTA */
#define IS_PR_FOGLIO (descr_sf.classi & EDIT)	/* app. a classe FOGLIO */
#define IS_PR_TOP	(descr_sf.classi & TOPL)	/* legale solo a TOP-level */
#define IS_PR_PROC	(descr_sf.classi & PROC_)	/* legale solo in procedura */
#define IS_PR_SCRIVI (descr_sf.classi & TXOU)	/* prim. di scrittura */
#define IS_PR_ESEGUI (descr_sf.classi & EXEC)	/* prim. ESEGUI SE SEV SEF ALTRIMENTI */
#define IS_PR_GUI (descr_sf.classi & PGUI)		/* app. a classe PGUI (Windows GUI) */
#define IS_PR_MM (descr_sf.classi & PMCI)		/* app. a classe PMCI (Interfaccia MCI) */

struct primitiva {	// aggiunto "unsigned" 960423
//>980709
//	unsigned char descr;	// descrizione generale
//	unsigned char classi;	// classi di appartenenza (1 bit per classe)
	unsigned int descr;		// descrizione generale
	unsigned int classi;	// classi di appartenenza (1 bit per classe)
//<980709
//	unsigned int descrs[8];	// descrizione di argomenti fino a 8
	unsigned short int descrs[8];// descriz. di argomenti fino a 8	// aggiunto "short" 980123
	void (*corpo) ();// indirizzo implementazione
	};

#endif // IN_ILSFUN

#pragma pack(push, logo, 1)

struct	hl {
	char l;
	char h;
};

union	u_hl {
//	unsigned u;		// modificato 980123
	unsigned short u;
	struct hl hl;
};

struct dview {
int	view,			/* handle di viewport GFX */
						/* estremi fisici (copia da GFX) : */
     	min_x, min_y,			/* viewport's upper left corner */
     	max_x, max_y;			/* viewport's lower right corner */

int	prima, dopo;		/* doppio link (indici a dview) */

 double	postarta,	/* Posizione di cursore scrivi su schermo TARTA */
	pos_x_t,		/* posizione x di tarta */
	pos_y_t,		/* posizione y di tarta */
	min_x_t,		/* minimo valore di x su video tarta*/
	max_x_t,		/* massimo valore di x su video tarta */
	min_y_t,		/* minimo valore di y su video tarta */
	max_y_t,		/* massimo valore di y su video tarta */
  	lx_a,			/* quantita' da			*/
	ly_a, 			/* sommare alle coordinate	*/
	lx_b,			/* attuali per avere i tre	*/
	ly_b,			/* vertici della tartaruga	*/
	lx_c,
	ly_c,
	alfa,			/* direzione attuale di tarta */
	senalfa,		/* seno di alfa */
	cosalfa;		/* coseno di alfa */

int	fltarta;	/* Modalita' di tartaruga  (VISIBILE, INVISIB.) */
node	flvideo;	/* Modalita'di video  (CHIUSO, APER., CIRC.)    */
node	penupdown;		/* Modalita' di penna (SU, GIU', INV., CANC.)   */
node	penmode;		/* Modalita' di penna (PAINT, INVERT, ERASE)   */
double	xScale, yScale;
int	xOrg, yOrg;
int	xDir, yDir;
int bitIndex;
int bitMode;
double xLowArea, yLowArea, xHighArea, yHighArea;
};


/* predicati che fanno uso
   dell' array di descrizione dei codici in MLCODICI */

#define C_EINI	1
#define C_DEL	2
#define C_OPER	4
#define C_WDEL	8
#define C_LDEL	16

#define IS_INI_ESPR(c)		(desc_c [c] & C_EINI)
#define IS_DELIMITATORE(c)	(desc_c [c] & C_DEL)
#define IS_OPERATORE(c)		(desc_c [c] & C_OPER)
#define IS_DEL_PAROLA(c)	(desc_c [c] & C_WDEL)
#define IS_DEL_LINEA(c)		(desc_c [c] & C_LDEL)

/* definizione di variabili di lavoro come campi di strutture */

#define	BIT_MARCA	128	/* bit in attr. per stringhe marcate */

		/* codifica delle finestre */
#define Q_COMANDI	0	/* quadro0 */
// #define Q_FOGLIO	1	/* quadro1 */
// #define Q_COMMENTO	2	/* quadro2 */
#define Q_TARTA		3	/* quadro3 */

/* strcture composta da offset e segment nell' ordine fisico di 8086 */
struct o_s {
//	unsigned off;		// 980122
//	unsigned seg;		// 980122
///	unsigned short off;	// 980123
///	unsigned short seg;	// 980123
	int off;
	};

/* union tra i vari long pointer e coppia di unsigned (offset segment) */
union vp_os {
	char *charp;
	node *longp;
//	int *intp;	// 980122
	short int *intp;
	double *doublep;
	struct o_s os;
	};

/* union tra long pointer e coppia di unsigned (offset segment) */
union p_os {
	char *p;
	struct o_s os;
	};

/* union tra long pointer ad un long e coppia di  FORSE NON SERVE PIU' 14.11.88
   unsigned (offset segment) */
union lp_os {
	node *lp;
	struct o_s os;
	};

struct nodoil {
//	int type;	// 980122
	short int type;
	node car;
	node cdr;
};

struct	car_cdr {
	node car;
	node cdr;
	};

union	cell {
	double real;
	char string [NODE_LENGTH - 2];
	struct car_cdr cc ;
	};

struct	nodo {
	char typ;
	char length;
	union cell value;
	};

#include "contesti.h"	// spostato qui da sopra

#pragma pack(pop, logo)

#endif // _LOGO_
