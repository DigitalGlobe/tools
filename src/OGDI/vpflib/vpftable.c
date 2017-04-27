/*****************************************************************************/
/*                                                                           */
/*   VPFTABLE.C                                                              */
/*                                                                           */
/*    This module contains functions to open, close and parse VPF relational */
/*    tables.  VPF tables are defined as being a set of rows and columns.    */
/*    Each column may contain a single value, a fixed array of values,       */
/*    or a variable number of values.                                        */
/*                                                                           */
/*   FUNCTIONS:                                                              */
/*      quiet_nan                                                            */
/*      cpy_del                                                              */
/*      parse_get_string                                                     */
/*      parse_get_char                                                       */
/*      parse_get_number                                                     */
/*      swap_two                                                             */
/*      swap_four                                                            */
/*      swap_eight                                                           */
/*      parse_data_def                                                       */
/*      vpf_nullify_table                                                    */
/*      vpf_open_check                                                       */
/*      vpf_open_table                                                       */
/*      vpf_close_table                                                      */
/*      is_vpf_table                                                         */
/*      is_vpf_null_float                                                    */
/*      is_vpf_null_double                                                   */
/*      format_date                                                          */
/*****************************************************************************/

#ifndef INCL_XVTH
#include <xvt.h>
#endif

#ifdef _MAC
#include <values.h>
#endif

#ifdef _MSDOS
#include <io.h>
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

/*
 * per heap block byte overhead
 */
#define HEAP_OVERHEAD 4

/* Global variable shared by VPFTABLE.C, VPFREAD.C, and VPFWRITE.C  */
/* The above modules are logically one, only separated by file size */
/* constraints. */

int32 STORAGE_BYTE_ORDER = LEAST_SIGNIFICANT;  /* default */



/* Return floating point Not a Number (for NULL values) */

#ifdef PROTO
double quiet_nan( int32 unused )
#else
     double quiet_nan( unused )
     int32 unused;
#endif
{
  static char nanstr[8] = {-1,-1,-1,-1,-1,-1,-1,127};
  double n;
  memcpy((char *) &n,&nanstr[0],sizeof(n));
  if (unused) return n;
  return n;
}

#ifdef PROTO
float float_quiet_nan( int32 unused )
#else
     float float_quiet_nan( unused )
     int32 unused;
#endif
{
  static char nanstr[4] = {-1,-1,-1,127};
  float n;
  memcpy((char *) &n,&nanstr[0],sizeof(n));
  if (unused) return n;
  return n;
}



/***************************************************************/
/*   cpy_del                                                   */
/*     get string until delimeter                              */
/***************************************************************/

#ifdef PROTO
static char *cpy_del(char *src, char delimiter, int32 *ind )
#else
     static char *cpy_del(src, delimiter, ind )
     char *src;
     char delimiter;
     int32 *ind;
#endif
{
  int32 i = 0L, skipchar = 0L;
  char *temp = (char *)NULL, *tempstr = (char *)NULL;
  size_t size = 0;

  /* remove all blanks ahead of good data */

  while ( *(src + skipchar) == SPACE || *(src + skipchar) == TAB )
    skipchar++ ;

  temp = (char *)(src + skipchar);

  /* If the first character is a COMMENT, goto LINE_CONTINUE */

  if ( *temp == (char)COMMENT ) {
    while ( *temp != (char)LINE_CONTINUE && *temp != (char)END_OF_FIELD && *temp != '\0'){
      temp++ ;
      skipchar ++ ;
    }
    skipchar++ ;
    temp++ ;			/* skip past LC, EOF, or NULL */
  }

  /* Start with temporary string value */
  size = (size_t)strlen (temp);
  tempstr = (char*)xvt_zmalloc ((size_t)(size + 10)*sizeof(char));

  if ( *temp == '"' ) {		/* If field is quoted, do no error checks */

    temp++ ;			/* skip past quote character */
    skipchar++ ;		/* update the position pointer */

    for ( i=0 ; *temp != '\0'; temp++,i++) {
      if ( *temp == (char)LINE_CONTINUE || *temp == (char)TAB ) {
        temp++ ;
        skipchar++ ;
      } else if ( *temp == '"' )
        break ;
      /* Now copy the char into the output string */
      *(tempstr + i) = *temp ;
    }
    *(tempstr + i) = '\0' ;	/* terminate string */
    *ind += ( i + skipchar + 2L) ; /* Increment position locate past */
    return tempstr ;		/* quote and semicolon */
  }

  /* search for delimiter to end, or end of string */

  i=0L ;			/* initialize */

  if ( *temp != (char)END_OF_FIELD ) { /* backward compatability check */

    for ( i=0L; *temp != '\0';temp++,i++){ /* Stop on NULL*/

      if ( ( *temp == (char)LINE_CONTINUE && *(temp+1) == '\n') ||  *temp == (char)TAB ) {
        temp++ ;
        skipchar++ ;
      } else if ( *temp == delimiter )
        break ;			/* break for delimiter  */
      /* Now copy the char into the output string */
      *(tempstr + i) = *temp ;
    }
    /* Eat the delimiter from ind also */
    *ind += ( i + skipchar + 1L) ; /* Increment position locate */
  }
  *(tempstr + i) = '\0' ;	/* terminate string */
  return tempstr;
}





