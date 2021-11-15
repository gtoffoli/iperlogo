// CDib.cpp : module file		G. Toffoli
/*
	980327	modif. ColorInPalette per NO palette; ColorOrIndex
	980306	modif. ColorInPalette; ColorToDib
	980303	ColorInPalette
	980226	modif. BitmapToSharedDib
	980225	nuove funzioni per BitLoad; gestione custom colors
	980224	here most bitmap-related functions from Turtlevw.cpp
			in ReadBitmapFromFile: DibToBitmap => DibRealize
	980223	new module for managing DIBs and related palettes
*/

#include "stdafx.h"
#include <io.h>
/// #include "..\resource.h"       // main symbols
#include "resource.h"       // main symbols
#include "CDib.h"
#include "iperlogo.h"

extern CIperlogoApp theApp;

#define NEW_DIB

void CColorDialogInitialize (CColorDialog * pColorDialog)
{
	pColorDialog->m_cc.Flags &= (~ CC_ANYCOLOR);
	pColorDialog->m_cc.lpCustColors = &(theApp.m_customColors[0]);
}

void CColorDialogFinalize (CColorDialog * pColorDialog)
{
}

// implement some "distance" metrics between 2 colors
int ColorDistance (COLORREF color1, COLORREF color2)
{
	int distance;
	int distRed, distGreen, distBlue;
	distRed =
		abs((WORD) GetRValue(color1) - (WORD) GetRValue(color2));
	distGreen =
		abs((WORD) GetGValue(color1) - (WORD) GetGValue(color2));
	distBlue =
		abs((WORD) GetBValue(color1) - (WORD) GetBValue(color2));
	distance = distRed + distGreen + distBlue;
	return distance;
}

CPalette* CPaletteFromStock ()
{
	DWORD logPalette [1+256];
	LOGPALETTE * pLogPalette = (LOGPALETTE *) &logPalette;
	PALETTEENTRY * pPalEntries = (PALETTEENTRY *)
		((char *) pLogPalette + offsetof(LOGPALETTE, palPalEntry));
	int sizePalette = sizeof(logPalette);
	memset((LPSTR) pLogPalette, 0, sizePalette);
	HPALETTE hPalette = (HPALETTE) ::GetStockObject(DEFAULT_PALETTE);
//	CPalette* pStockPalette = CPalette::FromHandle (hPalette);
	CPalette stockPalette;
	CPalette* pStockPalette = stockPalette.FromHandle (hPalette);
	int nEntries = pStockPalette->GetEntryCount();
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = nEntries;
	pStockPalette->GetPaletteEntries (0, nEntries, pPalEntries);
	CPalette* pPalette = new(CPalette);
	int result = pPalette->CreatePalette(pLogPalette);
	if (result) return pPalette;
	else {
		delete(pPalette);
		return NULL;
	}
}

// return the best substitute for inColor in the given palette
int CPaletteNearestColor (CPalette* pPalette, COLORREF inColor, COLORREF* outColor)
{
	int iNearestColor;	// index of nearest color in palette
	PALETTEENTRY entry;	// buffer for 1 palette entry

	iNearestColor = pPalette->GetNearestPaletteIndex (inColor);
	pPalette->GetPaletteEntries (iNearestColor, 1, &entry);

	*outColor = RGB(entry.peRed, entry.peGreen, entry.peBlue);
	return iNearestColor;
}

// put a color into a palette, after resizing it (+1);
// returns the index in the palette of the new color
int CPaletteAddColor (CPalette* pPalette, COLORREF newColor)
{
	PALETTEENTRY entry;	// buffer for 1 palette entry
	int nEntries = pPalette->GetEntryCount();
	++nEntries;
	if (! pPalette->ResizePalette(nEntries))
		return 0;
	entry.peRed = GetRValue(newColor);
	entry.peGreen = GetGValue(newColor);
	entry.peBlue = GetBValue(newColor);
	entry.peFlags = 0;
//	entry.peFlags = PC_NOCOLLAPSE;
	if (pPalette->SetPaletteEntries (nEntries-1, 1, &entry))
//		return nEntries;		// 980306
		return nEntries - 1;	//
	else
		return 0;
}

// if required color is not already in palette, and there is
// still place left, put it into the Iperlogo palette;
// else return best match
int ColorInPalette (CDC* pDC, COLORREF inColor, COLORREF* outColor, int tolerance)
{
	COLORREF color;
	int iColor = 0;

	if (theApp.rastercaps & RC_PALETTE) {
		int iNewColor;
		iColor =
			CPaletteNearestColor (theApp.m_pPalette, inColor, &color);
		int colorDistance = ColorDistance (inColor, color);
		if (colorDistance > tolerance) {
			iNewColor = CPaletteAddColor (theApp.m_pPalette, inColor);
			if (iNewColor > 0) {
				iColor = iNewColor;
				color = inColor;
			}
		}
	}
	else
		color = pDC->GetNearestColor(inColor);
	if (outColor != NULL)	// aggiunto 980313
		*outColor = color;
	return iColor;
}

COLORREF ColorOrIndex (COLORREF inColor)
{
	COLORREF outColor;
	if (theApp.rastercaps & RC_PALETTE) {
		WORD wPaletteIndex =
			theApp.m_pPalette->GetNearestPaletteIndex (inColor);
		outColor = PALETTEINDEX(wPaletteIndex);
	}
	else
		outColor = inColor;
	return outColor;
}

