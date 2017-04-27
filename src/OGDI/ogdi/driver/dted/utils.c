/******************************************************************************
 *
 * Component: OGDI DTED Driver
 * Purpose: Various DTED support functions.
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
 * Revision 1.7  2007/02/12 15:52:57  cbalint
 *
 *    Preliminary cleanup.
 *    Get rif of unitialized variables, and unused ones.
 *
 * Revision 1.6  2001/04/10 14:29:43  warmerda
 * Upgraded with changes from DND (hand applied to avoid losing bug fixes).
 * Patch also includes change to exclude zero elevations when computing
 * mincat/maxcat.
 * New style headers also applied.
 *
 */

#include "dted.h"

ECS_CVSID("$Id: utils.c,v 1.8 2016/06/28 14:32:45 erouault Exp $");

/* 
   ----------------------------------------------------------
   
   _parse_request
   
   extract esdir and nsfile from request, store in private data structure 
   
   ---------------------------------------------------------- 
   */

#define SYNTAXERRORMESSAGE "Badly formed request: %s, must be LayerName(loadtype)"

int _parse_request(s,sel,isInRam)
     ecs_Server *s;
     char *sel;
     int *isInRam;
{
  static int compiled = 0;
  static ecs_regexp *reg;
  char buffer[512];
  static char *atom1 = NULL;

  if (atom1 != NULL) {
    free(atom1);
    atom1 = NULL;
  }

  if (!compiled) {
    reg = EcsRegComp(".*\\((.*)\\)$");
    compiled = 1;
  }

  if (!EcsRegExec(reg,sel,NULL)) {
    sprintf(buffer,SYNTAXERRORMESSAGE,sel);
    ecs_SetError(&(s->result),1,buffer);
    return 0;
  }

  if (!ecs_GetRegex(reg,1,&atom1)) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
    return 0;
  }

  if (strlen(atom1) == 0) {
    sprintf(buffer,SYNTAXERRORMESSAGE,atom1);
    ecs_SetError(&(s->result),1,buffer);
    return 0;
  }

  *isInRam = TRUE;
      
  return 1;
}

/* 
   ----------------------------------------------------------
   
   _read_dted

   extract the information from a dted file and put them
   into lpriv.
   
   ---------------------------------------------------------- 
   */

