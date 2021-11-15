/* SHELL.CPP	G. Toffoli

010606	PR_SHELL_COMMANDLINE estensione [tipocomando]
000415	corretto PR_SHELL_GETFOLDERPATH per assenza di profili
000221	prima versione da esempio in MSDN Library - January 2000
		Platform SDK - User Interface Services - Windows Shell
		Shell Programmers Guide - Intermediate Shell Techniques - Creating Shortcuts
*/

#include "stdafx.h"

#include "objbase.h"
#include "shlobj.h"

#include "logo.h"
#include "ilcpp.h"

// CreateLink - uses the shell's IShellLink and IPersistFile interfaces 
//   to create and store a shortcut to the specified object. 
// Returns the result of calling the member functions of the interfaces. 
// lpszPathObj - address of a buffer containing the path of the object. 
// lpszPathLink - address of a buffer containing the path where the 
//   shell link is to be stored. 
// lpszDesc - address of a buffer containing the description of the 
//   shell link. 

HRESULT LinkInitialize ()
{
    HRESULT hres;
	void* pvReserved = 0;	// Reserved
//	DWORD dwCoInit = COINIT_APARTMENTTHREADED;	// COINIT value: Apartment model
//	hres = CoInitializeEx(pvReserved, dwCoInit);
	hres = CoInitialize(pvReserved);
	return hres;
}

void LinkUninitialize ()
{
	CoUninitialize();
}

//> 000221 GT (estensioni)
// HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc)
int CreateLink(
	LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc,
	LPCSTR lpszWorkDir, LPCSTR lpszArgs, LPCSTR lpszIconPath, int iIcon)
