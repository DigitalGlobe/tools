/*-----------------------------------------------------------------------------
 * Filename        : ossimTieGpt.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTieGpt
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iostream>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimReferenced.h>
%}

%rename(__set__) ossimTieGpt::operator=;
%rename(__lshift__) operator<<;
%rename(__rshift__) operator>>;
%rename(ossimTieGpt_print) print;
%rename(ossimTieGpt_printTab) printTab;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTieGpt.h"
