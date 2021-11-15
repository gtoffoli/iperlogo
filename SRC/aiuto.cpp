/* AIUTO.CPP

000429	esteso WriteReference
991228	aggiunto campo versione
990727	modificato nome cartella per file temporanei (tempDir)
990722	modificato gestione di workDirectory e execDirectory
990705	ristrutturato insieme a SelglossD.cpp
990705	generazione ciclica di nomi di file HTML
990511	cambiato nomi a campi di tabelle
990510	cambiato nomi a campi di tabelle
		gestito riferimenti multipli in campo "vedi anche" (see)
990416
990318
*/

#include "stdafx.h"
#include <fcntl.h>
#include <sys\stat.h>
#include <afxdao.h>

#include "Iperlogo.h"
#include "ilcpp.h"
#ifdef _WEB
#include "webframe.h"
#include "webview.h"
#endif _WEB

// #define pc_AIUTO	kw [79]
//extern char WorkDirectory [];
extern CString execDirectory;

CString tempDir = "\\temp\\";

node DaoQuery (const char* dbName, const char* sql, BOOL withHeading, BOOL collapseRows);
int QueryWindows (int mask, node *result);

int nome2dev (node lnome);
int dev2handle (int dev);

#ifdef _WEB
int apri_browser (node lDevName, node attrList);
void BrowserFileOpen (int nview, CString pathName);
#endif _WEB

int f_apri_s (const char *s_nome, int mode, int pmode);
int f_chiudi (int i);
void fputs_ (char *s, int fn, int stato, int l);

char strGlossary [] = "Glossario";
int maxGlossario = 32;
int nGlossario = 0;

void WriteHtml (const char *s)
{
    fputs_ ((char *) s, _file [dev_out], _fstato [dev_out], strlen ((char *) s));
}


void WriteMemo (node memo)
{
	char strMemo [16 * K];
	if (typ (memo) == LIST) MemScrivi (strMemo, memo);
	else getnam (strMemo, memo);
	CString cstrMemo = strMemo;
	cstrMemo.Replace("\n", " <br>\n");
    fputs_ (cstrMemo.GetBuffer (1), _file [dev_out], _fstato [dev_out], cstrMemo.GetLength());
}

void WriteHeader ()
{
	WriteHtml ("<html>\n");
	WriteHtml ("<head>\n");
	WriteHtml ("</head>\n");
	WriteHtml ("<body>\n");
}

void WriteFooter ()
{
	WriteHtml ("</body>\n");
	WriteHtml ("</html>\n");
}

void WriteBreak ()
{
	WriteHtml ("<br>\n");
}

//void WriteReference (node logoWord)
void WriteReference (node logoWord, CString delimiters)
{
	char text [MAXCHARS];
	MemScrivi (text, logoWord);
	CString source = text;
//	CString delimiters = ", ";
	CString ref;
	int lSource, lRef;
	int refCount = 0;
	while ((lSource = source.GetLength()) > 0) {
		ref = source.SpanExcluding (delimiters);
		if ((lRef = ref.GetLength()) > 0) {
			if (refCount++ > 0)
				WriteHtml (delimiters);
#ifdef OLD_REF
			WriteHtml ("<a href='iperlogo:vocimostra pri [");
			WriteHtml (ref);
			WriteHtml ("]'>");
#else // OLD_REF
			WriteHtml ("<a href='iperlogo:vocimostra «");
			WriteHtml (ref);
			WriteHtml ("»'>");
#endif // OLD_REF
			WriteHtml (ref);
			WriteHtml ("</a>");
			source = source.Right(lSource - lRef);
		}
		source.TrimLeft (delimiters);
	}
}

node DocList (char* helpName)
{
	// esegue la query su file .mdb
//	CString dbName = WorkDirectory;
//	dbName = dbName + "\\aiuti\\iperlogo.mdb";
	CString dbName = execDirectory + "\\aiuti\\iperlogo.mdb";
	CString sql;
	sql = "SELECT * FROM (glossario";
	sql = sql + " LEFT JOIN categorie ON categorie.IDcategoria = glossario.cat)";
	sql = sql + " LEFT JOIN ambienti ON ambienti.IDambiente = glossario.env";
	sql = sql + " WHERE ita='";
	sql = sql + helpName + "'";
	return DaoQuery (dbName, sql, TRUE, FALSE);
}

