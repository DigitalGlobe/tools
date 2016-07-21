/*-----------------------------------------------------------------------------
Filename        : ossimInit.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimInit which handles 
                  all aspects of initialization for OSSIM applications.
-----------------------------------------------------------------------------*/

%module pyossim

%{

#include <ossim/init/ossimInit.h>
#include <ossim/base/ossimFilename.h>        
#include <ossim/base/ossimConstants.h>

%}

%include "typemaps.i"

/* Handling the std::exception */
%include "exception.i"
%exception
{
        try
        {
                $action
        }
        catch(const std::exception& e)
        {
                SWIG_exception(SWIG_RuntimeError, e.what());
        }
}

/* This tells SWIG to treat char ** as a special case */
%typemap(in) ( int&, char ** )
{
        /* Check if is a list */
        if (PyList_Check($input)) 
        {
                $1 = PyList_Size($input);
                
                int i = 0;
                $2 = (char **) malloc(($1+1)*sizeof(char *));
                
                for (i = 0; i < $1; i++) 
                {
                        PyObject *o = PyList_GetItem($input,i);
                
                        if (PyString_Check(o))
                                $2[i] = PyString_AsString(PyList_GetItem($input,i));
                        else 
                        {
                                PyErr_SetString(PyExc_TypeError,"list must contain strings");
                                free($2);
                                return NULL;
                        }
                }
                $2[i] = 0;
        } 
 
        else 
        {
                PyErr_SetString(PyExc_TypeError,"not a list");
                return NULL;
        }
}

/* This cleans up the char ** array we mallocd before the function call */
%typemap(freearg) (int&, char **)
{
        free((char *) $2);
}


/* Handling Init Assignment operator */
%rename(__setattr__) ossimInit::operator=;


/* Include the header file containing the declarations to be wrapped */
%import "ossim/base/ossimConstants.h"

%include "ossim/init/ossimInit.h"