/* parse the next string token from the input string */
#ifdef PROTO
char *parse_get_string(int32 *ind, char *src,char delimeter )
#else
     char *parse_get_string( ind, src, delimeter )
     int32 *ind;
     char *src;
     char delimeter;
#endif
{
  char *temp = (char *)NULL;
  temp  = cpy_del((src + (*ind)),delimeter, ind);
  if( ! strcmp ( temp, (char *)TEXT_NULL ))
    strcpy ( temp, "" ) ;
  return temp;
}




/* parse the next character from the input string */
#ifdef PROTO
char parse_get_char(int32 *ind, char *src)
#else
     char parse_get_char( ind, src)
     int32 *ind;
     char *src;
#endif
{
  char temp;
  while ( *(src + (*ind)) == (char)SPACE || *(src + (*ind)) == (char)TAB )
    (*ind)++ ;
  temp  = *(src + (*ind));
  *ind += 2L;
  return temp;
}




/* parse the next numeric token from the input string */
#ifdef PROTO
int32 parse_get_number(int32 *ind, char *src,char delimeter)
#else
     int32 parse_get_number( ind, src, delimeter)
     int32 *ind;
     char *src;
     char delimeter;
#endif
{
  char *temp = (char *)NULL;
  int32  num = 0L;

  temp = cpy_del((src + (*ind)),delimeter, ind);
  if (strchr(temp, (int)VARIABLE_COUNT ) == NULL)
    num = (int32)atoi(temp);	/****should this be atol ?****/
  else
    num = -1L;
  if(temp != (char *)NULL) {
    xvt_free(temp);
    temp = (char *)NULL;
  }
  return num;
}



/*****************************************************************************/
/* swap_two                                                                  */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function performs a byte swap for a two-byte numeric field.      */
/*     This may be necessary if the data is stored in the opposite           */
/*     order of significance than the host platform.  Both parameters        */
/*     should point to a two-byte data element.                              */
/*                                                                           */
/*   Parameters:                                                             */
/*    in    <input> == (char *) pointer to the input value.                  */
/*    out  <output> == (char *) pointer to the returned swapped value.       */
/*****************************************************************************/
#ifdef PROTO
void swap_two ( char *in, char *out )
#else
     void swap_two ( in, out )
     char *in;
     char *out ;
#endif
{
  out[0] = in[1] ;
  out[1] = in[0] ;
}



/*****************************************************************************/
/* swap_four                                                                 */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function performs a byte swap for a four-byte numeric field.     */
/*     This may be necessary if the data is stored in the opposite           */
/*     order of significance than the host platform.  Both parameters        */
/*     should point to a four-byte data element.                             */
/*                                                                           */
/*   Parameters:                                                             */
/*    in    <input> == (char *) pointer to the input value.                  */
/*    out  <output> == (char *) pointer to the returned swapped value.       */
/*****************************************************************************/
#ifdef PROTO
void swap_four (char *in, char *out)
#else
     void swap_four (in, out)
     char *in;
     char *out;
#endif
{
  out[0] = in[3];
  out[1] = in[2];
  out[2] = in[1];
  out[3] = in[0];
  return;
}



/*****************************************************************************/
/* swap_eight                                                                */
/*                                                                           */
/*   Purpose:                                                                */
/*     This function performs a byte swap for an eight-byte numeric field.   */
/*     This may be necessary if the data is stored in the opposite           */
/*     order of significance than the host platform.  Both parameters        */
/*     should point to an eight-byte data element.                           */
/*                                                                           */
/*   Parameters:                                                             */
/*    in    <input> == (char *) pointer to the input value.                  */
/*    out  <output> == (char *) pointer to the returned swapped value.       */
/*****************************************************************************/
#ifdef PROTO
void swap_eight (char *in, char *out)
#else
     void swap_eight (in, out)
     char *in;
     char *out;
#endif
{
  out[0] = in[7];
  out[1] = in[6];
  out[2] = in[5];
  out[3] = in[4];
  out[4] = in[3];
  out[5] = in[2];
  out[6] = in[1];
  out[7] = in[0];
  return;
}



