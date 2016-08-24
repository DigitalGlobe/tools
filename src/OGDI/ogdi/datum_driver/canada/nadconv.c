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



/*
 ************************************************************
 *
 * Include files
 *
 ************************************************************
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef _WINDOWS
#include <io.h>
#endif
#include <unistd.h>
#include <fcntl.h>

#include <ogdi_macro.h>





/* 
 ************************************************************
 *
 * Defines and macros
 *
 ************************************************************
 */
#define INTGRID_INT

#ifdef sun
#define NEED_TO_SWAP
#endif

#ifdef NEED_TO_SWAP
#define BYTESWAP(BUFFER, SIZE, COUNT) byteswap(BUFFER, SIZE, COUNT)
#else
#define BYTESWAP(BUFFER, SIZE, COUNT)
#endif



/*
 ************************************************************
 *
 * Structures
 *
 ************************************************************
 */


typedef struct subGridType {
  double alimit[6];
  int agscount, astart;
  char anameg[8], apgrid[8];
} subGridType;
	
typedef struct NAD_DATA {
  int fd2;
  int fd;
  int norecs;				/* # overview header records */
  int nsrecs;				/* # subfile header records */
  int nfiles;				/* # of subfiles */
  int offset;				/* total offset for the file */
  subGridType *subGrid;
  
  int limflag;				/* value on grid limit */
  char typout[10];			/* grid shift units */
  char version[10];			/* version id */
  char fdatum[10];			/* from datum name */
  char tdatum[10];			/* to datum name */
  double tellips[2];			/* major/minor to axis */
  double fellips[2];			/* major/minor from axis */
  double diflat;				/* interpolated lat shifts */
  double diflon;				/* interpolated lon shifts */

#ifdef ACCURACIES
  double varx;				/* interpolated lat accuracy */
  double vary;				/* interpolated lon accuracy */
#endif

} NAD_DATA;

typedef struct gridDataType {
  char title[8];
  union {
    double d;
    int i;
    char c[8];
  } value;
} gridDataType;
 



/*
 ************************************************************
 *
 * Prototypes
 *
 ************************************************************
 */
#include "nadconv.h"
static int  gridint(NAD_DATA *nadPtr, double lat, double lon, int filen);
static int  fgrid(NAD_DATA *nadPtr, double lat, double lon);






/*
 ************************************************************
 *
 * Functions and procedures
 *
 ************************************************************
 */


#ifdef NEED_TO_SWAP
/*
 * Byte swapping routines required for binary compatibility
 * between platforms.  The ntv2_0.gsb file is in intel byte order.
 */

static void byteswap(void* buffer, int size, int count);

void 
byteswap( buf, size, num )
     void *buf;
     short int size;
     short int num;
{
  /*
   * History: 11/92  Todd Stellhorn  - original coding
   */
  short int      i, j;
  unsigned char  *p, tmp;

  p = ( unsigned char * ) buf;
  for (i = 0; i < num; i++) {
    for (j = 0; j < ( size >> 1 ); j++) {
      tmp = p[j];
      p[j] = p[size - j - 1];
      p[size - j - 1] = tmp;
    }
    p += size;
  }
} 
#endif


/*
 * Search the subgrid hierarchy to determine the subgrid that
 * the point falls into.
 */
