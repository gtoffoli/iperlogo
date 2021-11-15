// CDib.h : header file		G. Toffoli
/*

  980223	new module for managing DIBs and related palettes
*/

int ColorDistance (COLORREF color1, COLORREF color2);

void CColorDialogInitialize (CColorDialog * pColorDialog);
void CColorDialogFinalize (CColorDialog * pColorDialog);

CPalette* CPaletteFromStock ();
int CPaletteNearestColor (CPalette* pPalette, COLORREF inColor, COLORREF* outColor);
int CPaletteAddColor (CPalette* pPalette, COLORREF newColor);
// COLORREF ColorInPalette (COLORREF inColor, int tolerance);
int ColorInPalette (CDC* pDC, COLORREF inColor, COLORREF* outColor, int tolerance);
COLORREF ColorOrIndex (COLORREF inColor);
int MapPalette (CPalette *pFrom, CPalette *pTo);

HPALETTE MakeDibPalette (BITMAPINFOHEADER* lpInfo, CDC* pDC);
//HGLOBAL BitmapToSharedDib (HBITMAP hBitmap);
HGLOBAL BitmapToSharedDib (CDC* pDC, HBITMAP hBitmap);
int DibExplicitInfo (BITMAPINFOHEADER* pDib);
int DibNormalize (BITMAPINFOHEADER* pDib);
CBitmap* DibToBitmap (BITMAPINFOHEADER* pDib, CDC* pDC, CPalette* pTargetPalette);
CBitmap*  DibRealize (BITMAPINFOHEADER* pDib, CDC* pDC, CPalette* pPalette);
HGLOBAL ColorToDib (CDC* pDC, COLORREF inColor, int bitCount);
HGLOBAL ReadDib (int hFile);
CBitmap* LoadBitMapFromFile (int hFile, CDC* pDC, CPalette *pTargetPalette);
BOOL SaveBitMaptoFile (int hFile, CBitmap *pBitmap);

