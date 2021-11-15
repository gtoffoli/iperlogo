/* CONTESTI.H		Giovanni Toffoli

	990919	uso di #pragma pack per passaggio /Zp1 -> /Zp8 (standard)
	981224	DIM_LSTACK: 1000 -> 10000; MAX_CONTESTO: 10 -> 100
	980831	eliminato node
	980127	estratto da ILCOMUNE.C
*/

#define	DIM_LSTACK 10000	// dimensione dello stack logo in elementi di array
typedef unsigned long       DWORD;
// #define	node	DWORD
#define MAX_CONTESTO 100	// massima nidificazione di contesti

#pragma pack(push, contesti, 1)

struct	contesto {
node _id_contesto;		/* identificatore del motivo di attivazione contesto */
int	_dev_recupera,	/* device per comando recupera */
	_liv_procedura,	/* livello di nest delle procedure LOGO */
	_in_liv_proc,    /* valore di liv_procedura a inizio PAUSA/RECUPERA */
	_liv_funzione,	/* livello di profondita' delle funzioni */
	_in_liv_funzione;/* valore di liv_funzione a inizio PAUSA/RECUPERA */
node _funzione;	/* indir. funzione (primitiva o procedura) corrente */
int	_liv_esecuzione,	/* livello di discesa albero di esecuzione (blocchi) */
	_val_verifica,	/* valore corrente del predicato argomento di "se" */
	_conto_esegui;	/* contatore delle iterazioni di un blocco */
int	_RepCount,
	_RepTotal;
node _token,		/* elemento della lista di comando */
	_ini_token;    	/* token iniziale */
int	_n_arg_attesi,	/* numero argomenti attesi */
	_n_arg_trovati,	/* numero argomenti incontrati */
	_parentesi,      /* */
	_conto_parentesi,/* conto algebrico parentesi in valut. espressione */
	_p_sc,		/* pointer allo stack di controllo */
	_p_sv,		/* pointer allo stack dei valori */
	_ini_p_sv;	/* pointer iniziale allo stack dei valori in contesto*/
node _linea_com;	/* lista che rappresenta una linea di comando parsed */
};

#pragma pack(pop, contesti)

#define DIM_CONTESTO (sizeof (struct contesto))	/* dim. contesto in byte */

#define id_contesto (contesti[0]._id_contesto)
#define dev_recupera (contesti[0]._dev_recupera)
#define liv_procedura (contesti[0]._liv_procedura)
#define in_liv_proc (contesti[0]._in_liv_proc)
#define liv_funzione (contesti[0]._liv_funzione)
#define in_liv_funzione (contesti[0]._in_liv_funzione)
#define funzione (contesti[0]._funzione)
#define liv_esecuzione (contesti[0]._liv_esecuzione)
#define val_verifica (contesti[0]._val_verifica)
#define conto_esegui (contesti[0]._conto_esegui)
#define RepCount (contesti[0]._RepCount)
#define RepTotal (contesti[0]._RepTotal)
#define token (contesti[0]._token)
#define ini_token (contesti[0]._ini_token)
#define n_arg_attesi (contesti[0]._n_arg_attesi)
#define n_arg_trovati (contesti[0]._n_arg_trovati)
#define parentesi (contesti[0]._parentesi)
#define conto_parentesi (contesti[0]._conto_parentesi)
#define p_sc (contesti[0]._p_sc)
#define p_sv (contesti[0]._p_sv)
#define ini_p_sv (contesti[0]._ini_p_sv)
#define linea_com (contesti[0]._linea_com)
