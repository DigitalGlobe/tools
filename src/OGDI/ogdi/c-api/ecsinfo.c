/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Implements reading and use of "default info" files. 
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: ecsinfo.c,v $
 * Revision 1.4  2007/02/12 21:01:48  cbalint
 *      Fix win32 target. It build and works now. (tested with VC6)
 *
 * Revision 1.3  2007/02/12 16:09:06  cbalint
 *   *  Add hook macros for all GNU systems, hook fread,fwrite,read,fgets.
 *   *  Handle errors in those macro, if there are any.
 *   *  Fix some includes for GNU systems.
 *   *  Reduce remaining warnings, now we got zero warnings with GCC.
 *
 *  Modified Files:
 *  	config/unix.mak contrib/ogdi_import/dbfopen.c
 *  	contrib/ogdi_import/shapefil.h contrib/ogdi_import/shpopen.c
 *  	ogdi/c-api/ecs_xdr.c ogdi/c-api/ecsinfo.c ogdi/c-api/server.c
 *  	ogdi/datum_driver/canada/nadconv.c ogdi/driver/adrg/adrg.c
 *  	ogdi/driver/adrg/adrg.h ogdi/driver/adrg/object.c
 *  	ogdi/driver/adrg/utils.c ogdi/driver/rpf/rpf.h
 *  	ogdi/driver/rpf/utils.c ogdi/gltpd/asyncsvr.c
 *  	ogdi/glutil/iofile.c vpflib/vpfprim.c vpflib/vpfspx.c
 *  	vpflib/vpftable.c vpflib/vpftidx.c vpflib/xvt.h
 *
 * Revision 1.2  2001/04/09 15:04:34  warmerda
 * applied new source headers
 *
 */

#include "ecs.h"
#include <ogdi_macro.h>

#pragma comment(lib, "Ws2_32.lib")

ECS_CVSID("$Id: ecsinfo.c,v 1.4 2007/02/12 21:01:48 cbalint Exp $");

#ifdef _WINDOWS
#define strcasecmp(a,b) _stricmp(a,b)
#define strncasecmp(a,b,c) _strnicmp(a,b,c)
#endif
int ecs_DefReadIndex(char *directory, char *url, char* urlfile, char *key, char** result);
int ecs_DefReadFile(char* directory, char *filename, char *key, char **result);
int ecs_DefGetDirectoryFromURL(char *directory, char *url, char *file);
int ecs_DefReadALine(char *buf, char **key, char **value);
char * ecs_strtrim(char * string, const char * set, size_t *index);

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_GetDefaultInfo

   DESCRIPTION
      access the default information database based on 
      a url and a key value.
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char* url          string containing url
	 char* key          string containing key value (e.g. NORTH)
      OUTPUT
	 char** result      string containing result.  Mallocked
                            by the function; freed by calling routine.
			    Left as NULL if no key is found.

   END_PARAMETERS

   PRE_CONDITIONS
      -none
   END_PRE_CONDITIONS

   POST_CONDITIONS
      -none
   END_POST_CONDITIONS

   RETURN_VALUE
      int: 0 if unsuccessful, 1 otherwise.
      result: a mallocked char * if successful, NULL otherwise.  
            calling routine must free the memory.
   END_RETURN_VALUE

   PSEUDO_CODE
      2) verify if there is a DEFAULT_INFO user variable set
         a) if yes, read the index file and search for key value
	 b) if value found, stop and return it.
      3) verify if there is a index file in the USRHOME
         a) if yes, read the index file and search for key value
	 b) if value found, stop and return it.
      4) verify if there is a index file in the last directory of the URL.
         a) if yes, read the index file and search for key value
	 b) if value found, stop and return it.
   END_PSEUDO_CODE

   END_FUNCTION_INFORMATION

   ********************************************************************
   */