int _read_dted(ecs_Server *s, int xtile, int ytile) {
  char buffer[UHL_LENGTH], temp[3];
  char *chtmp;
  double origin_lon, origin_lat;
  double interval_lon, interval_lat;
  double lat_spacing, lon_spacing;
  LayerPrivateData *lpriv=s->layer[s->currentLayer].priv;
  ServerPrivateData *spriv =s->priv;
  int columns, rows;
  int size;
  
  fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, 0, SEEK_SET);

  spriv->firstcoordfilepos = 0;

  if (fread(buffer,1,UHL_LENGTH,spriv->ewdir[xtile].nsfile[ytile].filehandle) < UHL_LENGTH)
    return FALSE;
  spriv->firstcoordfilepos += UHL_LENGTH;

  if (buffer[0] == 'H') {
    if (fread (buffer,1,UHL_LENGTH,spriv->ewdir[xtile].nsfile[ytile].filehandle) < UHL_LENGTH)
      return FALSE;
    spriv->firstcoordfilepos += UHL_LENGTH;
  }

  origin_lon = parse_coord(buffer+4);
  origin_lat = parse_coord(buffer+12);
  interval_lon = atoi(subfield(buffer,20,4))/10.0;
  interval_lat = atoi(subfield(buffer,24,4))/10.0;
  
  columns = atoi(subfield(buffer,47,4));
  rows = atoi(subfield(buffer,51,4));

  lat_spacing = interval_lat / 3600.0;
  lon_spacing = interval_lon / 3600.0;
  spriv->ewdir[xtile].nsfile[ytile].region.north = origin_lat+lat_spacing/2+lat_spacing*rows;
  spriv->ewdir[xtile].nsfile[ytile].region.south = origin_lat-lat_spacing/2;
  spriv->ewdir[xtile].nsfile[ytile].region.west = origin_lon-lon_spacing/2;
  spriv->ewdir[xtile].nsfile[ytile].region.east = origin_lon+lon_spacing/2+lon_spacing*columns;
  spriv->ewdir[xtile].nsfile[ytile].region.ns_res = (spriv->ewdir[xtile].nsfile[ytile].region.north - spriv->ewdir[xtile].nsfile[ytile].region.south) / rows;
  spriv->ewdir[xtile].nsfile[ytile].region.ew_res = (spriv->ewdir[xtile].nsfile[ytile].region.east - spriv->ewdir[xtile].nsfile[ytile].region.west) / columns;

  spriv->ewdir[xtile].nsfile[ytile].columns=columns;
  spriv->ewdir[xtile].nsfile[ytile].rows=rows;

  /* find the DTED type */

  fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, spriv->firstcoordfilepos, SEEK_SET);
  if (fread(buffer,1,UHL_LENGTH,spriv->ewdir[xtile].nsfile[ytile].filehandle) < UHL_LENGTH)
    return FALSE;
  
  strncpy(temp,&buffer[63],1);
  temp[1]='\0';
  spriv->level=(int) strtol(temp,&chtmp, 10);

  spriv->firstcoordfilepos += DSI_LENGTH + ACC_LENGTH ;
  
  /* Read the data info if isInRam is true */

  if (lpriv->isInRam) {
    fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, spriv->firstcoordfilepos, SEEK_SET);
    if (lpriv->matrixbuffer != NULL) {
      free(lpriv->matrixbuffer);
      lpriv->matrixbuffer = NULL;
    }
    size = columns*(rows*2+8+4);
    lpriv->matrixbuffer = (unsigned char *) malloc(size);
    if (lpriv->matrixbuffer == NULL) {
      ecs_SetError(&(s->result),1,"not enough memory to load dted matrix in ram");      
      return FALSE; 
    }
    if (fread(lpriv->matrixbuffer,1,size,spriv->ewdir[xtile].nsfile[ytile].filehandle) < (unsigned int) size) {
      ecs_SetError(&(s->result),1,"read too much info in file");
      return FALSE; 
    }
  }
  return TRUE;
}

/* set spriv->maxcat and spriv->mincat */

int _sample_tiles(ecs_Server *s, ecs_TileStructure *t) {
  double ns_min, ns_max, ew_min, ew_max;
  double ns_inc, ew_inc;
  int x, y;
  int i,j, cat;
  int firsttime=1;
  int tileWidth, tileHeight, xpixel, ypixel;
  ServerPrivateData *spriv=s->priv;
  int range;
 
  int nb_xsamp=5;
  int nb_ysamp=5;
  
  ns_inc=(s->globalRegion.north-s->globalRegion.south)/spriv->ytiles;
  ew_inc=(s->globalRegion.east-s->globalRegion.west)/spriv->xtiles;

  s->globalRegion.ns_res = 1.0;
  s->globalRegion.ew_res = 1.0;

  for (x=0; x<spriv->xtiles; x++) {
    for (y=0; y<spriv->ytiles; y++) {
      if (spriv->ewdir[x].nsfile[y].used) {
	/* found one in use: sample it */
	/*  find the coordinates at selected intervals */
	ns_min=s->globalRegion.south+ns_inc*y;
	ns_max=s->globalRegion.south+ns_inc*(y+1);
	ew_min=s->globalRegion.west+ew_inc*x;
	ew_max=s->globalRegion.west+ew_inc*(x+1);
	_getTileDim(s,t,ew_min+1,ns_min+1, &tileWidth, &tileHeight);
	
	if (s->globalRegion.ns_res > ((ns_max - ns_min) / (double) tileHeight)) {
	  s->globalRegion.ns_res = ((ns_max - ns_min) / (double) tileHeight);
	}
	if (s->globalRegion.ew_res > ((ew_max - ew_min) / (double) tileWidth)) {
	  s->globalRegion.ew_res = ((ew_max - ew_min) / (double) tileWidth);
	}

	t->height=tileHeight;
	for (i=0; i< nb_xsamp; i++) {
	  for (j=0; j< nb_ysamp; j++) {

	    /* figure out the pixel position in the tile */
	    xpixel=(int) i*tileWidth/nb_xsamp;
	    ypixel=(int) j*tileHeight/nb_ysamp;;

	    /* call the callback */
	    _sample_getRawValue(s,t,x,y,xpixel,ypixel,&cat);
	    if ( cat != 0 ) { 
	      if (firsttime) {
		spriv->mincat=cat;
		spriv->maxcat=cat;
		firsttime=0;
	      } else {
		if (cat < spriv->mincat)
		  spriv->mincat=cat;	     
		if (cat >spriv->maxcat) 
		  spriv->maxcat=cat;
	      }
	    }
	  }
	}
      }
    }
  }

  /* adjust the sampled result:
     1) adjust maxcat outwards by 10%.
     2) if mincat < 0, adjust it down by 10 %.
  */

  range=spriv->maxcat-spriv->mincat;
  if (spriv->mincat>50 || spriv->mincat < 0) {
    spriv->mincat-=(int)(range* 0.10);
  }
  spriv->maxcat+=(int)(range*0.20);

  if (spriv->lastTile.none) {
    fclose(spriv->ewdir[spriv->lastTile.x].nsfile[spriv->lastTile.y].filehandle);
    spriv->lastTile.none=0;
    spriv->lastTile.x=-1;
    spriv->lastTile.y=-1;
  }


  return TRUE;
}

