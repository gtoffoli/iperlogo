// iperlogo.cpp : Defines the class behaviors for the application.
//
/* iperlogo.cpp		G. Toffoli

040305	aggiunto e tolto msg in LibraryProtectionIn per trovare baco in LibRegisterLevel
040205	uso dei livelli anche in registraz. di applicazioni; iniz. nomi in file di protezione
031029	portato a 365 i giorni di prova per livello 1
020208	dialoghi di uscita condizionati a isExitDialog; kw pc_CONFERMS
010928	tutti i def_.._name con iniziale maiuscola
010924	in FIRMA aggiunto calcolo di regCode (richiede ..): non è possibile
010605	IperlogoSetAttr per attributo MENU
010602	in FIRMA e CREALIB aggiunto indicatore di libreria free (isLibraryFree)
010601	esteso da 4 a 16 le posizioni riservare a librerie in file di protezione
010526	aggiunto PR_SIGNATURE dopo PR_VERSION
010201	applicazioni (librerie) che non richiedono registrazione
010110	protezione: registrazione Iperlogo non è valida x libreria
001226	corretto controllo protezione
001218	posticipato test su protezione/registrazione
001211	ritoccato nome_lib
001210	in caso di libreria non registra applicazione
001122	iniziato configurazione con libreria
000628	aggiunta la sezione canvas al profilo (CanvasSize)
000121	uso di paraml anziché param1 per la data di installazione:
		la documentazione parla di 16 bit per param1 .. param5, definti però come int !!!
000120	in protezione calcolo custom di elapsed time basato su param1 = data installazione
		portato a 45 e 90 i giorni di prova per livello 0 e 1
991226	m_pBitmap[] da iperlvw.h (condivise tra finestre Tarta)
991125	disattivato in versione _DEBUG forzatura execDirectory = workDirectory
991005	aggiunto PR_ROOT (RADICE)
990925	aggiunto PR_VERSION (VERSIONE)
990901	gestione modalita' TARTA (turtleMode) anche da attributi
		in versione _DEBUG forzato execDirectory = workDirectory
990826	variabile globale turtleMode (versione conn Objective Views)
990815	rivisto integr. con OVs in base a sample test.cpp
990811	integrazione di Objective Views (OVs); turtledoc
990729	iniziato uso delle Registry Keys (inattivo)
990722	avvio di Iperlogo clickando un file .il; aggiunto MidiExit
990318	variabile globale con parola iperlogoDir
990311	azzerato parametri aggiuntivi in file di registrazione
990310	aggiunto ExitNoConfirm
990305	basato su tabella il controllo di expiration
990126	portato a 64 (= MAX_DEV) max numero finestre per tipo
981220	disattivato Welcome window; aggiunto IperlogoGetAttr()
980903	chiusura morbida: ConfirmExit
980831	eliminato extern "C" per conversione a C++ dei moduli C
980714	aggiunto InitCommander per ASTASTO e TASTO (v. Iperlvw.cpp)
980624	controllo su esecuzioni multiple; opzione per primopiano
980213	prova statistiche di uso
980212	prova protezione Cprot30
980209	in ExitInstance aggiunto main_fin e TurtleExit
980205	modificato IperlogoExit (gia' WinAppExit)
980204	In InitApp aggiunto AfxEnableControlContainer();
980122	Disattivato Welcome !!!
971210	chiamata a qui_top
971020	reso piu' parametrici i nomi nel profile
970915	aggiunto GetTurtleViewIndex e GetEditorViewIndex
970618	inizializzazione pointers a turtleViews e editorViews
*/

#include "stdafx.h"
#include "iperlogo.h"

#include "ilcpp.h"

#include "ildde.h"
#include "Cprot30.h"

#include "mainfrm.h"
#include "iperldoc.h"
#include "iperlvw.h"
// #include "turtleMdl.h"
// #include "turtleDoc.h"
#include "turtlefr.h"
#include "turtlevw.h"
#include "editorfr.h"
#include "editorvw.h"
// #include "webframe.h"
// #include "webview.h"
#include "usage.h"
#include <direct.h>		// Required only for function declarations

/////////////////////////////////////////////////////////////////////////////
// Define the Objective Views GUIDs. The GUIDs are declared in OdGuids.h.
// Including initguid.h before OdGuids.h forces the GUIDs to be defined
// in this module. Skip this if statically linking to the Objective Views
// library, because the GUIDs will be linked into the app from the library.
#ifdef _OVDLL
#include "initguid.h"
#include "OdGuids.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define LIB16
// #define OVs

#define pc_RADICE kw [81]
#define pc_CONFERMA kw [86]

void main_ini ();
void main_fin ();
void ini_foglio ();

void InitCommander ();
int Welcome ();
void TurtleExit ();
void MidiExit ();
void MainMenuCollapse ();
node LoadFileList (char *s); 

int Passpartout;
extern int MemPages;
extern int CursorSide;
extern char DefaultTurtle[];
extern char DefaultEditor[];
extern char DefaultBrowser[];
char StartFile[64] = "";
extern char DefaultPlayer[];

extern CSize CanvasSize;	// 000628

extern int isBrowserStatus;
extern int isBrowserDialog;
extern int isExitDialog;	// 020208


int isProfile = FALSE;

CString cmdLine;		// command line
CString execDirectory;	// directory of exe file in command line
CString workDirectory;	// work directory at program start

CString	exeName;		// nome eseguibile(senza .exe) se <> da Iperlogo
char* ext_lib = ".lil";	// estensione nome di libreria
char nome_lib[32] = "";	// nome libreria da cercare
//>040205
// extern BOOL LibRegister(), IsLibRegistered();
extern BOOL LibRegister();
extern int LibRegisterLevel ();
//<040205

node iperlogoDir;			// 990318
int iperlogoIsMenu = TRUE;	// 010605

char CommanderFont[MAX_CHARS] = "";
char EditorFont[MAX_CHARS] = "";
char TurtleFont[MAX_CHARS] = "";

#ifdef OVs
int defaultTurtleMode;		// modalita' di esecuzione Objective Views per TARTA

CString turtleMode_name = "modotarta";
#endif // OVs
CString ini_name = "iperlogo.ini";
CString topmost_name = "primopiano";
CString mem_pages_name = "pagine";
CString cursor_side_name = "cursore";
CString commander_name = "comandi";
CString turtle_name = "tarta";
CString editor_name = "foglio";
CString browser_name = "browser";
CString welcome_name = "welcome";
CString start_file_name = "partenza";
//>010928
CString def_turtle_name = "Tarta";
CString def_editor_name = "Foglio";
CString def_browser_name = "Browser";
CString def_player_name = "Player";
//<010928
CString pass_name = "passpartout";

BOOL ForceTopmost = FALSE;
int isAppExit = FALSE;

int horzRes;
int vertRes;

