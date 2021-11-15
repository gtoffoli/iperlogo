
// dibgsdoc.cpp : implementation of the CDibgsiDoc class
//

#include "stdafx.h"
               
#include "resource.h"
               
// GSI*****------START
/* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0

#define MAXREAD  32768		       /* Number of bytes to be read during */


DWORD PASCAL lread (int fh, VOID far *pv, DWORD ul)
{
    DWORD     ulT = ul;
    BYTE huge *hp = (BYTE huge*) pv;

    while (ul > (DWORD) MAXREAD) {
	if (_lread(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
		return 0;
	ul -= MAXREAD;
	hp += MAXREAD;
    }
    if (_lread(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
	return 0;
    return ulT;
}




HPALETTE PASCAL NEAR MakeDIBPalette(LPBITMAPINFOHEADER lpInfo)
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
    for (i = 0; i < (WORD)lpInfo->biClrUsed; i++, lpRGB++) {
	    npPal->palPalEntry[i].peRed = lpRGB->rgbRed;
	    npPal->palPalEntry[i].peGreen = lpRGB->rgbGreen;
	    npPal->palPalEntry[i].peBlue = lpRGB->rgbBlue;
	    npPal->palPalEntry[i].peFlags = 0;
  	}

	  hLogPal = CreatePalette((LPLOGPALETTE)npPal);
	  LocalFree((HANDLE)npPal);
	  return(hLogPal);
  }

  /* 24-bit DIB with no color table.  return default palette.  Another
  ** option would be to create a 256 color "rainbow" palette to provide
  ** some good color choices.
  */
  else
	  return((HPALETTE) GetStockObject(DEFAULT_PALETTE));
}



//----------------------------------------------------------------------------
//
// La funzione si aspetta la handle già aperta
// l' oggetto CBitmap è già una DIB quindi .....
//
//

extern "C" long _tell(int hFile);

BOOL SaveBitMaptoFile(int hFile, CBitmap *pBitmap)
{
  BITMAPFILEHEADER     bmfhFileHeader;
  WORD		             nNumColors;
  HGLOBAL              hDIBInfo, hHeader;
  BITMAP               bmpDescriptor;
  LPBITMAPINFOHEADER   pBmpInfoHeader;
  LPSTR                pBittini;
  int  bError = FALSE;      
  long lResult;
  long lBytesScritti;
  UINT nBytesDaScrivere;
  long lImageSize;                
  // LPPALETTEENTRY pPalColors;
  // CBitmap*   pOldBitmap;
  // CPalette*  pOldPalette;

  TRY {                                 
    hHeader = GlobalAlloc(GMEM_SHARE, sizeof(BITMAPINFOHEADER) + 512 * sizeof(RGBQUAD) );
    pBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hHeader);
                                               
    lResult = pBitmap->GetObject(sizeof(bmpDescriptor), &bmpDescriptor);
                                                           
    // Determiniamo i colori in funzione dei bits per pixel                                                      
    nNumColors =  nNumColors = 1 << bmpDescriptor.bmBitsPixel;
                                                           
    // Preparazione del record di testa
    _fmemset((LPSTR) pBmpInfoHeader,  0, sizeof(BITMAPINFOHEADER));
    memset(&bmfhFileHeader, 0, sizeof(bmfhFileHeader));
         
         
    bmfhFileHeader.bfType = 0x4d42;   // BM

    // Original code ..... (very criptic !!) 
    lImageSize = ((((bmpDescriptor.bmWidth * bmpDescriptor.bmPlanes *
                                  (DWORD) bmpDescriptor.bmBitsPixel) + 31) & ~31) >> 3)
		                               * bmpDescriptor.bmHeight;

    // lImageSize = (bmpDescriptor.bmWidth * bmpDescriptor.bmPlanes * 
    //              bmpDescriptor.bmBitsPixel + 31) / 8 *
    //              bmpDescriptor.bmHeight;
    bmfhFileHeader.bfSize = lImageSize +
                             sizeof(BITMAPINFOHEADER) + sizeof(bmfhFileHeader)
                             + sizeof(RGBQUAD) * nNumColors;
     

    bmfhFileHeader.bfOffBits = sizeof(bmfhFileHeader) + sizeof(BITMAPINFOHEADER)
                               + sizeof(RGBQUAD) * nNumColors;
         
    pBmpInfoHeader->biSize					= sizeof(BITMAPINFOHEADER); // No RGB values
    pBmpInfoHeader->biWidth					= bmpDescriptor.bmWidth;
    pBmpInfoHeader->biHeight				= bmpDescriptor.bmHeight;
    pBmpInfoHeader->biPlanes				= bmpDescriptor.bmPlanes;
    pBmpInfoHeader->biBitCount			= bmpDescriptor.bmBitsPixel;
    pBmpInfoHeader->biCompression 	= BI_RGB;
    pBmpInfoHeader->biSizeImage			= 0; // Can be 0 if RGB
    pBmpInfoHeader->biXPelsPerMeter;
    pBmpInfoHeader->biYPelsPerMeter;
    pBmpInfoHeader->biClrUsed 			= 0;
    pBmpInfoHeader->biClrImportant 	= 0;

    /* write the BITMAPFILEHEADER */
    if (sizeof (bmfhFileHeader) != _lwrite (hFile, (LPSTR) &bmfhFileHeader, sizeof (bmfhFileHeader)))
	    AfxThrowFileException(CFileException::generic, 780);
                                          
    /* write the BITMAPINFOHEADER */
    if (sizeof(BITMAPINFOHEADER) != _lwrite (hFile, (LPSTR) pBmpInfoHeader, sizeof(BITMAPINFOHEADER)))
	    AfxThrowFileException(CFileException::generic, 781);

    hDIBInfo = GlobalAlloc(GHND, lImageSize);
		/* Check if sucessfull			                    */
    if (!hDIBInfo)	
	    AfxThrowMemoryException();
    pBittini =  (LPSTR) GlobalLock(hDIBInfo);
    if (!pBittini)	
	    AfxThrowMemoryException(); 
	    
#define NON_FUNGE
	    
#ifdef NON_FUNGE	    
    { 
      HDC hDC;
      hDC = GetDC(NULL); 
      // HPALETTE hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);
      // HPALETTE hPalOld =  SelectPalette(hDC, hPal, FALSE);
      // RealizePalette(hDC);
      // HBITMAP hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)); 
      // CBitmap pppp;
      // pppp.LoadBitmap(IDB_BITMAP1);
      
      lResult = GetDIBits(hDC,                         /* handle of device context	*/
                        // (HBITMAP) pppp.m_hObject, // hBmp, // 
                        (HBITMAP) pBitmap->m_hObject,  /* handle of bitmap	*/
                        0, // uStartScan,              /* starting scan line	*/
                        bmpDescriptor.bmHeight,        /* number of scan lines	*/
                        NULL,                          /* address of array with bitmap bits	*/
                        (LPBITMAPINFO) pBmpInfoHeader, /* address of structure with bitmap data	*/
                        DIB_RGB_COLORS);                                                                                    
                                                                                    
      /* write the COLORTABLE        */ 
      _lwrite (hFile, (LPSTR) pBmpInfoHeader + pBmpInfoHeader->biSize, sizeof(RGBQUAD) * nNumColors);

      lResult = GetDIBits(hDC,                         /* handle of device context	*/
                        // (HBITMAP) pppp.m_hObject, // hBmp, // 
                        (HBITMAP) pBitmap->m_hObject,  /* handle of bitmap	*/
                        0, // uStartScan,              /* starting scan line	*/
                        bmpDescriptor.bmHeight,        /* number of scan lines	*/
                        (LPSTR) pBittini,              /* address of array with bitmap bits	*/
                        (LPBITMAPINFO) pBmpInfoHeader, /* address of structure with bitmap data	*/
                        DIB_RGB_COLORS);
                        // DIB_RGB_COLORS);            /* type of color indices to use	*/
                        
      ReleaseDC(NULL, hDC);

    }      
#else
    // Funge ma solo per multipli di 32 di dimensione    
    lResult = pBitmap->GetBitmapBits(lImageSize, pBittini);
#endif
    
    lBytesScritti = 0;
                                
    if (lResult < lImageSize) {
      _lwrite(hFile, pBittini, lImageSize - lResult);
    }                            
    while (lBytesScritti < lImageSize) {
      nBytesDaScrivere = ((lImageSize - lBytesScritti) < 0x10000L) ? 
                          (UINT) (lImageSize - lBytesScritti) : 0x7FFFU;
      if (_lwrite(hFile, pBittini, nBytesDaScrivere) != nBytesDaScrivere)
        AfxThrowFileException(CFileException::generic, 785);
      lBytesScritti += nBytesDaScrivere;      
    }
    
              
  }
  CATCH(CException, Except) {
 
  }
  END_CATCH;
  
  return TRUE;
}  
  
//----------------------------------------------------------------------------
//
// La funzione si aspetta la handle già aperta
// 

CBitmap* LoadBitMapFromFile(int hFile)
{
  BITMAPFILEHEADER     bmfhFileHeader;
  WORD		             nNumColors;
  HGLOBAL              hDIBInfo        = NULL;
  BITMAP               bmpDescriptor;
  LPBITMAPINFOHEADER   lpBmpInfoHeader = NULL;
  CBitmap *pBitMap = NULL; 
  int  bError = FALSE;      
  long lResult;
  CDC  dcMemDC;                               
  int  ColorCoding;
  HPALETTE hpNewPal;
  CPalette *ppalOldPal, palNewPal;
  WORD     offBits;
                                                                               
  TRY {                                                                             
               
    // L' array iniziale prevede 256 colori                                       
    hDIBInfo = GlobalAlloc(GMEM_MOVEABLE, (DWORD)(sizeof(BITMAPINFOHEADER) + 
                         256 * sizeof(RGBQUAD)));
    lpBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hDIBInfo);

    /* read the BITMAPFILEHEADER */
    if (sizeof (bmfhFileHeader) != _lread (hFile, (LPSTR) &bmfhFileHeader, sizeof (bmfhFileHeader)))
	    AfxThrowFileException(CFileException::generic, 780);
                                          
    /* se type diverso da 'BM' */                                          
    if (bmfhFileHeader.bfType != 0x4d42)	
	    AfxThrowFileException(CFileException::invalidFile, 781);

    /* read the BITMAPINFOHEADER */
    if (sizeof(BITMAPINFOHEADER) != _lread (hFile, (LPSTR) lpBmpInfoHeader, sizeof(BITMAPINFOHEADER)))
	    AfxThrowFileException(CFileException::generic, 782);

    /* !!!!! for now, don't even deal with CORE headers */
    if (lpBmpInfoHeader->biSize == sizeof(BITMAPCOREHEADER))
	    AfxThrowFileException(CFileException::generic, 783);
                                              
    // Se non ha una color table può essere un 24 bit RGB non indicizzato 
    // che non vuole color table                                         
    if (!(nNumColors = (WORD)lpBmpInfoHeader->biClrUsed)) {    
    	/* no color table for 24-bit, default size otherwise */
	    if (lpBmpInfoHeader->biBitCount != 24)
	      nNumColors = 1 << lpBmpInfoHeader->biBitCount;	/* standard size table */
    }

    /*	fill in some default values if they are zero ---------------------------------------- */
    if (lpBmpInfoHeader->biClrUsed == 0)
      lpBmpInfoHeader->biClrUsed = (DWORD)nNumColors;

    if (lpBmpInfoHeader->biSizeImage == 0) {
	    lpBmpInfoHeader->biSizeImage = ((((lpBmpInfoHeader->biWidth * 
	                                     (DWORD)lpBmpInfoHeader->biBitCount) + 31) & ~31) >> 3)
			                               * lpBmpInfoHeader->biHeight;
    }

    /* get a proper-sized buffer for header, color table and bits */ 
    GlobalUnlock(hDIBInfo);
    hDIBInfo = GlobalReAlloc(hDIBInfo, lpBmpInfoHeader->biSize +
    				               	nNumColors * sizeof(RGBQUAD) +
					                  lpBmpInfoHeader->biSizeImage, 0);
		/* Check if resize sucessfull			                    */
    if (!hDIBInfo)	
	    AfxThrowMemoryException();

    lpBmpInfoHeader = (LPBITMAPINFOHEADER) GlobalLock(hDIBInfo);

    /* read the color table 															*/
    _lread (hFile, (LPSTR)(lpBmpInfoHeader) + lpBmpInfoHeader->biSize, nNumColors * sizeof(RGBQUAD));

    /* offset to the bits from start of DIB header        */
    offBits = (WORD) lpBmpInfoHeader->biSize + nNumColors * sizeof(RGBQUAD);

    if (bmfhFileHeader.bfOffBits != 0L)
    {
        _llseek(hFile, bmfhFileHeader.bfOffBits,SEEK_SET);
    }
    if (!(lpBmpInfoHeader->biSizeImage == lread(hFile, 
                                   (LPSTR) lpBmpInfoHeader + offBits, 
                                   lpBmpInfoHeader->biSizeImage))) {
	    
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
    
  // Se ho letto costriusco l' oggetto CBitmap  
  if (!bError) {    
    
    bmpDescriptor.bmType					= 0; // lpBmpInfoHeader;
    bmpDescriptor.bmWidth				  = (int) lpBmpInfoHeader->biWidth;
    bmpDescriptor.bmHeight				= (int) lpBmpInfoHeader->biHeight;
    bmpDescriptor.bmPlanes				= (BYTE) lpBmpInfoHeader->biPlanes;
    bmpDescriptor.bmBitsPixel		  = (BYTE) lpBmpInfoHeader->biBitCount;
    // Must round up to nearest multiple of 16 (using lResult to store temporary ...)
    lResult = (lpBmpInfoHeader->biWidth * lpBmpInfoHeader->biBitCount + 15) / 16;
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
                       
    if (hpNewPal = MakeDIBPalette(lpBmpInfoHeader)) {
      ppalOldPal = dcMemDC.SelectPalette(CPalette::FromHandle(hpNewPal), FALSE);     
      dcMemDC.RealizePalette();
    } 
    else {
      ppalOldPal = NULL;
    }
    
    pBitMap = new CBitmap;
		lResult = pBitMap->CreateCompatibleBitmap(&dcMemDC, 
		                                          (int) lpBmpInfoHeader->biWidth, 
		                                          (int) lpBmpInfoHeader->biHeight);

    dcMemDC.SelectObject(pOldBitmap);
                                   
    ColorCoding = DIB_RGB_COLORS;
                        // DIB_RGB_COLORS
                        // DIB_PAL_COLORS
                                       
    lResult = SetDIBits(dcMemDC.m_hDC,                 /* handle of device context	*/
                        (HBITMAP) pBitMap->m_hObject,  /* handle of bitmap	*/
                        0, // uStartScan,              /* starting scan line	*/
                        bmpDescriptor.bmHeight,        /* number of scan lines	*/
                        (LPSTR) lpBmpInfoHeader + offBits, /* address of array with bitmap bits	*/
                        (LPBITMAPINFO) lpBmpInfoHeader,    /* address of structure with bitmap data	*/
                        ColorCoding);               /* type of color indices to use	*/


    if (ppalOldPal) {
      dcMemDC.SelectPalette(ppalOldPal, FALSE);     
   } 

    dcMemDC.DeleteDC();
  }

  // Distruggo gli oggetti temporanei
  if (lpBmpInfoHeader) {
    GlobalUnlock(hDIBInfo);  
  } 
  if (hDIBInfo) {
    GlobalFree(hDIBInfo);  
  } 
  
  return pBitMap;
  
}        

