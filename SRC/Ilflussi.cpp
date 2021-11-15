/*ILFLUSSI.C	G. Toffoli

010605	in ASATTR aggiunto IperlogoSetAttr
000705	esteso ATTR e ASATTR a finestre Player (interfaccia usa dev, non view)
000511	definito MAX_LINE e sost. MAXCHARS con MAX_LINE in LEGGISTRINGA
000131	aggiunto PR_DRIVETYPES e PR_DRIVEREADY (dummy)
000118	aggiunto PR_CREACARTELLA, PR_RIMUOVICARTELLA, PR_RISALICARTELLA, PR_CARTELLE, PR_ARCHIVI
		completato funzionalità e controlli su COPIAARC
990722	esteso ATTR e ASATTR ad archivi
990702	aggiunto COPIAARC, RINOMINAARC
990515	riattivato scrittura su schermo TARTA (SCRIVI_SU, _prints)
990112	gestione di Commander (PC_COMANDI) in ATTR e SETATTR
981224	gestione di Browser in CHIUDI e FINESTRE
		controllo su chiusura di FOGLIO e BROWSER
981220	introdotto device Browser
981119	riattivato ATTR
980901	rivisto SCRIVISU e eliminato baco introdotto il 980728
980831	interfaccia cputs_, f_apri_s: char * -> const char *
980828	preparazione per conversione a C++; rivedere nomearc !!!!
		sostituoto min e max con __min e __max
980825	reimplementato CATALOGO
980728	in SCRIVISU riattivato casi TARTA e FOGLIO
		in LEGGIDA e SCRIVISU gestito esplicitamente caso CONSOLE !!!
		in LEGGIA, DOVELEGGI, SCRIVIA, DOVESCRIVI aumentato controlli
980727	eliminato errore aggiunto in rel 2.3f in apertura file
		reso nome2dev insensibile al case !!!
980709	introdotto device Player al posto di device Bottone
980210	modificato _fineleggi per device tipo FOGLIO
980204	corretto _bgets
980203	aggiunto TurtleWindows
980202	aggiunto dev2handle e dev2stato
980107	Gestione del foglio corrente; isWindow
971221	modificato _fineleggi
971217	aggiunto QueryWindows e modificato FINESTRE;
		aggiunto dev2nome; modificato ini_flussi
971124	ritoccato INI_FLUSSI e APRI (rivedere !!!!)
71112	ritoccato _prints per FOGLIO (rivedere !!!!)
971110	aggiunto f_apri_s (variante di f_apri)
971014	reimplementato lputs (output di stringa su STAMPANTE)
970915	aggiunto string2dev; non serve ?
970805	corretto chiusura foglio (non usa view LOGO)
970804	TARTA non e' piu' device preallocato !!!
970804	Iniziato ripulitura (eliminazione codice commentato per vers. DOS)
970618	per Editor: device di tipo FOGLIO, senza mediazione di ILVIEW
970603	ripristino provvisorio di device FOGLIO (bgets, bputs, LEGGIDA)
970503	uso di device MEMORY (nuovo) al posto di FOGLIO in MemScrivi
970423	recupero update del 02.10.96 (normalizzazione)
960910	aggiunto CURDIR
960517	iniziato porting specifico
960426	prima vers. Windows da Iperlogo del 30.06.89
*/

#include "stdafx.h"

#define TEMP
#define ILWIN
#define NORMALIZE
 
#define CMODULE
#include "ilcomune.cpp"
#include "ilfunz.h"

#ifdef NORMALIZE
int MatchKw (node, node);
#endif // NORMALIZE

#define MAX_LINE (4 * K)

#ifdef ILWIN
#include <direct.h>		// Required only for function declarations
#include <errno.h>		// Required for errno constants
#include <sys\stat.h>
#include <io.h>
#include <afx.h>

#define pc_IPERLOGO kw[77]
#define pc_ATTR kw[82]

int match_nocase (node, node);

void EditorClear (int);	
void EditorResetRead (int);
//int EditorGetLineCount (int);
//int EditorGetLineIndex (int);
int EditorEOF (int nview);
int EditorGetLine (int, char*, int);
void EditorReplace (int, char *);
int OpenPlayer (int dev, node lista);
int ClosePlayer (int dev);
int TurtleWindows (node *result);
int chiudi_browser (int dev);

void LogoPrint (int fun, char *s, int l);

#endif // ILWIN

// #define O_PLAYER 128	// 980703: al posto di O_BOTTONE


/*-----------------------------------------------------------------------
  inizializzazione dell' associazione dei flussi logici con device fisici
  -----------------------------------------------------------------------*/
void ini_flussi ()
{
  int i;
  for (i = MIN_DEV; i <= MAX_DEV; ++i) _file [i] = -1;
  _nome [CONSOLE] = pc_CONSOLE;
  _file [CONSOLE] = Q_COMANDI;        /* handle di console */
  _fstato [CONSOLE] = O_FINESTRA;     /* dichiara che console e' una fin. */
  _nome [STAMPANTE] = pc_STAMPANTE;
/*>971217
  _nome [FOGLIO] = pc_FOGLIO; _fstato [FOGLIO] = O_FINESTRA | O_FOGLIO;
>*/
#ifdef ILWIN
  _file [FOGLIO] = -1;				// Rivedere !!!!
//  _nome [TARTALFA] = pc_TARTA; _fstato [TARTALFA] = /* O_FINESTRA |*/ O_TARTA;
//  _file [TARTALFA] = Q_TARTA;               /* handle di tarta base */
#endif // ILWIN
  _nome [COM1] = pc_COM1;
  _nome [COM2] = pc_COM2;
  dev_leggi = dev_scrivi = dev_recupera = CONSOLE;
}
 
void err_pos (int dev)
// int dev;
{
	errore (26, funzione, _nome [dev]);
}

void dribble (char *s, int l)
// char	*s;
// int l;
{
  int locale;
  if (dev_dribble != 0) {
    locale = dev_out;
    dev_out = dev_dribble;
    _prints (s, l);
    dev_out = locale;
  }
}

/* ------------------------------------------------------------------------
  ritorna sotto forma di lista il messaggio di errore di codice specificato
  rappresentato con una o piu' stringhe e gli eventuali segnaposto $1 e $2
  sostituiti dagli oggetti o1 e o2
   ------------------------------------------------------------------------ */
node msgstr (int codice, node o1, node o2)
// int codice;
// node o1, o2;
{
  char c;
  int i, j;
  node lista, o;
  i = tab_stringhe [codice - 1];
  lista = NULLP;
  while ((c = s_stringhe [i]) != NULLC) {
    if (c == '$') {
      o = (s_stringhe [++i] == '1') ? o1 : o2;
      ++i;
    }
    else {
      j = i;
      while (((c = s_stringhe [j]) != NULLC) && (c != '$')) ++j;
      s_stringhe [j] = NULLC;
      o = logstr (&s_stringhe [i]);
      s_stringhe [j] = c;
      i = j;
    };
    lista = lstcat (lista, cons (o, NULLP, LIST));
  };
  return lista;
}

