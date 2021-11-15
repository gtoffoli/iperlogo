/* ILUTIL.C		G. Toffoli

010213	aggiunto confro2 (generalizzazione di confro)
000206	mod. nodeinarray (usato in set_intersection e set_difference)
991126	simulato breakpoint
991021	aggiunto LogoToText
980828	preparazione per conversione a C++
980123	disattivato funzioni obsolete	
970908	per compatibilita' VC++5.0: bool -> bool_ 
970503	recupero update del 02.10.96 (normalizzazione)
960517	spostato qui ERR provvisoriamente
960514	definito MessBox in modulo CPP
960410	prima vers. Windows da Iperlogo del 29.05.89
*/

#include "stdafx.h"

#define ILWIN 
#define TEMP
#define NORMALIZE
 
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

extern void MemScrivi (char *, node);

void BreakOnDebug ()
{
#ifdef _DEBUG
	int uno = 1;
	int infinito = uno/0;
#endif // _DEBUG
}

void LogoToText (node logo, char* pText)
{
	if (typ (logo) == LIST) MemScrivi (pText, logo);
	else getnam (pText, logo);
}

#ifdef ILWIN

void err1 (int codice)
{
	errore (codice, funzione, NULLP);
}
 
void err2 (int codice, node arg)
{
	errore (codice, funzione, arg);
} 

#endif // ILWIN 

#ifdef NORMALIZE
int MatchKw (node, node);
#endif // NORMALIZE

#ifdef TEMP

int MessBox (char* s);
void ExitNoConfirm ();

void FatalExit (char* s)
{
	if (s == NULL)
		s = "Errore fatale di sistema";
	MessBox(s);
	ExitNoConfirm ();
}

void printf_null (char *control)
{
	char s[256]; int n;
	n = sprintf (s, control);
	MessBox(s);
}
void printf_long (char *control, long val)
{
	char s[256]; int n;
	n = sprintf (s, control, val);
	MessBox(s);
}

void printf_int (char *control, int val)
{
	printf_long (control, (long) val);
}

void err (int codice, node object)
{
	printf_int ("errore numero %d", codice);
}

int stcd_i (char *s, int *val)
{
	return 0;
}

#endif // TEMP

/*-----------------------------------------------------------
  pone in un array di nodi gli elementi di una lista; riporta
  il numero di elementi se OK, 0 altrimenti
  -----------------------------------------------------------*/
int list2array (node lista, node *node_array, int maxn)
{
  int n = 0;
  while (lista != NULLP) {
    if (n++ == maxn) return 0;
    * (node_array++) = car (lista);
    lista = cdr (lista);   
  };
  return n;
}

/*------------------------------------------------------------------------
  riporta TRUE se un nodo e' membro di una lista rappr. come array di nodi
  ------------------------------------------------------------------------*/
int nodeinarray (node nodo, node *node_array, int n)
{
  int i = 0;
  while (i++ != n)
//> 000206
//	  if (nodo == * (node_array++))
	  if (_uguale (nodo, * (node_array++)))
//<
		  return TRUE;
  return FALSE;
}

/*-----------------------------------------------------
   Stampa brutalmente un messaggio di errore con la
   memoria.
  -----------------------------------------------------*/
void print_err (int n)
{
  printf_int ("Memoria base insufficiente (%d). ",n);
}

/*-----------------------------------------------------
   riporta il valore numerico assegnato in environment DOS
   alla stringa S, oppure -1
  -----------------------------------------------------*/
int getenv_i (char *s)
{
  int val; char *p;
  if ((p = getenv (s)) == NULL) val = -1; else stcd_i (p, &val);
  return val;
}

/*-----------------------------------------------------
  confronta 2 oggetti dello stesso tipo, liste escluse,
  e ritorna un intero che ne rispecchia la relazione
  -----------------------------------------------------*/
int confro (node term1, node term2)
{
  return ((typ (term1) == LBCD) && (typ (term2) == LBCD)) ?
    cmpnum (term1, term2) : match (nome (term1), nome (term2));
}


/*-----------------------------------------------------
  confronta 2 oggetti di tipo qualsiasi: 
  - lista vuota < qualsiasi oggetto
  - oggetto atomico < lista non vuota
  - in caso di liste non vuote, si fa il confronto tra i primi elementi
  -----------------------------------------------------*/