// node Glossary (char* likeName, node cat, node env)
node Glossary (char* likeName, char* strCat, char* strEnv,
	int noAlias, int soloSuperlogo, int soloIperlogo)
{
	// esegue la query su file .mdb
//	CString dbName = WorkDirectory;
//	dbName = dbName + "\\aiuti\\iperlogo.mdb";
	CString dbName = execDirectory + "\\aiuti\\iperlogo.mdb";

	CString sql = "SELECT ita FROM glossario";
	sql = sql + " WHERE IL=True AND ita LIKE '" + likeName + "'";
	if (strlen (strCat) != 0)
		sql = sql + " AND cat='" + strCat + "'";
	if (strlen (strEnv) != 0)
		sql = sql + " AND env='" + strEnv + "'";
	if (noAlias)
		sql = sql + " AND cat<>'a'";
	if (soloSuperlogo)
		sql = sql + " AND SL=True";
	if (soloIperlogo)
		sql = sql + " AND SL=False";
	return DaoQuery (dbName, sql, FALSE, TRUE);
}

void PR_DOCLIST ()
{
	char helpName [MAXCHARS];
	getnam (helpName, args[0]);
	risultato = DocList (helpName);
}

void PR_GLOSSARY ()
{
	char likeName [MAXCHARS] = "*";
	char strCat [MAXCHARS] = "";
	char strEnv [MAXCHARS] = "";
	int noAlias = TRUE;
	int soloSuperlogo = FALSE;
	int soloIperlogo = FALSE;

	if (n_argomenti > 0)
		getnam (likeName, args[0]);
	if (n_argomenti > 1)
		getnam (strCat, args[1]);
	if (n_argomenti > 2)
		getnam (strEnv, args[2]);
	if (n_argomenti > 3)
		noAlias = bool_ (args[3]);
	if (n_argomenti > 4)
		soloSuperlogo = bool_ (args[4]);
	if (n_argomenti > 5)
		soloIperlogo = bool_ (args[5]);

	int likeLen = strlen (likeName);
	if ((likeLen == 0) || (likeName[likeLen-1] != '*'))
		strcat (likeName, "*");

	risultato =
		Glossary (likeName, strCat, strEnv,
				noAlias, soloSuperlogo, soloIperlogo);
}

//int HelpOpen (char* windowName)
int HelpOpen (CString fileName)
{
//	CString htmlName = WorkDirectory;
//	htmlName = htmlName + "\\aiuti\\" + fileName + ".htm";
//	CString htmlName = execDirectory + "\\aiuti\\" + fileName + ".htm";
	CString htmlName = execDirectory + tempDir + fileName + ".htm";

	// salva il dispositivo di output corrente
	int devSave = dev_out;

	// crea il file HTML
	int devHelp = f_apri_s (htmlName, _O_WRONLY | _O_CREAT | _O_TRUNC /*| _O_RAW*/, _S_IWRITE | _S_IREAD);
	dev_out = devHelp;

	// scrive lo header
	WriteHeader ();

	return devSave;
}

void HelpClose (int devSave)
{
	// scrive il footer
	WriteFooter ();

	// chiude il file HTML
	int status = f_chiudi (dev_out);

	// ripristina il dispositivo di output originario
	dev_out = devSave;
}

void ShowWindowByDev (int dev);
#ifdef _WEB
void HelpShow (char* windowName, CString fileName)
{
//	CString htmlName = WorkDirectory;
//	htmlName = htmlName + "\\aiuti\\" + fileName + ".htm";
//	CString htmlName = execDirectory + "\\aiuti\\" + fileName + ".htm";
	CString htmlName = execDirectory + tempDir + fileName + ".htm";

	// crea finestra di browser se necessario
	int devBrowser;
	node browserName = metti_sc (windowName);
	devBrowser = nome2dev (browserName);
	if ((devBrowser < 0) || (! (_fstato [devBrowser] & O_BROWSER)))
		devBrowser = apri_browser (browserName, NULLP);

	// mostra su browser il contenuto del file HTML
	if (devBrowser > 0) {
//		ShowWindowByDev (devBrowser);
		BrowserFileOpen (dev2handle (devBrowser), htmlName);
	}
}

int HeadingOpenOrContinue (int n)
{
	if (n == 0)
		WriteHtml ("<h3 align=center> ");
	else
		WriteHtml (", ");
	return ++n;
}

void HeadingClose (int n)
{
	if (n != 0)
		WriteHtml (" </h3>");
}

