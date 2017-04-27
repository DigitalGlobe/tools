/*************************************************************************
 *
 *N  Module VPFREAD.C
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This module contains functions for reading VPF tables.  It, along
 *     with VPFTABLE.C and VPFWRITE.C (and VPFIO.C for UNIX), comprises a
 *     fairly extensive set of functions for handling VPF tables.
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
 *    Barry Michaels   May 1991  Original Version   DOS Turbo C
 *    David Flinn      Jul 1991  Merged with Barry & Mody's code for UNIX
 *    Jim TenBrink     Oct 1991  Split this module off from vpftable and
 *                               merged converter and vpfview branches
 *                               for the functions included here..
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   External Variables:
 *X
 *    None
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.  It includes #ifdefs for
 *    all system dependencies, so that it will work efficiently with
 *    either Turbo C in DOS or (at least) GNU C in UNIX.
 *E
 *
 *************************************************************************/
#ifndef INCL_XVTH
#include <xvt.h>
#endif
#if 0
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#endif

#ifdef _MSDOS
#include <io.h>
#include <dos.h>
#endif

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#ifdef _UNIX
#include <sys/stat.h>
#define   SEEK_SET    0         /* Turbo C fseek value */
#define   SEEK_CUR    1
#endif

#ifndef _MACHINE_
#include "machine.h"
#endif
#ifndef _VPFTABLE_H_
#include "vpftable.h"
#endif

extern int32 STORAGE_BYTE_ORDER;


/*
 * currently being used by the converter routines, not by the vpfview
 * routines
 */
#ifdef PROTO
char * get_line (FILE *fp)
#else
char * get_line (fp)
FILE *fp;
#endif
{
  int32 CurrentChar,   /* This is an int because fgetc returns an int */
      count ,
      NextBlock = 256 ,
      LineAllocation = 0 ;
  char *CurrentLine = (char *) NULL ;

/* This forever loop searches past all lines beginning with #,
   indicating comments. */

  for (;;) {
      CurrentChar = fgetc(fp);
      if ( CurrentChar == COMMENT )             /* skip past comment line */
        for (;CurrentChar != NEW_LINE; ) {
          if (CurrentChar == EOF) return (char *) NULL ;
          CurrentChar = fgetc (fp) ;
        }
      else
        break ;
    }  /* end of forever loop */

  if (CurrentChar == EOF ) return (char *) NULL ;

  for(count = 0; CurrentChar != EOF; CurrentChar = fgetc(fp), count++) {

    /* Allocate space for output line, if needed */

    if (! ( count < LineAllocation )) {
      LineAllocation += NextBlock ;
      if ( CurrentLine )
        CurrentLine = (char*) xvt_realloc (CurrentLine, (size_t)LineAllocation );
      else
        CurrentLine = (char*) xvt_zmalloc ((size_t)LineAllocation);
      if (!CurrentLine) {
#if 0
        printf("get_line: Out of Memory");
#endif

        return (char *) NULL ;
      }
    }
    if ( ( CurrentChar == (int32) LINE_CONTINUE ) ) {
      CurrentChar = fgetc(fp ) ;        /* read character after backslash */
      /* A newline will be ignored and thus skipped over */
      if ( CurrentChar == (int32) SPACE )  /* Assume line continue error */
        while ( fgetc (fp) != (int32) SPACE ) ;
      else if (CurrentChar != (int32) NEW_LINE ) {
        /* copy it if not new line */
        CurrentLine[count++] = (char) LINE_CONTINUE ;
        CurrentLine[count] = (char) CurrentChar ;
      } else
        count -- ;      /* Decrement the counter on a newline character */
    } else if (CurrentChar == (int32) NEW_LINE )     /* We're done */
        break;
    else
      CurrentLine[count] = (char)CurrentChar;

  }  /* end of for count */

  CurrentLine[count] = (char)('\0');  /* terminate string */
  return CurrentLine ;

}



