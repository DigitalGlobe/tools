/*************************************************************************
 *
 *N  Module VPFWRITE.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This contains functions for writing data to VPF tables.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     N/A
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *     Original Coding:  Tom Wood       Fall 1990
 *       Modifications:  David Flinn    January 1991
 *                                      July 1991
 *                       Barry Michaels  October 1991
 *                                      Modified from converter
 *                                      software (UNIX) for
 *                                      VPFVIEW software (DOS).
 *                       Jim TenBrink   October 1991
 *                                      Made vpfread.c and vpfwrite.c
 *                                      disjoint
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
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif

#if 0
#if _MSDOS
#include <io.h>
#include <malloc.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#endif
#endif

#ifndef _MACHINE_
#include "machine.h"
#endif

#ifdef _UNIX
#include <sys/stat.h>
#define   SEEK_SET    0         /* Turbo C fseek value */
#define   SEEK_CUR    1
#endif

#ifdef __VPF_H__
#include "vpf.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif

extern int32 STORAGE_BYTE_ORDER;

/* Include statically to reduce external module dependencies */

#ifdef PROTO
void *vpfmalloc( uint32 size )
#else
void *vpfmalloc( size )
uint32 size;
#endif
{
   void *p;
   p = (void *)xvt_zmalloc ((size_t)size);
   if (p == (void *)NULL) {
      xvt_note ("out of memory in vpfwrite\n");
   }
   return p;
}



/*************************************************************************
 *
 *N  write_key
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function writes an id triplet key from the specified file.
 *     It is assumed that there is enough free disk space to write to the
 *     file. It is also assumed that the file pointer (fp) is already opened
 *     for writing.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    key     <input> == (id_triplet_type) id triplet key.
 *    fp      <input> == (FILE *) input file pointer.
 *    return <output> == (int32) size of the key.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       July 1991      Based on read_key in vpftable.c
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
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
int32 write_key( id_triplet_type key, FILE *fp )
#else
int32 write_key( key, fp )
id_triplet_type key; 
FILE *fp;
#endif
{
  int32 size = 0 ;   /* to count size of key write */
  unsigned char tint ;
  short int tshort ;

   /* Assume that any count value has been written before this */
   /* Only write one key in this subroutine, do not write more */

  Write_Vpf_Char (&(key.type),fp,1);
  size += sizeof ( char ) ;

   switch (TYPE0(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.id ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.id ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( short int ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.id), fp, 1 ) ;
     size += sizeof ( int32 ) ;
     break;
   }

   switch (TYPE1(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.tile ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.tile ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( short int ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.tile), fp, 1 ) ;
     size += sizeof ( int32 ) ;
     break;
   }

   switch (TYPE2(key.type)) {
   case 0:
     break;
   case 1:
     tint = (unsigned char) key.exid ;
     Write_Vpf_Char ( &tint, fp, 1 ) ;
     size += sizeof ( char ) ;
     break;
   case 2:
     tshort = (short) key.exid ;
     Write_Vpf_Short ( &tshort, fp, 1 ) ;
     size += sizeof ( short int ) ;
     break;
   case 3:
     Write_Vpf_Int (&(key.exid), fp, 1 ) ;
     size += sizeof ( int32 ) ;
     break;
   }
  return size ;
}

