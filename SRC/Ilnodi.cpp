/*	ILNODI.C 	G. Toffoli
	creazione e gestione dello spazio dei nodi

010602	in FIRMA e CREALIB aggiunto indicatore di libreria free (isLibraryFree)
010115
010111	in ScriviFirma ripristino attributi file (data)
010110	dimensione e struttura firma_lib
001225	corretto ScriviFirma
001218	aggiunto secondo argomento a CREALIB (libcode)
		aggiunti LeggiLibreria e ScriviFirma
001126	corretto CREALIB e _lib
991229	aumentato MAX_PAG_BASE a 256
991218	iniziato recupero fi CREALIB e LIB (spostate da ilspazio.cpp)
981119	ritoccato prepara_mem e prepara_mem_base;
		aumentato MAX_PAG_BASE da 24 a 64
980831	in consstr, consptr: char * -> const char *
980828	preparazione per conversione a C++
960128	Ripulitura generale per WIN32, perdendo compatibilita' DOS
960409	prima versione per Windows da Iperlogo del 23.04.89
		eliminato codice per memoria estesa, espansa e file
*/

#include "stdafx.h"

#define ILWIN

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include <sys\stat.h>
#include <io.h>

/* codifica di valori relativi a Vectra-EMS (exp. memory manager) ILNODI.C */
#define	DIM_PAGINA (16*K)	/* dim. pagina mem. base o mem. estesa */	
#define BYTES_PAGINA 16380	/* 16kb - 4 per divisibilita' per 10 */
#define NODI_PAGINA (BYTES_PAGINA/NODE_LENGTH)
#define MAX_PAG_BASE 256	/* gia' 24 e poi 64: max. numero di pagine mem. */ 

char	*pagine_base [MAX_PAG_BASE+1];	/* puntatori alla mem. base per i nodi */

void vai_top ();	// 980828

extern void IperlogoExit ();
#ifdef ILWIN
extern void IperlogoExit ();
#endif // ILWIN

void valid (node nodo)
{
	if ((nodo < 0L) || (nodo >= tot_nodi)) {
		printf_long ("\n nodo illegale : %.4lx ", nodo);
		vai_top ();
	}
}

/********************************************
FUNZIONI PRIMITIVE
*******************************************/

int MemPages;

int pag_off (node nodo)
// node nodo;
{
	unsigned long virt_offset;	// node virtual offset (trascurando sfrido di pagina)
	int pag;					// page index
	pag = (virt_offset = (nodo * NODE_LENGTH)) / BYTES_PAGINA;
	ris.os.off = virt_offset - (pag * BYTES_PAGINA);
	return pag;    
}

/********************************************
PROCEDURE E FUNZIONI PER L' ACCESSO
AL CONTENUTO DEI NODI
*******************************************/
void leggi_nodo (node nodo)
{
	int lpage;
	valid (nodo);
	
	/* --------------------------------------------------------------------------
	Mette nella variabile globale il puntatore al nodo richiesto.
	Riporta l'offset del puntatore per uniformita' con le altre 
	routine di gestione dei nodi.
	-------------------------------------------------------------------------- */
	lpage = pag_off (nodo); /* !!! SIDE-EFFECT su ris.os.off !!! */
	/* offset di base + offset nodo */
	ris.charp = pagine_base [lpage] + ris.os.off;
	
}

/* ------------------------------------------------
riporta la word tipo-lunghezza
------------------------------------------------*/
int wordtype (node nodo)
{
	leggi_nodo(nodo);
	return *ris.intp;
}

/* ------------------------------------------------
riporta un intero con solo i bit che servono a
distinguere il tipo di nodo logo
------------------------------------------------*/
int typ (node nodo) 
{
	leggi_nodo (nodo);
	return (*ris.intp & CAMPO_TIPO);
}

/* ------------------------------------------------
mette (e riporta) la word del tipo 
------------------------------------------------*/
int puttyp (node nodo, int type)
{
	leggi_nodo (nodo);
	return (*ris.intp = type);
}