/* 
   ----------------------------------------------------------
   
   _sample_read_dted

   extract the information from a dted file.  Doesn't require 
   lpriv.
   
   ---------------------------------------------------------- 
   */
int _sample_read_dted(ecs_Server *s, int xtile, int ytile, int32 *firstcoordfilepos, FILE *fileptr) {
  char buffer[UHL_LENGTH], temp[3];
  char *chtmp;
  double origin_lon, origin_lat;
  double interval_lon, interval_lat;
  double lat_spacing, lon_spacing;
  ServerPrivateData *spriv =s->priv;
  int columns, rows;
  
  fseek(fileptr, 0, SEEK_SET);

  *firstcoordfilepos = 0;

  if (fread(buffer,1,UHL_LENGTH,fileptr) < UHL_LENGTH)
    return FALSE;
  *firstcoordfilepos += UHL_LENGTH;

  if (buffer[0] == 'H') {
    if (fread(buffer,1,UHL_LENGTH,fileptr) < UHL_LENGTH)
      return FALSE;
    *firstcoordfilepos += UHL_LENGTH;
  }

  origin_lon = parse_coord(buffer+4);
  origin_lat = parse_coord(buffer+12);
  interval_lon = atoi(subfield(buffer,20,4))/10.0;
  interval_lat = atoi(subfield(buffer,24,4))/10.0;
  
  columns = atoi(subfield(buffer,47,4));
  rows = atoi(subfield(buffer,51,4));

  lat_spacing = interval_lat / 3600.0;
  lon_spacing = interval_lon / 3600.0;
  spriv->ewdir[xtile].nsfile[ytile].region.north = origin_lat+lat_spacing/2+lat_spacing*rows;
  spriv->ewdir[xtile].nsfile[ytile].region.south = origin_lat-lat_spacing/2;
  spriv->ewdir[xtile].nsfile[ytile].region.west = origin_lon-lon_spacing/2;
  spriv->ewdir[xtile].nsfile[ytile].region.east = origin_lon+lon_spacing/2+lon_spacing*columns;
  spriv->ewdir[xtile].nsfile[ytile].region.ns_res = (spriv->ewdir[xtile].nsfile[ytile].region.north
	   -spriv->ewdir[xtile].nsfile[ytile].region.south) / rows;
  spriv->ewdir[xtile].nsfile[ytile].region.ew_res = (spriv->ewdir[xtile].nsfile[ytile].region.east 
	   -spriv->ewdir[xtile].nsfile[ytile].region.west) / columns;
  spriv->ewdir[xtile].nsfile[ytile].columns=columns;
  spriv->ewdir[xtile].nsfile[ytile].rows=rows;

  /* find the DTED type */

  fseek(fileptr, *firstcoordfilepos, SEEK_SET);
  if (fread(buffer,1,UHL_LENGTH,fileptr) < UHL_LENGTH)
    return FALSE;
  
  strncpy(temp,&buffer[63],1);
  temp[1]='\0';
  spriv->level=(int) strtol(temp,&chtmp, 10);
  
  *firstcoordfilepos += DSI_LENGTH + ACC_LENGTH ;

  return TRUE;
}


