#include "stdafx.h"
#include "dib.h"

#include "gif.h"

#include "Quant.h"
#include "Heckbert.h"

#define WIDTHBYTES(i)   ((i+31)/32*4)


WORD DibNumColors (VOID FAR *pv)
{
    int                 bits;
    LPBITMAPINFOHEADER  lpbi;
    LPBITMAPCOREHEADER  lpbc;

    lpbi = ((LPBITMAPINFOHEADER)pv);
    lpbc = ((LPBITMAPCOREHEADER)pv);

    /*  With the BITMAPINFO format headers, the size of the palette
     *  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
     *  is dependent on the bits per pixel ( = 2 raised to the power of
     *  bits/pixel).
     */
    if (lpbi->biSize != sizeof(BITMAPCOREHEADER)){
        if (lpbi->biClrUsed != 0)
            return (WORD)lpbi->biClrUsed;
        bits = lpbi->biBitCount;
    }
    else
        bits = lpbc->bcBitCount;

    switch (bits){
        case 1:
                return 2;
        case 4:
                return 16;
        case 8:
                return 256;
        default:
                /* A 24 bitcount DIB has no color table */
                return 0;
    }
}

WORD PaletteSize (VOID FAR * pv)
{
    LPBITMAPINFOHEADER lpbi;
    WORD               NumColors;

    lpbi      = (LPBITMAPINFOHEADER)pv;
    NumColors = DibNumColors(lpbi);

    if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
        return NumColors * sizeof(RGBTRIPLE);
    else
        return NumColors * sizeof(RGBQUAD);
}


HDIB BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal)
{
   BITMAP bm;                   // bitmap structure
   BITMAPINFOHEADER bi;         // bitmap header
   BITMAPINFOHEADER FAR *lpbi;  // pointer to BITMAPINFOHEADER
   DWORD dwLen;                 // size of memory block
   HANDLE hDIB, h;              // handle to DIB, temp handle
   HDC hDC;                     // handle to DC
   WORD biBits;                 // bits per pixel
   char * TheBits;

   /* check if bitmap handle is valid */

   if (!hBitmap)
      return NULL;

   /* fill in BITMAP structure, return NULL if it didn't work */
   if (!GetObject(hBitmap, sizeof(bm), (LPSTR)&bm))
      return NULL;

   /* if no palette is specified, use default palette */
   if (hPal == NULL)
      hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

   /* calculate bits per pixel */
   biBits = bm.bmPlanes * bm.bmBitsPixel;

   /* make sure bits per pixel is valid */
   if (biBits <= 1)
      biBits = 1;
   else if (biBits <= 4)
      biBits = 4;
   else if (biBits <= 8)
      biBits = 8;
   else /* if greater than 8-bit, force to 16-bit */
      biBits = 16;

   /* initialize BITMAPINFOHEADER */
   bi.biSize = sizeof(BITMAPINFOHEADER);
   bi.biWidth = bm.bmWidth;
   bi.biHeight = -bm.bmHeight;
   bi.biPlanes = 1;
   bi.biBitCount = biBits;
   bi.biCompression = BI_RGB;
   bi.biSizeImage = 0;
   bi.biXPelsPerMeter = 0;
   bi.biYPelsPerMeter = 0;
   bi.biClrUsed = 0;
   bi.biClrImportant = 0;

   /* calculate size of memory block required to store BITMAPINFO */
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi);

   /* get a DC */
   hDC = GetDC(NULL);

   /* select and realize our palette */
   hPal = SelectPalette(hDC, hPal, FALSE);
   RealizePalette(hDC);

   /* alloc memory block to store our bitmap */
   hDIB = GlobalAlloc(GHND, dwLen);

   /* if we couldn't get memory block */
   if (!hDIB)
   {
      /* clean up and return NULL */
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   /* lock memory and get pointer to it */
   lpbi = (BITMAPINFOHEADER *)GlobalLock(hDIB);

   /* use our bitmap info. to fill BITMAPINFOHEADER */
   *lpbi = bi;

   /*  call GetDIBits with a NULL lpBits param, so it will calculate the
    *  biSizeImage field for us
    */
   GetDIBits(hDC, hBitmap, 0, (WORD)-bi.biHeight, NULL, (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS);

   /* get the info. returned by GetDIBits and unlock memory block */
   bi = *lpbi;
   GlobalUnlock(hDIB);

   /* if the driver did not fill in the biSizeImage field, make one up */
   if (bi.biSizeImage == 0)
      bi.biSizeImage = WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;

   /* realloc the buffer big enough to hold all the bits */
   dwLen = bi.biSize + PaletteSize((LPSTR)&bi) + bi.biSizeImage;
   if (h = GlobalReAlloc(hDIB, dwLen, 0))
      hDIB = h;
   else
   {
      /* clean up and return NULL */
      GlobalFree(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }

   /* lock memory block and get pointer to it */
   lpbi = (BITMAPINFOHEADER *)GlobalLock(hDIB);

   /*  call GetDIBits with a NON-NULL lpBits param, and actualy get the
    *  bits this time
    */
   TheBits = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize((LPSTR)lpbi);

   if (GetDIBits(hDC, hBitmap, 0, (WORD)-bi.biHeight, TheBits, (LPBITMAPINFO)lpbi,
         DIB_RGB_COLORS) == 0)
   {
      /* clean up and return NULL */
      GlobalUnlock(hDIB);
      hDIB = NULL;
      SelectPalette(hDC, hPal, TRUE);
      RealizePalette(hDC);
      ReleaseDC(NULL, hDC);
      return NULL;
   }
   bi = *lpbi;

   /* clean up */
   GlobalUnlock(hDIB);
   SelectPalette(hDC, hPal, TRUE);
   RealizePalette(hDC);
   ReleaseDC(NULL, hDC);

   /* return handle to the DIB */
   return hDIB;
}

int  CDib::CreateFromBitmap(CBitmap *bmp)
{
 Free();
 m_hDib = BitmapToDIB((HBITMAP) *bmp, NULL);
 return m_hDib != NULL;
}

BOOL DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop)
{
    LPBITMAPINFOHEADER   lpbi;
    LPSTR                pBuf;

    if (!hdib)
        return PatBlt(hdc,x0,y0,dx,dy,rop);

    lpbi = (BITMAPINFOHEADER *)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi);
    SetDIBitsToDevice (hdc, x0, y0, dx, dy,
                       x1,y1,
                       x1,
                       dy,
                       pBuf, (LPBITMAPINFO)lpbi,
                       DIB_RGB_COLORS );

    GlobalUnlock(hdib);
    return TRUE;
}