/****************************************************************************/
/* PARSE_DATA_DEF                                                           */
/*                                                                          */
/*   This function parses a table's data definition and creates a header    */
/*   in memory that is associated with the table.                           */
/*                                                                          */
/*   Parameters:                                                            */
/*      table <inout> == (vpf_table_type *) vpf table structure.            */
/*      ddlen <input> == (int32) length of the table's data definition.  */
/*                                                                          */
/*      return value is the record length if all items are fixed length, or */
/*      -1 if the record contains variable length items                     */
/****************************************************************************/
#ifdef PROTO
int32 parse_data_def (vpf_table_type *table)
#else
     int32 parse_data_def (table)
     vpf_table_type *table;
#endif
{
  register int32 n,i;
  int32 p, k;
  char *buf,*des,*nar,*vdt, *tdx, *doc, byte ; /*temporary storage */
  char end_of_rec;
  int32 status;
  int32 ddlen;
  int32 reclen = 0;

  if (table->mode == Read)
    {
      ogdi_fread ((void*)&ddlen, sizeof(ddlen), 1, table->fp);

      /* Check the next byte to see if the byte order is specified */
      ogdi_fread (&byte, 1, 1, table->fp);
      p = 0;
      table->byte_order = LEAST_SIGNIFICANT; /* default */
      switch (toupper (byte))
	{
	case 'L':
	  p++;
	  break;
	case 'M':
	  table->byte_order = MOST_SIGNIFICANT;
	  p++;
	  break;
	}
      if (MACHINE_BYTE_ORDER != table->byte_order)
	{	  
	  k = ddlen;
	  swap_four ((char*)&k, (char*)&ddlen);
	}
      STORAGE_BYTE_ORDER = table->byte_order;

      if (ddlen < 0)
	{
	  xvt_note ("parse_data_def: Bad VPF file.");
	  return (0);
	}

      /* header without first 4 bytes */
      table->ddlen = ddlen + sizeof (int32);

      buf = (char*)xvt_zmalloc ((size_t)ddlen + 3);
      if (buf == NULL)
	{
	  xvt_note ("Parse_data_definition: malloc failed.");
	  return (0);
	}

      buf[0] = byte;		/* already have the first byte of the buffer */
      Read_Vpf_Char(&buf[1],table->fp,ddlen-1) ;
    }
  else
    {
      table->ddlen = strlen (table->defstr);
      ddlen = table->ddlen;

      buf = (char*)xvt_zmalloc((size_t)ddlen + 3);
      if (buf == NULL)
	{
	  xvt_note ("Parse_data_definition: Malloc failed.");
	  return (0);
	}

      strncpy (buf, table->defstr, (size_t)ddlen);
      p = 0;
      table->byte_order = LEAST_SIGNIFICANT; /* default */
      byte = buf[0];
      switch (toupper(byte))
	{
	case 'L':
	  p++;
	  break;
	case 'M':
	  table->byte_order = MOST_SIGNIFICANT;
	  p++;
	  break;
	}
      STORAGE_BYTE_ORDER = table->byte_order;
    }

  buf[ddlen-1] = '\0';		/* mark end of string for reading functions */
  if (buf[p] == ';')
    p++;			/* buf[p] is semi-colon */

  /* Get "description" field */
  des = parse_get_string (&p, buf, (char)COMPONENT_SEPERATOR);
  strncpy (table->description, des, 80);
  if(des != (char *)NULL) {
    xvt_free (des);
    des = (char *)NULL;
  }

  /* Get "narrative" field */
  nar = parse_get_string (&p, buf, (char)COMPONENT_SEPERATOR);
  strncpy (table->narrative ,nar, 12);
  if(nar != (char *)NULL)
    {xvt_free (nar);nar = (char *)NULL;}


  n = 0 ;
  /* get number of fields */
  for (i=p; i<ddlen; i++)
    if (buf[i] == LINE_CONTINUE)
      i++;			/* skip past line continue, and next character */
    else if (buf[i] == END_OF_FIELD) /* Found end of field */
      n++;			/* increment nfields */
    else if (buf[i] == COMMENT)	/* skip past comments */
      while (buf[i] != LINE_CONTINUE &&
	     buf[i] != END_OF_FIELD &&
	     buf[i] != '\0')
	i++;			/* increment i */

  table->nfields = n;

  table->header = (header_type)xvt_zmalloc ((size_t)(n + 1) * sizeof(header_cell));

  if (table->header == NULL)
    {
      xvt_note ("Parse_data_definition: malloc failed.");
      return (0);
    }

  /* Loop the the number of fields in the record */
  for (i=0; i<n; i++)
    {
      end_of_rec = FALSE;

      /* Get the "name" field */
      table->header[i].name = parse_get_string (&p, buf, FIELD_COUNT);

      if (i == 0)
	if ((strcmp (table->header[0].name, "ID") != 0) &&
	    (strcmp (table->header[0].name, "id") != 0))
          {
	    xvt_note ("parse_data_def: No 'ID' in header definition.");
	    return (0);
          }

      /* Get the "type" field */
      table->header[i].type  = (parse_get_char  (&p,buf));
      table->header[i].type  = (char)toupper (table->header[i].type);

      /* Get the "count" field */
      table->header[i].count = parse_get_number(&p,buf,FIELD_SEPERATOR );


      /* Check for variable length flag (-1) */
      if (table->header[i].count == -1)
        reclen = -1;

      /* Now set null values and add up record length, if fixed length */

      status = 0;

      switch (table->header[i].type)
        {
        case 'I':
	  if (reclen >= 0)
	    reclen += (sizeof (int32) * table->header[i].count);
	  table->header[i].nullval.Int = (int32) NULLINT ;
	  break;
        case 'S':
	  if (reclen >= 0)
	    reclen += (sizeof (short) * table->header[i].count);
	  table->header[i].nullval.Short = NULLSHORT;
	  break;
        case 'F':
	  if (reclen >= 0)
	    reclen += (sizeof (float) * table->header[i].count);
	  table->header[i].nullval.Float = NULLFLOAT;
	  break;
        case 'R':
	  if (reclen >= 0)
	    reclen += (sizeof (double) * table->header[i].count);
	  table->header[i].nullval.Double = NULLDOUBLE;
	  break;
        case 'L':
        case 'T':
	  if (reclen >= 0)
	    {			/* if fixed length */
              reclen += (sizeof (char) * table->header[i].count);
              table->header[i].nullval.Char =
		(char*)xvt_zmalloc ((size_t)table->header[i].count + 1);
              for (k=0; k<table->header[i].count; k++)
		table->header[i].nullval.Char[k] = NULLCHAR;
              table->header[i].nullval.Char[k] = '\0';
	    }
	  else
	    {			/* variable length */
              table->header[i].nullval.Char =
		(char*)xvt_zmalloc (VARIABLE_STRING_NULL_LENGTH + 1);
              for (k=0; k<VARIABLE_STRING_NULL_LENGTH; k++)
		table->header[i].nullval.Char[k] = NULLCHAR;
              table->header[i].nullval.Char[k] = '\0';
	    }
	  break;
        case 'C':
	  if (reclen >= 0)
	    reclen += (sizeof (coordinate_type) * table->header[i].count);
	  table->header[i].nullval.Other = '\0';
	  break;
        case 'Z':
	  if (reclen >= 0)
	    reclen += (sizeof (tri_coordinate_type)*table->header[i].count);
	  table->header[i].nullval.Other = '\0';
	  break;
        case 'B':
	  if (reclen >= 0)
	    reclen += (sizeof(double_coordinate_type)*table->header[i].count);
	  table->header[i].nullval.Other = '\0';
	  break;
        case 'Y':
	  if (reclen >= 0)
	    reclen +=
	      (sizeof(double_tri_coordinate_type) * table->header[i].count);
	  table->header[i].nullval.Other = '\0';
	  break;
        case 'D':
	  if (reclen >= 0)
	    reclen += ((sizeof (date_type)-1) * table->header[i].count);
	  strcpy (table->header[i].nullval.Date, NULLDATE);
	  break;
        case 'K':
	  reclen = -1;
	  table->header[i].nullval.Other = '\0';
	  break;
        case 'X':
	  /* do nothing */
	  table->header[i].nullval.Other = '\0';
	  break ;
        default:
	  xvt_note ("parse_data_def: no such type %c", table->header[i].type ) ; /*DGM*/
	  status = 1;
	  break;
        } /* switch type */

      if (status)
        return (0);

      /* Get "keytype" field */
      table->header[i].keytype  = parse_get_char (&p, buf);

      /* Get "description" field */
      des = parse_get_string (&p, buf, FIELD_SEPERATOR);
      strncpy (table->header[i].description, des, 80);
      if(des != (char *)NULL)
	{xvt_free(des);des = (char *)NULL;}

      /* Get "value description table" field */
      vdt = parse_get_string (&p, buf, FIELD_SEPERATOR);
      strncpy (table->header[i].vdt, vdt, 12);
      if(vdt != (char *)NULL)
	{xvt_free (vdt);vdt = (char *)NULL;}


      /* Get "thematic index" field */
      table->header[i].tdx = (char*)NULL;
      table->header[i].narrative = (char*)NULL;

      tdx = parse_get_string (&p, buf, FIELD_SEPERATOR);
      if (! strcmp (tdx, ""))
        {
	  table->header[i].tdx = (char*)NULL;
        }
      else
        {
	  if (strcmp (tdx,"-") != 0)
	    {
	      table->header[i].tdx = (char*)xvt_zmalloc (strlen (tdx)+ 1);
	      strcpy (table->header[i].tdx, tdx);
	    }
	  else
	    table->header[i].tdx = (char *)NULL;
        }
      if(tdx != (char *)NULL)
	{xvt_free(tdx);tdx = (char *)NULL;}

      /* Test for end of record */
      if (buf[p] == ':')
        end_of_rec = TRUE;


      /* Retreive "doc" field if present */
      if (!end_of_rec)
        {
	  doc = parse_get_string (&p, buf, FIELD_SEPERATOR);
	  if (! strcmp (doc, ""))
	    {
	      table->header[i].narrative = (char*)NULL;
	      end_of_rec = TRUE;
	    }
	  else
	    {
	      if (strcmp (doc, "-") != 0)
		{
		  table->header[i].narrative = (char*)xvt_zmalloc (strlen (doc) + 1);
		  strcpy (table->header[i].narrative, doc);
		}
	      else
		table->header[i].narrative = (char*)NULL;
	    }
	  if(doc != (char *)NULL)
	    {xvt_free(doc);doc = (char *)NULL;}
        }
      else
        table->header[i].narrative = (char*)NULL;

      p += 1;			/* Skip over the RECORD_SEPARATOR (:) */
    }
  if(buf != (char *)NULL)
    {xvt_free(buf);buf = (char *)NULL;}
  return reclen;
}