/* ------------------------------------------------
riporta il car nel nodo 
------------------------------------------------*/
node car (node nodo)
{
	leggi_nodo(nodo);
	return * (node *) (ris.charp + 2);	/* + 2 per prendere il car	*/
}

/* ------------------------------------------------
mette (e riporta) il car del nodo 
------------------------------------------------*/
node putcar (node nodo, node _car)
{
	valid (_car);
	leggi_nodo(nodo);
	return (* (node *) (ris.charp + 2) = _car); /* + 2 per prendere il car	*/
}

/* ------------------------------------------------
riporta il cdr nel nodo 
------------------------------------------------*/
node cdr (node nodo)
{
	leggi_nodo(nodo);
	return * (node *) (ris.charp + 6);	/* + 6 per prendere il car	*/
}

/* ------------------------------------------------
mette (e riporta) il cdr del nodo 
------------------------------------------------*/
node putcdr (node nodo, node cdr)
{
	valid (cdr);
	leggi_nodo(nodo);
	return (* (node *) (ris.charp + 6) = cdr);	/* + 6 per prendere il cdr	*/
}

// node consptr (char *ptr)
node consptr (const char *ptr)
{
	node nodo;
	if (--nodi == 0L) {
		msg (9);			/* "ho finito i nodi" */
		vai_top ();
		return NULLP;		// 980828
	} else {
		nodo = frelst;		/* sgancia nodo da lista libera */
		frelst = cdr (nodo);
		leggi_nodo (nodo);
		*ris.intp = LBCD;	/* tipo reale per evitare garbage collection ??? */
		ris.os.off += 2;
		*ris.longp = (unsigned long) ptr;
		/* aggiorna ult_cons */
		if (nodo > ult_cons) ult_cons = nodo;
		return nodo;
	}
}

char *node2ptr (node nodo)
{
	leggi_nodo (nodo);
	return (char *) * (unsigned long *) (ris.charp + 2);
}

/*-------------------------------------------
crea un nodo contente un reale e ne riporta il numero
( = indir. nello spazio dei nodi)
attenzione: non usa car e cdr ma lavora sul nodo
-------------------------------------------*/
node consreal (double reale)
{
	node nodo;
	if (--nodi == 0L) {
		msg (9);		/* "ho finito i nodi" */
		vai_top ();
		return NULLP;		// 980828
	} else {
		nodo = frelst;	/* sgancia nodo da lista libera */
		frelst = cdr (nodo);
		/* ris.os.off = */ leggi_nodo (nodo);/* + 0 per prendere il tipo */
		*ris.intp = LBCD;			/* tipo reale */
		ris.os.off += 2;			/* + 2 per reale (=car e cdr) */
		*ris.doublep = reale;
		/* aggiorna ult_cons */
		if (nodo > ult_cons) ult_cons = nodo;
		return nodo;
	}
}

/* ------------------------------------------------
riporta il numero reale contenuto nel nodo 
------------------------------------------------*/
double node2real (node nodo)
{
	leggi_nodo (nodo);		/* + 2 per reale (=car e cdr) */
	return * (double *) (ris.charp + 2);	/* + 2 per reale (=car e cdr) */
}

/*-------------------------------------------
crea un nodo contente una stringa (n caratteri) e ne 
riporta il numero ( = indir. nello spazio dei nodi)
attenzione: non usa car e cdr ma lavora sul nodo
-------------------------------------------*/
// node consstr (int n, char *s)
node consstr (int n, const char *s)
{
	node nodo;
	if (--nodi == 0L) {
		msg (9);			/* "ho finito i nodi" */
		vai_top ();
		return NULLP;		// 980828
	} else {
		nodo = frelst;			/* sgancia nodo da lista libera */
		frelst = cdr (nodo);
		/* ris.os.off = */ leggi_nodo (nodo);/* + 0 per prendere il tipo */
		*ris.intp = (n<<8) + LSTR;		/* lunghezza n - tipo stringa*/
		ris.os.off += 2;			/* + 2 per corpo (=car e cdr) */
		memcpy (ris.charp, s, n);
		if (n == 4) {			/* cdr e' significativo ? */
			ris.os.off += 4;				/* si: + 6 per cdr */
			*ris.longp = NULLP;
		};
		/* aggiorna ult_cons */
		if (nodo > ult_cons) ult_cons = nodo;
		return nodo;
	}
}