void ShowGlossary (char* likeName, char* strCat, char* strEnv,
	int noAlias, int soloSuperlogo, int soloIperlogo)
{
//	CString dbName = WorkDirectory;
//	dbName = dbName + "\\aiuti\\iperlogo.mdb";
	CString dbName = execDirectory + "\\aiuti\\iperlogo.mdb";

	char nameCat [MAXCHARS] = "";
	char nameEnv [MAXCHARS] = "";
	int likeLen = strlen (likeName);
	if ((likeLen == 0) || (likeName[likeLen-1] != '*')) {
		strcat (likeName, "*");
		++likeLen;
	}
	if (strlen (strCat) != 0) {
		CString sqlCat = "SELECT nome FROM categorie WHERE IDcategoria='";
			sqlCat = sqlCat + strCat + "'";
		node listCat = DaoQuery (dbName, sqlCat, FALSE, TRUE);
		pacstr (nameCat, nome (car (listCat)));
	}
	if (strlen (strEnv) != 0) {
		CString sqlEnv = "SELECT nome FROM ambienti WHERE IDambiente='";
			sqlEnv = sqlEnv + strEnv + "'";
		node listEnv = DaoQuery (dbName, sqlEnv, FALSE, TRUE);
		pacstr (nameEnv, nome (car (listEnv)));
	}

	node glossList =
		Glossary (likeName, strCat, strEnv,
				noAlias, soloSuperlogo, soloIperlogo);

	char suffix [8];
	sprintf(suffix, "%d", nGlossario++);
	if (nGlossario >= maxGlossario)
		nGlossario = 0;
	CString fileName = strGlossary;
	fileName += suffix;
	int devSave = HelpOpen (fileName);

	WriteHtml ("<h2 align=center> <font color='#FF0000'> ");
	WriteHtml ("Indice del Glossario di Iperlogo");
	WriteHtml (" </font> </h2>");

	int n = 0;
	if (strlen (strCat) != 0) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("categoria: ");
		WriteHtml (nameCat);
	}
	if (strlen (strEnv) != 0) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("ambiente: ");
		WriteHtml (nameEnv);
	}
	if (likeLen > 1) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("termini come ");
		WriteHtml (likeName);
	}
	if (noAlias) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("esclusi gli alias");
	}
	if (soloSuperlogo) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("solo Superlogo");
	}
	if (soloIperlogo) {
		n = HeadingOpenOrContinue (n);
		WriteHtml ("solo novità Iperlogo");
	}
	HeadingClose (n);

	node glossName;
	int termCount = 0;
	char strCount [8];
	while (glossList != NULLP) {
		glossName = car (glossList);
		glossList = cdr (glossList);

		sprintf(strCount, "%d", ++termCount);
		WriteHtml (strCount);
		WriteHtml (". ");
//		WriteReference (glossName);
		WriteReference (glossName, CString(""));
		WriteBreak ();
	}

	HelpClose (devSave);
	HelpShow (strGlossary, fileName);
}

void PR_SHOWGLOSSARY ()
{
	char likeName [MAXCHARS] = "*";
	char strCat [MAXCHARS] = "";
	char strEnv [MAXCHARS] = "";
	int noAlias = TRUE;
	int soloSuperlogo = FALSE;
	int soloIperlogo = FALSE;

	if (n_argomenti > 0)
		getnam (likeName, args[0]);
	if (n_argomenti > 1)
		getnam (strCat, args[1]);
	if (n_argomenti > 2)
		getnam (strEnv, args[2]);
	if (n_argomenti > 3)
		noAlias = bool_ (args[3]);
	if (n_argomenti > 4)
		soloSuperlogo = bool_ (args[4]);
	if (n_argomenti > 5)
		soloIperlogo = bool_ (args[5]);

	ShowGlossary (likeName, strCat, strEnv,
					noAlias, soloSuperlogo, soloIperlogo);

}

