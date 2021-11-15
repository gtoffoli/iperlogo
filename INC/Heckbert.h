/************************************************************************
 *                                                                      *
 *                  Copyright (c) 1991, Frank van der Hulst             *
 *                          All Rights Reserved                         *
 *                                                                      *
 * Authors:                                                             *
 *                FvdH - Frank van der Hulst (Wellington, NZ)           *
 *                                                                      *
 * Versions:                                                            *
 *    V1.1 910626 FvdH - QUANT released for DBW_RENDER                  *
 *    V1.2 911021 FvdH - QUANT released for PoV Ray                     *
 *    V1.4 920303 FvdH - Ported to GNU                                  *
 *    V1.6 921023 FvdH - Produce multi-image GIFs                       *
 *                     - Port to OS/2 IBM C Set/2                       *
 *                                                                      *
 ************************************************************************/
/* heckbert.h -- definitions for heckbert.c, virt_mem.c */

struct BOX {
	double			weightedvar,			/* weighted variance	*/
					mean[3];				/* centroid				*/
	unsigned long 	weight,					/* # of pixels in box	*/
					freq[3][MAXCOLORS];		/* Projected frequencies */
	int 			low[3], high[3];		/* Box extent			*/
};
typedef struct BOX Box;

#ifdef __cplusplus
extern "C"
{
#endif
Box *get_box(int n);
Box *get_box_tmp(int n);
void free_box(int n);
void open_box_file(int colors);
void close_box_file(void);

#define IN_COLOURS (1 << INPUT_BITS)
#define BYTE_COUNT ((long)IN_COLOURS * IN_COLOURS * IN_COLOURS)
			/* (2^INPUT_BITS)^3    */

#ifndef IN_HECKBERT
extern unsigned long *Histogram;
extern unsigned char *RGBmap;
#endif //IN_HECKBERT

int colorquant(int colors, int bits, int fast, double Cfactor);
void QuantHistogram(Box *box);
void QuantHistogram2(Box *box);

#ifdef __cplusplus
}
#endif