/* ------------------------------------------------------------------------
  riporta TRUE se la stringa MLOGO in ingresso e' valida come nome di Archivio
   ----------------------------------------------------------------------- */
int nomearc (node s)
// node s;
{
  char s_locale [MAXCHARS];
  pacstr (s_locale, nome (s));

#ifdef TEMP
	return TRUE;
#else // TEMP
  char fcb [40];
  inregs.h.ah = PARSE_FILENAME;
  inregs.h.al = 0;
  segregs.ds  = os_seg ((char *)&s_locale);
  inregs.x.si = os_off ((char *)&s_locale);
  segregs.es  = os_seg ((char *)&fcb);
  inregs.x.di = os_off ((char *)&fcb);
  intdosx (&inregs, &outregs, &segregs);
  return ((outregs.h.al == -1) || (*((char*) outregs.x.di + 1) == ' ')) ?
    FALSE : TRUE;
#endif // TEMP
}

/* ------------------------------------------------------------------------
  ritorna TRUE se esiste l' archivio con il nome specificato
  altrimenti FALSE
   ------------------------------------------------------------------------ */
int is_archivio (node ml_nome)
// node ml_nome;
{
	char searchfile [MAXCHARS];
	char pathbuffer[_MAX_PATH];
	char envvar[] = "";
	pacstr (searchfile, nome (ml_nome));
	
   /* Search for file in PATH environment variable: */
	_searchenv( searchfile, envvar, pathbuffer );
	return ( *pathbuffer != '\0' );
}

node dev2nome (int dev)
{
  return _nome [dev];
}

int dev2handle (int dev)
{
  return _file [dev];
}

int dev2stato (int dev)
{
  return _fstato [dev];
}


/* ------------------------------------------------------------------------
  se esiste un device [DI QUALS. TIPO col nome specificato ne ritorna
  l' indice  nell' array dei device, altrimenti ritorna -1
   ------------------------------------------------------------------------ */
int nome2dev (node lnome)
// node lnome;
{
	int i;
	for (i = 0; i <= MAX_DEV; ++i)
//>980727
//	if ((_file [i] != -1) && (_nome [i] == lnome)) return i;
	if (	(_file [i] != -1)
		&&	(	(_nome [i] == lnome)
			 ||	(match_nocase (nome (lnome), nome (_nome [i])) == 0)))
		return i;
//<980727
	return -1;
}

/* ------------------------------------------------------------------------
  se esiste un device [DI QUALS. TIPO col nome specificato ne ritorna
  l' indice  nell' array dei device, altrimenti ritorna -1
   ------------------------------------------------------------------------ */
int string2dev (char *s)
// char *s;
{
	char name [MAXCHARS];
	int i;
	for (i = 0; i <= MAX_DEV; ++i)
		if (_file [i] != -1) {
		pacstr (name, nome (_nome [i]));
			if (_stricmp (name, s) == 0) return i;
		}
	return -1;
}

/*------------------------------------------------------------------
  se esiste un device con l' handle e lo stato specificato
  ne riporta il numero di device (entry nell' indice), altrimenti -1
  ------------------------------------------------------------------*/
int handle2dev (int handle, int stato)
// int handle, stato;
{
  int i;
  for (i = 0; i <= MAX_DEV; ++i)
    if ((_file [i] == handle) && (_fstato [i] & stato)) return i;
  return -1;
}

/*-------------------------------------------------------------------------
  apre un file in lettura con il nome (stringa o parola MLOGO) specificato;
  se non ci riesce ritorna 0 o -1, altrimenti ritorna l' indice i dopo aver
  assegnato a _file [i] il puntatore al descrittore e a _nome [i] il nome
  -------------------------------------------------------------------------*/

int f_apri (node ml_nome, int mode, int pmode)
// node ml_nome;
// int mode;
// int	pmode;
{
  int i;
  char s_locale [MAXCHARS];
  if ((i = nuovo_dev ()) == 0) {}
  else {
    pacstr (s_locale, nome (ml_nome));
    if  ((_file [i] = _open (s_locale, mode, pmode)) != -1) {
      _nome [i] = ml_nome;
      _fstato [i] = O_ARCHIVIO;
    } else i = -1;
  };
  return i;
}

/*-------------------------------------------------------------------------
  come f_apri, ma con nome file specificato come stringa C
  -------------------------------------------------------------------------*/

// int f_apri_s (char *s_nome, int mode, int pmode)
int f_apri_s (const char *s_nome, int mode, int pmode)	// 980831
// char *s_nome;
// int mode;
// int	pmode;
{
  int i;
  if ((i = nuovo_dev ()) == 0) {}
  else {
    if  ((_file [i] = _open (s_nome, mode, pmode)) != -1) {
      _nome [i] = metti_sc (s_nome);
      _fstato [i] = O_ARCHIVIO;
    } else i = -1;
  };
  return i;
}

/*---------------------------------------------------------------------------
  chiude il file o la viewport cui e' stato assegnato in apertura 
  l' indice specificato.
  se ci riesce ritorna 0 altrimenti ritorna -1
  -------------------------------------------------------------------------*/
int f_chiudi (int i)
//int i;
{
  if (i >= MIN_DEV)
      if (_close (_file [i]) == 0) {
        _file [i] = -1;
        _fstato [i] = 0;
        return 0;
      }
 //     else	// 980828
      	return -1;
}

int currentWindow (int mask)
{
	int i;
	for (i = MIN_DEV; i <= MAX_DEV; ++i)
		if (   (_file [i] != -1)
			&& (_fstato [i] & mask) )
			return i;
	return -1;
}

int QueryWindows (int mask, node *result)
{
	int i, l;
	node list = NULLP;
	for (i = MIN_DEV; i <= MAX_DEV; ++i)
		if (   (_file [i] != -1)
			&& (_fstato [i] & mask) )
		list = cons (_nome [i], list, LIST);
	l = lstlen (list);
	if (result != NULL) *result = list;
	return l;
}

/* ------------------------------------------------------------------------
  trasferisce una linea terminata da LINEDEL o da "fine-foglio"
  da FOGLIO a stringa C per il numero massimo di caratteri specificati;
  ritorna la lunghezza della stringa comprendente eventuale LINEDEL
   ------------------------------------------------------------------------ */
int bgets (int dev, char *s, int maxc)
// char s [];
// int dev, maxc;
{
	int l;
/*>980204
	l = EditorGetLine (_file[dev], s, maxc);
*/
	l = EditorGetLine (_file[dev], s, maxc-2);
//	*(s+l) = CR; ++l;
	*(s+l) = LINEDEL; ++l;
/*<*/
	*(s+l) = NULLC;
	return l;
}

/* ------------------------------------------------------------------------
  (ri)scrive il contenuto di stringa-C S in buffer di DEVice tipo foglio
   ------------------------------------------------------------------------ */
void bputs (int dev, char *s)
// int dev;
// char *s;
{
	EditorReplace (_file[dev], s);			/* trasferisce caratteri */
}

/* ------------------------------------------------------------------------
  funzione simile alla fputs della libreria Lattice-C eccetto che :
  - fn e' un file-number (handle) anziche' un file-pointer
  - assegna TRUE a is_errore in caso di errore di disco
   ------------------------------------------------------------------------ */