static int 
fgrid(nadPtr,lat,lon)
     NAD_DATA *nadPtr;
     double lat,lon;
{
  int i;
  int filen;
  int onlimit;
  int ifnd[4];
  char oldnam[12];
  char name[12];
  
  strcpy(name,"NONE");
  filen = 0;


  /*
   * Search down through the hierarchical list of
   * sub-grids until a final match is found.
   */
  do {
    strcpy(oldnam,name);
    onlimit = 0;
    for(i=0; i<4; i++) 
      ifnd[i] = -1;
     
    /*
     * At each iteration, search the entire list.
     */
    for(i=0;i<nadPtr->nfiles;i++) {
      
      /*
       * Match on an indentical sub-grid name, and always
       * match on the first record (which contains
       * the boundary of the entire area).
       */
      if((!strncmp(name,nadPtr->subGrid[i].apgrid,6)) || 
	 (nadPtr->nfiles == 1)) {
	
	/*
	 * A simple box test to see if the point is within this
	 * sub-grid.
	 */
	if((lat >= nadPtr->subGrid[i].alimit[0]) && 
	   (lat <= nadPtr->subGrid[i].alimit[1]) && 
	(lon >= nadPtr->subGrid[i].alimit[2]) && 
	   (lon <= nadPtr->subGrid[i].alimit[3])) {

	  /*
	   * Determine if the point falls on a boundary, give
	   * priority to sub-grids where the point is not
	   * on a boundary.
	   */
	  if ((lat < nadPtr->subGrid[i].alimit[1]) && 
	      (lon < nadPtr->subGrid[i].alimit[3])) {
	    onlimit = 0;
	  }
	  else if ((lat == nadPtr->subGrid[i].alimit[1]) && 
		   (lon < nadPtr->subGrid[i].alimit[3])) {
	    onlimit = 1;
	  }
	  else if ((lat < nadPtr->subGrid[i].alimit[1]) && 
		   (lon == nadPtr->subGrid[i].alimit[3])) {
	    onlimit = 2;
	  }
	  else {
	    onlimit = 3;
	  }
		  
	  if (ifnd[onlimit] == -1) {
	    ifnd[onlimit] = i;
	  }
	  else {
	    return -1;
	  }
	}
      }
    }

    /*
     * Now choose the grid that has the best fit for this point.
     */
    for (i=0; i<4; i++) {
      if(ifnd[i] > -1) {
	filen = ifnd[i];
	nadPtr->limflag = i;
	strncpy(nadPtr->subGrid[filen].anameg, name, 8);
	break;
      }
    }
      
  } while (strncmp(oldnam,name,8) != 0);

  return filen;
}


/*
 * Interpolate an adjustment within a subgrid.
 */
