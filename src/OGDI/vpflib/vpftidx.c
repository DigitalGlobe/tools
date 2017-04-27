
/*************************************************************************
 *
 * Environmental Systems Research Institute (ESRI) Applications Programming
 *
 *N  Module VPFTIDX
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *P
 *   Purpose: Subroutines to create and read both the thematic index
 *	      or a gazeteer index on a VPF table column.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       September/October 1991
 *    Barry Michaels   January 1992   A few adjustments for DOS.
 *                                    Added sets to create function to
 *                                    take advantage of SET's disk-
 *				      swapping ability when low on memory.
 *                     Dec 1992       Added sorted directory per RFC#67
 *                                    to VPF.  Fixed text, added dates,
 *                                    rearranged create to use a scratch
 *                                    file instead of sets.
 *E
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions:
 *F
 *
 *    set_type read_thematic_index ( char *tablename,
 *				     char *value ) ;
 *
 *    ThematicIndex open_thematic_index ( char *tablename );
 *
 *    set_type search_thematic_index ( ThematicIndex *themindex,
 *				       char *value ) ;
 *
 *    void close_thematic_index ( ThematicIndex *themindex );
 *
 *    set_type read_gazetteer_index (char * vpfname, char * query_str ) ;
 *
 *    set_type search_gazetteer_index (ThematicIndex *idx,
 *				       char * query_str ) ;
 *
 *    int32 read_gazetteer_index_directory (
 *					ThematicIndexDirectory ** gid,
 *					ThematicIndexHeader     * gi,
 *					FILE * idx_fp);
 *
 *    int32 write_thematic_index_header ( ThematicIndexHeader h ,
 *					     FILE *fp ) ;
 *
 *    int32 read_thematic_index_header ( ThematicIndexHeader *h ,
 *					    FILE *fp ) ;
 *
 *    int32 write_thematic_index_directory (
 *				       ThematicIndexHeader h ,
 *				       ThematicIndexDirectory *d,
 *				       int32 size,
 *				       FILE *fp ) ;
 *
 *    int32 write_gazetteer_index_directory (
 *				       ThematicIndexHeader h ,
 *				       ThematicIndexDirectory *d,
 *				       int32 size,
 *				       FILE *fp ) ;
 *
 *    int32 create_thematic_index ( char indextype,
 *				       char *tablename,
 *				       char *idxname ,
 *				       char *columnname,
 *				       char *idx_set );
 *
 *    int32 create_gazetteer_index (char *tablename,
 *				       char *idx_fname ,
 *				       char *columnname,
 *				       char *idx_set);
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible. Created on UNIX first
 *E
 *************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif

#if 0
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#endif

#ifndef __VPF_H__
#include "vpf.h"
#endif
#ifndef _MACHINE_
#include "machine.h"
#endif
#ifndef H_MUSEDIR
#include "musedir.h"
#endif
#ifndef __STRFUNC_H__
#include "strfunc.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif
#ifndef __VPFTIDX_H__
#include "vpftidx.h"
#endif

#ifndef __USE_BSD
#define bcopy(src,dest,size)  memcpy(dest,src,size)
#endif

#ifdef PROTO
int32 read_gazetteer_index_directory(
                                   ThematicIndexDirectory **gid,
                                   ThematicIndexHeader     *gi,
                                   FILE                    *idx_fp);

int32 read_thematic_index_header ( ThematicIndexHeader *h,
                                             FILE *ifp );

int32 write_thematic_index_header ( ThematicIndexHeader h,
                                              FILE *ifp );

int32 write_thematic_index_directory ( ThematicIndexHeader h,
                                          ThematicIndexDirectory *d,
                                          int32 idsize, /* size of data */
                                          FILE *ifp );

int32 write_gazetteer_index_directory ( ThematicIndexHeader h,
                                          ThematicIndexDirectory *d,
                                          int32 idsize, /* size of data */
                                          FILE *ifp );
#else
int32 read_gazetteer_index_directory();

int32 read_thematic_index_header ( );

int32 write_thematic_index_header ( );

int32 write_thematic_index_directory ( );

int32 write_gazetteer_index_directory ( );
#endif

#define Whimper(str) { \
      xvt_note ("\nvpftidx: < %s >\n", str );}

#define SWhimper(str) { \
      set_type err; err = set_init (1) ;\
      xvt_note ("\nvpftidx: < %s >\n", str ) ;\
      return err ; }

#define OWhimper(str) { \
      xvt_note ("\nvpftidx: < %s >\n", str ) ;\
      return idx ; }

/*
 * an index file directory entry takes up a different amount
 * of space in memory than it does on disk; the following constant
 * should be used to calculate the starting offset for
 * index data
 *
 * example: if the number of items to be indexed is t, and each item is
 *          s bytes long, then use
 *
 *          sizeof(ThematicIndexHeader) + t * (s + DIR_ON_DISK_INCR)
 *
 * to find out where the first byte of indexing data will be placed.
 *
 * That first byte is what must be stored as the value of Header.nbytes
 */

#define DIR_ON_DISK_INCR (2 * sizeof(int32))

#ifdef PROTO
int Icmpval (const void *elem1,  const void *elem2 )
#else
int Icmpval (elem1,  elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.value.ival < d2.value.ival) return -1;
   if (d1.value.ival > d2.value.ival) return 1;
   return 0;
}


#ifdef PROTO
int Scmpval( const void *elem1,
		    const void *elem2 )
#else
int Scmpval( elem1, elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.value.sval < d2.value.sval) return -1;
   if (d1.value.sval > d2.value.sval) return 1;
   return 0;
}


#ifdef PROTO
int Fcmpval( const void *elem1,
		    const void *elem2 )
#else
int Fcmpval( elem1, elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.value.fval < d2.value.fval) return -1;
   if (d1.value.fval > d2.value.fval) return 1;
   return 0;
}


#ifdef PROTO
int Dcmpval( const void *elem1,
		    const void *elem2 )
#else
int Dcmpval( elem1, elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.value.dval < d2.value.dval) return -1;
   if (d1.value.dval > d2.value.dval) return 1;
   return 0;
}


#ifdef PROTO
int Ccmpval( const void *elem1,
		    const void *elem2 )
#else
int Ccmpval( elem1, elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.value.cval < d2.value.cval) return -1;
   if (d1.value.cval > d2.value.cval) return 1;
   return 0;
}


#ifdef PROTO
int STRcmpval( const void *elem1,
		      const void *elem2 )
#else
int STRcmpval( elem1, elem2 )
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   return (strcmp(d1.value.strval,d2.value.strval));
}


#ifdef PROTO
int bincmp (const void *elem1, const void *elem2)
#else
int bincmp (elem1, elem2)
const void *elem1;
const void *elem2;
#endif
{
   ThematicIndexDirectory d1,d2;

   memcpy(&d1,elem1,sizeof(ThematicIndexDirectory));
   memcpy(&d2,elem2,sizeof(ThematicIndexDirectory));

   if (d1.binid < d2.binid) return -1;
   if (d1.binid > d2.binid) return 1;
   return 0;
}

#define KEY_ID 1
#define KEY_TILE 2
#define KEY_EXT 3

/*************************************************************************
 *
 *N  create_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *
 *      Create a thematic index file on a given vpf table.
 *P
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    indextype  <input> == (char) either 'T' for thematic index or
 *					  'G' for gazetteer.
 *    tablename  <input> == (char *) path name of the vpf table.
 *    idxname    <input> == (char *) name of the index file to be created.
 *    columnname <input> == (char *) name of column to create index on
 *			    if the column is a triplet id, the columnname
 *			    should be as such: EDG_ID\ID
 *					       EDG_ID\TILE_ID
 *					       EDG_ID\EXT_ID
 *    idxset     <input> == (char *) array of character values to index on
 *                          if the index type is gazetteer.  Not used for
 *                          index type = 'T'.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October	 1991 
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *    void *vpfmalloc                                    VPFMISC.C
 *    vpf_open_table					 VPFTABLE.C
 *    table_pos
 *    table_element
 *    vpf_close_table
 *    write_thematic_index_header			see below
 *    write_thematic_index_directory			see below
 *    Vpf_Write_* macros
 *    set_init						SET.C
 *    set_insert
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.  developed on UNIX
 *E
 *************************************************************************/
