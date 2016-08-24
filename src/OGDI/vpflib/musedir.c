/*
 * MUSEDIR.C- a set of directory and file utilities implemented over XVT
 * functions. These functions could also be implemented over a specific
 * operating system to wean from XVT.
 * 
 * This file contains the following functions: dir_create dir_current dir_pop
 * dir_push dir_restore dir_save file_open file_spec_to_string muse_access
 * muse_filelength muse_check_path
 */


#ifndef INCL_XVTH
#include "xvt.h"
#endif

#ifdef unix
/* #include "direct.h" */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#endif

#include "machine.h"

#include "musedir.h"
#ifndef TRUE
#define TRUE 1
#endif

#ifdef _MSDOS
#include "io.h"
#endif

#ifdef _WINDOWS
#include <stdio.h>
#include <io.h>
#endif

#include <ctype.h>


/**************************************************************************/
/* FILE_OPEN                                                              */
/**************************************************************************/

static int muse_exists_path(const char* path)
{
  int acc;
/*  fprintf(stderr, "trying %s\n", path);*/
#ifdef _WINDOWS
  acc = _access(path, 0);
#else
  acc = access(path, 0); 
#endif
  return acc == 0;
}


static int muse_fix_path_case(const char* src_path, char dest_path[SZ_FNAME])
{
  char tmp_path[SZ_FNAME];
  int lastSlashPos;
  int i;
  int len;
  int curPartHasDot = 0;
  int curPartIsLowerCase = 1;
  int curPartIsUpperCase = 1;

  strcpy(dest_path, src_path);
  if (muse_exists_path(dest_path))
  {
    return 1;
  }

  lastSlashPos = -1;
  len = strlen(dest_path);

  /* Find the maximum part of the path that exists */
  for(i=len-1;i>=0;i--)
  {
    if (dest_path[i] == FILE_SEP)
    {
      memcpy(tmp_path, dest_path, i);
      tmp_path[i] = 0;
      if (muse_exists_path(tmp_path))
      {
        lastSlashPos = i;
        break;
      }
    }
  }

  i++;

  for(;i<len;i++)
  {
    if (dest_path[i] == '.')
    {
      curPartHasDot = 1;
    }
    else if (dest_path[i] >= 'a' && dest_path[i] <= 'z')
    {
      curPartIsUpperCase = 0;
    }
    else if (dest_path[i] >= 'A' && dest_path[i] <= 'Z')
    {
      curPartIsLowerCase = 0;
    }
    if ((dest_path[i] == FILE_SEP || i == len-1) && i != 0)
    {
      int limit = (dest_path[i] == FILE_SEP) ? i-1 : i;
      memcpy(tmp_path, dest_path, limit+1);
      tmp_path[limit+1] = 0;
      if (muse_exists_path(tmp_path) == 0)
      {
        int j;
        for(j=lastSlashPos+1;j<=limit;j++)
        {
          tmp_path[j] = toupper(tmp_path[j]);
        }
        if (curPartIsUpperCase == 1 || muse_exists_path(tmp_path) == 0)
        {
          for(j=lastSlashPos+1;j<=limit;j++)
          {
            tmp_path[j] = tolower(tmp_path[j]);
          }
          if (curPartIsLowerCase == 1 || muse_exists_path(tmp_path) == 0)
          {
            if (i == len-1 && !(dest_path[i] == '.' || (i-1>=0 && dest_path[i-1] == ';' && dest_path[i] == '1')))
            {
               char tmp_dest_path[SZ_FNAME];
               if (curPartHasDot == 0)
               {
                 strcpy(tmp_path, dest_path);
                 strcat(tmp_path, ".");
                 if (muse_fix_path_case(tmp_path, tmp_dest_path))
                 {
                   strcpy(dest_path, tmp_dest_path);
                   return 1;
                 }
               }

               strcpy(tmp_path, dest_path);
               strcat(tmp_path, ";1");
               if (muse_fix_path_case(tmp_path, tmp_dest_path))
               {
                 strcpy(dest_path, tmp_dest_path);
                 return 1;
               }

               return 0;
            }
            else
            {
              return 0;
            }
          }
          else
          {
            memcpy(dest_path, tmp_path, limit+1);
          }
        }
        else
        {
          memcpy(dest_path, tmp_path, limit+1);
        }
      }
      curPartIsLowerCase = 1;
      curPartIsUpperCase = 1;
      curPartHasDot = 0;
      lastSlashPos = i;
    }
  }
  return 1;
}



