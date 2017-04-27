
/* VPFTABLE.H */

#ifndef _VPF_TABLE_H_

#define _VPF_TABLE_H_

#include <stdio.h>
#include <math.h>
#include <float.h>


#ifdef _MSC
#define MAXLONG LONG_MAX
#endif

#ifdef _UNIX
#define MAXLONG LONG_MAX
#endif

#ifndef _MACHINE_H_
#include "machine.h"
#endif
#ifndef _VPF_IO_
#include "vpfio.h"
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif



#define VPF_DIR_SEPARATOR '\\'


/* This should be the ISO definition of date */
typedef char date_type[21] ;   /* Include null end of string */

/* NULL value type */
typedef union
   {
   char      *Char;
   short int Short;
   int32  Int;
   float     Float;
   double    Double;
   date_type Date;
   char      Other;
   } null_field;

/* The basic information carried for each field in the table */
typedef struct {
   char *name;           /* Name of the field */
   char *tdx;            /* Thematic index file name */
   char *narrative;      /* Name of a narrative table describing the field*/
   int32  count;      /* Number of items in this column (-1 =>variable)*/
   char description[81]; /* Field description */
   char keytype;         /* Type of key - (P)rimary, (F)oreign, (N)onkey */
   char vdt[13];         /* Value description table name */
   char type;            /* Data type - T,I,F,K,D */
   null_field nullval ;  /* This is used for the converter */
} header_cell, *header_type;

typedef enum { ram, disk, either, compute } storage_type;
#define RAM 0
#define DISK 1
#define EITHER 2
#define COMPUTE 3

typedef enum { Read, Write } file_mode ;

#define CLOSED 0
#define OPENED 1

/* Each column in a table row has a count and a pointer to the data */
/* and a null value default */
typedef struct
   {
   int32 count;
   void *ptr;
   } column_type;

/* A table row is an array of columns */
typedef column_type *row_type;
typedef column_type /*huge*/ *ROW;

/* Index for variable width tables.          */
/* One index cell for each row in the table. */
typedef struct {
   uint32  pos;
   uint32  length;
} index_cell;

typedef index_cell /*huge*/ *index_type;

/* VPF table structure: */
typedef struct {
   char            *path;           /* Directory path to the table           */
   int32            nfields;         /* Number of fields                      */
   int32            nrows;           /* Number of rows in the table           */
   int32            reclen;          /* Table record length (-1 => variable   */
   int32            ddlen;           /* Data definition string length         */
   FILE            *fp;             /* Table file pointer                    */
   FILE            *xfp;            /* Index file pointer                    */
   index_type      index;           /* Index structure                       */
   GHANDLE         idx_handle;      /* Huge data handle for index structure  */
   storage_type    storage;         /* Flag indicating table storage method  */
   storage_type    xstorage;        /* Flag indicating where index stored    */
   header_type     header;          /* Table header structure                */
   ROW             *row;            /* Array of table rows                   */
   GHANDLE         row_handle;      /* Huge data handle for row structure    */
   file_mode       mode;            /* Table is either reading or writing    */
   char            *defstr ;        /* rdf, definition string                */
   char            name[13];        /* Name of the VPF table                 */
   char            description[81]; /* Table description                     */
   char            narrative[13];   /* Table narrative file name             */
   unsigned char   status;          /* VPF table status - OPENED or CLOSED   */
   unsigned char   byte_order;      /* Byte order of the table's data        */
} vpf_table_type;

typedef struct {
   float x,y;
} coordinate_type;

typedef struct {
   double x,y;
} double_coordinate_type;

typedef struct {
   float x,y,z;
} tri_coordinate_type;

typedef struct {
   double x,y, z;
} double_tri_coordinate_type;

/* VPF id triplet field union structure.  Only one of the elements actually
stores the data correctly at any one time. */
typedef union {
   unsigned char  f1;
   unsigned short f2;
   uint32  f3;
} key_field;

/* id triplet internal storage type */
typedef struct {
   unsigned char type;
   int32  id, tile, exid;
} id_triplet_type;

/* These macros help determine the type in the key datatype */

#define TYPE0(cell) ((cell>>6)&(3))
#define TYPE1(cell) ((cell>>4)&(3))
#define TYPE2(cell) ((cell>>2)&(3))
#define TYPE3(cell) ((cell)&(3))

/* These macros set the value in the key datatype */

#define SETTYPE0(cell,value) cell = (((cell)&(~(3<<6)))|(((3)&(value))<<6))
#define SETTYPE1(cell,value) cell = (((cell)&(~(3<<4)))|(((3)&(value))<<4))
#define SETTYPE2(cell,value) cell = (((cell)&(~(3<<2)))|(((3)&(value))<<2))
#define SETTYPE3(cell,value) cell = (((cell)&(~(3)))|(((3)&(value))))

