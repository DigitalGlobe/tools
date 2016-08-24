/******************************************************************************
 *
 * Component: OGDI RPF Driver
 * Purpose: Implementation of various RPF support functions.
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
 * Revision 1.10  2016/06/28 14:32:45  erouault
 * Fix all warnings about unused variables raised by GCC 4.8
 *
 * Revision 1.9  2007/02/12 16:09:06  cbalint
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
 * Revision 1.8  2004/03/29 05:18:05  warmerda
 * Added check for FSDEVG field when reading TOC file (in parse_toc).
 * Apparently needed for CIB1 data, as per bug 917678.
 *
 * Revision 1.7  2001/06/26 18:14:55  warmerda
 * implement rpf_fopen_ci() to case insenstive file access
 *
 * Revision 1.6  2001/04/12 19:22:46  warmerda
 * applied DND support Image type support
 *
 */

#include "rpf.h"

ECS_CVSID("$Id: utils.c,v 1.10 2016/06/28 14:32:45 erouault Exp $");

void dyn_string_tolower(char *);

#ifdef _WINDOWS
#define DIR_CHAR '\\'
#else
#define DIR_CHAR '/'
#endif

/*
** rpf_fopen_ci()
**
** Cover for fopen() that can open a file with either upper
** case or lower case.  Allows grabbing of a.toc or A.TOC on
** Unix regardless of how the data came off CD. 
*/