// substitute each RGB entry in the FROM palette
// with the nearest entry in the TO palette
int MapPalette (CPalette *pFrom, CPalette *pTo)
{
	HBITMAP hFrom = (HBITMAP) pFrom->m_hObject;
	HBITMAP hTo = (HBITMAP) pTo->m_hObject;
	int nFrom = pFrom->GetEntryCount();
	int nTo = pTo->GetEntryCount();
	COLORREF crColor;
	PALETTEENTRY entry;
	PALETTEENTRY entries[256];	// fromEntries
	pFrom->GetPaletteEntries (0, nFrom, &entries[0]);
	int iFrom, iTo;
	for (iFrom=0; iFrom<nFrom; iFrom++) {
		entry = entries[iFrom];
		crColor =	// formato : 0x00bbggrr
			RGB(entry.peRed, entry.peGreen, entry.peBlue);
		iTo = pTo->GetNearestPaletteIndex (crColor);
		pTo->GetPaletteEntries (iTo, 1, &entry);
//		entry.peFlags = PC_NOCOLLAPSE;
		pFrom->SetPaletteEntries (iFrom, 1, &entry);
	}
	return TRUE;
}

// compute (or make explicit) some info fields;
// return size of colortable
int DibExplicitInfo (BITMAPINFOHEADER* pDib)
{
///	int maxColors = 1 << pDib->biBitCount;
	int maxColors = 1 << (pDib->biPlanes * pDib->biBitCount);
	int nColors = pDib->biClrUsed;
	if (nColors == 0)
		nColors = maxColors;
	pDib->biClrUsed = nColors;
	int sizeRow = (((pDib->biWidth * pDib->biPlanes *
			pDib->biBitCount) + 31) & ~31) >> 3;
	int sizeImage = pDib->biSizeImage;
	if (sizeImage == 0) {
		sizeImage = sizeRow * pDib->biHeight;
		pDib->biSizeImage = sizeImage;
	}
	return nColors;
}

// normalize a Dib
// by removing unused entries in the ColorTable
// and updating color indexes in the image bits
int DibNormalize (BITMAPINFOHEADER* pDib)
{
	int iRow, iColumn;
	unsigned int index;
	unsigned char* pRow;
	RGBQUAD cTab2[256];
	BOOL checkTable[256];
	int	convTable[256];

	// extract or compute some sizes and addresses
	int nColors = DibExplicitInfo (pDib);
	RGBQUAD* pColorTab =
		(RGBQUAD *) ((char*) pDib + pDib->biSize);
	int sizeRow = (((pDib->biWidth * pDib->biPlanes *
			pDib->biBitCount) + 31) & ~31) >> 3;
	int sizeImage = pDib->biSizeImage;
	char* pImage =
		(char*) pDib + pDib->biSize + nColors * sizeof(RGBQUAD);

	// build the check-table
	memset((LPSTR) &checkTable[0], 0, sizeof(checkTable));
	pRow = (unsigned char *) pImage;
	for (iRow=0; iRow<pDib->biHeight; iRow++) {
		for (iColumn=0; iColumn<pDib->biWidth; iColumn++) {
			index = pRow[iColumn];	// only valid for 8 bit (256 colors)
			++checkTable[index];
		}
		pRow += sizeRow;
	}

	// build the conversion table
	memset((LPSTR) &convTable[0], 0, sizeof(convTable));
	int nUsed = 0;
	int iColor;
	for (iColor=0; iColor<nColors; iColor++) {
		if (checkTable[iColor]) {
			convTable[iColor] = nUsed;
			++nUsed;
		}
	}
	pDib->biClrUsed = nUsed;

	// build a compacted the ColorTable
	memset((LPSTR) &cTab2[0], 0, sizeof(cTab2));
	for (iColor=0; iColor<nColors; iColor++) {
		if (checkTable[iColor])
			memcpy((char *) &(cTab2[convTable[iColor]]),
				(char *) &(pColorTab[iColor]),
				sizeof(RGBQUAD));
	}
	// move it to replace the original Colortable
	memcpy((char *) pColorTab,
			(char *) &cTab2[0],
			nUsed * sizeof(RGBQUAD));

	// convert the coding of the Pixels
	pRow = (unsigned char *) pImage;
	for (iRow=0; iRow<pDib->biHeight; iRow++) {
		for (iColumn=0; iColumn<pDib->biWidth; iColumn++) {
			index = pRow[iColumn];	// only valid for 8 bit (256 colors)
			pRow[iColumn] = convTable[index];
		}
		pRow += sizeRow;
	}
#define MOVE_PIXELS
#ifdef MOVE_PIXELS
	// move the the Pixels
	memcpy((char *) pColorTab + nUsed * sizeof(RGBQUAD),
			pImage,
			sizeImage);
#endif // MOVE_PIXELS
	return TRUE;
}


//HPALETTE PASCAL NEAR MakeDIBPalette(LPBITMAPINFOHEADER lpInfo, CDC* pDC)
HPALETTE MakeDibPalette(BITMAPINFOHEADER* lpInfo, CDC* pDC)
{
	NPLOGPALETTE npPal;
	RGBQUAD far *lpRGB;
	HPALETTE hLogPal; 
	WORD i;
	
	/* since biClrUsed field was filled during the loading of the DIB,
	** we know it contains the number of colors in the color table.
	*/
	if (lpInfo->biClrUsed) {
		npPal = (NPLOGPALETTE)LocalAlloc(LMEM_FIXED, sizeof(LOGPALETTE) + 
			(WORD)lpInfo->biClrUsed * sizeof(PALETTEENTRY));
		if (!npPal)
			return(FALSE);
		
		npPal->palVersion = 0x300;
		npPal->palNumEntries = (WORD)lpInfo->biClrUsed;
		
		/* get pointer to the color table */
		lpRGB = (RGBQUAD FAR *)((LPSTR)lpInfo + lpInfo->biSize);
		
		/* copy colors from the color table to the LogPalette structure */
//>980220
//		for (i = 0; i < (WORD)lpInfo->biClrUsed; i++, lpRGB++) {
//			npPal->palPalEntry[i].peRed = lpRGB->rgbRed;
//			npPal->palPalEntry[i].peGreen = lpRGB->rgbGreen;
//			npPal->palPalEntry[i].peBlue = lpRGB->rgbBlue;
//			npPal->palPalEntry[i].peFlags = 0;
//		}
		int NumColors = lpInfo->biClrUsed;
		int j;
		i = 0;
		PALETTEENTRY * pEntry;
		for (j = 0; j < NumColors; j++, lpRGB++) {
			PALETTEENTRY entry = * (PALETTEENTRY *) lpRGB;
			entry.peFlags = 0;
			if (* (DWORD *) &entry != 0) {
				pEntry = &(npPal->palPalEntry[i]);
				*pEntry = entry;
				pEntry->peFlags = 0;
				++i;
			}
		}
		npPal->palNumEntries = i;
//<		
		hLogPal = CreatePalette((LPLOGPALETTE)npPal);
		LocalFree((HANDLE)npPal);
		return(hLogPal);
	}
	
	/* 24-bit DIB with no color table.	return default palette.  Another
	** option would be to create a 256 color "rainbow" palette to provide
	** some good color choices.
	*/
	else
		return((HPALETTE) GetStockObject(DEFAULT_PALETTE));
}