void ExitNoConfirm () {
	theApp.OnAppExit();
	exit(0);
}

void IperlogoExit ()
{
//>020208
//	if (! is_lib)
	if (isExitDialog && (! is_lib))
//<
		if (! (((CMainFrame*) theApp.m_pMainWnd)->ConfirmExit ()))	// 980903
			return;													//
	ExitNoConfirm ();
}

void NotYetMsg ()                                                                               
{
	AfxMessageBox ("Funzione non ancora disponibile");
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoApp

BEGIN_MESSAGE_MAP(CIperlogoApp, CWinApp)
	//{{AFX_MSG_MAP(CIperlogoApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIperlogoApp construction

CIperlogoApp::CIperlogoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	int i;
	m_pPalette = NULL;		// 980223
	for (i=0; i<16; i++) {	// 980225
		m_customColors[i] = RGB(255, 255, 255);
	}
	for (i=0; i<MAX_BITMAPS; i++)
		(m_pBitmap[i]) = NULL;
}

CIperlogoApp::~CIperlogoApp()
{
	int i;
	if (m_pPalette) delete(m_pPalette);
	for (i=0; i<MAX_BITMAPS; i++)
		if (m_pBitmap[i]) delete(m_pBitmap[i]);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIperlogoApp object

// CIperlogoApp NEAR theApp;
// Ci risparmiamo un sacco di casts facendo lavorare il linker
CIperlogoApp theApp;

#ifdef OVs

/////////////////////////////////////////////////////////////////////////////
// The one and only Transaction Model

// The transaction model records all transactions that have occured within an 
// application and is central to the undo/redo, record/playback and event 
// logging functionality.

MvcTransactionModel theTransactionModel;

/////////////////////////////////////////////////////////////////////////////
// Objective Views property cache

// The property cache decreases memory overhead by caching property objects.

CODPropertyCache thePropCache;

#endif // OVs

/////////////////////////////////////////////////////////////////////////////
// CIperlogoApp initialization

CREATESTRUCT Commander, Turtle, Editor, Browser, Dialog, Welcom;

CSingleDocTemplate* CIperlogoApp::NewTurtleTemplate()
{
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_TURTLE,
#ifndef OVs
		RUNTIME_CLASS(CIperlogoDoc),
#else // OVs
		RUNTIME_CLASS(CTurtleDoc),
#endif // OVs
		RUNTIME_CLASS(CTurtleFrame),    // new SDI frame window
		RUNTIME_CLASS(CTurtleView));
	AddDocTemplate(pDocTemplate);
	return pDocTemplate;
}

CSingleDocTemplate* CIperlogoApp::NewEditorTemplate()
{
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_EDITOR,
		RUNTIME_CLASS(CIperlogoDoc),
		RUNTIME_CLASS(CEditorFrame),    // new SDI frame window
		RUNTIME_CLASS(CEditorView));	// !!!
	AddDocTemplate(pDocTemplate);
	return pDocTemplate;
}

CSingleDocTemplate* CIperlogoApp::NewBrowserTemplate()
{
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_WEB,
		RUNTIME_CLASS(CIperlogoDoc),
		RUNTIME_CLASS(CWebFrame),    // new SDI frame window
		RUNTIME_CLASS(CWebView));	// !!!
	AddDocTemplate(pDocTemplate);
	return pDocTemplate;
}

//>000120 010110: spostato fuori
char *mysecfile = "Il.dll /w";/* this is your security file */
unsigned long paraml;
int param1, param2, param3, param4; unsigned int param5;
// utilizzo solo la metà meno significativa (2 byte) dei 4 parametri interi (4 byte ciascuno)
// per consentire futuro utilizzo dell'altra metà
//>010601 utilizzo 16 bytes in modo compatibile col passato
// short* pIntParams[4] = { (short*) &param1, (short*) &param2, (short*) &param3, (short*) &param4 };
int* pIntParams[4] = { &param1, &param2, &param3, &param4 };
//<010601

int f_code;
//>010603
int isIperlogoRegistered = FALSE, isLibraryRegistered = FALSE;
int esitoIperlogo, esitoApplicazione;
//<010603
CTime currentTime;
time_t currentTime_t, customTime_t;
CTimeSpan customElapsedSpan;
int customElapsedDays, customElapsedMinutes;
//<

int expirationTable [17] = {
//	30,	60,		// livelli di accesso 0 e 1
//	45,	90,		// livelli di accesso 0 e 1
	45,	365,	// livelli di accesso 0 e 1 - 031029
	3700, 3700,	// livelli di accesso 2 e 3
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	// altro
};
// int libExpirationTable = 30;
int libExpirationTable = 45; // 040205: uniformato durata per livello 0

#define someerror 1

void PerRegistrarti (int regLevel, int elapsedDays, CString regSerial) {
	CString msg = "";
	char daysString[16];

	int leftDays = expirationTable [regLevel] - elapsedDays;
	if (is_lib && (esitoApplicazione < 0))
		leftDays = -1;
	if (leftDays <= expirationTable [0]) {
//>		msg += "Un'installazione di Iperlogo e' in uso da ";
		msg += "Un'installazione di ";
		msg += exeName;
		msg += " e' in uso da ";
//<
		_itoa(elapsedDays, daysString, 10);
		msg += daysString;
		msg += " giorni.\n";
		if (leftDays < 0) {
			msg += " Il periodo di prova e' terminato.\n";
		}
		else {
			msg += " Ti restano ancora ";
			_itoa(leftDays, daysString, 10);
			msg += daysString;
			msg += " giorni di prova.\n";
		}
		msg += "Per registrarti usa il menu Aiuti.\n";
		msg += "Il tuo numero di identificazione: ";
		msg += regSerial;
		AfxMessageBox (msg);
	}
}

//>010113
// riporta TRUE se i parametri 1..4 sono stati inizializzati nel vecchio modo
BOOL isOldIntParams ()
{
	return	   (param1 == 0x0101) && (param2 == 0x0202)
			&& (param3 == 0x0303) && (param4 == 0x0404) && (param5 == 0x0505);
}
// reinizializza i parametri 1..4 nel nuovo modo modo
void FreeOldIntParams ()
{
	param1 = 0; param2 = 0; param3 = 0; param4 = 0; param5 = 0;
	int cpResult = cpSetUParams (mysecfile,
			paraml, param1, param2, param3, param4, param5);
}
//<010113

#ifdef LIB16