/* This macro helps to write out a key */

#define ASSIGN_KEY (tYPE,kEY,loc,val) { \
    if (val < 1) { \
      tYPE(kEY.type,0); \
    } else if (val < (1<<8)) { \
      tYPE(kEY.type,1); \
      kEY.loc = val ; \
    } else if ( val < (1<<16)) { \
      tYPE(kEY.type,2); \
      kEY.loc = val; \
    } else { \
      tYPE(kEY.type,3); \
      kEY.loc = val; }}

/* define NULL values */

#define    VARIABLE_STRING_NULL_LENGTH   10
#define    NULLCHAR   ' '
#define    NULLTEXT   " "
#define    NULLSHORT  -MAXSHORT
#define    NULLINT    -MAXLONG
#define    NULLDATE   "                    "

#define    NULLFLOAT   ((float) float_quiet_nan (0))
#define    NULLDOUBLE  ((double) quiet_nan (0))




/* Functions: */
#ifdef PROTO
   double quiet_nan (int32 unused);
   double float_quiet_nan (int32 unused);
   void vpf_nullify_table (vpf_table_type *table);
   vpf_table_type vpf_open_table (char *tablename, storage_type storage,
                                                    char *mode, char *defstr);
   void vpf_close_table (vpf_table_type *table);
   char *read_text_defstr (FILE *infile, FILE *outerr);
   int32 index_length (int32 row_number, vpf_table_type table);
   int32 index_pos (int32 row_number, vpf_table_type table);
   id_triplet_type read_key (vpf_table_type table);
   int32 row_offset (int32 field, row_type row, vpf_table_type table);
   row_type read_next_row (vpf_table_type table);
   row_type rowcpy (row_type origrow, vpf_table_type table);
   row_type read_row (int32 row_number, vpf_table_type table);
   void free_row (row_type row, vpf_table_type table);
   row_type get_row (int32 row_number, vpf_table_type table);
   int32 table_pos (char *field_name,  vpf_table_type table);
   void *get_table_element (int32 field_number, row_type row,
                              vpf_table_type table, void *value, int32 *count);
   void *named_table_element (char *field_name, int32 row_number,
                              vpf_table_type table, void *value, int32 *count);
   void *table_element (int32 field_number, int32 row_number,
                              vpf_table_type table, void *value, int32 *count);
   int32 is_vpf_table (char *fname);
   int32 is_vpf_null_float (float num);
   int32 is_vpf_null_double (double num);
   char *parse_get_string (int32*, char*, char);
   FILE *vpfopencheck (char *filename, char *mode);
   int32 parse_data_def (vpf_table_type *table);

   /* Write functions */
   int32 write_key( id_triplet_type key, FILE *fp );
   int32 write_next_row( row_type row, vpf_table_type *table );
   int32 write_row( int32 rownum, row_type row, vpf_table_type *table );
   row_type create_row( vpf_table_type table );
   void nullify_table_element( int32 field, row_type row,
                vpf_table_type table );
   int32 put_table_element( int32 field, row_type row,
                vpf_table_type table,
                void *value, int32 count );
   void swap_two ( char *in, char *out );
   void swap_four ( char *in, char *out );
   void swap_eight ( char *in, char *out );
   void format_date (date_type date, char *fmtdate);
#else
   double quiet_nan ();
   float float_quiet_nan ();
   void vpf_nullify_table ();
   vpf_table_type vpf_open_table ();
   void vpf_close_table ();
   char *read_text_defstr ();
   int32 index_length ();
   int32 index_pos ();
   id_triplet_type read_key ();
   int32 row_offset ();
   row_type  read_next_row ();
   row_type rowcpy ();
   row_type  read_row ();
   void free_row ();
   row_type get_row ();
   int32 table_pos ();
   void *get_table_element ();
   void *named_table_element ();
   void *table_element ();
   int32 is_vpf_table ();
   int32 is_vpf_null_float ();
   int32 is_vpf_null_double ();
   int32 parse_data_def ();

   /* Write functions */
   int32 write_key ();
   int32 write_next_row ();
   int32 write_row ();
   row_type create_row ();
   void nullify_table_element ();
   int32 put_table_element ();
   void swap_two ();
   void swap_four ();
   void swap_eight ();
   void format_date ();
#endif /* If PROTO */

#ifdef MAIN
  FILE * errorfp = stderr;
#else
  extern FILE * errorfp;
#endif

#ifndef max
#define max(a,b)     ((a > b) ? a : b)
#endif

#ifndef min
#define min(a,b)     ((a < b) ? a : b)
#endif

#endif     /* #ifndef _VPF_TABLE_H_  */