void fputs_ (char *s, int fn, int stato, int l)
// char *s;
// int fn, stato, l;
{
  int count;
  char *s_corrente, c;

  if (stato & O_BINARIO)
    {if (_write (fn, s, l) != l) err1 (33);}
  else {
    while (l != 0) {
      s_corrente = s;
      count = 0;
      while ((l != 0) && ((c = *(s_corrente++)) != LINEDEL)) {
        ++count;
        --l;
      };
      if (count != 0)
        if (_write (fn, s, count) != count) {
          err1 (33);
          break;
        };
      if (c == LINEDEL) {
        --l;
        if (_write (fn, crlf, 2) != 2) {
          err1 (33);
          break;
        };
      };
      s = s_corrente;
    };
  };
}

/* ------------------------------------------------------------------------
  funzione simile alla fgets della libreria Lattice-C eccetto che :
  - fn e' un file-number (handle) anziche' un file-pointer
   ------------------------------------------------------------------------ */
int fgets_ (char *s, int n, int fn, int stato)
// char *s;
// int n, fn, stato;
{
  int status, i, l, binario;
  long pos, pos_ini;
  char c, *p;
  binario = stato & O_BINARIO;
  status = 1;
  p = s;
  
  pos = pos_ini = _lseek (fn, 0L, SEEK_CUR);
  while ((n > 0) && (status > 0)) {
/*  if (! binario) l = min(n, 40); legge da DOS in piccoli pezzi */
    l = __min (n, 512 - (511 & (int) pos));/* .. comunque entro un settore  */
    if ((i = status = _read (fn, p, l)) > 0) {
      if (p != s) {--p; ++i;};		/* per event. <CR>-<LF> a cavallo */ 
      while (   (i != 0)
             && (((c = *p) != CTRL_Z) || binario)
             && (binario || (c != CR)
                         || ((i != 1) && (*(p+1) != LF)))
            ) {++p; --i;};
      if (i != 0) {
        if (c == CR) {
          *(p++) = LINEDEL;
          ++pos_ini;			/* per corregere calcolo successivo */
        };
  break;
      }
      pos = pos + status;
      n = n - status;
    };
  };
  if (status == -1) {
    err1 (32);
    return 0;
  };
  *p = NULLC;
  _lseek (fn, (pos_ini + (l = p - s)) , SEEK_SET);
  return l;
}

/* ------------------------------------------------------------------------
  invia su device di list (STAMPANTE) una striga C di caratteri
  abortendo la funzione in caso di errore
   ------------------------------------------------------------------------ */
void lputs (char *s, int l)
// char *s;
// int l;
{
#ifdef ILDOS
  char c;
  inregs.x.dx = 0;	/* numero della stampante  */
  inregs.h.ah = 0;	/* codice di sottofunzione */
  while (l-- != 0) {
    if (lputc (c = *(s++))) {
      err (34);
      break;
    };
    if (c == CR) {
      lputc ((char) LF);
      i_stampante = 0;
    }
    else if (++i_stampante == d_stampa) {
      lputc ((char) CR);
      lputc ((char) LF);
      i_stampante = 0;
    };
  };
#endif // ILDOS
#ifdef ILWIN
			LogoPrint (0, s, l);
#endif // ILWIN
}


#ifdef ILDOS
/* ------------------------------------------------------------------------
  invia un carattere su device di list (STAMPANTE) eseguendo controllo di :
  carta presente, selezione, errore, time_out
   ------------------------------------------------------------------------ */
int lputc (char c)
{
  inregs.h.al = c; 
  int86 (PRINTER_IO, &inregs, &outregs);
  return ((!outregs.h.ah & 0x10) || (outregs.h.ah & 0x29));
}

#endif // ILDOS


char *pMemDevout;

void MemScrivi (char *s, node oggetto)
{
	int local = dev_out;
	dev_out = MEMORY;
	pMemDevout = s;
	_scrivi (oggetto);
	*pMemDevout = '\0';
	dev_out = local;
}

void tputs (int dev, char *s);

/* ------------------------------------------------------------------------
  output (ex formattato) su device di output corrente
  chiama computs/cputs/bputs/lputs/tputs/fputs per output
   ------------------------------------------------------------------------ */
void _prints (char *stringa, int l)
// char *stringa;
// int l;
{
int dev;
if (l != 0) {
  dev = dev_out;

	if (_fstato[dev] & O_TARTA) dev = TARTALFA;		// aggiunto
	else if (_fstato[dev] & O_FOGLIO) dev = FOGLIO;	// aggiunto

 switch (dev) {
  case CONSOLE:
    cputs_ (stringa);
    break;

  case STAMPANTE:
    lputs (stringa, l);
    break;

  case FOGLIO:
    bputs (dev_out, stringa);
    break;
    
#ifdef ILDOS
  case TARTALFA:
    break;
#endif // ILDOS
#ifdef ILWIN
  case TARTALFA:
    tputs (dev_out, stringa);
	break;
#endif // ILDOS

#ifdef ILWIN
  case MEMORY:
    memcpy (pMemDevout, stringa, l);
    pMemDevout += l;
    break;
#endif // ILWIN

  default:
    fputs_ (stringa, _file [dev_out], _fstato [dev_out], l);
  }
  }
}

void LUNGARC ()
{
	int dev;
	if ((dev = f_apri (arg_1, _O_RDWR | _O_RAW | _O_CREAT, _S_IWRITE | _S_IREAD)) <= 0) err2 (21, arg_1);
	else {
		risultato = long_num (_filelength (dev));
		f_chiudi (dev);
	}
}

/* ------------------------------------------------------------------------
  provando a leggere un singolo carattere da archivio numero fn ritorna
  - EOF		in caso di fine file fisico
  - il carattere letto altrimenti
   ------------------------------------------------------------------------ */
int getc_ (int fn)
// int fn;
{
  int status;
  char s [1];
  if ((status = _read (fn, s, 1)) == -1) err1 (32);
  return (status > 0) ? ((int) s [0]) : EOF;
}

/* ------------------------------------------------------------------------
  arretra di un carattere fisico la posizione corrente nell' archivio
  numero fn
   ------------------------------------------------------------------------ */
void ungetc_ (int fn)
// int fn;
{
  _lseek (fn, -1L, SEEK_CUR);
}

/* ------------------------------------------------------------------------
  provando a leggere da archivio numero fn ritorna
  - EOF		in caso di fine file fisico o logico (CTRL_Z)
  - LINEDEL	in caso di sequenza CR LF
  - il singolo carattere letto altrimenti
   ------------------------------------------------------------------------ */
int fgetc_ (int fn)
// int fn;
{
  int c, c2;
  switch (c = getc_ (fn)) {
  case CTRL_Z:
    c = EOF;
    ungetc_ (fn);
  case EOF:
    break;
  case CR:
    if ((c2 = getc_ (fn)) == LF) c = LINEDEL;
    else if (c2 != EOF) ungetc_ (fn);
  };
  return c;
}


