/* MUSEDIR.H */
#ifndef H_MUSEDIR
#define H_MUSEDIR

#ifndef INCL_XVTH
#include "xvt.h"
#endif
#ifdef _MACHINE_
#include "machine.h"
#endif
#ifdef _UNIX
#include <unistd.h>
#endif

#ifndef _WINDOWS
#ifdef _MAC
#define FILE_SEP ':'
#define FILE_EXT1 ""
#define FILE_EXT2 ""
#endif

#ifdef _MSDOS
#define FILE_SEP '\\'
#define FILE_EXT1 ""
#define FILE_EXT2 ""
#endif

#ifdef _UNIX
#define FILE_SEP '/'
#define FILE_EXT1 ""
#define FILE_EXT2 ""
#endif
#if XVT_OS == XVT_OS_HPUX
#define FILE_SEP '/'
#endif
#else
#define FILE_SEP '\\'
#define FILE_EXT1 ""
#define FILE_EXT2 ""
#endif

#define ERRSTATUS short

#ifndef STAT_SUCCESS
#define STAT_SUCCESS 0
#endif

#if PROTO
ERRSTATUS dir_create (char*);
ERRSTATUS dir_pop(char *);
ERRSTATUS dir_push(char *, char *);
ERRSTATUS dir_restore (char *);
ERRSTATUS dir_save(char *);
ERRSTATUS dir_current(char *);
/* ERRSTATUS file_spec_to_string (FILE_SPEC *, char *);*/
FILE *muse_file_open (char*, char*);
int muse_access (char *, int);
int32 muse_filelength (char *);
void muse_check_path (char*);
#else
ERRSTATUS dir_create ();
ERRSTATUS dir_pop();
ERRSTATUS dir_push();
ERRSTATUS dir_restore ();
ERRSTATUS dir_save();
ERRSTATUS dir_current();
ERRSTATUS file_spec_to_string ();
FILE *muse_file_open ();
int muse_access ();
int muse_filelength ();
void muse_check_path ();
#endif


#endif  /* H_MUSEDIR */








