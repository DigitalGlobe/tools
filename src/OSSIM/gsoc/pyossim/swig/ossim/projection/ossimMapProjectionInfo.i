/*-----------------------------------------------------------------------------
 * Filename        : ossimMapProjectionInfo.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimMapProjectionInfo
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/projection/ossimMapProjectionInfo.h>
%}

/* Handling assignment operator */
%rename(__set__) ossimMapProjectionInfo::operator=;

/* Handling the reserved function print */
%rename(ossimMapProjectionInfo_print) ossimMapProjectionInfo::print;

/* This tells SWIG to treat char ** as a special case */
%typemap(in) char ** 
{
        /* Check if input is a list */
        if (PyList_Check($input)) 
        {
                int size = PyList_Size($input);
                int i = 0;

                /* Allocate memory */
                $1 = (char *) malloc((size+1)*sizeof(char));

                for (i = 0; i < size; i++) 
                {
                        PyObject *o = PyList_GetItem($input,i);

                        if (PyString_Check(o))
                        {
                                $1[i] = PyString_AsString(PyList_GetItem($input,i));
                        }

                        else 
                        {
                                PyErr_SetString(PyExc_TypeError,"List must contain strings");
                                free($1);
                                return NULL;
                        }
                }
                $1[i] = 0;
        } 

        else 
        {
                PyErr_SetString(PyExc_TypeError,"not a list");
                return NULL;
        }
}

/* To return char** from a C function as Python List */
%typemap(out) char*
{
        int i;
        int len = 0;

        while ($1[len])
        {
                len++;
        }

        $result = PyList_New(len);

        for (i = 0; i < len; i++) 
        {
                PyList_SetItem($result, i, PyString_FromString($1[i]));
        }
}

%typemap(varin) const char * {
SWIG_Error(SWIG_AttributeError,"Variable $symname is read-only.");
SWIG_fail;
}


%include "ossim/base/ossimConstants.h"

/* Wrapping the class ossimMapProjectionInfo */
%include "ossim/projection/ossimMapProjectionInfo.h"
