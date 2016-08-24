#ifndef H_SQLLIB_D
#define H_SQLLIB_D

#ifndef H_VEC_D
#include "vec_d.h"
#endif

#ifndef ERRSTATUS
#define ERRSTATUS short
#endif

#ifndef MUSE_API
#define MUSE_API
#endif


#define MAX_QUERY_NAME_LEN 80
#define MAX_SQL_STMT_LEN 1000
#define MAX_NAME_LEN 32
#define MAX_LINE_LEN 1000
#define MAX_COLUMNS     100
#define MAX_VALUE_LEN 512

/*#define MAX_FIELDS 50 */
/*#define MAX_FIELD_LEN 512*/
#define RR_COUNT 18 /* number of points to approx a circle */

#define COMMA '\,'
#define TAB '\	'
#define MUSE_SQL_TEXT_DELIM '\''
#define IMPORT_TEXT_DELIM '\"'
#define NULL_CHAR '\0'

typedef union
{
    char char_val [MAX_VALUE_LEN];
    short short_val;
    int int_val;
    int32 long_val;
    float float_val;
    double double_val;
} SQL_VALUE;

typedef struct
{
	char name[MAX_NAME_LEN + 1];
	int type;
	int length;
	SQL_VALUE value;
} CELL;

typedef struct
{
    char name[MAX_NAME_LEN + 1];
    short num_columns;
    CELL cell[MAX_COLUMNS];
} TUPLE;


typedef struct
{
    char name[MAX_QUERY_NAME_LEN];
    char sql_stmt[MAX_SQL_STMT_LEN];
    char host[MAX_NAME_LEN];
    char server[MAX_NAME_LEN];
    char database[MAX_NAME_LEN];
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
}  SQL_QUERY;

/***************************************************************
@    SQL_DATA
****************************************************************
Defines a vector data structure
*/
typedef struct
{
    int32            magic;
    BOOLEAN         needs_setup;
    BOOLEAN         needs_execute;
    BOOLEAN         needs_redraw;
    BOOLEAN         needs_refresh;
    SQL_QUERY       sql_query;
    VEC            *vec;
}    SQL_DATA;


#endif