static int 
gridint(nadPtr,lat,lon,filen)
     NAD_DATA *nadPtr;
     double lat, lon;
     int filen;
{
  double n;			/* distance north se corner */
  double w;			/* distance west se corner */
  static double alat=0.0;		/* se grid value lat sec */
  static double blat=0.0;		/* sw grid value lat sec */
  static double clat=0.0;		/* ne grid value lat sec */
  static double dlat=0.0;		/* nw grid value lat sec */
  static double along=0.0;		/* se grid value long sec */
  static double blong=0.0;		/* sw grid value long sec */
  static double clong=0.0;		/* ne grid value long sec */
  static double dlong=0.0;		/* nw grid value long sec */
#ifdef ACCURACIES
  static double avarx;			/* accuracy lat point a meters */
  static double avary;			/* accuracy long point a meters */
  static double bvarx;			/* accuracy lat point b meters */
  static double bvary;			/* accuracy long point b meters */
  static double cvarx;			/* accuracy lat point c meters */
  static double cvary;			/* accuracy long point c meters */
  static double dvarx;			/* accuracy lat point d meters */
  static double dvary;			/* accuracy long point d meters */
#endif
  
  int nlon;			/* # of lines of long in grid */
  int header;			/* # recs before first grid shift rec */
  static int prev=0;			/* # previous rec transformed */
  int i;				/* # lines lat to input point */
  int j;				/* # lined long to input point */
  int irow,iread,jread;
  float buff[4];

#define EXPON 1e-12
#define GET_REAL(REC, VAR) \
    if (lseek(nadPtr->fd, ((REC)-1)*16, SEEK_SET) == -1) return NAD_ERROR; \
    if (read(nadPtr->fd, &VAR, 16) != 16) return NAD_ERROR; \
    BYTESWAP(&VAR, sizeof(VAR), 1);

  if (!nadPtr)
    return NAD_ERROR;

/* 
 * compute number of lines of longitude in grid 
 */
  nlon = ((int)(((nadPtr->subGrid[filen].alimit[3] - 
	    nadPtr->subGrid[filen].alimit[2]) /
	   nadPtr->subGrid[filen].alimit[5]) + EXPON)) + 1;

/* 
 * computes grid points and checks if they are in memory 
 */
  i = ((int)((lat - nadPtr->subGrid[filen].alimit[0]) / 
       nadPtr->subGrid[filen].alimit[4] + EXPON)) + 1;
  j = ((int)((lon - nadPtr->subGrid[filen].alimit[2]) / 
       nadPtr->subGrid[filen].alimit[5] + EXPON)) + 1;
  header = nadPtr->subGrid[filen].astart - 1;


  /* 
   * search within grid for closest 4 grid points and interpolate 
   * latitude and longitude and interpolate accuracies for lat and lon 
   */ 
  irow = (i-1) * nlon;
  if ((j == 0) || (j > nlon)) {
    iread = irow + nlon;
    jread = irow + 1;
  }
  else {
    iread = irow + j;
    jread = iread + 1;
  }

  
  /*
   * If the adjustment record is in memory, then don't read
   * it again.
   */
  if (prev != (iread + header))	{

    /*
     * Determine the number of observations to read based on
     * proximity to the subgrid edge.
     * limflag 
     *  0 = Interior point, get four observations
     *  1 = lat on edge, get two observations
     *  2 = long on edge, get two observations
     *  3 = both on edge, get one observation
     */
    if (nadPtr->limflag == 0) {
      GET_REAL(iread+header, buff);
      alat = buff[0];
      along = buff[1];
#ifdef ACCURACIES
      avarx = buff[2];
      avary = buff[3];
#endif

      GET_REAL(jread+header, buff);
      blat = buff[0];
      blong = buff[1];
#ifdef ACCURACIES
      bvarx = buff[2];
      bvary = buff[3];
#endif
      
      prev = iread + header;

      iread += nlon;
      jread += nlon;

      GET_REAL(iread+header, buff);
      clat = buff[0];
      clong = buff[1];
#ifdef ACCURACIES
      cvarx = buff[2];
      cvary = buff[3];
#endif

      GET_REAL(jread+header, buff);
      dlat = buff[0];
      dlong = buff[1];
#ifdef ACCURACIES
      dvarx = buff[2];
      dvary = buff[3];
#endif
    }
      
    else if (nadPtr->limflag == 1) {
      GET_REAL(iread+header, buff);
      alat = buff[0];
      along = buff[1];
#ifdef ACCURACIES
      avarx = buff[2];
      avary = buff[3];
#endif

      GET_REAL(jread+header, buff);
      blat = buff[0];
      blong = buff[1];
#ifdef ACCURACIES
      bvarx = buff[2];
      bvary = buff[3];
#endif
      
      prev = iread + header;

      clat = alat;
      clong = along;
      dlat = blat;
      dlong = blong;
#ifdef ACCURACIES
      cvarx = avarx;
      cvary = avary;
      dvarx = bvarx;
      dvary = bvary;
#endif
    }
    else if (nadPtr->limflag == 2) {
      GET_REAL(iread+header, buff);
      alat = buff[0];
      along = buff[1];
#ifdef ACCURACIES
      avarx = buff[2];
      avary = buff[3];
#endif

      prev = iread + header;
      
      blat = alat;
      blong = along;
#ifdef ACCURACIES
      bvarx = avarx;
      bvary = avary;
#endif

      iread += nlon;

      GET_REAL(iread+header, buff);
      clat = buff[0];
      clong = buff[1];
#ifdef ACCURACIES
      cvarx = buff[2];
      cvary = buff[3];
#endif

      dlat = clat;
      dlong = clong;
#ifdef ACCURACIES
      dvarx = cvarx;
      dvary = cvary;
#endif
    }
    else if (nadPtr->limflag == 3) {
      GET_REAL(iread+header, buff);
      alat = buff[0];
      along = buff[1];
#ifdef ACCURACIES
      avarx = buff[2];
      avary = buff[3];
#endif

      prev = iread + header;
      
      blat = alat;
      blong = along;
      clat = alat;
      clong = along;
      dlat = alat;
      dlong = along;

#ifdef ACCURACIES
      bvarx = avarx;
      bvary = avary;
      cvarx = avarx;
      cvary = avary;
      dvarx = avarx;
      dvary = avary;
#endif
    }
  }

  /* 
   * interpolate new position 
   */
  n = (lat - (nadPtr->subGrid[filen].alimit[0] + 
	      (i-1) * nadPtr->subGrid[filen].alimit[4])) / 
    nadPtr->subGrid[filen].alimit[4];
  
  w = (lon - (nadPtr->subGrid[filen].alimit[2] + 
	      (j-1) * nadPtr->subGrid[filen].alimit[5])) / 
    nadPtr->subGrid[filen].alimit[5];

  nadPtr->diflat = alat + (clat - alat) * n + (blat - alat) * 
    w + (alat - blat - clat + dlat) * n * w;

  nadPtr->diflon = along + (clong - along) * n + (blong - along) * 
    w + (along - blong - clong + dlong) * n * w;

#ifdef ACCURACIES
  /* 
   *  interpolation for accuracies 
   */
  nadPtr->varx = avarx + (cvarx - avarx) * n + 
      (bvarx - avarx) * w + (avarx - bvarx - cvarx + dvarx) * n * w;

  nadPtr->vary = avary + (cvary - avary) * n + 
      (bvary - avary) * w + (avary - bvary - cvary + dvary) * n * w;
#endif

  return NAD_OK;
}