//<
{

    HRESULT hres;

//> 000221 GT
	hres = LinkInitialize ();
	if (! SUCCEEDED(hres))
		return (int) hres;
//<

	IShellLink* psl;

	// Get a pointer to the IShellLink interface.
	hres = CoCreateInstance(CLSID_ShellLink, NULL,
//> 000221 GT (dava errore)
//		CLSCTX_INPROC_SERVER, IID_IShellLink, &psl);
		CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl);
//<
	if (SUCCEEDED(hres)) {
		IPersistFile* ppf;

		// Set the path to the shortcut target and add the description.
		if (SUCCEEDED(hres))
			hres = psl->SetPath(lpszPathObj);
		if (SUCCEEDED(hres))
			hres = psl->SetDescription(lpszDesc);

//> 000221 GT : estensioni
//	HRESULT STDMETHODCALLTYPE SetWorkingDirectory(LPCSTR pszDir);
//	HRESULT STDMETHODCALLTYPE SetArguments(LPCSTR pszArgs);
//	HRESULT STDMETHODCALLTYPE SetIconLocation(LPCSTR pszIconPath, int iIcon);
		if (SUCCEEDED(hres))
			if (lpszWorkDir)
				hres = psl->SetWorkingDirectory(lpszWorkDir);
		if (SUCCEEDED(hres))
			if (lpszArgs)
				hres = psl->SetArguments(lpszArgs);
		if (SUCCEEDED(hres))
			if (lpszIconPath)
				hres = psl->SetIconLocation(lpszIconPath, iIcon);
//<

		// Query IShellLink for the IPersistFile interface for saving the 
		// shortcut in persistent storage.
//> 000221 GT (dava errore)
//		hres = psl->QueryInterface(&IID_IPersistFile, &ppf);
		if (SUCCEEDED(hres))
			hres = psl->QueryInterface(IID_IPersistFile, (void **) &ppf);
//<
		if (SUCCEEDED(hres)) { 
			WCHAR wsz[MAX_PATH]; 

			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

			// Save the link by calling IPersistFile::Save.
			hres = ppf->Save(wsz, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
//> 000221 GT
	LinkUninitialize ();
//<
	return (int) hres;
}

void PR_SHELL_LINKCREATE ()
{
	char szPathObj[MAXCHARS], szPathLink[MAXCHARS], szDesc[MAXCHARS];
	char szWorkDir[MAXCHARS], szArgs[MAXCHARS], szIconPath[MAXCHARS];

	char *lpszWorkDir = NULL, *lpszArgs = NULL, *lpszIconPath = NULL;
	int iIcon = 0;

	pacstr (szPathObj, nome (args[0]));
	pacstr (szPathLink, nome (args[1]));
	pacstr (szDesc, nome (args[2]));
	if (n_argomenti > 3) {
		pacstr (szWorkDir, nome (args[3]));
		lpszWorkDir = &szWorkDir[0];
		if (n_argomenti > 4) {
			pacstr (szArgs, nome (args[4]));
			lpszArgs = &szArgs[0];
			if (n_argomenti > 5) {
				pacstr (szIconPath, nome (args[5]));
				lpszIconPath = &szIconPath[0];
				if (n_argomenti > 6)
					iIcon = num_int(args[6]);
			}
		}
	}

	int result = CreateLink(
		szPathObj, szPathLink, szDesc,
		lpszWorkDir, lpszArgs, lpszIconPath, iIcon);

	risultato = int_num(result);
}

void PR_SHELL_GETFOLDERPATH ()
{
// Get the directory for Windows Desktop. This is
// stored in the Registry under HKEY_CURRENT_USER\Software\
// Microsoft\Windows\CurrentVersion\Explorer\Shell Folders\Desktop.

	int intCommon = num_int(args[0]);
	int intFolder = num_int(args[1]);

  	HKEY startKey;
	CString foldername;

	if (intCommon == 0) {
		startKey = HKEY_CURRENT_USER;
		if (intFolder == 0)
			foldername = "Desktop";
		else
			foldername = "Programs";
	}
	else {
		startKey = HKEY_LOCAL_MACHINE;
		if (intFolder == 0)
			foldername = "Common Desktop";
		else
			foldername = "Common Programs";
	}

//> 000415
//	char szLink[MAX_PATH];
	char szLink[MAX_PATH] = "";
//<
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize = MAX_PATH;
	if (RegOpenKeyEx(startKey, 
		"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders", 
		0,KEY_QUERY_VALUE,
		&hCU) == ERROR_SUCCESS)
	{
		RegQueryValueEx( hCU, foldername, NULL,
			&lpType, (unsigned char *)&szLink, &ulSize);
		RegCloseKey(hCU);
//> 000415
//		risultato = logstr(szLink);
//<
	}
//> 000415
//	else
//		err1 (38);
	risultato = logstr(szLink);
//<
}

// converte estensione di nomefile in linea di comando completa di percorso
// il secondo argomento, facoltativo, indica il tipo di comando ("open" è il default)
void PR_SHELL_COMMANDLINE ()
{
	risultato = NULLP;
	char extension[MAX_PATH] = ".";
	char commandname[MAX_PATH] = "open";
	char application[MAX_PATH] = "";
	CString subKey;
	char commandline[MAX_PATH] = "";
	getnam(&extension[1], arg_1);
	if (n_argomenti >= 2)
		getnam(commandname, arg_2);
	HKEY hCU;
	DWORD lpType;
	ULONG ulSize;
	HKEY startKey = HKEY_CLASSES_ROOT;
	LPCTSTR lpValueName = NULL;
	LPDWORD lpReserved = NULL;

	if (RegOpenKeyEx(startKey, extension, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
	{
		ulSize = MAX_PATH;
		RegQueryValueEx( hCU, lpValueName, lpReserved, 
			&lpType, (unsigned char *)&application, &ulSize);
		RegCloseKey(hCU);
		if (lpType != REG_SZ) return;

		subKey = application;
		subKey += "\\shell\\";
		subKey += commandname;
		subKey +=  "\\command";
		if (RegOpenKeyEx(startKey, subKey, 0, KEY_QUERY_VALUE, &hCU) == ERROR_SUCCESS)
		{
			ulSize = MAX_PATH;
			RegQueryValueEx( hCU, lpValueName, lpReserved, 
				&lpType, (unsigned char *)&commandline, &ulSize);
			RegCloseKey(hCU);
			if (lpType != REG_SZ) return;

			risultato = logstr(commandline);
		}
	}
}