#ifdef PROTO
int32 VpfRead ( void *to, VpfDataType type, int32 count, FILE *from )
#else
int32 VpfRead ( to, type, count, from )
void *to;
VpfDataType type;
int32 count;
FILE *from;
#endif
{
  int32 retval=0, i ;

  switch ( type ) {
  case VpfChar:
    retval = fread ( to, sizeof (char), (size_t)count, from ) ;
    break ;
  case VpfShort:
    {
      short int stemp ,
                *sptr = (short *) to ;
      for ( i=0; i < count; i++ ) {
        retval = fread ( &stemp, sizeof (short), 1, from ) ;
        if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
           swap_two ( (char*)&stemp, (char*)sptr ) ;
        else
           *sptr = stemp;
        sptr++ ;
      }
    }
    break ;
  case VpfInteger:
    {
      if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER) {
        int32 itemp,
          *iptr = (int32 *) to ;
        for ( i=0; i < count; i++ ) {
          retval = fread ( &itemp, sizeof (int32), 1, from ) ;
          swap_four ((char*)&itemp, (char*)iptr ) ;
          iptr++ ;
        }
      } else {
        retval = fread ( to, sizeof (int32), (size_t)count, from ) ;
      }
    }
    break ;
  case VpfFloat:
    {
    float ftemp , *fptr = (float*)to;
    for (i=0; i<count; i++)
       {
       retval = fread (&ftemp, sizeof (float), 1, from);
       if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
       swap_four ((char*)&ftemp, (char*)fptr);
       else
           *fptr = ftemp;
       fptr++;
       }
    }
    break;
  case VpfDouble:
    {
      double dtemp ,
             *dptr = (double *) to ;
      for ( i=0; i < count; i++ ) {
        retval = fread ( &dtemp, sizeof (double), 1, from ) ;
        if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
           swap_eight ((char*)&dtemp, (char*)dptr ) ;
        else
           *dptr = dtemp;
        dptr++ ;
      }
    }
    break ;
  case VpfDate:
    {
      date_type *dp = (date_type *) to ;
      retval = fread(dp, sizeof(date_type)-1, (size_t)count, from);
    }
    break ;
   case VpfCoordinate:
      {
      if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
         {
         coordinate_type ctemp, *cptr = (coordinate_type*) to;
         for (i=0; i<count; i++)
            {
            retval = fread ( &ctemp, sizeof (coordinate_type), 1, from ) ;
            swap_four ((char*)&ctemp.x, (char*)&cptr->x ) ;
            swap_four ((char*)&ctemp.y, (char*)&cptr->y ) ;
            cptr++ ;
            }
         }
      else
         {
         retval = fread (to, sizeof(coordinate_type), (size_t)count, from ) ;
         }
      }
      break ;
   case VpfDoubleCoordinate:
      {
      double_coordinate_type dctemp ,
                             *dcptr = (double_coordinate_type *) to ;
      for (i=0; i<count; i++)
         {
         retval = fread ( &dctemp, sizeof (double_coordinate_type), 1, from ) ;
         if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
            {
            swap_eight ((char*)&dctemp.x, (char*)&dcptr->x ) ;
            swap_eight ((char*)&dctemp.y, (char*)&dcptr->y ) ;
            }
         else
            {
            dcptr->x = dctemp.x;
            dcptr->y = dctemp.y;
            }
         dcptr++ ;
         }
      }
      break ;
   case VpfTriCoordinate:
      {
      if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER)
         {
         tri_coordinate_type temp, *ptr = (tri_coordinate_type*)to;
         for (i=0; i<count; i++)
            {
            retval = fread (&temp, sizeof (tri_coordinate_type), 1, from);
            swap_four ((char*)&temp.x, (char*)&ptr->x);
            swap_four ((char*)&temp.y, (char*)&ptr->y);
            swap_four ((char*)&temp.z, (char*)&ptr->z);
            ptr++;
            }
         }
      else
         retval = fread (to, sizeof (tri_coordinate_type), (size_t)count, from);
      }
      break ;
   case VpfDoubleTriCoordinate:
      {
      double_tri_coordinate_type dttemp ,
                                 *dtptr = (double_tri_coordinate_type *) to ;
      for ( i=0; i < count; i++ ) {
        retval = fread ( &dttemp,sizeof (double_tri_coordinate_type), 1, from);
        if (STORAGE_BYTE_ORDER != MACHINE_BYTE_ORDER) {
           swap_eight ((char*)&dttemp.x, (char*)&dtptr->x ) ;
           swap_eight ((char*)&dttemp.y, (char*)&dtptr->y ) ;
           swap_eight ((char*)&dttemp.z, (char*)&dtptr->z ) ;
        } else {
           dtptr->x = dttemp.x;
           dtptr->y = dttemp.y;
           dtptr->z = dttemp.z;
        }
        dtptr++ ;
      }
      }
      break ;
   case VpfNull:
      /* Do Nothing */
      break ;
   default:
#if 0
      printf ("\nVpfRead: error on data type , %s >", type);
#endif

      break ;
   }   /* end of switch */

   return retval; /* whatever fread returns */
   }

/* #endif */