/**************************************************************************
 *
 *N  vpf_nullify_table
 *
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *    Nullify the given VPF table structure.
 *E
 *:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels  DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
void vpf_nullify_table( vpf_table_type *table )
#else
     void vpf_nullify_table( table )
     vpf_table_type *table ;
#endif
{

  if (!table) return;

  strcpy(table->name,"");
  table->path = NULL;
  table->nfields = 0;
  strcpy(table->description,"");
  strcpy(table->narrative,"");
  table->header = NULL;
  table->xfp = NULL;
  table->index = NULL;
  table->xstorage = 0;
  table->fp = NULL;
  table->nrows = 0;
  table->row = NULL;
  table->reclen = 0;
  table->ddlen = 0;
  table->defstr = NULL;
  table->storage = 0;
  table->mode = 0;
  table->byte_order = LEAST_SIGNIFICANT;
  table->status = CLOSED;
  return;
}


/****************************************************************************/
/* VPFOPENCHECK                                                             */
/*   Purpose:                                                               */
/*                                                                          */
/*   Parameters:                                                            */
/*    filename <input> == (char *) full path name of the file to be opened. */
/*    mode     <input> == (char *) mode of the file.                        */
/*    diskname <input> == (char *) descriptive name of the disk the file is */
/*                                 on.                                      */
/*    return  <output> == (FILE *) file pointer newly associated with       */
/*                                 filename.                                */
/****************************************************************************/
#ifdef PROTO
FILE *vpfopencheck (char *filename, char *mode)
#else
     FILE *vpfopencheck(filename, mode)
     char *filename;
     char *mode;