#ifdef PROTO
int32 create_thematic_index ( char indextype,
		 	         char *tablename,
				 char *idxname ,
				 char *columnname,
				 char *idx_set ) 
#else
int32 create_thematic_index ( indextype, tablename, idxname , columnname, idx_set ) 
char indextype;
char *tablename;
char *idxname;
char *columnname;
char *idx_set;
#endif
{

  FILE           *ifp, *tmpfp ;
  int32 i, j, tablepos, k,
       keycolumn = 0 ,	      /* 1 if key column */
       itemp , n,
       idsize , 	      /* either 2 or 4 bytes */
       totalids = 0,
       bin,
       binoff,
       datasize = 0;	      /* Directory value data size */
  char           *buf ,
		 ch ,
		 hack[255] ;
  vpf_table_type table ;
  ThematicIndexHeader		h ;
  ThematicIndexDirectory        *d;
  short int			stemp ;
  float				ftemp ;
  double			dtemp ;
  date_type                     datetemp ;
  id_triplet_type		key ;

  if (!tablename) Whimper("Unspecified table name");
  if (!idxname) Whimper("Unspecified index name");
  if (!columnname) Whimper("Unspecified column name");

  if ( indextype == 'G' ) {
     if (!idx_set) Whimper("Unspecified index set");
    return ( create_gazetteer_index ( tablename, idxname,columnname,idx_set)) ;
  }

  /* convert columnname to uppercase and check for id triplet */

  for ( i=0 ; i < (int32)strlen(columnname); i++ )   /* copy into header structure */
    h.vpf_column_name[i] = (char)toupper ( columnname[i] ) ;
  h.vpf_column_name[i] = '\0';

  h.index_type = 'T' ;		/* default */
  h.type_count = 1 ;

  for ( i=0 ; i < (int32)strlen(columnname); i++ ) {

    columnname[i] = (char)toupper ( columnname[i] ) ;

    /* check if request is on a triplet id field */

    if ( columnname[i] == '\\' ) {

      columnname[i+1] = (char)toupper ( columnname[i+1] ) ;
      
      switch ( columnname[i+1] ) {	/* locate sub-column key */
      case 'I':	/* ID of triplet id */
	keycolumn = KEY_ID ; break ;
      case 'T':	/* TILE_ID of triplet id */
	keycolumn = KEY_TILE ; break ;
      case 'E':	/* EXT_ID of triplet id */
	keycolumn = KEY_EXT ; break ;
      default:
	Whimper ("error in key column request") ;
	/* break ; Whimper contains a return */
      }	/* end of switch */
      strcpy( h.vpf_column_name, strupr ( columnname )) ;
      columnname[i] = '\0' ;
    }     /* end of if columnname == \ */
  }	/* end of for loop */

  table = vpf_open_table ( tablename, disk, "rb", '\0') ;
  if (!table.fp) {
     sprintf(hack,"Error opening %s\n",tablename);
     Whimper(hack);
  }


  /* Determine the id_data_size. This will save 50% of file size  */

  if ( table.nrows > MAX_ID ) {
    h.id_data_type = 'I' ;		/* int32s */
    idsize = sizeof ( int32 ) ;
  } else {
    h.id_data_type = 'S' ;		/* short ints */
    idsize = sizeof ( short int ) ;
  }

  /* Find position of column in input table */

  tablepos = table_pos ( columnname, table ) ;
  if (tablepos < 0) {
    vpf_close_table(&table);
    sprintf(hack,"Invalid column name (%s) for %s",columnname,tablename);
    Whimper(hack);
  }

  /* Now check data type */

  switch (table.header[tablepos].type) {
  case 'X':
    vpf_close_table(&table);
    Whimper ("Cannot make index on Null Column") ;
    /* A return is in Whimper - break statement cannot be reached */
  case 'T':
    if ( table.header[tablepos].count <= 0 ) {
      vpf_close_table(&table);
      Whimper ("Cannot make bin index on variable length text column") ;
    }
    datasize = table.header[tablepos].count ;
    h.type_count = datasize ;		/* the special case */
    break;
  case 'D': 
    datasize = sizeof ( date_type ) ;
    break;
  case 'I':
  case 'K':				/* treat keys like integers */
    datasize = sizeof ( int32 ) ;
    break ;
  case 'S':
    datasize = sizeof ( short int ) ;
    break ;
  case 'F':
    datasize = sizeof ( float ) ;
    break ;
  case 'R':
    datasize = sizeof ( double ) ;
    break ;
  case 'C':
  case 'Z':
  case 'B':
  case 'Y':
    vpf_close_table(&table);
    Whimper ("Cannot make index on Coordinate Column") ;
  default:
    vpf_close_table(&table);
    sprintf (hack,"No such type < %c >", table.header[tablepos].type ) ;
    Whimper ( hack ) ;
  }  /* end of switch */

  /* open output index file */

  if (( ifp = muse_file_open ( idxname, "w+b" )) == NULL ) {
    vpf_close_table(&table);
    Whimper ( idxname ) ;
  }

  /* open scratch file of (bin,binpos) for each table id */
  if (( tmpfp = tmpfile() ) == NULL ) {
    vpf_close_table(&table);
    fclose(ifp);
    Whimper ( "Error creating scratch file" ) ;
  }

  /* Create directory size as big as table, for worst case */

  d = (ThematicIndexDirectory *)
    xvt_malloc (sizeof (ThematicIndexDirectory));
  h.nbins = 0 ;		/* initialize the directory counter */

#define REALLOC_DIRECTORY(loc) { \
  if ((d = (ThematicIndexDirectory *) \
       realloc(d,sizeof(ThematicIndexDirectory) * (loc+1))) == NULL ) {\
    vpf_close_table(&table); \
    fclose(ifp); \
    fclose(tmpfp); \
    Whimper ("error in reallocing directory") ; \
  } \
  d[loc].binid = loc; \
  d[loc].num_items = 0 ; }

#define INCR_ID(loc,id) { \
  UNUSED_RET(fwrite(&loc,sizeof(int32),1,tmpfp)); \
  j = d[loc].num_items; \
  UNUSED_RET(fwrite(&j,sizeof(int32),1,tmpfp)); \
  d[loc].num_items++ ; \
  if (d[loc].num_items == 1) \
    d[loc].start_offset = id; \
  else if (d[loc].num_items == 2) \
    totalids += 2; \
  else \
    totalids++; }