BOOL StretchDibBlt (HDC hdc, int x, int y, int dx, int dy, 
				    HDIB hdib, int x0, int y0, int dx0, int dy0, LONG rop)
{
    LPBITMAPINFOHEADER lpbi;
    LPSTR        pBuf;
    BOOL         f;

    if (!hdib)
        return PatBlt(hdc,x,y,dx,dy,rop);

    lpbi = (BITMAPINFOHEADER *)GlobalLock(hdib);

    if (!lpbi)
        return FALSE;

    pBuf = (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi);

    f = StretchDIBits ( hdc,
                        x, y,
                        dx, dy,
                        x0, y0,
                        dx0, dy0,
                        pBuf, (LPBITMAPINFO)lpbi,
                        DIB_RGB_COLORS,
                        rop);

    GlobalUnlock(hdib);
    return f;
}

int CDib::Create(int Width,int Height)
{
 if (Height<0) Height *= -1;
 int RealWidth = (Width+3) & (~3);

 Free();
 m_hDib = GlobalAlloc(	GMEM_MOVEABLE,
						  sizeof(BITMAPINFOHEADER)
						+ 256*sizeof(RGBQUAD)
						+ RealWidth*Height 
						);
 if (!m_hDib) return 0;

 CDibLock p(m_hDib);
 BITMAPINFOHEADER	*pHeader	= p.GetHeader();
 RGBQUAD			*pPal		= p.GetPal();

 memset(pHeader,0,sizeof(BITMAPINFOHEADER));
 pHeader->biSize = sizeof(BITMAPINFOHEADER);
 pHeader->biWidth = Width;
 pHeader->biHeight = -Height;
 pHeader->biPlanes = 1;
 pHeader->biBitCount = 8;
 pHeader->biCompression = BI_RGB;
 pHeader->biSizeImage = 0;
 pHeader->biXPelsPerMeter = 0;
 pHeader->biYPelsPerMeter = 0;
 pHeader->biClrUsed = 0;
 pHeader->biClrImportant = 0;

 HDC hDc = ::GetDC(::GetDesktopWindow());
 GetSystemPaletteEntries(hDc,0,256,(LPPALETTEENTRY)pPal);
 ::ReleaseDC(::GetDesktopWindow(),hDc);

 return 1;
}