// Da device-dependent Bitmap, crea una DIB completa di Header,
// ColorTable e array di Pixel.
//
// GetDIBits (.., address of array = NULL, ..) sintetizza la
// ColorMap tenedo conto sia dei colori veramente usati che
// della Palette selezionata nel DC; i colori nella ColorTable
// sono quindi in numero diverso da quelli nella Palette stessa
// (cioe' diverso da theApp.m_pPalette->GetEntryCount()).
// In ogni caso la dimensione della ColorTable e' quella massima
//
// La memoria globale allocata per la DIB dovra' essere rilasciata
// dopo la fine del suo uso (es: scrittura su ClipBoard o File)

HGLOBAL BitmapToSharedDib (CDC* pDC,		// device context
						   HBITMAP hBitmap) // handle di Bitmap
{
	UINT nNumColors;	// number of colors (elements of ColorTable)
	HGLOBAL	hDib = NULL;
	BITMAP bitmap;		// copy of BITMAP structure
	DWORD DibInfo [
		(sizeof(BITMAPINFOHEADER) + 512 * sizeof(RGBQUAD)) / 4];
	BITMAPINFO* pDibInfo = (BITMAPINFO *) &DibInfo[0];
	BITMAPINFOHEADER* pDibHeader = (BITMAPINFOHEADER*) pDibInfo;
	LPBITMAPINFOHEADER pDib;
	DWORD offsetDibTable;
	DWORD offsetImage;
	DWORD sizeDib;			// total DIB size
	DWORD sizeDibTable;		// ColorTable size
	DWORD sizeImage;		// Image size
	DWORD* pDibTable;
	char* pImage;
	int result;
	
	// Copy BITMAP structure from CBitmap 													 
	result = GetObject(hBitmap, sizeof(bitmap), &bitmap);
	
	// Fill structure with dimension/format section : 
	// first 6 fields need be initialized :
	memset((LPSTR) pDibHeader, 0, sizeof(BITMAPINFO));
	pDibHeader->biSize = sizeof(BITMAPINFOHEADER);
	pDibHeader->biWidth = bitmap.bmWidth;
	pDibHeader->biHeight = bitmap.bmHeight;
	pDibHeader->biBitCount = bitmap.bmPlanes * bitmap.bmBitsPixel;
	// 980407: like Ms Paint, even if working at 16 bits
	// make 24 bits Dib (no colortable)
	if (pDibHeader->biBitCount == 16) {
		pDibHeader->biBitCount = 24;
	}
	pDibHeader->biPlanes = 1;
	pDibHeader->biCompression = BI_RGB;
	result = GetDIBits(pDC->GetSafeHdc(), // handle of DC
		hBitmap,			// handle of bitmap	object
		0,					// starting scan line
		bitmap.bmHeight,	// number of scan line
		(LPSTR) NULL,
		pDibInfo,			// structure with bitmap data
		DIB_RGB_COLORS);
	
	// Compute some sizes and offsets 													 
	offsetDibTable = offsetof(BITMAPINFO, bmiColors);
	nNumColors = pDibHeader->biClrUsed;
	if (nNumColors == 0)
///		nNumColors = 1 << pDibHeader->biBitCount; // was 256
		nNumColors = 1 << (pDibHeader->biPlanes * pDibHeader->biBitCount);
	sizeDibTable = 0;			// 980403: aggiunto
	if (nNumColors <= 256)		//
		sizeDibTable = nNumColors * sizeof(RGBQUAD);
	offsetImage = offsetDibTable + sizeDibTable;
	sizeImage = pDibHeader->biSizeImage;
	sizeDib = offsetDibTable + sizeDibTable + sizeImage;
	
	// Allocate DIB and compute sections' addresses
	hDib = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeDib);
	pDib = (LPBITMAPINFOHEADER) GlobalLock(hDib);
	pDibTable = (DWORD *) ((char *) pDib + offsetDibTable);
	pImage = (char *) pDib + offsetImage;
	
	// Copy dimension/format section
	// + Colortable section
	memcpy ((char *) pDib,
		(char *) pDibHeader,
		sizeof(BITMAPINFOHEADER) + sizeDibTable);
	
	// Fill the Image section (the Pixels) 
	result = GetDIBits(pDC->GetSafeHdc(), // handle of DC
		hBitmap,				// handle of bitmap	object 
		0,						// starting scan line
		bitmap.bmHeight,		// number of scan lines
		(LPSTR) pImage,			// array with bitmap bits
		(BITMAPINFO *) pDib,	// structure with bitmap data
		DIB_RGB_COLORS);
	return hDib;
}  