int nLibParams = 16;
// prende uno di 16 (4x4) valori compresi tra 0 e 255 (0xFF)
// in una successione up-compatibile
int GetLibParam (int pos)
{
	int i = pos % 4;
	int j = pos / 4;
	return (int) ((*pIntParams[i] >> (j * 8)) & 0xFF);
}
// riporta la prima posizione, tra le 16 riservate alle librerie nel file di protezione,
// il cui contenuto ha il valore specificato, oppure -1 se non trovata
int FindLibParam (int val)
{
	int pos;
	for (pos=0; pos<nLibParams; pos++)
		if (val == GetLibParam(pos))
			return pos;
	return -1;
}
// (ri)assegna una delle 16 posizioni riservate alle librerie nel file di protezione
void SetLibParam (int pos, int val)
{
	int i = pos % 4;
	int j = pos / 4;
	val = val << (j * 8);
	int mask = ~ (0xFF  << (j * 8));
	*pIntParams[i] = (*pIntParams[i] & mask) | val;
}
// libera la posizione occupata da val tra le 16 del file di protezione:
// da chiamare, passando il numero di libreria, quando si registra la libreria
int FreeLibParam (int val)
{
	int pos = FindLibParam (val);
	if (pos >= 0) {
		SetLibParam(pos, 0);
		int cpResult = cpSetUParams (theApp.m_regPath,
			paraml, param1, param2, param3, param4, param5);
	}
	return pos;
}

#else // LIB16

//>010111
// riporta la prima posizione (tra 0 e 3) in cui incontra val
// tra i primi 4 parametri del file di protezione, oppure -1 se non trovato
int FindIntParam (int val)
{
	int i;
	for (i=0; i<4; i++)
		if (val == *(pIntParams[i]))
			return i;
	return -1;
}
// libera posizione occupata da val tra i primi 4 parametri del file di protezione:
// da chiamare, passando il numero di libreria, quando si registra la libreria
int FreeIntParam (int val)
{
	int i = FindIntParam (val);
	if (i >= 0) {
		*(pIntParams[i]) = 0;
		int cpResult = cpSetUParams (theApp.m_regPath,
			paraml, param1, param2, param3, param4, param5);
	}
	return i;
}
void SetIntParam(int i, int val)
{
	*(pIntParams[i]) = val;
}

#endif // LIB16

/*	you may want to set the time-stamp of your security file
	to a standard value in order not to awake the users curiosity...
	you do it once - cProtect will keep it at that all the time */
// set dummy time to protection file and returns error (<> 0)
int SetDummyTime (char *mysecfile)
{
//	long yymmdd = 931111;	/* the date */
//	int hhmm = 311; 		/* the time */
	long yymmdd = 991122;	/* the date */	// 000120
	int hhmm = 1538; 		/* the time */	// 000120
	return (cpSetFTime (mysecfile, yymmdd, hhmm));
}

// protection-related actions at program start
int CIperlogoApp::ProtectionIn ()
{
	int timeSize = sizeof(time_t);
	long magic = 361946;
	long fileversion = 22;
//	char *mysecfile = "Il.dll /w";/* this is your security file */
	CString msg, daysCString;
	int result;
	char sernum[15];
	int elapsedDays;
	int cpResult;	// 000120 spostato fuori

	f_code = 0;
	m_regPath = mysecfile;
	m_regAccess = 0;
	m_regDays = 0;
	/* first check if the program is registered */
	result = cpIsRegistered (mysecfile);
	switch (result) {
	case INI_CONTROL_FAILED:// if the number in the '.ini' file is different from that stored in the security file.
		break;
	case INI_CONTROL_MISSING:// if the '.ini' file or the entry is not found.
		break;
	case DISKSERNUM_FAULT:	// if the C: disks serial number is different from one stored in the security file.
		break;
	case NOT_REGISTERED:	// if no valid regcode is found.
		break;
	case IS_REGISTERED: 	// if a valid regcode is found.
		isIperlogoRegistered = TRUE;	// 010603
		/* then check which access your program is registered for */
		char rcode[15];
		rcode[0] = '\0';	/* make rcode empty */
		f_code = cpCheckRegCode(mysecfile, rcode);
		if((f_code < 1) || (f_code > 15))
		{
			/* here you tell the user that something has gone wrong, the regcode in the security file is not matching his serial number*/
			/* and close the program...*/
			return someerror;
		}
		m_regAccess = f_code;
		m_regCode = rcode;
		break;
	case FILE_NOT_FOUND:	// if path is not valid.
		/*	if you have an installation procedure then this is an error
		and you should tell the user that somehow something
		went wrong: in this case the program ends here
		returning some errorcode */
		/*	if you dont have an installation procedure
		then this may be the first time the program is used,
		so the security file needs to be created.
		You should tell the user that obviously this is
		the first time and a serial number will be created
		which he should supply when registering */

		if(cpCFGMake(mysecfile, magic, fileversion)) {
			/* oops! could not create the file!!! */
			return someerror;
		}
//>040205
		m_regName = "";
		m_regCompany = "";
		result =
			cpSetRegisteredNames (m_regPath, m_regName, m_regCompany);
//<040205
		// paraml <- current time
		unsigned long paraml;
		CTime currentTime = CTime::GetCurrentTime();
		time_t currentTime_t = currentTime.GetTime();
		// initialize other params
		int param1, param2, param3, param4; unsigned int param5;
//>010110
//		param1 = 0x0101; param2 = 0x0202; param3 = 0x0303; param4 = 0x0404; param5 = 0x0505;
		param1 = 0; param2 = 0; param3 = 0; param4 = 0; param5 = 0;
		if (is_lib)
			paraml = 0;
		else
//<010110
			paraml = (unsigned long) currentTime_t;
		cpResult = cpSetUParams (m_regPath,
			paraml, param1, param2, param3, param4, param5);
		break;
	}

	/* now lets get the serial number */

/*>981207 : patch per malfunzionamento di cProtect
	if(cpGetSernum (mysecfile, sernum))
	{
		problems accessing the file...
		return someerror;
	}
*/
	BOOL sernumOk = FALSE;
	while (! sernumOk) {
		if(cpGetSernum (mysecfile, sernum))
			/* problems accessing the file...*/
			return someerror;
		if (isdigit (sernum[1]))
			sernumOk = TRUE;
		else {
			// cancella il security file
			// ri-crea il security file
			if(cpCFGMake(mysecfile, magic, fileversion))
				return someerror;
		}
	}
/*<*/

	m_regSerial = sernum;

	switch (result) {
	case INI_CONTROL_FAILED:// if the number in the '.ini' file is different from that stored in the security file.
		break;
	case INI_CONTROL_MISSING:// if the '.ini' file or the entry is not found
		break;
	case DISKSERNUM_FAULT:	// if the C: disks serial number is different from one stored in the security file.
		msg = "Nuova istallazione.\nPer registrarti usa il menu Aiuti.\nIl tuo numero di identificazione:\n";
		msg += m_regSerial;
		AfxMessageBox (msg);
		break;
	case NOT_REGISTERED:	// if no valid regcode is found.
		elapsedDays = cpDaysSinceInstallation(mysecfile);
		// get current time
		currentTime = CTime::GetCurrentTime();
		currentTime_t = currentTime.GetTime();
		// get user parameters
//		unsigned long paraml;
//		int param1, param2, param3, param4; unsigned int param5;
		cpResult = cpGetUParams (m_regPath,
			&paraml, &param1, &param2, &param3, &param4, &param5);
//<010113
		if (isOldIntParams())
			FreeOldIntParams();
//>010113
		// set paraml (time) if not set already
		if (   (paraml == 0)			// dopo 990311 o per installazione libreria
			|| (paraml == 0x01db3cab))	// prima di 990311
		{
//>010110
			if (is_lib)
				paraml = 0;
			else
//<010110
				paraml = (unsigned long) currentTime_t;
//>010110
			// initialize other params
//			param1 = 0x0101; param2 = 0x0202; param3 = 0x0303; param4 = 0x0404; param5 = 0x0505;
//<010110
			cpResult = cpSetUParams (m_regPath,
				paraml, param1, param2, param3, param4, param5);
//>	solo per controllo
//			cpResult = cpGetUParams (m_regPath,
//				&paraml, &param1, &param2, &param3, &param4, &param5);
//			cpResult = SetDummyTime (mysecfile);
//			if (cpResult)
//			{ return someerror;	}
//<
	}

		if (! is_lib) {	//>010110
			// get custom registration time
			customTime_t = (time_t) paraml;
			// get custom elapsed time
			customElapsedSpan = CTimeSpan(currentTime_t - customTime_t);
			customElapsedDays = customElapsedSpan.GetDays();
			customElapsedMinutes = customElapsedSpan.GetTotalMinutes();
			if (customElapsedDays < elapsedDays)
				elapsedDays = customElapsedDays;
			m_regDays = elapsedDays;
		}	//>010110
		break;
	case IS_REGISTERED: 	// if a valid regcode is found.
//<010113
		cpResult = cpGetUParams (m_regPath,
			&paraml, &param1, &param2, &param3, &param4, &param5);
		if (isOldIntParams())
			FreeOldIntParams();
//>010113
//>010528
		// get custom registration time (serve per firma)
		customTime_t = (time_t) paraml;
//<010528
		break;
	case FILE_NOT_FOUND:	// if path is not valid.

		/*	it is now time to set the ini-file entry
		if you wish to use that feature */
		//		if( cpMakeIniNote( mysecfile, "my section", "my entry", "myfile.ini /w" ))
		//		{
		/* analyze the errorcode returned */
		/* act accordingly */
		//		}

		cpResult = SetDummyTime (mysecfile);
		if (cpResult)
		{ return someerror;	}
		break;
	}

	/*	OK if we have come this far then either the program
	is registered and f_code has a value between 1 and 15
	or the program is not registered and f_code = 0 */

	/*	maybe you want to check how long this s.o.b has used
	your great program without registering and take some action?? */

	//	nella versione con libreria non raccoglie statistiche
	if (! is_lib) {
		/* if you want to keep record of usage then start the timer */
		if (cpStartProgram(mysecfile))
		{
			// what happened to the security file???
			// maybe tell the user???
			return someerror;
		}
	}

	/* here comes your program - you now know if and for which access
	the program is registered and can take proper actions
	depending on the value of f_code */
	return 0;
}

