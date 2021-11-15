/* ILMONDO.C	G. Toffoli

	990727	PR_GENERA al posto di CDOS
	980828	preparazione per conversione a C++
	960410	disattivato codice per mouse e touch-screen
	960409	prima versione per Windows
  
*/

#include "stdafx.h"

#define ILWIN
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#ifdef ILDOS
#include "gfx.h"
#endif // ILDOS

#ifdef ILWIN
#include "windows.h"
#include <time.h>                   
#include <process.h>
#endif // ILWIN

DWORD dwTime = 0L;

void TicksWait (DWORD dwDuration)
{
	clock_t NumTicksToWait = dwDuration + clock();
	while (NumTicksToWait > clock());
}

/* ----------------------------------------------------------------------- */
void ASTEMPO ()
{
#ifdef ILDOS
  _tempo = rnum_int (arg_1);
#endif // ILDOS
#ifdef ILWIN
	dwTime = clock() - num_long (arg_1); 
#endif // ILWIN
}

/* ----------------------------------------------------------------------- */
void TEMPO ()
{
#ifdef ILDOS
  risultato = int_num (_tempo);
#endif // ILDOS
#ifdef ILWIN
  risultato = long_num (clock() - dwTime);
#endif // ILWIN
}

/* ----------------------------------------------------------------------- */
void ASPETTA ()
{
#ifdef ILDOS
  int durata, scadenza;
  scadenza = _tempo + (durata = rnum_int (arg_1));
  if (durata > 1)
    while ((scadenza != _tempo) && (! is_break)) ;
#endif // ILDOS
#ifdef ILWIN
  DWORD dwDuration = num_long (arg_1);
  TicksWait (dwDuration);
#endif // ILWIN
}

void MemScrivi (char *s, node oggetto);
void PR_PROCESSOCREA ()
{
#define MAX_ARG 16
	char cmdName[MAXCHARS];
	char argText[MAX_ARG][MAXCHARS];
	char* argp;
	char* argv[MAX_ARG];
	node argument;
	int i;
	for (i = n_argomenti-1; i >=1; i--) {
		argument = (pop_sv ());
		argp = &(argText[i][0]);
		if (typ (argument) == LIST) MemScrivi (argp, argument);
		else getnam (argp, argument);
		argv[i] = argp;
	}
	node cmd = pop_sv ();
	if (typ (cmd) == LIST) MemScrivi (cmdName, cmd);
	else getnam (cmdName, cmd);
	argv[0] = &cmdName[0];
	argv[n_argomenti] = NULL;
	int mode = _P_NOWAIT;
	int result =
		_spawnv (mode, cmdName, argv);
	risultato = int_num (result);
}

#ifdef ILDOS

/* ----------------------------------------------------------------------- */
void SUONA ()
{
  node lista;
  no_int ();
  if (tipo_1 == LIST) {
    if (lstlen (lista = arg_1) & 1) {err_arg1 (); return;};
  }
  else lista = cons (arg_1, cons (arg_2, NULLP, LIST), LIST);
  l_musica = lstcat (l_musica, lista);
  is_musica = TRUE;
  si_int ();
/*<25.04.89 : toppa per interferenza con NODI_RAM_ESTESA */
  if (nodi_ram_estesa) while (is_musica) {};
/*>*/
}

/* ----------------------------------------------------------------------- */
void ZITTO ()
{
  l_musica = NULLP;
#ifdef IL89
  stopmus ();
#endif // IL89
}

/*--------------------------------
  emette un breve segnale acustico
  --------------------------------*/
void _beep ()
{
  if (l_musica == NULLP) {
    arg_1 = consreal (1000.0);
    arg_2 = consreal (4.0);
    SUONA ();
  }
}

/*---------------------------------------------------------------------
  ritorna in un long integer una coppia di interi binari rappresentanti
  frequenza e durata della nota da suonare; durata = 0 significa che la
  lista di note e' finita
  ---------------------------------------------------------------------*/
long _nota ()
{
  int frequenza, durata;
  long valore;
  durata = 0;
  while (l_musica != NULLP) {
    frequenza = rnum_int (car (l_musica));
    durata = rnum_int (car (l_musica = cdr (l_musica)));
    l_musica = cdr (l_musica);
  if (durata > 1) break;
    durata = 0;
  };
  return ((valore = frequenza) << 16) + durata;
}