/* ------------------------------------------------
copia in  s  i caratteri contenuti nel nodo e riporta 
il numero di caratteri copiati 
------------------------------------------------*/
unsigned int node2str (node nodo, char *s)
{
	unsigned int n;
	/* register char n,i; */
	leggi_nodo (nodo);
	++ris.os.off;
	n = *ris.charp;
	++ris.os.off;				/* + 1 per corpo (=car e cdr) */
	memcpy (s, ris.charp, n);
	return n;
}

/* ------------------------------------------------
copia i 10 (=NODE_LENGTH) bytes costituenti s dentro
il nodo 
------------------------------------------------*/
void putnode (node nodo, char *s)
{
	leggi_nodo (nodo);	/* + 0 per mettersi all'inizio */
	memcpy (ris.charp, s, NODE_LENGTH);
}

/* ------------------------------------------------
prende i 10 (=NODE_LENGTH) bytes costituenti un nodo e
li copia in  s
------------------------------------------------*/
void getnode (node nodo, char *s)
{
	leggi_nodo (nodo);	/* + 0 per mettersi all'inizio */
	memcpy (s, ris.charp, NODE_LENGTH);
}

/*-----------------------------------------------------------
aggiunge un carattere a cella di stringa LOGO, senza controlli
ma incrementando il campo lunghezza
-----------------------------------------------------------*/
void put_char (node cella, char c)
{
	char n;
	leggi_nodo (cella);
	*(ris.charp + (n = ++(*(ris.charp + 1))) + 1) = c;
	if (n == 4) * (node *) (ris.charp + 6) = NULLP;/* se necess. CDR = NULLP */
}


/*************************************************************************
PROCEDURE E FUNZIONI DI SERVIZIO
*************************************************************************/

/* -------------------------------------------------------------------------
Prepara la memoria per lo spazio dei nodi.
A seconda dei valori settati con il comando SET del dos usa la memoria
espansa (standard LIM) o un file.
Il nome del file o il numero di pagine di mem. da usare vengono
passate via environement (SET).

  Riporta  -1	se qualcosa e' andato storto, oppure
  n che e' il numero di pagine disponibili (n <= maxpag)
		(nel caso del file riporta 100 pagine disponibili)
---------------------------------------------------------------------- */
int prepara_mem ()
{
	int maxpag;
	
//	maxpag = MemPages;
	maxpag = __min (MemPages, MAX_PAG_BASE);

//	if ((maxpag < 4) || (maxpag > MAX_PAG_BASE))
//		return (-1);		/* massimo MAX_PAG_BASE (24) pagine */
	return prepara_mem_base (maxpag);
}

/* --------------------------------------------------------------------------
Si prepara ad usare la memoria BASE come spazio dei nodi

  Input  maxpag che e' il numero massimo di pagine di memoria da allocare
		per i nodi (1 pag. = 1638 nodi da 10 bytes)
		
		  Riporta  -1	se qualcosa e' andato storto, oppure
		  n che e' il numero di pagine disponibili (n <= maxpag)
		  
			Inizializza la variabile pt_nodi_base  che contiene il puntatore
			alla zona di memoria dei nodi
------------------------------------------------------------------------ */
int prepara_mem_base (int maxpag)
{
	int i;
	for ( i=0 ; i<maxpag ; i++ ) {
		pagine_base [i] = (char *) malloc (BYTES_PAGINA);
		if (pagine_base [i] == NULL)
//			return (i <= 3) ? (-1) : i; // almeno 4 pagine
			return -1;
	};
//	pagine_base [MAX_PAG_BASE] = NULL;	// tappo finale per rilascia_mem
	pagine_base [maxpag] = NULL;		// tappo finale per rilascia_mem
	return maxpag;
}