#endif
{
  FILE *fp;
  int32 retry;
  size_t len;
  char *copy;

  len = strlen (filename);
  copy = (char*)xvt_zmalloc (sizeof(char)*(len+1));
  strcpy(copy,filename);
  copy[len] = '.';
  copy[len+1] = '\0';
  fp = NULL;

  while (fp == NULL)
    {
      fp = muse_file_open(filename,mode);
      if ((fp == NULL) && ((fp = muse_file_open(copy,mode)) == NULL))
        {
	  retry = FALSE;
	  if (!retry) break;
        }
    }
  if(copy != (char *)NULL)
    {xvt_free(copy);copy = (char *)NULL;}
  return (fp);
}


/****************************************************************************/
/* VPF_OPEN_TABLE                                                           */
/*                                                                          */
/*   Purpose:                                                               */
/*     This function opens a vpf table and either loads it into RAM or sets */
/*     up the structure to read off of disk.                                */
/*                                                                          */
/*   Parameters:                                                            */
/*    tablename <input> == (char *) file name of the table.  As stated      */
/*                                  in the VPF standard, the table name     */
/*                                  must not end in 'x'.                    */
/*    storage   <input> == (storage_type) table storage mode -              */
/*                                  MUST be ram, disk, or either.           */
/*    mode      <input> == (char *) file mode for opening the table -       */
/*                                  MUST be the same as fopen() mode in C.  */
/*    defstr    <input> == (char *) table definition string used for        */
/*                                  creating a writable table.              */
/*                                  If write mode this MUST be a valid      */
/*                                  VPF table definition string.            */
/*    vpf_open_table <output> == (vpf_table_type) VPF table structure.      */
/****************************************************************************/
#ifdef PROTO
vpf_table_type vpf_open_table (char *tablename, storage_type storage,
			       char *mode, char *defstr)
#else
     vpf_table_type vpf_open_table (tablename, storage, mode, defstr)
     char         *tablename;
     storage_type storage;
     char         *mode;
     char         *defstr;