/* ----------------------------------------------------------------------- */
void STATO ()
{
  int i;
  if (typ (arg_1) == LBCD) risultato = logval (_stato (rnum_int (arg_1)));
  else if (((i = nome2dev (arg_1)) == COM1) || (i == COM2))
    risultato = int_num (comstat (i - COM1));
  else err_arg1 ();
}

/* ----------------------------------------------------------------------- */
void MISURA ()
{
  risultato = int_num (_misura (rnum_int (arg_1)));
}

#endif // ILDOS


#ifdef IL89

/*<14.03.89 *************************************************

   LA VERSIONE PER TOUCH SCREEN DELLA HP (VECTRA ES e SIMILI)
   E' IN FONDO AL FILE

*************************************************************/

/* --------------------------------------------------------------------------

    VERSIONE TOUCH SCREEN Philips "Professional TS VP 120" SU RS232  

    Il touch screen deve essere in BINARY TRANSMISSION  e  LINEAR MODE
    (coordinate in binario tra (0,0) e (110,90);
    N.B. in linear mode l'origine e' in BASSO a DESTRA )

-------------------------------------------------------------------------- */

/*---------------------------------------------------------------------------
  rende attivo il Touch screen su RS232 - COM2
  programma il touch screen con la lista o la stringa arg_1;
  pulisce il (flag del) buffer del touch screen;
  -------------------------------------------------------------------------*/
void ASTOUCH ()
{
  prog_touch = (typ (arg_1) == LIST) ? arg_1 : nome (arg_1);
  buff_touch = FALSE;			/* vuota il (flag del) buffer */
/*<09.06.89
  no_int ();
  ij_rx [1] = 0;			/* azzera il buffer di COM2 */
  si_int ();
  is_touch_attivo = TRUE;               /* segnala che il touch e' attivato */
*/
  if (! is_touch_attivo) {
    is_touch_attivo = TRUE;             /* segnala che il touch e' attivato */
                                /* la prima volta controlla se c'e' il mouse */
    inregs.x.ax = 0;			/* funzione: init mouse */
    int86 (INT_MOUSE, &inregs, &outregs);
    if (   (outregs.x.ax != -1)
        || (getenv ("MOUSE") == NULL)) {/* mouse non presente o non voluto */
/*<03.07.89
      no_int ();                        /* usa COM2 per il touch screen */
      ij_rx [1] = 0;			/* azzera il buffer di COM2 */
      si_int ();
>*/
    } else {				/* mouse presente */
      is_mouse_attivo = TRUE;		/* segnala mouse presente */
      inregs.x.ax = 4;			/* funzione: put mouse at x,y */
      inregs.x.cx = 20;			/* x = 20 */
      inregs.x.dx = 20;			/* y = 20 */
      int86 (INT_MOUSE, &inregs, &outregs);
    };
  };
/*<03.07.89*/
  if (! is_mouse_attivo) {
    no_int ();                    /* sta usando COM2 per il touch screen */
    ij_rx [1] = 0;                /* azzera il buffer di COM2 */
    si_int ();
  };
/* > */
}

/*---------------------------------------------------------------------------
  riporta il valore con cui e' programmato il touch screen 
  -------------------------------------------------------------------------*/
void TOUCH ()
{
  risultato = prog_touch;
}

/*---------------------------------------------------------------------------
  riporta VERO o FALSO a seconda che ci sia una "toccata" nel buffer
  del touch screen 
  -------------------------------------------------------------------------*/
void TOUCH_ ()				/* primitiva TOUCH? */
{
                                  /* vede se e' presente una toccata e */
                                  /* aggiorna buff_touch */
/*<06.07.89
  (is_mouse_attivo) ? leggi_mouse () : leggi_touch ();
*/
  if (! is_mouse_attivo) leggi_touch ();
/* > */
  risultato = logval (buff_touch);
}

/*---------------------------------------------------------------------------
  Smista al mouse o al touch la lettura 
  -------------------------------------------------------------------------*/
/*<03.07.89
int leggi_touch_mouse ()
{
  return (is_mouse_attivo) ? leggi_mouse () : leggi_touch ();
}
> */

