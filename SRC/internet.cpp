/*INTERNET.CPP	G. Toffoli

040309	finito recupero
040229	reiniziato funzioni FTP dopo perdita file
040220	funzioni FTP	
000131	ritoccato Albero in assenza di vero filtro
000118	generalizzato Catalogo, con 2 nuovi argomenti
991217	aggiunto Albero
980831	eliminato extern "C" per conversione a C++ dei moduli C
980825	prima versione per reimplementazione di CATALOGO
		piccola correzione a f_chiudu
*/

#include "stdafx.h"

#include "logo.h"

#include "ilcpp.h"

#ifdef OLD_CATALOGO

void Catalogo (char *s, node *pRis)
{
    CFileFind finder;
//	CString fileName;
    BOOL bWorking = finder.FindFile(s);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		libera_2 ();
		*pRis = lstcat (*pRis, cons (logstr (finder.GetFileName()), NULLP, LIST));
    }
//	finder.Close();
}

#else // OLD_CATALOGO

void Catalogo (char *s, node *pRis, int noDirectory, int noFile)
{
    CFileFind finder;
    BOOL bWorking = finder.FindFile(s);
    while (bWorking)
    {
		bWorking = finder.FindNextFile();
		if (   ((! noDirectory) || (! finder.MatchesMask(FILE_ATTRIBUTE_DIRECTORY)))
			&& ((! noFile) || finder.MatchesMask(FILE_ATTRIBUTE_DIRECTORY)))
		{
			libera_2 ();
			*pRis = lstcat (*pRis, cons (logstr (finder.GetFileName()), NULLP, LIST));
		}
    }
//	finder.Close();
}

#endif // OLD_CATALOGO

// accoda alla (cella di) lista passata come argomento
// elementi di lista corrispondenti agli archivi nel percorso/pattern corrente;
// tratta ricorsivamente gli archivi che sono sottocartelle
void Albero (CString findPath, CString findPattern, node leftmostNode)
{
    CFileFind finder1, finder2;
	CString fileName;
	node logoFileName, listElement, previousLeftmostNode;
    BOOL bWorking;
//> 000131
	// é specificato effettivamente un filtro ?
	BOOL isFilter = ((findPattern.GetLength() > 0) && (findPattern.Compare("*.*") != 0));
//<
	if (findPath.GetLength() > 0)
		findPath += "\\";
	// archivi il cui nome soddisfa il pattern
    bWorking = finder1.FindFile(findPath + findPattern);
    while (bWorking)
    {
        bWorking = finder1.FindNextFile();
//		if (finder1.MatchesMask(FILE_ATTRIBUTE_ARCHIVE)) {
		if (! finder1.MatchesMask(FILE_ATTRIBUTE_DIRECTORY)) {
			libera_2 ();
			fileName = finder1.GetFileName();
			logoFileName = logstr (fileName);
			listElement = cons (logoFileName, NULLP, LIST);
			leftmostNode = putcdr (leftmostNode, listElement);
		}
    }
//	finder1.Close();
	// ricorsione su sottocartelle
    bWorking = finder2.FindFile(findPath + "*.*");
    while (bWorking)
	{
        bWorking = finder2.FindNextFile();
		if (finder2.MatchesMask(FILE_ATTRIBUTE_DIRECTORY) && (! finder2.IsDots())) {
			libera_2 ();
			fileName = finder2.GetFileName();
			logoFileName = logstr (fileName);
			listElement = cons (logoFileName, NULLP, LIST);
			previousLeftmostNode = leftmostNode;
			leftmostNode = putcdr (leftmostNode, cons (listElement, NULLP, LIST));
			Albero (findPath + fileName, findPattern, listElement);
//> 000131
//			if (lstlen (listElement) < 2) {
			if (isFilter && lstlen (listElement) < 2) {
//<
				leftmostNode = previousLeftmostNode;
				putcdr (leftmostNode, NULLP);
			}
		}
    }
//	finder2.Close();
}

//>040229

#include <afxinet.h>
void LogoToText (node logo, char* pText);

CInternetSession *pInternetSession = NULL;
#define MAX_FTP_CONNECTIONS 1
CFtpConnection *pFtpConnections[MAX_FTP_CONNECTIONS+1] = { NULL, NULL };
int iFtpConnection = 1; // indice default e, per ora, unico

void PR_FTP_APRI()
{
#ifdef CATCH_ERROR
	char server[MAX_CHARS], userName[MAX_CHARS], password[MAX_CHARS];
	LPCTSTR pstrServer = NULL, pstrUserName = NULL, pstrPassword = NULL;
	INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER;
	BOOL bPassive = FALSE;
	LogoToText (arg_1, server);
	pstrServer = &server[0];
	if (n_argomenti > 1) {
		LogoToText (arg_2, userName);
		pstrUserName = &userName[0];
		if (n_argomenti > 2) {
			LogoToText (arg_3, password);
			pstrPassword = &password[0];
		}
	}
	if (! pInternetSession) {
		LPCTSTR pstrAgent = "Iperlogo";
		DWORD dwContext = 1;
		DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
		LPCTSTR pstrProxyName = NULL;
		LPCTSTR pstrProxyBypass = NULL;
		DWORD dwFlags = 0;
		pInternetSession =
			new CInternetSession(pstrAgent, dwContext, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags);
	}
	if (pFtpConnections[iFtpConnection])
		{ err1 (15); return ;}
	CFtpConnection *pFtpConnection;
	try
	{
		pFtpConnection =
			pInternetSession->GetFtpConnection(pstrServer, pstrUserName, pstrPassword, nPort, bPassive);
	}
	catch(CInternetException internetE)
	{
		err1 (38);  return ;
	}
	pFtpConnections[iFtpConnection] = pFtpConnection;
#endif
}