#if XVT_CC_PROTO
FILE           *
muse_file_open(char *path, char *mode)
#else
FILE           *
muse_file_open(path, mode)
    char           *path;
    char           *mode;
#endif

{
    char            pathext[SZ_FNAME];
    char            real_path[SZ_FNAME];

    strcpy(pathext, path);
    muse_check_path(pathext);

    if (muse_fix_path_case(pathext, real_path) == 0)
    {
      return NULL;
    }

    return fopen(real_path, mode);

/*
#ifdef _MAC
    if ((file = fopen(pathext, mode)) == (FILE *) NULL)
    {

	if (strchr(pathext, '.') == NULL)
	    strcat(pathext, ".");
	strcat(pathext, ";1");
	file = fopen(pathext, mode);
    }
    return (file);
#endif				
*/
}



/**************************************************************************/
/* FIL_OPEN                                                              */
/**************************************************************************/

/*

#if XVT_CC_PROTO
int
fil_open(char *path, int mode)
#else
int
fil_open(path, mode)
    char           *path;
    int             mode;
#endif

{
    int             file;
    char            pathext[SZ_FNAME];
    char            lobuf[SZ_FNAME];
    char            upbuf[SZ_FNAME];
    int             i, len;
    BOOLEAN         DOT_FOUND = 0;
    int32            lenup;
    char            *s;

    strcpy(pathext, path);
    muse_check_path(pathext);

#ifdef _WINDOWS
    file = open(pathext, mode);
    return (file);
#endif				

#ifdef unix
    acc = access(pathext, amode);
    return (acc);
#if 0
    memset(lobuf, (char) NULL, SZ_FNAME);
    memset(upbuf, (char) NULL, SZ_FNAME);
    len = strlen(pathext);
    strcpy(upbuf, pathext);
    strcpy(lobuf, pathext);

    for (i = len-1; i >= 0; i--)
    {
	if (pathext[i] == '.')
	{
	    DOT_FOUND = TRUE;
	}
        if (pathext[i] == (int) FILE_SEP) 
            break;
    }
#if 0
    for (i = len - 1; (i >= 0 && pathext[i] != (int) FILE_SEP); i--)
    {
	upbuf[i] = toupper(pathext[i]);
	lobuf[i] = tolower(pathext[i]);
    }
#endif
    s = strchr(&pathext[1],'/');
    if (s != (char *)NULL)
      lenup = strlen(s);
    else
      lenup = len;
    for (i = len - lenup; i < len; i++)
    {
	upbuf[i] = toupper(pathext[i]);
	lobuf[i] = tolower(pathext[i]);
    }

    if ((file = open(pathext, mode)) < 0)
    {

	if (!DOT_FOUND)
	    strcat(pathext, ".");
	if ((file = open(pathext, mode)) < 0)
	{
	    strcat(pathext, ";1");
	    file = open(pathext, mode);
	}
    }
    if (file < 0)
    {
	if ((file = open(lobuf, mode)) < 0)
	{

	    if (!DOT_FOUND)
		strcat(lobuf, ".");
	    if ((file = open(lobuf, mode)) < 0)
	    {
		strcat(lobuf, ";1");
		file = open(lobuf, mode);
	    }
	}
    }
    if (file < 0)
    {
	if ((file = open(upbuf, mode)) < 0)
	{

	    if (!DOT_FOUND)
		strcat(upbuf, ".");
	    if ((file = open(upbuf, mode)) < 0)
	    {
		strcat(upbuf, ";1");
		file = open(upbuf, mode);
	    }
	}
    }
    return (file);
#endif				
#endif				

}

*/

/**************************************************************************/
/* MUSE_ACCESS                                                            */
/**************************************************************************/