// protection-related actions at program start
int CIperlogoApp::ProtectionOut ()
{
	//	nella versione con libreria non raccoglie statistiche
//	if (is_lib)
//		return 0;

	/*	time to finish the program... */
	/*	if you started the timer then now is the proper time
		to stop it */
	if(cpEndProgram (m_regPath))
	{
		// what happened to the security file???
		// maybe tell the user???
		return someerror;
	}

	/*	now you have the opportunity to enlighten the user
		of how much he has been using your great program...*/
//>020208
	if (isExitDialog)  {
//<
		CUsage UsageDialog;
		int result = UsageDialog.DoModal();
		if (result == IDOK) {
		}
	}
	return 0;
}

//>010110
#define DIM_REGCODE 12
#define OFFSET_FREE DIM_REGCODE
#define OFFSET_LIBSERIAL (OFFSET_FREE + 4 )
#define OFFSET_DATA (OFFSET_LIBSERIAL + 4)
#define DIM_FIRMA (OFFSET_DATA + 4)
extern char firma_lib [DIM_FIRMA];
int librarySerial;		// 010201
int isLibraryFree;		// 010602
time_t libraryTime_t;	// 010526
BOOL ScriviFirma();

BOOL ScriviDataInFirma()
{
	BOOL ok = FALSE;
	libraryTime_t = * ((time_t *) &firma_lib[OFFSET_DATA]);
	if (libraryTime_t == 0) {
		CTime currentTime = CTime::GetCurrentTime();
		libraryTime_t = currentTime.GetTime();
		* ((time_t *) &firma_lib[OFFSET_DATA]) = libraryTime_t;
		ok = ScriviFirma();
	}
	return ok;
}

// chiamata solo se c'è libreria (is_lib)
int CIperlogoApp::LibraryProtectionIn ()
{
	// registrazione
/*> 040205
	if (IsLibRegistered ())
	{
		isLibraryRegistered = TRUE;	// 010603
		m_regAccess = 3;
	}
	else
	{
		m_regAccess = 0;
	}
*/
	m_regAccess = LibRegisterLevel ();
//>040305
//char ra[MAX_CHARS];
//_itoa(m_regAccess, ra, 10);
//AfxMessageBox (CString("m_regAccess = ") + ra);
//<040305
	if (m_regAccess > 0)
		isLibraryRegistered = TRUE;
/*< 040205 */
	BOOL isAnnotated = (FindLibParam(librarySerial) >= 0);
	// tempo corrente
	CTime currentTime = CTime::GetCurrentTime();
	time_t currentTime_t = currentTime.GetTime();
	// tempo di prima esecuzione in libreria
	libraryTime_t = * ((time_t *) &firma_lib[OFFSET_DATA]);
	if (! isLibraryRegistered) {
		// prima esecuzione ?
		if (libraryTime_t == 0) {
			if (isAnnotated && (! isLibraryFree)) {
				return -1;	// disinstallata ? può solo essere registrata
			}
			// riscrive firma con data aggiornata in libreria
//			libraryTime_t = currentTime_t;
//			* ((time_t *) &firma_lib[OFFSET_DATA]) = libraryTime_t;
//			BOOL ok = ScriviFirma();
			ScriviDataInFirma();
		}
		if (! isAnnotated) {
			if (! isLibraryFree) {
				// (ri)cerca posto in parametri di file di protezione
				int i = FindLibParam (0);
				if (i < 0) {
					return -2;	// non c'è posto
				}
				// annota numero libreria in file di protezione se non libera
				SetLibParam(i, librarySerial);
				int cpResult = cpSetUParams (m_regPath,
					paraml, param1, param2, param3, param4, param5);
			}
		}
	}
	// tempo trascorso
	CTimeSpan libraryElapsedSpan = CTimeSpan(currentTime_t - libraryTime_t);
	int libraryElapsedDays = libraryElapsedSpan.GetDays();
	m_regDays = libraryElapsedDays;
	return 0;
}
//<010111