int ecs_GetDefaultInfo(char* url, char* key, char** result) {
  
  char *env;
  char directory[512];
  char *value;
  char urlfile[256];
  
  /* process the index if any from DEFAULT_INFO and set it up */

  /* if this is remote, don't ignore the first two methods */
  /*
    if (strncasecmp("gltp://",url, 7) != 0) {
    */
  env=getenv("DEFAULT_INFO");
  if (env) {
    if (ecs_DefReadIndex(env, url, NULL, key, &value)) {
      *result=value;
      return TRUE;
    }
  } 
  
  /* process the index if any from USRHOME */
  
  env=getenv("USRHOME");
#ifdef INFO_DEBUG
  fprintf(stderr,"looking at usrhome\n");
#endif
  if (env) {
    if (ecs_DefReadIndex(env, url, NULL, key, &value)) {
      *result=value;
      return TRUE;
    }
  } 
  /*    
	}
	*/
  /* get the directory from the url */
#ifdef INFO_DEBUG
  fprintf(stderr,"looking for directory in url\n");
#endif
  if (!ecs_DefGetDirectoryFromURL(directory, url, urlfile)) {
    return FALSE;
  }

#ifdef INFO_DEBUG
  fprintf(stderr,"looking in %s\n", directory);
#endif
  if (ecs_DefReadIndex(directory, url, urlfile,  key, &value)) {

    *result=value;
    return TRUE;
    
  }

  return FALSE;

}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_DefGetDirectoryFromURL

   DESCRIPTION
      private function: return the directory name based on the
      url
   END_DESCRIPTION

   PARAMETERS
      OUTPUT
         char* directory    directory of where to look for index
      INPUT
         char* url          string containing url
   END_PARAMETERS

   PRE_CONDITIONS
      -directory is pre-allocated, and is long enough to store the
      name of the directory.
   END_PRE_CONDITIONS

   POST_CONDITIONS
      -none
   END_POST_CONDITIONS

   RETURN_VALUE
      int: 0 if unsuccessful, 1 otherwise.
      result: directory name is placed in pre-allocated string "directory".
              filename if url points to a file (or NULL if none) is 
	      placed in file
   END_RETURN_VALUE

   PSEUDO_CODE
   END_PSEUDO_CODE

   END_FUNCTION_INFORMATION

   ********************************************************************
   */


int ecs_DefGetDirectoryFromURL(char *directory, char *url, char *file) {
  char* dir;
  char* tmp;
  struct stat buf;

  tmp=(char*) malloc (strlen(url)+1);
  if (!tmp) 
    return FALSE;

  strcpy(tmp,url);

  if (strncasecmp("gltp://",tmp, 7) == 0) {
    dir=&tmp[7];
    dir=strchr(dir,'/')+1;
    /* check for double slash */
    if (dir[0]=='/') {
      dir++;
    }

    dir=strchr(dir,'/');
  } else {
    dir=&tmp[6];
    dir=strchr(dir,'/');
  }

  /* check for extra slash */
  if (dir[1]=='/' || dir[2]==':')
    dir++;

  /* check if this is a file */
  if (stat(dir, &buf)!= 0) {
    free(tmp);
    return FALSE;
  }

#ifdef _WINDOWS
  if (_S_IFREG & (buf.st_mode))
#else
  if (S_ISREG(buf.st_mode)) 
#endif
  {
    /* strip off the filename */
    int i= strlen(dir)-1;

    while (dir[i]!='/' && i>0)
      i--;

    /* return the filename in file */
    strcpy(file, &dir[i+1]);
    dir[i]='\0';
  } else {
    file[0]='\0';
  }
 
  strcpy(directory,dir);
#ifdef INFO_DEBUG
printf("Directory calculated from the URL : %s\n", directory);
#endif 
  free(tmp);
  
  return TRUE;
}



