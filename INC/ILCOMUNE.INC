/* ILCOMUNE.INC					21.04.89
								26.04.96
  dichiarazione di simboli esterni definiti in moduli assembler

  960409 GT - eliminato commenti nidificati

  31.10.88 GT -
  25.10.88 GT - eliminato C/A/D-FOGLIO e C/A-COMMENTO
*/

/****************************************************************************
		DATI PUBBLICI DEFINITI IN MODULI ASSEMBLER :
  ***************************************************************************/

extern
int	bcd_flag;	/* flags di errore in bcd (in modulo MLBMCOMN */

extern
char	desc_c [];	/* array di descrizione dei codici di tastiera
						(in modulo MLCODICI.ASM) */
extern
struct primitiva sis_tab [],	/* tabella descrittori funzioni di sistema */
		fine_sis [];	/* fine tabella di sistema */

extern
int	precisione;	/* precisione in aritmetica BCD */

extern
int	mlstack [];	/* stack di controllo e dei valori */

		/* definiti in MLLINGUA.C */
extern
char	m_M_LOGO [],	/* messaggio di presentazione M-LOGO */
	m_FOGLIO [],	/* scheletro di messaggio in linea di SERVIZIO */
	n_foglio [],	/* nome di parametro che identifica dimesione FOGLIO */
	n_stack [],	/* nome di parametro che identifica dimensione STACK */
	n_memoria [],
	n_tarta [],
	n_partenza [],
	n_stampa [];

extern
char	modelli [];	/* nomi di modello in messaggio iniziale */

		/* da C.ASM */

extern
char	partenza [],	/* nome eventuale archivio da recuperare al via      */
	nome_exe [],	/* "SLOGO" */
	nome_lib [],	/* "" */
	puntolib [],	/* ".LIB" */
	chiave1 [],	/* "xxxxxxxx */
	chiave2 [],	/* "xxxxxxxx */
	dosmsg_1 [],	/* messaggio : manca disco originale */
	dosmsg_2 [],	/* messaggio : errore su disco */
	dosmsg_3 [],	/* messaggio : attendere prego */
	*c_tarta,	/* indirizzo buffer per copia schermo TARTA */
	*f_tarta;	/* ind. fin. buffer per copia schermo TARTA */

#ifndef IL89

extern
char	s_stringhe [DIM_STRINGHE],/* super-stringa delle stringhe C */
		*parole [];		/* parole LOGO nel testo in lingua */

#else // IL89

extern
char	s_stringhe [DIM_STRINGHE],/* super-stringa delle stringhe C */
	parole [],	/* inizio delle parole LOGO nel testo in lingua */
	messaggi [],	/* inizio dei messaggi nel testo in lingua */
	i_lingua [],	/* inizio del testo in lingua */
	f_lingua [];	/* fine del testo in lingua */

#endif // IL89

/* ILCOMUNE.V */
