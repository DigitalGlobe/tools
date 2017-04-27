/*
 * NAD to NAD conversion program.
 *
 * This package has four functions:
 *  NAD_DATA* NAD_Init(char *filename, char *fdatum, char *tdatum)
 *    - Pass this routine the name of the binary grid data
 *      file that contains the adjustment data.
 *    - The routine returns a pointer to a structure containing
 *      lookups to the adjustment subgrids.
 *    - Checks that the adjustment file really contains the
 *      requested datums (source datum == fdatum and target data == tdatum).
 *      If not returns a NULL;
 *    - Returns NULL on any other error.
 *  int NAD_Forward(NAD_DATA *nadPtr, double *lat, double *lon)
 *    - Pass a lat/long pair and it will adjust forward according
 *      to the adjustments in the file.  
 *    - Converted values are returned in place.
 *    - Returns NAD_OK upon success.
 *  int NAD_Reverse(NAD_DATA *nadPtr, double *lat, double *lon)
 *    - Similar to NAD_Reverse, but performs reverse correction.
 *  void NAD_Close(NAD_DATA *nadPtr)
 *    - Closes grid file and frees memory.
 *
 * Note that the conversion routines convert lat/long values
 * specifed as decimal seconds:
 *  D*3600 + M*60 + seconds/60
 *
 * Tom Moore, March 1997
 * William Lau, January 1997
 *
 * Based on the Fortran program INTGRID.
 *
 *
 * Copyright (C) 1997 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef INTGRID_H
#define INTGRID_H


/* 
 ************************************************************
 *
 * Defines and macros
 *
 ************************************************************
 */
#define NAD_OK    0
#define NAD_ERROR 1



/*
 ************************************************************
 *
 * Structures
 *
 ************************************************************
 */
/*
 * Declare an opaque pointer to the private data structure
 */
#ifndef INTGRID_INT
typedef void* NAD_DATA;
#endif

/*
 ************************************************************
 *
 * Prototypes
 *
 ************************************************************
 */
NAD_DATA   * NAD_Init(char *filename, char *fdatum, char *tdatum);
void         NAD_Close(NAD_DATA *nadPtr);
int          NAD_Forward(NAD_DATA *nadPtr, double *lat, double *lon);
int          NAD_Reverse(NAD_DATA *nadPtr, double *lat, double *lon);

#endif

typedef struct N_COORD {
	int dlatn;				/* output latitude degrees */
	int dlonn;				/* output longitude degrees */
	int mlatn;				/* output latitude minutes */
	int mlonn;				/* output longitude minutes */
	double slatn;				/* output latitude */
	double slonn;				/* output longitude */

} N_COORD;