int _fineleggi (int dev)
// int dev;
{
  int fn, c;
  if (_fstato [dev] & O_FOGLIO)
///   return (EditorGetLineIndex (_file[dev]) >= EditorGetLineCount (_file[dev])); 
	return EditorEOF (_file[dev]);
//#ifdef ILDOS // 971221
  else
  	if (dev < MIN_DEV) return FALSE;
//#endif // ILDOS
  else {
    if ((c = getc_ (fn = _file [dev])) != EOF) ungetc_ (fn);
    return ((c == EOF) || (c == CTRL_Z));
  }
}

int _finescrivi (int dev)
// int dev;
{
  int status;
  char s [1];

  if (dev == CONSOLE) return FALSE;
  else if (dev < MIN_DEV) return TRUE;
  else {
    if ((status = _read (_file [dev], s, 1)) == 1) {
      _lseek (_file [dev], -1L, SEEK_CUR);
      return (s [0] == CTRL_Z) ? TRUE : FALSE;
    }
    else {
      if (status == -1) err1 (32);
      return TRUE;
    }
  }
}

/* ------------------------------------------------------------------------
   ------------------------------------------------------------------------ */
void LEGGICAR ()
{
  char s_locale [2];
  int dev;
  char c;
  static char oldc = -1;
  dev = dev_leggi;			/* equipara fogli a FOGLIO */

  switch (dev) {
  case CONSOLE:				/* da console */
    s_locale [0] = c = getch_ ();
    s_locale [1] = NULLC;
    dribble (s_locale, 1);
	if (oldc != 0)
		c = case_char (c);
    oldc = c;
    break;

#ifdef ILDOS
  case FOGLIO:				/* da buffer di foglio */
    c = (_fineleggi (dev)) ?  EOF : (int) *(p_cbuf + i_leggi_foglio++);
    break;
#endif // ILDOS

  default:   				/* da file */
    c = fgetc_ (_file [dev_leggi]);
  };
  iwtstr ();
  risultato = (c == EOF) ? mettip (S_VUOTA) : mettip (wtstr (c));
}

void LEGGISTR ()
{
  int dev, i, j, is_finito, locale;
  char *p, c, s_locale [MAX_LINE+1];
  locale = is_esc;
  is_esc = TRUE;
  R = iwtstr ();
  is_finito = FALSE;
  dev = dev_leggi;			/* equipara fogli a FOGLIO */
  if (_fstato [dev] & O_FOGLIO) {
    dev = FOGLIO;
  };
  do {
    switch (dev) {
    case CONSOLE:				/* da console */
      i = cgets_ (s_locale, MAX_LINE);
      dribble (s_locale, i);
		case_string (s_locale);
      break;

    case FOGLIO:				/* da buffer di editor */
      i = bgets (dev_leggi, s_locale, MAX_LINE);
      break;

    default:				/* da file */
      i = fgets_ (s_locale, MAX_LINE, _file [dev_leggi], _fstato [dev_leggi]);
    };
    if ((i != 0) && (s_locale [i-1] == LINEDEL)) {
	--i;
        is_finito = TRUE;
    };
    p = &s_locale [0];
    for (j = 0; j < i; ++j) {
      R = wtstr (c = *(p++));
      libera_1 ();
    };
  } while ((i != 0) && (! is_finito));
  is_esc = locale;
  risultato = R;
}

/* ------------------------------------------------------------------------
  modifica il valore della variabile di stato booleana is_eco (ASECO)
   ------------------------------------------------------------------------ */
void ECO ()
{
  is_eco = bool_ (arg_1);
}
 
/*--------------------------------------------------------------------------
  ritorna VERO se arg_1 e' il nome di un archivio presente su drive corrente
  --------------------------------------------------------------------------*/
void ARCHIVIO ()
{
  risultato = logval (is_archivio (arg_1));
}

/*---------------------------------------------------------------------------
  cancella un file passando a BDOS (funzione 41h) la specifica completa
  deì file
  ---------------------------------------------------------------------------*/
void CANCEARC ()
{
	int dev;
	char s_locale [MAXCHARS];
	dev = nome2dev (arg_1);
	if ((dev != -1) && (_fstato [dev] & O_ARCHIVIO))	/* archivio aperto */
		err2 (24, arg_1);
	else {
 		pacstr (s_locale, nome (arg_1));
		if (remove (s_locale)) err2 (21, arg_1);
	}
}

void PR_COPIAARC ()
{
	int to_dev;
	char from_name [MAXCHARS], to_name [MAXCHARS];
	char from_path [_MAX_PATH], to_path [_MAX_PATH];
	char envvar[] = "";
	BOOL from_result, to_result;

	risultato = NULLP;
 	pacstr (from_name, nome (arg_2));
 	pacstr (to_name, nome (arg_1));
	// Search for FROM file in the PATH environment variable
	_searchenv(from_name, envvar, from_path);
	if (*from_path == '\0' ) {
		err2 (21, arg_2);
		return;
	}
	// is the TO file already open ?
	to_dev = nome2dev (arg_1);
	if ((to_dev != -1) && (_fstato [to_dev] & O_ARCHIVIO)) {	// archivio già aperto
		err2 (23, arg_1);
		return;
	}
//>000118
	// is to_name the name of a directory ?
	BOOL is_to_directory = FALSE;
	CFileStatus static_to_status;
	if (CFile::GetStatus(to_name, static_to_status)) {
		BYTE static_to_attribute = static_to_status.m_attribute;
		is_to_directory = static_to_attribute & FILE_ATTRIBUTE_DIRECTORY;
	}
//<
	CFileStatus from_status, to_status;
	// open the source file, read status, allocate buffer, read content
	CFileException* pFromError = NULL;
	UINT nOpenFlags_from = CFile::modeRead;
	CFile from_file;
	from_result = from_file.Open(from_path, nOpenFlags_from, pFromError);
//>000118
	if (! from_result) {
		err2 (31, arg_2);	// non riesco ad aprire ..
		return;
	}
	// get from_file FileName and other information
	CString fromFileName = from_file.GetFileName();
	// se destinazione è una cartella, fa merge con nome archivio from
	// e ripete test su apertura
	CString toName = to_name;
	if (is_to_directory) {
		toName = toName + "\\" + fromFileName;
		// is the TO file already open ?
		to_dev = nome2dev (arg_1);
		if ((to_dev != -1) && (_fstato [to_dev] & O_ARCHIVIO)) {	// archivio già aperto
			from_file.Close ();
			err2 (23, arg_1);
			return;
		}
	}
//<
	from_result = from_file.GetStatus(from_status);
	DWORD from_length = from_file.GetLength ();
	void* lpBuf = malloc (from_length + K);
	UINT from_count = from_file.Read ((void *) lpBuf, from_length);
	from_file.Close ();

	// create and write the destination file
	CFileException* pToError = NULL;
	UINT nOpenFlags_to = CFile::modeCreate | CFile::modeWrite;
	CFile to_file;
//>000118
//	to_result = to_file.Open(to_name, nOpenFlags_to, pToError);
	to_result = to_file.Open(toName, nOpenFlags_to, pToError);
	if (to_result) {
//<
		to_result = to_file.GetStatus (to_status);
		strcpy (to_path, to_status.m_szFullName);
		to_file.Write (lpBuf, from_count);
		to_file.Close ();
//		// free the buffer
//		free (lpBuf);
		// copy status dates
		to_result = CFile::GetStatus(to_path, to_status);
		to_status.m_ctime = from_status.m_ctime;	// The date and time the file was created.
		to_status.m_mtime = from_status.m_mtime;	// The date and time the file was last modified.
		to_status.m_atime = from_status.m_atime;	// The date and time the file was last accessed for reading.
		CFile::SetStatus(to_path, to_status);
		risultato = logstr (to_path);
//>000118
	}
	// free the buffer
	free (lpBuf);
//<

}