/*************************************************************************
 *
 *N  index_length
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the length of a specified row from the table
 *     index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row_number <input> == (int32) row number in the table.
 *    table      <input> == (vpf_table_type) VPF table structure.
 *    return    <output> == (int32) length of the table row or 0 on error.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991       Original Version   DOS Turbo C
 *    Dave Flinn       July 1991      UNIX extensions
 *    JTB              10/91          removed aborts()
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
int32 index_length( int32 row_number,
                       vpf_table_type table )
#else
int32 index_length( row_number, table )
int32 row_number;
vpf_table_type table;
#endif
{
   int32   recsize,len=0;
   uint32 ulen;
   int32 pos;

   STORAGE_BYTE_ORDER = table.byte_order;

   if (row_number < 1) row_number = 1;
   if (row_number > table.nrows) row_number = table.nrows;

   switch (table.xstorage) {
      case COMPUTE:
         len = table.reclen;
         break;
      case DISK:
         recsize = sizeof(index_cell);
         fseek( table.xfp, (int32)(row_number*recsize), SEEK_SET );

         if ( ! Read_Vpf_Int(&pos,table.xfp,1) ) {
#if 0
           printf ("\nindex_length: error reading index.") ;
#endif

           len = 0 ;
         }

         if ( ! Read_Vpf_Int(&ulen,table.xfp,1) ) {
#if 0
           printf ("\nindex_length: error reading index.");
#endif
           return 0 ;
         }
         len = ulen;
         break;
      case RAM:
         len = table.index[row_number-1].length;
         break;
      default:
        if ( table.mode == Write && table.nrows != row_number ) {
           /* Just an error check, should never get here in writing */
           xvt_note ("index_length: error trying to access row %d",
                   (int) row_number ) ;
           len = 0;
        }
        break;
   }
   return len;
}

/*************************************************************************
 *
 *N  index_pos
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the position of a specified row from the table
 *     index.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row_number <input> == (int32) row number in the table.
 *    table      <input> == (vpf_table_type) VPF table structure.
 *    return    <output> == (int32) position of the table row
 *                          or zero on error.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991       Original Version   DOS Turbo C
 *    Dave Flinn       July 1991      Updated for UNIX
 *    JTB              10/91          removed aborts()
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
int32 index_pos (int32 row_number, vpf_table_type table)
#else
int32 index_pos (row_number, table)
int32 row_number;
vpf_table_type table;
#endif
{
   int32 recsize;
   uint32 pos = 0;   /* Intergraph solution TR#GX323 */

   STORAGE_BYTE_ORDER = table.byte_order;

   /* ERO: previously clamped row_number to [1, table.nrows] but does not */
   /* seam to be sane, especially if table.nrows = 0 */
   if (row_number < 1 || row_number > table.nrows)
   {
     xvt_note ("index_pos: error trying to access row %d/%d in table %s\n",
                   (int) row_number, table.nrows, table.path ) ;
     return 0;
   }

   switch (table.xstorage) {
      case COMPUTE:
         pos = table.ddlen + ((row_number-1) * table.reclen);
         break;
      case DISK:
         recsize = sizeof(index_cell);
         fseek( table.xfp, (int32)(row_number*recsize), SEEK_SET );
         if ( ! Read_Vpf_Int(&pos,table.xfp,1) ) {
#if 0
           printf ("\nindex_length: error reading index.");
#endif
           pos = 0 ;
         }
         break;
      case RAM:
         pos = table.index[row_number-1].pos;
         break;
      default:
         if ( table.mode == Write && table.nrows != row_number ) {
           /* Just an error check, should never get here in writing */
           xvt_note ("index_pos: error trying to access row %d",
                   (int) row_number ) ;
           pos = 0;
         }
         break;
   }
   return pos;
}


/* Compute the offset from the start of the row to the given field */
#ifdef PROTO
int32 row_offset( int32 field, row_type row, vpf_table_type table)
#else
int32 row_offset( field,  row, table)
int32 field;
row_type row;
vpf_table_type table;
#endif
{
   int32 offset,n,size;
   int32 i;
   id_triplet_type key;
   static int32 keysize[] = {0,sizeof(char),sizeof(short int),sizeof(int32)};

   if (field < 0 || field >= table.nfields) return -1;

   offset = 0L;
   for (i=0;i<field;i++) {
      switch (table.header[i].type) {
         case 'I':
            offset += sizeof(int32)*row[i].count;
            break;
         case 'S':
            offset += sizeof(short int)*row[i].count;
            break;
         case 'L':
         case 'T':
            offset += sizeof(char)*row[i].count;
            break;
         case 'F':
            offset += sizeof(float)*row[i].count;
            break;
         case 'D':
            offset += sizeof(date_type)*row[i].count;
            break;
         case 'K':
            get_table_element(i,row,table,&key,&n);
            size = sizeof(char) +
                   keysize[TYPE0(key.type)] +
                   keysize[TYPE1(key.type)] +
                   keysize[TYPE2(key.type)];
            offset += size*row[i].count;
            break;
         case 'R':
            offset += sizeof(double)*row[i].count;
            break;
         case 'C':
            offset += sizeof(coordinate_type)*row[i].count;
            break;
         case 'B':
            offset += sizeof(double_coordinate_type)*row[i].count;
            break;
         case 'Z':
            offset += sizeof(tri_coordinate_type)*row[i].count;
            break;
         case 'Y':
            offset += sizeof(double_tri_coordinate_type)*row[i].count;
            break;
      }
   }
   return offset;
}

