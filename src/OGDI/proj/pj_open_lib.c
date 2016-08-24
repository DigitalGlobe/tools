/* standard location file open procedure */
#ifndef lint
static const char SCCSID[]="@(#)pj_open_lib.c	4.2   93/08/25 GIE REL";
#endif
#define PJ_LIB__
#include <projects.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
static char *
proj_lib_name =
#ifdef HOME_LIB
HOME_LIB;
#else
0;
#endif
FILE *
pj_open_lib(char *name, char *mode) {
  char fname[FILENAME_MAX+1], *sysname;
  FILE *fid;
  int n = 0;
  
  /*	
     if (*name == '~' && name[1] == DIR_CHAR)
     if (sysname = getenv("HOME")) {
     (void)strcpy(fname, sysname);
     fname[n = strlen(fname)] = DIR_CHAR;
     fname[++n] = '\0';
     (void)strcpy(fname+n, name + 1);
     sysname = fname;
     } else
     return NULL;
     else if (*name == DIR_CHAR || (*name == '.' && name[1] == DIR_CHAR) ||
     (!strncmp(name, "..", 2) && name[2] == DIR_CHAR) )
     sysname = name;
     else if ((sysname = getenv("PROJ_LIB")) || (sysname = proj_lib_name)) {
     (void)strcpy(fname, sysname);
     fname[n = strlen(fname)] = DIR_CHAR;
     fname[++n] = '\0';
     (void)strcpy(fname+n, name);
     sysname = fname;
     } else 
     sysname = name;
     */

  errno = 0;
  if (!(fid = fopen(name,mode))) {
    if ((sysname = getenv("GRASSLAND"))) {
      (void)strcpy(fname, sysname);
      fname[n = strlen(fname)] = DIR_CHAR;
      fname[++n] = '\0';
      (void)strcat(fname+n,"misc");
      fname[n = strlen(fname)] = DIR_CHAR;
      fname[++n] = '\0';
      (void)strcpy(fname+n, name);
      sysname = fname;    
    } else {
      sysname = name;
    }
    
    if (fid = fopen(sysname, mode))
      errno = 0;
  }
  return(fid);
}