/*-------------------------------------------
rilascia la memoria occupata dai nodi
--------------------------------------------*/
void rilascia_mem ()
{
	/* smista alla routine competente */
	rilascia_mem_base ();
}

/*-------------------------------------------
rilascia la memoria BASE occupata dai nodi
--------------------------------------------*/
void rilascia_mem_base ()
{
	int i = 0;
	while  (pagine_base [i] != NULL)
		free (pagine_base [i++]);
}


/*-------------------------------------------
marca un nodo
(pone ad 1 il bit specificato del campo tipo)
--------------------------------------------*/
void set_tipo (node nodo, int bit)
{
	leggi_nodo (nodo);
	*ris.intp |= bit;
}

/*-------------------------------------------
smarca un nodo
(pone ad 0 il bit specificato del campo tipo)
--------------------------------------------*/
void res_tipo (node nodo, int bit)
{
	leggi_nodo (nodo);
	*ris.intp &= (~bit);
}

/*-------------------------------------------
ritorna TRUE (e ZF falso) se il nodo e' marcato,
FALSE (e ZF vero) altrimenti
--------------------------------------------*/
int bit_tipo (node nodo, int bit)
{
	return (wordtype (nodo) & bit);
}

/********************************************
PROCEDURE AUSILIARIE AL GARBAGE COLLECTION
*******************************************/

/*------------------------------------------------------------------------
partendo da un oggetto, marca un intera rete di nodi, cioe' tutti i nodi
appartenenti all' oggetto o comunque ad esso connessi tramite catene di
car e cdr; la rete viene percorsa prima in profondita'
------------------------------------------------------------------------*/
void marca_rete (node nodo)
{
	int traccia = FALSE;
	int livello;
	node temp;
	union u_hl tipo;
	if (is_smarcato (nodo)) {
		livello = 0;
		while (TRUE) {
			tipo.u = wordtype (nodo) & 0x0f0f;
			if (   (desc_ogg [tipo.hl.l] & 0xf0)	/* se car di nodo e' nodo */
				&& ((temp = car (nodo)) != NULLP)
				&& is_smarcato (temp)			/*	non gia' marcato */
				) { 					/* scende a figlio */
				push_sc (nodo); nodo = temp; ++livello;
			}
			else {
no_giu:
			/* marca nodo dopo essere sceso (o aver tentato) */
			/* e prima di tentare di avanzare a destra */
			set_tipo (nodo, BIT_LIBERA);
			if (
				(	(desc_ogg [tipo.hl.l] & 0x0f) /* cdr di nodo e' nodo ? */
				|| ((tipo.hl.l == LSTR) && (tipo.hl.h == 4)))
				&& ((temp = cdr (nodo)) != NULLP)
				&& is_smarcato (temp)
				)
				nodo = temp;				/* a destra (a frat. minore) */
			else {					/* risale se puo'*/
				if (livello == 0) break;
				nodo = pop_sc ();
				--livello;
				tipo.u = wordtype (nodo) & 0x0f0f;
				goto no_giu;
			}
			}
		}
	}
}

/*-----------------------------------------------------------------
ritorna TRUE se il nodo non e' marcato nel bit riservato a LIBERA
-----------------------------------------------------------------*/
int is_smarcato (node nodo)
{
	return ! bit_tipo (nodo, BIT_LIBERA);
}

/*-------------------------------------
marca tutti i nodi della lista libera
-------------------------------------*/
void marca_ll ()
{
	node nodo;
	nodo = frelst;
	while ((nodo != NULLP) && (nodo <= ult_cons)) {
		leggi_nodo (nodo);	
		*ris.intp |= BIT_LIBERA;
		nodo = * (node *) (ris.charp + 6);
	};
}

