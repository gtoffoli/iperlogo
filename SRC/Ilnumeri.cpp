/*ILNUMERI.C	G. Toffoli
  operazioni su oggetti REAL LOGO

000110	messa una toppa a edi_bcd: da rivedere ancora
990115	corretto LSHIFT e ASHIFT
980908	corretto BITXOR
980828	preparazione per conversione a C++
970330	reimplementato le 6 funzioni di versione 961002
970207	reintrodotto (dummy) 6 funzioni di versione 961002
960409	implementato ACASO e RIACASO
960409	prima vers. Windows da Iperlogo del 05.07.89
*/

#include "stdafx.h"

#define ILWIN
#define TEMP

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"


void ASCIFRE ()		/* usato da edo_num per formattamento output */
{
  precisione = rnum_int (arg_1)-1;/* senza -1 edo_num ha una cifra di piu' */
}

void CIFRE ()
{
  risultato = int_num (precisione + 1);
}

/*----------------------------------------------------------------------------
  converte un oggetto numerico LOGO senza decimali in short integer C
  --------------------------------------------------------------------------*/
// short int num_int (lnum)	/* EX binbcd (bcd) */
int num_int (node lnum)	/* EX binbcd (bcd) */
// node lnum;
{
// return (short int) node2real (lnum);
  return (int) node2real (lnum);
}

/*----------------------------------------------------------------------------
  converte un oggetto numerico LOGO senza decimali in long integer C
  --------------------------------------------------------------------------*/
long int num_long (node lnum)	/* EX lbinbcd (bcd) */
// node lnum;
{
  return (long int) node2real (lnum);
}

/*----------------------------------------------------------------------------
  converte un oggetto numerico LOGO in short integer C
  dopo arrotondamento a intero
  --------------------------------------------------------------------------*/
// short int rnum_int (lnum)		/* EX bin (bcd) */
int rnum_int (node lnum)		/* EX bin (bcd) */
// node lnum;
{
  return num_int (roundnum (lnum));
}

/*----------------------------------------------------------------------------
  converte un oggetto numerico LOGO in long integer C
  dopo arrotondamento a intero
  --------------------------------------------------------------------------*/
long int rnum_long (node lnum)		/* EX lbin (bcd) */
// node lnum;
{
  return num_long (roundnum (lnum));
}

/*----------------------------------------------------------------------------
  converte in oggetto numerico LOGO uno short integer C
  --------------------------------------------------------------------------*/
node int_num (int i)			/* EX makbcd/makint (int) */
// int i;
{
  return consreal ((double) i);
}

/*----------------------------------------------------------------------------
  converte in REAL LOGO un long integer C positivo
  --------------------------------------------------------------------------*/
node long_num (long l)			/* EX lmakbcd (l) */
// long int l;
{
  return consreal ((double) l);
}

/*----------------------------------------------------------------------------
  ritorna short integer C <0, =0, >0 a seconda che sia
  num1 < num2, num1 = num2, num1 > num2
  --------------------------------------------------------------------------*/
int cmpnum (node num1, node num2)		/* EX cmpbcd (num1, num2) */
// node num1, num2;
{
  double a,b;
  if ((a=node2real (num1)) < (b=node2real (num2))) return (-1);
  if (a == b) return 0;
  return 1;
}

/*----------------------------------------------------------------------------
  ritorna il segno di oggetto numerico LOGO (- -> TRUE, + -> FALSE)
  --------------------------------------------------------------------------*/
int _segno (node lnum)
// node lnum;
{
  return (node2real (lnum) < 0);
}

/*----------------------------------------------------------------------------
  converte REAL LOGO a stringa di caratteri decimali ASCII
  --------------------------------------------------------------------------*/
int edo_num (node lnum, char *s)		/* EX edobcd (numbcd, s) */
// node lnum;
// char *s;
{
  double num;
                                /* se e' 0 lo mette a mano per evitare '.' */
  if ((num = node2real (lnum)) == 0.0) {
    *(s++) = '0'; *s = '\0'; return 1; };
                                /* precisione viene messo al posto di '*' */
  return sprintf (s, "%.*G", precisione, num);
}