char* IntToDec(int intValue)
{
	static char s[256];
	return _itoa(intValue, s, 10);
}

int CIperlogoApp::LoadUpdateProfile (int isUpdate)
{
	BOOL result;
	if (! isUpdate) {		// 980403: aggiunto condizione
		HDC hDC = ::GetDC(NULL);
		CDC DC;
		CDC* pDC = DC.FromHandle(hDC);	
		int cx = pDC->GetDeviceCaps(HORZRES);
		int cy = pDC->GetDeviceCaps(VERTRES);
		horzRes = cx;
		vertRes = cy;
		char path[MAX_CHARS];
		_searchenv("iperlogo.ini", "", path);
		if ((path[0] == '\0') && (workDirectory != execDirectory)) {
			_chdir (execDirectory);
			_searchenv("iperlogo.ini", "", path);
			_chdir (workDirectory);
		}
		if (path[0] == '\0') {
			AfxMessageBox ((CString ("Non trovo il file ")) + ini_name);
			return FALSE;
		}
		ini_name = path;
		if ((cx >= 1024) && (cy >= 768)) {
			commander_name = "comandi_1024";
			turtle_name = "tarta_1024";
			editor_name = "foglio_1024";
			browser_name = "browser_1024";
			welcome_name = "welcome_1024";
		}
		else if ((cx >= 800) && (cy >= 600)) {
			commander_name = "comandi_800";
			turtle_name = "tarta_800";
			editor_name = "foglio_800";
			browser_name = "browser_800";
			welcome_name = "welcome_800";
		}
		else if ((cx >= 640) && (cy >= 480)) {
			commander_name = "comandi";
			turtle_name = "tarta";
			editor_name = "foglio";
			browser_name = "browser";
			welcome_name = "welcome";
		}
		else {
			AfxMessageBox ("Dimensioni di schermo non adatte");
			return FALSE;
		}
	}

	if (isUpdate && isProfile) {

		result = WritePrivateProfileString(commander_name, "x", IntToDec(Commander.x), ini_name);
		result = WritePrivateProfileString(commander_name, "y", IntToDec(Commander.y), ini_name);
		result = WritePrivateProfileString(commander_name, "cx", IntToDec(Commander.cx), ini_name);
		result = WritePrivateProfileString(commander_name, "cy", IntToDec(Commander.cy), ini_name);
		result = WritePrivateProfileString(commander_name, "Font", CommanderFont, ini_name);

		result = WritePrivateProfileString(turtle_name, "x", IntToDec(Turtle.x), ini_name);
		result = WritePrivateProfileString(turtle_name, "y", IntToDec(Turtle.y), ini_name);
		result = WritePrivateProfileString(turtle_name, "cx", IntToDec(Turtle.cx), ini_name);
		result = WritePrivateProfileString(turtle_name, "cy", IntToDec(Turtle.cy), ini_name);
		result = WritePrivateProfileString(turtle_name, "Font", TurtleFont, ini_name);

		result = WritePrivateProfileString(editor_name, "x", IntToDec(Editor.x), ini_name);
		result = WritePrivateProfileString(editor_name, "y", IntToDec(Editor.y), ini_name);
		result = WritePrivateProfileString(editor_name, "cx", IntToDec(Editor.cx), ini_name);
		result = WritePrivateProfileString(editor_name, "cy", IntToDec(Editor.cy), ini_name);
		result = WritePrivateProfileString(editor_name, "Font", EditorFont, ini_name);
		// Flushes the cache
		result = WritePrivateProfileString(NULL, NULL, NULL, ini_name);
	}

	if (! isUpdate) {
		Commander.x = GetPrivateProfileInt(commander_name, "x", 0, ini_name);
		Commander.y = GetPrivateProfileInt(commander_name, "y", 0, ini_name);
		Commander.cx = GetPrivateProfileInt(commander_name, "cx", 640, ini_name);
		Commander.cy = GetPrivateProfileInt(commander_name, "cy", 240, ini_name);

		Turtle.x = GetPrivateProfileInt(turtle_name, "x", 0, ini_name);
		Turtle.y = GetPrivateProfileInt(turtle_name, "y", 0, ini_name);
		Turtle.cx = GetPrivateProfileInt(turtle_name, "cx", 640, ini_name);
		Turtle.cy = GetPrivateProfileInt(turtle_name, "cy", 480, ini_name);
		Turtle.hMenu = (HMENU) GetPrivateProfileInt(turtle_name, "menu", 1, ini_name);

		Editor.x = GetPrivateProfileInt(editor_name, "x", 0, ini_name);
		Editor.y = GetPrivateProfileInt(editor_name, "y", 0, ini_name);
		Editor.cx = GetPrivateProfileInt(editor_name, "cx", 640, ini_name);
		Editor.cy = GetPrivateProfileInt(editor_name, "cy", 480, ini_name);
		Editor.hMenu = (HMENU) GetPrivateProfileInt(editor_name, "menu", 1, ini_name);

		Browser.x = GetPrivateProfileInt(browser_name, "x", 0, ini_name);
		Browser.y = GetPrivateProfileInt(browser_name, "y", 0, ini_name);
		Browser.cx = GetPrivateProfileInt(browser_name, "cx", 640, ini_name);
		Browser.cy = GetPrivateProfileInt(browser_name, "cy", 480, ini_name);
		Browser.hMenu = (HMENU) GetPrivateProfileInt(browser_name, "menu", 1, ini_name);
		isBrowserStatus = GetPrivateProfileInt(browser_name, "stato", 1, ini_name);
		isBrowserDialog = GetPrivateProfileInt(browser_name, "vai", 1, ini_name);

		Welcom.x = GetPrivateProfileInt(welcome_name, "x", 0, ini_name);
		Welcom.y = GetPrivateProfileInt(welcome_name, "y", 0, ini_name);
		Welcom.cx = GetPrivateProfileInt(welcome_name, "cx", 640, ini_name);
		Welcom.cy = GetPrivateProfileInt(welcome_name, "cy", 480, ini_name);
 
 		ForceTopmost = GetPrivateProfileInt("Iperlogo", topmost_name, 0, ini_name);
 		MemPages = GetPrivateProfileInt("Iperlogo", mem_pages_name, 16, ini_name);
		CursorSide = GetPrivateProfileInt("Iperlogo", cursor_side_name, 16, ini_name);
		Passpartout = GetPrivateProfileInt("Iperlogo", pass_name, 0, ini_name);
#ifdef OVs
		defaultTurtleMode = GetPrivateProfileInt("Iperlogo", turtleMode_name, 0, ini_name);
#endif // OVs

		GetPrivateProfileString("Iperlogo", def_turtle_name, "", &DefaultTurtle[0], 32, ini_name);
		GetPrivateProfileString("Iperlogo", def_editor_name, "", &DefaultEditor[0], 32, ini_name);
		GetPrivateProfileString("Iperlogo", def_browser_name, "", &DefaultBrowser[0], 32, ini_name);
		GetPrivateProfileString("Iperlogo", def_player_name, "", &DefaultPlayer[0], 32, ini_name);
		GetPrivateProfileString("Iperlogo", start_file_name, "", &StartFile [0], 64, ini_name);
//>000628
		CanvasSize.cx = GetPrivateProfileInt("canvas", "cx", 800, ini_name);
		CanvasSize.cy = GetPrivateProfileInt("canvas", "cy", 800, ini_name);
//<
	}
	return TRUE;
}

