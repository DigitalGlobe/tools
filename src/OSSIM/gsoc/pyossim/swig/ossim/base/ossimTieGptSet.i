/*-----------------------------------------------------------------------------
 * Filename        : ossimTieGptSet.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimTieGptSet
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <iostream>
#include <vector>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimTieGpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/matrix/newmat.h>
%}

/* Handling operators */
%rename(__set__) ossimTieGptSet::operator=;
%rename(ossimTieGptSet_printTab) printTab;

%include "ossim/base/ossimConstants.h"
%include "ossim/base/ossimTieGptSet.h"