/* retrieve nothing but the level from a tile */
int _get_level(ecs_Server *s, int xtile, int ytile, int *level) {
  char buffer[UHL_LENGTH], temp[3], dtedfilename[256];
  char *chtmp;
  ServerPrivateData *spriv =s->priv;
  
  strcpy(dtedfilename,spriv->pathname);
  strcat(dtedfilename,"/");
  strcat(dtedfilename,spriv->ewdir[xtile].name);
  strcat(dtedfilename,"/");
  strcat(dtedfilename,spriv->ewdir[xtile].nsfile[ytile].name);
  
#ifdef _WINDOWS
  spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "rb");
#else
  spriv->ewdir[xtile].nsfile[ytile].filehandle = fopen(dtedfilename, "r");
#endif
    
  if (!spriv->ewdir[xtile].nsfile[ytile].filehandle) {
    return FALSE;
  }
  
  fseek(spriv->ewdir[xtile].nsfile[ytile].filehandle, UHL_LENGTH, SEEK_SET);
  if (fread(buffer,1,UHL_LENGTH,spriv->ewdir[xtile].nsfile[ytile].filehandle) < UHL_LENGTH)
    return FALSE;
  
  if (buffer[0] == 'H') {
    if (fread(buffer,1,UHL_LENGTH,spriv->ewdir[xtile].nsfile[ytile].filehandle) < UHL_LENGTH)
      return FALSE;
  }

  fclose(spriv->ewdir[xtile].nsfile[ytile].filehandle);
  spriv->ewdir[xtile].nsfile[ytile].filehandle=NULL;
  strncpy(temp,&buffer[63],1);
  temp[1]='\0';
  *level=(int) strtol(temp,&chtmp, 10);
#if 0
  printf("level is %d\n", *level);
#endif
  return TRUE;
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

double parse_coord(buffer)
     char *buffer;
{
  int deg, min, sec;
  double degrees;
  char hem;

  deg = atoi(subfield(buffer,0,3));
  min = atoi(subfield(buffer,3,2));
  sec = atoi(subfield(buffer,5,2));
  degrees = deg + min/60.0 + sec/3600.0;
  hem = buffer[7];
  if (hem == 's' || hem == 'S' || hem == 'w' || hem == 'W')
    degrees = -degrees;
  return degrees;
}

/* 
   ----------------------------------------------------------
   _VerifyLocation:
   
   check if the path is valid. Check if the directory exist
   and check if the parent path contain a dmed file.
   
   ---------------------------------------------------------- 
   */
   
int 
_verifyLocation(s)
     ecs_Server *s;
{
  int returnvalue;
  DIR *dirlist;
  FILE *test;
  ServerPrivateData *spriv = s->priv;
  char *ptr,*ptr1,*ptr2;

  returnvalue = FALSE;
  /* Check if the path is valid */
  dirlist = opendir(spriv->pathname);
  if (dirlist != NULL) {
    closedir(dirlist);
    /* Check the parent directory */
    ptr = ptr1 = spriv->pathname;
    while(ptr[0]!='\0') {
      if ((ptr[0]=='/') && (ptr[1]!='\0')) {
	ptr1 = ptr;
      }
      ptr++;
    }
    ptr1++;
    ptr2 = malloc(ptr1-(spriv->pathname)+6);
    if (ptr2 != NULL) {
      strncpy(ptr2,spriv->pathname,ptr1-(spriv->pathname));
      ptr2[ptr1-(spriv->pathname)] = '\0';
      strcat(ptr2,"dmed");
      test = fopen(ptr2,"r");
      if (test != NULL) {
	fclose(test);
	returnvalue = TRUE;
      } else {
	strncpy(ptr2,spriv->pathname,ptr1-(spriv->pathname));
	strcat(ptr2,"DMED");
	test = fopen(ptr2,"r");
	if (test != NULL) {
	  fclose(test);
	  returnvalue = TRUE;
	}
      }
      free( ptr2 );
    }
  }
  if (!returnvalue)
    ecs_SetError(&(s->result),1,"Invalid URL. The dted directory is invalid");
  return returnvalue;
}