/*---------------------------------------------------------------------------
  Legge il MOUSE e riporta TRUE o FALSE a seconda che un bottone sia stato
  premuto o meno (e quindi ci sia una "toccata" nel buffer)
  Aggiorna i valori di riga_touch, colonna_touch e buff_touch.
  -------------------------------------------------------------------------*/
int leggi_mouse ()
{
  inregs.x.ax = 3;			/* funzione: get mouse position */
  int86 (INT_MOUSE, &inregs, &outregs);
/*
  outregs.x.bx;			/* stato dei bottoni */
  outregs.x.cx;			/* coordinata x */
  outregs.x.dx;			/* coordinata y */
*/
  if (outregs.x.bx != 0) {		/* se c'e' un bottone premuto */
    colonna_touch = outregs.x.cx;	/* memorizza le coordinate attuali */
    riga_touch = outregs.x.dx;
    buff_touch = TRUE;			/* segnala toccata in buffer */
    return TRUE;			/* e riporta TRUE */
  }
  return FALSE;				/* nessun bottone premuto */
}

/*---------------------------------------------------------------------------
  riporta TRUE o FALSE a seconda che ci sia una "toccata" significativa
  nel buffer del touch screen.
  Aggiorna i valori di riga_touch, colonna_touch e buff_touch.
  -------------------------------------------------------------------------*/
int leggi_touch ()
{
#define MIN_Y 10	/* minimo Y accettabile */
#define MAX_Y 80	/* massimo Y accettabile */
#define RANGE_Y 70.0	/* MAX_Y - MIN_X in formato float */

  int c, esito = FALSE;
  int x, y, cc;			/* x e y letti e close code */
  int xx, yy;			/* x e y piu' recenti e buoni */

  if (! is_touch_attivo) return FALSE;		/* se touch inattivo esce */

ritenta:  /* INIZIO DEL CICLO per buttare dati vecchi e mondezza */
				/* legge fino ad 7F o 7E */
  while (   ((c = comgetc_nowait (1)) != -1)	/* -1 e' il buffer vuoto */
         && (c != 0x7F)
         && (c != 0x7E) ) {};
  if (c != -1) {
                                /* ha trovato il carattere iniziale */
    x = comgetc_nowait (1);	/* legge e controlla i dati */
    y = comgetc_nowait (1);
    cc = comgetc_nowait (1);
    if (  (x < 0) || (x > 110) || (y < 0) || (y > 90) || (cc != 0x71) )
      goto ritenta;

    esito = TRUE;		/* i dati sono corretti */
    xx = x; yy = y;		/* li conserva e controlla se c'e' */
    goto ritenta;		/* una toccata piu' recente */
  };

  if (esito == TRUE) {
/*<09.06.89
			/* in xx e yy ci sono le piu' recenti toccate 
			significative.  Le traduce in righe (0-24) e
			colonne (0-79) rigirandole perche' l'origine
			del TS e' in basso a destra */
  if (yy < MIN_Y) yy = MIN_Y;		/* riporta y in area significativa */
  if (yy > MAX_Y) yy = MAX_Y;
  colonna_touch = 79 - (xx / (110.0 / 79.0));	/* mette i valori nelle var.*/
  riga_touch = 24 - ((yy - MIN_Y) / (RANGE_Y /24.0));	/* globali */
  buff_touch = TRUE;				/* segnala toccata in buffer*/
*/
			/* in xx e yy ci sono le piu' recenti toccate 
			significative.  Le traduce in pixel reali 
			rigirandole perche' l'origine
			del TS e' in basso a destra */
					/* riporta y in area significativa */
  yy = (yy < MIN_Y) ? MIN_Y : (yy > MAX_Y) ? MAX_Y : yy;
				/* mette i valori nelle var. globali */
  colonna_touch = colonna2x ((int) (79 - (xx / (110.0 / 79.0))));
  riga_touch = riga2y ((int) (24 - ((yy - MIN_Y) / (RANGE_Y /24.0))));
  buff_touch = TRUE;				/* segnala toccata in buffer*/
/* > */
  };

  return esito;

#undef MIN_Y		/* cancella macro locali */
#undef MAX_Y
#undef RANGE_Y
}

/*---------------------------------------------------------------------------
  riporta una lista di tre elementi:
  il nome della finestra piu' superficiale toccata e le coordinate di dove 
  e' stato toccato il video (lavora in righe e colonne  25x80)
  -------------------------------------------------------------------------*/