CDib& CDib::operator =(CDib &a)
{
 CDibLock la(&a);

 if (la.Is32K())
		{
		 Create32K(la.GetWidth(),la.GetHeight());
		 {
		  CDibLock l(this);
		  memcpy(l.GetImg(),la.GetImg(),la.GetHeight() * la.GetRealWidth() *2);
		 }
		}
 else	{
		 Create(la.GetWidth(),la.GetHeight());
		 {
		  CDibLock l(this);
		  memcpy(l.GetPal(),la.GetPal(),1024);
		  memcpy(l.GetImg(),la.GetImg(),la.GetHeight() * la.GetRealWidth());
		 }
		}
 return a;
}

void CDib::Resize(int W,int H)
{
 CDib temp;
 int w,h;
 int k;

 {
  CDibLock l(this);
  
  k = l.Is32K() ? 2 : 1;
  w = l.GetWidth();
  h = l.GetHeight();
  if ( (w==W) && (h==H) ) return;
 }
 temp = *this;
 Free();
 
 if (k==1)	
	{
	 Create(W,H);
	 {
	  CDibLock In(&temp),Out(this);
	  int x,y;
	  BYTE *p = Out.GetImg(), *q;
	  memcpy(Out.GetPal(),In.GetPal(),1024);
	  
	  for(y=0;y<H;y++)
	  {
	   q = In.GetImg() + MulDiv(y,h,H) * In.GetRealWidth();
	   for(x=0;x<W;x++)
	   {
		p[x] = q[MulDiv(x,w,W)];
	   }
	   p += Out.GetRealWidth();
	  }
	 }
	}
 else
	{
	 Create32K(W,H);
	 {
	  CDibLock In(&temp),Out(this);
	  int x,y;
	  WORD *p = (WORD*)Out.GetImg(), *q;
	  
	  for(y=0;y<H;y++)
	  {
	   q = ((WORD*)In.GetImg()) + MulDiv(y,h,H) * In.GetRealWidth();
	   for(x=0;x<W;x++)
	   {
		p[x] = q[MulDiv(x,w,W)];
	   }
	   p += Out.GetRealWidth();
	  }
	 }
	}
}

int CDib::Create32K(int Width,int Height)
{
 if (Height<0) Height *= -1;
 int RealWidth = (Width+1) & (~1);

 Free();
 m_hDib = GlobalAlloc(	GMEM_MOVEABLE,
						  sizeof(BITMAPINFOHEADER)
						+ RealWidth*Height*2 
						);
 if (!m_hDib) return 0;

 CDibLock p(m_hDib);
 BITMAPINFOHEADER	*pHeader	= p.GetHeader();

 memset(pHeader,0,sizeof(BITMAPINFOHEADER));
 pHeader->biSize = sizeof(BITMAPINFOHEADER);
 pHeader->biWidth = Width;
 pHeader->biHeight = -Height;
 pHeader->biPlanes = 1;
 pHeader->biBitCount = 16;
 pHeader->biCompression = BI_RGB;
 pHeader->biSizeImage = 0;
 pHeader->biXPelsPerMeter = 0;
 pHeader->biYPelsPerMeter = 0;
 pHeader->biClrUsed = 0;
 pHeader->biClrImportant = 0;

 return 1;
}

void CDib::Free()
{
 if (m_hDib) GlobalFree(m_hDib);
 m_hDib = NULL;
}