/*-------------------------------------------
smarca tutti i nodi nel bit riservato
al garbage collection (non serve ?)
smarca_n ()
{
node i;
for (i = 0; i < tot_nodi; ++i) res_tipo (i, BIT_LIBERA);
}
--------------------------------------------*/

/*-------------------------------------------
libera tutti i nodi non marcati
dalla primitiva di garbage collection
e smarca gli altri
--------------------------------------------*/
void pacca_n ()
{
	node i;
	// int tipo;
	// char s [128];
	for (i = 0; i <= ult_cons; ++i)
	{
		leggi_nodo (i);  
		if (*ris.intp & BIT_LIBERA) *ris.intp ^= BIT_LIBERA;
		else {
			* (ris.intp++) = 0; /* annulla TIPO (non serve) */
			* (ris.longp++) = NULLP;	/* annulla CAR (non serve) */
			*  ris.longp = frelst;	/* aggancia a lista libera */
			frelst = i;
			++nodi;
		};
	}
}

/********************************************
PROCEDURE AUSILIARIE AL TRACCIAM. DI OGGETTI
********************************************/

void _nontracciare ()
{
	node i;
	for (i = 0; i < tot_nodi; ++i) {
		leggi_nodo (i);  
		*ris.intp &= (~BIT_TRACCIA);
	}
} 

/********************************************
PROCEDURE PER LA CREAZIONE DELLO SPAZIO
DEI NODI, PER IL PRELIEVO E LA RESTITUZIONE
DEI NODI STESSI
;******************************************* */

/*---------------------------------------
inizializza la lista dei nodi liberi.
Ogni pagina contiene BYTES_PAGINA/NODE_LENGTH nodi (ora e' 1638 nodi)
----------------------------------------*/
void inifre (int pagine, int pagina)
// int pagine, /* pagine totali */
// 	pagina; /* pagina succ. a quelle event. gia' inizializzate */
{
	node nodo;
	/* lette gia' tutte le pagine ? */
	if (pagina == pagine) return;
	/* primo dei nodi da inizializzare */
	nodo = pagina * (long) NODI_PAGINA;
	/* costruisce la catena dei cdr */
	while (nodo < tot_nodi) {
		leggi_nodo (nodo);
		* (ris.intp++) = 0; 		/* azzera TIPO: non serve */
		* (ris.longp++) = NULLP;	/* azzera CAR : non serve */
		*  ris.longp = ++nodo;		/* mette CDR */
	};
}

/*-------------------------------------------
crea un nodo con i componenti specificati e
ne riporta il numero
( = indir. nello spazio dei nodi)
function cons (car, cdr, typ : integer) : integer
-------------------------------------------*/
node cons (node car, node _cdr, int type)
{
	node nodo;
	if (--nodi == 0L) {
		msg (9);		/* "ho finito i nodi" */
		vai_top ();
		return NULLP;		// 980828
	} else {
		nodo = frelst;	/* sgancia nodo da lista libera */
		leggi_nodo (nodo);
		*(ris.intp++) = type;
		*(ris.longp++) = car;
		frelst = *ris.longp;
		*ris.longp = _cdr;
		
		/* aggiorna ult_cons */
		if (nodo > ult_cons) ult_cons = nodo;
		return nodo;
	}
}

long int LibHeader [7];
extern char nome_lib [32];
// *******************************
//	Nuova struttura Firma:
//	12 byte : codice di registrazione
//	 4 byte : isLibraryFree (già liberi)
//	 4 byte : numero libreria
//	 4 byte : data prima esecuzione
// *******************************
// #define DIM_FIRMA 16
#define DIM_REGCODE 12
#define OFFSET_FREE DIM_REGCODE
#define OFFSET_LIBSERIAL (OFFSET_FREE + 4 )
#define OFFSET_DATA (OFFSET_LIBSERIAL + 4)
#define DIM_FIRMA (OFFSET_DATA + 4)
char firma_lib [DIM_FIRMA];
char* msg_lib = "non riesco ad aprire la libreria";
#include <afxwin.h>		// MFC core and standard components
#include <direct.h>		// Directory control
extern CString execDirectory;// directory of exe file in command line