static FILE *rpf_fopen_ci( const char *dir, const char *file, 
                           const char *access )
{
    char	*filename = malloc(strlen(dir)+strlen(file)+3);
    FILE 	*fp;
    int		i;

    if( filename == NULL )
        return NULL;

    if( dir[strlen(dir)-1] == '/' || dir[strlen(dir)-1] == '\\' )
        sprintf( filename, "%s%s", dir, file );
    else
        sprintf( filename, "%s%c%s", dir, DIR_CHAR, file );

    /* try natural */
    fp = fopen( filename, access );

    /* try upper case */
    if( fp == NULL )
    {
        for( i = strlen(dir); filename[i] != '\0'; i++ )
        {
            if( filename[i] >= 'a' && filename[i] <= 'z' )
                filename[i] = filename[i] - 'a' + 'A';
        }
        fp = fopen( filename, access );
    }
        
    /* try lower case */
    if( fp == NULL )
    {
        for( i = strlen(dir); filename[i] != '\0'; i++ )
        {
            if( filename[i] >= 'A' && filename[i] <= 'Z' )
                filename[i] = filename[i] - 'A' + 'a';
        }
        fp = fopen( filename, access );
    }
     
/*    printf( "fp=%p, filename=%s\n", fp, filename ); */
   
    free( filename );

    return fp;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _prepare_rpflayer

DESCRIPTION

    Initialize the current layer private structure with the layer resquest
    information.

END_DESCRIPTION

PRECONDITIONS

    l->priv must be set in memory and with default values.

END_PRECONDITIONS

POSTCONDITIONS

    The layer structure is initialized.

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer

END_PARAMETERS

RETURN_VALUE

    int: Indicate if it's a success or a error situation.

END_FUNCTION_INFORMATION

PSEUDOCODE

    Parse the selection request and try to link the information
    in it with the table of content. If the link is not found,
    return an error message. If it's found, set the entry pointer
    to this request.

    Initialize tile structure with ecs_TileInitialize. The different
    arguments are already in the table of content. If a problem occur,
    return an error message.

    Return a success message

END_PSEUDOCODE

*******************************************************************
*/

int dyn_prepare_rpflayer(s,l)
    ecs_Server *s;
    ecs_Layer *l;
{
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  char *scale;
  char *zone;
  char *type;
  char *producer;
  char *boundid;
  char *buffer;
  int i,count,stringlen,val;
  ecs_Region region;

  /*
    Parse the selection request and try to link the information
    in it with the table of content. Simply scan the selection
    string and put a pointer to the beginning of each substring.
    */
  scale = NULL;
  zone = NULL;
  type = NULL;
  producer = NULL;
  boundid = NULL;
  
  buffer = malloc(strlen(l->sel.Select)+1);
  if (buffer == NULL) {
    ecs_SetError(&(s->result),1,"Not enough memory");
    return FALSE;
  }
  strcpy(buffer,l->sel.Select);
  count = 0 ;
  scale = buffer;
  stringlen = strlen(buffer);
  for (i=0;i<stringlen;i++) {
    if (buffer[i] == '@') {
      buffer[i] = '\0';
      count++;

      switch(count) {
      case 1: 
	zone = &buffer[i+1];
	break;
      case 2:
	type = &buffer[i+1];
	break;
      case 3:
	producer = &buffer[i+1];
	break;
      case 4:
	boundid = &buffer[i+1];
	buffer[stringlen] = '\0';
	break;
      default:
	ecs_SetError(&(s->result),1,"Bad request format. Expect scale@zone@rpf_type@producer@boundary_id");
	free(buffer);
	return FALSE;
      }
    }
  }

  if (boundid == NULL) {
    ecs_SetError(&(s->result),1,"Bad request format. Expect scale@zone@rpf_type@producer@boundary_id");
    free(buffer);
    return FALSE;
  }

  val = atoi(boundid);

  /* Found an entry in the toc with these four informations. If it's not there, return an error. */

  for(i=0; i<(int) spriv->toc->num_boundaries; i++) {
#if 0
    printf("scale:%s\n",spriv->toc->entries[i].scale);
    printf("zone:%s\n",spriv->toc->entries[i].zone);
    printf("type:%s\n",spriv->toc->entries[i].type);
    printf("producer:%s\n",spriv->toc->entries[i].producer);
    printf("boundary:%d\n",spriv->toc->entries[i].boundary_id);
    printf("---------------------\n");
#endif
    if (strstr(spriv->toc->entries[i].scale,scale) != NULL) {
      if (strstr(spriv->toc->entries[i].zone,zone) != NULL) {
	if (strstr(spriv->toc->entries[i].type,type) != NULL) {
	  if (strstr(spriv->toc->entries[i].producer,producer) != NULL) {
	    if (val == spriv->toc->entries[i].boundary_id) {
	      lpriv->entry = &(spriv->toc->entries[i]);
	      break;
	    }
	  }
	}
      }
    }
  }
  if (lpriv->entry == NULL) {
    ecs_SetError(&(s->result),1,"This request don't exist in the table of content of RPF");
    free(buffer);
    return FALSE;
  }

  free(buffer);

  /*
    Initialize tile structure with ecs_TileInitialize. The differents
    arguments are already in the table of content.
    */

  region.north = lpriv->entry->nw_lat;
  region.south = lpriv->entry->sw_lat;
  region.east = lpriv->entry->ne_long;
  region.west = lpriv->entry->nw_long;
  region.ns_res = (region.north - region.south) / (1536*lpriv->entry->vert_frames);
  region.ew_res = (region.east - region.west) / (1536*lpriv->entry->horiz_frames);

  if (l->sel.F == Matrix) {
    if (!ecs_TileInitialize( s, &(lpriv->tilestruct), &(region), 
			     lpriv->entry->horiz_frames, lpriv->entry->vert_frames, 
			     1536, 1536, dyn_PointCallBack, NULL)) { 
      ecs_SetError(&(s->result), 1, "Unable to retrieve tile structure.");
      return FALSE;
    }
  } else {
    if (!ecs_TileInitialize( s, &(lpriv->tilestruct), &(region), 
			     lpriv->entry->horiz_frames, lpriv->entry->vert_frames, 
			     1536, 1536, dyn_ImagePointCallBack, NULL)) { 
      ecs_SetError(&(s->result), 1, "Unable to retrieve tile structure.");
      return FALSE;
    }
  }

  /* Check if it's a black and white image */

  lpriv->isColor = TRUE;
  if (strstr(spriv->toc->entries[i].type,"CIB") != NULL) {  
    lpriv->isColor = FALSE;
  }

  return TRUE;
}

/*
*******************************************************************

FUNCTION_INFORMATION

NAME

    _read_rpftile

DESCRIPTION

    Initialize the current layer private structure with the layer resquest
    information.

END_DESCRIPTION

PRECONDITIONS

    A dyn_SelectLayer must have been call previously succesfully.

END_PRECONDITIONS

POSTCONDITIONS

    The layer structure contain a complete tile for the position 
    tile_row and tile_col in the tile grid.

END_POSTCONDITIONS

PARAMETERS

    INPUT

        ecs_Server *s: The driver information
        ecs_Layer *l: The current layer
        int tile_row: The requested tile row
        int tile_col: The requested tile column.

END_PARAMETERS

RETURN_VALUE

    int: Indicate if it's a success or a error situation.

END_FUNCTION_INFORMATION

PSEUDOCODE

    If tile_row and tile_col is the same than the one in lpriv,
    return a success.

    Free the previous tile in the layer. If isActive is true, erase
    all the substructures related to this layer (except entry and
    tilestruct).

    Allocate lpriv->ff. If a problem occur, set isActive to false
    and return an error message.

    Parse the frame with parse_frame. The framefile name is in the TOC.
    If a problem occur, set isActive to false and return an error message.

    With the header of ff, calculate the region of the current tile and
    the attributes lpriv->columns and lpriv->rows.

        origin_lat = lpriv->ff->cover.nw_lat - 1536.E0*lpriv->ff->cover.vert_interval;
        origin_lon = lpriv->ff->cover.nw_long + 1536.E0*lpriv->ff->cover.horiz_interval;
        lon_spacing = lpriv->ff->cover.horiz_interval;
        lat_spacing = lpriv->ff->cover.vert_interval;
        lpriv->columns = 1536L;
        lpriv->rows = 1536L;
        lpriv->region.north = origin_lat+1+lat_spacing/2;
        lpriv->region.south = origin_lat-lat_spacing/2;
        lpriv->region.west = origin_lon-lon_spacing/2;
        lpriv->region.east = origin_lon+1+lon_spacing/2;
        lpriv->region.ns_res = (lpriv->region.north - lpriv->region.south) / lpriv->rows;
        lpriv->region.ew_res = (lpriv->region.east - lpriv->region.west) / lpriv->columns;

    Allocate lpriv->rgb_pal, the color palette. If a problem occur, 
    set isActive to false and return an error message.

    Allocate lpriv->cct. If a problem occur, set isActive to false 
    and return an error message.

    Allocate lpriv->rpf_table. If a problem occur, 
    set isActive to false and return an error message.

    Call parse_clut.

    Call get_comp_lut.

    Allocate the tile buffer. If a problem occur, 
    set isActive to false and return an error message.

    For each value i between 0 and 6
    Begin

        For each value j between 0 and 6
        Begin

            Call get_rpf_image_tile for the tile i*6+j. 
            Set lpriv->buffertile[i*6+j].isActive to true.

        End

    End

    Set equi_cat with the color category equivalence

    Return a success message.

END_PSEUDOCODE

*******************************************************************
*/

int dyn_read_rpftile(s, l, tile_row, tile_col)
    ecs_Server *s;
    ecs_Layer *l;
    int tile_row;
    int tile_col;
{
  /*double origin_lon, origin_lat;*/
  /*double lat_spacing, lon_spacing;*/
  register LayerPrivateData *lpriv = (LayerPrivateData *) l->priv;
  int i,j,k;
  char *framefile;
  const char *dir;
  
  /*
    Check if tile_row and tile_col is the same than the one in lpriv.
    */

  if ((tile_row == lpriv->tile_row) &&
      (tile_col == lpriv->tile_col)) {
    return TRUE;
  }

  /* Free the previous tile in the layer */

  if (lpriv->ff != NULL)
    free(lpriv->ff);
  if (lpriv->rgb_pal != NULL)
    free(lpriv->rgb_pal);
  if (lpriv->rpf_table != NULL)
    free(lpriv->rpf_table);
  if (lpriv->cct != NULL)
    free(lpriv->cct);
  if (lpriv->buffertile != NULL)
    free(lpriv->buffertile);
  lpriv->tile_row = tile_row;
  lpriv->tile_col = tile_col;
  lpriv->mincat = 0;
  lpriv->maxcat = 0;
  lpriv->firstcoordfilepos = 0;
  lpriv->ff = NULL;
  lpriv->rgb_pal = NULL;
  lpriv->n_pal_cols = 0;
  lpriv->rpf_table = NULL;
  lpriv->blackpixel = 0;
  lpriv->cct = NULL;
  lpriv->boundary_num = 0;
  lpriv->firsttile = 0;
  lpriv->buffertile = NULL;
  lpriv->isActive = lpriv->entry->frames[tile_col][tile_row].exists;
  lpriv->rows = lpriv->entry->frames[tile_col][tile_row].frame_row;
  lpriv->columns = lpriv->entry->frames[tile_col][tile_row].frame_col;

  if (lpriv->isActive == FALSE)
    return TRUE;

  lpriv->ff = (Frame_file *)malloc(sizeof(Frame_file));
  if (lpriv->ff == NULL) {
    ecs_SetError(&(s->result),1,"not enough memory");      
    return FALSE;
  }

  /* Create the framefile string */

  framefile = malloc(strlen(lpriv->entry->frames[tile_col][tile_row].directory)+
		     strlen(lpriv->entry->frames[tile_col][tile_row].filename)+3);
  if (framefile == NULL) {
    lpriv->isActive = FALSE;
    ecs_SetError(&(s->result),1,"Cannot parse frame file"); 
    free(lpriv->ff);
    lpriv->ff = NULL;
    return FALSE; 
  }

  dir = lpriv->entry->frames[tile_col][tile_row].directory;

  if( dir[strlen(dir)-1] == '\\' || dir[strlen(dir)-1] == '/' )
      sprintf( framefile, "%s%s", dir, 
               lpriv->entry->frames[tile_col][tile_row].filename);
  else
      sprintf( framefile, "%s%c%s", dir, DIR_CHAR,
               lpriv->entry->frames[tile_col][tile_row].filename);

  /* Parse the frame */
  
  if (!parse_frame(s, lpriv->ff, framefile)) {
    lpriv->isActive = FALSE;
    free(framefile);
    ecs_SetError(&(s->result),1,"Cannot parse frame file"); 
    free(lpriv->ff);
    lpriv->ff = NULL;
    return FALSE; 
  }

  /*origin_lat = lpriv->ff->cover.nw_lat  - 1536.E0*lpriv->ff->cover.vert_interval;
  origin_lon = lpriv->ff->cover.nw_long + 1536.E0*lpriv->ff->cover.horiz_interval;*/
  /*lon_spacing = lpriv->ff->cover.horiz_interval;
  lat_spacing = lpriv->ff->cover.vert_interval;*/
  
  lpriv->columns = 1536L;
  lpriv->rows = 1536L;

  lpriv->rgb_pal = (Rgb *) malloc(217L*sizeof(Rgb));
  if (lpriv->rgb_pal == NULL)
  {
    lpriv->isActive = FALSE;
    free(framefile);
    ecs_SetError(&(s->result),1,"not enough memory to load rpf matrix in ram");      
    return FALSE; 
  }

  lpriv->cct = (uint *)malloc(256L*sizeof(uint));
  if (lpriv->cct == NULL)
  {
    lpriv->isActive = FALSE;
    free(framefile);
    ecs_SetError(&(s->result),1,"not enough memory to load rpf cct in ram");      
    return FALSE; 
  }
  lpriv->rpf_table = (uchar *)malloc(4096L*4L*4L);
  if (lpriv->rpf_table == NULL)
  {
    lpriv->isActive = FALSE;
    free(framefile);
    ecs_SetError(&(s->result),1,"not enough memory to load rpf table in ram");      
    return FALSE; 
  }

  parse_clut(s, lpriv->ff, framefile, lpriv->rgb_pal, 0L, lpriv->cct, lpriv->ff->NITF_hdr_len, &lpriv->n_pal_cols, &lpriv->blackpixel);

  get_comp_lut(s, lpriv->ff, framefile, lpriv->rpf_table, lpriv->cct, 0L);

  /* Read the data info if isInRam is true */
  
  lpriv->buffertile = malloc(36*sizeof(tile_mem));
  for (i=0; i<6L; i++)
  for (j=0; j<6L; j++)
  {
    k = i*6+j;
    get_rpf_image_tile(s, lpriv->ff, framefile, (int)lpriv->ff->indices[i][j], lpriv->rpf_table, lpriv->buffertile[k].data, 1L, lpriv->blackpixel);
    lpriv->buffertile[k].isActive = TRUE;
  }

  for(i=0;i<(int) lpriv->n_pal_cols;i++) {
    if (lpriv->isColor == TRUE)
      lpriv->equi_cat[i] = 
	(lpriv->rgb_pal[i].r/43)*36 +
	(lpriv->rgb_pal[i].g/43)*6 +
	lpriv->rgb_pal[i].b/43 + 1;
    else
      lpriv->equi_cat[i] = ((lpriv->rgb_pal[i].r + lpriv->rgb_pal[i].g + lpriv->rgb_pal[i].b)/3) + 1;
  }

  free(framefile);
  return TRUE;
}

/* 
   ----------------------------------------------------------
   _VerifyLocation:
   
   check if the path is valid. Check if the directory exist
   and check if the parent path contain a a.toc file.
   
   ---------------------------------------------------------- 
   */
   
int 
dyn_verifyLocation(s)
     ecs_Server *s;
{
  int returnvalue;
  DIR *dirlist;
  FILE *test;
  register ServerPrivateData *spriv = s->priv;
  char *ptr,*ptr1;

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
    test = rpf_fopen_ci( spriv->pathname, RGPF_TOC, "r" );
    if (test != NULL) {
        fclose(test);
        returnvalue = TRUE;
    }
  }

  if (!returnvalue)
  {
    ecs_SetError(&(s->result),1,"Invalid URL. The rpf directory is invalid"); 
  }
  return returnvalue;
}

/* 
   -------------------------------------------------------------------------
   _initRegionWithDefault:
   
   preparation de la fenetre globale pour le server                    
   --------------------------------------------------------------------------
   */