/* 
   -------------------------------------------------------------------------
   _initRegionWithDefault:
   
   preparation de la fenetre globale pour le server                    
   --------------------------------------------------------------------------
   */

int _initRegionWithDefault(s)
     ecs_Server *s;
{
  char buffer[256];
  struct dirent *structure1;
  DIR *dirlist1;
  struct dirent *structure2;
  DIR *dirlist2=NULL;
  static int compiled = 0;
  static ecs_regexp *reglet;
  static ecs_regexp *regnum;
  int x,y;
  char *letter;
  char *number;
  int first = TRUE;
  ServerPrivateData *spriv = s->priv;
  double increment_x = 1.0;
  double increment_y = 1.0;

  if (!compiled) {
    reglet = EcsRegComp("([A-Za-z])");
    regnum = EcsRegComp("([0-9]+)");
    compiled = 1;
  }

  dirlist1 = opendir(spriv->pathname);
  structure1 = (struct dirent *) readdir(dirlist1);
  ecs_SetText(&(s->result)," "); /* make sure an empty list is returned in all case */

  while (structure1 != NULL) {
    if (!((strcmp(structure1->d_name,".") == 0) || 
	  (strcmp(structure1->d_name,"..") == 0)||
          (strcmp(structure1->d_name,"CVS") == 0))) {

      /* Extraction des donnees du repertoire structure1->d_name */
     
      if (!EcsRegExec(regnum,structure1->d_name,NULL)) {
	sprintf(buffer,"Badly formed dted directory name: %s. The number is incorrect",structure1->d_name);
	ecs_SetError(&(s->result),1,buffer);
	closedir(dirlist1);
	closedir(dirlist2);
	return 0;
      }
      if (!EcsRegExec(reglet,structure1->d_name,NULL)) {
	sprintf(buffer,"Badly formed dted directory name: %s. No letters",structure1->d_name);
	ecs_SetError(&(s->result),1,buffer);
	closedir(dirlist1);
	closedir(dirlist2);
	return 0;
      }

      if (!ecs_GetRegex(regnum,0,&number)) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	closedir(dirlist1);
	closedir(dirlist2);
	return 0;
      }
      if (!ecs_GetRegex(reglet,0,&letter)) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	free(number);
	closedir(dirlist1);
	closedir(dirlist2);
	return 0;
      }
      x = atoi(number);
      if ((letter[0] == 'w') || (letter[0] == 'W'))
	x = -x;
      if (first) {
	s->globalRegion.west = (double) x;
	s->globalRegion.east = (double) x;
      } else {
	if ((double) x > s->globalRegion.east) {
	  if ((x - s->globalRegion.east) > increment_x)
	    increment_x = x - s->globalRegion.east;
	  s->globalRegion.east = (double) x;
	}
	if ((double) x < s->globalRegion.west) {
	  if ((s->globalRegion.west - x) > increment_x)
	    increment_x = s->globalRegion.west - x;
	  s->globalRegion.west = (double) x;
	}
      }

      free(number);
      free(letter);

      sprintf(buffer,"%s/%s",spriv->pathname,structure1->d_name);

      dirlist2 = opendir(buffer);
      structure2 = (struct dirent *) readdir(dirlist2);
      
      while (structure2 != NULL) {
	if (!((strcmp(structure2->d_name,".") == 0) || 
              (strcmp(structure2->d_name,"..") == 0)||
              (strcmp(structure2->d_name,"CVS") == 0))) {
	  if (!EcsRegExec(regnum,structure2->d_name,NULL)) {
	    sprintf(buffer,"Badly formed dted file name: %s. The number is incorrect",structure2->d_name);
	    ecs_SetError(&(s->result),1,buffer);
	    closedir(dirlist1);
	    closedir(dirlist2);
	    return 0;
	  }
	  if (!EcsRegExec(reglet,structure2->d_name,NULL)) {
	    sprintf(buffer,"Badly formed dted file name: %s. No letters",structure2->d_name);
	    ecs_SetError(&(s->result),1,buffer);
	    closedir(dirlist1);
	    closedir(dirlist2);
	    return 0;
	  }
	  
	  if (!ecs_GetRegex(regnum,0,&number)) {
	    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	    closedir(dirlist1);
	    closedir(dirlist2);
	    return 0;
	  }
	  if (!ecs_GetRegex(reglet,0,&letter)) {
	    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	    free(number);
	    closedir(dirlist1);
	    closedir(dirlist2);	    
	    return 0;
	  }
	  y = atoi(number);
	  if ((letter[0] == 's') || (letter[0] == 'S'))
	    y = -y;
	  free(number);
	  free(letter);
	  if (first) {
	    s->globalRegion.north = (double) y;
	    s->globalRegion.south = (double) y;
	    first = FALSE;
	  } else {
	    if ((double) y > s->globalRegion.north) {
	      if ((y - s->globalRegion.north) > increment_y)
		increment_y = y - s->globalRegion.north;
	      s->globalRegion.north = (double) y;
	    }
	    if ((double) y < s->globalRegion.south) {
	      if ((s->globalRegion.south - y) > increment_y)
		increment_y = s->globalRegion.south - y;
	      s->globalRegion.south = (double) y;
	    }
	  }
	}
	structure2 = (struct dirent *) readdir(dirlist2);
      }

      closedir(dirlist2);
    }

    structure1 = (struct dirent *) readdir(dirlist1);
  }

  closedir(dirlist1);
  
  s->globalRegion.east += increment_x;
  s->globalRegion.north += increment_y;

  s->globalRegion.ns_res = (s->globalRegion.north - s->globalRegion.south)/2000.0;
  s->globalRegion.ew_res = (s->globalRegion.east - s->globalRegion.west)/2000.0;
  
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

