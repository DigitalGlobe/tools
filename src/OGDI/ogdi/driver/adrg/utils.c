/******************************************************************************
 *
 * Component: OGDI ADRG Driver
 * Purpose: Supporting ADRG functions.
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: utils.c,v $
 * Revision 1.8  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.7  2007/02/12 16:09:06  cbalint
 *   *  Add hook macros for all GNU systems, hook fread,fwrite,read,fgets.
 *   *  Handle errors in those macro, if there are any.
 *   *  Fix some includes for GNU systems.
 *   *  Reduce remaining warnings, now we got zero warnings with GCC.
 *
 *  Modified Files:
 *  	config/unix.mak contrib/ogdi_import/dbfopen.c
 *  	contrib/ogdi_import/shapefil.h contrib/ogdi_import/shpopen.c
 *  	ogdi/c-api/ecs_xdr.c ogdi/c-api/ecsinfo.c ogdi/c-api/server.c
 *  	ogdi/datum_driver/canada/nadconv.c ogdi/driver/adrg/adrg.c
 *  	ogdi/driver/adrg/adrg.h ogdi/driver/adrg/object.c
 *  	ogdi/driver/adrg/utils.c ogdi/driver/rpf/rpf.h
 *  	ogdi/driver/rpf/utils.c ogdi/gltpd/asyncsvr.c
 *  	ogdi/glutil/iofile.c vpflib/vpfprim.c vpflib/vpfspx.c
 *  	vpflib/vpftable.c vpflib/vpftidx.c vpflib/xvt.h
 *
 * Revision 1.6  2003/08/27 05:00:06  warmerda
 * Fixed problems with _read_adrg(), _read_overview() and _initRegionWithDefault
 * so that the files are actually closed after use.  As per bug 795612.
 *
 * Revision 1.5  2001/06/22 16:37:50  warmerda
 * added Image support, upgraded headers
 *
 */

#include "adrg.h"

ECS_CVSID("$Id: utils.c,v 1.8 2016/06/28 14:32:45 erouault Exp $");

/* 
   ----------------------------------------------------------
   
   _read_adrg

   extract the information from the .GEN file and put them
   into lpriv.
   
   ---------------------------------------------------------- 
   */

