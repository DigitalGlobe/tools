#ifndef H_SQLLIB_F
#define H_SQLLIB_F

#ifndef H_MUSE
#include "muse.h"
#endif

#if XVT_CC_PROTO
BOOLEAN MUSE_API is_update_stmt(char *sql_stmt);
#else
BOOLEAN MUSE_API is_update_stmt();
#endif

#if XVT_CC_PROTO
BOOLEAN MUSE_API is_select_stmt(char *sql_stmt);
#else
BOOLEAN MUSE_API is_select_stmt();
#endif

#if XVT_CC_PROTO
ERRSTATUS freshen_muse_tables ( int db );
#else
ERRSTATUS freshen_muse_tables ();
#endif


#if XVT_CC_PROTO
ERRSTATUS MUSE_API muse_str_trim(char *a, char *b);
#else
ERRSTATUS MUSE_API muse_str_trim();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_paste_into_table(SQL_DATA *sql_data, char *output_table);
#else
ERRSTATUS sqllib_paste_into_table();
#endif

#if XVT_CC_PROTO
int32 sqllib_tuple_find ( TUPLE *tuple, char *column_name);
#else
int32 sqllib_tuple_find ();
#endif


#if XVT_CC_PROTO
ERRSTATUS MUSE_API sqllib_do_group_fuction(int db, char *group_function,
    char *column_name, int column_type, char *table_name, void *return_value);
#else
ERRSTATUS MUSE_API sqllib_do_group_fuction();
#endif
#if XVT_CC_PROTO
int32 MUSE_API muse_str_locate(char *a, char *b);
#else
int32 MUSE_API muse_str_locate();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_bind_select(int db, int cursor, TUPLE *tuple);
#else
ERRSTATUS sqllib_bind_select();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_describe_table(int db, char *table_name, TUPLE *tuple);
#else
ERRSTATUS sqllib_describe_table();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_describe_select(int db, int select_cursor, TUPLE *tuple);
#else
ERRSTATUS sqllib_describe_select();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_map_tuples(TUPLE *tuple_in, TUPLE *tuple_out, int32 tuple_map[]);
#else
ERRSTATUS sqllib_map_tuples();
#endif

#if XVT_CC_PROTO
ERRSTATUS sqllib_read_definitions ( FILE *fp, TUPLE *tuple, char *primary_key);
#else
ERRSTATUS sqllib_read_definitions ();
#endif
/* loads tuple structure from a definition file
   format: column_name    column_type        column_length  initial value
   Column names beginning with * will become PRIMARY KEYS.
   Lines beginning with # are ignored.
*/


#if XVT_CC_PROTO
ERRSTATUS sqllib_units_dms_to_dd(char *point, double *latdd, double *londd);
ERRSTATUS dmsdd(double dms, double *dd);
#else
ERRSTATUS sqllib_units_dms_to_dd();
ERRSTATUS dmsdd();
#endif


/***************************************************************
@     sql_data_construct()
****************************************************************
function to costruct sql_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       sql_data_construct(FILE * file, SQL_DATA ** sql_data, BYTE_ORDER * bo);
#else
ERRSTATUS       sql_data_costruct();
#endif


/***************************************************************
@     sql_data_destruct()
****************************************************************
function to destruct sql_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       sql_data_destruct(FILE * file, BOOLEAN destruct, SQL_DATA ** sql_data, BYTE_ORDER * bo);
#else
ERRSTATUS       sql_data_destruct();
#endif


/***************************************************************
@     decode_sql_data()
****************************************************************
function to decode sql_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       decode_sql_data(unsigned char *record, SQL_DATA * sql_data);
#else
ERRSTATUS       decode_sql_data();
#endif


/***************************************************************
@     encode_sql_data()
****************************************************************
function to encode sql_data structure
*/

#if XVT_CC_PROTO
ERRSTATUS       encode_sql_data(unsigned char *record, SQL_DATA * sql_data);
#else
ERRSTATUS       encode_sql_data();
#endif


/***************************************************************
@    sqllib_generate_vec()
****************************************************************
Execute the SQL query drawing into a VEC
*/
#if XVT_CC_PROTO
ERRSTATUS
sqllib_generate_vec(SQL_DATA *sql_data);
#else
ERRSTATUS
sqllib_generate_vec();
#endif
/*
Description:
*/


/***************************************************************
@    sqllib_close()
****************************************************************
Close and disconnect from the MUSE SQL database
*/
#if XVT_CC_PROTO
ERRSTATUS
sqllib_close(SQL_QUERY *sql_query, int *db_handle);
#else
ERRSTATUS
sqllib_close();
#endif
/*
Description:
*/


/***************************************************************
@    sqllib_import()
****************************************************************
Import ASCII table into the MUSE SQL database
*/
#if XVT_CC_PROTO
ERRSTATUS
sqllib_import(int db_handle, char *filename);
#else
ERRSTATUS
sqllib_import();
#endif
/*
Description:
The table must be comma delimited ASCII with the first
line coantaining field names.  It must be in the default
directory,
*/

/***************************************************************
@    sqllib_open()
****************************************************************
Connect and login to the MUSE SQL database
*/
#if XVT_CC_PROTO
ERRSTATUS
sqllib_open(SQL_QUERY *sql_query, int *db_handle);
#else
ERRSTATUS
sqllib_open();
#endif
/*
Description:
*/


/***************************************************************
@    sqllib_units_dms_to_dd()
****************************************************************
*/
#if XVT_CC_PROTO
ERRSTATUS
sqllib_units_dms_to_dd(char *point, double *latdd, double *londd);
#else
ERRSTATUS
sqllib_units_dms_to_dd();
#endif
/*
Description:
*/
/***************************************************************
@    sqllib_test_connection()
****************************************************************
*/
#if XVT_CC_PROTO
BOOLEAN
sqllib_test_connection(SQL_QUERY *sql_query);
#else
BOOLEAN
sqllib_test_connection();
#endif
/*
Description:
Returns TRUE if the connection fails.
Returns FALSE if the connection succeeds.
*/


#if XVT_CC_PROTO
ERRSTATUS MUSE_API sqllib_parse_CDA_column_names(char *line, TUPLE *tuple);
ERRSTATUS MUSE_API sqllib_parse_CDA_column_types(char *line, TUPLE *tuple);
ERRSTATUS MUSE_API sqllib_parse_CDA_values(char *line, TUPLE *tuple);
ERRSTATUS MUSE_API sqllib_create_table(int db, TUPLE *tuple, char *primary_key);
ERRSTATUS MUSE_API sqllib_insert_tuple(int db, TUPLE *tuple);
ERRSTATUS MUSE_API sqllib_cell_value_set(char *value, CELL *cell);
#else
ERRSTATUS MUSE_API sqllib_parse_CDA_column_names();
ERRSTATUS MUSE_API sqllib_parse_CDA_column_types();
ERRSTATUS MUSE_API sqllib_parse_CDA_values();
ERRSTATUS MUSE_API sqllib_create_table();
ERRSTATUS MUSE_API sqllib_insert_tuple();
ERRSTATUS MUSE_API sqllib_cell_value_set();
#endif

#endif