// From a DIB, create a CBitmap object with elements
// mapped to the system palette
#define NEW_DibToBitmap
CBitmap* DibToBitmap (BITMAPINFOHEADER* pDib, CDC* pDC, CPalette* pTargetPalette)
{
	BOOL result;
	BITMAP bitmap;
	DWORD offsetDibColors;
	CDC memDC;
	CBitmap* pBitmap = NULL; 
	CBitmap* pOldBitmap;
	CBitmap appoBitmap;
	long lResult;
	int nNumColors;
	HPALETTE hPalette;
	CPalette palette;
	CPalette* pPalette;
	CPalette* pOldPalette;

	/* offset to the bits from start of DIB header		  */
	nNumColors = pDib->biClrUsed;
	offsetDibColors = pDib->biSize + nNumColors * sizeof(RGBQUAD);

	// Create a DC to hold palette
	memDC.CreateCompatibleDC (NULL);
	
	// Come diavolo si inizializza una bitmap senza un altra non si capisce
	// il blocco seguente andrebbe sostituito usando la CreateBitmapIndirect()
	// che però non funziona, al posto della CreateCompatibleBitmap che prende gli
	// attributi da quella già selezionata nel DC.
	switch (nNumColors) {
	case 16  :
		appoBitmap.LoadBitmap(IDB_BITMAP16);
		break;
	case 256 :
		appoBitmap.LoadBitmap(IDB_BITMAP256);
		break;
	default  :
		appoBitmap.LoadBitmap(IDB_BITMAP16M);
		break;
	}
	pOldBitmap = memDC.SelectObject(&appoBitmap);
	
	// Create a palette, attach it to the context,
	// and map its entries to those of the system palette
	if (hPalette = MakeDibPalette(pDib, pDC)) {
		pPalette = palette.FromHandle(hPalette);
		if (pTargetPalette)
			MapPalette (pPalette, pTargetPalette);
		pOldPalette = memDC.SelectPalette(pPalette, FALSE);	   
		UINT nRemapped = memDC.RealizePalette();
	} 
	else
		pOldPalette = NULL;

#ifdef NEW_DibToBitmap

	HBITMAP hBitmap;
	hBitmap = CreateDIBitmap (memDC.m_hDC, // pDC->m_hDC,
			(BITMAPINFOHEADER *) pDib,
			CBM_INIT,
			(BYTE *) pDib + offsetDibColors,
			(BITMAPINFO *) pDib,
			DIB_RGB_COLORS);
	// da handle a struttura di BITMAP 													 
	result = GetObject(hBitmap, sizeof(bitmap), &bitmap);
	// da struttura di BITMAP a CBitmap													 
	pBitmap = new(CBitmap);
	result =
	pBitmap->CreateBitmapIndirect(&bitmap);
	pBitmap->SetBitmapDimension (
		(int) pDib->biWidth, (int) pDib->biHeight);

#else // NEW_DibToBitmap

	bitmap.bmType = 0; // pBmpInfoHeader;
	bitmap.bmWidth = /** (int) **/ pDib->biWidth;
	bitmap.bmHeight = /** (int) **/ pDib->biHeight;
	bitmap.bmPlanes = /** (BYTE) **/ pDib->biPlanes;
	bitmap.bmBitsPixel = /** (BYTE) **/ pDib->biBitCount;
	// Must round up to nearest multiple of 16
	// (using lResult to store temporary ...)
	lResult = (pDib->biWidth * pDib->biBitCount + 15) / 16;
	bitmap.bmWidthBytes = /** (int) **/ (lResult * 16);
	bitmap.bmBits = NULL;
	
	// Create the bitmap
	pBitmap = new CBitmap;
	lResult = pBitmap->CreateCompatibleBitmap(&memDC, 
		(int) pDib->biWidth, 
		(int) pDib->biHeight);
	pBitmap->SetBitmapDimension (
		(int) pDib->biWidth, (int) pDib->biHeight);
	memDC.SelectObject(pOldBitmap);

#endif // NEW_DibToBitmap

	// Fill the bitmap
	lResult = SetDIBits(memDC.m_hDC,/* handle of device context	*/
		(HBITMAP) pBitmap->m_hObject,/* handle of bitmap	*/
		0,							/* starting scan line	*/
		bitmap.bmHeight,			/* number of scan lines	*/
		(LPSTR) pDib + offsetDibColors,	/* address of array with bitmap bits */
		(LPBITMAPINFO) pDib,		/* address of structure with bitmap data */
		DIB_RGB_COLORS);			/* type of color indices to use */
	
	if (pOldPalette)
		memDC.SelectPalette(pOldPalette, FALSE);	  
	memDC.DeleteDC();
	appoBitmap.DeleteObject();	// 980306
	
	return pBitmap;
}

// create an 8x8 Dib filled with the color specified,
// which is already in the Iperlogo palette (?);
// allocates global buffer, to be released after use
HGLOBAL ColorToDib (CDC* pDC, COLORREF inColor, int bitCount)
{
	HGLOBAL hDib;
	BITMAPINFOHEADER* pDib;
	DWORD sizeHeader = sizeof(BITMAPINFOHEADER);
	DWORD sizeTable = sizeof(RGBQUAD);
	int width = 8;
	int height = 8;
	int sizeRow = (((width * bitCount) + 31) & ~31) >> 3;
	int sizeImage = height * sizeRow;
	int sizeDib = sizeHeader + sizeTable + sizeImage;

	COLORREF color;
	int iColor = ColorInPalette (pDC, inColor, &color, 0);

	// allocate and lock buffer									  
	if ((hDib = GlobalAlloc(GMEM_MOVEABLE, sizeDib)) == NULL)
		return NULL;
	if ((pDib = (BITMAPINFOHEADER *) GlobalLock(hDib)) == NULL)
		return NULL;
	RGBQUAD* pTable = (RGBQUAD*) ((char *) pDib + sizeHeader);
	BYTE* pImage = (BYTE*) ((char *) pDib + sizeHeader + sizeTable);

	// zero structure
	memset((LPSTR) pDib, 0, sizeDib);
	// fill header
	pDib->biSize = sizeof(BITMAPINFOHEADER);
	pDib->biWidth = 8; 
	pDib->biHeight = 8; 
	pDib->biPlanes = 1; 
	pDib->biBitCount = 8; 
	pDib->biCompression = BI_RGB; 
	pDib->biSizeImage = sizeImage; 
//	pDib->biXPelsPerMeter; 
//	pDib->biYPelsPerMeter; 
	pDib->biClrUsed = 1; 
	pDib->biClrImportant = 1;
	// fill 1-entry ColorTab
	pTable->rgbRed = GetRValue(color);
	pTable->rgbGreen = GetGValue(color);
	pTable->rgbBlue = GetBValue(color);
	// fill Image bits
	memset((LPSTR) pImage, /*iColor*/ 0, sizeImage);

	return hDib;
}

