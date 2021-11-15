/*ILLOGICA.C	G. Toffoli

	980828	preparazione per conversione a C++
	970908 - per compatibilita' VC++5.0: bool -> bool_ 
	960910 - aggiunto XOR (or esclusivo logico)
	960410 - prima vers. per Windows da vers. Iperlogo del 15.11.88
*/

#include "stdafx.h"

#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

/*---------------------------------------------------------------------------
  ritorna "VERO" se esiste l' oggetto di tipo specificato, altrimenti "FALSO"
  ---------------------------------------------------------------------------*/
node esiste (int tipo)
// int tipo;
{
  return (logval (cercav (arg_1, tipo) != NULLP));
}
 
/*--------------------------------------------------------------------------
  ritorna TRUE se l' intero che viene ritornato da confro(nta) e' uguale a 0
  --------------------------------------------------------------------------*/
int ug_atomo (node cosa_1, node cosa_2)
// node cosa_1, cosa_2;
{
  return (confro (cosa_1, cosa_2) == 0);
} 
 
/*-------------------------------------------------------------
  ritorna TRUE se i due argomenti sono uguali, altrimenti FALSE
  -------------------------------------------------------------*/
int _uguale (node cosa_1, node cosa_2)
// node cosa_1, cosa_2;
{
  int mio_val, is_finito, livello, poppa_cosa; 
  mio_val = TRUE;
  is_finito = FALSE;
  livello = 0; 
  while ((!is_finito) && mio_val) {
    poppa_cosa = FALSE;   
    if (cosa_1 == cosa_2) poppa_cosa = TRUE; 
    else {
      if ((cosa_1 != NULLP) && (cosa_2 != NULLP)) {
        if ((typ (cosa_1) == LIST) && (typ (cosa_2) == LIST)) { /* prima in */
          push_sc (cosa_1); push_sc (cosa_2);		/* ... profondita' */
          cosa_1 = car (cosa_1); cosa_2 = car (cosa_2);
          ++livello;
        } else          /* non si tratta di 2 liste */
          if ((typ (cosa_1) == LIST) || (typ (cosa_2) == LIST)) mio_val = FALSE;
          else { mio_val = ug_atomo (cosa_1, cosa_2); poppa_cosa = TRUE; }
      }
      else if ((cosa_1 /*!= NULLP*/) || (cosa_2 /*!= NULLP*/)) mio_val = FALSE;
    };
    if (poppa_cosa)
      if (livello == 0) is_finito = TRUE;
      else {
        --livello; 
        cosa_2 = cdr (pop_sc ()); cosa_1 = cdr (pop_sc ());
      };
  }  
  while (livello != 0) {pop_sc (); pop_sc (); --livello;};
  return mio_val;
}
 
/*---
  not
  ---*/
void NON ()
{
  risultato = logval (! bool_ (arg_1));
}
 
/*---
  and
  ---*/
void AMB ()
{
  int locale, i;
  locale = TRUE;
  for (i = 1; i <= n_argomenti; ++i) locale = bool_ (pop_sv ()) && locale;
  risultato = logval (locale);
}

/*------------
  or inclusivo
  ------------*/
void OPPURE ()
{
  int locale, i;
  locale = FALSE;
  for (i = 1; i <= n_argomenti; ++i) locale = bool_ (pop_sv ()) || locale;
  risultato = logval (locale);
}
/*------------------
  or esclusivo (XOR)
  ------------------*/
void XOR ()
{
  int locale, i;
  locale = 0;
  for (i = 1; i <= n_argomenti; ++i)
	if (bool_ (pop_sv ())) ++locale;
  risultato = logval (locale == 1);
}


/*------- 
  numero?
  -------*/
void NUMERO ()
{
  node bcd;
  char s_locale [128];
  if (tipoalfa (arg_1)) {
    pacstr (s_locale, nome (arg_1));
    risultato = logval (edi_num (s_locale, &bcd) > 0);
  }
  else risultato = logval (tipo_1 == LBCD);
}

/*----------
  primitiva?
  ----------*/
void PRIMITIVA ()
{
  risultato = esiste (SFUN);
}

/*---------------------------------
  def? : e' definita la procedura ?
  ---------------------------------*/
void DEF ()
{
  risultato = esiste (UFUN);
}

/*-----------------------------
  cosa? : esiste la variabile ?
  -----------------------------*/
void COSA_ ()
{
/*<31.08.87
  risultato = esiste (LVAR);
*/
  node oggetto;
  risultato = logval (   ((oggetto = cercav (arg_1, LVAR)) != NULLP)
                      && (car (oggetto) != NULLVALUE));
/*>*/
}
 
/*-------
  uguale?
  -------*/
void UGUALE ()
{
  risultato = logval (_uguale (arg_1, arg_2));
}

/*-------
  minore?
  -------*/
void MIN ()
{
  if (tipo_1 == LIST) err_arg1 ();
  else if (tipo_2 == LIST) err_arg2 ();
  else risultato = logval (confro (arg_1, arg_2) < 0);
}

/*---------
  maggiore?
  ---------*/
void MAG ()
{
  if (tipo_1 == LIST) err_arg1 ();
  else if (tipo_2 == LIST) err_arg2 ();
  else risultato = logval (confro (arg_1, arg_2) > 0);
}

/* ILLOGICA.C */