/*************************************************************************
 *
 *N  read_key
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads an id triplet key from a VPF table.
 *     The table must be open for read.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table    <input>  == (vpf_table_type) VPF table.
 *    read_key <output> == (id_triplet_type) id triplet key.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991       Original Version   DOS Turbo C
 *    Dave Flinn       July 1991      Updated for UNIX
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
id_triplet_type read_key( vpf_table_type table )
#else
id_triplet_type read_key( table )
vpf_table_type table;
#endif
{
   id_triplet_type key;
   unsigned char ucval;
   unsigned short int uival;

   STORAGE_BYTE_ORDER = table.byte_order;

   key.id = 0L;
   key.tile = 0L;
   key.exid = 0L;

   /* just doing this to be consistent */
   Read_Vpf_Char (&(key.type),table.fp,1);

   switch (TYPE0(key.type)) {
      case 0:
         break;
      case 1:

         Read_Vpf_Char (&ucval, table.fp, 1 ) ;
         key.id = (int32)ucval;
         break;
      case 2:

         Read_Vpf_Short (&uival, table.fp, 1 ) ;
         key.id = (int32)uival;
         break;
      case 3:

         Read_Vpf_Int (&(key.id), table.fp, 1 ) ;
         break;
   }
   switch (TYPE1(key.type)) {
   case 0:
     break;
   case 1:
     Read_Vpf_Char (&ucval, table.fp, 1 ) ;
     key.tile = (int32)ucval;
     break;
   case 2:
     Read_Vpf_Short (&uival, table.fp, 1 ) ;
     key.tile = (int32)uival;
     break;
   case 3:
     Read_Vpf_Int (&(key.tile), table.fp, 1 ) ;
     break;
   }

   switch (TYPE2(key.type)) {
   case 0:
     break;
   case 1:
     Read_Vpf_Char (&ucval, table.fp, 1 ) ;
     key.exid = (int32)ucval;
     break;
   case 2:
     Read_Vpf_Short (&uival, table.fp, 1 ) ;
     key.exid = (int32)uival;
     break;
   case 3:
     Read_Vpf_Int (&(key.exid), table.fp, 1 ) ;
     break;
   }

   return key;
 }