void PR_RINOMINAARC ()
{
// int rename( const char *oldname, const char *newname );
// Each of these functions returns 0 if it is successful.
// On an error, the function  returns a nonzero value and sets errno to one of the following values:
// EACCES File or directory specified by newname already exists or could not be created (invalid path); or oldname is a directory and newname specifies a different path.
// ENOENT File or path specified by oldname not found.
// EINVAL Name contains invalid characters.

	risultato = NULLP;

	int from_dev, to_dev;
	char from_name [MAXCHARS], to_name [MAXCHARS];
	char from_path [_MAX_PATH];
//	char to_path [_MAX_PATH];
	char envvar[] = "";

	risultato = NULLP;
 	pacstr (from_name, nome (arg_2));
 	pacstr (to_name, nome (arg_1));
	// Search for FROM file in the PATH environment variable
	_searchenv(from_name, envvar, from_path);
	if (*from_path == '\0' ) {
		err2 (21, arg_2);
		return;
	}
	// is the TO file open ?
	to_dev = nome2dev (arg_1);
	if ((to_dev != -1) && (_fstato [to_dev] & O_ARCHIVIO)) {	/* archivio aperto */
		err2 (23, arg_1);
		return;
	}
	// is the FROM file open ?
	from_dev = nome2dev (arg_2);
	if ((from_dev != -1) && (_fstato [from_dev] & O_ARCHIVIO)) {	/* archivio aperto */
		err2 (23, arg_2);
		return;
	}
	int result = rename (from_name, to_name);
	if (result == 0) {
//		risultato = logstr (to_path);
	}
	else if (errno == EACCES)	// File or directory specified by newname already exists or could not be created (invalid path); or oldname is a directory and newname specifies a different path.
		err2 (31, arg_1);
	else if (errno == ENOENT)	// File or path specified by oldname not found.
		err2 (21, arg_2);
	else if (errno == EINVAL)	// Name contains invalid characters.
		err2 (5, arg_1);
}

void NONREGISTRARE ()
{
  f_chiudi (dev_dribble);
  dev_dribble = 0;
}

void REGISTRA ()
{
  int dev = nome2dev (arg_1);
  if (dev == CONSOLE) NONREGISTRARE ();
  else if ((dev != -1) && (_fstato [dev] & O_ARCHIVIO))	/* archivio aperto */
    err2 (23, arg_1);
  else {						/* arch. da aprire */
    if ((dev = f_apri (arg_1, _O_WRONLY | _O_RAW | _O_APPEND, _S_IWRITE | _S_IREAD)) <= 0)
      err2 (31, arg_1);
    else dev_dribble = dev;				/* tutto OK */
  };
}

void APRI ()
{
	int dev;
	node lista_arg = NULLP, secondo = NULLP;
	node lDevName;
  if (typ (arg_1) == LIST) {
    lista_arg = cdr (arg_1);
			/* controllo correttezza del 2^ parametro */
    if (lista_arg != NULLP)
      if (   (! MatchKw ((secondo = car (lista_arg)), pc_BINARIO))
          && (! MatchKw (secondo, pc_TARTA))
          && (! MatchKw (secondo, pc_FOGLIO))
//>980709
//          && (! MatchKw (secondo, pc_BOTTONE)) ) {
          && (! MatchKw (secondo, pc_PLAYER)) ) {
//<980709
        err2 (5, arg_1);
        return ;
      };
//>980710
//	arg_1 = car (arg_1);
	lDevName = car(arg_1);	// DA QUI sostituito ovunque arg_1 con lDevName
//>980710
  }
//>980727
  else
	lDevName = arg_1;
//>980727
  dev = nome2dev (lDevName);
  if (dev >= MIN_DEV) err2 (23, lDevName);		/* gia' aperto */
////  else if (dev != -1) {}					/* device built-in rivedere */
//  else if (pc_FOGLIO == secondo) {			/* finestra tipo FOGLIO */
  else if (MatchKw (secondo, pc_FOGLIO)) {		/* finestra tipo FOGLIO */
      if (apri_foglio (lDevName) == -1) err2 (31, lDevName);
  }
  else if (MatchKw (secondo, pc_TARTA)) {		/* finestra tipo TARTA */
//    if (apri_tarta (arg_1) == -1) err2 (31, arg_1);
	if (apri_tarta (lDevName, NULL, -1, -1, -1, -1, -1, -1, -1.0, -1.0, -1.0, -1.0) == -1)
		err2 (31, lDevName);
  }
  else if (MatchKw (secondo, pc_PLAYER)) {		/* finestra tipo FOGLIO */
      if (OpenPlayer (dev, arg_1) == -1) err2 (31, lDevName);
  }
  else {										/* archivio */
    if ((dev = f_apri (lDevName, _O_RDWR | _O_RAW | _O_CREAT, _S_IWRITE | _S_IREAD)) <= 0)
		err2 (31, lDevName);
    else _fstato [dev] |= (MatchKw (secondo, pc_BINARIO) ? O_BINARIO : 0);
  };
}

/*-------------------------------------------------------------------------
  riporta l' indice di un entry per nuovo device nella relativa tabella,
  se lo trova, altrimenti -1
  -------------------------------------------------------------------------*/
int nuovo_dev () {
  int i = MIN_DEV;
  while ((i <= MAX_DEV) && (_file [i] != -1)) ++i;
  return (i > MAX_DEV) ? 0 : i;
}

void CHIUDI ()
{
  int dev, handle;
  node lista;
  lista = (typ (arg_1) == LIST) ? arg_1 : cons (arg_1, NULLP, LIST);
  while ((! is_errore) && (lista != NULLP)) {
    arg_1 = car (lista); lista = cdr (lista);
    dev = nome2dev (arg_1);
    if (dev == -1)					/* non aperto */
      err2 (22, arg_1);
    else if ((dev < MIN_DEV) /* && (dev != FOGLIO) */) {}	/* device  preallocato */
    else if (_fstato [dev] & O_FINESTRA) {		/* finestra .. */
      handle = _file [dev];
      if (_fstato [dev] & O_TARTA)			/* .. tipo TARTA */
        chiudi_tarta (handle);
      else if (_fstato [dev] & O_FOGLIO) {		/* .. tipo FOGLIO */
          if (! chiudi_foglio (dev)) {
 			err2 (30, arg_1);
			return;
		  }
	  }
      else if (_fstato [dev] & O_BROWSER) {		/* .. tipo BROWSER */
          if (! chiudi_browser (dev)) {
			err2 (30, arg_1);
			return;
		  }
	  }
      else if (_fstato [dev] & O_PLAYER)		/* .. tipo PLAYER */
          ClosePlayer (dev);
      _file [dev] = -1;
      _fstato [dev] = 0;
    }
    else if (f_chiudi (dev) != 0) 			/* archivio */
        err2 (36, arg_1);
    if (! is_errore)
      if (dev == dev_leggi) dev_leggi = dev_in = CONSOLE;
      else if (dev == dev_scrivi) dev_scrivi = dev_out = CONSOLE;
  };
}