/* Count up bins */

  switch ( table.header[tablepos].type ) {
  case 'I':

    for ( i=1; i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&itemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.ival == itemp )
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.ival = itemp ;
	h.nbins++ ;
      }
      INCR_ID(k,i) ;
    }   /* end of for i loop */

    break ;
  case 'T':

    for ( i = 1; i <= table.nrows; i++ ) {

      if (h.type_count > 1) {

	 /* read in record from disk */
	 buf = (char *) table_element (tablepos,i,table,NULL,&n);

	 /* strlen ( buf ) should equal datasize */

	 for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	   if ( ! strcmp ( d[k].value.strval, buf ) )
	     break ;/* Found a match */

	 if ( k == h.nbins ) { 		/* New value in column */
	   REALLOC_DIRECTORY ((size_t)k);
	   d[k].value.strval = (char*)xvt_malloc (strlen (buf));
	   (void) bcopy ( buf, d[k].value.strval, strlen(buf) ) ;
           d[k].value.strval[n] = '\0';
	   h.nbins++ ;
	 }
	 INCR_ID(k,i) ;
	 xvt_free(buf);

      } else {

	 /* read in record from disk */
	 table_element (tablepos,i,table,&ch,&n);

	 for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	   if ( d[k].value.cval == ch )
	     break ;/* Found a match */

	 if ( k == h.nbins ) { 		/* New value in column */
	   REALLOC_DIRECTORY ((size_t)k);
	   d[k].value.cval = ch;
	   h.nbins++ ;
	 }
	 INCR_ID(k,i) ;

      }
    }   /* end of for i loop */

    break ;
  case 'S':

    for ( i=1; i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&stemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.sval == stemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.sval = stemp ;
	h.nbins++ ;
      }
      INCR_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'F':

    for ( i=1; i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&ftemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.fval == ftemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.fval = ftemp ;
	h.nbins++ ;
      }
      INCR_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'R':
    for ( i=1; i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&dtemp,&n);
      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.dval == dtemp )
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.dval = dtemp ;
	h.nbins++ ;
      }
      INCR_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'K':

    for ( i=1; i <= table.nrows; i++ ) {
      /* read in record from disk */
      table_element ( tablepos,i,table,&key,&n);

      if ( keycolumn == KEY_ID ) 
	itemp = key.id ;
      else if ( keycolumn == KEY_TILE ) 
	itemp = key.tile ;
      else 
	itemp = key.exid ;

      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( d[k].value.ival == itemp ) 
	  break ;/* Found a match */
      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.ival = itemp ;
	h.nbins++ ;
      }
      INCR_ID (k,i) ;
    }   /* end of for i loop */

    break ;
  case 'D':

    for ( i = 1; i <= table.nrows; i++ ) {

      /* read in record from disk */
      table_element (tablepos,i,table,datetemp,&n);
      datetemp[sizeof(date_type)-1]='\0';

      /* strlen ( datetemp ) should equal datasize */

      for ( k=0; k < h.nbins; k++ )	/* Search directory for value*/
	if ( ! strcmp ( d[k].value.strval, datetemp ) )
	   break ;/* Found a match */

      if ( k == h.nbins ) { 		/* New value in column */
	REALLOC_DIRECTORY ((size_t)k);
	d[k].value.strval = (char*)xvt_malloc ( sizeof (date_type) + 1);
	(void) bcopy ( datetemp, d[k].value.strval, sizeof(date_type) ) ;
	d[k].value.strval[sizeof(date_type)-1] = '\0';
	h.nbins++ ;
      }
      INCR_ID(k,i) ;

    }   /* end of for i loop */

    break ;
  default:
    vpf_close_table(&table);
    fclose(ifp);
    fclose(tmpfp);
    if (h.column_type == 'T' && h.type_count > 1) {
       for ( i=0; i < h.nbins; i++ ) xvt_free(d[i].value.strval);
    }
    xvt_free ((char*)d);
    Whimper ("error in table reading switch, no such type") ;
  } /* end of switch */

  /* set up header and write it out */
  /* h.id_data_size and columnname set up above */
  /* h.type_count set above */

  h.nbytes = THEMATIC_INDEX_HEADER_SIZE +
             h.nbins * ( datasize + DIRECTORY_SIZE ) ;

  h.column_type = table.header[tablepos].type ;
  if ( h.column_type == 'K' )		/* It's really an I */
    h.column_type = 'I' ;
  h.sort = 'S';
  for ( i=0; i < 3; i++ )
    h.padding[i] = '\0';

  /* only write the table name, no pathname */

  for ( i = strlen ( tablename ); i > 0; i-- )
    if ( tablename[i] == DIR_SEPARATOR ) break ;
  if ( tablename[i] == DIR_SEPARATOR )
#ifdef _MSDOS
    strncpy ( h.vpf_table_name, strupr ( &tablename[i+1] ), 13 ) ;
#else
    strncpy ( h.vpf_table_name, &tablename[i+1], 13 ) ;
#endif
  else
#ifdef _MSDOS
    strncpy( h.vpf_table_name, strupr ( tablename), 13 );
#else
    strncpy( h.vpf_table_name, tablename, 13 );
#endif
  
  for ( i=strlen(h.vpf_table_name); i < 12 ; i++ )
    h.vpf_table_name[i] = ' ' ;
  h.vpf_table_name[12] = '\0';

  /* columnname setup above */

  h.table_nrows = table.nrows ;

  if (write_thematic_index_header (h, ifp) == 0L) {
    vpf_close_table(&table);
    fclose(ifp);
    fclose(tmpfp);
    if (h.column_type == 'T' && h.type_count > 1) {
       for ( i=0; i < h.nbins; i++ ) xvt_free(d[i].value.strval);
    }
    xvt_free ((char*)d);
    Whimper ( "error writing index header" ) ;
  }

  /* Now write out the rest of the header directory */

  /*   ... First, sort the directory ...   */
  switch (h.column_type) {
     case 'I':
	qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Icmpval);
	break;
     case 'S':
	qsort((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Scmpval);
	break;
     case 'F':
	qsort((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Fcmpval);
	break;
     case 'R':
	qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Dcmpval);
	break;
     case 'T':
	if (h.type_count == 1)
	   qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Ccmpval);
	else
	   qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								   STRcmpval);
	break;
     case 'D':
	qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								   STRcmpval);
	break;
     case 'K':
	qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory),
								     Icmpval);
	break;
  }

  if (write_thematic_index_directory (h, d, idsize, ifp) == 0L) {
    vpf_close_table(&table);
    fclose(ifp);
    fclose(tmpfp);
    if (h.column_type == 'T' && h.type_count > 1) {
       for ( i=0; i < h.nbins; i++ ) xvt_free(d[i].value.strval);
    }
    xvt_free ((char*)d);
    Whimper ( "error writing index directory" ) ;
  }

  /* now write the data */

  /* First, fill up all id slots with 0 placeholders */
  itemp = 0;
  stemp = 0;
  if ( h.id_data_type == 'I' )
     for (i=0;i<totalids;i++) Write_Vpf_Int( &itemp, ifp, 1 );
  else
     for (i=0;i<totalids;i++) Write_Vpf_Short( &stemp, ifp, 1 );

  rewind(tmpfp);

  /* Restore original bin order */
  qsort ((void*)d, (size_t)h.nbins, sizeof(ThematicIndexDirectory), bincmp);

  for ( j=1; j <= table.nrows; j++ ) {
     UNUSED_RET(fread(&bin,sizeof(bin),1,tmpfp));
     UNUSED_RET(fread(&binoff,sizeof(binoff),1,tmpfp));
     if (d[bin].num_items == 1) continue;
     if ( h.id_data_type == 'I' ) {
	fseek(ifp, d[bin].start_offset + (binoff*sizeof(int32)), 0);
	itemp = (int32)j;
	Write_Vpf_Int ( &itemp, ifp, 1 ) ;
     } else {
	fseek(ifp, d[bin].start_offset + (binoff*sizeof(short int)), 0);
	stemp = (short int)j;
	Write_Vpf_Short ( &stemp, ifp, 1 ) ;
     }
  }

  /* close up shop and exit */

  vpf_close_table ( &table );
  fclose ( tmpfp );
  fclose ( ifp ) ;
  if (h.column_type == 'T' && h.type_count > 1) {
     for ( i=0; i < h.nbins; i++ ) xvt_free(d[i].value.strval);
  }
  xvt_free ((char*)d);
  return ( h.nbins ) ;

}

#ifdef PROTO
ThematicIndexDirectory tidx_linear_search( char *value,
						  ThematicIndexHeader h,
						  FILE *ifp )