void DITO ()
{
  int nb, nv, x, y;
  node lnome;
  struct dview *pv;
                                                    /* legge ultima toccata */
/*<06.07.89
  (is_mouse_attivo) ? leggi_mouse () : leggi_touch ();
*/
  if (! is_mouse_attivo) leggi_touch ();
/* > */
/*<09.06.89
  x = (colonna_touch * (VIDEO_X_F / 80.0));	/* coord. in pixel virtuali */
  y = (riga_touch * (VIDEO_Y_F / 25.0));	/* (le stesse di open_view) */
  x = scalex (x);				/* coord. in pixel FISICI */
  y = scaley (y);				/* (quelle della struct GFX) */
*/
  x = colonna_touch;			/* coord. in pixel fisici */
  y = riga_touch;			/* quelle della struct del GFX */
/* > */
  for (nb = 0; nb < N_BOTT; ++nb)	/* controlla i bottoni */
    if (dentro_bot (x, y, bott [nb])) break; 
  if (nb < N_BOTT) lnome = nbott2nome (nb);
  else {
    no_int (); semaforo = ROSSO; si_int ();	/* occupa regione critica */
    nv = nview;
    while (nv != -1) {	/* controlla tutte le finestre andando all'indietro */
      pv = view [nv];
      if (dentro_fin (x, y, pv)) break;
      nv = pv->prima;
    };
    lnome = nview2nome (nv);
    no_int (); semaforo = VERDE; si_int ();	/* libera regione critica */
  };
  risultato = cons (lnome,
                    cons (int_num (colonna_touch),
		          cons (int_num (riga_touch), NULLP, LIST),
		          LIST),
		    LIST);
}

/*14.03.89>*/

/*---------------------------------------------------------------------------
  Dato il numero di un bottone (indice in bott) riporta il nome
  logo del bottone
  -------------------------------------------------------------------------*/
node nbott2nome (nb)
int nb;
{
  int i;
  node nome_bot;
  for (i = MIN_DEV;i <= MAX_DEV; ++i)	/* controlla fin. utente */
    if (   (_file [i] == nb)
/*<07.07.89
        && (_fstato [i] & (O_BOTTONE | O_FINESTRA)) )
*/
        && (_fstato [i] & O_BOTTONE ) )
/* > */
    {
      nome_bot = _nome [i]; 
      break;
    };
  return nome_bot;
}

/*---------------------------------------------------------------------------
  Controlla se il punto (x,y) e' dentro il bottone pb (puntatore 
  a descrittore).
  Si suppone che (x,y) sia congruente con i valori usati dal GFX
  -------------------------------------------------------------------------*/
int dentro_bot (x, y, pb)
int x, y;
struct dbott *pb;
{
  return (pb == NULL) ? FALSE
                      :(   (pb->min_x <= x)
                        && (x <= pb->max_x)
                        && (pb->min_y <= y)
                        && (y <= pb->max_y)
                       );
}

/*---------------------------------------------------------------------------
  Dato il numero di una finestra (indice in view) riporta il nome
  logo della finestra (se l'argomento e' -1 riporta pc_VUOTA)
  -------------------------------------------------------------------------*/
node nview2nome (nv)
int nv;
{
  int i;
  node nome_fin;
  if (nv == -1) nome_fin = pc_VUOTA;		/* nessuna finestra toccata */
  else if (nv == Q_COMANDI) nome_fin = pc_COMANDI; /* controlla le finestre */
  else if (nv == Q_FOGLIO) nome_fin = pc_FOGLIO;   /* sempre presenti */
  else if (nv == Q_COMMENTO) nome_fin = pc_STATO;
  else if (nv == Q_TARTA) nome_fin = pc_TARTA;
  else for (i = MIN_DEV;i <= MAX_DEV; ++i)	/* controlla fin. utente */
         if (   (_file [i] == nv)
             && (_fstato [i] & O_FINESTRA)) {
            nome_fin = _nome [i]; 
            break;
          };
  return nome_fin;
}

/*---------------------------------------------------------------------------
  Controlla se il punto (x,y) e' dentro la finestra pv (puntatore 
  a descrittore).
  Si suppone che (x,y) sia congruente con i valori del descrittore GFX
  -------------------------------------------------------------------------*/