void PR_FTP_CHIUDI()
{
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return ;}
	CInternetSession *pInternetSession = pFtpConnection->GetSession();
	pFtpConnection->Close();
	pFtpConnections[iFtpConnection] = NULL;
	delete (pInternetSession);
}

void PR_FTP_GET()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char local_pathName [MAXCHARS], remote_pathName [MAXCHARS];
	BOOL bFailIfExists = TRUE;
	DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL;
	DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY;
	DWORD dwContext = 1;
	LogoToText (arg_1, remote_pathName);
	if (n_argomenti >= 2)
		if (mettip(arg_2) == kw [37])
			dwFlags = FTP_TRANSFER_TYPE_BINARY;
		else
			dwFlags = FTP_TRANSFER_TYPE_ASCII;
	if (n_argomenti >= 3)
		LogoToText (arg_3, local_pathName);
	else
		strcpy (local_pathName, remote_pathName);
	if (n_argomenti >= 4)
		bFailIfExists = bool_(args[3]);
	if (n_argomenti >= 5)
		dwAttributes = num_int(args[4]);
	if (! pFtpConnection->GetFile(local_pathName, remote_pathName, bFailIfExists, dwAttributes, dwFlags, dwContext))
		{ err1 (38); return; }
}


void PR_FTP_PUT()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char local_pathName [MAXCHARS], remote_pathName [MAXCHARS];
	DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY;
	DWORD dwContext = 1;
	LogoToText (arg_1, local_pathName);
	if (n_argomenti >= 2)
		if (arg_2 == mettip(kw [37]))
			dwFlags = FTP_TRANSFER_TYPE_BINARY;
		else
			dwFlags = FTP_TRANSFER_TYPE_ASCII;
	if (n_argomenti >= 3)
		LogoToText (arg_3, remote_pathName);
	else
		strcpy (remote_pathName, local_pathName);
	if (! pFtpConnection->PutFile(local_pathName, remote_pathName, dwFlags, dwContext))
		{ err1 (38); return; }
}


void PR_FTP_CANCEARC()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char pathName [MAXCHARS];
	LogoToText (arg_1, pathName);
	if (! pFtpConnection->Remove(pathName))
		{ err1 (38); return; }
}


void PR_FTP_RINOMINAARC()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char new_pathName [MAXCHARS], old_pathName [MAXCHARS];
	LogoToText (arg_1, new_pathName);
	LogoToText (arg_2, old_pathName);
	if (! pFtpConnection->Rename(old_pathName, new_pathName))
		{ err1 (38); return; }
}


void PR_FTP_CARTELLA()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	CString pathName;
	if (! pFtpConnection->GetCurrentDirectory(pathName))
		{ err1 (38); return; }
}


void PR_FTP_CAMBIACARTELLA()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char pathName [MAXCHARS];
	LogoToText (arg_1, pathName);
	if (! pFtpConnection->SetCurrentDirectory(pathName))
		{ err1 (38); return; }
}


void PR_FTP_CREACARTELLA()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char pathName [MAXCHARS];
	LogoToText (arg_1, pathName);
	if (! pFtpConnection->CreateDirectory(pathName))
		{ err1 (38); return; }
}


void PR_FTP_RIMUOVICARTELLA()
{
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return; }
	// eseguo
	char pathName [MAXCHARS];
	LogoToText (arg_1, pathName);
	if (! pFtpConnection->RemoveDirectory(pathName))
		{ err1 (38); return; }
}

void FtpCatalogo (CFtpConnection *pFtpConnection, char *pathName, node *pRis, int noDirectory, int noFile)
{
	DWORD dwContext = 1;
    CFtpFileFind *pFinder =
		new CFtpFileFind(pFtpConnection, dwContext);
    BOOL bWorking = pFinder->FindFile(pathName);
    BOOL bDirectory;
    while (bWorking)
    {
		bWorking = pFinder->FindNextFile();
		if (noDirectory || noFile)
			bDirectory = pFinder->IsDirectory();
		if (   ((! noDirectory) || bDirectory)
			&& ((! noFile) || (! bDirectory)))
		{
			libera_2 ();
			*pRis = lstcat (*pRis, cons (logstr (pFinder->GetFileName()), NULLP, LIST));
		}
    }
	pFinder->Close();
	delete(pFinder);
}


void PR_FTP_CATALOGO()
{
	// controllo argomento
	char pathName [MAXCHARS];
	if (n_argomenti == 0)
		strcpy (pathName, "*.*");
	else if (! nomearc (arg_1))
		{err_arg1 (); return;}
	LogoToText (arg_1, pathName);
	// controllo esistenza connessione
	CFtpConnection *pFtpConnection = pFtpConnections[iFtpConnection];
	if (! pFtpConnection)
		{ err1 (15); return ;}
	// eseguo
	R = NULLP;
	FtpCatalogo (pFtpConnection, pathName, &R, 0, 0);
	risultato = R;
}


void PR_FTP_ALBERO()
{
}

 
void PR_FTP_ATTR()
{
}


/* INTERNET.CPP */
