/* ILSTACK.CPP		G. Toffoli

991126	simulato breakpoint
980831	eliminato extern "C" per conversione a C++ dei moduli C
			rispostato lstack in ilcomune.cpp
980127	versione .CPP da versione .C
960423	disattivazioni temporanee
960410	prima vers. Windows da Iperlogo del 12.06.89
*/

#include "stdafx.h"

#include "ilcpp.h"

void vai_top();

void FatalExit (char* s);
char* MsgOverflow = "Errore di sistema (overflow dello stack)";
void BreakOnDebug();

/*
  overflow dello stack logo:
  sovrapposizione tra stack di controllo e stack dei valori
*/

void overflow ()
{
//  msg (1);
//  vai_top ();
	BreakOnDebug();
	FatalExit (MsgOverflow);
}
void underflow ()
{
//  msg (1);
//  vai_top ();
	BreakOnDebug();
	FatalExit (MsgOverflow);
}

/***************************************************
 	OPERAZIONI GENERALI SULLO STACK DI CONTROLLO
  **************************************************/
// extern "C"
void ini_stack ()
{
  p_sc = 0;
  ini_p_sv = p_sv = DIM_LSTACK-1;
}

/*--------------------------------------------
  push di un valore sullo stack di controllo
  -------------------------------------------*/
// extern "C"
void push_sc (node valore)
{
  lstack [p_sc++] = valore;
  if (p_sc == p_sv) overflow ();
}

/*------------------------------------------
  pop di un valore dallo stack di controllo
  ------------------------------------------*/
// extern "C"
node pop_sc ()
{
  if (p_sv == 0) underflow ();
  return lstack [--p_sc];
}

/*-----------------------------------------
  ritorna il valore dell' i-esimo elemento
  dalla cima dello stack di controllo
  ----------------------------------------*/
// extern "C"
node get_sc (int i)
{
  return lstack [p_sc - i];
}

/*-----------------------------------------------------
  modifica il valore dell' i-esimo elemento dalla cima
  dello stack di controlo; ritorna il vecchio valore
  ----------------------------------------------------*/
// extern "C"
node mod_sc (node valore, int i)
{
  node old;
  old = lstack [p_sc - i];
  lstack [p_sc - i] = valore;
  return old;
}

/*************************************************
 	OPERAZIONI GENERALI SULLO STACK DEI VALORI
 *************************************************/

/*----------------------------------------
  push di un valore sullo stack dei valori
  ----------------------------------------*/
// extern "C"
void push_sv (node valore)
{
  lstack [p_sv--] = valore;
  if (p_sc == p_sv) overflow ();
}

/*---------------------------------------
  pop di un valore dallo stack dei valori
  ---------------------------------------*/
// extern "C"
node pop_sv ()
{
  if (p_sv == DIM_LSTACK) underflow ();
  return lstack [++p_sv];
}

/*----------------------------------------
  ritorna il valore dell' i-esimo elemento
  dalla cima dello stack dei valori
  ----------------------------------------*/
// extern "C"
node get_sv (int i)
{
  return lstack [p_sv + i];
}

/*----------------------------------------------------
  modifica il valore dell' i-esimo elemento dalla cima
  dello stack dei valori; ritorna il vecchio valore
  ----------------------------------------------------*/
// extern "C"
node mod_sv (node valore, int i)
{
  node old;
  old = lstack [p_sv + i];
  lstack [p_sv + i] = valore;
  return old;
}

