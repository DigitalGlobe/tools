
#ifndef __VPFTIDX_H__
#define __VPFTIDX_H__ 1

#include <stdio.h>
#ifndef __SET_H__
#include "set.h"
#endif
 
typedef struct {                        /* Total of 40 bytes */
  int32      nbytes ,                /* 40 + directory length */
                nbins ,                 /* Directory size */
                table_nrows ;           /* Num rows in original table */
  char          index_type ,            /* T = thematic, G = gazetteer */
                column_type ;           /* T, I, R, S, F, K */
  int32      type_count ;            /* usually 1, but more for T */
  char          id_data_type ,          /* I if > 32767, else S */
		vpf_table_name[13] ,
		vpf_column_name[25] ,   /* 16 bytes + 9 for TILE_ID */
		sort ,                  /* 'S' if directory sorted */
		padding[3] ;            /* To make it a nice 60 bytes */
} ThematicIndexHeader ;
  
#define         THEMATIC_INDEX_HEADER_SIZE    60
 
#define         DIRECTORY_SIZE                (sizeof(int32)*2)
 
#define         MAX_ID          32767   /* Threshold between S and I ids */ 

typedef union
   {
   char    cval , *strval;
   int32    ival;
   short   sval;
   float   fval;
   double  dval;
   } ThematicIndexValue;

typedef struct {                        /* length = sizeof (datatype) + */
  ThematicIndexValue value;             /*          8 * nbins           */
  int32      binid,
                start_offset ,
                num_items ;             /* For each value, count the ids */  
} ThematicIndexDirectory ;
 
typedef struct {
   ThematicIndexHeader h;
   ThematicIndexDirectory *d, *gid;
   FILE *fp;
} ThematicIndex;
 
/* Prototype Definitions */
#ifdef PROTO
  int32 create_thematic_index (char, char*, char*, char*, char*);
   set_type read_thematic_index (char*, char*);
   ThematicIndex open_thematic_index (char*, int32);
   set_type search_thematic_index (ThematicIndex*, char*);
   void close_thematic_index (ThematicIndex*);
   int32 create_gazetteer_index (char*, char*, char*, char*);
   set_type search_gazetteer_index (ThematicIndex*, char*);
   set_type read_gazetteer_index (char*, char*);
#else
   int32 create_thematic_index ();
   set_type read_thematic_index ();
   ThematicIndex open_thematic_index ();
   set_type search_thematic_index ();
   void close_thematic_index ();
   int32 create_gazetteer_index ();
   set_type search_gazetteer_index ();
   set_type read_gazetteer_index ();
#endif
#endif