/*************************************************************************
 *
 *N  write_next_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function writes the next row of the table.
 *     The parameter row must be initialized prior to this functional, either
 *     by being read in from an existing table or set to valid values.
 *     A row with any empty columns should not be written out.
 *     The parameter table must be a valid table and initialized prior to
 *     this function, by vpf_open_table.  It is assumed that there is
 *     enough free disk space to write to the file. It is also assumed that
 *     the file pointer (table->fp) is already opened for writing. The
 *     variable count, set to the values in row, must be greater than 0,
 *     otherwise, if count is -1 the vpf_write functions will lock up
 *     (row[].count should never have a value of 0). Note that if errorfp
 *     is used, it must be opened prior to this function.
 *
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row        <input> == (row_type) the row to write to the table.
 *    table      <input> == (vpf_table_type *) vpf table structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Dave Flinn       July 1991      Based on read_next_row.
 *    Barry Michaels    Oct 1991      Added row as a parameter.
 *    JTB              10/91          guaranteed function always
 *                                    returns a value:
 *                                     0: record written
 *                                    -1: unknown field type
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
 *   None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
int32 write_next_row(row_type row, vpf_table_type * table )
#else
int32 write_next_row( row, table )
row_type row;
vpf_table_type * table;
#endif
{
   register int32 i,
                     j;
   char            * tptr,
                   * output ;
   int32          recordsize = 0;
   int32          count;
   id_triplet_type * keys;
   uint32 pos_for_ndx,
                     length;
   int32               retn_val = 0;
   static coordinate_type   dummycoord = {(float)0.0, (float)0.0};

   STORAGE_BYTE_ORDER = table->byte_order;

   table->nrows++;
   fseek(table->fp, 0L, SEEK_END);
   pos_for_ndx = ftell(table->fp); /* begining of new row */

   for (i = 0; i < table->nfields; i++) {   /* for each column */

     count = row[i].count ;          /* Retrieve count from row.  Should
                                        be 0 if variable length null */

     /* In case this column is variable length, write out count */

     if (count == 0) count = 1;

     if ( table->header[i].count < 0 ) {
       Write_Vpf_Int ( &count, table->fp, 1 ) ;
       recordsize += sizeof ( int32 ) ;
     }

     /* Now write out the data type */

     switch (table->header[i].type) {

     case 'T':
       if ( count == 0 )        /* Assume this is variable length text
                                   and don't do anything */
         break ;

       /* This loop insures that the exact number of characters are written
          out to disk. */

       output = (char *) vpfmalloc ( count + 1 ) ;  /* include null byte */
       for (j = 0, tptr = row[i].ptr; j < count; j++, tptr++)
         if ( *tptr )
           output[j] = *tptr ;
         else
           output[j] = SPACE ;
       output[count] = '\0' ;
       Write_Vpf_Char( output ,table->fp, count) ;
       if(output != (char *)NULL)
         {xvt_free ( output ) ;output = (char *)NULL;}
       recordsize += sizeof ( char ) * count ;
       break;

     case 'I':
       Write_Vpf_Int (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( int32 ) * count ;
       break;

     case 'S':
       Write_Vpf_Short (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( short int ) * count ;
       break;

     case 'F':
       Write_Vpf_Float (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( float ) * count ;
       break;

     case 'R':
       Write_Vpf_Double (row[i].ptr, table->fp, count ) ;
       recordsize += sizeof ( double ) * count ;
       break;

     case 'D':  /* date has 21 chars in memory, not on disk */
       Write_Vpf_Date (row[i].ptr, table->fp, count ) ;
       recordsize += ( sizeof ( date_type ) - 1 ) * count ;
       break;

     case 'C':
       if (row[i].ptr) {
          Write_Vpf_Coordinate(row[i].ptr,table->fp,count);
       } else {
          for (j=0;j<count;j++)
             Write_Vpf_Coordinate(&dummycoord,table->fp,count);
       }
       recordsize += sizeof ( coordinate_type ) * count ;
       break;

     case 'B':
       Write_Vpf_DoubleCoordinate(row[i].ptr,table->fp,count);
       recordsize += sizeof ( double_coordinate_type ) * count ;
       break;

     case 'Z':
       Write_Vpf_CoordinateZ(row[i].ptr,table->fp,count);
       recordsize += sizeof ( tri_coordinate_type ) * count ;
       break;

     case 'Y':
       Write_Vpf_DoubleCoordinateZ(row[i].ptr,table->fp,count);
       recordsize += sizeof ( double_tri_coordinate_type ) * count ;
       break;

     case 'K':
       keys = (id_triplet_type *) vpfmalloc (count*sizeof(id_triplet_type)) ;
       memcpy (keys, row[i].ptr, (size_t)count * sizeof(id_triplet_type) ) ;
       for (j=0;j<count;j++)
         recordsize += write_key ( keys[j], table->fp);
       if(keys != (id_triplet_type *)NULL)
         {xvt_free ((char*)keys);keys = (id_triplet_type *)NULL;}
       break;

     case 'X':
       /* do nothing */
       break;

     default:
       xvt_note ("write_next_row: no such type < %c >",
               table->header[i].type ) ;  /*DGM*/
       return(-1);
     }
   }

   if ( table->xfp ) {  /* only for variable length columns */
     length = recordsize ;
     fseek( table->xfp, 0, SEEK_END );

     Write_Vpf_Int ( &pos_for_ndx, table->xfp, 1 ) ;
     Write_Vpf_Int ( &length, table->xfp, 1 ) ;
   }

   return retn_val;
}


/*************************************************************************
 *
 *N  create_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function creates a null row for the given table.
 *     The parameter table must be a valid table and initialized prior to
 *     this function, by vpf_open_table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) row of the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels    Oct 1991
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
 *   None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
row_type create_row( vpf_table_type table )
#else
row_type create_row( table )
vpf_table_type table;
#endif
{
   int32 i;
   row_type row;

   row = (row_type)vpfmalloc(table.nfields*sizeof(column_type));
   for (i=0;i<table.nfields;i++) {
      row[i].count = table.header[i].count;
      row[i].ptr = NULL;
   }
   return row;
}



/*************************************************************************
 *
 *N  nullify_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Frees one field element - no action is taken if the
 *     field index is invalid.
 *     The parameter row must be initialized prior to this functional, either
 *     buy being read in from an existing table or set to valid values. The
 *     parameter table must be a valid table and initialized prior to this
 *     function, by vpf_open_table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     field <input>  == (int32) column offset.
 *     row   <inout> == (row_type) row containing element to be removed.
 *     table <inout> == (vpf_table_type) VPF table owning row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *   RDF  7/91  original
 *   JTB  10/91 removed call to exit();
 *E
 *************************************************************************/
#ifdef PROTO
void nullify_table_element( int32            field,
                            row_type       row,
                            vpf_table_type table )
#else
void nullify_table_element(  field, row, table )
int32     field;
row_type       row;
vpf_table_type table;
#endif
{
   if (field < 0 || field >= table.nfields)
     return;

   if (row[field].ptr)
   {
     if(row[field].ptr != (void *)NULL)
       xvt_free((char *)row[field].ptr);
     row[field].ptr = NULL;
     row[field].count = table.header[field].count;
   }
}

/*************************************************************************
 *
 *N  put_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     Copies one element into the designated field.
 *     The parameter row must be initialized prior to this functional, either
 *     buy being read in from an existing table or set to valid values. The
 *     parameter table must be a valid table and initialized prior to this
 *     function, by vpf_open_table. Note that if errorfp is used, it must
 *     be opened prior to this function.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *     field <input>  == (int32) column offset.
 *     row   <in-out> == (row_type) row containing target field.
 *     table <in-out> == (vpf_table_type) VPF table owning row.
 *     value <in>     == (void *) source field element.
 *     count <in>     == (int32) number of items in value.
 *     put_table_element <output> == (int32)
 *                                    0 --> element write succeeded
 *                                    1 --> unknown element type or
 *                                          invalid column offset
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *   RDF  7/91  original
 *   JTB  10/91 removed call to exit();
 *              guaranteed function always returns value
 *              0: element write succeeded
 *             -1: unknown element type or invalid column (field) offset
 *E
 *************************************************************************/
#ifdef PROTO
int32 put_table_element( int32              field,
                       row_type         row,
                       vpf_table_type   table,
                       void           * value,
                       int32         count )
#else
int32 put_table_element( field, row, table, value, count )
int32       field;
row_type         row;
vpf_table_type  table;
void           *value;
int32        count;
#endif
{
   int32 i, len, stat;
   char *str;

   stat=0;

   if ((count != table.header[field].count) &&
       (table.header[field].count > 0)) {
      xvt_note ("Invalid element count! (%d, %d)\n",
             count,table.header[field].count);
      return -1;
   }

   if (field < 0 || field >= table.nfields)
     return -1;

   row[field].count = count;

   if (row[field].ptr != (void *)NULL) {
      xvt_free(row[field].ptr);
      row[field].ptr = NULL;
   }

   switch ( table.header[field].type ) {
      case 'T':
        len = (int32)max(count,table.header[field].count);
        str = (char *) vpfmalloc( len + 1 );
        row[field].ptr = (char *) vpfmalloc ( len + 1 ) ;
        strcpy( str, value );
        for ( i = strlen(value) ; i < table.header[field].count; i++ )
           str[i] = SPACE ;
        str[len] = '\0';
        memcpy (row[field].ptr, str, (size_t)(len+1));
        if(str != (char *)NULL)
          {xvt_free(str);str = (char *)NULL;}
        break ;

      case 'D':
        row[field].ptr = (date_type *) vpfmalloc (count*sizeof(date_type));
        memcpy (row[field].ptr, value, sizeof (date_type) * (size_t)count);
        break;

      case 'I' :
        row[field].ptr = (int32 *) vpfmalloc (count*sizeof(int32));
        memcpy (row[field].ptr, value, sizeof (int32) * (size_t)count);
        break;

      case 'S' :
        row[field].ptr = (short int *) vpfmalloc (count*sizeof(short int));
        memcpy (row[field].ptr, value, sizeof (short) * (size_t)count);
        break;

      case 'F':
        row[field].ptr = (float *) vpfmalloc (count*sizeof(float));
        memcpy (row[field].ptr, value, sizeof (float) * (size_t)count);
        break;

      case 'R':
        row[field].ptr = (double *) vpfmalloc (count*sizeof(double));
        memcpy (row[field].ptr, value, sizeof (double) * (size_t)count);
        break;

      case 'K':
        row[field].ptr =
          (id_triplet_type *) vpfmalloc ( count*sizeof(id_triplet_type ));
        memcpy (row[field].ptr, value, sizeof(id_triplet_type) *
                                                               (size_t)count);
        break;

      case 'C':
        if (value) {
           row[field].ptr = (coordinate_type*) xvt_zmalloc ((size_t)count *
                                                    sizeof (coordinate_type));
           if (row[field].ptr)
              memcpy (row[field].ptr, value, sizeof (coordinate_type) *
                                                               (size_t)count);
        } else {
           row[field].ptr = NULL;
        }
        break;

      case 'Z':
        if (value) {
          row[field].ptr = (tri_coordinate_type *)
             xvt_zmalloc ((size_t)count * sizeof(tri_coordinate_type));
          if (row[field].ptr)
             memcpy (row[field].ptr, value,
                      sizeof(tri_coordinate_type) * (size_t)count);
        } else {
           row[field].ptr = NULL;
        }
        break;

      case 'B':
        if (value) {
           row[field].ptr = (double_coordinate_type *)
             xvt_zmalloc ((size_t)count * sizeof (double_coordinate_type));
           if (row[field].ptr)
              memcpy (row[field].ptr, value,
                       sizeof (double_coordinate_type) * (size_t)count);
        } else {
           row[field].ptr = NULL;
        }
        break;

      case 'Y':
        if (value) {
           row[field].ptr = (double_tri_coordinate_type *)
             xvt_zmalloc ((size_t)count * sizeof (double_tri_coordinate_type));
           if (row[field].ptr)
              memcpy( row[field].ptr, value,
                      sizeof (double_tri_coordinate_type) * (size_t)count);
        } else {
           row[field].ptr = NULL;
        }
        break;

      default:
        xvt_note ("text2vpf: No such data type < %c > in vpf\n",
                table.header[field].type ) ;  /*DGM*/
        stat = -1;
        break ;
   }

   return stat;
}



/* ========================================================================

   Environmental Systems Research Institute (ESRI) Applications Programming

       Project:                 Conversion from ARC/INFO to VPF
       Original Coding:         Tom Wood        Fall 1990
       Modifications:           David Flinn     January 1991
                                                July 1991
                                JTB             10/91

   The following functions are used when writing from a big-endian machine.
   VPF requires little-endian words, so byte-swapping must be performed.

   ======================================================================== */

/* #ifdef UNIX */

#ifdef PROTO
int32 VpfWrite ( void *from, VpfDataType type, int32 count, FILE *to )
#else
int32 VpfWrite ( from, type, count, to )
void *from;
VpfDataType type;
int32 count;
FILE *to;
#endif
{
  int32 retval = 0;
  int32 i;

  switch ( type ) {
  case VpfChar:
    retval = fwrite (from, sizeof (char), (size_t)count, to);
    break ;
  case VpfShort:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
        short int stemp ,
                *sptr = (short *) from ;
        for ( i=0; i < count; i++, sptr++ ) {
           swap_two ((char*)sptr,(char*)&stemp);
           retval = fwrite ( &stemp, sizeof (short), 1, to ) ;
        }
      } else {
        retval = fwrite (from, sizeof (short), (size_t)count, to);
      }
    }
    break ;
  case VpfInteger:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
         int32 itemp,
           *iptr = (int32 *) from ;
         for ( i=0; i < count; i++, iptr++ ) {
           swap_four ((char*)iptr, (char*)&itemp);
           retval = fwrite ( &itemp, sizeof (int32), 1, to ) ;
         }
      } else {
         retval = fwrite (from, sizeof (int32), (size_t)count, to);
      }
    }
    break ;
  case VpfFloat:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
         float ftemp ,
            *fptr = (float *) from ;
         for ( i=0; i < count; i++, fptr++ ) {
           swap_four ((char*)fptr, (char*)&ftemp);
           retval = fwrite ( &ftemp, sizeof (float), 1, to ) ;
         }
      } else {
         retval = fwrite (from, sizeof (int32), (size_t)count, to);
      }
    }
    break ;
  case VpfDouble:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
         double dtemp ,
             *dptr = (double *) from ;
         for ( i=0; i < count; i++, dptr++ ) {
           swap_eight ((char*)dptr, (char*)&dtemp);
           retval = fwrite ( &dtemp, sizeof (double), 1, to ) ;
         }
      } else {
         retval = fwrite (from, sizeof (double), (size_t)count, to);
      }
    }
    break ;
  case VpfDate: /* only write out 20, not 21 chars */
    retval = fwrite (from, sizeof (date_type) - 1, (size_t)count, to);
    break ;
  case VpfCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
         coordinate_type ctemp ,
                      *cptr = (coordinate_type *) from ;
         for ( i=0; i < count; i++, cptr++ ) {
           swap_four ((char*)&cptr->x, (char*)&ctemp.x);
           swap_four ((char*)&cptr->y, (char*)&ctemp.y);
           retval = fwrite ( &ctemp, sizeof (coordinate_type), 1, to ) ;
         }
      } else {
         retval = fwrite (from, sizeof (coordinate_type), (size_t)count, to);
      }
    }
    break ;
  case VpfDoubleCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
         double_coordinate_type dctemp ,
                             *dcptr = (double_coordinate_type *) from ;
         for ( i=0; i < count; i++, dcptr++ ) {
           swap_eight ((char*)&dcptr->x, (char*)&dctemp.x);
           swap_eight ((char*)&dcptr->y, (char*)&dctemp.y);
           retval = fwrite ( &dctemp, sizeof (double_coordinate_type),
                             1, to ) ;
         }
      } else {
         retval = fwrite (from, sizeof (double_coordinate_type),
                          (size_t)count, to);
      }
    }
    break ;
  case VpfTriCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
        tri_coordinate_type ttemp ,
                          *tptr = (tri_coordinate_type *) from ;
        for ( i=0; i < count; i++, tptr++ ) {
           swap_four ((char*)&tptr->x, (char*)&ttemp.x);
           swap_four ((char*)&tptr->y, (char*)&ttemp.y);
           swap_four ((char*)&tptr->z, (char*)&ttemp.z);
           retval = fwrite ( &ttemp, sizeof (tri_coordinate_type), 1, to ) ;
        }
      } else {
        retval = fwrite (from, sizeof (tri_coordinate_type),
                                                           (size_t)count, to);
      }
    }
    break ;
  case VpfDoubleTriCoordinate:
    {
      if (MACHINE_BYTE_ORDER != STORAGE_BYTE_ORDER) {
        double_tri_coordinate_type dttemp ,
                    *dtptr = (double_tri_coordinate_type *) from ;
        for ( i=0; i < count; i++, dtptr++ ) {
           swap_eight ((char*)&dtptr->x, (char*)&dttemp.x);
           swap_eight ((char*)&dtptr->y, (char*)&dttemp.y);
           swap_eight ((char*)&dtptr->z, (char*)&dttemp.z);
           retval = fwrite ( &dttemp,sizeof (double_tri_coordinate_type),
                             1, to);
        }
      } else {
        retval = fwrite ( from,sizeof (double_tri_coordinate_type),
                         (size_t)count, to);
      }
    }
    break ;
  case VpfNull:
    /* Do Nothing */
    break ;
  default:
    xvt_note ("VpfWrite: error on data type < %s >",(char*) type ) ;  /*DGM*/
    break ;
  }

  return retval;
}

/* #endif */