/*************************************************************************
 *
 *N  read_next_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads the next row of the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) the next row in the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991       Original Version   DOS Turbo C
 *    Dave Flinn       July 1991      Updated for UNIX
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
 *    void *vpfmalloc()
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
row_type read_next_row( vpf_table_type table )
#else
row_type read_next_row( table )
vpf_table_type table;
#endif
{
  register int32 i,j;
  int32      status;
  char     *tptr;
  int32 size,count;
  row_type row;
  id_triplet_type * keys;
  coordinate_type dummycoord;
  int32 id = -1;

  if (feof(table.fp))
    {
      return NULL;
    }

  STORAGE_BYTE_ORDER = table.byte_order;

  row = (row_type)xvt_zmalloc (((size_t)table.nfields+1) * sizeof(column_type));
  if( row == NULL )
  {
      xvt_note ("Out of memory in read_next_row()\n");
      return NULL;
  }

  for (i=0;i<table.nfields;i++) row[i].ptr = NULL;

  for (i=0;i<table.nfields;i++)
    {
      if (table.header[i].count < 0)
	{
	  /* Read the count subfield for variable length fields */
	  Read_Vpf_Int (&count, table.fp, 1);

	  if ((unsigned int) count > 2000000) {
            char szMessage[256];
            if( strlen(table.path) < 128 )
                sprintf(szMessage, "Repeat count for field %d of record %d of table %s is %d\n",
                        i, id, table.path, (unsigned int)count);
            else
                sprintf(szMessage, "Repeat count for field %d of record %d of table %s is %d\n",
                        i, id, table.path + strlen(table.path) - 128, (unsigned int)count);
            xvt_note ("%s", szMessage);
	    free_row ( row, table ) ;
	    return (row_type) NULL;
	  }	 
	}
      else
	{
	  count = table.header[i].count;
	}
      row[i].count = count;      

      status = 0;
      switch (table.header[i].type) {
      case 'T':
      case 'L':
	if (count == 1) {
	  row[i].ptr = (char *)xvt_zmalloc(sizeof(char));
          if( row[i].ptr == NULL )
          {
              xvt_note ("Out of memory in read_next_row()\n");
              free_row ( row, table ) ;
              return (row_type) NULL;
          }
	  Read_Vpf_Char(row[i].ptr, table.fp, 1) ;
	} else {
	  size = count*sizeof(char);
	  row[i].ptr = (char*) xvt_zmalloc((size_t)size+2);
	  tptr = (char*)xvt_zmalloc ((size_t)size+2);
          if( row[i].ptr == NULL || tptr == NULL )
          {
              xvt_note ("Out of memory in read_next_row()\n");
              xvt_free(tptr);
              free_row ( row, table ) ;
              return (row_type) NULL;
          }
	  Read_Vpf_Char(tptr,table.fp,count) ;
	  tptr[count] = '\0';
	  strcpy(row[i].ptr,tptr);
	  xvt_free(tptr);
          tptr = (char *)NULL;
	}
	break;
      case 'I':
	row[i].ptr = (int32*)xvt_zmalloc((size_t)count * sizeof (int32));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_Int (row[i].ptr, table.fp, count ) ;
        if( i == 0 && count == 1 )
            id = ((int32*)row[i].ptr)[0];
	break;
      case 'S':
	row[i].ptr = (short*)xvt_zmalloc ((size_t)count * sizeof (short));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_Short (row[i].ptr, table.fp, count ) ;
	break;
      case 'F':
	row[i].ptr = (float*)xvt_zmalloc ((size_t)count * sizeof (float));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_Float (row[i].ptr, table.fp, count ) ;
	break;
      case 'R':
	row[i].ptr = (double*)xvt_zmalloc ((size_t)count * sizeof (double));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_Double (row[i].ptr, table.fp, count ) ;
	break;
      case 'D':
	row[i].ptr = (date_type*)xvt_zmalloc ((size_t)count *
					      sizeof (date_type));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_Date (row[i].ptr, table.fp, count ) ;
	break;
      case 'C':
	/* Coordinate strings may be quite large.          */
	/* Allow for null coordinate string pointer if     */
	/* not enough memory that can be handled one       */
	/* coordinate at a time in higher level functions. */
	row[i].ptr = (coordinate_type*)xvt_zmalloc ((size_t)count *
						    sizeof(coordinate_type));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        if (row[i].ptr)
	  Read_Vpf_Coordinate(row[i].ptr,table.fp,count);
	else
	  for (j=0;j<count;j++)
	    Read_Vpf_Coordinate(&dummycoord,table.fp,1);
	break;
      case 'Z':
	row[i].ptr = (tri_coordinate_type*)xvt_zmalloc ((size_t)count *
							sizeof (tri_coordinate_type));
        if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
	Read_Vpf_CoordinateZ(row[i].ptr,table.fp,count);
	break;
      case 'B':
	row[i].ptr = (double_coordinate_type*)xvt_zmalloc ((size_t)count *
							   sizeof (double_coordinate_type));
	if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
        Read_Vpf_DoubleCoordinate(row[i].ptr,table.fp,count);
	break;
      case 'Y':
	row[i].ptr = (double_tri_coordinate_type*)xvt_zmalloc ((size_t)count *
							       sizeof (double_tri_coordinate_type));
        if( row[i].ptr == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
	Read_Vpf_DoubleCoordinateZ(row[i].ptr,table.fp,count);
	break;
      case 'K':   /* ID Triplet */
	row[i].ptr = (id_triplet_type*)xvt_zmalloc ((size_t)count *
                                                    sizeof (id_triplet_type));
	keys = (id_triplet_type*)xvt_zmalloc ((size_t)count *
					      sizeof (id_triplet_type));
        if( row[i].ptr == NULL || keys == NULL )
        {
            xvt_note ("Out of memory in read_next_row()\n");
            xvt_free ((char*)keys);
            free_row ( row, table ) ;
            return (row_type) NULL;
        }
	for (j=0;j<count;j++) {
	  keys[j] = read_key(table);
	}
	memcpy (row[i].ptr, keys, (size_t)count *
		sizeof(id_triplet_type));
	xvt_free ((char*)keys);
        keys = (id_triplet_type*)NULL;
	break;
      case 'X':
	row[i].ptr = NULL;
	break;
      default:
	xvt_note ("%s%s >>> read_next_row: no such type < %c >",
		  table.path,table.name,table.header[i].type ) ;
	status = 1;
	break ;
      }   /* end of switch */
      if (status == 1) {
	free_row ( row, table ) ;
	return (row_type) NULL;
      }
    }
  return row;
}

