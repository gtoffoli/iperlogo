/* SHORTCUT.CPP	G. Toffoli

000117	prima versione
*/

//>000117
#define CINTERFACE
//<
#include "stdafx.h"

// CreateLink - uses the shell's IShellLink and IPersistFile interfaces 
//   to create and store a shortcut to the specified object. 
// Returns the result of calling the member functions of the interfaces. 
// lpszPathObj - address of a buffer containing the path of the object. 
// lpszPathLink - address of a buffer containing the path where the 
//   shell link is to be stored. 
// lpszDesc - address of a buffer containing the description of the 
//   shell link. 
 
HRESULT CreateLink(LPCSTR lpszPathObj, 
    LPSTR lpszPathLink, LPSTR lpszDesc) 
{ 
    HRESULT hres; 
    IShellLink* psl; 
 
    // Get a pointer to the IShellLink interface
//>000117
//    hres = CoCreateInstance(&CLSID_ShellLink, NULL,
//        CLSCTX_INPROC_SERVER, &IID_IShellLink, &psl);
    hres = CoCreateInstance(CLSID_ShellLink, NULL, 
        CLSCTX_INPROC_SERVER, IID_IShellLink, (void **) &psl);
//<

    if (SUCCEEDED(hres)) {
        IPersistFile* ppf;
 
        // Set the path to the shortcut target and add the
        // description.
        psl->lpVtbl->SetPath(psl, lpszPathObj);
        psl->lpVtbl->SetDescription(psl, lpszDesc);

       // Query IShellLink for the IPersistFile interface for saving the
       // shortcut in persistent storage.
//>000117
//        hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile,
//            &ppf);
        hres = psl->lpVtbl->QueryInterface(psl, IID_IPersistFile,
            (void **) &ppf);
//<

        if (SUCCEEDED(hres)) {
            WORD wsz[MAX_PATH];

            // Ensure that the string is ANSI.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1,
                wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save.
            hres = ppf->lpVtbl->Save(ppf, wsz, TRUE);
            ppf->lpVtbl->Release(ppf);
        }
        psl->lpVtbl->Release(psl);
    }

    return hres;
}

#ifdef DOC

HRESULT CreateShortCut::CreateIt (LPCSTR pszShortcutFile, LPSTR pszLink, 
LPSTR pszDesc)
{
HRESULT hres;
IShellLink *psl;

// Create an IShellLink object and get a pointer to the IShellLink 
// interface (returned from CoCreateInstance).
hres = CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
IID_IShellLink, (void **)&psl);
if (SUCCEEDED (hres))
{
IPersistFile *ppf;

// Query IShellLink for the IPersistFile interface for 
// saving the shortcut in persistent storage.
hres = psl->QueryInterface (IID_IPersistFile, (void **)&ppf);
if (SUCCEEDED (hres))
{ 
WORD wsz [MAX_PATH]; // buffer for Unicode string

// Set the path to the shortcut target.
hres = psl->SetPath (pszShortcutFile);

if (! SUCCEEDED (hres))
AfxMessageBox ("SetPath failed!");

// Set the description of the shortcut.
hres = psl->SetDescription (pszDesc);

if (! SUCCEEDED (hres))
AfxMessageBox ("SetDescription failed!");

// Ensure that the string consists of ANSI characters.
MultiByteToWideChar (CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

// Save the shortcut via the IPersistFile::Save member function.
hres = ppf->Save (wsz, TRUE);

if (! SUCCEEDED (hres))
AfxMessageBox ("Save failed!");

// Release the pointer to IPersistFile.
ppf->Release ();
}
// Release the pointer to IShellLink.
psl->Release ();
}
return hres;
} 

#endif // DOC