void PR_SHOWHELP ()
{
	char helpName [MAXCHARS];
	getnam (helpName, args[0]);
	node docList = DocList (helpName);

	char suffix [8];
	sprintf(suffix, "%d", nGlossario++);
	if (nGlossario >= maxGlossario)
		nGlossario = 0;
	CString fileName = strGlossary;
	fileName += suffix;
	int devSave = HelpOpen (fileName);

	WriteHtml ("<h2 align=center> <font color='#FF0000'> ");
	WriteHtml ("Voci del Glossario di Iperlogo");
	WriteHtml (" </font> </h2>");

	// interna nomi delle colonne
	node name_ita = metti_sc ("ita");
	node name_eng = metti_sc ("eng");
	node name_cat = metti_sc ("categorie.nome");
	node name_env = metti_sc ("ambienti.nome");
	node name_abr = metti_sc ("abr");
	node name_ext = metti_sc ("ext");
	node name_desc = metti_sc ("desc");
	node name_synt1 = metti_sc ("synt1");
	node name_synt2 = metti_sc ("synt2");
	node name_es1 = metti_sc ("es1");
	node name_es2 = metti_sc ("es2");
	node name_see = metti_sc ("see");
	node name_note = metti_sc ("note");
	node name_ver = metti_sc ("ver");

	while (docList != NULLP) {
		node doc = car (docList);

		// estrae valori delle colonne
		node val_cat = _assoc (name_cat, doc);
		node val_env = _assoc (name_env, doc);
		node val_ita = _assoc (name_ita, doc);
		node val_eng = _assoc (name_eng, doc);
		node val_abr = _assoc (name_abr, doc);
		node val_ext = _assoc (name_ext, doc);
		node val_desc = _assoc (name_desc, doc);
		node val_synt1 = _assoc (name_synt1, doc);
		node val_synt2 = _assoc (name_synt2, doc);
		node val_es1 = _assoc (name_es1, doc);
		node val_es2 = _assoc (name_es2, doc);
		node val_see = _assoc (name_see, doc);
		node val_note = _assoc (name_note, doc);
		node val_ver = _assoc (name_ver, doc);

		// nome italiano
		WriteHtml ("<h3> <font color='#00FF00'>");
		WriteHtml (helpName);
		WriteHtml ("</font> </h3>\n");
		// categoria di parola
		if (val_cat != NULLP) {
			WriteHtml ("<u>Categoria</u> : \n");
			_scrivi (cdr (val_cat));
			WriteBreak ();
		}
		// ambiente di Iperlogo
		if (val_env != NULLP) {
			WriteHtml ("<u>Ambiente</u> : \n");
			_scrivi (cdr (val_env));
			WriteBreak ();
		}
		// forma estesa
		if (val_ext != NULLP) {
			WriteHtml ("<u>Forma estesa</u> : \n");
//			WriteReference (cdr (val_ext));
			WriteReference (cdr (val_ext), CString(""));
			WriteBreak ();
		}
		// forma abbreviata
		if (val_abr != NULLP) {
			WriteHtml ("<u>Forma abbreviata</u> : \n");
			_scrivi (cdr (val_abr));
			WriteBreak ();
		}
		// nome Inglese
		if (val_eng != NULLP) {
			WriteHtml ("<u>Inglese</u> : \n");
			_scrivi (cdr (val_eng));
			WriteBreak ();
		}
		// sintassi 1
		if (val_synt1 != NULLP) {
			WriteHtml ("<u>Sintassi</u> : \n");
			WriteHtml ("<b>");
			_scrivi (cdr (val_synt1));
			WriteHtml ("</b>");
			WriteBreak ();
		}
		// sintassi 2
		if (val_synt2 != NULLP) {
			WriteHtml ("<u>Sintassi 2 </u> : \n");
			WriteHtml ("<b>");
			_scrivi (cdr (val_synt2));
			WriteHtml ("</b>");
			WriteBreak ();
		}
		// descrizione
		if (val_desc != NULLP) {
			WriteHtml ("<u>Descrizione</u> : <br>\n");
			WriteMemo (cdr (val_desc));
			WriteBreak ();
		}
		// esempio 1
		if (val_es1 != NULLP) {
			WriteHtml ("<u>Esempio</u> : <br>\n");
			WriteMemo (cdr (val_es1));
			WriteBreak ();
		}
		// esempio 2
		if (val_es2 != NULLP) {
			WriteHtml ("<u>Altro esempio</u> : <br>\n");
			WriteMemo (cdr (val_es2));
			WriteBreak ();
		}
		// vedi anche
		if (val_see != NULLP) {
			WriteHtml ("<u>Vedi anche</u> : \n");
//			WriteReference (cdr (val_see));
			WriteReference (cdr (val_see), CString(", "));
			WriteBreak ();
		}
		// versione
		if (val_ver != NULLP) {
			WriteHtml ("<u>Voce presente da versione</u> : \n");
			_scrivi (car (cdr (val_ver)));
			WriteBreak ();
		}
		// note
		if (val_note != NULLP) {
			WriteHtml ("<u>Note</u> : <br>\n");
			WriteMemo (cdr (val_note));
			WriteBreak ();
		}

		docList = cdr (docList);
	}

	HelpClose (devSave);
	HelpShow (strGlossary, fileName);
}
#endif _WEB