int dentro_fin (x, y, pv)
int x, y;
struct dview *pv;
{
  return (   (pv->min_x <= x)
          && (x <= pv->max_x)
          && (pv->min_y <= y)
          && (y <= pv->max_y)
         );
}

/*---------------------------------------------------------------------------
---------------------------------------------------------------------------*/
void hide_mouse_cur ()
{
/*<16.06.89*/
  if (is_mouse_acceso) {
    is_mouse_acceso = FALSE;
/*>*/
/*<06.07.89*/
    semaforo = VERDE;                   /* o.k. al blink e altro */
/* > */
    inregs.x.ax = 2;                    /* funzione: hide cursor */
    int86 (INT_MOUSE, &inregs, &outregs);
  };
}

/*---------------------------------------------------------------------------
---------------------------------------------------------------------------*/
void show_mouse_cur ()
{
/*<16.06.89*/
  if (is_mouse_attivo && (! is_mouse_acceso)) {
    is_mouse_acceso = TRUE;
/*>*/
/*<06.07.89*/
    semaforo = ROSSO;                   /* niente blink (e altro?) */
/* > */
    inregs.x.ax = 1;			/* funzione: show cursor */
    int86 (INT_MOUSE, &inregs, &outregs);
  };
}

/* ---------------------------------------------------------------------------
   Routines di scalatura di valori per gli assi  X  e  Y  dati in
   RIGHE e COLONNE. (righe = 0..24; colonne = 0..79)
   Origine nell'angolo sup. sinistro dello schermo sia per righe e colonne
   che per X e Y.
   Le routine di scala si occupano di adattare i valori 
   all'hardware presente.
   Serve solo per leggi_touch (credo).

   Esistono 6 diverse modalita' che sono previste in IperLOGO, qual'e'
   quella attuale dipende dal "tipo_video" richiesto e dalla scheda
   che e' presente nel PC.  Indicando le 6 modalita' con A..F lo schema con
   cui viene stabilita la modalita' e' la seguente:
   (tv = tipo_video definita in ILVIEW.C; EGA_200 e' def. in iperlogo.h)

----------\  tv |
tipo scheda \   |   1   |   2   |   3   |EGA_200|   4   |       |
-  -  -  -  --\ |-------------------------------------------------
                |       |       |       |       |       |       |
scheda Hercules |   A   |   A   |   A   |   A   |   A   |       |
------------------------------------------------------------------
                |       |       |       |       |       |       |
scheda  CGA     |   B   |   C   |   B   |   B   |   B   |       |
------------------------------------------------------------------
                |       |       |       |       |       |       |
scheda  EGA     |   B   |   C   |   D   |   E   |   D   |       |
------------------------------------------------------------------
                |       |       |       |       |       |       |
scheda  VGA     |   B   |   C   |   D   |   E   |   F   |       |
------------------------------------------------------------------

   La modalita' A e' quella specifica della scheda Hercules. (720x348)
   La mod. B e' quella di SLOGO grafica (4) con 4 colori in 320x200
   La mod. C e' quella CGA monocromatica in 640x200 (80 colonne+grafica)
   La mod. D e' la massima per la EGA e ha 16 colori in 640x350 (43 righe)
   La mod. E e' come la C ma in 16 colori (su EGA o VGA)
   La mod. F e' il modo 16 colori della VGA in 640x480
		   (il GFX non gestisce la VGA in 256 colori)

----------------------------
   Per l'asse X  si hanno solo 3 possibilita': A, B oppure (C-D-E-F) che
   sono tutte con l'asse X di 640
--------------------------------------------------------------------------- */

int colonna2x (x)
int x;
{
/*<03.07.89
  modificati tutti i coefficienti costanti
> */
  int i;
  double temp;
  temp = x / 79.0;			/* temp e' tra 0 e 1 compresi */
  if (_gfx.card_monitor & HERC_CARD)		/* MOD. A */
    i = (int) (temp * /* 720.0 - 1.0 */ 719.0);
  else if (   (tipo_video == 1)			/* MOD. B */
           || ((_gfx.card_monitor & CGA_CARD) && (tipo_video != 2)) )
    i = (int) (temp * /* 320.0 - 1.0 */ 319.0);
  else						/* MOD. C D E F */
    i = (int) (temp * /* 640.0 - 1.0 */ 639.0);

  return i;
}

