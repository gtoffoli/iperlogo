/* ILSFUN.H	G. Toffoli

	990919	uso di #pragma pack per passaggio /Zp1 -> /Zp8 (standard)
	981121	estratto da LOGO.H,
			spostato campi in descrittore di primitiva
*/


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


/* elemento descrittore di primitiva di sistema (da aggiornare)

  descr :	num. argom. nominale	bit 0-3
		num. argom. facoltativi		bit 4-7
		num. argomenti massimo		bit 8-11
		comando/funzione	bit 5
		precedenza		bit 6-7

  descr_i	lista			bit 0
		parola				bit 1
		vero/falso			bit 2
		intero				bit 3
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
// #define N_ILLIMITATO	(descr_sf.classi & 0x40)
// #define IS_PR_FUNZIONE	(descr_sf.classi & 0x80)
#define N_ILLIMITATO	(descr_sf.descr & 0x1000)
#define IS_PR_FUNZIONE	(descr_sf.descr & 0x2000)

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

#pragma pack(push, ilsfun, 1)

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

#pragma pack(pop, ilsfun)