/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_DefReadIndex

   DESCRIPTION
      private function: read the index file, look for a match
      to the url, then call the 
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char* directory    directory of where to look for index
         char* url          string containing url
	 char* key          string containing key value (e.g. NORTH)
      OUTPUT
	 char** result      string containing result.  Mallocked
                            by the function; freed by calling routine.
			    Left as NULL if no key is found.

   END_PARAMETERS

   PRE_CONDITIONS
      -none
   END_PRE_CONDITIONS

   POST_CONDITIONS
      -result mallocked if successful, else NULL.
   END_POST_CONDITIONS

   RETURN_VALUE
      int: 0 if unsuccessful, 1 otherwise.
      result: a mallocked char * if successful, NULL otherwise.  
            calling routine must free the memory.
   END_RETURN_VALUE

   PSEUDO_CODE
   END_PSEUDO_CODE

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

int ecs_DefReadIndex(char *directory, char *url, char *urlfile, char *key, char** result) {
  FILE *fptr;
  char *indexfile;    /* the index file */
  char *filename;     /* name of database file to search */
  char *tmpkey;
  char buf[1024];
  int found;

  /* open the index file in the directory */

  indexfile=(char *) malloc (strlen(directory) + 14);
  if (!indexfile) {
    return FALSE;
  }

  strcpy(indexfile, directory);
  if (indexfile[strlen(indexfile)-1]!='/') 
    strcat(indexfile,"/");
  strcat(indexfile, DEFAULTS_INDEX_FILE);

#ifdef INFO_DEBUG
printf ("The index file is %s \n", indexfile);
#endif

  fptr=fopen(indexfile, "r");
  free(indexfile);

  if (!fptr) {
#ifdef INFO_DEBUG
printf ("Could not open the index file \n");
#endif
    return FALSE;
  }
  
  /* read until we find a match for this url */

  filename=NULL;
  found=FALSE;
  while (!feof(fptr)) {
    ogdi_fgets(buf, MAX_DEF_LINE_LENGTH, fptr);
    if (ecs_DefReadALine(buf,&tmpkey,&filename)) {
#ifdef INFO_DEBUG
      printf("read KEY=>%s< FILENAME=>%s<\n", tmpkey, filename); 
#endif
      if (urlfile==NULL) {
	/* this is a full url */
	if (strcasecmp(url,tmpkey)== 0) {
	  found=TRUE;
#ifdef INFO_DEBUG
	  printf("FOUND URL MATCH %s\n",tmpkey); 
	  printf("value=%s\n", filename);
#endif
	  break;
	}
      } else {
#ifdef INFO_DEBUG
	printf("comparing urlfile=%s to filename=%s\n",urlfile, tmpkey);
#endif
	if (strcasecmp(urlfile,tmpkey)== 0 || (strcmp(urlfile,"") == 0 && strcmp(tmpkey,"*")== 0)) {
	  found=TRUE;
#ifdef INFO_DEBUG
	  printf("FOUND KEY %s\n",tmpkey); 
	  printf("value=%s\n", filename);
#endif
	  /* check the file */
	  
	  break;
	}
      }
    }    
  }
  fclose(fptr);
  
  if (!found) {
    return FALSE;
  }
  /* if match found, read the file */

  if (ecs_DefReadFile(directory, filename, key, result)) {
    return TRUE;
  } else {
    *result=NULL;
    return FALSE;
  }

  /* if no match found, return false */
  return FALSE;

}

/* 

take the line in "buf", return a pointer to "key" and
a pointer to the value".  place a NULL after each string, so
they can be copied out.

PRECONDITION:
destination must be writeable 

*/

int ecs_DefReadALine(char *buf, char **key, char **value) {

  int i=0;

  /* ignore comments */
  if (buf[0]=='#') 
    return FALSE;

  if (buf[strlen(buf)-1]=='\n') 
    buf[strlen(buf)-1]='\0'; /* remove \n */
  

  /* find key */
  while(buf[i]==' ' || buf[i]=='\t') {      
    i++;
  }
  
	if (buf[i] == '\0') 
		return FALSE;

  *key=&buf[i];
  
  /* find end of key */
  while(buf[i]!=' ' && buf[i] != '\t' && buf[i] !='\0') 
    i++;

	if (buf[i] == '\0') {
		*value=&buf[i];
		return TRUE;
  }

  buf[i++]='\0';
  
  /* find end of whitespace */
  while(buf[i]==' ' || buf[i]=='\t')
    i++;
  *value=&buf[i];
  
  return TRUE;

}


