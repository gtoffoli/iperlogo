/******************************************************************************
* In order to make life a little bit easier when using the GIF file format,   *
* this library was written, and which does all the dirty work...	      *
*									      *
*					Written by Gershon Elber,  Jun. 1989  *
*******************************************************************************
* History:								      *
* 14 Jun 89 - Version 1.0 by Gershon Elber.				      *
*  3 Sep 90 - Version 1.1 by Gershon Elber (Support for Gif89, Unique names). *
******************************************************************************/


#ifndef GIF_LIB_H
#define GIF_LIB_H

#ifdef __cpluplus
extern "C" {
#endif

#include <stdio.h>
#define GIF_LIB_VERSION	" Version 1.1, "

#define	GIF_ERROR	0
#define GIF_OK		1

#ifndef TRUE
#define TRUE		1
#define FALSE		0
#endif


typedef	short		      GifBooleanType;
typedef	unsigned char	GifPixelType;
typedef unsigned char *	GifRowType;
typedef unsigned char	GifByteType;

typedef enum  {
              UNDEFINED_RECORD_TYPE,
              SCREEN_DESC_RECORD_TYPE,
	            IMAGE_DESC_RECORD_TYPE,				/* Begin with ',' */
              EXTENSION_RECORD_TYPE,				/* Begin with '!' */
              TERMINATE_RECORD_TYPE				  /* Begin with ';' */
              } GifRecordType;

/******************************************************************************
* O.k. here are the routines one can access in order to decode GIF file:      *
* (GIF_LIB file DGIF_LIB.C).	                                					      *
******************************************************************************/
  #ifdef __cplusplus
  extern "C" {
  #endif

FILE *    DGifOpenFile(const char *GifFileName);
void      DGifGetScreenDesc(long *Width,long *Height,long *ColorRes,long *BackGround,long *BitsPerPixel,char *palette);
short     DGifGetRecordType(GifRecordType *GifType);
void      DGifGetImageDesc(long *Left,long *Top,long *Width,long *Height,char *palette);
void      DGifSetupDecompress(long size);
short     DGifGetLine(char *GifLine,long GifLineLen);
void	  DGifIncLine(int *Line);
void      DGifCloseFile(void);

short     EGifOpenFileName(const char *FileName);
void      EGifPutScreenDesc(long Width,long Height,long ColorRes,long BkGrnd,long BitsPerPixel,char *ColorMap);
void	  EGifPutImageDesc(long Left,long Top,long Width,long Height,long BitsPerPixel,int interlaced);
short     EGifPutLine(char *Line,long LineLen);
void      EGifCloseFile(void);

  #ifdef __cplusplus
  }
  #endif


#ifdef __cpluplus
}
#endif
#endif /* GIF_LIB_H */