/*
 * Perform a forward adjustment of the point.
 */
int
NAD_Forward(nadPtr, lat, lon) 
     NAD_DATA *nadPtr;
     double *lat, *lon;
{
  int filen;

  if (!nadPtr)
    return NAD_ERROR;

  if ((filen = fgrid(nadPtr, *lat, *lon)) < 0) 
    return NAD_ERROR;

  if (gridint(nadPtr, *lat, *lon, filen) != NAD_OK)
    return NAD_ERROR;

  *lat = *lat + nadPtr->diflat;
  *lon = *lon + nadPtr->diflon;

  return NAD_OK;
}


/*
 * Perform a reverse adjustment of the point.
 */
int
NAD_Reverse(nadPtr, lat, lon)
     NAD_DATA *nadPtr;
     double *lat, *lon;
{
  int filen, i;
  double tlat, tlong;

  if (!nadPtr)
    return NAD_ERROR;

  if ((filen = fgrid(nadPtr, *lat, *lon)) < 0) 
    return NAD_ERROR;

  nadPtr->diflat = 0.0;
  nadPtr->diflon = 0.0;

  for (i=0; i<4; i++) {
    tlat = *lat - nadPtr->diflat;
    tlong = *lon - nadPtr->diflon;
    if (i>0) {
      if ((filen = fgrid(nadPtr, tlat, tlong)) < 0) 
	return NAD_ERROR;
    }
    if (gridint(nadPtr, tlat, tlong, filen) != NAD_OK)
      return NAD_ERROR;
  }

  *lat = *lat - nadPtr->diflat;
  *lon = *lon - nadPtr->diflon;

  return NAD_OK;
}


#define GET_INT(REC, VAR) \
    lseek(nadPtr->fd, ((REC)-1)*16, SEEK_SET); \
    ogdi_read(nadPtr->fd, &buff, 16); \
    VAR = buff.value.i; \
    BYTESWAP(&VAR, sizeof(VAR), 1); 

