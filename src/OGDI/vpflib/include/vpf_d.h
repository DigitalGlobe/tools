#ifndef H_VPF_DEF
#define H_VPF_DEF
#include <stdio.h>

#define CLOSED 0
#define OPENED 1

/* define NULL values */

#define		VARIABLE_STRING_NULL_LENGTH	10
#define   	NULLCHAR	' '
#define		NULLTEXT	" "
#define		NULLSHORT	-SHRT_MAX
#define 	NULLINT     	-LONG_MAX
#define		NULLDATE	" "

#if XVT_OS_ISUNIX
#define		NULLFLOAT	((float) quiet_nan (0))
#define		NULLDOUBLE	((double) quiet_nan (0))
#elif XVT_CC == XVT_THINK
#define		NULLFLOAT	((float)3.40282346638528860e+38)
#define		NULLDOUBLE	((double)1.797693134862315708e+308)
#else
#define		NULLFLOAT	((float) 3.402823466e+38)
#define		NULLDOUBLE	((double) 1.7976931348623158e+30)
#endif

/* These are all the metacharacters used to parse out input text files */
#define	 	COMPONENT_SEPERATOR	';'
#define		LINE_CONTINUE		'\\'
#define		SPACE			' '
#define		TEXT_NULL		"-"
#define		COMMENT_CHAR		'"'
#define		FIELD_COUNT		'='
#define		FIELD_SEPERATOR		','
#define		END_OF_FIELD		':'
#define		FIELD_PARTITION		'^'
#define		NEXT_ELEMENT		'|'
#define		COMMENT			'#'
#define		NEW_LINE		'\n'
#define		VARIABLE_COUNT		'*'
#define		TAB			'\t'

/* These macros help determine the type in the key datatype */

#define TYPE0(cell) ((cell>>6)&(3))
#define TYPE1(cell) ((cell>>4)&(3))
#define TYPE2(cell) ((cell>>2)&(3))
#define TYPE3(cell) ((cell)&(3))

typedef enum
{
    VpfNull,
    VpfChar,
    VpfShort,
    VpfInteger,
    VpfFloat,
    VpfDouble,
    VpfDate,
    VpfKey,
    VpfCoordinate,
    VpfTriCoordinate,
    VpfDoubleCoordinate,
    VpfDoubleTriCoordinate,
    VpfUndefined
}
                VpfDataType;


typedef union
{
    unsigned char   f1;
    unsigned short int f2;
    uint32 f3;
}               key_field;

typedef
char            VDATE[21];		/* Include null end of string */

typedef union
{
    char           *Char;
    short           Short;
    int32            Int;
    float           Float;
    double          Double;
    VDATE           Date;
    char            Other;
}               null_field;


typedef enum
{
    Read,
    Write
}
                file_mode;



/* Each column in a table row has a count and a pointer to the data */
/* and a null value default */
typedef struct
{
    int32            count;
    void           *ptr;
}               COLUMN;

/* A table row is an array of columns */
typedef COLUMN *ROW;

/* Index for variable width tables.          */
/* One index cell for each row in the table. */
typedef struct
{
    uint32   pos;
    uint32   length;
}               INDEX;


/* The basic information carried for each field in the table */
typedef struct
{
    char           *name;	/* Name of the field */
    char            description[81];	/* Field description */
    char            keytype;	/* Type of key - (P)rimary, (F)oreign,
				 * (N)onkey */
    char            vdt[13];	/* Value description table name */
    char           *tdx;	/* Thematic index file name */
    char            type;	/* Data type - T,I,F,K,D */
    int32            count;	/* Number of items in this column (-1
				 * =>variable) */
    null_field      nullval;	/* This is used for the converter */
    char           *narrative;	/* Name of a narrative table describing the
				 * field */
}               header_cell, *header_type;


typedef struct
{
    float           x;
    float           y;
}               XY_COORDINATE;

typedef struct
{
    double          x;
    double          y;
}               XY_DCOORDINATE;

typedef struct
{
    float           x;
    float           y;
    float           z;
}               XYZ_COORDINATE;

typedef struct
{
    double          x;
    double          y;
    double          z;
}               XYZ_DCOORDINATE;

typedef struct
{
    unsigned char   type;
    int32            id;
    int32            tile;
    int32            exid;
}               ID_TRIPLET;

/***************************************************************
@    HEADER_SIZE
****************************************************************
Size of header to be read.
*/
typedef struct
{
    int32            header_length;
    char            byte_order;
}               HEADER_SIZE;