#else
ThematicIndexDirectory tidx_linear_search( value,  h, ifp )
char *value;
ThematicIndexHeader h;
FILE *ifp;
#endif
{
  int32 i , ival = 0;
  short sval = 0 , Match = (-1) ;
  float fval = (float)0.0;
  double dval = 0.0 , atof () ;
  date_type dateval;
  char *buf = (char *) NULL, bufalloc=0 ;
  ThematicIndexDirectory d;

  d.value.ival = 0;
  d.start_offset = 0;
  d.num_items = 0;

  if ( value ) 		/* search for pattern */
    switch ( h.column_type ) {			/* using address */
    case	'I':
      memcpy ( &ival, value, sizeof (int32)) ;
      break ;
    case	'S':
      memcpy ( &sval, value, sizeof (short int)) ;
      break ;
    case	'F':
      memcpy ( &fval, value, sizeof (float)) ;
      break ;
    case	'R':
      memcpy ( &dval, value, sizeof (double)) ;
      break ;
    case	'T':
      buf = (char*)xvt_malloc ((size_t)h.type_count);
      /* leave value as is */
      bufalloc = 1;
      break ;
    case 	'D':
      /* leave value as is */
      break;
    }
  else return d;

  for ( i=0; i < h.nbins; i++ ) {

    switch ( h.column_type ) {
    case 	'I':
      Read_Vpf_Int ( &d.value.ival, ifp, 1) ;
      if ( ival == d.value.ival ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'S':         
      Read_Vpf_Short ( &d.value.sval, ifp, 1) ;
      if ( sval == d.value.sval ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'F':
      Read_Vpf_Float ( &d.value.fval, ifp, 1) ;
      if ( fval == d.value.fval ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'R':         
      Read_Vpf_Float ( &d.value.dval, ifp, 1) ;
      if ( dval == d.value.dval ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'T':
      Read_Vpf_Char ( buf, ifp, h.type_count ) ;
      bufalloc = 1;
      if ( ! strcmp ( value, buf ) ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    case 	'D':
      Read_Vpf_Char ( dateval, ifp, sizeof(date_type) ) ;
      dateval[sizeof(date_type)-1] = '\0';
      if ( ! strncmp ( value, dateval, sizeof(date_type) ) ) {
	/* found the match */
	Match = (short)i;
	i = h.nbins ;	/* exit */
      }
      break ;
    } /* end of switch */

    Read_Vpf_Int ( &d.start_offset, ifp, 1) ;
    Read_Vpf_Int ( &d.num_items, ifp, 1) ;

  }  /* end of i loop */

  if ( bufalloc ) xvt_free ( buf ) ;

  if (Match < 0) {
     d.value.ival = 0;
     d.start_offset = 0;
     d.num_items = 0;
  }

  return d;
}

#ifdef PROTO
ThematicIndexDirectory tidx_binary_search( char *value,
					          ThematicIndexHeader h,
						  FILE *ifp )
#else
ThematicIndexDirectory tidx_binary_search( value, h, ifp )
char *value;
ThematicIndexHeader h;
FILE *ifp;
#endif
{
  int32 			i , ival = 0, left, right;
  short int			sval = 0 , found = 0 , recsize = 0;
  float fval = (float)0.0 ;
  double			dval = 0.0 , atof () ;
  date_type			dateval;
  char				*buf = (char *) NULL, bufalloc=0 ;
  ThematicIndexDirectory	d ;

  d.value.ival = 0;
  d.start_offset = 0;
  d.num_items = 0;

  if ( value ) 		/* search for pattern */
    switch ( h.column_type ) {			/* using address */
    case	'I':
      memcpy ( &ival, value, sizeof (int32)) ;
      recsize = sizeof(int32) + 8;
      break ;
    case	'S':
      memcpy ( &sval, value, sizeof (short int)) ;
      recsize = sizeof(short int) + 8;
      break ;
    case	'F':
      memcpy ( &fval, value, sizeof (float)) ;
      recsize = sizeof(float) + 8;
      break ;
    case	'R':
      memcpy ( &dval, value, sizeof (double)) ;
      recsize = sizeof(double) + 8;
      break ;
    case	'T':
      buf = (char*)xvt_malloc ((size_t)h.type_count);
      /* leave value as is */
      bufalloc = 1;
      recsize = (short)(h.type_count * sizeof(char) + 8);
      break ;
    case 	'D':
      recsize = sizeof(date_type) + 8;
      break;
    }
  else return d;

  left = 0;
  right = h.nbins-1;

  do {

    i = (left+right)/2;
    fseek(ifp,THEMATIC_INDEX_HEADER_SIZE+(i*recsize),0);

    switch ( h.column_type ) {
    case 	'I':
      Read_Vpf_Int ( &d.value.ival, ifp, 1) ;
      if ( ival == d.value.ival )
	found = 1;
      else if ( d.value.ival > ival )
	right = i-1;
      else
	left = i+1;
      break ;
    case 	'S':         
      Read_Vpf_Short ( &d.value.sval, ifp, 1) ;
      if ( sval == d.value.sval )
	found = 1;
      else if ( d.value.sval > sval )
	right = i-1;
      else
	left = i+1;
      break ;
    case 	'F':
      Read_Vpf_Float ( &d.value.fval, ifp, 1) ;
      if ( fval == d.value.fval )
	found = 1;
      else if ( d.value.fval > fval )
	right = i-1;
      else
	left = i+1;
      break ;
    case 	'R':         
      Read_Vpf_Float ( &d.value.dval, ifp, 1) ;
      if ( dval == d.value.dval )
	found = 1;
      else if ( d.value.dval > dval )
	right = i-1;
      else
	left = i+1;
      break ;
    case 	'T':
      Read_Vpf_Char ( buf, ifp, h.type_count ) ;
      bufalloc = 1;
      if ( strcmp ( value, buf ) == 0 )
	found = 1;
      else if ( strcmp( value, buf ) < 0 )
	right = i-1;
      else
	left = i+1;
      break;
    case 	'D':
      Read_Vpf_Char ( dateval, ifp, sizeof(date_type) ) ;
      dateval[sizeof(date_type)-1]='\0';
      bufalloc = 1;
      if ( strncmp ( value, dateval, sizeof(date_type) ) == 0 )
	found = 1;
      else if ( strncmp( value, dateval, sizeof(date_type) ) < 0 )
	right = i-1;
      else
	left = i+1;
      break;
    } /* end of switch */

  }  while ((!found) && (left <= right));

  if (found) {
    Read_Vpf_Int ( &d.start_offset, ifp, 1) ;
    Read_Vpf_Int ( &d.num_items, ifp, 1) ;
  } else {
    d.value.ival = 0;
  }

  if ( bufalloc ) xvt_free ( buf ) ;

  return d;
}

/*************************************************************************
 *
 *N  read_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read an index file, regardless of its type ( T or G ) and 
 *	return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idxname <input> == (char *) name of index file
 *    value   <input> == (char *) address of value to search on. 
 *			 THE PROGRAMMER MUST INSURE THAT THIS ADDRESS
 *			 CORRESPONDS TO THE TABLE COLUMN AND INDEX FILE.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.  This set is
 *                      allocated here and should be nuked when no longer
 *                      needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_*                              VPFREAD.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
set_type read_thematic_index ( char *idxname,
			       char *value )
#else
set_type read_thematic_index ( idxname, value )
char *idxname;
char *value;
#endif
{
  int32 			i , ival = 0;
  short int                     sval = 0;
  FILE				*ifp ;
  char				hack[80] ;
  ThematicIndexHeader		h ;
  ThematicIndexDirectory	d ;
  set_type			s ;

  /* open output index file */

  if (( ifp = muse_file_open ( idxname, "rb" )) == NULL ) {
    sprintf ( hack, "No such index < %s >", idxname ) ;
    SWhimper ( hack ) ;
  }

  if (read_thematic_index_header (&h, ifp) == 0L ) {
    fclose(ifp);
    SWhimper ( "error reading index header" ) ;
  }

  if ( h.index_type == 'G' ) {
    fclose ( ifp ) ;
    s = read_gazetteer_index ( idxname, value ) ;
    return s ;
  }

  s = set_init ( h.table_nrows ) ;

  if (h.sort == 'S')
     d = tidx_binary_search(value,h,ifp);
  else
     d = tidx_linear_search(value,h,ifp);

  if ( d.start_offset == 0 ) {
     fclose(ifp);
     return s;
  }

  if ( d.num_items == 0) {
     set_insert( d.start_offset, s );
     fclose(ifp);
     return s;
  }

  if ( fseek ( ifp, d.start_offset, 0 ) != 0 ) {
    fclose(ifp);
    SWhimper ( "error in fseek") ;
  }

  /* read data into user-defined pointer.  User must free this pointer */
  if ( h.id_data_type == 'I' )
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Int ( &ival, ifp, 1 ) ;
      set_insert ( ival, s ) ;			/* set the id in the set */
    }
  else
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Short ( &sval, ifp, 1 ) ;
      set_insert ( (int32) sval, s ) ;
    }

  fclose ( ifp ) ;

  return s ;					/* also return set */

}   /*end of read_index */


/*************************************************************************
 *
 *N  open_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Open and initialize a thematic index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idxname <input> == (char *) name of index file
 *
 *   Returns:
 *
 *    ThematicIndex     Thematic index structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
ThematicIndex open_thematic_index ( char *idxname, int32 storage )
#else
ThematicIndex open_thematic_index ( idxname, storage )
char *idxname;
int32 storage;
#endif
{
  char				hack[80];
  ThematicIndex		        idx ;
  int32 			i;

  idx.d = NULL;
  idx.gid = NULL;

  /* open output index file */

  if (( idx.fp = muse_file_open ( idxname, "rb" )) == NULL ) {
    sprintf ( hack, "No such index < %s >", idxname ) ;
    OWhimper ( hack ) ;
  }

  if (read_thematic_index_header (&idx.h, idx.fp) == 0L)
    OWhimper ( "error reading index header" ) ;

  if ( idx.h.index_type == 'G' ) {
    /* gazetteer_index  */
     if (read_gazetteer_index_directory(&idx.gid,&idx.h,idx.fp) == 0L) {
	fclose(idx.fp);
	idx.fp = NULL;
     }
     return idx;
  }

  if (storage == RAM) {
     idx.d = (ThematicIndexDirectory*)xvt_malloc ((size_t)idx.h.nbins *
					      sizeof(ThematicIndexDirectory));
     if (idx.d) {

       for ( i=0; i < idx.h.nbins; i++ ) {

	 switch ( idx.h.column_type ) {
	  case 	'I':
	    Read_Vpf_Int ( &idx.d[i].value.ival, idx.fp, 1) ;
	    break ;
	  case 	'S':
	    Read_Vpf_Short ( &idx.d[i].value.sval, idx.fp, 1) ;
	    break ;
	  case 	'F':
	    Read_Vpf_Float ( &idx.d[i].value.fval, idx.fp, 1) ;
	    break ;
	  case 	'R':
	    Read_Vpf_Float ( &idx.d[i].value.dval, idx.fp, 1) ;
	    break ;
	  case 	'T':
	    if (idx.h.type_count == 1) {
	      Read_Vpf_Char ( &idx.d[i].value.cval, idx.fp,
			      idx.h.type_count ) ;
	    } else {
	      idx.d[i].value.strval = (char*)xvt_malloc ((size_t)idx.h.type_count+1);
	      Read_Vpf_Char ( idx.d[i].value.strval, idx.fp,
			      idx.h.type_count ) ;
              idx.d[i].value.strval[idx.h.type_count] = '\0';
	    }
	    break ;
	  case 	'D':
	    idx.d[i].value.strval = (char*)xvt_malloc (sizeof(date_type)+1);
	    Read_Vpf_Char ( idx.d[i].value.strval, idx.fp,
			    sizeof(date_type) );
	    idx.d[i].value.strval[sizeof(date_type)-1] = '\0';
	    break ;
	 } /* end of switch */

	 Read_Vpf_Int ( &idx.d[i].start_offset, idx.fp, 1) ;
	 Read_Vpf_Int ( &idx.d[i].num_items, idx.fp, 1) ;

       }  /* end of i loop */

       if (idx.h.sort != 'S') {
	 /* Sort the directory in memory */
	 switch ( idx.h.column_type ) {
	  case 	'I':
	    qsort ((void*)idx.d, (size_t)idx.h.nbins,
				    sizeof (ThematicIndexDirectory), Icmpval);
	    break ;
	  case 	'S':
	    qsort((void*)idx.d, (size_t)idx.h.nbins,
				     sizeof(ThematicIndexDirectory), Scmpval);
	    break ;
	  case 	'F':
	    qsort ((void*)idx.d, (size_t)idx.h.nbins,
				     sizeof(ThematicIndexDirectory), Fcmpval);
	    break ;
	  case 	'R':
	    qsort ((void*)idx.d, (size_t)idx.h.nbins,
				     sizeof(ThematicIndexDirectory), Dcmpval);
	    break ;
	  case 	'T':
	    if (idx.h.type_count == 1) {
	      qsort ((void*)idx.d, (size_t)idx.h.nbins,
				    sizeof (ThematicIndexDirectory), Ccmpval);
	    } else {
	      qsort ((void*)idx.d, (size_t)idx.h.nbins,
				  sizeof (ThematicIndexDirectory), STRcmpval);
	    }
	    break ;
	  case 	'D':
	    qsort ((void*)idx.d, (size_t)idx.h.nbins,
				  sizeof (ThematicIndexDirectory), STRcmpval);
	    break ;
	 } /* end of switch */

       }
     }

  } else {
     idx.d = NULL;
  }

  return idx;
}

/*************************************************************************
 *
 *N  search_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To search an index file, regardless of its type ( T or G ) and
 *	return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx     <input> == (ThematicIndex) thematic index structure
 *                       previously initialized with open_thematic_index().
 *    value   <input> == (char *) address of value to search on.
 *			 THE PROGRAMMER MUST INSURE THAT THIS ADDRESS
 *			 CORRESPONDS TO THE TABLE COLUMN AND INDEX FILE.
 *                       It should never be NULL.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
set_type search_thematic_index ( ThematicIndex *idx,
				 char *value )
#else
set_type search_thematic_index ( idx, value )
ThematicIndex *idx;
char *value;
#endif
{
  int32 			i , ival = 0;
  short int			sval = 0 ;
  ThematicIndexDirectory	d, dsearch, *dptr = NULL;
  set_type			s ;

  /* open output index file */

  if (!idx->fp) {
     s = set_init(1);
     return s;
  }

  if ( idx->h.index_type == 'G' ) {
    s = search_gazetteer_index ( idx, value ) ;
    return s ;
  }

  /* create set */

  s = set_init ( idx->h.table_nrows ) ;

  /* Look for a matching directory entry */

  d.start_offset = 0;

  if (idx->d) {

     /* Directory in memory */
     switch ( idx->h.column_type ) {
       case 'I':
	 memcpy( &dsearch.value.ival, value, sizeof(int32) );
	 dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			Icmpval);
	 break ;
       case 'S':
	 memcpy( &dsearch.value.sval, value, sizeof(short int) );
	 dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			Scmpval);
	 break ;
       case 'F':
	 memcpy( &dsearch.value.fval, value, sizeof(float) );
	 dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			Fcmpval);
	 break ;
       case 'R':
	 memcpy( &dsearch.value.dval, value, sizeof(double) );
	 dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			Dcmpval);
	 break ;
       case 'T':
	 if (idx->h.type_count == 1) {
	   memcpy( &dsearch.value.cval, value, sizeof(char) );
	   dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			Ccmpval);
	 } else {
	   dsearch.value.strval = (char*)xvt_malloc((size_t)idx->h.type_count+1);
	   memcpy (dsearch.value.strval, value, (size_t)idx->h.type_count);
	   dsearch.value.strval[idx->h.type_count] = '\0';
	   dptr = (ThematicIndexDirectory *)bsearch(
                        (void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			STRcmpval);
	   xvt_free(dsearch.value.strval);
	 }
	 break ;
       case 'D':
	 dsearch.value.strval = (char*)xvt_malloc(sizeof(date_type)+1);
	 memcpy( dsearch.value.strval, value, sizeof(date_type) );
	 dsearch.value.strval[sizeof(date_type)-1] = '\0';
	 dptr = (ThematicIndexDirectory *)bsearch(
			(void *)&dsearch, (void *)idx->d,
			(size_t)idx->h.nbins, sizeof(ThematicIndexDirectory),
			STRcmpval);
	 xvt_free(dsearch.value.strval);
	 break ;
     } /* end of switch */

     if (dptr)
	d = *dptr;
     else
	d.start_offset = 0;

  } else {

     /* Search directory  on disk */
     if (idx->h.sort == 'S')
       d = tidx_binary_search(value,idx->h,idx->fp);
     else
       d = tidx_linear_search(value,idx->h,idx->fp);
  }

  if ( d.start_offset == 0 ) {
     return s;
  }

  if ( d.num_items == 0) {
     set_insert( d.start_offset, s );
     return s;
  }

  /* We've got a match, so now read the ids in */

  if ( fseek ( idx->fp, d.start_offset, 0 ) != 0 ) {
    set_nuke(&s);
    SWhimper ( "error in fseek") ;
  }

  /* read data into user-defined pointer.  User must free this pointer */

  if ( idx->h.id_data_type == 'I' )
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Int ( &ival, idx->fp, 1 ) ;
      set_insert ( ival, s ) ;			/* set the id in the set */
    }
  else
    for ( i=0 ; i < d.num_items; i++ ) {
      Read_Vpf_Short ( &sval, idx->fp, 1 ) ;
      set_insert ( (int32) sval, s ) ;
    }

  return s ;					/* also return set */

}   /*end of search_index */