// Add a static BOOL that indicates whether the class was
// registered so that we can unregister it in ExitInstance
static BOOL bClassRegistered = FALSE;

BOOL CIperlogoApp::FirstInstance()
{
	CWnd *pWndPrev, *pWndChild;

	// Determine if another window with our class name exists...
	if (pWndPrev = CWnd::FindWindow(_T("IperlogoClass"),NULL))
	{
		// if so, does it have any popups?
		pWndChild = pWndPrev->GetLastActivePopup();

		// If iconic, restore the main window
		if (pWndPrev->IsIconic())
		pWndPrev->ShowWindow(SW_RESTORE);

		// Bring the main window or its popup to the foreground
		pWndChild->SetForegroundWindow();

		// and we are done activating the previous one.
		return FALSE;
	}
	// First instance. Proceed as normal.
	else
			return TRUE;
}

// registry-related actions
int RegistryInfo (int livello, char* lpValueName, char* lpValue)
{
	HKEY hKeyIn,	// handle to previously open key
		hKeyOut;	// handle to newly open key

	char* lpSubKeys[8] = {		// names of subkeys
		"SOFTWARE",
		"Linl srl & Sisco srl, Roma",
		"Iperlogo Tre",
		"3.05.990211"
	};
	char* lpSubKey;				// pointer to name of current subkey

	DWORD ulOptions = 0;		// reserved; must be 0
	REGSAM samDesired =			// security access mask
		KEY_QUERY_VALUE;

	int i = 0;
	hKeyIn = HKEY_LOCAL_MACHINE;

	while (i < livello) {
		lpSubKey = lpSubKeys[i];

		LONG openReturn =
		RegOpenKeyEx (hKeyIn, lpSubKey, ulOptions, samDesired, &hKeyOut);
		if (openReturn != ERROR_SUCCESS)
			return -i;
		hKeyIn = hKeyOut;
		++i;
	}

	LPDWORD lpReserved = NULL;  // reserved; must be NULL
	DWORD valueType;			// buffer for value type
static BYTE valueData[MAXCHARS];// data buffer
	DWORD dataSize = MAXCHARS;	// data buffer size

	LONG queryResult;
	if (lpValueName) {
		queryResult = RegQueryValueEx (hKeyIn, lpValueName,
				lpReserved,
				&valueType, &valueData[0], &dataSize);
		if (queryResult != ERROR_SUCCESS)
			return -1;
		memcpy (lpValue, &valueData[0], dataSize);
		lpValue[dataSize] = '\0';
	}
	return 0;
}


node LoadFileList (char *s) 
{
	return cons (pc_RECUPERA, cons (DEL_VIRGOLETTE, cons (metti_sc (s), NULLP, LIST), LIST), LIST);
}

extern CMenu* MainMenuGetItem (UINT iPos);
void LibMainMenuCollapse ()
{
	int i;
	CMainFrame* mainFrame = (CMainFrame*) theApp.m_pMainWnd;
	CMenu* mainMenu = mainFrame->GetMenu ();
	CMenu* menuPopup;
	mainMenu->RemoveMenu(6, MF_BYPOSITION);	// Pausa/Continua
	mainMenu->RemoveMenu(5, MF_BYPOSITION);	// Interrompi
	mainMenu->RemoveMenu(4, MF_BYPOSITION);	// Esegui
	menuPopup = MainMenuGetItem (3);		// Aiuto
	CString nomeRegistra = (CString) "Registra " + exeName + "..";	// Registra <applicazione>
	while (menuPopup->GetMenuItemID(0) != ID_REGISTER)
			menuPopup->RemoveMenu (0, MF_BYPOSITION);
//>010601 : non rimuovo conmunque il menu di registrazione
//	if (theApp.m_regAccess == 0)
		menuPopup->ModifyMenu(ID_REGISTER, MF_BYCOMMAND, ID_REGISTER, (LPCTSTR) nomeRegistra);
//	else
//		menuPopup->RemoveMenu (0, MF_BYPOSITION);
	mainMenu->RemoveMenu(2, MF_BYPOSITION);	// Imposta
	menuPopup = MainMenuGetItem (1);		// Finestre
	while (menuPopup->GetMenuItemCount() > 0) {
		menuPopup->RemoveMenu (0, MF_BYPOSITION); 
	}
	menuPopup = MainMenuGetItem (0);
	int itemCount = menuPopup->GetMenuItemCount();
	for (i=itemCount-1; i>=0; i--) {
		if (menuPopup->GetMenuItemID(i) != ID_APP_EXIT)
			menuPopup->RemoveMenu (i, MF_BYPOSITION); 
	}
	mainFrame-> DrawMenuBar();
}

BOOL CIperlogoApp::InitInstance()
{
/*>980624
	If a previous instance of the application is already running,
	then activate it and return FALSE from InitInstance to
	end the execution of this instance.
*/
	if(! FirstInstance())
		return FALSE;

	// Register our unique class name that we wish to use
	WNDCLASS wndcls;

	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL
											// defaults

	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = ::DefWindowProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different
                                                  // icon
	wndcls.hCursor = LoadCursor( IDC_ARROW );
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;

	// Specify our own class name for using FindWindow later
	wndcls.lpszClassName = _T("IperlogoClass");

	// Register new class and exit if it fails
	if(!AfxRegisterClass(&wndcls))
	{
		TRACE("Class Registration Failed\n");
		return FALSE;
	}
	bClassRegistered = TRUE;
/*<980624*/

#ifdef OVs
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
//		AfxMessageBox(IDP_OLE_INIT_FAILED);
		AfxMessageBox("Da sostituire");
		return FALSE;
	}
#endif // OVs

	AfxEnableControlContainer();
