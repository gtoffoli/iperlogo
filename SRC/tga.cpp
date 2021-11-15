#include "stdafx.h"
#include "dib.h"
#include <malloc.h>

#pragma pack(1)
struct sTargaHeader
{
 unsigned char	IdLen;			// 0
 unsigned char	CMapType;		// 1
 unsigned char	SubType;		// 2
 unsigned char	x0;				// 3
 unsigned char	x1;				// 4
 unsigned short	MapLen;			// 5,6
 unsigned char	x2,x3,x4,x5,x6;	// 7,8,9,10,11
 unsigned short	Width,			// 12,13
				Height;			// 14,15
 unsigned char	PixelSize;		// 16
 unsigned char	Flags;			// 17

 int IsBottomUp()		{return !(Flags & 0x20);}
 int InterlaceType()	{return Flags >> 6;}
 int DecodePossible();
};
#pragma pack()

int sTargaHeader::DecodePossible()
{
	if (   (CMapType  > 1)	
		|| (PixelSize < 1) 
		|| (PixelSize > 32)
		|| ((Flags & 7)!=0 )
        || (InterlaceType()!= 0)
	   ) return 0;
	return 1;
}

static int BlockCount,DupPixelCount,Last=0;
static int	(*fnReadPixel)(int PixelSize, FILE *f);
static unsigned short Remap[256];

int ReadPixel(int PixelSize, FILE *f)
{
 unsigned short w;
 int			r,g,b;
 
 switch (PixelSize)
 {
  case  8 : return Remap[fgetc(f)]; 
  case 15 : 
  case 16 : fread(&w,sizeof(WORD),1,f);return w; 
  case 24 : b = fgetc(f); g = fgetc(f); r = fgetc(f);
			return ((r>>3)<<10)|((g>>3)<<5)|(b>>3);
  case 32 : b = fgetc(f); g = fgetc(f); r = fgetc(f); fgetc(f);
			return ((r>>3)<<10)|((g>>3)<<5)|(b>>3);
 }
 return 0;
}

int ReadPixelRLE(int PixelSize, FILE *f)
{
 int			i;
 

 if (DupPixelCount>0) {DupPixelCount--;return Last;}
 if (--BlockCount < 0)
	{
	 i = fgetc(f);
	 if (i & 0x80) // start dup
		{ 
		 DupPixelCount = i & 0x7F;
		 BlockCount = 0;
		}
	 else
		{
		 BlockCount = i &0x7F;
		}
	}
 Last = ReadPixel(PixelSize,f);
 return Last;
}

static void ReadTGA_Color(sTargaHeader *Header,FILE *f,unsigned short *Buffer)
{
 unsigned short *p;
 int w = Header->Width;
 if (w&1) w++;
 for(int l=0;l<Header->Height;l++)
 {
  if (Header->IsBottomUp()) 
		p = Buffer + w * (Header->Height-l-1);
  else  p = Buffer + w * l;
  for(int c=0;c<Header->Width;c++)
  {
   *p++ = fnReadPixel(Header->PixelSize,f);
  }
 }
}

static void ReadTGA_BW(sTargaHeader *Header,FILE *f,unsigned short *Buffer)
{
}

int CDib::LoadTGA(const char *fname)
{
 FILE *f = fopen(fname,"rb");
 sTargaHeader Header;

 if (!f) return 0;
 fread(&Header,18,1,f);
 if (!Header.DecodePossible()) {fclose(f);return 0;}
 if (!Create32K(Header.Width,Header.Height)) {fclose(f);return 0;}

 CDibLock lDib(this);
 unsigned short *Buffer = (unsigned short*)lDib.GetImg();
 
 BlockCount = DupPixelCount = 0;
 if (Header.SubType>8)	{fnReadPixel = ReadPixelRLE;Header.SubType-=8;}
 else					{fnReadPixel = ReadPixel;}
 switch (Header.SubType)
 {
  case 1 : // ColorMap
		   ReadTGA_Color(&Header,f,Buffer);
		   break;
  case 2 : // RGB	
		   switch (Header.PixelSize)
		   {
		    case 15 :
			case 16 : 
			case 24 : 
			case 32 : ReadTGA_Color(&Header,f,Buffer);break;
			default : fclose(f); return 0;
		   }
		   break;
  /*
  case 3 : // B&W
		   ReadTGA_BW(&Header,f,Buffer);
		   break;
  */
  default: fclose(f); return 0;
 }
 fclose(f);
 return 1;
}

int CDib::SaveTGA(const char *fname)
{
 FILE *f = fopen(fname,"wb");
 sTargaHeader Header;
 WORD *p;
 
 if (!f) return 0;
 
 CDibLock l(this);
 memset(&Header,0,sizeof(Header));
 
 Header.IdLen		= 0;
 Header.CMapType	= 0;
 Header.SubType		= 2;
 Header.MapLen		= 0;
 Header.Width		= l.GetWidth();
 Header.Height		= l.GetHeight();
 Header.PixelSize	= 16;
 Header.Flags		= 0;

 fwrite(&Header,sizeof(Header),1,f);
 
 p = (WORD*)l.GetImg();
 for(int i=0; i<l.GetHeight();i++)
 {
  fwrite(p,l.GetWidth(),2,f);
  p+= l.GetRealWidth();
 }
 fclose(f);
 return 1;
}