int _read_adrg(s,l)
     ecs_Server *s;
     ecs_Layer *l;
{
  register ServerPrivateData *spriv = s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  FILE *fichier;
  int first;
  char c,sc[4];
  char buffer[20];
  double x,y;
  int i,j,count;
  int isTiled;
  
  fichier = fopen(spriv->genfilename,"r");
  if (fichier == NULL) {
    ecs_SetError(&(s->result),1,"Unable to open the .GEN file");
    return FALSE;
  }

  c = getc(fichier);

  while(!feof(fichier)) {
    if (c==(char) 30) {
      ogdi_fread(sc,3,1,fichier);
      if(strncmp("GIN",sc,3) == 0) {
	first = TRUE;

	/* Jump ^^ et PRT */

	fseek(fichier,7,SEEK_CUR);

	/* Read NAM */

	ogdi_fread(buffer,8,1,fichier);
	strncpy(lpriv->imgname,buffer,8);
	lpriv->imgname[8] = '\0';


	/* Jump ^^ STR LOD LAD UNIloa */

	fseek(fichier,17,SEEK_CUR);

	/* Read SWO SWA NWO NWA NEO NEA SEO SEA */

	for(i=0;i<4;i++) {
	  ogdi_fread(buffer,11,1,fichier);
	  buffer[11] = '\0';
	  x = parse_coord_x(buffer);
	  ogdi_fread(buffer,10,1,fichier);
	  buffer[10] = '\0';
	  y = parse_coord_y(buffer);

	  if (first) {
	    lpriv->region.east = x;
	    lpriv->region.west = x;
	    lpriv->region.north = y;
	    lpriv->region.south = y;
	    first = FALSE;
	  } else {
	    if (x<(lpriv->region.west))
	      lpriv->region.west = x;
	    if (x>(lpriv->region.east))
	      lpriv->region.east = x;
	    if (y<(lpriv->region.south))
	      lpriv->region.south = y;
	    if (y>(lpriv->region.north))
	      lpriv->region.north = y;
	  }
	}

	/* Jump SCA */
	
	fseek(fichier,9,SEEK_CUR);
	
	/* Read ZNA */
	
	ogdi_fread(buffer,2,1,fichier);
	buffer[2] = '\0';
	lpriv->zonenumber = atoi(buffer);
	
	/* Jump PSP IMR */
	
	fseek(fichier,6,SEEK_CUR);
	
	/* Read ARV */
	
	ogdi_fread(buffer,8,1,fichier);
	buffer[8] = '\0';
	lpriv->ARV = atoi(buffer);
	
	/* Read BRV */
	
	ogdi_fread(buffer,8,1,fichier);
	buffer[8] = '\0';
	lpriv->BRV = atoi(buffer);
	
	/* Read LSO PSO */
	
	ogdi_fread(buffer,11,1,fichier);
	buffer[11] = '\0';
	lpriv->LSO = parse_coord_x(buffer);

	ogdi_fread(buffer,10,1,fichier);
	buffer[10] = '\0';
	lpriv->PSO = parse_coord_y(buffer);
	
	/* Jump TXT ^^ NUL NUS NLL NLS */
	
	fseek(fichier,89,SEEK_CUR);
	
	/* Read NFL */

	ogdi_fread(buffer,3,1,fichier);
	buffer[3] = '\0';
	lpriv->rowtiles = atoi(buffer);
	lpriv->rows = lpriv->rowtiles * 128;
	lpriv->region.ns_res = (lpriv->region.north - lpriv->region.south) / (double) lpriv->rows;
	
	/* Read NFC */
	
	ogdi_fread(buffer,3,1,fichier);
	buffer[3] = '\0';
	lpriv->coltiles = atoi(buffer);
	lpriv->columns = lpriv->coltiles * 128;
	lpriv->region.ew_res = (lpriv->region.east - lpriv->region.west) / (double) lpriv->columns;
	
	/* Jump PNC PNL COD ROD POR PCB PVB */
	
	fseek(fichier,17,SEEK_CUR);
	
	/* Read BAD and check if it valid. If not, search another GIN */
	
	ogdi_fread(buffer,12,1,fichier);
#ifdef _WINDOWS
	if (strnicmp(buffer,lpriv->imgfilename,12) != 0) {
#else
	if (strncasecmp(buffer,lpriv->imgfilename,12) != 0) {
#endif
	  c = getc(fichier);
	  continue;
	}
	lpriv->imgfilename[12] = '\0';
	
	/* Read the TIF */
	
	ogdi_fread(buffer,1,1,fichier);
	if (buffer[0] == 'N')
	  isTiled = FALSE;
	else
	  isTiled = TRUE;
	
	
	/* If is tiled, get the TSI. */
	
	if (isTiled) {
	  /* Jump ^^ BID WS1 WS2 ^^ */
	  fseek(fichier,47,SEEK_CUR);
	}
	
	lpriv->tilelist = (int *) malloc(sizeof(int)*lpriv->rowtiles*lpriv->coltiles);
	if (lpriv->tilelist == NULL) {
	  ecs_SetError(&(s->result),1,"Not enough memory");
          fclose( fichier );
	  return FALSE;
	}
	
	count = 0;
	for(i=0;i<lpriv->rowtiles;i++) {
	  for(j=0;j<lpriv->coltiles;j++) {
	    if (isTiled) {
	      ogdi_fread(buffer,5,1,fichier);
	      buffer[5] = '\0';
	      lpriv->tilelist[count] = atoi(buffer);
	    } else {
	      lpriv->tilelist[count] = count+1;
	    }
	    count++;
	  }
	}

        fclose( fichier );
	return TRUE;      
      }
    }
    
    c = getc(fichier);
  }

  ecs_SetError(&(s->result),1,"ADRG image not found");
  fclose( fichier );
  return FALSE;
}


/* 
   ----------------------------------------------------------
   
   _read_overview

   extract the information from the .GEN file and put them
   into the overview attribute of spriv.
   
   ---------------------------------------------------------- 
   */

int _read_overview(s)
     ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;
  register LayerPrivateData *lpriv = (LayerPrivateData *) &(spriv->overview);
  FILE *fichier;
  /*int first;*/
  char c,sc[4];
  char buffer[20];
  int i,j,count;
  int isTiled;
  
  lpriv->tilelist = NULL;
  lpriv->buffertile = NULL;

  fichier = fopen(spriv->genfilename,"r");
  if (fichier == NULL) {
    ecs_SetError(&(s->result),1,"Unable to open the .GEN file");
    return FALSE;
  }

  c = getc(fichier);

  while(!feof(fichier)) {
    if (c==(char) 30) {
      ogdi_fread(sc,3,1,fichier);
      if(strncmp("OVV",sc,3) == 0) {
	/*first = TRUE;*/

	/* Jump ^^ et PRT */

	fseek(fichier,7,SEEK_CUR);

	/* Read NAM */

	ogdi_fread(buffer,8,1,fichier);
	strncpy(lpriv->imgname,buffer,8);
	lpriv->imgname[8] = '\0';

	/* Jump STR */
	
	fseek(fichier,2,SEEK_CUR);
	
	/* Read ARV */
	
	ogdi_fread(buffer,8,1,fichier);
	buffer[8] = '\0';
	lpriv->ARV = atoi(buffer);
	
	/* Read BRV */
	
	ogdi_fread(buffer,8,1,fichier);
	buffer[8] = '\0';
	lpriv->BRV = atoi(buffer);
	
	/* Read LSO PSO */
	
	ogdi_fread(buffer,11,1,fichier);
	buffer[11] = '\0';
	lpriv->LSO = parse_coord_x(buffer);
	ogdi_fread(buffer,10,1,fichier);
	buffer[10] = '\0';
	lpriv->PSO = parse_coord_y(buffer);
	
	/* Jump ^^ NUL NUS NLL NLS */
	
	fseek(fichier,25,SEEK_CUR);
	
	/* Read NFL */

	ogdi_fread(buffer,3,1,fichier);
	buffer[3] = '\0';
	lpriv->rowtiles = atoi(buffer);
	lpriv->rows = lpriv->rowtiles * 128;
	
	/* Read NFC */
	
	ogdi_fread(buffer,3,1,fichier);
	buffer[3] = '\0';
	lpriv->coltiles = atoi(buffer);
	lpriv->columns = lpriv->coltiles * 128;
	
	/* Jump PNC PNL COD ROD POR PCB PVB */
	
	fseek(fichier,17,SEEK_CUR);
	
	/* Read BAD and check if it valid. If not, search another GIN */
	
	ogdi_fread(buffer,12,1,fichier);
	strncpy(lpriv->imgfilename,buffer,12);
	lpriv->imgfilename[12] = '\0';
	
	/* Read the TIF */
	
	ogdi_fread(buffer,1,1,fichier);
	if (buffer[0] == 'N')
	  isTiled = FALSE;
	else
	  isTiled = TRUE;
	
	
	/* If is tiled, get the TSI. */
	
	if (isTiled) {
	  /* Jump ^^ BID WS1 WS2 ^^ */
	  fseek(fichier,47,SEEK_CUR);
	}
	
	lpriv->tilelist = (int *) malloc(sizeof(int)*lpriv->rowtiles*lpriv->coltiles);
	if (lpriv->tilelist == NULL) {
	  ecs_SetError(&(s->result),1,"Not enough memory");
          fclose( fichier );
	  return FALSE;
	}
	
	count = 0;
	for(i=0;i<lpriv->rowtiles;i++) {
	  for(j=0;j<lpriv->coltiles;j++) {
	    if (isTiled) {
	      ogdi_fread(buffer,5,1,fichier);
	      buffer[5] = '\0';
	      lpriv->tilelist[count] = atoi(buffer);
	    } else {
	      lpriv->tilelist[count] = count+1;
	    }
	    count++;
	  }
	}

	/* Set the bounding rectangle of the matrix with the global region
	   (no region set for the overview, only data convertion). */

        fclose( fichier );
	return TRUE;      
      }
    }
    
    c = getc(fichier);
  }

  ecs_SetError(&(s->result),1,"ADRG overview not found");
  fclose( fichier );

  return FALSE;
}