int dyn_initRegionWithDefault(s)
     ecs_Server *s;
{
  int i;
  register ServerPrivateData *spriv = s->priv;
  Toc_file *toc;
  Toc_entry *entry;
  double lat_min, lat_max, lon_min, lon_max;

  spriv->toc = (Toc_file *)malloc(sizeof(Toc_file));

  spriv->toc->entries = parse_toc(s, spriv->pathname, &spriv->toc->head, &spriv->toc->num_boundaries);
  if (spriv->toc->entries == (Toc_entry *)NULL)
  {
    return 0;
  }
  toc = spriv->toc;

  lat_min = lon_min =  300.E0;
  lat_max = lon_max = -300.E0;
  for (i=0; i<(int)toc->num_boundaries; i++)
  {
    if (toc->entries[i].invalid_geographics == 1L)
	  continue;
    entry = &toc->entries[i];
	if (entry->nw_lat  > lat_max) lat_max = entry->nw_lat;
	if (entry->se_lat  < lat_min) lat_min = entry->se_lat;
	if (entry->se_long > lon_max) lon_max = entry->se_long;
	if (entry->nw_long < lon_min) lon_min = entry->nw_long;
  }

  s->globalRegion.north = lat_max;
  s->globalRegion.south = lat_min;
  s->globalRegion.east  = lon_max;
  s->globalRegion.west  = lon_min;

  s->globalRegion.ns_res = (s->globalRegion.north - s->globalRegion.south)/2000.0;
  s->globalRegion.ew_res = (s->globalRegion.east - s->globalRegion.west)/2000.0;

  return 1;
}


int dyn_IsOutsideRegion(n,s,e,w,region)
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


/***********************************************************************\
*
* Function:      check_swap, swap
*
* Description:
*     
*      Handle endian-ness properly for this machine.
*
\***********************************************************************/

static int     do_swap;


/*****
  check_swap

  Verify whether we need to perform byte swapping.
*****/
void check_swap(unsigned char little_endian)
{
  unsigned char      *c_ptr;
  unsigned short   s;

  s = 1;
  c_ptr = (unsigned char *) &s;

  do_swap = 0L;
  if (*c_ptr == 0 && little_endian)
    do_swap = 1L;
  else if (*c_ptr == 1 && !little_endian)
    do_swap = 1L;
}


/*****
  swap

  Swap the bytes if necessary.
*****/
void swap(unsigned char *ptr, size_t count)
{
  if (do_swap)
  {
    unsigned char   *end, temp;

    end = &ptr[count - 1];
    while (end > ptr)
    {
      temp = *end;
      *end = *ptr;
      *ptr = temp;

      ptr++, end--;
    }
  }
}
/***********************************************************************\
*
* Function:      parse_toc
*
* Description:
*     
*      Read and parse the table of contents file.
*
\***********************************************************************/
 
#ifdef _WINDOWS
#define FILE_SEP '\\'
#else
#define FILE_SEP '/'
#endif