/*************************************************************************
 *
 *N  close_thematic_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	Close a thematic index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx <input> == (ThematicIndex *) thematic index
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
void close_thematic_index ( ThematicIndex *idx )
#else
void close_thematic_index ( idx )
ThematicIndex *idx;
#endif
{
  int32 i;

  fclose ( idx->fp ) ;
  if (idx->d) {
     if ( (idx->h.column_type == 'T' && idx->h.type_count > 1) ||
	  (idx->h.column_type == 'D') ) {
	for (i=0;i<idx->h.nbins;i++)
	   if (idx->d[i].value.strval)
	      xvt_free(idx->d[i].value.strval);
     }
     xvt_free ((char*)idx->d);
  }
  if (idx->gid)
     xvt_free ((char*)idx->gid);
}

/*************************************************************************
 *
 *N  create_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To create a gazetteer file
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *
 *    tablename  <input> == (char *) the name of an existing file
 *					<= 11 chars long
 *    idx_fname  <input> == (char *) name of index file
 *    columnname <input> == (char *) is a column existing in tablename
 *				        <= 24 chars long
 *
 *    idx_set <input> == (char *) is an array of printable ASCII characters 
 *				with no duplicates
 *
 *    return value < 0
 *
 *  	  -1 some OS error, check errno for something more exact
 *        -2 tablename is not a vpf table
 *        -3 columnname is not in the vpf table 
 *        -4 column type not text
 *
 *    return value = 0
 *         an index record for each character in index_set has been created
 *
 *         the index file name is the same as tablename but with .?ti
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 *	Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#ifdef PROTO
int32 create_gazetteer_index (char *tablename,
				 char *idx_fname ,
				 char *columnname,
				 char *idx_set)
#else
int32 create_gazetteer_index (tablename, idx_fname, columnname, idx_set)
char *tablename;
char *idx_fname;
char *columnname;
char *idx_set;
#endif
{
  vpf_table_type t;
  row_type r;
  int32 c;
  FILE	*idx_fp;
  ThematicIndexHeader gi;
  ThematicIndexDirectory *gid;
  set_type *idx_bit_sets;
  register int32 i, j, l;
  int32	set_byte_size;

  if (!tablename) return -2;
  if (!idx_fname) return -1;
  if (!columnname) return -3;
  if (!idx_set) return -1;

  t = vpf_open_table(tablename, disk, "rb", '\0');
  if (!t.fp) return -2;
#ifdef _MSDOS
  c = table_pos(strupr(columnname), t);
#else
  c = table_pos(columnname, t);
#endif
  if (c<0) return -3;

  if (t.header[c].type != 'T') {
    vpf_close_table(&t);
    return -4;
  }

  idx_fp = muse_file_open(idx_fname, "wb");

  if (idx_fp == NULL) {
    vpf_close_table(&t);
    return -1;
  }

  /* only write out the table name, not the rest */

  for ( i = strlen ( tablename ); i > 0; i-- )
    if ( tablename[i] == DIR_SEPARATOR ) break ;
  if ( tablename[i] == DIR_SEPARATOR )