// create a Bitmap from a Dib for the given logical palette,
// by adding, to the palette, lacking Colortable indexes
// and updating color indexes in the image bits
CBitmap*  DibRealize (BITMAPINFOHEADER* pDib, CDC* pDC, CPalette* pPalette)
{
	CBitmap* pBitmap = NULL;
	RGBQUAD* pColorTab;
	PALETTEENTRY newEntries[256];
	int	convTable[256];
	int result;

	int bitPixel = pDib->biPlanes * pDib->biBitCount;
	int nColors = 0;		//
	int maxColors = 0;		//
	if (bitPixel != 24) {	// 980407: aggiunto test
		// minimize the colortable size
		maxColors = 1 << (pDib->biPlanes * pDib->biBitCount);
		if (maxColors == 256) {
			if (! DibNormalize (pDib))
				return NULL;
		}
		nColors = DibExplicitInfo (pDib);
		pColorTab = (RGBQUAD *) ((char*) pDib + pDib->biSize);
	}

	// extract or compute some sizes and addresses
	int sizeRow = (((pDib->biWidth * pDib->biPlanes *
		pDib->biBitCount) + 31) & ~31) >> 3;
	int sizeImage = pDib->biSizeImage;
	char* pImage =
		(char*) pDib + pDib->biSize + nColors * sizeof(RGBQUAD);
	
	if ((pPalette != NULL) && (maxColors == 256)) {
		
		// buid color indexes conversion table (colortable->palette)
		// while listing colortable entries not found in the palette
		int iColor;
		COLORREF inColor, outColor;
		int iPalette;
		RGBQUAD quad;
		PALETTEENTRY entry;
		BYTE red, green, blue;
		memset((LPSTR) &newEntries[0], 0, sizeof(newEntries));
		int nNewEntries = 0;
		int nPalEntries = pPalette->GetEntryCount();
		for (iColor=0; iColor<nColors; iColor++) {
			quad = pColorTab[iColor];
			entry.peRed = red = quad.rgbRed;
			entry.peGreen = green = quad.rgbGreen;
			entry.peBlue = blue = quad.rgbBlue;
			entry.peFlags = 0;
			inColor = RGB(red, green, blue);
			iPalette = 
				CPaletteNearestColor (pPalette, inColor, &outColor);
			if (ColorDistance (inColor, outColor) == 0)
				convTable[iColor] = iPalette;
			else {
				newEntries[nNewEntries] = entry;
				convTable[iColor] = nPalEntries + nNewEntries;
				++nNewEntries;
			}
		}
		if (nNewEntries) {		
			// resize palette and add new entries to it
			if (! pPalette->ResizePalette(nPalEntries + nNewEntries))
				return 0;
			result =
				pPalette->SetPaletteEntries (nPalEntries, nNewEntries, &newEntries[0]);
			ASSERT(result);
		}
	}
	
	if (pPalette) {
		pDC->SelectPalette(pPalette, FALSE);	   
		pDC->RealizePalette();
	}

	// get the handle of a new bitmap object
	HBITMAP hBitmap;
	hBitmap = CreateDIBitmap (pDC->GetSafeHdc(),
		(BITMAPINFOHEADER *) pDib,
		CBM_INIT,
		pImage,
		(BITMAPINFO *) pDib,
		DIB_RGB_COLORS);

	if (hBitmap == NULL) {
		TRACE("null bitmap\n");
		return NULL;
	}
	
	// from handle to CBitmap object (vedi esempio vecchio)
	pBitmap = new(CBitmap);
	pBitmap->Attach (hBitmap);
	pBitmap->SetBitmapDimension (
		(int) pDib->biWidth, (int) pDib->biHeight);

	return pBitmap;
}


// Legge da file .BMP gia' aperto una DIB, allocandogli il buffer
// Riporta handle del buffer con la struttura DIB oppure NULL
HGLOBAL ReadDib (int hFile)
{
	BITMAPFILEHEADER bmfh;
	HGLOBAL hDib;
	BITMAPINFOHEADER* pDib;
	DWORD sizeDib;
	DWORD sizeDibHeader;
	UINT nBytesToRead;
	UINT nBytesRead;

	// read the BITMAPFILEHEADER
	nBytesToRead = sizeof (bmfh);
	nBytesRead = _read (hFile, (LPSTR) &bmfh, nBytesToRead);
	if (nBytesRead != nBytesToRead)
		return NULL;
	// type must be "BM"									   
	if (bmfh.bfType != * (WORD *) "BM")	
		return NULL;
	// compute DIB size									   
	sizeDib = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	// allocate and lock buffer									  
	if ((hDib = GlobalAlloc(GMEM_MOVEABLE, sizeDib)) == NULL)
		return NULL;
	if ((pDib = (BITMAPINFOHEADER *) GlobalLock(hDib)) == NULL)
		return NULL;
	// read the DIB info (Header + Colors + Pixels)
	nBytesRead = _read (hFile, (LPSTR) pDib, sizeDib);
	sizeDibHeader = pDib->biSize;
	if (	(nBytesRead == sizeDib)
		&&	(	(sizeDibHeader == 12)	// bitmap OS/2 1.x
			 ||	(sizeDibHeader >= 16))	// DIB di Windows
		)
		return hDib;
	else {
	// release temporary objects
		GlobalUnlock(hDib);  
		GlobalFree(hDib);
		return NULL;
	}
}