/*
	Standard initialization
	If you are not using these features and wish to reduce the size
	of your final executable, you should remove from the following
	the specific initialization routines you do not need.
*/

	char WorkDirectory [MAX_CHARS];
	_getcwd (WorkDirectory, MAX_CHARS);
	workDirectory = WorkDirectory;
	workDirectory.MakeLower ();
	is_lib = FALSE;
	cmdLine = GetCommandLine ();
	cmdLine.MakeLower ();
	int posExe = cmdLine.Find("iperlogo.exe");
	if (posExe >= 0)
		exeName = "Iperlogo";
	else {
		// e' configurazione con libreria ?
		int pos2 = cmdLine.Find(".exe");
		if (pos2 > 0) {
			CString string2 = cmdLine.Left (pos2);
			posExe = string2.ReverseFind('\\');
			if (posExe >= 0) {
				++posExe;
 				execDirectory = cmdLine.Left (posExe);
				exeName = cmdLine.Mid(posExe, pos2-posExe);
				strcpy (nome_lib, (LPCTSTR) exeName);
				strcat (nome_lib, ext_lib);
				is_lib = TRUE;
			}
		}
		if (posExe < 0) {
			AfxMessageBox ("L'applicazione non è correttamente installata");
			exit(0);
		}
	}
 	execDirectory = cmdLine.Left (posExe);
	execDirectory.TrimLeft ("\"");
	execDirectory.TrimRight ("\\/");

	SetDialogBkColor();        // Set dialog background color to gray

//	LoadStdProfileSettings();

	int i;
	theApp.m_mainStatusShown = 0;
	theApp.m_mainPrinterData = NULL;
	theApp.m_turtlePrinterData = NULL;
	theApp.m_editorPrinterData = NULL;
	for (i = 0; i < MAX_TURTLE_VIEWS; ++i) theApp.m_turtleViews[i] = NULL;
	for (i = 0; i < MAX_EDITOR_VIEWS; ++i) theApp.m_editorViews[i] = NULL;
	for (i = 0; i < MAX_BROWSER_VIEWS; ++i) theApp.m_browserViews[i] = NULL;
	nview = -1;

	if (! LoadUpdateProfile (FALSE))
		return FALSE;

	main_ini();				//// initializations in module ILMAIN.C
	iperlogoDir = metti_sc (execDirectory);

//>010603 anticipato
	esitoIperlogo = ProtectionIn();
	if (is_lib) {
		// anticipato qui da dentro LibraryProtectionIn()
		librarySerial = * ((int *) &firma_lib[OFFSET_LIBSERIAL]);
		// libreria non richiede registrazione ?
		isLibraryFree = * ((int *) &firma_lib[OFFSET_FREE]);
		esitoApplicazione = LibraryProtectionIn();
	}
//<010603 anticipato
	if (is_lib && (isLibraryFree || (librarySerial == 0))) {
		esitoApplicazione = 0;
		m_regAccess = 3;
	}
	else {
//		if (ProtectionIn() != 0) {
		if (esitoIperlogo != 0) {
			AfxMessageBox ("Applicazione non correttamente installata");
			exit(0);
		}
//>010114
		if (is_lib) {
			if (esitoApplicazione != 0) {
				if (esitoApplicazione == -1)
					AfxMessageBox ("L'applicazione è stata reinstallata");
				else if (esitoApplicazione == -2)
					AfxMessageBox ("Troppe applicazioni in prova");
//				exit(0);	// 010115
			}
		}
//<010114
//>010201
	}
//<010201

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIperlogoDoc),
		RUNTIME_CLASS(CMainFrame),     // main SDI frame window
		RUNTIME_CLASS(CIperlogoView));
	AddDocTemplate(pDocTemplate);

/*
	pDocTemplate = new CSingleDocTemplate(
		IDR_TURTLE,
		RUNTIME_CLASS(CIperlogoDoc),
		RUNTIME_CLASS(CTurtleFrame),     // alternate SDI frame window
		RUNTIME_CLASS(CTurtleView));
	AddDocTemplate(pDocTemplate);
*/
	// enable file manager drag/drop and DDE Execute open
//>001210
//	nella versione con libreria non registra applicazione
	if (! is_lib) {
//<001210
		EnableShellOpen();
		RegisterShellFileTypes();
	}
	// simple command line parsing
/*>990721
	if (m_lpCmdLine[0] == '\0')
	{
		// create a new (empty) document
		OnFileNew();
	}
	else
	{
		// open an existing document
		OpenDocumentFile(m_lpCmdLine);
	}
*/
	// create a new (empty) document
	OnFileNew();
	if (m_lpCmdLine[0] != '\0')
	{
		// (prepare to) open an existing document
		strcpy(StartFile, m_lpCmdLine+1);
		StartFile[strlen(StartFile)-1] = '\0';
	}
/*<*/
        
	m_pMainWnd->DragAcceptFiles();

	CreateDdeServer();

	if (is_lib)
		expirationTable [0] = libExpirationTable;
	if (m_regAccess == 0)
		PerRegistrarti (0, m_regDays, m_regSerial);

	InitCommander ();

	if (! is_lib)
	{
		if (m_regDays > expirationTable [m_regAccess])
		{
			MainMenuCollapse ();
			StartFile[0] = '\0';
		}
		else
		{
			ini_foglio ();
			ini_tarta ();
			if (StartFile[0] != '\0')
				NestedExec (LoadFileList (StartFile));
		}
	}

//	qui_top ();		dummy ?
	
	// carica libreria ?
	if (is_lib)
	{
//>010605
		if (! isLibraryRegistered)
			iperlogoIsMenu = FALSE;
//<010605
		LibMainMenuCollapse ();
		if ((esitoApplicazione == 0) && (m_regDays <= expirationTable [m_regAccess]))	// 010115
		{
			ini_foglio ();
			ini_tarta ();
			node startFunction, startCommand;
			if (cercav (startFunction = metti_sc (exeName), UFUN))
			{
				startCommand = cons (startFunction, NULLP, LIST);
				NestedExec (startCommand);
			}
		}
	}

	((CMainFrame*) m_pMainWnd)->CommandPrompt ();
	return TRUE;
}

int CIperlogoApp::ExitInstance()
{
//>980624
	if(bClassRegistered)
		::UnregisterClass(_T("IperlogoClass"),AfxGetInstanceHandle());
//<980624

	//	ProtectionOut ();	// va in errore se chiuso da Menu
	int result = CWinApp::ExitInstance();
#ifndef NEW_EXIT		// 980403: spostato in OnAppExit
//>980624
	if(bClassRegistered) {
//<980624
		DestroyDdeServer ();
		MidiExit ();
		TurtleExit ();
		main_fin ();
#endif // NEW_EXIT
//>980624
	}
//<980624
	return result;
}

void CIperlogoApp::ExitApplication()
{
	OnAppExit();
}

int CIperlogoApp::GetTurtleViewIndex (CTurtleView* pView)
{
	int i;
	for (i = 0; i < MAX_TURTLE_VIEWS; ++i)
		if (m_turtleViews[i] == pView) return i;
	return -1;
}

int CIperlogoApp::GetEditorViewIndex (CEditorView* pView)
{
	int i;
	for (i = 0; i < MAX_EDITOR_VIEWS; ++i)
		if (m_editorViews[i] == pView) return i;
	return -1;
}

