
/*****************************************************************************/
/* VPFSPX.C                                                                  */
/*                                                                           */
/*   Purpose:                                                                */
/*      This module contains functions for reading the VPF spatial index     */
/*      file.  A VPF spatial index is a modified quad-tree index with a      */
/*      cross-over bin for features spanning more than one spatial cell.     */
/*      There is one interface function in this module -                     */
/*      spatial_index_search().  The spatial index creation function is      */
/*      not included.                                                        */
/*****************************************************************************/
#ifndef INCL_XVTH
#include "xvt.h"
#endif

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#ifdef _UNIX
#define O_BINARY 0
#include <unistd.h>
#include <memory.h>
#include <malloc.h>
#endif
#ifdef _MSDOS
#include <mem.h>
#include <conio.h>
#include <dos.h>
#include <io.h>
#include <alloc.h>
#endif
#ifdef _UNIX
#include <sys/stat.h>
#endif
#include <stdlib.h>
#ifndef H_MUSEDIR
#include "musedir.h"
#endif
#ifndef __VPFSPX_H__
#include "vpfspx.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#endif

/******* cellmap definitions **/
#define  START     0
#define  NUMFEAT   1

#define OUT        0
#define IN         1

#define ID         1
#define BOUND      0

#define TOTAL_NUMBER 0
#define BOUND_START  1
#define CELLMAP_SIZE 5

#define XMIN 0
#define YMIN 1
#define XMAX 2
#define YMAX 3

typedef struct
   {
   int32 start, numfeat;
   } cellmap_rec_type;

typedef struct
   {
   int32 bound, id;
   } spx_rec_type;

typedef struct
   {
   FILE             *fp;       /* File pointer */
   int32             maplen;    /* Number of cells in the tree */
   cellmap_rec_type *cellmap;  /* The tree of cells */
   int32             shift;     /* Size of data before real cell records */
   unsigned char    box[4];    /* Search box */
   } spx_type;


/*************************************************************************
 *
 *N  is_over
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Determine if two boxes overlap.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    box1    <input>==(unsigned char[4]) first box to check.
 *    box2    <input>==(unsigned char[4]) second box to check.
 *    return <output>==(int) 1 if the boxes overlap,
 *                             0 if the boxes do not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
#ifdef PROTO
static int is_over( unsigned char box1[4], unsigned char box2[4] )
#else
static int is_over( box1, box2 )
unsigned char box1[4];
unsigned char box2[4];
#endif
{
   int xmin,xmax,ymin,ymax;

   if(box2[XMIN] >= box1[XMIN] && box2[XMIN] <= box1[XMAX])
      xmin = IN;
   else
      xmin = OUT;
   if(box2[XMAX] >= box1[XMIN] && box2[XMAX] <= box1[XMAX])
      xmax = IN;
   else
      xmax = OUT;
   if(box2[YMIN] >= box1[YMIN] && box2[YMIN] <= box1[YMAX])
      ymin = IN;
   else
      ymin = OUT;
   if(box2[YMAX] >= box1[YMIN] && box2[YMAX] <= box1[YMAX])
      ymax = IN;
   else
      ymax = OUT;
   /* complete overlap */
   if(xmin == OUT && xmax == OUT && box2[XMIN] <= box1[XMIN] &&
      box2[XMAX] >= box1[XMAX])
      xmin = IN;
   if(ymin == OUT && ymax == OUT && box2[YMIN] <= box1[YMIN] &&
      box2[YMAX] >= box1[YMAX])
      ymin = IN;

   if((xmin == IN || xmax == IN) && (ymin == IN || ymax == IN)) return 1;

   return 0;
}