#endif
{
  vpf_table_type   table; 
  char tablepath[255], *idxname,*ptr;
  int32 i, j, tablesize, idxsize, qty;
  uint32 ulval;
  char idxext,idxmaj;

  memset( &table, 0, sizeof(table) );
  strcpy(tablepath,tablename);

  /* Parse out name and path */
  j = -1;
  i=strlen(tablepath);
  while (i>0)
    {
      if (tablepath[i] == '\\' || tablepath[i] == '/' || tablepath[i] == ':' )
      {
          j = i;
          break;
      }
      i--;
    }
  strncpy(table.name,&(tablepath[j+1]),12);
  table.path = (char*)xvt_zmalloc (strlen (tablepath) + 5);
  strcpy(table.path, tablepath);

  /* Establish a read or write table operation */

  if (mode[0] == 'r')
    table.mode = Read;
  else
    table.mode = Write;

  table.fp = muse_file_open (tablepath, mode);
  table.storage = storage;

  if (table.fp == NULL)
    {
      xvt_note ("\nvpf_open_table: error opening <%s>\n",tablepath); /*DGM*/
      if (table.path != (char*)NULL)
	{
	  xvt_free (table.path);
	  table.path = (char*)NULL;
	}
      return table;
    }

  /* If file is to be created, copy the def string ptr into header for now */

  if (table.mode == Write)
    table.defstr = defstr;

  tablesize = muse_filelength (table.path);

  /* Populate table structure with correct data, either for read or write */

  table.reclen = parse_data_def (&table);

  if (table.mode == Write) 
    {				/* write out header */
      rewind (table.fp);
      Write_Vpf_Int (&table.ddlen, table.fp, 1);
      Write_Vpf_Char (table.defstr, table.fp, table.ddlen);
      if (table.defstr != (char*)NULL)
	{
	  xvt_free (table.defstr);
	  table.defstr = (char*)NULL;
	}
      table.defstr = (char*)NULL;
      table.nrows = 0;
    }

  if (table.reclen > 0)
    {
      /* Index file does not exist */
      table.xstorage = COMPUTE;
      if (table.mode != Write)
	table.nrows = (tablesize - table.ddlen)/table.reclen;
      table.xfp = (FILE*)NULL;
    }
  else
    {

      qty = strlen(tablename);
      ptr = &(tablename[qty-3]);
      if (!strnicmp ("fcs", ptr, 3)) {
	idxext = 'z';		/* index for fcs is 'fcz' not 'fcx' */
	idxmaj = 'Z';
      } else {
	idxext = 'x';
	idxmaj = 'X';
      }

      /* Index file does exist */
      idxname = (char*)xvt_zmalloc (sizeof (char) * (strlen (tablepath)+2));
      strcpy(idxname,tablepath);

      /* Test for UNIX CD-ROM filename terminator "." */
      if (idxname[strlen(tablepath)-1] == '.')
	idxname[strlen(tablepath)-2] = idxext;
      else
        idxname[strlen(tablepath)-1] = idxext;

      table.xfp = muse_file_open(idxname, mode);
      if (table.xfp == NULL) {
	/* Test for UNIX CD-ROM filename terminator "." */
	if (idxname[strlen(tablepath)-1] == '.')
	  idxname[strlen(tablepath)-2] = idxmaj;
	else
	  idxname[strlen(tablepath)-1] = idxmaj;
	
	table.xfp = muse_file_open(idxname, mode);
      }

      /* The FCX CASE */

      if (table.xfp == NULL && idxext == 'z') {
	idxext = 'x';
	idxmaj = 'X';

	/* Test for UNIX CD-ROM filename terminator "." */
	if (idxname[strlen(tablepath)-1] == '.')
	  idxname[strlen(tablepath)-2] = idxext;
	else
	  idxname[strlen(tablepath)-1] = idxext;
	
	table.xfp = muse_file_open(idxname, mode);
	if (table.xfp == NULL) {
	  /* Test for UNIX CD-ROM filename terminator "." */
	  if (idxname[strlen(tablepath)-1] == '.')
	    idxname[strlen(tablepath)-2] = idxmaj;
	  else
	    idxname[strlen(tablepath)-1] = idxmaj;
	  
	  table.xfp = muse_file_open(idxname, mode);
	}	
      }


      if (idxname != (char*)NULL)
      {
        xvt_free (idxname);
        idxname = (char*)NULL;
      }
      
      /* If mode is READ and no variable length index exists then */
      if ((!table.xfp) && (table.mode == Read))
	{
      /* build in-memory index by reading the whole table file */
      int pos;
      table.nrows = 0;
      table.xstorage = RAM;
      table.index = NULL;
      pos = table.ddlen;
      fseek (table.fp, pos,SEEK_SET);
      /*fprintf(stderr, "%s\n", tablepath);*/
      while(pos != tablesize)
      {
        int nextPos;
        free_row(read_next_row(table), table);
        table.nrows++;
        table.index = realloc(table.index, table.nrows * sizeof (index_cell));
        nextPos = ftell(table.fp);
        table.index[table.nrows-1].pos = pos;
        table.index[table.nrows-1].length = nextPos - pos;
        /*fprintf(stderr, "%d : %d %d\n", table.nrows-1, pos,nextPos - pos);*/
        pos = nextPos;
      }
      
      table.idx_handle = table.index;
#if 0
      /* something has gone wrong so free up memory and return NULL */
	  for (i=0; i<table.nfields; i++)
            if (table.header[i].name != (char*)NULL)
	      {
		xvt_free (table.header[i].name);
		table.header[i].name = (char*)NULL;
	      }
	  if (table.header != (header_type)NULL)
            {
	      xvt_free ((char*)table.header);
	      table.header = (header_type)NULL;
            }
	  if (table.path != (char*)NULL)
            {
	      xvt_free (table.path);
	      table.path = (char*)NULL;
            }
	  fclose (table.fp);
	  table.fp = NULL;
	  return table;
#endif
	}

      /* Only read in index if file is read only */
      else if (table.xfp && (table.mode == Read))
	{
	  Read_Vpf_Int (&(table.nrows), table.xfp, 1L);
	  Read_Vpf_Int (&ulval, table.xfp, 1L);
          if( (unsigned int)table.nrows > 100 * 1024 * 1024 )
          {
            xvt_note ("vpf_open_table: <%s> : table.nrows = %d\n",tablepath, table.nrows);
            fclose(table.xfp);
            table.nrows = 0;
            return table;
          }
	  idxsize = table.nrows * sizeof (index_cell) + 10L;

	  /* Load the index into RAM */
	  table.xstorage = RAM;
#if 1
	  table.idx_handle = xvt_zmalloc (idxsize);
	  table.index = table.idx_handle;
#else
	  table.idx_handle = galloc (idxsize);
	  table.index = (index_type)glock (table.idx_handle)
#endif
	    for (i=0; i<table.nrows; i++)
	      {
		Read_Vpf_Int (&(table.index[i].pos), table.xfp, 1L);
		Read_Vpf_Int (&(table.index[i].length),table.xfp,1L);
	      }
	  fclose(table.xfp);
	}
      else if (table.mode == Write)
	{
	  /* Write out dummy header record for index file.      */
	  /* vpf_close_table finishes the job.                  */
	  Write_Vpf_Int ( &(table.ddlen), table.xfp, 1L);
	  Write_Vpf_Int ( &(table.ddlen), table.xfp, 1L);
	  table.xstorage = DISK;
	  table.index = (index_type)NULL;
	}
    } /* if table.reclen > 0  */


  if ((storage != disk) && ( table.mode == Read))
    {
      fseek (table.fp, index_pos (1L, table),SEEK_SET);
#if 1
      table.row_handle = xvt_zmalloc ((table.nrows + 1) * sizeof (row_type));
      table.row = table.row_handle;

#else
      table.row_handle = galloc ((table.nrows + 1) * sizeof (row_type));
      table.row = (ROW*)glock (table.row_handle);
#endif
      for (i=0; i<table.nrows; i++)
      {
        table.row[i] = read_next_row (table);
      }
      fclose (table.fp);
      table.storage = RAM;
    }
  table.status = OPENED;

  return table;
}