int confro2 (node term1, node term2)
{
	if (term1 == term2)			// gli oggetti coincidono
		return 0;
	else if (term1 == NULLP)	// lista 1 vuota
		return -1;
	else if (term2 == NULLP)	// lista 2 vuota
		return 1;
	else if ((typ (term1) == LBCD) && (typ (term2) == LBCD))	// numeri
		return cmpnum (term1, term2);
	else if (tipoalfa (term1) && tipoalfa (term2))				// parole e stringhe
		return match (nome (term1), nome (term2));
	else if ((typ (term1) == LIST) && (typ (term2) == LIST))	// liste non vuote
		return confro2(car (term1), car (term2));
	else return 0;	// ?
}

/*-------------------------------------------------
  ritorna TRUE per oggetto "VERO o "FALSO
  -------------------------------------------------*/
int boolp (node arg)
{
#ifndef NORMALIZE
  return ((arg == pc_VERO) || (arg == pc_FALSO));
#else // NORMALIZE
  return (MatchKw (arg, pc_VERO) || MatchKw (arg, pc_FALSO));
#endif // NORMALIZE
}
 
/*-------------------------------------------------
  converte parola "VERO / "FALSO in valore booleano
  -------------------------------------------------*/
int bool_ (node arg)
{
#ifndef NORMALIZE
  return (arg == pc_VERO);
#else // NORMALIZE
  return MatchKw (arg, pc_VERO);
#endif // NORMALIZE
}
 
/*-------------------------------------------------
  converte valore booleano in parola "VERO / "FALSO
  -------------------------------------------------*/
node logval (int arg)
{
  return (arg) ? pc_VERO : pc_FALSO;
}
 
/*---------------------------------------------
  ritorna elemento corrente di "lista_aperta"
  e avanza "lista_aperta" a successivo elemento
  ---------------------------------------------*/
node leggi_ele ()
{
  node ele;
  ele = NULLP;
  if (lista_aperta != NULLP) {
    ele = car (lista_aperta);
    lista_aperta = cdr (lista_aperta);
  };
  return ele;
}

/*------------------------------------------------------------------------
  riporta TRUE se alfa e' un oggetto di tipo LBCD, oppure di tipo LSTR o LWRD
  ma convertibile in LBCD; riporta per indirizzo l' eventuale oggetto LBCD
  ------------------------------------------------------------------------*/
int conv_num_node (node oggetto, node *num)
{
  char	s_locale [128];
  if (typ (oggetto) == LBCD) {
    *num = oggetto;
    return TRUE;
  }
  else if (tipoalfa (oggetto)) {
    pacstr (s_locale, nome (oggetto));
    return (edi_num (s_locale, num) > 0);
  }
  else return FALSE;
}
 
/*------------------------------------------------------------------------
  riporta TRUE se alfa e' un oggetto di tipo LBCD, oppure di tipo LSTR o LWRD
  ma convertibile in LBCD; riporta per indirizzo il valore dell'
  eventuale oggetto LBCD
versione BIS sicuramente usata in ILTARTA
  ------------------------------------------------------------------------*/
int conv_num_double (node oggetto, double *num)
{
  char	s_locale [128];
/*   int temp; */
  node tempnod;
  if (typ (oggetto) == LBCD) {		/* caso 1 */
    *num = node2real (oggetto);
    return TRUE;
  };
  if (tipoalfa (oggetto)) {		/* caso 2 */
    pacstr (s_locale, nome (oggetto));
/*     temp = edi_num (s_locale, &tempnod); */
    if (edi_num (s_locale, &tempnod) > 0) {
      *num = node2real (tempnod);
      return TRUE;
    };
  };
  return FALSE;				/* ultimo caso */
}

#ifdef IL89		// condizione aggiunta 980123

/* ------------------------------------------------------------------------ */
/* riporta il componente offset di un long pointer */
unsigned os_off (p)
char *p;
{
  union p_os temp;
  temp.p = p;
  return temp.os.off;
}

/* riporta il componente segment di un long pointer */
unsigned os_seg (p)
char *p;
{
  union p_os temp;
  temp.p = p;
  return temp.os.seg;
}

#endif // IL89

/* replica a destinazione bit(s) di origine selezionato(i) tramite maschera */
void copy_bits (char *from, char *to, char mask)
{
  *to = (*to & (0xFF ^ mask)) | (*from & mask);
}

/* ILUTIL.C */