// La funzione si aspetta la handle già aperta
// l' oggetto CBitmap è già una DIB quindi .....
BOOL SaveBitMaptoFile(int hFile, CBitmap *pBitmap)
{
	BOOL result = TRUE;
	BITMAPFILEHEADER bmfhFileHeader;
///	WORD nNumColors;
	UINT nNumColors;
	HGLOBAL hDIBInfo = 0;
	HGLOBAL hHeader;
	BITMAP bmpDescriptor;
	LPBITMAPINFOHEADER pBmpInfoHeader;
	char * pBittini = NULL;
	int  bError = FALSE;	  
	long lResult;
	long lBytesScritti;
	UINT nBytesDaScrivere;
	long lImageSize;				
	
	TRY {								  
		hHeader = GlobalAlloc(GMEM_SHARE, sizeof(BITMAPINFOHEADER) + 512 * sizeof(RGBQUAD) );
		pBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hHeader);
		
		lResult = pBitmap->GetObject(sizeof(bmpDescriptor), &bmpDescriptor);
		
		// Determiniamo i colori in funzione dei bits per pixel 													 
		nNumColors =  nNumColors = 1 << bmpDescriptor.bmBitsPixel;
		
		// Preparazione del record di testa
		memset((LPSTR) pBmpInfoHeader,	0, sizeof(BITMAPINFOHEADER));
		memset(&bmfhFileHeader, 0, sizeof(bmfhFileHeader));
		
		
		bmfhFileHeader.bfType = 0x4d42;   // BM
		
		// Original code ..... (very criptic !!) 
		lImageSize = ((((bmpDescriptor.bmWidth * bmpDescriptor.bmPlanes *
			(DWORD) bmpDescriptor.bmBitsPixel) + 31) & ~31) >> 3)
			* bmpDescriptor.bmHeight;
		
		bmfhFileHeader.bfSize = lImageSize +
			sizeof(BITMAPINFOHEADER) + sizeof(bmfhFileHeader)
			+ sizeof(RGBQUAD) * nNumColors;
		
		
		bmfhFileHeader.bfOffBits = sizeof(bmfhFileHeader) + sizeof(BITMAPINFOHEADER)
			+ sizeof(RGBQUAD) * nNumColors;
		
		pBmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER); // No RGB values
		pBmpInfoHeader->biWidth = bmpDescriptor.bmWidth;
		pBmpInfoHeader->biHeight = bmpDescriptor.bmHeight;
///		pBmpInfoHeader->biPlanes = bmpDescriptor.bmPlanes;
///		pBmpInfoHeader->biBitCount = bmpDescriptor.bmBitsPixel;
		pBmpInfoHeader->biBitCount = bmpDescriptor.bmPlanes * bmpDescriptor.bmBitsPixel;
		pBmpInfoHeader->biPlanes = 1;
		pBmpInfoHeader->biCompression = BI_RGB;
		pBmpInfoHeader->biSizeImage = 0; // Can be 0 if RGB
		pBmpInfoHeader->biXPelsPerMeter;
		pBmpInfoHeader->biYPelsPerMeter;
		pBmpInfoHeader->biClrUsed = 0;
		pBmpInfoHeader->biClrImportant = 0;
		
		UINT inLength, outLength;
		/* write the BITMAPFILEHEADER */
		inLength = sizeof (bmfhFileHeader);
		outLength = _write (hFile, (LPSTR) &bmfhFileHeader, inLength);
		if (outLength != inLength)
			AfxThrowFileException(CFileException::generic, 780);
		
		/* write the BITMAPINFOHEADER */
		inLength = sizeof(BITMAPINFOHEADER);
		outLength = _write (hFile, (LPSTR) pBmpInfoHeader, inLength);
		if (outLength != inLength)
			AfxThrowFileException(CFileException::generic, 781);
		
		hDIBInfo = GlobalAlloc(GHND, lImageSize);
		/* Check if sucessfull */
		if (!hDIBInfo)	
			AfxThrowMemoryException();
		pBittini =	(char *) GlobalLock(hDIBInfo);
		if (!pBittini)	
			AfxThrowMemoryException(); 
		
#define NON_FUNGE
		
#ifdef NON_FUNGE		
		{ 
			HDC hDC;
			hDC = GetDC(NULL); 
			lResult = GetDIBits(hDC,			/* handle of device context	*/
				// (HBITMAP) pppp.m_hObject, // hBmp, // 
				(HBITMAP) pBitmap->m_hObject,  /* handle of bitmap	*/
				0, // uStartScan,			   /* starting scan line	*/
				bmpDescriptor.bmHeight, 	   /* number of scan lines	*/
				NULL,						   /* address of array with bitmap bits */
				(LPBITMAPINFO) pBmpInfoHeader, /* address of structure with bitmap data */
				DIB_RGB_COLORS);																					
			
			/* write the COLORTABLE 	   */ 
			_write (hFile, (LPSTR) pBmpInfoHeader + pBmpInfoHeader->biSize, sizeof(RGBQUAD) * nNumColors);
			
			lResult = GetDIBits(hDC,			/* handle of device context	*/
				// (HBITMAP) pppp.m_hObject, // hBmp, // 
				(HBITMAP) pBitmap->m_hObject,  /* handle of bitmap	*/
				0, // uStartScan,			   /* starting scan line	*/
				bmpDescriptor.bmHeight, 	   /* number of scan lines	*/
				(LPSTR) pBittini,			   /* address of array with bitmap bits */
				(LPBITMAPINFO) pBmpInfoHeader, /* address of structure with bitmap data */
				DIB_RGB_COLORS);
			
			ReleaseDC(NULL, hDC);
		}	   
#else
		// Funge ma solo per multipli di 32 di dimensione	 
		lResult = pBitmap->GetBitmapBits(lImageSize, pBittini);
