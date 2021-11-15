#ifndef DIB_H
#define DIB_H


typedef HANDLE HDIB;

HDIB FAR BitmapToDIB(HBITMAP hBitmap, HPALETTE hPal);
BOOL StretchDibBlt (HDC hdc, int x, int y, int dx, int dy, 
				    HDIB hdib, int x0, int y0, int dx0, int dy0, LONG rop);
BOOL DibBlt (HDC hdc, int x0, int y0, int dx, int dy, HANDLE hdib, int x1, int y1, LONG rop);

class CDib:public CObject
{
 public :
 HDIB	m_hDib;
 
 int  Create(int Width,int Height);
 int  Create32K(int Width,int Height);
 int  CreateFromBitmap(CBitmap *bmp);

 void Free();
 BOOL Is32K();

 BOOL StretchBlt(CDC *pDC, int DCx,int DCy, int DCw, int DCh,
					  int DIBx, int DIBy, int DIBw, int DIBh,
					  LONG rop)
		{
			if (m_hDib) 
					return StretchDibBlt(pDC->m_hDC,DCx,DCy,DCw,DCh,m_hDib,DIBx,DIBy,DIBw,DIBh,rop);
			else	return 0;
		}
 BOOL Blt(CDC *pDC,	int DCx,int DCy, int DCw=0, int DCh=0,
				int DIBx=0,int DIBy=0,
				LONG rop=SRCCOPY);

 int LoadGIF(const char *fname);
 int SaveGIF(const char *fname);
 int LoadTGA(const char *fname);
 int SaveTGA(const char *fname);
 void Resize(int w,int h);

 void Quantize(); // 32K->256

 CDib(){m_hDib=NULL;}
 //CDib(HDIB hDib){m_hDib = hDib;}
 ~CDib(){Free();}
 CDib& operator =(CDib &a);
};


inline int ABSVALUE(int a) {return a>=0?a:-a;}

class CDibLock
{
 protected:
 HDIB	m_hDib;
 char	*m_pDib;
 
 public:
 CDibLock(CDib *pDib);
 CDibLock(HDIB hDib);
 ~CDibLock();

 BITMAPINFOHEADER	*GetHeader()	{return (BITMAPINFOHEADER*)m_pDib;}
 
 BOOL Is32K()	 {return  GetHeader()->biBitCount==16;}
 int GetWidth()	 {return  GetHeader()->biWidth;}
 int GetHeight() {return ABSVALUE(GetHeader()->biHeight);}
 int GetRealWidth();
 
 RGBQUAD			*GetPal()		{return Is32K() ? NULL : (RGBQUAD*)(m_pDib+sizeof(BITMAPINFOHEADER));}
 unsigned char		*GetImg()		{return (unsigned char *)(m_pDib+sizeof(BITMAPINFOHEADER)
																	+(Is32K() ? 0 : 256*sizeof(RGBQUAD))
															  );}

 void SetVgaPal(unsigned char *pPal); // 768 [0..63] values
};

class CDibDC:public CDC
{
 protected:
 CPalette	*m_oldPalette;
 CBitmap	*m_oldBitmap;
 CBitmap	m_Bmp;
 CDib		*m_pDib;

 public:
 CDibDC(CDib *pDib);
 ~CDibDC();
};

#endif