/*
 * Description:
 * 
 */

/***************************************************************
@    STANDARD_HEADER
****************************************************************
Table Desciption and documentation filename.
*/
typedef struct
{
    char            table_description[101];
    char            separator_1;
    char            narrative_table[13];

}               STANDARD_HEADER;

/*
 * Description:
 * 
 */

/***************************************************************
@    REPEAT_HEADER
****************************************************************
Repeated for each column defined header.
*/
typedef struct
{
    char            name[17];
    char            separator_1;
    char            data_type;
    char            separator_2;
    char            number[5];
    char            separator_3;
    char            key_type;
    char            separator_4;
    char            column_description[101];
    char            separator_5;
    char            value_description[13];
    char            separator_6;
    char            thematic_index[13];
    char            separator_7;
    char            separator_8;
}               REPEAT_HEADER;

/*
 * Description:
 * 
 */


/***************************************************************
@    VPF_TABLE
****************************************************************
VPF Table Structure (standard header).
*/
typedef struct
{
    HEADER_SIZE     header_size;
    char            separator_1;
    STANDARD_HEADER standard_header;
    char            separator_2;
    REPEAT_HEADER   repeat_header[34];
    char            separator_3;
}               VPF_TABLE, VPT;

/*
 * Description:
 * 
 */




/***************************************************************
************      FILE STRUCTURES     **************************
***************************************************************/


/***************************************************************
@    DHT
****************************************************************
Database Header Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    char            vpf_version[11];
    char            database_name[9];
    char            database_desc[101];
    char            media_standard[21];
    char            originator[51];
    char            addressee[101];
    char            media_volumes;
    char            seq_numbers;
    char            num_data_sets;
    char            security_class;
    char            downgrading[4];
    char            downgrade_date[21];
    char            releasability[21];
    char            other_std_name[51];
    char            other_std_date[21];
    char            other_std_ver[11];
    char            transmittal_id;
    char            edition_number[11];
    char            edition_date[21];
}               DHT;

/*
 * Description:
 * 
 */

/***************************************************************
@    LAT
****************************************************************
Library Attribute Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    char            library_name[9];
    float           xmin;
    float           ymin;
    float           xmax;
    float           ymax;
}               LAT;

/*
 * Description:
 * 
 */

/***************************************************************
@    LHT
****************************************************************
Library Header Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    char            product_type[13];
    char            library_name[13];
    char            description[101];
    char            data_struct_code;
    int32            scale;
    char            source_series[16];
    char            source_id[31];
    char            source_edition[21];
    char            source_name[101];
    char            source_date[21];
    char            security_class;
    char            downgrading[4];
    char            downgrading_date[21];
    char            releasability[21];
}               LHT;

/*
 * Description:
 * 
 */

/***************************************************************
@    GRT
****************************************************************
Geographic Refernce Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    char            data_type[4];
    char            units[4];
    char            ellipsoid_name[16];
    char            ellipsoid_detail[51];
    char            vert_datum_name[16];
    char            vert_datum_code[4];
    char            sound_datum_name[16];
    char            sound_datum_code[4];
    char            geo_datum_name[16];
    char            geo_datum_code[11];
    char            projection_name[21];
    char            projection_code[3];
    char            parameter1[21];
    char            parameter2[21];
    char            parameter3[21];
    char            parameter4[21];
    char            false_origin_x[21];
    char            false_origin_y[21];
    char            false_origin_z[21];
    char            reg_pt[11];
    char            reg_long[16];
    char            reg_lat[16];
    char            reg_z[6];
    char            reg_table_x[16];
    char            reg_table_y[16];
    char            reg_table_z[16];
    char            diag_pt[11];
    char            diag_long[16];
    char            diag_lat[16];
    char            diag_z[6];
    char            diag_table_x[16];
    char            diag_table_y[16];
    char            diag_table_z[16];
}               GRT;

/*
 * Description:
 * 
 */

/***************************************************************
@    CAT
****************************************************************
Coverage Attribute Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    char            coverage_name[9];
    char            description[51];
    int32            level;
}               CAT;

/*
 * Description:
 * 
 */

/**************************************************************************/
/* DATA STRUCTURES                                                        */
/**************************************************************************/

/***************************************************************
@    INDEX_DATA
****************************************************************
Offset and Length.
*/
typedef struct
{
    int32            offset;
    int32            length;
}               INDEX_DATA;

/*
 * Description:
 * 
 */

