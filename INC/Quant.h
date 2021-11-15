/************************************************************************
 *                                                                      *
 *                  Copyright (c) 1991, Frank van der Hulst             *
 *                          All Rights Reserved                         *
 *                                                                      *
 * Authors:                                                             *
 *      FvdH - Frank van der Hulst (Wellington, NZ)                     *
 *                                                                      *
 * Versions:                                                            *
 *    V1.1 910626 FvdH - QUANT released for DBW_RENDER                  *
 *    V1.2 911021 FvdH - QUANT released for PoV Ray                     *
 *    V1.5 920403 FvdH - Allow any number of files                      *
 *    V1.6 921023 FvdH - Produce multi-image GIFs                       *
 *                     - Port to OS/2 IBM C Set/2                       *
 *                                                                      *
 ************************************************************************/
/* quant.h -- definitions global to quant package */
#define HECKBERT

#include <stdio.h>
#define HUGE_PTR *

#define TRUE		1
#define FALSE		0

/* Readability constants. */
#define RED			0
#define GREEN		1
#define BLUE		2

/* Maximum number of colormap entries.  To make larger than 2^8, the rgbmap
	type will have to be changed from unsigned chars to something larger. */

#define MAXCOLORS		256

#define OUTPUT_BITS 8
#ifdef HECKBERT
#define INPUT_BITS 5
#else
#define INPUT_BITS 8
#endif

#ifdef __TURBOC__
#define CHECK_ALLOC( ptr, type, number, msg) {\
	if ((ptr = (type *) farmalloc((long)sizeof(type) * (unsigned long)(number))) == NULL) {\
		printf("\nCreating %s: MALLOC - out of memory\n", msg);\
		exit(1);\
	 } \
  }
#else
#define CHECK_ALLOC(ptr, type, number, msg) { \
	if ((ptr = (type *) malloc(sizeof(type) * number)) == NULL) { \
		printf("\nCreating %s: malloc - out of memory\n, msg"); \
		exit(1); \
	} \
}
#endif

#define putw(word, file) {\
	putc((word) & 0xff, file);\
	putc((word) >> 8, file);\
	}

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _WINDOWS_
typedef unsigned char		UCHAR;		/* 8 bits */
typedef unsigned short int	UINT;		/* 16 bits */
typedef unsigned long int	ULONG;		/* 32 bits */
#endif

extern UCHAR palette[MAXCOLORS][3];
extern char				disp_image;		/* Display image flag */
extern char				input_type;		/* Input format switch */


void open_file(char *fname, int num);
void err_exit(void);
int get_pixel(UCHAR *p);
int get_pixel2(UCHAR *p);
void write_file(int num_files, char *input_file, int Xres, int Yres,
					 int colors, int output_type);
void close_file(void);

int	pal_index(UCHAR *pixel);
#ifdef __cplusplus
}
#endif