#endif
		
		lBytesScritti = 0;
		
		//	  if (lResult < lImageSize) {
		//		_lwrite(hFile, pBittini, lImageSize - lResult);
		//	  } 						   
		while (lBytesScritti < lImageSize) {
			nBytesDaScrivere = ((lImageSize - lBytesScritti) < 0x10000L) ? 
				(UINT) (lImageSize - lBytesScritti) : 0x7FFFU;
			if (_write(hFile, pBittini, nBytesDaScrivere) != nBytesDaScrivere)
				AfxThrowFileException(CFileException::generic, 785);
			lBytesScritti += nBytesDaScrivere;		
			pBittini += nBytesDaScrivere;
		}
  }
  CATCH(CException, Except) {
	  result = FALSE;
  }
  END_CATCH;
  
  if (hDIBInfo) {
	  GlobalUnlock(hDIBInfo);
	  GlobalFree(hDIBInfo);
  }  
  return result;
}  

// La funzione si aspetta la handle già aperta
CBitmap* LoadBitMapFromFile(int hFile, CDC* pDC, CPalette *pTargetPalette)
{
	BITMAPFILEHEADER bmfhFileHeader;
///	WORD nNumColors;
	UINT nNumColors;
	HGLOBAL hDIBInfo = NULL;
	LPBITMAPINFOHEADER pBmpInfoHeader = NULL;
	CBitmap *pBitmap = NULL; 
	int  bError = FALSE;	  
	CDC  dcMemDC;								
#ifndef NEW_DIB
	long lResult;
	BITMAP				 bmpDescriptor;
	int  ColorCoding;
	HPALETTE hpNewPal;
	CPalette *ppalOldPal, palNewPal;
#endif // NEW_DIB
///	WORD offBits;
	UINT offBits;
	DWORD hFileError = HFILE_ERROR;
	UINT nBytesToRead;
	UINT nBytesRead;

#ifdef NEW_DIB

	HGLOBAL hDib;
	BITMAPINFOHEADER* pDib;

	hDib = ReadDib (hFile);
	if (hDib == NULL) 
		bError = TRUE;
	else {
		pDib = (BITMAPINFOHEADER *) GlobalLock (hDib);
		pBmpInfoHeader = pDib;

		// Se non ha una color table può essere un 24 bit RGB non indicizzato 
		// che non vuole color table										 
		nNumColors = pBmpInfoHeader->biClrUsed;    
		if (! nNumColors) {    
			/* no color table for 24-bit, default (standard) size otherwise */
			if (pBmpInfoHeader->biBitCount != 24)
///				nNumColors = 1 << pBmpInfoHeader->biBitCount;
				nNumColors = 1 << (pBmpInfoHeader->biPlanes * pBmpInfoHeader->biBitCount);
		}
		//	fill in some default values if they are zero
		if (pBmpInfoHeader->biClrUsed == 0)
			pBmpInfoHeader->biClrUsed = /*(DWORD)*/ nNumColors;
		if (pBmpInfoHeader->biSizeImage == 0) {
			pBmpInfoHeader->biSizeImage = ((((pBmpInfoHeader->biWidth * 
///				(DWORD)pBmpInfoHeader->biBitCount) + 31) & ~31) >> 3)
				pBmpInfoHeader->biPlanes * pBmpInfoHeader->biBitCount) + 31) & ~31) >> 3)
				* pBmpInfoHeader->biHeight;
		}
	}

#else // NEW_DIB

	TRY {																			  
		
		// L' array iniziale prevede 256 colori 									  
		hDIBInfo = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 
			256 * sizeof(RGBQUAD)));
		pBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hDIBInfo);
		
		/* read the BITMAPFILEHEADER */
		//	if (sizeof (bmfhFileHeader) != _lread (hFile, (LPSTR) &bmfhFileHeader, sizeof (bmfhFileHeader)))
		nBytesToRead = sizeof (bmfhFileHeader);
		nBytesRead = _read (hFile, (LPSTR) &bmfhFileHeader, nBytesToRead);
		if (nBytesRead != nBytesToRead) {
			DWORD LastError = GetLastError();
			AfxThrowFileException(CFileException::generic, 780);
		}								  
		/* se type diverso da 'BM' */										   
		if (bmfhFileHeader.bfType != 0x4d42)	
			AfxThrowFileException(CFileException::invalidFile, 781);
		
		/* read the BITMAPINFOHEADER */
		// if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR) pBmpInfoHeader, sizeof(BITMAPINFOHEADER)))
		nBytesToRead = sizeof(BITMAPINFOHEADER);
		nBytesRead = _read (hFile, (LPSTR) pBmpInfoHeader, nBytesToRead);
		if (nBytesRead != nBytesToRead)
			AfxThrowFileException(CFileException::generic, 782);
		
		// !!!!! for now, don't even deal with CORE headers
		if (pBmpInfoHeader->biSize == sizeof(BITMAPCOREHEADER))
			AfxThrowFileException(CFileException::generic, 783);
		
		// Se non ha una color table può essere un 24 bit RGB non indicizzato 
		// che non vuole color table										 
		// if (!(nNumColors = (WORD)pBmpInfoHeader->biClrUsed)) {    
		nNumColors = pBmpInfoHeader->biClrUsed;    
		if (! nNumColors) {    
			/* no color table for 24-bit, default (standard) size otherwise */
			if (pBmpInfoHeader->biBitCount != 24)
				nNumColors = 1 << pBmpInfoHeader->biBitCount;
		}
		
		//	fill in some default values if they are zero
		if (pBmpInfoHeader->biClrUsed == 0)
			pBmpInfoHeader->biClrUsed = /*(DWORD)*/ nNumColors;
		if (pBmpInfoHeader->biSizeImage == 0) {
			pBmpInfoHeader->biSizeImage = ((((pBmpInfoHeader->biWidth * 
				(DWORD)pBmpInfoHeader->biBitCount) + 31) & ~31) >> 3)
				* pBmpInfoHeader->biHeight;
		}
		
		/* get a proper-sized buffer for header, color table and bits */ 
		GlobalUnlock(hDIBInfo);
		hDIBInfo = GlobalReAlloc(hDIBInfo, pBmpInfoHeader->biSize +
			nNumColors * sizeof(RGBQUAD) +
			pBmpInfoHeader->biSizeImage, 0);
		/* Check if resize sucessfull								*/
		if (!hDIBInfo)	
			AfxThrowMemoryException();
		
		pBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hDIBInfo);
		
		/* read the color table 															*/
		//	_lread (hFile, (LPSTR)(pBmpInfoHeader) + pBmpInfoHeader->biSize, nNumColors * sizeof(RGBQUAD));
		_read (hFile, (LPSTR) pBmpInfoHeader + pBmpInfoHeader->biSize, nNumColors * sizeof(RGBQUAD));
		
		/* offset to the bits from start of DIB header		  */
		offBits = (WORD) pBmpInfoHeader->biSize + nNumColors * sizeof(RGBQUAD);
		
		if (bmfhFileHeader.bfOffBits != 0L)
		{
//			_llseek(hFile, bmfhFileHeader.bfOffBits,SEEK_SET);
			_lseek(hFile, bmfhFileHeader.bfOffBits,SEEK_SET);
		}
		//	if (!(pBmpInfoHeader->biSizeImage == lread(hFile, 
		if (!(pBmpInfoHeader->biSizeImage == _read(hFile, 
			(LPSTR) pBmpInfoHeader + offBits, 
			pBmpInfoHeader->biSizeImage))) {
			AfxThrowFileException(CFileException::generic, 785);
		}									   
	}
	CATCH(CException, Except) {
		if (Except->IsKindOf(RUNTIME_CLASS(CMemoryException))) {
			// Sei nella merda fino al collo non c' è memoria !!
		}
		AfxMessageBox("Errore di lettura bitmap");
		bError = TRUE;
	}
	END_CATCH;