/*-------------------------------------------------------------------------
crea su archivio un' immagine a basso livello dello spazio di lavoro e di
alcune aree dati dell' interprete ad esso correlate
-------------------------------------------------------------------------*/
void CREALIB ()
{
	memset(firma_lib, 0, sizeof(firma_lib));
	* ((int *) &firma_lib[OFFSET_LIBSERIAL]) = num_int(arg_2);
//>010602
	int isLibraryFree = FALSE;
	if (n_argomenti >= 3) isLibraryFree = bool_(arg_3);
	* ((int *) &firma_lib[OFFSET_FREE]) = isLibraryFree;
//<010602
	int file_lib, i=0;
	next_pagina = 1 + ult_cons / NODI_PAGINA;	/* 1 + max pagina usata */
	pacstr (nome_lib, nome (arg_1));	/* nome dell' archivio */
	LibHeader [0] = (long int) frelst;
	LibHeader [1] = (long int) ult_cons;
	LibHeader [2] = (long int) nodi;
	LibHeader [3] = (long int) tot_nodi;
	LibHeader [4] = (long int) next_pagina;
	LibHeader [5] = (long int) n_parole;	// 001126
	LibHeader [6] = (long int) n_indice;	// 001126

//	if ((file_lib = open (nome_lib, O_WRONLY | O_CREAT | O_TRUNC | O_RAW)) != -1)
	if ((file_lib = open (nome_lib, O_WRONLY | O_CREAT | O_TRUNC | O_RAW, _S_IREAD | _S_IWRITE)) != -1)
		/* "firma" */
		if (write (file_lib, firma_lib, DIM_FIRMA) == DIM_FIRMA)
			/* stato spazio dei nodi*/
			if (write (file_lib, (char *) &LibHeader[0], sizeof(LibHeader)) == sizeof(LibHeader))	// 001126
				/* indice del dizionario */
//>001126
//				if (write (file_lib, (char *) &n_parole, DIM_LIB) == DIM_LIB)
if (	(write (file_lib, (char *) &conto_diz[0], sizeof(conto_diz)) == sizeof(conto_diz))
	&&	(write (file_lib, (char *) &indice_diz[0], sizeof(indice_diz)) == sizeof(indice_diz))
	&&	(write (file_lib, (char *) &kw[0], sizeof(kw)) == sizeof(kw))
	&&	(write (file_lib, (char *) &_nome[0], sizeof(_nome)) == sizeof(_nome))
	&&	(write (file_lib, (char *) &tab_prog[0], sizeof(tab_prog)) == sizeof(tab_prog))
)
//<
			/* pagine di memoria dei nodi */
			for (i=0; i < next_pagina; ++i) {
				if (write (file_lib, pagine_base [i], BYTES_PAGINA) != BYTES_PAGINA) break;
			};
			if ((i == (next_pagina)) && (close (file_lib) == 0));/* Tutto bene */
			else err1 (33);
}

void LIB ()
{
	int file_lib;
	if (! is_lib) {
		pacstr (nome_lib, nome (arg_1));
		if ((file_lib = open (nome_lib, O_RDONLY | O_RAW)) == -1)
			err2 (21, arg_1);			// "Non trovo l'archivio .."
		read (file_lib, firma_lib, DIM_FIRMA); // scarta la firma
		_lib (file_lib);
	};
}