/*************************************************************************
 *
 *N  rowcpy
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns a copy of the specified row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    origrow    <input> == (row_type) row to copy.
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) copy of the row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
 *    void *vpfmalloc()                            VPFMISC.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
row_type rowcpy( row_type origrow,
                 vpf_table_type table )
#else
row_type rowcpy( origrow, table )
row_type origrow;
vpf_table_type table;
#endif
{
   register int32 i;
   int32      count;
   int32 size;
   row_type row;

   row = (row_type)xvt_zmalloc ((size_t)table.nfields * sizeof (column_type));
   for (i=0;i<table.nfields;i++) {
      count = origrow[i].count;
      row[i].count = count;
      switch (table.header[i].type) {
         case 'L':
         case 'T':
            if (count==1) {
               row[i].ptr = (char *)xvt_zmalloc(1);
               memcpy(row[i].ptr,origrow[i].ptr,sizeof(char));
            } else {
               size = count*sizeof(char);
               row[i].ptr = (char*)xvt_zmalloc ((size_t)size + 1);
               strcpy(row[i].ptr,origrow[i].ptr);
            }
            break;
         case 'I':
            size = count*sizeof(int32);
            row[i].ptr = (int32*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'S':
            size = count*sizeof(short int);
            row[i].ptr = (short*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'F':
            size = count*sizeof(float);
            row[i].ptr = (float*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'R':
            size = count*sizeof(double);
            row[i].ptr = (double*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'C':
            size = count*sizeof(coordinate_type);
            row[i].ptr = (coordinate_type*)xvt_zmalloc ((size_t)size);
            if (row[i].ptr && origrow[i].ptr)
               memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            else
               row[i].ptr = NULL;
            break;
         case 'Z':
            size = count*sizeof(tri_coordinate_type);
            row[i].ptr = (tri_coordinate_type*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'B':
            size = count*sizeof(double_coordinate_type);
            row[i].ptr = (double_coordinate_type*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'Y':
            size = count*sizeof(double_tri_coordinate_type);
            row[i].ptr = (double_tri_coordinate_type*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'D':  /* Date */
            size = count*sizeof(date_type);
            row[i].ptr = (date_type*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'K':  /* ID Triplet */
            size = count*sizeof(id_triplet_type);
            row[i].ptr = (id_triplet_type*)xvt_zmalloc ((size_t)size);
            memcpy (row[i].ptr, origrow[i].ptr, (size_t)size);
            break;
         case 'X':
            row[i].ptr = NULL;
            break;
          default:
            xvt_note ("row_cpy: error in data type < %c >",
                     table.header[i].type ) ;
            abort () ;
            break ;
          }     /* end of switch */
    }           /* end of table.nfields */
   return row;
}

/*************************************************************************
 *
 *N  read_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function reads a specified row from the table.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row_number <input> == (int32) row number.
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) row that was read in.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991      DOS Turbo C
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
 *    int32 index_pos()                             VPFTABLE.C
 *    row_type read_next_row()                         VPFTABLE.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
row_type read_row( int32 row_number,
                   vpf_table_type table )
#else
row_type read_row( row_number, table )
int32 row_number;
vpf_table_type table;
#endif
{
   int32 fpos;

   /* 
      Intergraph solution DAP 12/10/97 TR323 Don't try to read
      if table is empty. 
      */

   if (table.fp != NULL) {
     fpos = index_pos(row_number,table);
     if ( fpos != (long)NULL) {
       fseek(table.fp,fpos,SEEK_SET);
       return read_next_row(table);
     } else {
       return NULL;
     }
   } else {
     return NULL;
   }
}

/*************************************************************************
 *
 *N  free_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function frees the memory that was dynamically allocated for the
 *     specified row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row   <input> == (row_type) row to be freed.
 *    table <input> == (vpf_table_type) vpf table structure.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
void free_row (row_type row, vpf_table_type table)
#else
void free_row ( row,  table)
row_type row;
vpf_table_type table;
#endif
   {
   register int32 i;

   if (!row)
      return;
   for (i=0; i<table.nfields; i++)
      if (row[i].ptr != (void*)NULL)
         {xvt_free (row[i].ptr);
         row[i].ptr = (void*)NULL;
         }
   if (row != (row_type)NULL)
      {
      xvt_free ((char*)row);
      row = (row_type)NULL;
      }
   }

/*************************************************************************
 *
 *N  get_row
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the specified row of the table.  If the table
 *     is stored in memory, the row is copied from there.  If it is on disk,
 *     it is read and returned.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    row_number <input> == (int32) row number in range [1 .. table.nrows].
 *    table      <input> == (vpf_table_type) vpf table structure.
 *    return    <output> == (row_type) returned row.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991      DOS Turbo C
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
 *    row_type rowcpy                VPFREAD.C
 *    row_type read_row              VPFREAD.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
row_type get_row( int32 row_number,
                  vpf_table_type table )
#else
row_type get_row( row_number, table )
int32 row_number;
vpf_table_type table;
#endif
{
   row_type row;

   row_number = max(min(row_number, table.nrows), 1);

   if (table.storage == RAM) {
      row = rowcpy(table.row[row_number-1],table);
      return row;
   } else {
      return read_row( row_number, table );
   }
}

/*************************************************************************
 *
 *N  table_pos
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the column offset of the specified field name
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    field_name <input> == (char *) field name.
 *    table      <input> == (vpf_table_type) VPF table structure.
 *    table_pos  <output> == (int32) returned column number.
 *                          UNIX returns -1 if not exists
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
int32  table_pos (char * field_name, vpf_table_type table)
#else
int32  table_pos (field_name, table)
char * field_name; 
vpf_table_type table;
#endif
   {
   register int32 i;
   int32 col;

   col = -1;
   for (i=0; i<table.nfields; i++)
      {
#ifdef _WINDOWS
	if (strcmpi (field_name, table.header[i].name) == 0)
#else
	if (strcasecmp (field_name, table.header[i].name) == 0)
#endif /* mb */
         {
         col = i;
         break;
         }
      }
   return col;
   }