#if XVT_CC_PROTO
int
muse_access(char *path, int amode)
#else
int
muse_access(path, amode)
    char           *path;
    int             amode;
#endif

{
    int             acc;
    char            pathext[SZ_FNAME];
    char            real_path[SZ_FNAME];

    strcpy(pathext, path);
    muse_check_path(pathext);

    if (muse_fix_path_case(pathext, real_path) == 0)
    {
      return -1;
    }

#ifdef _WINDOWS
    acc = _access(real_path, amode);
#else
    acc = access(real_path, amode);
#endif
    return acc;
}



/**************************************************************************/
/* MUSE_FILELENGTH                                                        */
/**************************************************************************/



#if XVT_CC_PROTO
int32
muse_filelength(char *path)
#else
int32
muse_filelength(path)
    char           *path;
#endif

{

#ifdef unix
    struct stat buf;
#endif 

    FILE           *file;
    int32            length;

    length = 0;
    file = muse_file_open(path, "rb");
    if (file != NULL) {
#ifdef _WINDOWS
      length = _filelength(fileno(file));
#else 
      if (fstat(fileno(file),&buf) == 0) {
	length = (int32) buf.st_size;
      } else {
	length = 0;
      }
#endif
      fclose(file);
    }
    return (length);
}



/************************************************************************/
/* MUSE_CHECK_PATH                                                      */
/************************************************************************/

void
muse_check_path(path)
    char *path;
{
#ifdef unix
    char tpath[SZ_FNAME];
    char *tp = tpath, *pp1 = path, *pp2;
    DIR *dir;
    struct dirent *de;

    /*fprintf(stderr, "muse_check_path: \"%s\"\n", path);*/
    while (*pp1) {
        if (*pp1 == '/' || *pp1 == '\\') {
            pp1++;
            if (tp >= tpath + SZ_FNAME)
                return;
            *tp++ = FILE_SEP;
            continue;
        }
        pp2 = pp1 + 1;
        while (*pp2 && *pp2 != '/' && *pp2 != '\\')
            pp2++;
        if (tp >= tpath + SZ_FNAME)
            return;
        *tp = 0;
	/*fprintf(stderr, "muse_check_path: directory \"%s\" \"%.*s\"\n", tpath, (int)(pp2 - pp1), pp1);*/
        dir = opendir(tpath);
        if (!dir) {
	    /*fprintf(stderr, "muse_check_path: directory \"%s\" does not exist\n", tpath);*/
            return;
	}
        while ((de = readdir(dir))) {
	    /*fprintf(stderr, "muse_check_path: name \"%s\"\n", de->d_name);*/
            if (strncasecmp(de->d_name, pp1, pp2 - pp1))
                continue;
            if (!de->d_name[pp2 - pp1])
                break;
            if (de->d_name[pp2 - pp1] == '.' && !de->d_name[pp2 - pp1 + 1])
                break;
            if (de->d_name[pp2 - pp1] == ';' && de->d_name[pp2 - pp1 + 1] == '1' && !de->d_name[pp2 - pp1 + 2])
                break;
            if (de->d_name[pp2 - pp1] == '.' && de->d_name[pp2 - pp1 + 1] == ';' && de->d_name[pp2 - pp1 + 2] == '1' &&
		!de->d_name[pp2 - pp1 + 3])
                break;
        }
        if (de) {
            strncpy(tp, de->d_name, tpath + SZ_FNAME - tp);
	    tp = memchr(tp, 0, tpath + SZ_FNAME - tp);
	    if (!tp)
		tp = tpath + SZ_FNAME;
	    /*fprintf(stderr, "muse_check_path: new name \"%s\"\n", tpath);*/
        }
        closedir(dir);
        if (!de)
            return;
	pp1 = pp2;
    }
    if (tp >= tpath + SZ_FNAME)
	return;
    *tp = 0;
    strncpy(path, tpath, SZ_FNAME);
    /*fprintf(stderr, "muse_check_path: returning \"%s\"\n", path);*/
#else
    int32            i, length;

    length = strlen(path);

    for (i = 0; i < length; i++)
	if (path[i] == '\\')
	    path[i] = FILE_SEP;
    return;
#endif
}