int _lib (int file_lib)
{
/* < NON SPOSTARE (vedi anche CREALIB e modulo ILCOMUNE) */
	node frelst_lib, 	/* nodo iniziale lista libera */
		ult_cons_lib;		/* massimo numero di nodo usato (CONS) */
	long int nodi_lib,		/* numero nodi liberi */
		tot_nodi_lib;		/* numero nodi in totale */
	int pagine_lib; 	/* numero pagine di mem. base o estesa */
/* > FINO A QUI */
	node next_nodo;
	int i = 0;

	/* stato spazio dei nodi origine */
	if (read (file_lib, (char *) &LibHeader[0], sizeof(LibHeader)) == sizeof(LibHeader))	// 001126
		frelst_lib = LibHeader [0];
		ult_cons_lib = LibHeader [1];
		nodi_lib = LibHeader [2];
		tot_nodi_lib = LibHeader [3];
		pagine_lib = (int) LibHeader [4];
		n_parole = LibHeader [5];
		n_indice = LibHeader [6];

		/* test su compatibilita' spazi */
		if (ult_cons_lib < tot_nodi)
			/* indice del dizionario */
//>001126
//			if (read (file_lib, (char *) &n_parole, DIM_LIB) == DIM_LIB)
if (	(read (file_lib, (char *) &conto_diz[0], sizeof(conto_diz)) == sizeof(conto_diz))
	&&	(read (file_lib, (char *) &indice_diz[0], sizeof(indice_diz)) == sizeof(indice_diz))
	&&	(read (file_lib, (char *) &kw[0], sizeof(kw)) == sizeof(kw))
	&&	(read (file_lib, (char *) &_nome[0], sizeof(_nome)) == sizeof(_nome))
	&&	(read (file_lib, (char *) &tab_prog[0], sizeof(tab_prog)) == sizeof(tab_prog))
)
//<
				/* pagine di memoria dei nodi */
				for (i=0; i<pagine_lib; ++i) {
					if (read (file_lib, pagine_base [i], BYTES_PAGINA) != BYTES_PAGINA) break;
				};
	/* aggiorna stato spazio dei nodi */
	frelst = frelst_lib;
	ult_cons = ult_cons_lib;
	nodi = nodi_lib + (tot_nodi - tot_nodi_lib);
	if (pagine_lib < pagine) {
		next_nodo = pagine_lib * (node) NODI_PAGINA;
		putcdr (next_nodo - 1, next_nodo);
	}
	else putcdr (tot_nodi-1, NULLP);
	if ((i == pagine_lib) && (close (file_lib) == 0))
		return pagine_lib;			/* Tutto bene */
	IperlogoExit ();
	return FALSE;
}

int LeggiLibreria ()
{
	int file_lib;
	if ((file_lib = open (nome_lib, O_RDONLY | O_RAW)) == -1)
		{ AfxMessageBox (msg_lib); exit (1); }
	read (file_lib, firma_lib, DIM_FIRMA);
	if (firma_lib [0] == 26) {
		close (file_lib);		/* libreria dummy ? */
		is_lib = FALSE;
		return 0;
	}
	else {
		return _lib (file_lib);	/* carica libreria */
	}
}

BOOL ScriviFirma ()
{
	// salva cartella corrente e si posiziona su cartella radice
	char cartella[MAXCHARS+1];
	 _getcwd(cartella, MAXCHARS);
	_chdir (execDirectory);
	// legge in modo statico (a file chiuso) gli attributi del file
	CFileStatus rStatus; 
	BOOL ok = CFile::GetStatus (nome_lib, rStatus);
	if (! ok)
	{
// AfxMessageBox ("Errore in applicazione - 1");
		return FALSE;
	}
// AfxMessageBox (nome_lib);
	int file_lib;
	BOOL result = FALSE;
	if ((file_lib = open(nome_lib, O_WRONLY | O_RAW)) >= 0) {
		if (write (file_lib, firma_lib, DIM_FIRMA) == DIM_FIRMA) {
			result = TRUE;
// AfxMessageBox ("Ho riscritto la firma");
		}
		close (file_lib);
		// ripristina in modo statico gli attributi del file (inclusa data)
		CFile::SetStatus (nome_lib, rStatus);
	}
	else
		AfxMessageBox ("Errore in apertura di libreria");
	// ripristina cartella corrente
	_chdir (cartella);

	return result;
}