/*----------------------------------------------------------------------------
  cerca di interpretare stringa iniziale di caratteri come numero decim.
  codif. in ASCII per convertirlo in REAL LOGO riporta 0 se non e' un
  numero altrimenti riporta il numero di caratteri che costituiscono 
  il numero + 1
  --------------------------------------------------------------------------*/

#define isdigit(C) ((C >= '0') && (C <= '9'))

int edi_num (char *s, node *num)		/* EX edibcd (s, num) */
// char *s;
// node *num;
{
   char *ini_s = s;
   int i,n_car = 1;
   if ((*s == '+') || (*s == '-')) {	/* salta il segno iniziale */
      ++n_car;
      ++s;
   }; 
   if (*s == '.') {			/* se e' '.' e dopo ..		*/
      ++s;
      if ( isdigit (*s) ) {		/* .. e' cifra: allora		*/
	 n_car += 2;			/* aggiorna contatore e 	*/
	 ++s;				/* puntatore e			*/
	 goto dopopunto;		/* va al seguito del '.'	*/
      } else {
         goto nonnumero; 	/* se e' '.' iniziale che non e' 	*/
				/* seguito da digit allora e' un non_numero */
      };
   };
   if (! isdigit (*s)) goto nonnumero;  /* ora voglio un digit */
   /* ORA: ha saltato l'eventuale segno, non e' il caso di '.' iniziale,
	   ha trovato il primo digit del numero.			*/
   ++s;
   ++n_car;
   while (isdigit (*s)) { ++s; ++n_car; }; /* salta i numeri dopo il primo */
   if (*s == '.') {			/* se il while si e' fermato	*/
      ++s;				/* per un '.' lo salta.		*/
      ++n_car;
   };
dopopunto:	/* siamo dopo il '.' decimale il numero c'e' di sicuro */
   while (isdigit (*s)) { ++s; ++n_car; }; /* salta i numeri dopo il '.' */
   if ((*s == 'E') || (*s == 'e')) {  /* tratta eventuale esponente */
      ++s;			/* salta le 'E'				*/
      i = 1;			/* contatore di caratteri parziale = 1	*/
      if ((*s == '+') || (*s == '-')) {	/* salta il segno esponente */
         ++i;
         ++s;
      }; 
      if (! isdigit (*s)) goto finenumero; /* esp. senza digit lo ignora */
      ++s;      			/* esponente valido: agg. puntatore e*/
      n_car += i + 1;			/*  contatore 			*/
      while (isdigit (*s)) { ++s; ++n_car; }; /* e salta altri digit esp. */
   }
finenumero:
//> 000110
   if (! IS_DELIMITATORE (*s)) goto nonnumero;
//<
  *num = consreal (atof (ini_s));
   return n_car;
nonnumero:
   return 0;
}
#undef isdigit		/* DIMENTICA MACRO */

/*----------------------------------------------------------------------------
  riporta il numero arg_1 formattato con arg_2 cifre intere
  e arg_3 (facoltativo) cifre decimali
  --------------------------------------------------------------------------*/
void DECIMALE ()
{
  char s_locale [260];
  edo_num (arg_1, s_locale);
  risultato = logstr (s_locale);
  return ;
}

/*----------------------------------------------------------------------------
  riporta il numero arg_1 in formato esponenziale con arg_2 cifre di mantissa
  e arg_3 (facoltativo) cifre di caratteristica
  --------------------------------------------------------------------------*/
void SCIENTIFICA ()
{
  char s_locale [260];
  edo_num (arg_1, s_locale);
  risultato = logstr (s_locale);
  return ;
}

#ifndef TEMP

/*----------------------------------------------------------------------------
   MONNEZZA  da togliere quando verranno fatte  form  e  eform
  --------------------------------------------------------------------------*/
