/* SYSTEM_F.H */

#ifndef H_SYSTEM_F
#define H_SYSTEM_F

#include <stdio.h>

/*
 * JLL #if XVT_OS_IS_SUNOS #include <sys/stdtypes.h> #endif
 */
#ifndef H_MUSE1
#include "muse1.h"
#endif

#if XVT_CC_PROTO
ERRSTATUS MUSE_API muse_error(ERRSTATUS status);
#else
ERRSTATUS MUSE_API muse_error();
#endif

#if XVT_CC_PROTO
void            lowercase(char *);
void            eol(char *, char *);
#else
void            lowercase();
void            eol();
#endif

/***************************************************************
@    big_fread()
****************************************************************
Reads in an arbitrarily large chunk of data
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
big_fread(
	  FILE * file,
	  GHANDLE handle,
	  uint32 length_in_bytes,
	  NUM_TYPE num_type,
	  DATA_TYPE data_type);
#else
ERRSTATUS MUSE_API big_fread();
#endif

/*
 * Description: Big_fread allows reading of blocks of data larger than 64K
 * bytes on all systems, including DOS.
 */

/***************************************************************
@    big_fwrite()
****************************************************************
Writes an arbitrarily large chunk of data
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
big_fwrite(
	   FILE * file,
	   GHANDLE handle,
	   uint32 length_in_bytes,
	   NUM_TYPE num_type,
	   DATA_TYPE data_type);
#else
ERRSTATUS MUSE_API big_fwrite();
#endif

/*
 * Description: Big_fwrite allows writing of blocks of data larger than 64K
 * bytes on all systems, including DOS.
 */

/***************************************************************
@    char_to_double()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API char_to_double(unsigned char *record, double *d, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API char_to_double();
#endif

/***************************************************************
@    char_to_long()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API char_to_long(unsigned char *record, int32 *l, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API char_to_long();
#endif

/***************************************************************
@    char_to_short()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API char_to_short(unsigned char *record, short *s, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API char_to_short();
#endif

/***************************************************************
@    char_to_ushort()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API char_to_ushort(unsigned char *record, USHORT * s, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API char_to_ushort();
#endif

/***************************************************************
@    context_sensitive_help()
****************************************************************
Invokes the MUSE context sensitive help
*/

#if XVT_CC_PROTO
void 
context_sensitive_help(
		       FILE_SPEC * help_file_spec,
		       char *help_topic);
#else
void            context_sensitive_help();
#endif

/*
 * Description: This extension of the XVT help facility (provided by XVT as
 * \xvt\source\vhelp.c) is to be called when the user pushes the HELP button
 * in a dialog.  The first argument is the XVT FILE_SPEC of the help file.
 * The second argument is the specific help topic that relates to dialog that
 * contains the help button.  The help_topic given as the second argument
 * must contain no leading or trailing spaces.
 */

/***************************************************************
@    convert_double()
****************************************************************
Converts among double machine formats
*/

#if XVT_CC_PROTO
void 
convert_double(
	       unsigned char *the_double,
	       int32 type);
#else
void            convert_double();
#endif

/*
 * Description: This function converts the_double number between different
 * local double number formats.
 * 
 * The type argument determines the type of conversion performed. The following
 * are supported
 * 
 * TYPE  CONVERSION
 * 
 * 1 = IEEE to VAX 2 = VAX to IEEE 3 = IEEE to SUN 4 = SUN to IEEE 5 = SUN to
 * VAX 6 = VAX to SUN 7 = IEEE to MAC 8 = MAC to IEEE
 * 
 */

/***************************************************************
@dir_create()
****************************************************************
Create a new directory
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_create(char *);
#else
ERRSTATUS MUSE_API dir_create();
#endif

/*
 * Description:
 */

/***************************************************************
@dir_pop()
****************************************************************
Move to the parent directory
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_pop(char *);
#else
ERRSTATUS MUSE_API dir_pop();
#endif

/*
 * Description: Call this function to change to a parent directory. If
 * chg_def_flag is TRUE then operating system calls will be made.  In any
 * case the last argument path will be set to a non-portable string
 * representation of the path.
 */
/***************************************************************
@dir_push()
****************************************************************
Move to the subdirectory directory
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_push(char *, char *);
#else
ERRSTATUS MUSE_API dir_push();
#endif

/*
 * Description:
 */

/***************************************************************
@dir_restore()
****************************************************************
Restores the saved directory
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_restore(char *);
#else
ERRSTATUS MUSE_API dir_restore();
#endif

/*
 * Description:
 * 
/*************************************************************** @dir_save() ***************************************************************
 * 
 * Save the current directory
 */

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_save(char *);
#else
ERRSTATUS MUSE_API dir_save();
#endif

/*
 * Description:
 * 
/***************************************************************
 * @dir_current() ***************************************************************
 * 
 * Get the current directory
 */

#if XVT_CC_PROTO
ERRSTATUS MUSE_API dir_current(char *);
#else
ERRSTATUS MUSE_API dir_current();
#endif