void CHIUDITUTTO ()
{
  int i;
  for (i = MIN_DEV; i <= MAX_DEV; ++i)
    if (i != dev_dribble)
      if ((_file [i] != -1) && (! (_fstato [i] & O_FINESTRA))) {
        if (f_chiudi (i) != 0) err2 (36, arg_1);		/* giusto ?? CM */
        if (i == dev_leggi) dev_leggi = dev_in = CONSOLE;
        if (i == dev_scrivi) dev_scrivi = dev_out = CONSOLE;
      };
}

void LEGGIDA ()
{
	int dev = nome2dev (arg_1);
	if (dev == -1)
		err2 (22, arg_1);						/* non aperto */
//>980728
	else if (dev == dev_leggi)
		return;
	else if (dev == CONSOLE) {
	}
//<980728
	else if (	(dev == STAMPANTE)				/* STAMPANTE */
			 ||	(_fstato [dev] & O_TARTA))		/* tipo tarta */
		err2 (25, arg_1);
	else if (_fstato [dev] & O_FOGLIO) {		/* tipo foglio */
		i_leggi_foglio = 0;
		EditorResetRead (_file[dev]);
	}
	else if (_fstato [dev] & O_ARCHIVIO) {		/* archivio */
		if ((_lseek (_file [dev], 0L, SEEK_SET)) == -1L)
			err1 (32);
	};
	if (! is_errore)
		dev_in = dev_leggi = dev;
}

void DACHELEGGI ()
{
  risultato = _nome [dev_leggi];
}

void SCRIVISU ()
{
	int dev, fn;
	long i, pos;
	char s_locale [MAXCHARS];
	dev = nome2dev (arg_1);
	if (dev == -1)
		err2 (22, arg_1);			/* non aperto */

#ifdef ILWIN
	if (dev == dev_scrivi)
		return;

	if (dev_scrivi == STAMPANTE)
		LogoPrint (2, NULL, 0);

	if (dev == CONSOLE) {
	}
	else if (dev == STAMPANTE) {
		LogoPrint (1, NULL, 0);
	}

#endif // ILWIN

	else if (_fstato [dev] & O_TARTA) {		/* tipo tarta */
//#ifdef ILDOS
		if (MatchKw (_schermo, pc_COMANDI)) err2 (47, arg_1);
//#endif // ILDOS
//		err2 (25, arg_1);	// 980728
	}
	else if (_fstato [dev] & O_FOGLIO) {	/* tipo foglio */
#ifdef ILDOS
		AFINETESTO ();
#endif // ILDOS
//		i_scrivi_foglio = 0;	// 980728: da implementare !!!!!
	}
	else if ((pos = _lseek ((fn = _file [dev]), 0L, SEEK_END)) == -1L)
		err1 (32);
	else if ((pos != 0L) && ((127L & pos) == 0L)) {
		_lseek (fn, (long) (-128), 1);
		if (_read (fn, s_locale, 128) != 128) err1 (32);
		else {
			i = 0; while ((i < 128) && (s_locale [i] != CTRL_Z)) ++i;
			if (_lseek (fn, (long) (i-128), 1) == -1L) err1 (32);
		};
	};
							/* archivio */
	if (! is_errore) dev_out = dev_scrivi = dev;
}

void SUCHESCRIVI ()
{
  risultato = _nome [dev_scrivi];
}

/* ------------------------------------------------------------------------
   Riporta una lista con i nomi degli archivi e delle finestre aperte
   ------------------------------------------------------------------------ */
void ARCAPERTI ()
{
  int i;
  risultato = NULLP;
  for (i = MIN_DEV; i <= MAX_DEV; ++i)
    if (_file [i] != -1)
      risultato = cons (_nome [i], risultato, LIST);
}

/* ------------------------------------------------------------------------
   Riporta una lista con i nomi delle finestre aperte del tipo specificato
   in arg_1.
   Il primo nome della lista e' la finestra attuale (per TARTA e FOGLIO).
   ------------------------------------------------------------------------ */
void FINESTRE ()
{
	int mask;
	if (MatchKw (arg_1, pc_TARTA)) mask = O_TARTA;
	else if (MatchKw (arg_1, pc_FOGLIO)) mask = O_FOGLIO;	// aggiunto 971217
	else if (MatchKw (arg_1, pc_BROWSER)) mask = O_BROWSER;	// aggiunto 981224
	else { err_arg1 (); return; }

                                /* prelievo di tutte le finestre richieste */
/*> 971217
  int i;
  risultato = NULLP;
  for (i = MIN_DEV; i <= MAX_DEV; ++i)
    if (   (_file [i] != -1)
        && (_fstato [i] & mask) )
      risultato = cons (_nome [i], risultato, LIST);
*/
	if (mask == O_TARTA)
		TurtleWindows (&risultato);
	else if (mask == O_FOGLIO)
		QueryWindows (mask, &risultato);
	else if (mask == O_BROWSER)
		QueryWindows (mask, &risultato);
/*<*/
                                /* unione insiemistica per avere in testa */
                                /* la finestra attuale di quel tipo */
}
 
/* ------------------------------------------------------------------------
  ritorna una parola che rappresenta il disco (drive) default
   ------------------------------------------------------------------------ */
void DISCO ()
{
	iwtstr ();
	risultato = mettip (wtstr ((char) (_getdrive() - 1  + 'A')));
}

/* ------------------------------------------------------------------------
  assegna il disco (drive) default
   ------------------------------------------------------------------------ */
void ASDISCO ()
{
	int c;
	irdstr (nome (arg_1));
	c = rdstr ();
	_chdrive((c = toupper(c)) - 'A' + 1);
	DISCO ();
	irdstr (nome (risultato));
	if (c != rdstr ()) err_arg1 ();
}

#ifdef DOC	// from winbase.h
#define DRIVE_UNKNOWN     0
#define DRIVE_NO_ROOT_DIR 1
#define DRIVE_REMOVABLE   2
#define DRIVE_FIXED       3
#define DRIVE_REMOTE      4
#define DRIVE_CDROM       5
#define DRIVE_RAMDISK     6
#endif // DOC
// riporta i drive configurati sotto forma di lista di sottoliste
// della forma [<lettera> <tipo-drive>] dove tipo-drive è un numero
void PR_DRIVETYPES ()
{
	DWORD driveBits = GetLogicalDrives();
	int driveNumber = 0;
	char driveChar;
	char driveName [2] = "a";
	LPTSTR lpszRootPathName = TEXT("?:\\");
	UINT driveType;
	risultato = NULLP;
	node element, lastElement = NULLP;
	while (driveBits) {
		if (driveBits & 0x00000001) {
			driveChar = 'a' + driveNumber;
			driveName[0] = driveChar;
//			driveType = GetDriveType (driveName);
			*lpszRootPathName = TEXT(driveChar);
			driveType = GetDriveType (lpszRootPathName);
			element = cons (logstr(driveName),
						cons (int_num(driveType), NULLP, LIST), LIST);
			element = cons (element, NULLP, LIST);
			if (lastElement == NULLP)
				risultato = element;
			else
				putcdr (lastElement, element);
			lastElement = element;
		}
		driveBits >>= 1;
		++driveNumber;
	}
}