void monnezza ()
{
  form (1,2,3); /* chiamata fittizia non eseguita per far segnalare
			errore dal linker (per ricordare l'incompletezza) */
  eform (1,2,3); /* chiamata fittizia non eseguita per far segnalare
			errore dal linker (per ricordare l'incompletezza) */
}

#endif // TEMP

node addnum (node num1, node num2)
// node num1, num2;
{
  return consreal (node2real (num1) + node2real (num2));
}

void SOMMA ()
{
  int i;
  double d;
  d = node2real (pop_sv ());
  for (i = 1; i < n_argomenti; ++i) d = d + node2real (pop_sv ());
  risultato = consreal (d);
}

node subnum (node num1, node num2)
// node num1, num2;
{
  return consreal (node2real (num1) - node2real (num2));
}

void DIFFERENZA ()
{
  risultato = (n_argomenti == 1) ? 
			  consreal (- node2real (arg_1))
			: consreal (node2real (arg_1) - node2real (arg_2));
}

node mltnum (node num1, node num2)
// node num1, num2;
{
  return consreal (node2real (num1) * node2real (num2));
}

void PRODOTTO ()
{
  int i;
  double d;
  d = node2real (pop_sv ());
  for (i = 1; i < n_argomenti; ++i) d = d * node2real (pop_sv ());
  risultato = consreal (d);
}

node divnum (node num1, node num2)
// node num1, num2;
{
  return consreal (node2real (num1) / node2real (num2));
}

void QUOZIENTE ()
{
#ifdef ILDOS
  if (is_zero (arg_2) && (! is_zero (arg_1))) err (8);
#endif // ILDOS
#ifdef ILWIN
  if (is_zero (arg_2) && (! is_zero (arg_1))) err1 (8);
#endif // ILWIN
  else risultato = consreal (floor (node2real (arg_1) / node2real (arg_2)));
}

void RESTO ()
{
#ifdef ILDOS
  if (is_zero (arg_2) && (! is_zero (arg_1))) err (8);
#endif // ILDOS
#ifdef ILWIN
  if (is_zero (arg_2) && (! is_zero (arg_1))) err1 (8);
#endif // ILWIN
  else risultato = consreal (fmod (node2real (arg_1), node2real (arg_2)));
}

void DIVISIONE ()
{
  if (is_zero (arg_1)) risultato = arg_1;
#ifdef ILDOS
  else if (is_zero (arg_2)) err (8);
#endif // ILDOS
#ifdef ILWIN
  else if (is_zero (arg_2)) err1 (8);
#endif // ILWIN
  else risultato = consreal (node2real (arg_1) / node2real (arg_2));
}

void INTERO ()
{
  risultato = consreal (floor (node2real (arg_1)));
}

/*----------------------------------------------------------------------------
  ritorna oggetto numerico LOGO arrotondato a intero piu' vicino
  --------------------------------------------------------------------------*/
node roundnum (node lnum)		/* EX roundbcd (bcd) */
// node lnum;
{
  return consreal (floor (node2real (lnum) + 0.5));	/* giusto ? */
}

void ARRO ()
{
  risultato = consreal (floor (node2real (arg_1) + 0.5));/* giusto ? */
}

void ASSO ()
{
  risultato = consreal (fabs (node2real (arg_1)));
}

void PIGRECA ()
{
  risultato = consreal (PI);
}

void GRADI ()
{
  risultato = consreal (node2real (arg_1) * (180.0 / PI));
}

void RADIANTI ()
{
  risultato = consreal (node2real (arg_1) * (PI / 180.0));
}

void SENO ()
{
  risultato = consreal (sin (node2real (arg_1) * (PI/180.0)));
}

void COSENO ()
{
  risultato = consreal (cos (node2real (arg_1) * (PI/180.0)));
}

void TANGENTE ()
{
  risultato = consreal (tan (node2real (arg_1) * (PI/180.0)));
}

void ARCTAN ()
{
  risultato = consreal ((180.0/PI) * atan (node2real (arg_1)));
}

/*----------------------------------------------------------------------------
  ritorna TRUE se il numero e' zero
  --------------------------------------------------------------------------*/
