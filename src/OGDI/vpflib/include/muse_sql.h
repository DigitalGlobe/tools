#ifndef H_MUSE_SQL

#define H_MUSE_SQL

/*
 *	Symbolic constants for C data types related to database data types
 */
#define MUSE_SQL_CHAR		1
#define MUSE_SQL_SHORT		2
#define MUSE_SQL_INT		3
#define MUSE_SQL_LONG		4
#define MUSE_SQL_FLOAT		5
#define MUSE_SQL_DOUBLE		6

#define MUSE_SQL_CHAR_ARRAY		11
#define MUSE_SQL_SHORT_ARRAY		12
#define MUSE_SQL_INT_ARRAY		13
#define MUSE_SQL_LONG_ARRAY		14
#define MUSE_SQL_FLOAT_ARRAY		15
#define MUSE_SQL_DOUBLE_ARRAY		16

#define MUSE_SQL_CHAR_ARRAY2		21
#define MUSE_SQL_SHORT_ARRAY2		22
#define MUSE_SQL_INT_ARRAY2		23
#define MUSE_SQL_LONG_ARRAY2		24
#define MUSE_SQL_FLOAT_ARRAY2		25
#define MUSE_SQL_DOUBLE_ARRAY2		26

#define MUSE_SQL_CHAR_ARRAY3		31
#define MUSE_SQL_SHORT_ARRAY3		32
#define MUSE_SQL_INT_ARRAY3		33
#define MUSE_SQL_LONG_ARRAY3		34
#define MUSE_SQL_FLOAT_ARRAY3		35
#define MUSE_SQL_DOUBLE_ARRAY3		36

/*
 *	Symbolic constants to represent API calls
 *
 */
#define MUSE_SQL_CONNECT		1
#define MUSE_SQL_LOGIN			2
#define MUSE_SQL_CURSOR_OPEN		3
#define MUSE_SQL_PREPARE		4
#define MUSE_SQL_SET_PARAM		5
#define MUSE_SQL_BIND			6
#define MUSE_SQL_EXECUTE		7
#define MUSE_SQL_FETCH			8
#define MUSE_SQL_CURSOR_CLOSE		9
#define MUSE_SQL_TABLES			10
#define MUSE_SQL_DESCRIBE_TABLE		11
#define MUSE_SQL_DESCRIBE_COLUMNS	12
#define MUSE_SQL_COMMIT			13
#define MUSE_SQL_ROLLBACK		14
#define MUSE_SQL_LOGOUT			15
#define MUSE_SQL_DISCONNECT		16
#define MUSE_SQL_FOREIGN_TABLES		17
#define MUSE_SQL_DESCRIBE_FOREIGN_TABLE	18
#define MUSE_SQL_GET_VARRAY		19
#define MUSE_SQL_SET_VARRAY		20

/*
 *      Functions
 */

#if XVT_CC_PROTO
int muse_sql_connect( char *dbname, char *host );
int muse_sql_login( int db, char *login, char *passwd, char *database );
int muse_sql_get_varray( int db, char *pathname );
int muse_sql_set_varray( int db, char *pathname );
int muse_sql_cursor_open( int db, char *cursor_name );
int muse_sql_cursor_close( int db, int cursor );
int muse_sql_prepare( int db, int cursor, char *sql_stmt );
int muse_sql_set_param( int db, int cursor, int param, int type,
			VOID *address, int length, int *var_length );
int muse_sql_bind( int db, int cursor, int column, int type,
		   VOID *address, int length, int *ret_length );
int muse_sql_execute( int db, int cursor );
int muse_sql_fetch( int db, int cursor );
int muse_sql_tables( int db, int cursor );
int muse_sql_describe_table( int db, int cursor, char *table );
int muse_sql_describe_columns( int db, int cursor1, int cursor2 );
int muse_sql_commit( int db, int cursor );
int muse_sql_rollback( int db, int cursor );
int muse_sql_logout( int db );
int muse_sql_disconnect( int db );
int muse_sql_foreign_tables( int db, int cursor );
int muse_sql_describe_foreign_table( int db, int cursor, char *table );
#else
int muse_sql_connect( );
int muse_sql_login( );
int muse_sql_get_varray( );
int muse_sql_set_varray( );
int muse_sql_cursor_open( );
int muse_sql_cursor_close( );
int muse_sql_prepare( );
int muse_sql_set_param( );
int muse_sql_bind( );
int muse_sql_execute( );
int muse_sql_fetch( );
int muse_sql_tables( );
int muse_sql_describe_table( );
int muse_sql_describe_columns( );
int muse_sql_commit( );
int muse_sql_rollback( );
int muse_sql_logout( );
int muse_sql_disconnect( );
int muse_sql_foreign_tables( );
int muse_sql_describe_foreign_table( );
#endif

#endif /* H_MUSE_SQL */
