#ifdef _WINDOWS
#include <windows.h>
#endif
#include <stdlib.h>
#include "glutil.h"
#include <string.h>

/* prototype */
char *gl_GetRegistryString(char *regPath, char *keyname);
int gl_SetRegistryString(char *regPath, char *keyname, char *keyinfo);

/*************************************************************************
* Retreive the information related to where is the bin path of GRASSLAND 
*
*  Results:
*	A string
*
**************************************************************************/
char *getGLenv()
{
static char *gl = NULL;

    if (gl != NULL)
	return gl;

#ifdef _WINDOWS
    
    gl = gl_GetRegistryString(GLHOME, APPLICATION_NAME);
    if (gl == NULL) 
	return NULL;
#else
    gl = getenv("GRASSLAND");
    if (gl == NULL) 
	return NULL;
#endif
    return gl;

}

/****************************************************************************
 * Retreive the information related to where is the home user path of GRASSLAND
 *
 *  Results:
 *	A string
 *
 ****************************************************************************/
char *getUSRHOMEenv()
{
static char *gl = NULL;

    if (gl != NULL)
	return gl;

#ifdef _WINDOWS
    
    gl = gl_GetRegistryString(GLHOME, APPLICATION_USER_HOME);
    if (gl == NULL) 
	return NULL;
#else
    gl = getenv("HOME");
    if (gl == NULL) 
	return NULL;
#endif
    return gl;

}  

/*************************************************************************
* Retreive the information related to where is the GISRC file 
*
*  Results:
*	A string
*
**************************************************************************/
char *getGISRCenv()
{
static char *gl = NULL;

    if (gl != NULL)
	return gl;

#ifdef _WINDOWS
    
    gl = gl_GetRegistryString(GLHOME, APPLICATION_USER_GISRC);
    if (gl == NULL) 
	return NULL;
#else
    gl = getenv("GISRC");
    if (gl == NULL) 
	return NULL;
#endif
    return gl;

} 

/*************************************************************************
* Retreive the information related to where is the GISBASE path 
*
*  Results:
*	A string
*
**************************************************************************/
char *getGISBASEenv()
{
static char *gl = NULL;

    if (gl != NULL)
	return gl;

#ifdef _WINDOWS
    
    gl = gl_GetRegistryString(GLHOME, APPLICATION_USER_GISBASE);
    if (gl == NULL) 
	return NULL;
#else
    gl = getenv("GISBASE");
    if (gl == NULL) 
	return NULL;
#endif
    return gl;

}
#ifdef _WINDOWS 
/*************************************************************************
* Retreive the key information from HKEY_LOCAL_MACHINE in the registry
*
*  Results:
*	A string
*
**************************************************************************/
char *gl_GetRegistryString(char *regPath, char *keyname)
{
    HKEY hkey;
    DWORD dwSize;
    DWORD dwType;
    int ret;
    char *value = NULL;

    ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, regPath, 0, KEY_READ, &hkey);
    if (ret == ERROR_SUCCESS) {
	ret = RegQueryValueEx(hkey, keyname, NULL, &dwType, NULL, &dwSize);
	if (ret == ERROR_SUCCESS) {
	    if (dwType != REG_SZ) {
		RegCloseKey(hkey);
		return NULL;
	    }
	    value = (char *) malloc(dwSize);
	    if (value) {
		ret = RegQueryValueEx(hkey, keyname, NULL, &dwType, value,
				      &dwSize);
		if (ret != ERROR_SUCCESS) {
		    free((char *) value);
		    value = NULL;
		}
	    }
	}
	RegCloseKey(hkey);
    }
    return value;
}

/*************************************************************************
* Sets the key information from HKEY_LOCAL_MACHINE in the registry
*
*  Results:
*	1 if success
*       0 if fail
*
**************************************************************************/
int gl_SetRegistryString(char *regPath, char *keyname, char *keyinfo)
{
    HKEY hkey;
    DWORD dwDispose;
    int ret;
    int size;
    char *classer;
  
    ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hkey, &dwDispose);
    if (ret == ERROR_SUCCESS) {
        size = strlen(keyinfo);
	ret = RegSetValueEx(hkey, keyname, 0, REG_SZ, keyinfo, size);
	if (ret != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return 0;
	}
	else {
		RegCloseKey(hkey);
		return 1;
	}
    }
    else {
    	return 0;
    }
}
#endif