/***************************************************************
@    double_to_char()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
double_to_char(
	       unsigned char *record,
	       double *l,
	       short big_endian,
	       int32 *c);
#else
ERRSTATUS MUSE_API double_to_char();
#endif

/***************************************************************
@    file_spec_to_string()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API file_spec_to_string
                (
		                 FILE_SPEC * file_spec,
		                 char *string
);
#else
ERRSTATUS MUSE_API file_spec_to_string();
#endif

/*
 * Description:
 * 
 */

/***************************************************************
@    get_ini_string()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
get_ini_string(
	       FILE * ini_file,
	       char *section_name,
	       char *item_name,
	       char *default_value,
	       char *value,
	       size_t len);
#else
ERRSTATUS MUSE_API get_ini_string();
#endif

/*
 * Description: Opens the file MUSE.INI in the staartup directory. Retuns a
 * file pointer on success or NULL on failure.
 */

/***************************************************************
@    ini_open()
****************************************************************
*/

#if XVT_CC_PROTO
FILE           *ini_open(char *);
#else
FILE           *ini_open();
#endif

/*
 * Description: Opens the file MUSE.INI in the staartup directory. Retuns a
 * file pointer on success or NULL on failure.
 */

/***************************************************************
@    muse_open_file_dlg()
****************************************************************
Get a file spec for opening.
*/

#if XVT_CC_PROTO
FL_STATUS MUSE_API 
muse_open_file_dlg(
    FILE_SPEC *temp_file_spec,
    char *message);
#else
FL_STATUS MUSE_API muse_open_file_dlg();
#endif

/*
 * Description: Modifies XVT's open_file_dlg on the Macintosh
 * to filter based on the three letters following a period in
 * file name.  It makes the Mac see files more like
 * Motif and Win.
 */

/***************************************************************
@    long_to_char()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API long_to_char(unsigned char *record, int32 *l, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API long_to_char();
#endif

/***************************************************************
@    short_to_char()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API short_to_char(unsigned char *record, short *s, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API short_to_char();
#endif

/***************************************************************
@    start_ini_section()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API 
start_ini_section(
		  char *ini_file_name,
		  char *section_name);
#else
ERRSTATUS MUSE_API start_ini_section();
#endif

/*
 * Description:
 */

/***************************************************************
@    string_to_file_spec()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API string_to_file_spec(FILE_SPEC * file_spec, char *string);
#else
ERRSTATUS MUSE_API string_to_file_spec();
#endif

/*
 * Description:
 */

/***************************************************************
@    ushort_to_char()
****************************************************************
*/

#if XVT_CC_PROTO
ERRSTATUS MUSE_API ushort_to_char(unsigned char *record, USHORT * l, short big_endian, int32 *c);
#else
ERRSTATUS MUSE_API ushort_to_char();
#endif

/**************************************************************************/
/* FILE_OPEN                                                              */
/**************************************************************************/

#if XVT_CC_PROTO
FILE           *file_open(char *, char *);
#else
FILE           *file_open();
#endif

/**************************************************************************/
/* MUSE_ACCESS                                                            */
/**************************************************************************/

#if XVT_CC_PROTO
int             muse_access(char *, int);
#else
int             muse_access();
#endif

/**************************************************************************/
/* MUSE_FILELENGTH                                                        */
/**************************************************************************/

#if XVT_CC_PROTO
int32            muse_filelength(char *);
#else
int32            muse_filelength();
#endif

#if MUSE_DEBUG

#if XVT_CC_PROTO
void           *muse_malloc(size_t size, char *string);
#else
void           *muse_malloc();
#endif

#endif

#if MUSE_DEBUG

#if XVT_CC_PROTO
void            muse_free(void *block, char *string);
#else
void            muse_free();
#endif

#endif

#if XVT_CC_PROTO
void           *Talloc(size_t size, int32 *index, char *str);
#else
void           *Talloc();
#endif

#if XVT_CC_PROTO
void            Tfree(unsigned char *block, int32 *index, char *str);
#else
void            Tfree();
#endif

#if XVT_CC_PROTO
void            muse_log(char *string);
#else
void            muse_log();
#endif


/***************************************************************
@    EndTalloc()
****************************************************************
*/

#if XVT_CC_PROTO
void            EndTalloc(void);
#else
void            EndTalloc();
#endif



void
#if XVT_CC_PROTO
context_sensitive_help(FILE_SPEC * help_file_spec, char *topic);
#else
context_sensitive_help();
    FILE_SPEC      *help_file_spec;
    char           *topic;
#endif

/***************************************************************
@    pacify()
****************************************************************
Inform user of program status
*/

#if XVT_CC_PROTO
BOOLEAN MUSE_API 
pacify(
       char *message,
       short percent_complete);
#else
BOOLEAN MUSE_API pacify();
#endif

/*
 * Description: The message and percent_complete are displayed in the status
 * window.  The status window has a cancel button Pacify returns TRUE if the
 * user has pressed this cancel button. In this case you should terminate the
 * operation and call pacify with a "Ready" message and a NULL
 * percent_complete.
 */

#endif				/* H_SYSTEM_F */