/* ---------------------------------------------------------------------------
   vedi commento sopra  (colonna2x)
----------------------------
   Per l'asse Y si hanno 4 possibilita': A, (B-C-E) che hanno l'asse Y
   di 200, D con l'asse Y di 350 oppure F con l'asse Y di 480 
--------------------------------------------------------------------------- */
int riga2y (y)
int y;
{
/*<03.07.89
  modificati tutti i coefficienti costanti
> */
  int i;
  double temp;
  temp = y / 24.0;			/* temp e' tra 0 e 1 compresi */
  if (_gfx.card_monitor & HERC_CARD)		/* MOD. A */
    i = (int) (temp * /* 348.0 - 1.0 */ 347.0);
  else if ((_gfx.card_monitor & CGA_CARD) || (tipo_video == 1)/* MOD. B C E */
					  || (tipo_video == 2)
					  || (tipo_video == EGA_200))
    i = (int) (temp * /* 200.0 - 1.0 */ 199.0);
  else if ((_gfx.card_monitor & EGA_CARD) || (tipo_video == 3)) /* MOD D */
    i = (int) (temp * /* 350.0 - 1.0 */ 349.0);
  else /* if (_gfx.card_monitor & VGA_CARD) */	/* MOD F */
    i = (int) (temp * /* 480.0 - 1.0 */ 479.0);

  return i;
}


/*<14.03.89 *****************************************************************

          VERSIONE PER TOUCH SCREEN DELLA HP (VECTRA ES e SIMILI)

/*---------------------------------------------------------------------------
  programma il touch screen con la lista o la stringa arg_1 e
  pulisce il buffer del touch screen  (mette a falso il segnalatore)
  -------------------------------------------------------------------------*/
void ASTOUCH ()
{
  prog_touch = (typ (arg_1) == LIST) ? arg_1 : nome (arg_1);
  buff_touch = FALSE;			/* vuota il (flag del) buffer */
}

/*---------------------------------------------------------------------------
  riporta il valore con cui e' programmato il touch screen 
  -------------------------------------------------------------------------*/
void TOUCH ()
{
  risultato = prog_touch;
}

/*---------------------------------------------------------------------------
  riporta VERO o FALSO a seconda che ci sia una "toccata" nel buffer
  del touch screen 
  -------------------------------------------------------------------------*/
void TOUCH_ ()				/* primitiva TOUCH? */
{
  risultato = logval (buff_touch);
}

/*<27.02.89
/*---------------------------------------------------------------------------
  riporta le coordinate di dove e' stato toccato il video
  (lavora in righe e colonne  25x80)
  -------------------------------------------------------------------------*/
void DITO ()
{
  risultato = cons (int_num (riga_touch),
		    cons (int_num (colonna_touch), NULLP, LIST),
		    LIST);
}
*/

/*---------------------------------------------------------------------------
  riporta una lista di tre elementi:
  il nome della finestra piu' superficiale toccata e le coordinate di dove 
  e' stato toccato il video (lavora in righe e colonne  25x80)
  -------------------------------------------------------------------------*/
void DITO ()
{
  int nv, x, y;
  struct dview *pv;
  x = (colonna_touch * (VIDEO_X_F / 80.0));	/* coord. in pixel virtuali */
  y = (riga_touch * (VIDEO_Y_F / 25.0));	/* (le stesse di open_view) */
  x = scalex (x);				/* coord. in pixel FISICI */
  y = scaley (y);				/* (quelle della struct GFX) */
  no_int (); semaforo = ROSSO; si_int ();	/* occupa regione critica */
  nv = nview;
  while (nv != -1) {	/* controlla tutte le finestre andando all'indietro */
    pv = view [nv];
    if (dentro_fin (x, y, pv)) break;
    nv = pv->prima;
  };
  risultato = cons (nview2nome (nv),
                    cons (int_num (riga_touch),
		          cons (int_num (colonna_touch), NULLP, LIST),
		          LIST),
		    LIST);
  no_int (); semaforo = VERDE; si_int ();	/* libera regione critica */
}
/*27.02.89>*/


***************************************************************************

*/

#endif // IL89

/* ILMONDO.C */