// estratto da: ENUMDRV.C  (Microsoft Source Code Samples)
//
/********************************************************************************\ 
*    FUNCTION: CheckRM(LPTSTR) 
* 
*    PURPOSE: Verifies that a removeable media drive contains a disk 
* 
*    COMMENTS: 
* 
*        This function is called each time a drive type is determined to be 
*        removeable (DRIVE_REMOVEABLE).  An attempt is made to open a 
*        file in the root directory of the drive.  If the attempt succeeds, 
*        then media is present in the drive and subsequent calls to the 
*        drive can be safely made.  If the call fails, then there is no media 
*        present in the drive and no attempts should be made to access this 
*        drive.  The Error Mode is temporarily set to 1 to allow failures 
*        to immediately return to the calling program.  This eliminates 
*        unwanted dialog boxes that prompt for disks to be placed in the 
*        drive. 
* 
*    INPUT: szDriveName - removeable media drive name (ex - "a:") 
* 
*    OUTPUT: Returns TRUE if media present 
*                    FALSE if media is not present 
\*********************************************************************************/ 
//
BOOL CheckRM( LPTSTR lpszDriveName )
{
//	TCHAR    szFileName[DIRECTORY_STRING_SIZE];
	TCHAR    szFileName[MAXCHARS];
	DWORD    dwHold;

	SetErrorMode( SEM_FAILCRITICALERRORS );

	lstrcpy( szFileName, lpszDriveName );
	lstrcat( szFileName, TEXT(".") );

	dwHold = GetFileAttributes (szFileName);

	SetErrorMode( 0 );

    //  If no error, media must be in drive.
	if (dwHold != 0xFFFFFFFF) {
		return(TRUE);
	}
    else {
//		dwHold = GetLastError();
//		if( dwHold != ERROR_NOT_READY )
//			ErrorMsg(TEXT("CheckRM: Get Removable Media Info Failure."));

        return(FALSE);
    }
}

void PR_DRIVEREADY ()
{
	LPTSTR	lpszDriveName = "a:";	// template con "a" come esempio
	int c;
	irdstr (nome (arg_1));
	c = rdstr ();
	*lpszDriveName = c;
	DWORD driveBits = GetLogicalDrives();
	BOOL isDriverReady = CheckRM (lpszDriveName);
	risultato = logval (isDriverReady);
}

/* --------------------------------------------------------------
  assegna la directory default per il drive default o specificato
   -------------------------------------------------------------- */
void CHDIR ()
{
	char dirname[MAXCHARS];
	getnam (dirname, arg_1);
	if (_chdir (dirname)) err_arg1 ();
}

/* ----------------------------------------------------------------
   full path of the current working directory for the default drive
   ---------------------------------------------------------------- */
void CURDIR ()
{
	char path[MAXCHARS+1];
	if (! _getcwd(path, MAXCHARS)) err_arg1 ();
	else risultato = logstr (path);
}

/* ------------------------------------------------------------------------
  specifica la nuova posizione corrente nel file di lettura
   ------------------------------------------------------------------------ */
void LEGGIA ()
{
	long pos;
	if (_segno (arg_1)) err_arg1 ();
	pos = num_long (arg_1);
//>980728
	if (dev_leggi == CONSOLE)
		err2 (26, pc_CONSOLE);
	else if (_fstato [dev_leggi] & O_TARTA)
		err2 (26, pc_TARTA);
	else
	if (_fstato [dev_leggi] & O_FOGLIO) {
	}
//<980728
	if (dev_leggi >= MIN_DEV) {
		if (_lseek (_file [dev_leggi], pos, SEEK_SET) == -1L) err1 (27);
	}
	else err_pos (dev_leggi);
}

/* ------------------------------------------------------------------------
  ritorna la posizione corrente nel file di lettura
   ------------------------------------------------------------------------ */
void DOVELEGGI ()
{
//>980728
	if (dev_leggi == CONSOLE)
		err2 (26, pc_CONSOLE);
	else if (_fstato [dev_leggi] & O_TARTA)
		err2 (26, pc_TARTA);
	else
//<980728>
	if (_fstato [dev_leggi] & O_FOGLIO) {
		risultato = int_num (i_leggi_foglio);
	}
	else if (dev_leggi >= MIN_DEV) {
		risultato = long_num (_lseek (_file [dev_leggi], 0L, SEEK_CUR));
	}
	else err_pos (dev_leggi);
}
 
/* ------------------------------------------------------------------------
  specifica la nuova posizione corrente nel file di scrittura
   ------------------------------------------------------------------------ */
void SCRIVIA ()
{
	long pos;
	if (_segno (arg_1)) err_arg1 ();
	pos = num_long (arg_1);
//>980728
	if (dev_scrivi == CONSOLE)
		err2 (26, pc_CONSOLE);
	else if (_fstato [dev_scrivi] & O_TARTA)
		err2 (26, pc_TARTA);
	else
//<980728
	if (_fstato [dev_scrivi] & O_FOGLIO) {
#ifndef TEMP
		VAICURSORE ()
#endif // TEMP
	}
	else if (dev_scrivi >= MIN_DEV) {
		if (_lseek (_file [dev_scrivi], pos, SEEK_SET) == -1L) err1 (27);
	}
	else err_pos (dev_scrivi);
}

/* ------------------------------------------------------------------------
  ritorna la posizione corrente nel file di scrittura
   ------------------------------------------------------------------------ */
void DOVESCRIVI ()
{
//>980728
	if (dev_scrivi == CONSOLE)
		err2 (26, pc_CONSOLE);
	else if (_fstato [dev_scrivi] & O_TARTA)
		err2 (26, pc_TARTA);
	if (_fstato [dev_scrivi] & O_FOGLIO) {
		risultato = 0;	// int_num (i_scrivi_foglio); da implementare !!!
	} else
//<980728
	if (dev_scrivi >= MIN_DEV) {
		risultato = long_num (_lseek (_file [dev_scrivi], 0L, SEEK_CUR));
	}
	else err_pos (dev_scrivi);
}

// void Catalogo (char *s, node *pRis);
void Catalogo (char *s, node *pRis, int noDirectory, int noFile);
void PR_CATALOGO ()
{
	char s_locale [MAXCHARS];
	if (n_argomenti == 0) strcpy (s_locale, "*.*");
	else if (! nomearc (arg_1)) {err_arg1 (); return;}
	else pacstr (s_locale, nome (arg_1));
	R = NULLP;
//	Catalogo (s_locale, &R);
	Catalogo (s_locale, &R, 0, 0);
	risultato = R;
}

