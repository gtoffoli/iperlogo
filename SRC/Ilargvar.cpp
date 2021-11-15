/*ILARGVAR.C	G. Toffoli

modulo contenente le funzioni in C che hanno un numero di argomenti
variabile.  Non viene utilizzato il file  ILARGVAR.I  generato
dal compilatore ma il file  ILARGVA2.I  cha va scritto a mano.

	980831	ridefinito msg
	980828	preparazione per conversione a C++
	890621	agg. NODI2LISTA
	890612	preso  MSGSENZA  dal modulo  ILFLUSSI.C
	890224	preso  MSG  dal modulo  ILFLUSSI.C 

*/

#include "stdafx.h"

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#include <stdarg.h>

/* ------------------------------------------------------------------------
   Crea una lista con gli n nodi in argomento.
   ------------------------------------------------------------------------ */
node nodi2lista (int n, node nodo)
// int n;                      /* numero dei nodi passati */
// node nodo;                  /* primo dei nodi da mettere nella lista */
{
  node *elenodi,            /* puntatore per accedere ai parametri */
       temp;                /* risultato temporaneo */

  temp = NULLP;
  elenodi = &nodo + (n - 1);      /* punta all'ultimo dei nodi nella lista */
  while (n-- > 0)
    temp = cons (*(elenodi--), temp, LIST);
  return temp;
}


/* ------------------------------------------------------------------------
  nucleo di msg
   ------------------------------------------------------------------------ */
void msgsenza (int codice, node o1, node o2)
{
  char c;
  int i, j;
  i = tab_stringhe [codice - 1];
  while ((c = s_stringhe [i]) != NULLC) {
    if (c == '$') {
      _mostra ((s_stringhe [++i] == '1') ? o1 : o2);
      ++i;
    }
    else {
      j = i;
      while (((c = s_stringhe [j]) != NULLC) && (c != '$')) ++j;
      s_stringhe [j] = NULLC;
      _prints (&s_stringhe [i], j-i);
      s_stringhe [j] = c;
      i = j;
    };
  };
}

#ifdef VECCHIO

/*----------------------------------------------------------------------
  stampa messaggio generico estraendolo dalla super_stringa dei messaggi
  tramite tabella contenente l' indice dei messaggi stessi e sostituendo
  gli eventuali parametri $1, $2, ... con gli argomenti o1, o2, ...
  ----------------------------------------------------------------------*/
void msg (int codice, node o1, node o2)
// int codice;
// node o1,o2;
{
	dev_out = CONSOLE;
	msgsenza (codice, o1, o2);
	a_capo ();
	dev_out = dev_scrivi;
}

#else // VECCHIO

/* ------------------------------------------------------------------------
  msg nuovo (980831)
   ------------------------------------------------------------------------ */
void msg (int codice, ...)
{
	dev_out = CONSOLE;

#define MAX_VAR 2
	node o[MAX_VAR+1];
	int n_args = 0;
	int i_arg;
	char c;
	int i, j;
	i = tab_stringhe [codice - 1];

	va_list v1;
	va_start (v1, codice);

	while ((c = s_stringhe [i]) != NULLC) {
		if (c == '$') {
//			_mostra ((s_stringhe [++i] == '1') ? o1 : o2);
			i_arg = s_stringhe [++i] - '0';
			while (n_args++ < i_arg)  {
				o[n_args] = va_arg (v1, node);
			}
			_mostra (o[i_arg]);
			++i;
		}
		else {
			j = i;
			while (((c = s_stringhe [j]) != NULLC) && (c != '$')) ++j;
			s_stringhe [j] = NULLC;
			_prints (&s_stringhe [i], j-i);
			s_stringhe [j] = c;
			i = j;
		};
	};

	va_end (v1);

	a_capo ();
	dev_out = dev_scrivi;
}

#endif // VECCHIO

/* ILARCVAR.C */