char *subfield(buffer,index,length)
     char *buffer;
     int index;
     int length;
{
  static char sub[20];
  int k;
  for (k=0;k<length;++k)
    sub[k] = buffer[index+k];
  sub[k]=0;
  return sub;
}

double parse_coord_x(buffer)
     char *buffer;
{
  double deg, min, sec;
  double degrees;

  deg = atof(subfield(buffer,1,3));
  min = atof(subfield(buffer,4,2));
  sec = atof(subfield(buffer,6,5));
  degrees = deg + min/60.0 + sec/3600.0;
  if (buffer[0] == '-')
    degrees = -degrees;
  return degrees;
}

double parse_coord_y(buffer)
     char *buffer;
{
  double deg, min, sec;
  double degrees;

  deg = atof(subfield(buffer,1,2));
  min = atof(subfield(buffer,3,2));
  sec = atof(subfield(buffer,5,5));
  degrees = deg + min/60.0 + sec/3600.0;
  if (buffer[0] == '-')
    degrees = -degrees;
  return degrees;
}


/* 
   ----------------------------------------------------------
   _VerifyLocation:
   
   check if the .GEN file is valid. 
   
   ---------------------------------------------------------- 
   */
   
int 
_verifyLocation(s)
     ecs_Server *s;
{
  int returnvalue;
  FILE *test;
  register ServerPrivateData *spriv = s->priv;

  returnvalue = FALSE;

  test = fopen(spriv->genfilename,"r");
  if (test != NULL) {
    fclose(test);
    returnvalue = TRUE;
  }

  if (!returnvalue)
    ecs_SetError(&(s->result),1,"Invalid ADRG URL. The .GEN file is invalid");
  return returnvalue;
}