int CIperlogoApp::GetBrowserViewIndex (CWebView* pView)
{
	int i;
	for (i = 0; i < MAX_BROWSER_VIEWS; ++i)
		if (m_browserViews[i] == pView) return i;
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CIperlogoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void HideCommander ()
{
	BOOL result;
	WINDOWPLACEMENT current;
	
    current.length =  sizeof(WINDOWPLACEMENT);
	result =
		theApp.m_pMainWnd->GetWindowPlacement(&current);
	ASSERT(result);
    current.showCmd = SW_HIDE; 
    current.flags = WPF_SETMINPOSITION;                                                  
	result =
		theApp.m_pMainWnd->SetWindowPlacement(&current);
	ASSERT(result);
}                                                                                

void ShowCommander ()
{
	BOOL result;
	WINDOWPLACEMENT current;

    current.length =  sizeof(WINDOWPLACEMENT);
	result =
		theApp.m_pMainWnd->GetWindowPlacement(&current);
	ASSERT(result);
    current.showCmd = SW_SHOW; 
    current.flags = WPF_SETMINPOSITION;                                                  
	result =
		theApp.m_pMainWnd->SetWindowPlacement(&current);
	ASSERT(result);
}

/////////////////////////////////////////////////////////////////////////////
// CIperlogoApp commands

// NOTA: Non passa qui chiudendo da System Bar del Commander
void CIperlogoApp::OnAppExit() 
{
	// TODO: Add your command handler code here

//>020208
//	if (! is_lib) {
	if (isExitDialog && (! is_lib)) {
//<
		if (! (((CMainFrame*) m_pMainWnd)->ConfirmExit ()))
				return;					//
		ProtectionOut ();
		((CMainFrame*) m_pMainWnd)->RecordProfile ();		// 980403
	}

#ifdef NEW_EXIT		// 980403: spostato da ExitInstance
	DestroyDdeServer ();
	TurtleExit ();
	main_fin ();
#endif // NEW_EXIT

	isAppExit = TRUE;
	CWinApp::OnAppExit();

	if (! is_lib)
		LoadUpdateProfile (TRUE);
}

node rassoc (node nome, node val, node lista);
node rassoc2 (node nome, node val1, node val2, node lista);
node IperlogoGetAttr ()
{
	node lista = NULLP;

	lista = rassoc (pc_CONFERMA, logval(isExitDialog), lista);	// 020208

	lista = rassoc2 (pc_DIM, 
		consreal (horzRes),
		consreal (vertRes), lista);

//	node iperlogoDir = metti_sc (WorkDirectory);	// 990318
	lista = rassoc (pc_RADICE, iperlogoDir, lista);	// 990722

	return lista;
}

node KwAssoc (node tail, node kWord);
void IperlogoSetAttr (node attrList)
{
	node menu = KwAssoc (attrList, pc_MENU);
	if (menu != NULLP) {
		menu = car (cdr (menu));
		if (MatchKw (menu, pc_FALSO))
			iperlogoIsMenu = FALSE;
		else if (MatchKw (menu, pc_VERO))
			iperlogoIsMenu = TRUE;
	}
//>020208
	node conferma = KwAssoc (attrList, pc_CONFERMA);
	if (conferma != NULLP) {
		conferma = car (cdr (conferma));
		if (MatchKw (conferma, pc_FALSO))
			isExitDialog = FALSE;
		else if (MatchKw (conferma, pc_VERO))
			isExitDialog = TRUE;
	}
//<020208
}

void PR_VERSION ()
{
	int nVersion = 0; 
	CString strVersion;
	BOOL ok = strVersion.LoadString(IDS_VERSION);
	if (ok)
		nVersion = atoi(strVersion);
	risultato = int_num (nVersion);
}

void PR_ROOT ()
{
	risultato = metti_sc (execDirectory);

}

//>040205
// void ComputeFirmaLib (char* dest, CString regSerial, CString exeName, int librarySerial);
void ComputeFirmaLib (char* dest, CString regSerial, CString exeName, int librarySerial, int regLevel);
//>040205
void PR_SIGNATURE ()
{
//>040205
//	if (n_argomenti == 3)
//	{
	if (n_argomenti >= 3)
	{
		int regLevel = 3;
		if (n_argomenti >= 4)
			regLevel = num_int(args[3]);
//<040205
		char regCode[32];
		char a1[32];
		char a2[32];
		getnam(a1, arg_1);
		CString regSerial = a1;
		getnam(a2, arg_2);
		CString exeName = a2;
		librarySerial = num_int(arg_3);
//>010924
		// manca nome applicazione o è Iperlogo ?
		if ((exeName.GetLength == 0) || (exeName.CompareNoCase("Iperlogo") == 0)) 
		{
			int regAccess = librarySerial;
			int result = -1;	// toppa provvisoria ?
//			int result = cpMakeRegCode(regAccess, (const char*) a1, regCode);
			if (result != 0)
				regCode[0] = 0;
		}
		else
//>010924
//>040205
//			ComputeFirmaLib (regCode, regSerial, exeName, librarySerial);
			ComputeFirmaLib (regCode, regSerial, exeName, librarySerial, regLevel);
//<040205
		risultato = logstr(regCode);
	}
	else
	{
		int execCode;
		int isRegistered;
		node isFree = NULLP;
		time_t firstExecution;
		if (is_lib)
		{
			execCode = librarySerial;
			isRegistered = isLibraryRegistered;
			isFree = cons (logval(isLibraryFree), NULLP, LIST);
			firstExecution = libraryTime_t;
		}
		else
		{
			execCode = -1;
			isRegistered = isIperlogoRegistered;
			firstExecution = customTime_t;
		}
		struct tm* ptm = gmtime(&firstExecution);
//		int sec, min, hour, wday, yday;
		int mday = ptm->tm_mday;
		int mon = ptm->tm_mon + 1;
		int year = ptm->tm_year + 1900;
//>040209
//		node codelist = cons (int_num(execCode), cons (logstr(theApp.m_regSerial), cons (logval(isRegistered), isFree, LIST), LIST), LIST);
		node codelist = cons (int_num(execCode), cons (logstr(theApp.m_regSerial), cons (int_num(theApp.m_regAccess), isFree, LIST), LIST), LIST);
//<040200
		node datelist = cons (int_num(mday), cons (int_num(mon), cons (int_num(year), NULLP, LIST), LIST), LIST);
		node applicazioni = NULLP;
		int i;
		for (i=0; i<nLibParams; i++)
			if (GetLibParam(i) != 0) {
				applicazioni = cons (int_num(GetLibParam(i)), applicazioni, LIST);
			}
		risultato = cons (codelist, cons (datelist, cons (applicazioni, NULLP, LIST), LIST), LIST);
	}
}