/*************************************************************************
 *
 *N  get_record
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Read spatial index record 'num'.  Insert the record into the search
 *    set if it falls within the search box.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    num  <input>==(int32) record number.
 *    spx  <input>==(spx_type *) spatial index record structure.
 *    set <output>==(set_type) search set to be updated.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
#ifdef PROTO
static void get_record(int32 num, spx_type *spx, set_type set )
#else
static void get_record (num, spx, set )
int32 num;
spx_type *spx;
set_type set;
#endif
{
   int32 offset;
   int32 i,j,index;      /* DGM */
   unsigned char box[4];
   int32 ltmp;
   spx_rec_type *rec;
   int32 machine_byte_order = MACHINE_BYTE_ORDER; /* DGM */

   index  = num -1;
   offset = spx->cellmap[index].start + spx->shift;
   fseek(spx->fp,offset,SEEK_SET);
   rec = (spx_rec_type *)xvt_zmalloc(spx->cellmap[index].numfeat*
                                sizeof(spx_rec_type));
   ogdi_fread(rec,sizeof(spx_rec_type),spx->cellmap[index].numfeat,spx->fp);

   if(machine_byte_order != LEAST_SIGNIFICANT)    /* DGM */
     for(i=0;i<spx->cellmap[index].numfeat;i++){ 
        /* spx->buf[i][0] is a four byte buffer - don't swap */
        /* spx->buf[i][ID] is an integer - swap */
        ltmp = rec[i].id;
        swap_four((char*)&ltmp,(char*)&rec[i].id);
     }

   for(j=0;j<spx->cellmap[index].numfeat;j++) {
      memcpy(&box,&(rec[j].bound),4*sizeof(unsigned char));
      if (is_over(box,spx->box)) {
	 set_insert(rec[j].id,set);
      }
   }

   xvt_free((char*)rec);
}


/*************************************************************************
 *
 *N  search_cell
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Search the current cell for local records and, if the box is in the
 *    search area, search the cell's children.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    record <input>==(int32) record number.
 *    level  <input>==(int) level of the index tree.
 *    bnd    <input>==(unsigned char[4]) bounding box.
 *    spx    <input>==(spx_type *) spatial index record structure.
 *    set   <output>==(set_type) search set to be updated.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
#ifdef PROTO
static void search_cell( int32 record, int level, unsigned char bnd[4],
			 spx_type *spx, set_type set )
#else
static void search_cell( record, level, bnd, spx, set )
int32 record;
int level;
unsigned char bnd[4];
spx_type *spx;
set_type set;
#endif
{
   int i;
   unsigned char locbnd[4],cut;

  if (record > spx->maplen) return;

  for(i=0;i<4;i++) locbnd[i] = bnd[i];
  if(level != 0)
    { /* level even -> cut on x , level odd -> cut on y */
      if(level%2 == 1) cut = bnd[0] + ((bnd[2] - bnd[0]) >> 1);
      else             cut = bnd[1] + ((bnd[3] - bnd[1]) >> 1);
      /* cut on x right cell */
      if(level%2 == 1 && record%2 == 0) locbnd[0] = cut;
      /* cut on x left cell */
      if(level%2 == 1 && record%2 == 1) locbnd[2] = cut;
      /* cut on y upper cell */
      if(level%2 == 0 && record%2 == 0) locbnd[1] = cut;
      /* cut on y lower cell */
      if(level%2 == 0 && record%2 == 1) locbnd[3] = cut;
     }
  if(is_over(locbnd,spx->box) == 1)
    { get_record(record,spx,set);
      if(record*2 <= spx->maplen)
       { search_cell(record*2,  level+1,locbnd, spx, set);
	 search_cell(record*2+1,level+1,locbnd, spx, set);
       }
    }
}


/*************************************************************************
 *
 *N  spatial_index_search
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Search the named spatial index file to find all records falling
 *    within the specified extent.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    fname <input>==(char *) path name to a VPF spatial index file.
 *    x1    <input>==(float) left coordinate of the search box.
 *    y1    <input>==(float) bottom coordinate of the search box.
 *    x2    <input>==(float) right coordinate of the search box.
 *    y2    <input>==(float) top coordinate of the search box.
 *    set  <output>==(set_type) search set to be filled.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Mody Buchbinder  Jul 1991
 *E
 *************************************************************************/
#ifdef PROTO
set_type spatial_index_search( char *fname,
			       float x1, float y1, float x2, float y2 )