/*************************************************************************
 *
 *N  vpf_close_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function frees an entire table from memory.
 *     The table must have been previously opened with vpf_open_table().
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    table       <inout> == (vpf_table_type) VPF table structure.
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
void vpf_close_table( vpf_table_type *table )
#else
     void vpf_close_table( table )
     vpf_table_type *table;
#endif
{
  register int32 i;

  if (!table)
    return;

  if (table->status != OPENED)
    return;

  /* If the table is writable, write out the final record count */

  if (table->mode == Write && table->xfp)
    {
      rewind (table->xfp);
      Write_Vpf_Int ( &table->nrows, table->xfp, 1 ) ;
      Write_Vpf_Int ( &table->ddlen, table->xfp, 1 ) ;
    }

  for (i=0; i<table->nfields; i++)
    {
      if (table->header[i].name != (char*)NULL)
	{
	  xvt_free (table->header[i].name);
	  table->header[i].name = (char*)NULL;
	}
      /* free up null text string */
      if (table->header[i].type == 'T' || table->header[i].type == 'L')
	if (table->header[i].nullval.Char != (char *)NULL)
	  {
            xvt_free (table->header[i].nullval.Char);
            table->header[i].nullval.Char = (char*)NULL;
	  }
      /* free up index file string */
      if (table->header[i].tdx != (char*)NULL)
	{
	  xvt_free (table->header[i].tdx);
	  table->header[i].tdx = (char*)NULL;
	}
      /* free up narrative table string */
      if (table->header[i].narrative != (char*)NULL)         
	{
	  xvt_free (table->header[i].narrative);
	  table->header[i].narrative = (char*)NULL;
	}
    }
  if (table->header != (header_type)NULL)
    {
      xvt_free ((char*)table->header);
      table->header = (header_type)NULL;
    }

  switch (table->storage)
    {
    case RAM:
      for (i=0; i<table->nrows; i++)
	free_row (table->row[i], *table);
      if (table->row != (ROW*)NULL)
	{
//	  gunlock (table->row_handle);
	  gfree (table->row_handle);
	  table->row = (ROW*)NULL;
	}
      break;
    case DISK:
      if( table->fp != NULL )
          fclose (table->fp);
      break;
    default:
      xvt_note ("%s%s: unknown storage flag: %d\n",table->path,table->name,
                table->storage);
      break;
    }

  switch (table->xstorage)
    {
    case RAM:
      if (table->index != (index_type)NULL)
	{
//	  gunlock (table->idx_handle);
	  gfree (table->idx_handle);
	  table->index = (index_type)NULL;
	}
      break;
    case DISK:
      fclose (table->xfp);
      break;
    case COMPUTE:
      break;
    default:
      xvt_note ("%s%s: unknown index storage flag: %d\n",
                table->path,table->name,table->storage);
      break;
    }
  table->nfields = 0;
  if(table->path != (char *)NULL)
    {xvt_free(table->path);table->path = (char *)NULL;}
  table->status = CLOSED;
}