#endif // NEW_DIB
	
	// Se ho letto costriusco l' oggetto CBitmap  
	if (!bError) {	  

#ifdef NEW_DIB
//		pBitmap = DibToBitmap (pBmpInfoHeader, pDC, pTargetPalette);
		pBitmap = DibRealize (pBmpInfoHeader, pDC, pTargetPalette);
#else // NEW_DIB
		bmpDescriptor.bmType					= 0; // pBmpInfoHeader;
		bmpDescriptor.bmWidth				  = (int) pBmpInfoHeader->biWidth;
		bmpDescriptor.bmHeight				= (int) pBmpInfoHeader->biHeight;
		bmpDescriptor.bmPlanes				= (BYTE) pBmpInfoHeader->biPlanes;
		bmpDescriptor.bmBitsPixel		  = (BYTE) pBmpInfoHeader->biBitCount;
		// Must round up to nearest multiple of 16 (using lResult to store temporary ...)
		lResult = (pBmpInfoHeader->biWidth * pBmpInfoHeader->biBitCount + 15) / 16;
		bmpDescriptor.bmWidthBytes		= (int) (lResult * 16);
		bmpDescriptor.bmBits					= NULL;
		
		// Create a DC to hold palette
		dcMemDC.CreateCompatibleDC(NULL);
		
		CBitmap* pOldBitmap, AppoBitMap;
		// Come diavolo si inizializza una bitmap senza un altra non si capisce
		// il blocco seguente andrebbe sostituito usando la CreateBitmapIndirect()
		// che però non funziona, al posto della CreateCompatibleBitmap che prende gli
		// attributi da quella già selezionata nel DC.
		switch (nNumColors) {
		case 16  :
			AppoBitMap.LoadBitmap(IDB_BITMAP16);
			break;
			
		case 256 :
			AppoBitMap.LoadBitmap(IDB_BITMAP256);
			break;
			
		default  :
			AppoBitMap.LoadBitmap(IDB_BITMAP16M);
			break;
		}
		pOldBitmap = dcMemDC.SelectObject(&AppoBitMap);
		
		if (hpNewPal = MakeDibPalette(pBmpInfoHeader)) {
			ppalOldPal = dcMemDC.SelectPalette(CPalette::FromHandle(hpNewPal), FALSE);	   
			dcMemDC.RealizePalette();
		} 
		else {
			ppalOldPal = NULL;
		}
		
		pBitmap = new CBitmap;
		lResult = pBitmap->CreateCompatibleBitmap(&dcMemDC, 
			(int) pBmpInfoHeader->biWidth, 
			(int) pBmpInfoHeader->biHeight);
		//>971022
		pBitmap->SetBitmapDimension (
			(int) pBmpInfoHeader->biWidth,
			(int) pBmpInfoHeader->biHeight);
		//<
		
		dcMemDC.SelectObject(pOldBitmap);
		ColorCoding = DIB_RGB_COLORS;
		// DIB_RGB_COLORS
		// DIB_PAL_COLORS
		
		lResult = SetDIBits(dcMemDC.m_hDC,				   /* handle of device context	*/
			(HBITMAP) pBitmap->m_hObject,  /* handle of bitmap	*/
			0, // uStartScan,			   /* starting scan line	*/
			bmpDescriptor.bmHeight, 	   /* number of scan lines	*/
			(LPSTR) pBmpInfoHeader + offBits, /* address of array with bitmap bits */
			(LPBITMAPINFO) pBmpInfoHeader,    /* address of structure with bitmap data */
			ColorCoding);				/* type of color indices to use */

		if (ppalOldPal)
			dcMemDC.SelectPalette(ppalOldPal, FALSE);	  
		dcMemDC.DeleteDC();
#endif // NEW_DIB
	}
	
	// Distruggo gli oggetti temporanei
#ifdef NEW_DIB
	if (hDib)
		GlobalFree(hDib);
#else // NEW_DIB
	if (pBmpInfoHeader)
		GlobalUnlock(hDIBInfo);  
	if (hDIBInfo)
		GlobalFree(hDIBInfo);  
#endif // NEW_DIB
	
	return pBitmap;
}		 