#else
set_type spatial_index_search( fname,  x1, y1, x2, y2 )
char *fname;
float x1;
float y1;
float x2;
float y2;
#endif
{
   int32 head[6];
   int32 htmp[6];
   int32 ltmp;
   float bnd[4],xf,yf;
   unsigned char tempbox[4];
   set_type set;
   spx_type spx;
   int32 i;
   int32 machine_byte_order = MACHINE_BYTE_ORDER;

   spx.fp = muse_file_open (fname, "rb");
   if (!spx.fp)
      {
      set.size = 0;
      set.buf = NULL;
      return set;
      }

/*   read basic header - int  0 -> number of features in coverage */
/*                       int  1 - 4 -> boundaries of coverage     */
/*                       int  5 -> size of tree in records - each */
/*                                 record = 2 integers (8 byte)   */
   ogdi_fread (htmp, sizeof (int32), 6, spx.fp);

   if (machine_byte_order != LEAST_SIGNIFICANT) 
      for (i=0; i<6; i++)
         swap_four ((char*)&htmp[i], (char*)&head[i]);
      else
         memcpy (head, htmp, 6*sizeof (int32));

   spx.shift = 6*sizeof (int32);
   spx.maplen = head[CELLMAP_SIZE];

#if 0 /* DGM */
   set = set_init(head[TOTAL_NUMBER]+1L);
#endif
   set = set_init (head[TOTAL_NUMBER]);

   memcpy (&bnd[0], &head[BOUND_START], 4*sizeof (float));

   /* If the boundaries of the cellmap are completely within the */
   /* search area, all features are on */
   if ( (x1 <= bnd[XMIN]) && (x2 >= bnd[XMAX]) &&
	(y1 <= bnd[YMIN]) && (y2 >= bnd[YMAX]) ) {
      set_on(set);
      fclose(spx.fp);
      return set;
   }

/* allocate cells */
   spx.cellmap = (cellmap_rec_type *)xvt_zmalloc(spx.maplen*
                                     sizeof(cellmap_rec_type));
   if (!spx.cellmap) {
      /* Memory allocation failed - return null set */
      fclose(spx.fp);
      set_nuke(&set);
      set.size=0;
      return set;
   }

/* read the cell tree */
   ogdi_fread(spx.cellmap,sizeof(cellmap_rec_type),spx.maplen,spx.fp);

   if(machine_byte_order != LEAST_SIGNIFICANT)  /* DGM */
     for(i=0;i<spx.maplen;i++){
       ltmp = spx.cellmap[i].start;
       swap_four((char*)&ltmp,(char *)&spx.cellmap[i].start);
       ltmp = spx.cellmap[i].numfeat;
       swap_four((char*)&ltmp,(char *)&spx.cellmap[i].numfeat);
     }

   spx.shift += 2*spx.maplen*sizeof(int32);

/* translate search box to 1 byte numbers */
   xf = 255.0 / (bnd[XMAX] - bnd[XMIN]);
   yf = 255.0 / (bnd[YMAX] - bnd[YMIN]);
   spx.box[XMIN] = (char)((x1 - bnd[XMIN]) * xf);
   if (x1 <= bnd[XMIN]) spx.box[XMIN] = 0;
   if (x1 >= bnd[XMAX]) spx.box[XMIN] = 255;
   spx.box[YMIN] = (char)((y1 - bnd[YMIN]) * yf);
   if (y1 <= bnd[YMIN]) spx.box[YMIN] = 0;
   if (y1 >= bnd[YMAX]) spx.box[YMIN] = 255;
   spx.box[XMAX] = (char)(((x2 - bnd[XMIN]) * xf) + 1);
   if (x2 <= bnd[XMIN]) spx.box[XMAX] = 0;
   if (x2 >= bnd[XMAX]) spx.box[XMAX] = 255;
   spx.box[YMAX] = (char)(((y2 - bnd[YMIN]) * yf) + 1);
   if (y2 <= bnd[YMIN]) spx.box[YMAX] = 0;
   if (y2 >= bnd[YMAX]) spx.box[YMAX] = 255;

/** start search **/
   tempbox[XMIN] = 0;
   tempbox[YMIN] = 0;
   tempbox[XMAX] = 0xff;
   tempbox[YMAX] = 0xff;
   search_cell(1,0,tempbox,&spx,set);

   fclose(spx.fp);

   xvt_free((char*)spx.cellmap);

   return set;
}