#ifdef _MSDOS
    strncpy ( gi.vpf_table_name, strupr ( &tablename[i+1] ), 13 ) ;
#else
    strncpy ( gi.vpf_table_name, &tablename[i+1], 13 ) ;
#endif
  else
#ifdef _MSDOS
    strncpy(gi.vpf_table_name, strupr ( tablename), 13 );
#else
    strncpy(gi.vpf_table_name, tablename, 13);
#endif
  strncpy(gi.vpf_column_name, columnname,25);

  gi.column_type = 'T';
  gi.index_type  = 'G';
  gi.type_count = 1 ;
  gi.id_data_type = 'S' ;
  gi.nbins       = strlen(idx_set);
  gi.table_nrows = t.nrows;
  gi.sort        = ' ';
  gi.padding[0]  = ' ';
  gi.padding[1]  = ' ';
  gi.padding[2]  = ' ';
  set_byte_size = (int32)ceil(t.nrows/8.0);

  /* force input string to lower case , rdf */

  for ( i=0; i < gi.nbins; i++ )
    idx_set[i] = (char)tolower (idx_set[i]);

  /*
   * ti.nbytes is the offset into the index file at which the
   * bit arrays start
   */

  gi.nbytes = THEMATIC_INDEX_HEADER_SIZE +
              gi.nbins * ( sizeof (char) + DIRECTORY_SIZE ) ;

  gid = (ThematicIndexDirectory*)xvt_malloc (sizeof (ThematicIndexDirectory)
							  * (size_t)gi.nbins);
  idx_bit_sets = (set_type*)xvt_malloc (sizeof(set_type) * (size_t)gi.nbins);

  for (i = 0; i < gi.nbins; i++) {
    idx_bit_sets[i]   = set_init(t.nrows);
    gid[i].value.cval = idx_set[i];
    gid[i].num_items  = 1;
  }

  /*
   * lets suck up some CPU cycles here
   */
  for (l = 0; l < t.nrows; l++) {

    r = get_row(l+1, t);

    for (i = 0; i < gi.nbins; i++)
      for (j = 0; j < r[c].count; j++)
	/* No matter the input character, make it lower case , rdf added */
        if ( tolower (*((char *) r[c].ptr + j)) == idx_set[i]) {
          set_insert(l, idx_bit_sets[i]);
          break;
        }

    free_row(r, t);

  }

  vpf_close_table(&t);

  if (write_thematic_index_header (gi, idx_fp) == '\0') {
    fclose(idx_fp);
    for (i = 0; i < gi.nbins; i++)
      set_nuke(&idx_bit_sets[i]);
    xvt_free ((char*)idx_bit_sets);
    xvt_free ((char*)gid);
    return -1;
  }

  if (write_gazetteer_index_directory(gi,
                                     gid,
                                     set_byte_size,
                                     idx_fp) == 0         ) {
    fclose(idx_fp);
    for (i = 0; i < gi.nbins; i++)
      set_nuke(&idx_bit_sets[i]);
    xvt_free ((char*)idx_bit_sets);
    xvt_free ((char*)gid);
    return -1;
  }

  for (i = 0; i < gi.nbins; i++) {
    if ( ! Write_Vpf_Char(idx_bit_sets[i].buf, idx_fp, set_byte_size) ) {
      fclose(idx_fp);
      for (i = 0; i < gi.nbins; i++)
        set_nuke(&idx_bit_sets[i]);
      return -1;
    }
  }

  fclose(idx_fp);
  for (i = 0; i < gi.nbins; i++)
    set_nuke(&idx_bit_sets[i]);
  xvt_free ((char*)idx_bit_sets);
  xvt_free ((char*)gid);

  return 0;
}