Toc_entry *parse_toc(ecs_Server *s, char *dir, Header *head, uint *num_boundaries)
{
  Toc_entry   *entries, *entry;
  Frame_entry   *frame;

  Location locations[4];   /* from 2 to 4 */
  FILE     *toc;
  ushort   n, path_length;
  int     i, j, k;
  char     *directory;
  ushort   boundary_id, frame_row, frame_col;
  int     new_boundary_ids=0L;
  uint     N_index_recs;            /* # frame file index records */
  
  uint     path_off;                /* offset of frame file pathname */
  ushort   Bound_rec_len;           /* Boundary record length */
  ushort   N_pathname_recs;         /* # frame file pathname records */
  ushort   Index_rec_len;           /* frame file index record length */
  uint     bnd_rec_tbl_off;         /* Bound. rect. table offset */
  uint     frm_index_tbl_off;       /* Frame file index table offset */
  char     NITF[5];
  char     string[256];
  
 /* Open input "A.TOC" */

  toc = rpf_fopen_ci( dir, RGPF_TOC, "rb" );

  if( toc  == NULL )
  {
    sprintf(string, "parsetoc: Can't open %s",RGPF_TOC);
    ecs_SetError(&(s->result),1,string);
    return((Toc_entry *)NULL);
  }

  tprintf ("parsetoc: here 1\n");

 /* Check for NITF header */

  fseek(toc, 0, SEEK_SET);
  ogdi_fread(NITF, 4L, 1, toc) ;
  NITF[4] = '\0';
  if (strcmp(NITF, "NITF") == 0) /* Match: skip NITF hdr */
  {
      char FSDWNG[6];

      head->NITF_hdr_len = 410L;

      /* Determine if conditional FSDEVG field exists */
      /* by examining FSDWNG field */
      /* Adjust hdr length by 40 characters if needed */
      /* note: rpf/cib1 products need this */
      fseek(toc, 280, SEEK_SET);
      ogdi_fread(FSDWNG, 6L, 1, toc);
      if( memcmp(FSDWNG, "999998", 6) == 0 )
          head->NITF_hdr_len += 40L;
  }
  else /* not in NITF format */
  {
    head->NITF_hdr_len = 0L;
  } /* else */

 /* Skip over NITF header, if it is there. */

  fseek(toc, head->NITF_hdr_len, SEEK_SET);

 /* Read header */

  ogdi_fread(&head->endian, sizeof(head->endian), 1, toc) ;   /* 1 byte */
  check_swap(head->endian);

  ogdi_fread(&head->hdr_sec_len, sizeof(head->hdr_sec_len), 1, toc);  /* 2 bytes */
  swap((ucharp)&head->hdr_sec_len, sizeof(head->hdr_sec_len)) ;
  ogdi_fread(head->filename, sizeof(head->filename), 1, toc);  /* 12 bytes */

 /* Read rest of header so we can write it later */

  ogdi_fread(&head->new, sizeof(head->new), 1, toc) ;
  ogdi_fread(head->standard_num, sizeof(head->standard_num), 1, toc) ;
  ogdi_fread(head->standard_date, sizeof(head->standard_date), 1, toc);
  ogdi_fread(&head->classification, sizeof(head->classification), 1, toc);
  ogdi_fread(head->country, sizeof(head->country), 1, toc);
  ogdi_fread(head->release, sizeof(head->release), 1, toc); 
  ogdi_fread(&head->loc_sec_phys_loc, sizeof(head->loc_sec_phys_loc), 1, toc) ;
  swap((ucharp)&head->loc_sec_phys_loc,sizeof(head->loc_sec_phys_loc));

 /* Fseek to start of location section */

  fseek(toc, head->loc_sec_phys_loc, SEEK_SET); 

 /* Locate the correct sections */

  locations[0].id = LOC_BOUNDARY_SECTION_SUBHEADER;  /* 148 */
  locations[1].id = LOC_BOUNDARY_RECTANGLE_TABLE;    /* 149 */
  locations[2].id = LOC_FRAME_FILE_INDEX_SUBHEADER;  /* 150 */
  locations[3].id = LOC_FRAME_FILE_INDEX_SUBSECTION; /* 151 */
  parse_locations(s, toc, locations, 4L);

  for (i = 0; i < 4; i++)
    if ((int) locations[i].phys_index == ~0)
    {
      sprintf(string, "Can't locate section %d in table of contents",locations[i].id);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }
  tprintf ("parsetoc: here 2\n");

 /* Read boundary rectangles */
 /* Number of Boundary records */

  fseek(toc, locations[0].phys_index, SEEK_SET);
  ogdi_fread(&bnd_rec_tbl_off, sizeof(bnd_rec_tbl_off), 1, toc);
  swap((ucharp)&bnd_rec_tbl_off, sizeof(bnd_rec_tbl_off));
  ogdi_fread(&n, sizeof(n), 1, toc);
  swap((ucharp)&n, sizeof(n));

  *num_boundaries = (int)n;
  sprintf(string,"parse_toc: n = %d\n",n);
  tprintf(string);

 /* Boundary record length */

  ogdi_fread(&Bound_rec_len, sizeof(Bound_rec_len), 1, toc) ;
  swap((ucharp)&Bound_rec_len, sizeof(Bound_rec_len));

  fseek(toc, locations[1].phys_index, SEEK_SET);

 /* Read Boundary rectangle records */

  entries = (Toc_entry *) malloc((size_t)n * sizeof(Toc_entry));
  if (entries == (Toc_entry *)NULL)
    {
      ecs_SetError(&(s->result),1,"Error on malloc of entries");
      return((Toc_entry *)NULL);
    }
  for (i = 0; i < (int)n; i++)
  {
    ogdi_fread(&entries[i].type, 1, 5, toc);         /* e.g. "CADRG" */
    entries[i].type[5] = '\0';
    if (i == 0L)
    {
      if (strncmp(entries[i].type,"CADRG",5) == 0)
	head->rpf_type = RPF_CADRG;
      else if (strncmp(entries[i].type,"CIB",3) == 0)
	head->rpf_type = RPF_CIB;
      else if (strncmp(entries[i].type,"CDTED",5) == 0)
	head->rpf_type = RPF_CDTED;
    }
    ogdi_fread(&entries[i].compression, 1, 5, toc);
    ogdi_fread(&entries[i].scale, 1, 12, toc);
    ogdi_fread(&entries[i].zone, 1, 1, toc);
    ogdi_fread(&entries[i].producer, 1, 5, toc);

    ogdi_fread(&entries[i].nw_lat, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].nw_long, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].sw_lat, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].sw_long, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].ne_lat, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].ne_long, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].se_lat, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].se_long, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].vert_resolution, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].horiz_resolution, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].vert_interval, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].horiz_interval, sizeof(real8), 1, toc);
    ogdi_fread(&entries[i].vert_frames, sizeof(uint), 1, toc);
    ogdi_fread(&entries[i].horiz_frames, sizeof(uint), 1, toc);

    swap((ucharp)&entries[i].nw_lat, sizeof(real8));
    swap((ucharp)&entries[i].nw_long, sizeof(real8));
    swap((ucharp)&entries[i].sw_lat, sizeof(real8));
    swap((ucharp)&entries[i].sw_long, sizeof(real8));
    swap((ucharp)&entries[i].ne_lat, sizeof(real8));
    swap((ucharp)&entries[i].ne_long, sizeof(real8));
    swap((ucharp)&entries[i].se_lat, sizeof(real8));
    swap((ucharp)&entries[i].se_long, sizeof(real8));
    swap((ucharp)&entries[i].vert_resolution, sizeof(real8));
    swap((ucharp)&entries[i].horiz_resolution, sizeof(real8));
    swap((ucharp)&entries[i].vert_interval, sizeof(real8));
    swap((ucharp)&entries[i].horiz_interval, sizeof(real8));
    swap((ucharp)&entries[i].vert_frames, sizeof(uint));
    swap((ucharp)&entries[i].horiz_frames, sizeof(uint));

    entries[i].type[5] = '\0';
    entries[i].compression[5] = '\0';
    entries[i].scale[12] = '\0';
    entries[i].zone[1] = '\0';
    entries[i].producer[5] = '\0';
	if(entries[i].nw_lat  <=  150.E0 && entries[i].nw_lat  >= -150.E0 &&
       entries[i].se_lat  <=  150.E0 && entries[i].se_lat  >= -150.E0 &&
       entries[i].nw_long <=  400.E0 && entries[i].nw_long >= -400.E0 &&
       entries[i].se_long <=  400.E0 && entries[i].se_long >= -400.E0)
      entries[i].invalid_geographics = 0L;
    else
    {
      entries[i].invalid_geographics = 1L;
      entries[i].nw_lat = 1536.E0;
      entries[i].nw_long = 0.E0;
      entries[i].sw_lat = 0.E0;
      entries[i].sw_long = 0.E0;
      entries[i].ne_lat = 1536.E0;
      entries[i].ne_long = 1536.E0;
      entries[i].se_lat = 0.E0;
      entries[i].se_long = 1536.E0;
      entries[i].vert_resolution = 1.E0;
      entries[i].horiz_resolution = 1.E0;
      entries[i].vert_interval = 1.E0;
      entries[i].horiz_interval = 1.E0;
    }
    entries[i].frames = (Frame_entry **)
      malloc((size_t)entries[i].vert_frames * sizeof(Frame_entry *));
    if (entries[i].frames == (Frame_entry **)NULL)
    {
      sprintf(string,"Error on malloc of entries[%d].frames",i);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }

    for (j = 0; j < entries[i].vert_frames; j++)
    {
      entries[i].frames[j] = (Frame_entry *)
        malloc((size_t)entries[i].horiz_frames * sizeof(Frame_entry));
      if (entries[i].frames[j] == (Frame_entry *)NULL)
      {
        sprintf(string, "Error on malloc of entries[%d].frames[%d]",i,j);
        ecs_SetError(&(s->result),1,string);
        return((Toc_entry *)NULL);
      }
      for (k = 0; k < entries[i].horiz_frames; k++) {
        entries[i].frames[j][k].exists = 0L;
	entries[i].frames[j][k].directory= (char *) NULL;
      }
    }

  }  /* for i < n_bound_recs */

 /* Read # of frame file index records */
 /* Skip 1 byte security classification */
 /* locations[2] is loc of frame file index section subheader */

  fseek(toc, locations[2].phys_index + 1, SEEK_SET);
  ogdi_fread(&frm_index_tbl_off, sizeof(frm_index_tbl_off), 1, toc);
  swap((ucharp)&frm_index_tbl_off, sizeof(frm_index_tbl_off));
  ogdi_fread(&N_index_recs, sizeof(N_index_recs), 1, toc);
  swap((ucharp)&N_index_recs, sizeof(N_index_recs));

  ogdi_fread(&N_pathname_recs, sizeof(N_pathname_recs), 1, toc);
  swap((ucharp)&N_pathname_recs, sizeof(N_pathname_recs));

  ogdi_fread(&Index_rec_len, sizeof(Index_rec_len), 1, toc);
  swap((ucharp)&Index_rec_len, sizeof(Index_rec_len));

 /* Read frame file index records */

  for (i = 0; i < (int)N_index_recs; i++)
  {

   /* locations[3] is frame file index table subsection */

    fseek(toc, locations[3].phys_index + Index_rec_len*i, SEEK_SET);

    ogdi_fread(&boundary_id, sizeof(boundary_id), 1, toc);
    swap((ucharp)&boundary_id, sizeof(boundary_id));
    if (i == 0 && boundary_id == 0)
       new_boundary_ids = 1L;
    if (new_boundary_ids == 0L)
       boundary_id--;
    
    if (boundary_id > (ushort)(*num_boundaries-1L))
    {
      sprintf(string,"Bad boundary id in FF index record %d", i);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }

    entry = &entries[boundary_id];
    entry->boundary_id = boundary_id;

    ogdi_fread(&frame_row, sizeof(frame_row), 1, toc);
    ogdi_fread(&frame_col, sizeof(frame_col), 1, toc);
    swap((ucharp)&frame_row, sizeof(frame_row));
    swap((ucharp)&frame_col, sizeof(frame_col));
    if (new_boundary_ids == 0L)
    {
      frame_row--;
      frame_col--;
    }
    else
    {
      /* Trick so that frames are numbered north to south */
      frame_row = (entry->vert_frames-1L) - frame_row;
    }
   
    if ((int)frame_row > entry->vert_frames-1L)
    {
      sprintf(string,"Bad row num:%d, in FF index record %d",frame_row,i);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }

    if ((int)frame_col > entry->horiz_frames-1L)
    {
      sprintf(string,"Bad col number in FF index record %d", i);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }

    frame = &entry->frames[frame_row][frame_col];
	
    frame->frame_row = frame_row;
    frame->frame_col = frame_col;

    if (frame->exists)
    {
      sprintf(string,"FF %d is a duplicate", i);
      ecs_SetError(&(s->result),1,string);
      return((Toc_entry *)NULL);
    }

   /* Pathname offset */

    ogdi_fread(&path_off, sizeof(path_off), 1, toc);
    swap((ucharp)&path_off, sizeof(path_off));

   /* Get filename tail */

    ogdi_fread(frame->filename, 1, 12, toc);
    frame->filename[12] = '\0';
#ifndef _WINDOWS
    dyn_string_tolower((char *) frame->filename);
#endif

    /* Check if the filename is an overview */

    for (j=0;j<12;j++) {
      if (strcmp(&(frame->filename[j]),".OVR") == 0 ||
	  strcmp(&(frame->filename[j]),".ovr") == 0) {
	entry->invalid_geographics = 1L;
	break;
      }
    }

   /* Get file geo reference */

    ogdi_fread(frame->georef, 1, 6, toc);
    frame->georef[6] = '\0';

   /* Go to start of pathname record */
   /* New path_off offset from start of frame file index section of TOC?? */
   /* Add pathoffset wrt frame file index table subsection (loc[3]) */

    fseek(toc, locations[3].phys_index + path_off, SEEK_SET); 

    ogdi_fread(&path_length, sizeof(path_length), 1, toc);
    swap((ucharp)&path_length, sizeof(path_length));

    frame->directory = (char *) malloc((size_t)path_length + (size_t)4L +
                                            strlen(dir));
    if (frame->directory == (char *)NULL)
      {
        ecs_SetError(&(s->result),1,"Error on malloc of frame->directory");
        return((Toc_entry *)NULL);
      }
    frame->directory[0] = '\0';

    directory = (char *) malloc((size_t)(path_length + 1));
    if (directory == (char *)NULL)
      {
        ecs_SetError(&(s->result),1,"Error on malloc of directory");
        return((Toc_entry *)NULL);
      }

   /* Read directory name from toc. */

    ogdi_fread(directory, 1, path_length, toc);
    directory[path_length] = '\0';

   /* Skip 1st 2 chars: "./": */

    if (directory[2] != '\0')
      strcat(frame->directory,&directory[2]);
#ifndef _WINDOWS
    dyn_string_tolower((char *) frame->directory);
#endif

    free(directory);

    frame->exists = 1L;

  }  /* for i = N_index_recs */

  fclose(toc);
  tprintf ("parsetoc: here end\n");
  sprintf(string,"parse_toc: *num_boundaries = %d\n",*num_boundaries);
  tprintf(string);

  return(entries);
}

/***********************************************************************\
*
* Function:      parse_locations
*
* Description:
*     
*      Locate sections in file.
*
\***********************************************************************/

int parse_locations(ecs_Server *s, FILE *fin, Location *locs, int count)
{
  int     i, j;
  ushort   n;
  ushort   id;
  uint     phys_index;
  ushort   us;
  uint     ui;
  (void) s;

 /* Initialize indices so we can later tell if they weren't found */

  for (j = 0; j < count; j++)
    locs[j].phys_index = (uint)(~0);

 /* Skip location section length */

  ogdi_fread(&us, sizeof(us), 1, fin);

 /* Skip component location table offset */

  ogdi_fread(&ui, sizeof(ui), 1, fin);

 /* How many sections: # of section location records */

  ogdi_fread(&n, sizeof(n), 1, fin);
  swap((ucharp)&n, sizeof(n));

 /* Skip location record length */

  ogdi_fread(&us, sizeof(us), 1, fin);

 /* Skip component aggregate length */

  ogdi_fread(&ui, sizeof(ui), 1, fin);

 /* Now go find the ones we want */

  for (i = 0; i < (int)n; i++)
  {
    ogdi_fread(&id, sizeof(id), 1, fin);

   /* Skip section length */

    ogdi_fread(&ui, sizeof(ui), 1, fin);

    ogdi_fread(&phys_index, sizeof(phys_index), 1, fin);

    swap((ucharp)&id, sizeof(id));
    swap((ucharp)&phys_index, sizeof(phys_index));

    for (j = 0; j < count; j++)
      if (locs[j].id == id)
        locs[j].phys_index = phys_index;

  }  /* for i */
  
  return(1L);

}   /* parse_locations */

