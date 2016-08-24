#ifdef _WINDOWS
#include <string.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <stdio.h>
#else
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "glutil.h"
#include <ogdi_macro.h>
/*
 *----------------------------------------------------------------------
 * remove_dir 
 *
 *	Empty a directory (delete all *.* files) and remove the directory 
 *
 * Results:
 *	A int (1 = done, 0 = error
 *
 *----------------------------------------------------------------------
 */
int remove_dir(path)
     char *path;
{
#ifdef _WINDOWS     		  
  struct _finddata_t c_file;
  long hfile;
  char current_dir[_MAX_PATH];

  if (_getcwd(current_dir,_MAX_PATH) == NULL) {
	return 0;
  }

  if (_chdir(path)) {
	return 0;
  }
  
  if ((hfile = _findfirst("*.*", &c_file)) == -1L) {
	return 0;
  } else {
	do {
		unlink(c_file.name);
	}
	while(_findnext(hfile, &c_file) == 0);
	_findclose(hfile);
  }   
  

  _chdir(current_dir);
 
  return (int) RemoveDirectory(path);
#else
 char buffer[256];
 
 sprintf(buffer,"rm -r %s",path);
 ogdi_system(buffer);
 return 0;
#endif
}

/*
 *----------------------------------------------------------------------
 * ConvertFStoBSString --
 *
 *	Convert a string with forward slashes to a string with backslashes
 *	The returned string must be freed with ckfree.
 *
 * Results:
 *	A string
 *
 * Side effects:
 *	Memory is allocated.
 *----------------------------------------------------------------------
 */
char *ConvertFStoBSString(char *in)
{
    char *out;
    char *ip, *op;
    int len;

    len = strlen(in) + 1;
    out = malloc(len);
    if (out == NULL) return in;

    op = out; ip = in;
    while (*ip != '\0') {
	if (*ip == '/') {
	    *op = '\\';
	} else {
	    *op = *ip;
	}
	ip++; op++;
    }
    *op = '\0';
    return out;
}

/*
 *----------------------------------------------------------------------
 * ConvertBStoFSString --
 *
 *	Convert a string with backslashes to a string with Forward slashes
 *	The returned string must be freed with ckfree.
 *
 * Results:
 *	A string
 *
 * Side effects:
 *	Memory is allocated.
 *----------------------------------------------------------------------
 */
char *ConvertBStoFSString(char *in)
{
    char *out;
    char *ip, *op;
    int len;

    len = strlen(in) + 1;
    out = malloc(len);
    if (out == NULL) return in;
    
    op = out; ip = in;
    while (*ip != '\0') {
	if (*ip == '\\') {
	    *op = '/';
	} else {
	    *op = *ip;
	}
	ip++; op++;
    }
    *op = '\0';
    return out;
}

#ifdef _WINDOWS
int list_element (out, element, desc, mapset, lister)
    FILE *out;
    char *element;
    char *desc;
    char *mapset;
    int (*lister)();
{
    char path[1000];
    char buf[400];
    int count,j;
    struct _finddata_t c_file;
    long hfile;
    char current_dir[_MAX_PATH];

    count = 0;
/*
 * convert . to current mapset
 */
    if (strcmp (mapset,".") == 0)
	mapset = G_mapset();

/*
 * get the full name of the GIS directory within the mapset
 * and list its contents (if it exists)
 *
 * if lister() routine is given, the ls command must give 1 name
 */
    G__file_name (path, element, "", mapset);
    if(access(path, 0) != 0)
	return 0;

/*
 * if a title so that we can call lister() with the names
 * otherwise the ls must be forced into columnar form.
 */

    if (_getcwd(current_dir,_MAX_PATH) == NULL) 
	return 0;


    if (_chdir(path)) 
	return 0;

    fprintf(out,"in mapset %s:\n\n",mapset);

    if ((hfile = _findfirst("*.*", &c_file)) == -1L) {
	_chdir(current_dir);
	return 0;
    }

    count = 0;
    do {
	if ( (c_file.attrib & (_A_HIDDEN | _A_SYSTEM)) == 0 && c_file.name[0] != '.') {
		fprintf(out,"    %s\n", c_file.name);
		count++;
	}
    } while(_findnext(hfile, &c_file) == 0);

    fprintf(out,"\n");

    _findclose(hfile);

    _chdir(current_dir);

    return count;
}
#endif