int is_zero (node x)
// node x;
{
  return (node2real (x) == 0);
}

void ACASO ()
{
	int random = (int) ((rand() * node2real (arg_1)) / RAND_MAX);
	risultato = consreal ((double) random);
}

void RIACASO ()
{
	int seed = (n_argomenti) ? rnum_int (arg_1) : 1;
	srand(seed);
}

void POTENZA ()
{
  risultato = consreal (pow (node2real (arg_1), node2real (arg_2)));
}

/*----------------------------------------------------------------------------
  primitiva che riporta la radice quadrata dell' argomento;
  dopo aver calcolato un valore di innesco con _ln e _esp
  viene usata la formula iterativa di Erone (Hart [68] pag. 90)
  --------------------------------------------------------------------------*/
void RADICE2 ()
{
  risultato = consreal (sqrt (node2real (arg_1)));
}

/*----------------------------------------------------------------------------
  calcola la unzione esponenziale con riduzione di range e sviluppo
  in serie di potenze
  --------------------------------------------------------------------------*/
void ESP ()
{
  risultato = consreal (exp (node2real (arg_1)));
}

/*----------------------------------------------------------------------------
  primitiva che riporta il logaritmo in base E dell' argomento
  --------------------------------------------------------------------------*/
void LN ()
{
  risultato = consreal (log (node2real (arg_1)));
}

/*----------------------------------------------------------------------------
  primitiva che riporta il logaritmo in base 10 dell' argomento
  --------------------------------------------------------------------------*/
void LOG_10 ()
{
  risultato = consreal (log10 (node2real (arg_1)));
}

void MINUS ()
{
  risultato = consreal (- (node2real (arg_1)));
}

void BITAND ()
{
  int i, n;
  n = rnum_int (pop_sv ());
  for (i = 1; i < n_argomenti; ++i) n = n & rnum_int (pop_sv ());
  risultato = int_num (n);
}

void BITOR ()
{
  int i, n;
  n = rnum_int (pop_sv ());
  for (i = 1; i < n_argomenti; ++i) n = n | rnum_int (pop_sv ());
  risultato = int_num (n);
}

void BITXOR ()
{
  int i, n;
  int or = 0, and = 0;
  for (i = 0; i < n_argomenti; ++i) {
  	n = rnum_int (pop_sv ());
// 	and = and | (~ (or ^ n));
  	and = and | (or & n);		// 980908
  	or = or | n;
  }
  risultato = int_num (or ^ and);
}

void BITNOT ()
{
  risultato = int_num (~ rnum_int (arg_1));
}


/* 6 funzioni da re-implementare */

void RADSIN ()
{
  risultato = consreal (sin (node2real (arg_1)));
}

void RADCOS ()
{
  risultato = consreal (cos (node2real (arg_1)));
}

void RADTAN ()
{
  risultato = consreal (tan (node2real (arg_1)));
}

void RADARCTAN ()
{
  risultato = consreal (atan (node2real (arg_1)));
}

long int leftShift (long int x, int n)
{
	return x << n;
}

void LSHIFT ()
{
//  risultato = long_num (rnum_long (arg_1) << rnum_int (arg_2));
	unsigned long int x = rnum_long (arg_1);
	int n = rnum_int (arg_2);
	if (n > 0)
		x = x << n;
	else if (n < 0)
		x = x >> (-n);
	risultato = long_num (x);
}

void ASHIFT ()
{
//  risultato = long_num (rnum_long (arg_1) >> rnum_int (arg_2));
	long int x = rnum_long (arg_1);
	int n = rnum_int (arg_2);
	if (n > 0)
		x = x << n;
	else if (n < 0)
		x = x >> (-n);
	risultato = long_num (x);
}

void PR_SIGN ()
{
	double sign = node2real (arg_1);
	if (sign < 0) sign = -1;
	else if (sign > 0) sign = 1;
	risultato = consreal (sign);
}

/* ILNUMERI.C */