void Albero (CString findPath, CString findPattern, node leftmostNode);
// il secondo argomento non viene mai usato
void PR_ALBERO ()
{
	char s [MAXCHARS];
	// pattern di ricerca default
	CString findPath = "";
	CString findPattern = "*.*";
	if (n_argomenti > 0) {
//		if (! nomearc (arg_1)) {err_arg1 (); return;}
		pacstr (s, nome (arg_1));
		findPattern = s;
		if (n_argomenti > 1) {
			pacstr (s, nome (arg_2));
			findPath = s;
		}
	}
	// inizializza risultato con lista contenente
	// solo il cammino della cartella corrente
	CURDIR ();
	node cartella = risultato;
	if (findPath.GetLength() > 0) {
		pacstr (s, cartella);
		CString currentPath = s;
		currentPath = currentPath + "\\" + findPath;
		cartella = logstr(currentPath);
	}
	R = cons (cartella, NULLP, LIST);
	// passa per indirizzo variabile protetta da GC
	Albero (findPath, findPattern, R);
	risultato = R;
}

// EEXIST: Directory was not created because dirname is the name of an existing file, directory, or device
// ENOENT: Path was not found
void PR_CREACARTELLA ()
{
	char dirname [MAXCHARS];
	pacstr (dirname, nome (arg_1));
	if (_mkdir(dirname)) err_arg1 ();
	if (_chdir (dirname)) err_arg1 ();
}

// ENOTEMPTY: Given path is not a directory; directory is not empty; or directory is either current working directory or root directory. 
// ENOENT: Path is invalid.
void PR_RIMUOVICARTELLA ()
{
	char dirname [MAXCHARS];
	pacstr (dirname, nome (arg_1));
	if (_rmdir(dirname)) err_arg1 ();
}

// ENOENT: Path was not found
void PR_RISALICARTELLA ()
{
	if (_chdir ("..")) err_arg1 ();
}

void PR_CARTELLE ()
{
	R = NULLP;
	Catalogo (NULL, &R, 0, 1);
	risultato = R;
}

void PR_ARCHIVI ()
{
	R = NULLP;
	Catalogo (NULL, &R, 1, 0);
	risultato = R;
}

void FINELEGGI ()
{
  risultato = logval (_fineleggi (dev_leggi));
}

void FINESCRIVI ()
{
  risultato = logval (_finescrivi (dev_scrivi));
}

node rassoc (node nome, node val, node lista);
node StaticFileGetAttr (node logoFileName)
{
	node lista = NULLP;
	char fileName [MAXCHARS];
	CFileStatus rStatus;
    pacstr (fileName, nome (logoFileName));
	BOOL ok =
		CFile::GetStatus (fileName, rStatus);
	if (ok)
		lista = rassoc (pc_ATTR, int_num (rStatus.m_attribute), lista);
	return lista;
}

node KwAssoc (node tail, node kWord);
int StaticFileSetAttr (node logoFileName, node attrList)
{
	char fileName [MAXCHARS];
	CFileStatus rStatus;
    pacstr (fileName, nome (logoFileName));
	BOOL ok =
		CFile::GetStatus (fileName, rStatus);
	if (! ok)
		return 1;
	node attr = KwAssoc (attrList, pc_ATTR);
	attr = car (cdr (attr));
	int attrByte = num_int (attr);
	rStatus.m_ctime = 0;
	rStatus.m_mtime = 0;
	rStatus.m_atime = 0;
	rStatus.m_size = 0;
	rStatus.m_szFullName[0] = '\0';
	rStatus.m_attribute = attrByte;
	CFile::SetStatus (fileName, rStatus);
	return 0;
}

node IperlogoGetAttr ();
node CommanderGetAttr ();
node TurtleGetAttr (int nview);
node EditorGetAttr (int nview);
node BrowserGetAttr (int nview);
node PlayerGetAttr (int dev);
void ATTR ()
{
if (MatchKw (arg_1, pc_SCHERMO))
	risultato = IperlogoGetAttr ();
else if (MatchKw (arg_1, pc_IPERLOGO))
	risultato = IperlogoGetAttr ();
else if (MatchKw (arg_1, pc_CONSOLE) || MatchKw (arg_1, pc_COMANDI))
	risultato = CommanderGetAttr ();
else {
	int dev = nome2dev (arg_1);
	if (dev == -1) {					/* non aperto */
//		err2 (22, arg_1);
		risultato = StaticFileGetAttr (arg_1);
		if (! risultato)
			err2 (21, arg_1);			// non trovo l'archivio ..
	}
    else if (_fstato [dev] & O_FINESTRA) {		/* finestra .. */
		int handle = _file [dev];
		if (_fstato [dev] & O_TARTA)			/* .. tipo TARTA */
			risultato = TurtleGetAttr (handle);
		else if (_fstato [dev] & O_FOGLIO)		/* .. tipo FOGLIO */
			risultato = EditorGetAttr (handle);
		else if (_fstato [dev] & O_BROWSER)		/* .. tipo BROWSER */
			risultato = BrowserGetAttr (handle);
		else if (_fstato [dev] & O_PLAYER)		/* .. tipo PLAYER */
			risultato = PlayerGetAttr (dev);
		else if (_fstato [dev] & O_ARCHIVIO)	/* .. tipo ARCHIVIO */
			;
    }
}
}

void IperlogoSetAttr (node attrList);	// 010605
void CommanderSetAttr (node attrList);
void TurtleSetAttr (int nview, node attrList);
void EditorSetAttr (int nview, node attrList);
void BrowserSetAttr (int nview, node attrList);
void PlayerSetAttr (int dev, node attrList);
void ASATTR ()
{
//>010605
if (MatchKw (arg_1, pc_IPERLOGO))
	IperlogoSetAttr (arg_2);
else
//<010605
if (MatchKw (arg_1, pc_CONSOLE) || MatchKw (arg_1, pc_COMANDI))
	CommanderSetAttr (arg_2);
else {
	risultato = CommanderGetAttr ();
	node nome = arg_1;
	int dev = nome2dev (nome);
	if (dev == -1) {				/* non aperto */
//		err2 (22, arg_1);
		int errore = StaticFileSetAttr (arg_1, arg_2);
		if (errore != 0)				// da distinguere
			err2 (21, arg_1);			// non trovo l'archivio ..
	}
    else if (_fstato [dev] & O_FINESTRA) {		/* finestra .. */
		int handle = _file [dev];
		if (_fstato [dev] & O_TARTA)			/* .. tipo TARTA */
			TurtleSetAttr (handle, arg_2);
		else if (_fstato [dev] & O_FOGLIO)		/* .. tipo FOGLIO */
			EditorSetAttr (handle, arg_2);
		else if (_fstato [dev] & O_BROWSER)		/* .. tipo BROWSER */
			BrowserSetAttr (handle, arg_2);
		else if (_fstato [dev] & O_PLAYER)		/* .. tipo PLAYER */
			PlayerSetAttr (dev, arg_2);
		else if (_fstato [dev] & O_ARCHIVIO)	/* .. tipo ARCHIVIO */
			;
    }
}
}

/* ILFLUSSI.C */
