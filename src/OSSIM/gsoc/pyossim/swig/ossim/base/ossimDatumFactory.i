/*-----------------------------------------------------------------------------
 * Filename        : ossimDatumFactory.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDatumFactory
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <ossim/base/ossimString.h>
%}

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

/* Handling the templates */
%include "std_vector.i"
%include "std_string.i"
namespace std
{
        %template(vectorossimString) vector<ossimString>;
};

/* Wrapping the class ossimDatumFactory */
%include "ossim/base/ossimConstants.h" 
%include "ossim/base/ossimDatumFactory.h" 