/***********************************************************************\
*
* Function:      parse_clut
*
* Description:
*     
*   Read and parse just the colortable information in a frame file.
*
\***********************************************************************/

#define MAXOFFSETRECS 10  /* Max # of color/gray offset records: usually 5 */
#define MAXCCRECS 5       /* Max # of color converter records: usually 2   */

int 
parse_clut(s, frame, filename, rgb, ReducedColorTable, cct, Nitf_hdr_len, n_cols, blackpixel)
ecs_Server *s;
Frame_file *frame;
char *filename;
Rgb *rgb;
int ReducedColorTable;
uint *cct;
int Nitf_hdr_len;
uint *n_cols;
uchar *blackpixel;
{
  FILE     *fin;
  Location loc[3];
  int     i,j;
  uint     loc_sec_phys_loc;     /* location section physical location */
  uchar    N_offset_recs;        /* # of offset records */
  uchar    N_cc_offset_recs;     /* # of color converter offset records */
  ushort   Offset_rec_len = 17;  /* offset record length */
  Color_offset  *col_off;        /* color offset: array of records. */
  ushort   table_id[MAXOFFSETRECS];         /* Color/gray offset record fields*/
  uint     n_col_recs[MAXOFFSETRECS];       /* # color records */
  uchar    col_elem_len[MAXOFFSETRECS];     /* color element length */
  ushort   hist_rec_len[MAXOFFSETRECS];     /* histogram record length */
  uint     color_tbl_offset[MAXOFFSETRECS]; /* color table offset */
  uint     hist_tbl_offset[MAXOFFSETRECS];  /* histogram table offset */
  uint     clrmap_off_tbl_off;   /* colormap offset table offset */
  /* color converter subsection hdr */
  uint     cct_off_tbl_off;        /* color convertion offset table offset */
  ushort   cct_off_recl;           /* cc offset recl */
  ushort   cct_recl;               /* cc recl */
  /* color converter offset record fields */
  ushort   cct_id[MAXCCRECS];      /* color conversion table ID */
  uint     cct_nrec[MAXCCRECS];    /* # cc recs */
  uint     cct_tbl_off[MAXCCRECS]; /* cc table offset */
  uint     cct_src[MAXCCRECS];     /* cc src color/gray offset table offset */
  uint     cct_tgt[MAXCCRECS];     /* cc tgt color/gray offset table offset */
  double   mindistblackp;          /* minimum distance for black pixel */
  double   dist;                   /* distance for black pixel */
  double   r,g,b;
  char     string[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;

  (void) frame;

  fin = rpf_fopen_ci( spriv->pathname, filename, "rb" );
  if (fin == NULL )
  {
    sprintf(string,"Can't open %s",filename);
    ecs_SetError(&(s->result),1,string);
    return(0L);
  }

 /* Skip NITF header if there */

  fseek(fin, Nitf_hdr_len, SEEK_SET);

 /* Skip header up to location of location section: 48-4=44 */

  fseek(fin, 48-4, SEEK_CUR);

  ogdi_fread(&loc_sec_phys_loc, sizeof(loc_sec_phys_loc), 1, fin);
  swap((ucharp)&loc_sec_phys_loc, sizeof(loc_sec_phys_loc));

 /* Go to location section */

  fseek(fin, loc_sec_phys_loc, SEEK_SET);

 /* Locate the proper section */

  loc[0].id = LOC_COLORGRAY_SECTION_SUBHEADER;  /* 134 */
  loc[1].id = LOC_COLORMAP_SUBSECTION;          /* 135 */
  loc[2].id = LOC_COLOR_CONVERTOR_SUBSECTION;   /* 139 */
  parse_locations(s, fin, loc, 3L);

  if ((int) loc[0].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find color/gray section subheader (ID=134) location");
    return(0L);
  }

 /* Go find the color table: loc[0].id=LOC_CLUT */

  fseek(fin, loc[0].phys_index, SEEK_SET);

 /* Read section subheader */
 /* Number of offset records:1-5  */

  ogdi_fread(&N_offset_recs, sizeof(N_offset_recs), 1, fin);

 /* Number of color converter offset records */

  ogdi_fread(&N_cc_offset_recs, sizeof(N_cc_offset_recs), 1, fin);

 /* Read colormap offset table */

  col_off = (Color_offset *) malloc (sizeof(Color_offset)*
                                          (size_t)N_offset_recs);
  if (col_off == (Color_offset *)NULL)
  {
    ecs_SetError(&(s->result),1,"Parse_clut malloc error on col_off");
    return(0L) ;
  }

 /* Check for colormap subsection: id = 135 */
 /* ?? can't find this section */

  if ((int) loc[1].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find colormap subsection location ID=135");
    free((char *)col_off);
    return(0L);
  }

 /* Read color/gray offset records (colormap subsection) */

  fseek(fin, loc[1].phys_index, SEEK_SET);

 /* colormap offset table offset length:4 */

  ogdi_fread(&clrmap_off_tbl_off, sizeof(clrmap_off_tbl_off), 1, fin);
  swap((ucharp)&clrmap_off_tbl_off, sizeof(clrmap_off_tbl_off));

 /* offset record length:17 */

  ogdi_fread(&Offset_rec_len, sizeof(Offset_rec_len), 1, fin);
  swap((ucharp)&Offset_rec_len, sizeof(Offset_rec_len));

 /* Read colormap offset table */

  for (i=0; i<N_offset_recs; i++)
  {

   /* id: 3:grayscale 4:rgbm216, 5:rgbm32, 6:rgbm16 */

    ogdi_fread(&table_id[i], sizeof(table_id[i]), 1, fin);
    swap((ucharp)&table_id[i], sizeof(table_id[i]));

    ogdi_fread(&n_col_recs[i], sizeof(n_col_recs[i]), 1, fin);
    swap((ucharp)&n_col_recs[i], sizeof(n_col_recs[i]));

    ogdi_fread(&col_elem_len[i], sizeof(col_elem_len[i]), 1, fin);

   /* Moved down to here */

    ogdi_fread(&hist_rec_len[i], sizeof(hist_rec_len[i]), 1, fin);
    swap((ucharp)&hist_rec_len[i], sizeof(hist_rec_len[i]));

   /* Color table offset */

    ogdi_fread(&color_tbl_offset[i], sizeof(color_tbl_offset[i]), 1, fin);
    swap((ucharp)&color_tbl_offset[i], sizeof(color_tbl_offset[i]));

   /* Hist. table offset */

    ogdi_fread(&hist_tbl_offset[i], sizeof(hist_tbl_offset[i]), 1, fin);
    swap((ucharp)&hist_tbl_offset[i], sizeof(hist_tbl_offset[i]));

  } /* for i = N_offset_recs */

   /* Go to proper file position wrt colormap subsection (loc[1]) */

  for (i=0; i<N_offset_recs; i++)
  {

    fseek(fin, loc[1].phys_index + color_tbl_offset[i], SEEK_SET) ;

    if (table_id[i] == 3 ||           /* read, use grayscale  */
        (ReducedColorTable == 0L &&   /* read, use colortable */
         i == 0L))
    {
      *n_cols = n_col_recs[i];
      *blackpixel = 0;
      mindistblackp = 1.E20;
      for (j = 0; j < (int)n_col_recs[i]; j++)     /* 216, 32, or 16 */
      {
        if (table_id[i] == 3L)
        {
           ogdi_fread(&rgb[j].r, 1, 1, fin);             /* rgb is mono */
           rgb[j].g = rgb[j].b = rgb[j].r;
        }
        else
           ogdi_fread(&rgb[j], sizeof(rgb[j]), 1, fin);  /* rgb is rgba, size=4 */
        r = (double)(unsigned char)(rgb[j].r);
        g = (double)(unsigned char)(rgb[j].g);
        b = (double)(unsigned char)(rgb[j].b);
        dist = sqrt(r*r+g*g+b*b);
        if (dist < mindistblackp)
        {
          mindistblackp = dist;
          *blackpixel = (uchar)j;
        }
      } /* for j */

    } /* if table_id */

  } /* for i = N_offset_recs */

  if (ReducedColorTable != 0)
  {
    /* Read cct records */

    fseek(fin, loc[2].phys_index, SEEK_SET);
 
    ogdi_fread(&cct_off_tbl_off, sizeof(cct_off_tbl_off), 1, fin);
    swap ((ucharp)&cct_off_tbl_off, sizeof(cct_off_tbl_off)) ;
 
    ogdi_fread(&cct_off_recl, sizeof(cct_off_recl), 1, fin);
    swap ((ucharp)&cct_off_recl, sizeof(cct_off_recl)) ;
 
    ogdi_fread(&cct_recl, sizeof(cct_recl), 1, fin);
    swap ((ucharp)&cct_recl, sizeof(cct_recl)) ;
 
   /* Read colormap conversion table */

    for (i=0; i<N_cc_offset_recs; i++)
    {

     /* id: 128:cct/rgbm/32, 129:cct/rgbm/16 */

      ogdi_fread(&cct_id[i], sizeof(cct_id[i]), 1, fin);
      swap((ucharp)&cct_id[i], sizeof(cct_id[i]));

      ogdi_fread(&cct_nrec[i], sizeof(cct_nrec[i]), 1, fin);
      swap((ucharp)&cct_nrec[i], sizeof(cct_nrec[i]));

      ogdi_fread(&cct_tbl_off[i], sizeof(cct_tbl_off[i]), 1, fin);
      swap((ucharp)&cct_tbl_off[i], sizeof(cct_tbl_off[i]));

      ogdi_fread(&cct_src[i], sizeof(cct_src[i]), 1, fin);
      swap((ucharp)&cct_src[i], sizeof(cct_src[i]));

      ogdi_fread(&cct_tgt[i], sizeof(cct_tgt[i]), 1, fin);
      swap((ucharp)&cct_tgt[i], sizeof(cct_tgt[i]));

    } /* for i = N_cc_offset_recs */

    for (i=0; i<N_cc_offset_recs; i++)
    {
     /* Go to proper file position wrt color map subsection (loc[1]) */

      fseek(fin, loc[1].phys_index + cct_tgt[i], SEEK_SET) ;

     /* id: 3:grayscale 4:rgbm216, 5:rgbm32, 6:rgbm16 */

      ogdi_fread(&table_id[i], sizeof(table_id[i]), 1, fin);
      swap((ucharp)&table_id[i], sizeof(table_id[i]));

      ogdi_fread(&n_col_recs[i], sizeof(n_col_recs[i]), 1, fin);
      swap((ucharp)&n_col_recs[i], sizeof(n_col_recs[i]));

      ogdi_fread(&col_elem_len[i], sizeof(col_elem_len[i]), 1, fin);

     /* Moved down to here */

      ogdi_fread(&hist_rec_len[i], sizeof(hist_rec_len[i]), 1, fin);
      swap((ucharp)&hist_rec_len[i], sizeof(hist_rec_len[i]));

     /* Color table offset */

      ogdi_fread(&color_tbl_offset[i], sizeof(color_tbl_offset[i]), 1, fin);
      swap((ucharp)&color_tbl_offset[i], sizeof(color_tbl_offset[i]));

     /* Hist. table offset */

      ogdi_fread(&hist_tbl_offset[i], sizeof(hist_tbl_offset[i]), 1, fin);
      swap((ucharp)&hist_tbl_offset[i], sizeof(hist_tbl_offset[i]));

     /* Go to proper file position wrt colormap subsection (loc[1]) */

      fseek(fin, loc[1].phys_index + color_tbl_offset[i], SEEK_SET) ;

      if ((i+1L) == ReducedColorTable)
      {
        *blackpixel = 0;
        mindistblackp = 1.E20;
        for (j = 0; j < (int)n_col_recs[i]; j++)     /* 32 or 16 */
        {
          ogdi_fread(&rgb[j], sizeof(rgb[j]), 1, fin);  /* rgb is rgba, size=4 */
          r = (double)(unsigned char)(rgb[j].r);
          g = (double)(unsigned char)(rgb[j].g);
          b = (double)(unsigned char)(rgb[j].b);
          dist = sqrt(r*r+g*g+b*b);
          if (dist < mindistblackp)
          {
            mindistblackp = dist;
            *blackpixel = (uchar)j;
          }
        }
      }

    } /* for i = N_cc_offset_recs */

    for (i=0; i<N_cc_offset_recs; i++)
    {
     /* Go to proper file position wrt color conversion subsection (loc[2]) */

      fseek(fin, loc[2].phys_index + cct_tbl_off[i], SEEK_SET) ;

      if ((i+1L) == ReducedColorTable)
      {
        ogdi_fread(cct, sizeof(uint), 216, fin);
      } /* if table_id */

    } /* for i = N_cc_offset_recs */
  }

  if (ReducedColorTable==0)   /* 216 colors chosen */
    for (j=0; j<CADRG_COLORS; j++)
      cct[j] = (uint)j ;
  
  fclose(fin);
  free((char *)col_off);

  return 0;
}  /* parse_clut */

/***********************************************************************\
*
* Function:  parse_frame
*
* Description:
*     
*      Parse the frame file format.
*
*
\***********************************************************************/

int
parse_frame(s, file, filename)
ecs_Server *s;
Frame_file *file;
char *filename;
{
  Location loc[11];
  FILE    *fin;
  int     i, j;
  char     NITF[5];
  ascii    date[8];
  uint     lkup_off_tbl_off;     /* 2lookup offset table offset */
  ushort   lkup_tbl_off_recl;    /* lookup table offset record length */
  uint     subfr_mask_tbl_off;   /* subframe mask table offset */
  int      nsubfr=0;               /* the number of subframes */
  char     string[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;

  fin = rpf_fopen_ci( spriv->pathname, filename, "rb" );
  if (fin == NULL)
    {
      sprintf(string,"Can't open frame file %s",filename);
      ecs_SetError(&(s->result),1,string);
      return(0L);
    }

 /* Check for NITF header */

  fseek(fin, 0, SEEK_SET);
  ogdi_fread(NITF, 4L, 1, fin) ;
  NITF[4] = '\0';
  if (strcmp(NITF, "NITF") == 0) /* Match: skip NITF hdr */
  {
    fseek(fin,413, SEEK_SET) ;
    fseek(fin, 31, SEEK_CUR) ;  /* Key off date, pos 32 in RPF header */
    ogdi_fread( date, sizeof(date), 1, fin);
    if (strncmp(date,"199",3) == 0)        /* Check for short 413 hdr */
       file->NITF_hdr_len = 413L;
    else                                   /* Else long 426 hdr */
       file->NITF_hdr_len = 426L;
  }
  else /* not in NITF format */
  {
    file->NITF_hdr_len = 0L;
  } /* else */

 /* Skip Nitf header */

  fseek(fin, file->NITF_hdr_len, SEEK_SET);

 /* Read header */

  ogdi_fread(&file->head.endian, sizeof(file->head.endian), 1, fin) ;
  check_swap(file->head.endian);
  ogdi_fread(&file->head.hdr_sec_len, sizeof(file->head.hdr_sec_len), 1, fin);
  ogdi_fread( file->head.filename, sizeof(file->head.filename), 1, fin);
  ogdi_fread(&file->head.new, sizeof(file->head.new), 1, fin) ;
  ogdi_fread( file->head.standard_num, sizeof(file->head.standard_num), 1, fin) ;
  if (strncmp(&file->head.standard_num[9],"41",2) == 0)
     file->head.rpf_type = RPF_CIB;
  else if (strncmp(&file->head.standard_num[9],"38",2) == 0)
     file->head.rpf_type = RPF_CADRG;
  else if (strncmp(&file->head.standard_num[9],"44",2) == 0)
     file->head.rpf_type = RPF_CDTED;
  ogdi_fread( file->head.standard_date, sizeof(file->head.standard_date), 1, fin);
  ogdi_fread(&file->head.classification, sizeof(file->head.classification), 1,fin);
  ogdi_fread( file->head.country, sizeof(file->head.country), 1, fin);
  ogdi_fread( file->head.release, sizeof(file->head.release), 1, fin); 
  ogdi_fread(&file->head.loc_sec_phys_loc, sizeof(file->head.loc_sec_phys_loc), 1, fin) ;

  swap((ucharp)&file->head.hdr_sec_len, sizeof(file->head.hdr_sec_len)) ;
  swap((ucharp)&file->head.loc_sec_phys_loc, sizeof(file->head.loc_sec_phys_loc));

 /* Position to start of location section: 2 choices: */

  fseek(fin, file->head.loc_sec_phys_loc, SEEK_SET) ;  

 /* Locate the sections we need */

  loc[0].id = LOC_COMPRESSION_SECTION;              /* 131 */
  loc[1].id = LOC_IMAGE_DESCR_SUBHEADER;            /* 136 */
  if (file->head.rpf_type == RPF_CDTED)
     loc[2].id = LOC_COMPRESSION_PARAMETER_SUBSECTION; /* 133 */
  else
     loc[2].id = LOC_COMPRESSION_LOOKUP_SUBSECTION;    /* 132 */
  loc[3].id = LOC_SPATIAL_DATA_SUBSECTION;          /* 140 */
  loc[4].id = LOC_IMAGE_DISPLAY_PARAM_SUBHEADER;    /* 137 */
  loc[5].id = LOC_MASK_SUBSECTION;                  /* 138 */
  loc[6].id = LOC_COVERAGE_SECTION;                 /* 130 */
  loc[7].id = LOC_REPLACE_UPDATE_SECTION_SUBHEADER; /* 146 */
  loc[8].id = LOC_REPLACE_UPDATE_TABLE;             /* 147 */
  loc[9].id = LOC_ATTRIBUTE_SECTION_SUBHEADER;      /* 141 */
  loc[10].id = LOC_ATTRIBUTE_SUBSECTION;            /* 142 */

  parse_locations(s, fin, loc, 11L);

  if ((int) loc[0].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find LOC_COMPRESSION section in FF");
  }
  if ((int) loc[1].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find LOC_IMAGE section in FF");
    return(0L);
  }

 /* Read the coverage section */

  if ((int) loc[6].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find LOC_COVERAGE_SECTION in FF");
    return(0L);
  }

  ogdi_fread(&file->cover.nw_lat, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.nw_long, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.sw_lat, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.sw_long, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.ne_lat, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.ne_long, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.se_lat, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.se_long, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.vert_resolution, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.horiz_resolution, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.vert_interval, sizeof(real8), 1, fin);
  ogdi_fread(&file->cover.horiz_interval, sizeof(real8), 1, fin);

  swap((ucharp)&file->cover.nw_lat, sizeof(real8));
  swap((ucharp)&file->cover.nw_long, sizeof(real8));
  swap((ucharp)&file->cover.sw_lat, sizeof(real8));
  swap((ucharp)&file->cover.sw_long, sizeof(real8));
  swap((ucharp)&file->cover.ne_lat, sizeof(real8));
  swap((ucharp)&file->cover.ne_long, sizeof(real8));
  swap((ucharp)&file->cover.se_lat, sizeof(real8));
  swap((ucharp)&file->cover.se_long, sizeof(real8));
  swap((ucharp)&file->cover.vert_resolution, sizeof(real8));
  swap((ucharp)&file->cover.horiz_resolution, sizeof(real8));
  swap((ucharp)&file->cover.vert_interval, sizeof(real8));
  swap((ucharp)&file->cover.horiz_interval, sizeof(real8));

 /* Read the compression tables */

  if ((int) loc[0].phys_index != ~0)
  {
     fseek(fin, loc[0].phys_index, SEEK_SET);
     ogdi_fread(&file->compr.algorithm   , sizeof(file->compr.algorithm   ), 1, fin);
     ogdi_fread(&file->compr.noff_recs   ,sizeof(file->compr.noff_recs   ), 1,fin);
     ogdi_fread(&file->compr.nparm_off_recs,sizeof(file->compr.nparm_off_recs), 1,fin);

     swap((ucharp)&file->compr.algorithm     ,sizeof(file->compr.algorithm   ));
     swap((ucharp)&file->compr.noff_recs     ,sizeof(file->compr.noff_recs   ));
     swap((ucharp)&file->compr.nparm_off_recs,sizeof(file->compr.noff_recs   ));
  }

/* If not CDTED read lookup table section */

  if (file->head.rpf_type != RPF_CDTED)
  {
    file->loc_lut_shdr = loc[2].phys_index;
    if ((int) loc[2].phys_index == ~0)
    {
      ecs_SetError(&(s->result),1,"Warning: Can't find compr. lookup subsection in FrameFile: Using alternate computation");

     /* length of compr. sect. subhdr = 10 */

      fseek(fin, loc[0].phys_index + 10, SEEK_SET);
    }
    else
    {
      fseek(fin, loc[2].phys_index, SEEK_SET);
    } /* else */

   /* Read Header fields for CIB */

    ogdi_fread(&lkup_off_tbl_off, sizeof(lkup_off_tbl_off), 1, fin);
    ogdi_fread(&lkup_tbl_off_recl, sizeof(lkup_tbl_off_recl), 1, fin);
    swap ((ucharp)&lkup_off_tbl_off, sizeof(lkup_off_tbl_off)) ;
    swap ((ucharp)&lkup_tbl_off_recl, sizeof(lkup_tbl_off_recl)) ;

    for (i=0; i<4; i++)
    {
      ogdi_fread(&file->lut[i].id         , sizeof(file->lut[i].id         ), 1,fin);
      ogdi_fread(&file->lut[i].records    , sizeof(file->lut[i].records    ), 1, fin);
      ogdi_fread(&file->lut[i].values     , sizeof(file->lut[i].values     ), 1, fin);
      ogdi_fread(&file->lut[i].bit_length , sizeof(file->lut[i].bit_length ), 1, fin);
      ogdi_fread(&file->lut[i].phys_offset, sizeof(file->lut[i].phys_offset), 1, fin);
      swap((ucharp)&file->lut[i].id         , sizeof(file->lut[i].id         ));
      swap((ucharp)&file->lut[i].records    , sizeof(file->lut[i].records    ));
      swap((ucharp)&file->lut[i].values     , sizeof(file->lut[i].values     ));
      swap((ucharp)&file->lut[i].bit_length , sizeof(file->lut[i].bit_length ));
      swap((ucharp)&file->lut[i].phys_offset, sizeof(file->lut[i].phys_offset));

      if (file->lut[i].records    != 4096 ||
          file->lut[i].values     != 4 ||
          file->lut[i].bit_length != 8)
      {
        ecs_SetError(&(s->result),1,"Bad VQ info in compression record");
        return(0L);
      }

    }

    /* For each compression table */
    for (i = 0; i < 4; i++)
    {

     /* Position from compression lookup subsection: loc[2] */

      fseek(fin, loc[2].phys_index + file->lut[i].phys_offset, SEEK_SET);

     /* Skip tables */

      fseek(fin, 4096*4, SEEK_CUR);

    }  /* for i=1 to 4 (# compression tables, 1 for each pixel row)  */
  }

/* If CDTED read compression parameter section */

  else if (file->head.rpf_type == RPF_CDTED)
  {
    uint  comp_off_tbl_off;
    ushort comp_off_recl;
    int bits_off;
    int bitshuff_off;
    uchar bits[16];
    int hufflen=0;

    file->comp_parm_shdr = loc[2].phys_index;
    if ((int) loc[2].phys_index == ~0)
    {
      ecs_SetError(&(s->result),1,"Warning: Can't find compr. parameter subsection in FrameFile");
      return(0L);

    }
    else
    {
      fseek(fin, loc[2].phys_index, SEEK_SET);
    } /* else */

   /* Read Header fields for CDTED */

    ogdi_fread(&comp_off_tbl_off, sizeof(comp_off_tbl_off), 1, fin);
    ogdi_fread(&comp_off_recl, sizeof(comp_off_recl), 1, fin);
    swap ((ucharp)&comp_off_tbl_off, sizeof(comp_off_tbl_off)) ;
    swap ((ucharp)&comp_off_recl, sizeof(comp_off_recl)) ;

    file->comp = (Comp_parm *)malloc(sizeof(Comp_parm)*
                                             file->compr.nparm_off_recs);
    bitshuff_off = ~0;
    for (i=0; i<file->compr.nparm_off_recs; i++)
    {
      ogdi_fread(&file->comp[i].id     , sizeof(file->comp[i].id     ), 1,fin);
      ogdi_fread(&file->comp[i].rec_off, sizeof(file->comp[i].rec_off), 1, fin);
      swap((ucharp)&file->comp[i].id     , sizeof(file->comp[i].id     ));
      swap((ucharp)&file->comp[i].rec_off, sizeof(file->comp[i].rec_off));

      if (file->comp[i].id == 1)
        bitshuff_off = file->comp[i].rec_off;
      
    }  /* for i */

    if (bitshuff_off == ~0)
    {
      ecs_SetError(&(s->result),1,"Compression param ID 1 not found");
      return(0L);
    }

    bits_off = bitshuff_off + 7 ;    /* skip 4 vars in tbl: 2+2+2+1 */

   /* Skip Huffman bits */

    file->comp_parm_shdr = loc[2].phys_index + bits_off;
    fseek(fin, loc[2].phys_index + bits_off, SEEK_SET);
    ogdi_fread(bits, 16, 1, fin) ;    /* bits array */
    for (i=0; i<16; i++)
       hufflen += bits[i];   /* len of huffval table */
    fseek(fin, hufflen, SEEK_CUR);

  }

 /* Read the image data */

  fseek(fin, loc[1].phys_index, SEEK_SET);
  ogdi_fread(&file->img.spectral_groups, sizeof(file->img.spectral_groups), 1, fin);
  ogdi_fread(&file->img.subframe_tables, sizeof(file->img.subframe_tables), 1, fin);
  ogdi_fread(&file->img.spectral_tables, sizeof(file->img.spectral_tables), 1, fin);
  ogdi_fread(&file->img.spectral_lines,  sizeof(file->img.spectral_lines ), 1, fin);
  ogdi_fread(&file->img.horiz_subframes, sizeof(file->img.horiz_subframes), 1, fin);
  ogdi_fread(&file->img.vert_subframes,  sizeof(file->img.vert_subframes ), 1, fin);
  ogdi_fread(&file->img.output_cols,     sizeof(file->img.output_cols    ), 1, fin);
  ogdi_fread(&file->img.output_rows,     sizeof(file->img.output_rows    ), 1, fin);

  swap((ucharp)&file->img.spectral_groups, sizeof(file->img.spectral_groups));
  swap((ucharp)&file->img.subframe_tables, sizeof(file->img.subframe_tables));
  swap((ucharp)&file->img.spectral_tables, sizeof(file->img.spectral_tables));
  swap((ucharp)&file->img.spectral_lines,  sizeof(file->img.spectral_lines));
  swap((ucharp)&file->img.horiz_subframes, sizeof(file->img.horiz_subframes));
  swap((ucharp)&file->img.vert_subframes,  sizeof(file->img.vert_subframes));
  swap((ucharp)&file->img.output_cols,     sizeof(file->img.output_cols));
  swap((ucharp)&file->img.output_rows,     sizeof(file->img.output_rows));
  ogdi_fread(&subfr_mask_tbl_off, sizeof(subfr_mask_tbl_off), 1, fin) ;
  swap((ucharp)&subfr_mask_tbl_off,sizeof(subfr_mask_tbl_off));
  if (subfr_mask_tbl_off == 0xFFFFFFFFL)
     file->all_subframes = 1;  /* TRUE */
  else
     file->all_subframes = 0 ;   /* FALSE */

 /* Fseek to LOC_IMAGE_DISPLAY_PARAM_SUBHEADER, ID=137 */

  if ((int) loc[4].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find IMAGE_DISPLAY_PARAM_SUBHEADER section in Frame file");
    return(0L);
  }

 /* Image Display Parameters Subheader */

  fseek(fin, loc[4].phys_index, SEEK_SET);

  if (file->head.rpf_type == RPF_CDTED)
    nsubfr = 1;
  else if (file->head.rpf_type == RPF_CADRG || file->head.rpf_type == RPF_CIB)
    nsubfr = 6;
  if (file->img.vert_subframes  != nsubfr ||
      file->img.horiz_subframes != nsubfr)
  {
    ecs_SetError(&(s->result),1,"Bad number of subframes per frame");
    return(0L);
  }

 /* Fseek to LOC_SPATIAL_DATA_SUBSECTION, ID=140 */

  file->loc_data = loc[3].phys_index;
  if ((int) loc[3].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find SPATIAL_DATA_SUBSECTION section in Frame file");
    return(0L);
  }

 /* Read the mask data */

  if (!file->all_subframes)
  {

   /* Fseek to LOC_MASK_SUBSECTION, ID=138 */

    if ((int) loc[5].phys_index == ~0)
    {
      ecs_SetError(&(s->result),1,"Can't find MASK_SUBSECTION section in Frame file");
      return(0L);
    }
    fseek(fin, loc[5].phys_index, SEEK_SET);
    fseek(fin, subfr_mask_tbl_off, SEEK_CUR);    /* go to offset: skip hdr */
    for (i=0; i<nsubfr; i++)
    for (j=0; j<nsubfr; j++)
    {
      ogdi_fread(&file->indices[i][j], sizeof(uint), 1, fin);
      swap((ucharp)&file->indices[i][j], sizeof(uint));
    }
  }
  else
  {
    for (i=0; i<nsubfr; i++)
    for (j=0; j<nsubfr; j++)
      file->indices[i][j] = (uint)((i*6L+j)*6144L);
  }

 /* Fseek to LOC_ATTRIBUTE_SECTION_SUBHEADER, ID=141 */

#ifdef LATER
  if (loc[9].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find LOC_ATTRIBUTE_SUBHEADER section in Frame file");
    return(0L);
  }
  fseek(fin, loc[10].phys_index, SEEK_SET);
  if (loc[10].phys_index == ~0)
  {
    ecs_SetError(&(s->result),1,"Can't find LOC_ATTRIBUTE_SECTION_SUBHEADER section in Frame file");
    return(0L);
  }
  fseek(fin, loc[10].phys_index, SEEK_SET);
  ogdi_fread(&num_att_rec, sizeof(num_att_rec), 1, fin) ;
  swap((ucharp)&num_att_rec, sizeof(num_att_rec));
#endif

  fclose(fin);

  return(1L);
}  /* parse_frame */

/***********************************************************************\
*
* Function:      get_comp_luts
*
* Description:
*     
*      Get the compression lookup tables
*
\***********************************************************************/

BOOLEAN
get_comp_lut(s, file, filename, table, cct, ReducedColorTable)
ecs_Server   *s;
Frame_file   *file;
char         *filename;
uchar        *table;
uint         *cct;
int         ReducedColorTable;
{
  FILE *fin;
  uint offset;
  int index;
  int tindex;
  int i,j,k;
  char string[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  
 /* Open the file */

  fin = rpf_fopen_ci( spriv->pathname, filename, "rb" );
  if (fin == NULL)
  {
    sprintf(string,"Can't open frame file %s",filename);
    ecs_SetError(&(s->result),1,string);
    return FALSE;
  }

 /* Loop thru the compression tables */

  for (i=0; i<4; i++)
  {

   /* Seek to the table postion */

    offset = file->loc_lut_shdr + file->lut[i].phys_offset;
    fseek(fin, offset, SEEK_SET);

   /* Read the table */

    ogdi_fread(&table[i*4096L*4L], (size_t)1, (size_t)(4096*4), fin);

   /* Transform the table if reduced color table */

    if (ReducedColorTable)   /* 1 or 2 */
    {
      for (j=0; j<4096; j++)
      for (k=0; k<4; k++)
      {
        index = i*4096L*4L + j*4L + k;
        tindex = (unsigned char)table[index];
        table[index] = (unsigned char)cct[tindex];
      }
    }
  }

  fclose(fin);

  return TRUE;

}

/***********************************************************************\
*
* Function:      get_rpf_image_tile
*
* Description:
*     
*      Get a decompressed RPF image tile
*
\***********************************************************************/

BOOLEAN
get_rpf_image_tile(s, file, filename, tno, table, tile, decompress, blackpixel)
ecs_Server *s;
Frame_file *file;
char       *filename;
int       tno;
uchar      *table;
uchar      *tile;
int       decompress;
uchar      blackpixel;
{
  FILE *fin;
  uchar *ptr;
  uchar *subframe;
  int  i, j, t, e;
  uint  val;
  int  index;
  int  offset;
  char  string[256];
  register ServerPrivateData *spriv = (ServerPrivateData *) s->priv;
  
 /* If blank subframe zero out tile */

  if (tno == -1L && tno == ~0)
  {
    memset((uchar *)tile, (int)(uchar)blackpixel, (size_t)(256*256));
    return TRUE;
  }

 /* Open the file */

  fin = rpf_fopen_ci( spriv->pathname, filename, "rb" );
  if (fin == NULL)
  {
    sprintf(string,"Can't open frame file %s",filename);
    ecs_SetError(&(s->result),1,string);
    return(0L);
  }

 /* Loop thru the compression tables */


 /* Alloc the space for the subframe */

  if ((subframe = (uchar *)malloc((size_t)6144L)) == (uchar *)NULL)
  {
     ecs_SetError(&(s->result),1,"Can't alloc space for subframe");
     return FALSE;
  }

 /* Seek to start of subframe */

  offset = file->loc_data + tno;
  fseek(fin, offset, SEEK_SET);

 /* Read the subframe */

  if( fread(subframe, 1, 6144, fin) != 6144 )
  {
    fclose(fin);
    free(subframe);
    return FALSE;
  }
  fclose(fin);

  ptr = subframe;

 /* Decompress the tile */

  if (decompress)
  {
    for (i = 0; i < 256; i += 4)
      for (j = 0; j < 256; j += 8, ptr += 3)
      {

       /* Get first 12-bit value as index into VQ table */

        val = (uint)((ptr[0] << 4) | (ptr[1] >> 4));
        for (t = 0; t < 4; t++)
        for (e = 0; e < 4; e++)
        {
          index = t*4096L*4L + val*4L + e;
          tile[(i+t)*256L+(j+e   )] = (unsigned char)table[index];
#if 0
          if ( tile[(i+t)*256L+(j+e   )] > 31)
            printf ("i,t,j,e %ld %ld %ld %ld %ld %ld %ld %ld\n",i,t,j,e,
              tile[(i+t)*256L+(j+e   )],index,val,table[index]);
#endif
        }

       /* Get second 12-bit value as index into VQ table */

        val = (uint)(((ptr[1] & 0x0F) << 8) | ptr[2]);
        for (t = 0; t < 4; t++)
        for (e = 0; e < 4; e++)
        {
          index = t*4096L*4L + val*4L + e;
          tile[(i+t)*256L+(j+e+4L)] = (unsigned char)table[index];
#if 0
          if ( tile[(i+t)*256L+(j+e+4L)] > 31)
            printf ("i,t,j,e+4L %ld %ld %ld %ld %ld\n",i,t,j,e+4L,
              tile[(i+t)*256L+(j+e+4L)]);
#endif
        }
      }
  }
  else
  {
    for (i = 0; i < 6144; i++)
      tile[i] = subframe[i];
  }

  free((char *)subframe);

  return TRUE;
}

#ifndef _WINDOWS
void dyn_string_tolower(chaine)
     char *chaine;
{
  int i;
  
  for( i=0; i< (int) strlen(chaine); i++) {
    if (chaine[i] >= 'A' && chaine[i] <= 'Z')
      chaine[i] += 32;
  }
}
#endif

void free_toc(Toc_file *toc)
{
  int i,j,k;

 /* Loop thru the boundaries and free the arrays */

  for (i=0; i<(int)toc->num_boundaries; i++)
  {
    if(toc->entries[i].frames != (Frame_entry **)NULL)
    {																					   	
      for (j=0; j<toc->entries[i].vert_frames; j++)
      {
        if(toc->entries[i].frames[j] != (Frame_entry *)NULL)
        {
          for (k=0; k<toc->entries[i].horiz_frames; k++)		
	    
            if(toc->entries[i].frames[j][k].directory != (char *)NULL)
              free(toc->entries[i].frames[j][k].directory);
	  free((char *)toc->entries[i].frames[j]);
        }
      }
    free((char *)toc->entries[i].frames);
    }
  }
  if(toc->entries != (Toc_entry *)NULL)
    free((char *)toc->entries);

}