/*************************************************************************
 *
 *N  get_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the element in the given row in the given
 *     column.  If the element is a single element (count=1), the value
 *     is passed back via the void pointer *value; otherwise, an array
 *     is allocated and passed back as the return value.
 *     NOTE: If an array is allocated in this function, it should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    field_number <input> == (int32) field column number.
 *    row          <input> == (row_type) vpf table row.
 *    table        <input> == (vpf_table_type) VPF table structure.
 *    value       <output> == (void *) pointer to a single element value.
 *    count       <output> == (int32 *) pointer to the array size for a
 *                                    multiple element value.
 *    return      <output> == (void *) returned multiple element value.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991             DOS Turbo C
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
 *    void *vpfmalloc()
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
void *get_table_element( int32 field_number,
                         row_type row,
                         vpf_table_type table,
                         void *value,
                         int32  *count )
#else
void *get_table_element( field_number, row, table, value, count )
int32 field_number;
row_type row;
vpf_table_type table;
void *value;
int32  *count;
#endif
{
   int32   col;
   char     * tptr;
   void     * retvalue;
#ifdef HAVE_ICONV
   static int do_iconv = -1;
   static iconv_t iconvd = (iconv_t)-1;

   if (do_iconv == -1)
   {
     do_iconv = getenv("CONVERT_OGDI_TXT_TO_UTF8") != NULL;
     if (do_iconv)
     {
       iconvd = iconv_open("UTF-8", "ISO-8859-1");
     }
   }
#endif

   retvalue = NULL;
   col = field_number;
   if ((col < 0) || (col >= table.nfields)) {
      xvt_note ("GET_TABLE_ELEMENT: Invalid field number %d\n",
              (int) field_number);
      return NULL;
   }

   if (!row) return NULL;

   switch (table.header[col].type) {
      case 'X':
         retvalue = NULL;
         break;
      case 'L':
      case 'T':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(char));
         } else {
            int i, ascii = 1;
            retvalue = (char*)xvt_zmalloc (((size_t)2 * row[col].count + 1) *
                                                               sizeof (char));
            tptr = (char*)xvt_zmalloc (((size_t)row[col].count + 1) *
                                                               sizeof (char));
            memcpy (tptr, row[col].ptr, (size_t)row[col].count *
                                                               sizeof (char));
            tptr[row[col].count] = '\0';
            for(i=0;tptr[i]!=0;i++)
            {
              if ((unsigned char)tptr[i] >= 128)
              {
                ascii = 0;
                if (table.header[col].type == 'T')
                {
                  //fprintf(stderr, "a T column contains non-ASCII text\n");
                }
                break;
              }
            }
#ifndef HAVE_ICONV
            strcpy((char *)retvalue,tptr);
#else
            if (ascii == 1 || iconvd == (iconv_t)-1)
              strcpy((char *)retvalue,tptr);
            else
            {
              size_t nin = row[col].count;
              size_t nout = 2 * row[col].count;
              void* saved_retvalue = retvalue;
              char* saved_tptr = tptr;
              char** outbuf = (char**)&retvalue;

              size_t ret = iconv(iconvd, &tptr, &nin, outbuf, &nout);
              tptr = saved_tptr;
              retvalue = saved_retvalue;
              row[col].count = strlen(retvalue);
              if (ret == (size_t)-1)
              {
                fprintf(stderr, "Can't convert '%s' to UTF-8. Truncating to '%s'\n", tptr, (char*)retvalue);
              }
              else
              {
                //fprintf(stderr, "UTF-8 text : '%s'\n", (char*)retvalue);
              }
            }
#endif
            if(tptr != (char *)NULL)
              {xvt_free(tptr);tptr = (char *)NULL;}
         }
         break;
      case 'I':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(int32));
         } else {
            retvalue = (int32*)xvt_zmalloc ((size_t)row[col].count *
                                                            sizeof(int32));
            memcpy (retvalue, row[col].ptr,(size_t) row[col].count *
                                                            sizeof(int32));
         }
         break;
      case 'S':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(short int));
         } else {
            retvalue = (short*)xvt_zmalloc ((size_t)row[col].count *
                                                           sizeof(short int));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                           sizeof(short int));
         }
         break;
      case 'F':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(float));
         } else {
            retvalue = (float*)xvt_zmalloc ((size_t)row[col].count *
                                                               sizeof(float));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                               sizeof(float));
         }
         break;
      case 'R':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(double));
         } else {
            retvalue = (double*)xvt_zmalloc ((size_t)row[col].count *
                                                             sizeof (double));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                              sizeof(double));
         }
         break;
      case 'C':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(coordinate_type));
         } else {
            if (row[col].ptr) {
               retvalue = (coordinate_type*)xvt_zmalloc ((size_t)row[col].count *
                                                    sizeof (coordinate_type));
               if (retvalue)
                  memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                     sizeof(coordinate_type));
            } else {
               retvalue = NULL;
            }
         }
         break;
      case 'Z':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(tri_coordinate_type));
         } else {
            retvalue = (tri_coordinate_type*)xvt_zmalloc ((size_t)row[col].count *
                                                sizeof (tri_coordinate_type));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                 sizeof(tri_coordinate_type));
         }
         break;
      case 'B':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(double_coordinate_type));
         } else {
            retvalue = (double_coordinate_type*)xvt_zmalloc
                   ((size_t)row[col].count * sizeof (double_coordinate_type));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                              sizeof(double_coordinate_type));
         }
         break;
      case 'Y':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(double_tri_coordinate_type));
         } else {
            retvalue = (double_tri_coordinate_type*)xvt_zmalloc
               ((size_t)row[col].count * sizeof (double_tri_coordinate_type));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                          sizeof(double_tri_coordinate_type));
         }
         break ;
      case 'D':
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(date_type));
         } else {
            retvalue = (date_type*)xvt_zmalloc ((size_t)row[col].count *
                                                          sizeof (date_type));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                           sizeof(date_type));
         }
         break;
      case 'K':  /* ID Triplet */
         if (table.header[col].count == 1) {
            memcpy(value,row[col].ptr,sizeof(id_triplet_type));
         } else {
            retvalue = (id_triplet_type*)xvt_zmalloc ((size_t)row[col].count *
                                                    sizeof (id_triplet_type));
            memcpy (retvalue, row[col].ptr, (size_t)row[col].count *
                                                     sizeof(id_triplet_type));
         }
         break;
   }
   *count = row[col].count;

   return retvalue;
}

