/*-----------------------------------------------------------------------------
 * Filename        : ossimTDpt.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTDpt
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iostream>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimTDpt.h>
%}

%rename(__set__) ossimTDpt::operator=;
%rename(__lshift__) operator<<;
%rename(__lshift__) operator>>;
%rename(ossimTDpt_print) print;
%rename(ossimTDpt_printTab) printTab;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTDpt.h"