/* postconditions: spriv->mincat, spriv->maxcat, spriv->level set */

int _readDMED (ecs_Server *s) {
  ServerPrivateData *spriv = s->priv;
  int x,y;
  int found=0;
  /*   spriv->mincat=1; */
  /* spriv->maxcat=2000; */

  for (x=0; x<spriv->xtiles; x++) {
    for (y=0; y<spriv->ytiles; y++)
      if (spriv->ewdir[x].nsfile[y].used) {
	if (! _get_level(s, x, y, &(spriv->level))) {
	  return FALSE;
	} else {
	  found=1;
	  break;
	}
      }
    if (found) {
      break;
    }
  }
  if (!found) {
    return FALSE;
  }

  return TRUE;
}

int _readValuesFromDirList(ecs_Server *s) {
  char buffer[256];
  struct dirent *structure1;
  DIR *dirlist1;
  struct dirent *structure2;
  DIR *dirlist2 = NULL;
  static int compiled = 0;
  static ecs_regexp *reglet;
  static ecs_regexp *regnum;
  int x,y, i,j, tile_x, tile_y;
  char *letter;
  char *number;
  int first = TRUE;
  ServerPrivateData *spriv = s->priv;
  double increment_x = 1.0;
  double increment_y = 1.0;
  
  EWDir ** dirlist = NULL;
  int nb_ewdir=0;

  if (!compiled) {
    reglet = EcsRegComp("([A-Za-z])");
    regnum = EcsRegComp("([0-9]+)");
    compiled = 1;
  }

  dirlist1 = opendir(spriv->pathname);
  structure1 = (struct dirent *) readdir(dirlist1);
  ecs_SetText(&(s->result)," "); /* make sure an empty list is returned in all case */

  while (structure1 != NULL) {
    if (!((strcmp(structure1->d_name,".") == 0) || 
	  (strcmp(structure1->d_name,"..") == 0)||
	  (strcmp(structure1->d_name,"CVS") == 0))) {

      /* Extraction des donnees du repertoire structure1->d_name */
     
      if (!EcsRegExec(regnum,structure1->d_name,NULL)) {
	sprintf(buffer,"Badly formed dted directory name: %s. The number is incorrect",structure1->d_name);
	ecs_SetError(&(s->result),1,buffer);
	closedir(dirlist1);
        if( dirlist2 != NULL )
            closedir(dirlist2);
	return 0;
      }
      if (!EcsRegExec(reglet,structure1->d_name,NULL)) {
	sprintf(buffer,"Badly formed dted directory name: %s. No letters",structure1->d_name);
	ecs_SetError(&(s->result),1,buffer);
	closedir(dirlist1);
        if( dirlist2 != NULL )
            closedir(dirlist2);
	return 0;
      }

      if (!ecs_GetRegex(regnum,0,&number)) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	closedir(dirlist1);
        if( dirlist2 != NULL )
            closedir(dirlist2);
	return 0;
      }
      if (!ecs_GetRegex(reglet,0,&letter)) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	free(number);
	closedir(dirlist1);
        if( dirlist2 != NULL )
            closedir(dirlist2);
	return 0;
      }

      x = atoi(number);
      if (x > 1000) {
	x = x / 10000;
      }
      if ((letter[0] == 'w') || (letter[0] == 'W'))
	x = -x;
      if (first) {
	s->globalRegion.west = (double) x;
	s->globalRegion.east = (double) x;
      } else {
	if ((double) x > s->globalRegion.east) {
	  if ((x - s->globalRegion.east) > increment_x)
	    increment_x = x - s->globalRegion.east;
	  s->globalRegion.east = (double) x;
	}
	if ((double) x < s->globalRegion.west) {
	  if ((s->globalRegion.west - x) > increment_x)
	    increment_x = s->globalRegion.west - x;
	  s->globalRegion.west = (double) x;
	}
      }

      free(number);
      free(letter);


      /* record the ewdirs */
#if 0
      printf("ewdir= %s\n", structure1->d_name);
#endif
      nb_ewdir++;
      dirlist = (EWDir **) realloc (dirlist, sizeof(EWDir *)* nb_ewdir);
      if (!dirlist) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	return FALSE;
      }
      dirlist[nb_ewdir-1]= (EWDir *) malloc (sizeof(EWDir));

      if (! dirlist[nb_ewdir-1]) {
	ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	return FALSE;	
      }
      strcpy(dirlist[nb_ewdir-1]->name,structure1->d_name);
      dirlist[nb_ewdir-1]->nb_files=0;
      dirlist[nb_ewdir-1]->nsfile=NULL;

      sprintf(buffer,"%s/%s",spriv->pathname,structure1->d_name);

      /* parse the subdirectory */
      dirlist2 = opendir(buffer);
      structure2 = (struct dirent *) readdir(dirlist2);
      
      while (structure2 != NULL) {

	if (!((strcmp(structure2->d_name,".") == 0) || 
	      (strcmp(structure2->d_name,"..") == 0)||
	      (strcmp(structure2->d_name,"CVS") == 0))) 
	  if ((strlen(structure2->d_name) > 4) && (strncasecmp("dt", &(structure2->d_name[strlen(structure2->d_name)-3]),2)) == 0) {
	    if (!EcsRegExec(regnum,structure2->d_name,NULL)) {
	      sprintf(buffer,"Badly formed dted file name: %s. The number is incorrect",structure2->d_name);
	      ecs_SetError(&(s->result),1,buffer);
	      closedir(dirlist1);
              if( dirlist2 != NULL )
                  closedir(dirlist2);
	      return 0;
	    }
	    if (!EcsRegExec(reglet,structure2->d_name,NULL)) {
	      sprintf(buffer,"Badly formed dted file name: %s. No letters",structure2->d_name);
	      ecs_SetError(&(s->result),1,buffer);
	      closedir(dirlist1);
              if( dirlist2 != NULL )
                  closedir(dirlist2);
	      return 0;
	    }
#if 0	  
	    printf("nsdir= %s\n", structure2->d_name);
#endif
	    dirlist[nb_ewdir-1]->nsfile=(NSFile *) realloc (dirlist[nb_ewdir-1]->nsfile, (sizeof(NSFile) * (++dirlist[nb_ewdir-1]->nb_files)));
	    if (!dirlist[nb_ewdir-1]->nsfile) {
	      ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	      return FALSE;
	    }
	    strcpy(dirlist[nb_ewdir-1]->nsfile[dirlist[nb_ewdir-1]->nb_files-1].name,structure2->d_name);
	    
	    if (!ecs_GetRegex(regnum,0,&number)) {
	      ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	      closedir(dirlist1);
              if( dirlist2 != NULL )
                  closedir(dirlist2);
	      return 0;
	    }
	    if (!ecs_GetRegex(reglet,0,&letter)) {
	      ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
	      free(number);
	      closedir(dirlist1);
              if( dirlist2 != NULL )
                  closedir(dirlist2);	    
	      return 0;
	    }
	    y = atoi(number);
	    if (y > 1000) {
	      y = y / 10000;
	    }
	    if ((letter[0] == 's') || (letter[0] == 'S'))
	      y = -y;
	    
	    dirlist[nb_ewdir-1]->nsfile[dirlist[nb_ewdir-1]->nb_files-1].coord=y;
	    dirlist[nb_ewdir-1]->coord=x;
	    
	    
	    free(number);
	    free(letter);
	    if (first) {
	      s->globalRegion.north = (double) y;
	      s->globalRegion.south = (double) y;
	      first = FALSE;
	    } else {
	      if ((double) y > s->globalRegion.north) {
		if ((y - s->globalRegion.north) > increment_y)
		  increment_y = y - s->globalRegion.north;
		s->globalRegion.north = (double) y;
	      }
	      if ((double) y < s->globalRegion.south) {
		if ((s->globalRegion.south - y) > increment_y)
		  increment_y = s->globalRegion.south - y;
		s->globalRegion.south = (double) y;
	      }
	    }
	  }
	structure2 = (struct dirent *) readdir(dirlist2);
      }
      
      closedir(dirlist2);
    }
    
    structure1 = (struct dirent *) readdir(dirlist1);
  }
  
  closedir(dirlist1);
  
  s->globalRegion.east += increment_x;
  s->globalRegion.north += increment_y;

  /* these values are not used for anything, since the res is not constant */
  s->globalRegion.ns_res = (s->globalRegion.north - s->globalRegion.south)/2000.0;
  s->globalRegion.ew_res = (s->globalRegion.east - s->globalRegion.west)/2000.0;

  /* allocate the directory structure in spriv*/

  spriv->xtiles=(int) (s->globalRegion.east- s->globalRegion.west);
  spriv->ytiles=(int) (s->globalRegion.north- s->globalRegion.south);