/*************************************************************************
 *
 *N  read_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read an gazeeteer index file, and return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    idx_fname <input> == (char *) the name of a vpf gazetteer index
 *                         created with a call to to create_gaz_index.
 *
 *    query_str <input> == (char *) string that contains only characters
 *                         which form a subset of the characters passed
 *                         to create_gaz_index when idx_fname was created.
 *
 *      the vpf table from which the file idx_fname was derived must
 *      exist in the same directory as idx_fname
 *
 * out: case the returned set size > 0
 *        a bit array which can be used as a rough filter for the
 *        vpf table from which the index file idx_fname was derived.
 *    
 *      bits which are set in the array are the record numbers in the vpf table
 *        (from which idx_fname was derived) which may contain query_string
 *
 *      case the returned set size = 0
 *        some system error occurred
 *        most likely the indexed vpf table couldn't be found in the
 *        same directory
 *
 * note: query_str is treated as a set, so duplicate occurrences
 *       of the same character are ignored. I need a way of flagging
 *       those duplicate occurrences. Instead of sorting the string
 *       and removing duplicates, I use the num_items field in each
 *       element of the directory array.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 * 	Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
set_type read_gazetteer_index (char *idx_fname, char *query_str )
#else
set_type read_gazetteer_index (idx_fname, query_str )
char *idx_fname;
char *query_str;
#endif
{
  ThematicIndexHeader gi;
  ThematicIndexDirectory
                    * gid;
  FILE              * idx_fp;
  static set_type            query_set = {0, 0},
		      xsect_set,
                      result_set;
  register int32        query_len = strlen(query_str),
                      i,
                      j;
  int32	     set_byte_size;
  char query_char;

  if (!idx_fname) return query_set;
  if (!query_str) return query_set;

  idx_fp = muse_file_open(idx_fname, "rb");

  if (idx_fp == NULL)
    return query_set;

  if (read_thematic_index_header (&gi, idx_fp) == '\0') {
    fclose(idx_fp);
    return query_set;
  }

  if (read_gazetteer_index_directory (&gid, &gi, idx_fp) == '\0') {
    fclose(idx_fp);
    return query_set;
  }

  for (j = 0; j < gi.nbins; j++)
    gid[j].num_items = 0;

  query_set = set_init(gi.table_nrows);
  xsect_set = set_init(gi.table_nrows);

  set_on(query_set);
  set_byte_size = (int32)ceil (gi.table_nrows/8.0);

  for (i = 0; i < query_len; i++) {

    query_char = (char)tolower (query_str[i]);

    for (j = 0; j < gi.nbins; j++)
      if (gid[j].value.cval == query_char)  {
        gid[j].num_items++;
        break;
      }
    if (gid[j].num_items > 1)
      continue;

    fseek(idx_fp, gid[j].start_offset, 0);
    if ( ! Read_Vpf_Char(xsect_set.buf, idx_fp, set_byte_size) ) {
      xvt_note ("read_gazetteer_index: error reading index");  /*DGM*/
      set_off(query_set);
      set_nuke(&xsect_set);
      fclose(idx_fp);
      xvt_free ((char*)gid);
      return query_set ;
    }

    result_set = set_intersection(query_set, xsect_set);
    set_assign(&query_set, result_set);
    set_nuke(&result_set);

  }

  fclose(idx_fp);
  xvt_free ((char*)gid);
  set_nuke(&xsect_set);

  return query_set;
}

/*************************************************************************
 *
 *N  search_gazetteer_index
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To search an gazeeteer index file, and return a set type array
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 * in : idx is the gazetteer index opened with open_thematic_index
 *
 *      query_str contains only characters which form a subset of
 *      the characters passed to create_gaz_index when idx_fname
 *      was created
 *
 *      the vpf table from which the file idx_fname was derived must
 *      exist in the same directory as idx_fname
 *
 * out: case the returned set size > 0
 *        a bit array which can be used as a rough filter for the
 *        vpf table from which the index file idx_fname was derived.
 *    
 *      bits which are set in the array are the record numbers in the vpf table
 *        (from which idx_fname was derived) which may contain query_string
 *
 *      case the returned set size = 0
 *        some system error occurred
 *        most likely the indexed vpf table couldn't be found in the
 *        same directory
 *
 * note: query_str is treated as a set, so duplicate occurrences
 *       of the same character are ignored. I need a way of flagging
 *       those duplicate occurrences. Instead of sorting the string
 *       and removing duplicates, I use the num_items field in each
 *       element of the directory array.
 *
 *   Returns:
 *
 *    set_type		a set corresponding to the vpf table, where each
 *			bit relates to the ID in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    	Jim Tenbrink
 * 	Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
set_type search_gazetteer_index (ThematicIndex *idx, char *query_str )
#else
set_type search_gazetteer_index ( idx, query_str )
ThematicIndex *idx;
char *query_str;
#endif
{
  static set_type  query_set = {0, 0}, xsect_set, result_set;
  register int32        query_len = strlen(query_str),
                      i,
                      j;
  int32	     set_byte_size;
  char                query_char;

  if (idx->fp == NULL)
    return query_set;

  for (j = 0; j < idx->h.nbins; j++)
    idx->gid[j].num_items = 0;

  query_set = set_init(idx->h.table_nrows);
  xsect_set = set_init(idx->h.table_nrows);

  set_on(query_set);
  set_byte_size = (int32)ceil (idx->h.table_nrows / 8.0);

  for (i = 0; i < query_len; i++) {

    query_char = (char)tolower (query_str[i]);

    for (j = 0; j < idx->h.nbins; j++)
      if (idx->gid[j].value.cval == query_char)  {
	idx->gid[j].num_items++;
	break;
      }
    if (idx->gid[j].num_items > 1)
      continue;

    fseek(idx->fp, idx->gid[j].start_offset, 0);
    if ( ! Read_Vpf_Char(xsect_set.buf, idx->fp, set_byte_size) ) {
      set_type err;
      err =  set_init ( 1 ) ;
      xvt_note ("read_gazetteer_index: error reading index");  /*DGM*/
      return err ;
    }

    result_set = set_intersection(query_set, xsect_set);
    set_assign(&query_set, result_set);
    set_nuke(&result_set);

  }

  set_nuke(&xsect_set);

  return query_set;
}

/*************************************************************************
 *
 *N  read_gazetteer_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read a gazetteer index directory.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 * in : *gid is undefined
 *
 *      *gi is the gazetteer index header for the open index file
 *      referenced by idx_fp
 *
 * out: case return value is one
 *        *gid is an array of directory entries with gi->nbin elements
 *
 *        the sval member of the value union contains the indexed character
 *
 *        the start_offset member of the directory structure contains the
 *        disk offset for the start of the indexed characters bit array
 *
 *      case zero return value
 *        an error was probably encountered during the read
 *        *gid is undefined
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	read_thematic_index_header		see below
 *	set_init				SET.C
 *	set_insert
 *	Vpf_Read_* macros
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
int32 read_gazetteer_index_directory(
				   ThematicIndexDirectory **gid,
				   ThematicIndexHeader     *gi,
				   FILE                    *idx_fp)
#else
int32 read_gazetteer_index_directory(gid, gi, idx_fp)
ThematicIndexDirectory **gid;
ThematicIndexHeader     *gi;
FILE                *idx_fp;
#endif
{
  int32 i;

  if ( fseek ( idx_fp, THEMATIC_INDEX_HEADER_SIZE, 0 )!= 0 )
    return 0 ;

  *gid = (ThematicIndexDirectory *) xvt_malloc (sizeof (ThematicIndexDirectory)
							 * (size_t)gi->nbins);
  if (*gid == NULL)
    return 0;

  for (i = 0; i < gi->nbins; i++) {
    if ( ( ! Read_Vpf_Char(  &( (*gid)[i].value.cval ),   idx_fp, 1) ) ||
	 ( ! Read_Vpf_Int(   &( (*gid)[i].start_offset ), idx_fp, 1) ) ||
	 ( ! Read_Vpf_Int(   &( (*gid)[i].num_items ),    idx_fp, 1) )) {
      xvt_note ("read_gazetteer_index_directory: error reading dir");  /*DGM*/
      return ('\0');
    }
  }
  return 1;
}

/*************************************************************************
 *
 *N  read_thematic_index_header
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To read the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < in/out > == (ThematicIndexHeader *) header structure to be filled
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	Read_Vpf_Int
 *	Read_Vpf_Char
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define RWhimper() { \
   xvt_note ("\nread_thematic_index_header: error reading header");\
    } /*DGM*/