/***************************************************************
@    INDEX
****************************************************************
Index for index table.
*/
typedef struct
{
    int32            nr_records;
    int32            size;
    INDEX_DATA    **record;
}               IDX, EDX, TXX, ENX;

/*
 * Description:
 * 
 */


/***************************************************************
@    FT
****************************************************************
Feature Table.
*/
typedef struct
{
    VPT            *vpt;
    int32            id;
    short           tile_id;
    int32            table;
    void           *data;
}               FT, LFT, TFT, PFT;

/*
 * Description:
 * 
 */

/***************************************************************
@    END_RECORD
****************************************************************
Entity Node Primitive Record (Points).
*/
typedef struct
{
    int32            id;
    int32            pft_id;
    int32            contain_face;
    char            first_edge[2];	/* to be defined as NULL */
    int32            nr_coordinates;	/* Not a VPF specified field */
    XY_COORDINATE **coordinate;
}               END_RECORD;

/*
 * Description:
 * 
 */
/***************************************************************
@  END
****************************************************************
Entity Node Primitive Table (Points)
*/
typedef struct
{
    VPT            *vpt;
    int32            nr_records;	/* Nat a VPF specified field */
    END_RECORD    **record;
}               END;

/*
 * Description:
 * 
 */
/***************************************************************
@    EDG_RECORD
****************************************************************
Edge Primitive Record (Lines).
*/
typedef struct
{
    int32            id;
    int32            lft_id;
    int32            start_node;
    int32            end_node;
    ID_TRIPLET     *right_face;
    ID_TRIPLET     *left_face;
    ID_TRIPLET     *right_edge;
    ID_TRIPLET     *left_edge;
    int32            nr_coordinates;	/* Not a VPF specified field */
    XY_COORDINATE **coordinate;
}               EDG_RECORD;

/*
 * Description:
 * 
 */
/***************************************************************
@  EDG
****************************************************************
Edge Primitive Table (Lines)
*/
typedef struct
{
    VPT            *vpt;
    int32            nr_records;	/* Nat a VPF specified field */
    EDG_RECORD    **record;
}               EDG;

/*
 * Description:
 * 
 */
/***************************************************************
@    TXT_RECORD
****************************************************************
Text Primitive Record.
*/
typedef struct
{
    int32            id;
    int32            txt_id;
    char           *string;
    int32            nr_coordinates;	/* Not a VPF specified field */
    XY_COORDINATE **coordinate;
}               TXT_RECORD;

/*
 * Description:
 * 
 */
/***************************************************************
@  TXT
****************************************************************
Text Primitive Table
*/
typedef struct
{
    VPT            *vpt;
    int32            nr_records;	/* Nat a VPF specified field */
    TXT_RECORD    **record;
}               TXT;

/*
 * Description:
 * 
 */
/**************************************************************************/
/* TABLE_DATA                                                             */
/**************************************************************************/
typedef struct
{
    int32            offset;
    int32            size;
}               TABLE_DATA;

/**************************************************************************/
/* VECTOR                                                                */
/**************************************************************************/
typedef struct
{
    int32            magic;
    DHT            *dht;
    LAT            *lat;
    CAT            *cat;
    GRT            *grt;
    LHT            *lht;
    EDG            *edg;
    EDX            *edx;
    LFT            *lft;
    TXT            *txt;
    TXX            *txx;
    TFT            *tft;
    ENX            *enx;
    END            *end;
    PFT            *pft;
}               VECTOR;

/* VPF table structure: */
typedef struct
{
    char            name[13];	/* Table filename */
    int32            nfields;	/* Number of fields */
    char            description[81];	/* Table description */
    char            narrative[13];	/* Table narrative file name */
    header_type     header;	/* Table header structure */
    FILE           *fp;		/* Table file pointer */
    FILE           *xfp;	/* Index table file pointer */
    IDX            *idx;	/* Index data */
    int32            nrows;	/* Number of rows in the table */
    ROW            *row;	/* Array of table rows */
    int32            size;	/* Size of table in bytes */
    int32            reclen;	/* Table record length (-1 = variable) */
    int32            ddlen;	/* Header length */
    int32            offset;	/* Offset in bytes to start of current table */
    int32            fpos;	/* Location of the filepointer */
    char           *defstr;	/* rdf, definition string */
    file_mode       mode;	/* Table is either reading or writing */
    unsigned char   status;	/* VPF table status - OPENED or CLOSED */
    unsigned char   byte_order;	/* Byte order of the table's data */
}               VPFTABLE;
#endif				/* H_VPF */