/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      ecs_DefReadFile

   DESCRIPTION
      private function: read the data file, look for a match
      for the key, return the value (if any).
   END_DESCRIPTION

   PARAMETERS
      INPUT
         char* directory    the directory of the file to read
         char* filename     the filename to look in
	 char* key          string containing key value (e.g. NORTH)
      OUTPUT
	 char** result      string containing result.  Mallocked
                            by the function; freed by calling routine.
			    Left as NULL if no key is found.

   END_PARAMETERS

   PRE_CONDITIONS
      -none
   END_PRE_CONDITIONS

   POST_CONDITIONS
      -result mallocked if successful, else NULL.
   END_POST_CONDITIONS

   RETURN_VALUE
      int: 0 if unsuccessful, 1 otherwise.
      result: a mallocked char * if successful, NULL otherwise.  
            calling routine must free the memory.
   END_RETURN_VALUE

   PSEUDO_CODE
   END_PSEUDO_CODE

   END_FUNCTION_INFORMATION

   ********************************************************************
   */


int ecs_DefReadFile(char *directory, char *filename, char *key, char **result) {
  FILE *fptr;
  char *indexfile;
  char buf[1024];
  char *tmpkey, *value, *tmpfile;
  size_t len;
  
  indexfile=(char *) malloc (strlen(directory) +strlen(filename)+3);
  if (!indexfile) {
    return FALSE;
  }

  strcpy(indexfile, directory);
  if (indexfile[strlen(indexfile)-1]!='/') 
    strcat(indexfile,"/");
  tmpfile=ecs_strtrim(filename," \t",&len);

  strncat(indexfile, tmpfile, len);

  fptr=fopen(indexfile, "r");
  free(indexfile);

  if (!fptr) {
    return FALSE;
  }
  
  /* read until we find a match for this url */

  filename=NULL;
  while (!feof(fptr)) {
    ogdi_fgets(buf, MAX_DEF_LINE_LENGTH, fptr);
    if (ecs_DefReadALine(buf,&tmpkey,&value)) {
#ifdef INFO_DEBUG
      printf("read KEY=>%s< VALUE=>%s<\n", tmpkey, value);
      printf("comparing tmpkey=%s to key=%s\n",tmpkey, key);
#endif
      if (strcmp(tmpkey,key)== 0) {
#ifdef INFO_DEBUG
	printf("FOUND KEY %s\n",tmpkey);
	printf("value=%s\n", value);
#endif       

	*result=(char *) malloc (strlen(value)+1);
	if (!*result) {
		fclose(fptr);
	  return FALSE;
	}
	strcpy(*result, value);
	fclose(fptr);
	return TRUE;

	break;

      }
    }
  }
  

  fclose(fptr);
  

  *result=NULL;
  return FALSE;

}

/*
   ********************************************************************

   FUNCTION_INFORMATION

   NAME
      strtrim

   DESCRIPTION
      trim a list of characters off both ends of a string

   END_DESCRIPTION

   PARAMETERS
      INPUT
        char * string: the string to be trimmed
        const char * set: a string consisting of characters to be trimmed
      OUTPUT
        size_t *index: the length of the trimmed string.  this is 0
	   if all the elements were stripped; it is strlen(string) if
	   nothing was stripped.
      
   END_PARAMETERS

   RETURN_VALUE
      a pointer to the start of the string.

   END_FUNCTION_INFORMATION

   ********************************************************************
   */

char * ecs_strtrim(char * string, const char * set, size_t *index) {

  int begin, end, length;
  char ok, *tmp;
  
  begin=strspn(string,set);
  if (begin==(int) strlen(string)) {
      *index=0;
      return string;
  }

  length=strlen(set);
  end=strlen(string)-1;  

  while(1) {
    ok=string[end];
    if ((int)strcspn(set,&ok)==length) break;
    end--;
    if (end == 0) break;
  }

  *index=end-begin+1;
  tmp=&string[begin];
  return tmp;
}