BOOL CDib::Is32K()
{
 CDibLock Lck(this);
 BITMAPINFOHEADER *h;

 h=Lck.GetHeader();
 if (!h) return FALSE;

 return h->biBitCount == 16;
}

CDibLock::CDibLock(CDib *pDib)
{
 if (!pDib) 
	{
	 m_pDib = NULL;
	 return;
	}
 m_hDib = pDib->m_hDib;
 m_pDib = (char*)GlobalLock(m_hDib);
 if (!m_pDib) 
	{
	 m_pDib = NULL;
	}
}

CDibLock::CDibLock(HDIB hDib)
{
 m_hDib = hDib;
 m_pDib = (char*)GlobalLock(m_hDib);
}

CDibLock::~CDibLock()
{
 GlobalUnlock(m_hDib);
}

int CDibLock::GetRealWidth()
{
 if (Is32K())	return (GetWidth()+1) & (~1);
 else			return (GetWidth()+3) & (~3);
}

BOOL CDib::Blt(CDC *pDC,	int DCx,int DCy, int DCw, int DCh,
				int DIBx,int DIBy,
				LONG rop)
{
 int DIBw,DIBh;

 if (!m_hDib) return 0;
 {
  CDibLock lDib(this);

  DIBw = lDib.GetWidth();
  DIBh = lDib.GetHeight();
  if (DIBh<0) DIBh = -DIBh;
  if (!DCw) DCw = DIBw;
  if (!DCh) DCh = DIBh;
 }
 
 return StretchDibBlt(pDC->m_hDC,DCx,DCy,DCw,DCh,m_hDib,DIBx,DIBy,DIBw,DIBh,rop);
}

void CDibLock::SetVgaPal(unsigned char *pPal)
{
 RGBQUAD *DibPal = GetPal();
 for(int i=0;i<256;i++)
 {
  DibPal->rgbRed   = (*pPal++)<<2;
  DibPal->rgbGreen = (*pPal++)<<2;
  DibPal->rgbBlue  = (*pPal++)<<2;
  DibPal++;
 }
}


extern "C"
{
 static Box *box_buff;
 static WORD *pPixel;
 static int  nPixel;


 int get_pixel2(UCHAR *pixel) {return get_pixel(pixel);}
 int get_pixel(UCHAR *pixel)
 {
  WORD a;

  if (!nPixel) return 0;
  
  a = *pPixel++;
  pixel[RED]   = (a>>10) & 0x1F;
  pixel[GREEN] = (a>> 5) & 0x1F;
  pixel[BLUE]  =  a      & 0x1F;

  nPixel--;
  return 1;
 }

 Box *get_box_tmp(int n)
 {
	return &box_buff[n];
 }

 void free_box(int n) {}

 Box *get_box(int n)
 {
	return &box_buff[n];
 }

 
}

 void InitQuantBoxes()
 {
	box_buff	= new Box[256];
	RGBmap	= new unsigned char[BYTE_COUNT];
	Histogram = new unsigned long[BYTE_COUNT];
  	
	memset(box_buff,0, 256*sizeof(Box));
	memset(RGBmap,0,BYTE_COUNT);
	memset(Histogram,0, BYTE_COUNT*sizeof(long));
  
 }

 void CloseQuantBoxes()
 {
	delete [] Histogram;
	delete [] RGBmap;
	delete [] box_buff;
 }

void CDib::Quantize()
{
 CDib		NewDib;		
  
 if (!Is32K()) return;

 {
  CDibLock	l(this);
  NewDib.Create(l.GetWidth(),l.GetHeight());
 }

 {
  CDibLock	l(this),
			lNew(&NewDib);
  unsigned char *pPixelNew = lNew.GetImg();

  InitQuantBoxes();
  
  
  pPixel = (WORD*)l.GetImg();
  nPixel = l.GetWidth()*l.GetHeight();

  QuantHistogram(get_box(0));
  memset(palette,0,768);
  int colors = colorquant(255,INPUT_BITS,0,(double) (255.0 / 31.0));

  pPixel = (WORD*)l.GetImg();
  nPixel = l.GetWidth()*l.GetHeight();
  for(int i=0;i<l.GetWidth()*l.GetHeight();i++)
  {
   UCHAR pixel[3];

   get_pixel(pixel);
   *pPixelNew++ = 1+pal_index(pixel);
  }

  RGBQUAD *pPal = lNew.GetPal();
  pPal[0].rgbRed   = 0;
  pPal[0].rgbGreen = 0;
  pPal[0].rgbBlue  = 0;
  pPal[0].rgbReserved = 0;
  for(i=0;i<255;i++)
  {
   pPal[i+1].rgbRed   = palette[i][RED];
   pPal[i+1].rgbGreen = palette[i][GREEN];
   pPal[i+1].rgbBlue  = palette[i][BLUE];
   pPal[i+1].rgbReserved = 0;
  }

  CloseQuantBoxes();
 }

 *this = NewDib;
}