/*************************************************************************
 *
 *N  is_vpf_table
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  *
 *   Purpose:
 *P
 *     This function determines if the file at the specified location
 *     is a valid VPF table.  It returns TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  *
 *   Parameters:
 *A
 *    fname   <input> == (char *) file system path.
 *    return <output> == (int32) TRUE (1) if the file is a VPF table;
 *                             FALSE (0) if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  *
 *   History:
 *H
 *    Barry Michaels   April 1991                        DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
int32 is_vpf_table( char *fname )
#else
     int32 is_vpf_table( fname )
     char *fname;
#endif
{
  FILE *fp;
  int32 n, ok;

  fp = muse_file_open( fname, "rb" );
  if (!fp) {
    return FALSE;
  }
  Read_Vpf_Int ( &n, fp, 1 ) ;
  fseek( fp, n-1, SEEK_CUR );
  if (fgetc(fp) == ';')
    ok = TRUE;
  else
    ok = FALSE;
  fclose(fp);
  return ok;
}


/*************************************************************************
 *
 *N  is_vpf_null_float
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether a floating point number is the
 *     same as the VPF representation of the floating point NULL value.
 *     It returns TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    num     <input> == (float) number to evaluate.
 *    return <output> == (int32) TRUE (1) if the number is VPF NULL;
 *                             FALSE (0) if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   April 1991                        DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
int32 is_vpf_null_float( float num )
#else
     int32 is_vpf_null_float( num )
     float num;
#endif
{
  float nan;

  nan = (float)float_quiet_nan(0);
  if (memcmp(&nan,&num,sizeof(float))==0) return 1;
  return 0;
}

/*************************************************************************
 *
 *N  is_vpf_null_double
 *
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Purpose:
 *P
 *     This function determines whether a double precision floating point
 *     number is the same as the VPF representation of the double
 *     precision floating point NULL value.  It returns TRUE or FALSE.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   Parameters:
 *A
 *    num     <input> == (double) number to evaluate.
 *    return <output> == (int32) TRUE (1) if the number is VPF NULL;
 *                             FALSE (0) if not.
 *E
 *::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 *
 *   History:
 *H
 *    Barry Michaels   April 1991                        DOS Turbo C
 *E
 *************************************************************************/
#ifdef PROTO
int32 is_vpf_null_double( double num )
#else
     int32 is_vpf_null_double( num )
     double num;
#endif
{
  double nan;

  nan = (double)quiet_nan(0);
  if (memcmp(&nan,&num,sizeof(double))==0) return 1;
  return 0;
}

/**************************************************************************/
/* FORMAT_DATE                                                            */
/**************************************************************************/
#if XVT_CC_PROTO
void format_date (date_type date, char *fmtdate)
#else
     void format_date (date, fmtdate)
     date_type date;
     char *fmtdate;
#endif
{
  char year[8], month[8], day[8], hour[8], mn[8], sec[8];
  
  date[20]='\0';
  strncpy (year, date, 4);
  year[4]='\0';
  strncpy (month, &date[4], 2);
  month[2]='\0';
  strncpy (day, &date[6], 2);
  day[2]='\0';
  strncpy (hour, &date[8], 2);
  hour[2]='\0';
  strncpy (mn, &date[10], 2);
  mn[2]='\0';
  strncpy (sec, &date[12], 2);
  sec[2]='\0';
  sprintf (fmtdate,"%s/%s/%s %s:%s:%s", month, day, year, hour, mn, sec);
}