#define GET_CHAR(REC, VAR) \
    lseek(nadPtr->fd, ((REC)-1)*16, SEEK_SET); \
    ogdi_read(nadPtr->fd, &buff, 16); \
    strncpy(VAR, buff.value.c, 8); \
    {char *s; for(s=(VAR)+7; s>=(VAR) && (*s==0 || *s == ' '); *s--=0);}

#define GET_DBL(REC, VAR) \
    lseek(nadPtr->fd, ((REC)-1)*16, SEEK_SET); \
    ogdi_read(nadPtr->fd, &buff, 16); \
    VAR = buff.value.d; \
    BYTESWAP(&VAR,sizeof(VAR),1); 

/*
 * Initialize the conversion by reading in the subgrid headers.
 */
NAD_DATA* 
NAD_Init(filename, fdatum, tdatum)
     char *filename;
     char *fdatum;
     char *tdatum;
{
  NAD_DATA *nadPtr;
  int i, j, count;
  gridDataType buff;

  nadPtr = calloc(1,sizeof(NAD_DATA));
  if (!nadPtr) {
    return NULL;
  }
  nadPtr->subGrid = NULL;

#ifdef _WINDOWS
  nadPtr->fd = open(filename, O_RDONLY | O_BINARY);
#else
  nadPtr->fd = open(filename, O_RDONLY );
#endif
  if (nadPtr->fd < 0) {
    free(nadPtr);
    return NULL;
  }

  nadPtr->offset = 0;

  GET_INT(1,nadPtr->norecs);
  GET_INT(2,nadPtr->nsrecs);
  GET_INT(3,nadPtr->nfiles);
  GET_CHAR(4,nadPtr->typout);
  GET_CHAR(5,nadPtr->version);
  GET_CHAR(6,nadPtr->fdatum);
  GET_CHAR(7,nadPtr->tdatum);
  GET_DBL(8,nadPtr->fellips[0]);
  GET_DBL(9,nadPtr->fellips[1]);
  GET_DBL(10,nadPtr->tellips[0]);
  GET_DBL(11,nadPtr->tellips[1]);

  /*
   * Confirm that the source and target datums are correct.
   */
  if ((strncmp(fdatum, nadPtr->fdatum, 8) != 0) ||
      (strncmp(tdatum, nadPtr->tdatum, 8) != 0)) {
    NAD_Close(nadPtr);
    return NULL;
  }

  /*
   * Allocate space to hold the subgrid records.
   */
  nadPtr->subGrid = calloc(nadPtr->nfiles, sizeof(subGridType));
  if (!nadPtr->subGrid) {
    NAD_Close(nadPtr);
    return NULL;
  }

  /*
   * Loop through all of the subgrid header records.  Skip
   * over the actual adjustment data (don't read the detail until
   * later).
   */
  count = nadPtr->norecs;
  for(i=0;i<nadPtr->nfiles;i++) {
    count++;
    /*
     * Read the name of the sub grid, and validate that this
     * is a correct record.
     */
    GET_CHAR(count, nadPtr->subGrid[i].anameg);
    if (strncmp(buff.title, "SUB_NAME", 8) != 0) {
      NAD_Close(nadPtr);
      return NULL;
    }
    count++;
    GET_CHAR(count, nadPtr->subGrid[i].apgrid);
    count += 3;
    /*
     * Read the limits of this subgrid.
     */
    for (j=0; j<6; j++) {
      GET_DBL(count, nadPtr->subGrid[i].alimit[j]);
      count++;
    }
    GET_INT(count,  nadPtr->subGrid[i].agscount);
    nadPtr->subGrid[i].astart = count + 1;
    count += nadPtr->subGrid[i].agscount;
  }

  return nadPtr;
}


/*
 * Close the grid file and release memory.
 */
void
NAD_Close(nadPtr)
     NAD_DATA *nadPtr;
{
  if (!nadPtr)
    return;
  if (nadPtr->fd)
    close(nadPtr->fd);
  if (nadPtr->subGrid)
    free(nadPtr->subGrid);
  free(nadPtr);
}