#if 0  
  printf("xtiles=%d, ytiles=%d", spriv->xtiles, spriv->ytiles);
#endif  
  spriv->ewdir=(EWDir *) (malloc (sizeof(EWDir) * spriv->xtiles));
  if (spriv->ewdir == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
    return FALSE;
  }

  for (i=0; i<(int) spriv->xtiles; i++) {
    spriv->ewdir[i].nsfile=(NSFile *) malloc (sizeof(NSFile) * spriv->ytiles);
    if (spriv->ewdir[i].nsfile == NULL) {
      ecs_SetError(&(s->result),1,"Not enough memory to allocate server");  
      return FALSE;
    }
    for (j=0; j< (int) spriv->ytiles; j++) {
      spriv->ewdir[i].nsfile[j].used=0;
      spriv->ewdir[i].nsfile[j].filehandle=NULL;
    }
  }


  
  /* sort the list of files we got into the 2d array of files */
  for (x=0; x < nb_ewdir; x++) {
    tile_x=(int) (dirlist[x]->coord - s->globalRegion.west);
    for (y=0; y < dirlist[x]->nb_files; y++) {
      /* there is a -1 because it is compared with the SW corner */

      tile_y=(int) (s->globalRegion.north - dirlist[x]->nsfile[y].coord -1 ); 
      strcpy(spriv->ewdir[tile_x].nsfile[tile_y].name,dirlist[x]->nsfile[y].name);
      spriv->ewdir[tile_x].nsfile[tile_y].used=1;

    }
    strcpy(spriv->ewdir[tile_x].name,dirlist[x]->name);
  }

  /* free the memory */

  for (y=0; y < nb_ewdir; y++) {
    if ( dirlist[y]->nsfile)
      free(dirlist[y]->nsfile);
    free(dirlist[y]);
  }

  if (dirlist) 
    free(dirlist);
  
  /* set the layer name based on the region */

  sprintf(buffer, "%s","DTED");
  strcpy(spriv->layername, buffer);

  return 1;
}