/*************************************************************************
 *
 *N  named_table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the element in the specified row in the column
 *     matching the given field name.  If the element is a single element
 *     (count=1), the value is passed back via the void pointer *value;
 *     otherwise, an array is allocated and passed back as the return value.
 *     NOTE: If an array is allocated in this function, it should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    field_name <input> == (char *) field name.
 *    row_number <input> == (int32) row_number.
 *    table      <input> == (vpf_table_type) VPF table structure.
 *    value     <output> == (void *) pointer to a single element value.
 *    count     <output> == (int32 *) pointer to the array size for a multiple
 *                                  element value.
 *    return    <output> == (void *) returned multiple element value.
 *                          or NULL if field_name could not be found
 *                          as a column
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
 *    void *vpfmalloc()                    VPFREAD.C
 *    row_type get_row()                   VPFREAD.C
 *    void free_row()                      VPFREAD.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
void *named_table_element( char * field_name,
                           int32         row_number,
                           vpf_table_type   table,
                           void           * value,
                           int32       * count )
#else
void *named_table_element( field_name, row_number, table, value, count )
char *      field_name;
int32    row_number;
vpf_table_type   table;
void           * value;
int32       * count;
#endif
{
   int32     col;
   row_type     row;
   void       * retvalue;

   col = table_pos(field_name, table);

   if (col < 0) {
      xvt_note ("%s: Invalid field name <%s>\n",table.name,field_name);
      return NULL;
   }
   row = get_row(row_number,table);

   retvalue = get_table_element( col, row, table, value, count );

   free_row(row, table);

   return retvalue;
}

/*************************************************************************
 *
 *N  table_element
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function returns the element in the specified row in the column
 *     matching the given field number.  If the element is a single element
 *     (count=1), the value is passed back via the void pointer *value;
 *     otherwise, an array is allocated and passed back as the return value.
 *     NOTE: If an array is allocated in this function, it should be freed
 *     when no longer needed.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    field_number <input> == (int32) field number (offset from
 *                                   first field in table).
 *    row_number <input> == (int32) row_number.
 *    table      <input> == (vpf_table_type) VPF table structure.
 *    value     <output> == (void *) pointer to a single element value.
 *    count     <output> == (int32 *) pointer to the array size for a multiple
 *                                  element value.
 *    return    <output> == (void *) returned multiple element value or
 *                          NULL of the field number is invalid
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   May 1991                     DOS Turbo C
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
 *    row_type get_row()                   VPFREAD.C
 *    void free_row()                      VPFREAD.C
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Portability:
 *O
 *    This module should be ANSI C compatible.
 *E
 *************************************************************************/
#ifdef PROTO
void *table_element( int32         field_number,
                     int32         row_number,
                     vpf_table_type   table,
                     void           * value,
                     int32       * count )
#else
void *table_element( field_number, row_number, table, value, count )
int32         field_number;
int32         row_number;
vpf_table_type   table;
void           * value;
int32       * count;
#endif
{
   row_type    row;
   void      * retvalue;

   row      = get_row(row_number, table);
   retvalue = get_table_element(field_number, row, table, value, count);
   free_row(row,table);

   return retvalue;
}
