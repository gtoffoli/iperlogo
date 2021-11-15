/* ILCONFIG.H

	980908	conversione a CPP: rieliminato extern "C"
*/

#define DllImport    __declspec( dllimport )
#define DllExport    __declspec( dllexport )

// DllExport char * __stdcall ConfigGet (int i);
// DllExport extern "C" char * __stdcall ConfigGet (int i);// 980831
DllExport char * __stdcall ConfigGet (int i);		// 980908

#define ITALIANO