CDibDC::CDibDC(CDib *pDib)
{
 int w,h;
 BOOL is32K;
 
 CreateCompatibleDC(NULL);
 m_pDib = pDib;

 {
  CDibLock l(pDib);
  w		= l.GetWidth();
  h		= l.GetHeight();
  is32K	= l.Is32K();

  m_Bmp.CreateBitmap(w,h,1, is32K?16:8, l.GetImg());

  m_oldBitmap = SelectObject(&m_Bmp);
 }
}

CDibDC::~CDibDC()
{
 SelectObject(m_oldBitmap);
 DeleteDC();
 m_pDib->CreateFromBitmap(&m_Bmp);
}


int CDib::LoadGIF(const char *fname)
{
 FILE *f;
 long l,t,w,h,colors,bgnd,bpp;
 char palI[768],pal[768];
 GifRecordType grt;
 int More = 1;

 f = DGifOpenFile(fname);
 if (!f) return 0;

 DGifGetScreenDesc(&w,&h,&colors,&bgnd,&bpp,pal);

 do
 {
  DGifGetRecordType(&grt);
  switch (grt)
  {
   case IMAGE_DESC_RECORD_TYPE :
				 DGifGetImageDesc(&l,&t,&w,&h,palI);
				 if (!Create(w,h)) {DGifCloseFile(); return 0;}
				 {
				  CDibLock l(this);
				  BYTE *p;
				  RGBQUAD *pPal = l.GetPal();
				  int iLine = 0;

				  for(int i=0;i<h;i++)
				  {
				   p = l.GetImg() + iLine * l.GetRealWidth();
				   DGifGetLine((char*)p,l.GetWidth());
				   DGifIncLine(&iLine);
				  }
				  memset(pPal,0,256*sizeof RGBQUAD);
				  for(i=0;i<256;i++)
				  {
				   pPal->rgbRed   = pal[3*i+0];
				   pPal->rgbGreen = pal[3*i+1];
				   pPal->rgbBlue  = pal[3*i+2];
				   pPal++;
				  }
				 }
				 break;
   case EXTENSION_RECORD_TYPE :
			More = 0;
			break;
   case TERMINATE_RECORD_TYPE :
			More = 0;
			break;
   default :
			break;
  }
 } while (More);
 DGifCloseFile();
 return 1;
}

int CDib::SaveGIF(const char *filename)
{
 CDibLock l(this);
 BYTE pal[768],*p;
 RGBQUAD *rgb;
 int Left=0,Top=0;
 
 if (0!=EGifOpenFileName(filename)) return 0;

 if (Is32K()) Quantize();

 p=pal;
 rgb=l.GetPal();
 for(int i=0;i<256;i++)
 {
  p[0] = rgb->rgbRed;
  p[1] = rgb->rgbGreen;
  p[2] = rgb->rgbBlue;
 
  p+=3;rgb++;
 }
 EGifPutScreenDesc( l.GetWidth(),  l.GetHeight(),  8,  0,  8, (char*)pal);
 EGifPutImageDesc( Left,  Top,  l.GetWidth(),  l.GetHeight(), 8, FALSE);
 p=l.GetImg();
 for(i=0;i<l.GetHeight();i++)
 {
  EGifPutLine((char*)p,l.GetWidth());
  p+=l.GetRealWidth();
 }
 EGifCloseFile();
 return 1;
}