#ifdef PROTO
int32 read_thematic_index_header ( ThematicIndexHeader *h, FILE *ifp ) 
#else
int32 read_thematic_index_header ( h, ifp )
ThematicIndexHeader *h;
FILE *ifp;
#endif
{

/*  char *valid_index_types = "TG";   DGM */
  char *valid_index_types = "TGIB";  /* DGM */
  char *valid_column_types = "TDISFR";
  char *valid_id_data_types = "IS";
  char *valid_sort = "S ";

  if ( fseek ( ifp, 0, 0 ) != 0 ) 	/* rewind, just in case */
    return 0 ;				/* error */
  if ( ! Read_Vpf_Int ( &h->nbytes, ifp, 1 ) )
    RWhimper();
  if ( ! Read_Vpf_Int ( &h->nbins, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Int ( &h->table_nrows, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->index_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->column_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Int ( &h->type_count, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->id_data_type, ifp, 1 ) )
    RWhimper() ;
  if ( ! Read_Vpf_Char ( &h->vpf_table_name, ifp, 12 ) )
    RWhimper() ;
  h->vpf_table_name[12] = '\0';
  if ( ! Read_Vpf_Char ( &h->vpf_column_name, ifp, 24 ) )
    RWhimper() ;
  h->vpf_column_name[24] = '\0';
  if ( ! Read_Vpf_Char ( &h->sort, ifp, 1 ) )
    RWhimper() ;
  h->sort = (char)toupper(h->sort);
  if ( ! Read_Vpf_Char ( &h->padding, ifp, 3 ) )
    RWhimper() ;

  /* Perform a simple (but theoretically not foolproof) validity check */
  if (!strchr (valid_index_types,(char)toupper(h->index_type)))
     return ('\0');
  if (!strchr (valid_column_types,(char)toupper(h->column_type)))
     return ('\0');
  if (!strchr (valid_id_data_types,(char)toupper(h->id_data_type)))
     return ('\0');
  if (!strchr (valid_sort,(char)toupper(h->sort)))
     return ('\0');

  return 1 ;
}

/*************************************************************************
 *
 *N  write_thematic_index_header
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure to write from
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	Write_Vpf_Int
 *	Write_Vpf_Char
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WWhimper() {\
	      xvt_note ("write_thematic_index_header: error reading header");}

#ifdef PROTO
int32 write_thematic_index_header ( ThematicIndexHeader h, FILE *ifp ) 
#else
int32 write_thematic_index_header ( h, ifp ) 
ThematicIndexHeader h;
FILE *ifp; 
#endif
{
  if ( fseek ( ifp, 0, 0 ) != 0 ) 	/* rewind, just in case */
    WWhimper() ;				/* error */
  if ( ! Write_Vpf_Int ( &h.nbytes, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.nbins, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.table_nrows, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.index_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.column_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Int ( &h.type_count, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.id_data_type, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.vpf_table_name, ifp, 12 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.vpf_column_name, ifp, 25 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.sort, ifp, 1 ) )
    WWhimper() ;
  if ( ! Write_Vpf_Char ( &h.padding, ifp, 3 ) )
    WWhimper() ;
  return 1 ;
}


/*************************************************************************
 *
 *N  write_thematic_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure
 *	d < input > == (ThematicIndexDirectory *) directory array structure
 *	idsize < input > == (int32) size of each data element.
 *		for T indexes, this is either 2 bytes or 4 bytes
 *		for G indexes, it will be num_in_set (set) 
 *			       or the size of the bit array.
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	Vpf_Write_*
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WTWhimper() { \
   xvt_note ("write_thematic_index_directory: error reading header");}

#ifdef PROTO
int32 write_thematic_index_directory ( ThematicIndexHeader h, 
					  ThematicIndexDirectory *d,
					  int32 idsize , /* size of data */
					  FILE *ifp )
#else
int32 write_thematic_index_directory ( h, d, idsize , ifp )
ThematicIndexHeader h;
ThematicIndexDirectory *d;
int32 idsize;/* size of data */
FILE *ifp;
#endif
{
  int32	offset = h.nbytes ,
		i, zero=0 ;

  /* rewind, just in case */
  if ( fseek ( ifp, THEMATIC_INDEX_HEADER_SIZE, 0 ) != 0 )
    WTWhimper() ;

  for ( i=0; i < h.nbins; i++ ) {
    
    switch ( h.column_type ) {
    case 'I':
      if ( ! Write_Vpf_Int ( &d[i].value.ival, ifp, 1 ) )
	WTWhimper() ;
      break ;
    case 'T':
      if (h.type_count == 1) {
        if ( ! Write_Vpf_Char ( &d[i].value.cval, ifp, h.type_count ) )
	  WTWhimper() ;
      } else {
        if ( ! Write_Vpf_Char ( d[i].value.strval, ifp, h.type_count ) )
	  WTWhimper() ;
      }
      break ;
    case 'D':
      if ( ! Write_Vpf_Char ( d[i].value.strval, ifp, sizeof(date_type) ) )
	WTWhimper() ;
      break ;
    case 'S':
      if ( ! Write_Vpf_Short ( &d[i].value.sval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    case 'F':
      if ( ! Write_Vpf_Float ( &d[i].value.fval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    case 'R':
      if ( ! Write_Vpf_Double ( &d[i].value.dval, ifp, 1 ) )
	WTWhimper() ;				
      break ;
    }	/* end of switch */

    if (d[i].num_items > 1) {

       if ( ! Write_Vpf_Int ( &offset, ifp, 1 ) )
	 WTWhimper() ;
       d[i].start_offset = offset;

       if ( ! Write_Vpf_Int ( &d[i].num_items, ifp, 1 ) )
	 WTWhimper() ;
    
       /* this offset is constant, no matter the data type */

       offset += ( idsize * d[i].num_items ) ;

    } else {

       /* start_offset has been set to the only id value above */
       if ( ! Write_Vpf_Int ( &d[i].start_offset, ifp, 1 ) )
	 WTWhimper() ;

       if ( ! Write_Vpf_Int ( &zero, ifp, 1 ) )
	 WTWhimper() ;

    }

  }  /* end of i loop */

  return 1 ;

}    /* end of write_directory */


/*************************************************************************
 *
 *N  write_gazetteer_index_directory
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *	To write the gazetteer thematic index header in a standard way
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *	h < input > == (ThematicIndexHeader) header structure 
 *	d < input > == (ThematicIndexDirectory *) directory array structure
 *	idsize < input > == (int32) size of each data element.
 *		for T indexes, this is either 2 bytes or 4 bytes
 *		for G indexes, it will be num_in_set (set) 
 *			       or the size of the bit array.
 *	ifp <input>  == ( FILE *ifp ) index file pointer
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       October 1991
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Functions Called:
 *F
 *	Vpf_Write_*
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/

#define WTGWhimper() { \
	  xvt_note ("write_gazetteer_index_directory: error writing header");}

#ifdef PROTO
int32 write_gazetteer_index_directory ( ThematicIndexHeader h, 
					  ThematicIndexDirectory *d, 
					  int32 idsize , /* size of data */
					  FILE *ifp )
#else
int32 write_gazetteer_index_directory (  h, d, idsize, ifp )
ThematicIndexHeader h;
ThematicIndexDirectory *d;
int32 idsize;
FILE *ifp;
#endif
{
  int32	offset = h.nbytes ,
                i ;

  /* rewind, just in case */

  if ( fseek ( ifp, THEMATIC_INDEX_HEADER_SIZE, 0 )!= 0 )
    WTGWhimper() ;				

  for ( i=0; i < h.nbins; i++ ) {
    
    if ( ! Write_Vpf_Char ( &d[i].value.cval, ifp, 1 ) )
      WTGWhimper() ;

    if ( ! Write_Vpf_Int ( &offset, ifp, 1 ) )
	WTGWhimper() ;				
    if ( ! Write_Vpf_Int ( &d[i].num_items, ifp, 1 ) )
	WTGWhimper() ;				
    
    /* this offset is constant, no matter the data type */

    offset += ( idsize * d[i].num_items ) ;

  }  /* end of i loop */

  return 1 ;

}    /* end of write_directory */