/* 
   -------------------------------------------------------------------------
   _initRegionWithDefault:
   
   Prepare the global region of this driver

   Extracting the global bounding box imply a search of all
   images MBR in the .GEN file. Each image start with ^^GIN and
   the first coordinate of this image is kept 32 spaces after.
   At this location, we got two coordinate for each corner of the
   MBR.

   SWO         11 char      South-West corner longitude
   SWA         10 char      South-West corner latitude
   NWO         11 char      North-West corner longitude
   NWA         10 char      North-West corner latitude
   NEO         11 char      North-East corner longitude
   NEA         10 char      North-East corner latitude
   SEO         11 char      South-East corner longitude
   SEA         10 char      South-East corner latitude
   
   --------------------------------------------------------------------------
   */

int _initRegionWithDefault(s)
     ecs_Server *s;
{
  register ServerPrivateData *spriv = s->priv;
  int first = TRUE;
  FILE *fichier;
  char c,sc[4];
  char buffer[12];
  double x,y;
  int i;
  
  fichier = fopen(spriv->genfilename,"r");
  if (fichier == NULL) {
    ecs_SetError(&(s->result),1,"Unable to open the .GEN file");
    return FALSE;
  }

  c = getc(fichier);

  while(!feof(fichier)) {
    if (c==(char) 30) {
      ogdi_fread(sc,3,1,fichier);
      if(strncmp("GIN",sc,3) == 0) {
	fseek(fichier,32,SEEK_CUR);
	for(i=0;i<4;i++) {
	  ogdi_fread(buffer,11,1,fichier);
	  x = parse_coord_x(buffer);
	  ogdi_fread(buffer,10,1,fichier);
	  y = parse_coord_y(buffer);

	  if (first) {
	    s->globalRegion.east = x;
	    s->globalRegion.west = x;
	    s->globalRegion.north = y;
	    s->globalRegion.south = y;
	    first = FALSE;
	  } else {
	    if (x<(s->globalRegion.west))
	      s->globalRegion.west = x;
	    if (x>(s->globalRegion.east))
	      s->globalRegion.east = x;
	    if (y<(s->globalRegion.south))
	      s->globalRegion.south = y;
	    if (y>(s->globalRegion.north))
	      s->globalRegion.north = y;
	  }
	}
      }
    }

    c = getc(fichier);
  }

  s->globalRegion.ns_res = (s->globalRegion.north - s->globalRegion.south)/1000.0;
  s->globalRegion.ew_res = (s->globalRegion.east - s->globalRegion.west)/1000.0;
  
  fclose( fichier );
  return 1;
}


int _IsOutsideRegion(n,s,e,w,region)
     double n,s,e,w;
     ecs_Region *region;
{
  if ((n < region->south) || 
      (s > region->north) || 
      (e < region->west)  || 
      (w > region->east)) {	
    return 1;
  }
  return 0;
}

