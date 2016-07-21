/*-----------------------------------------------------------------------------
 * Filename        : ossimImageChain.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageChain
 * ---------------------------------------------------------------------------*/

%module pyossim

%{

#include <vector>
#include <map>
        
#include <ossim/base/ossimId.h>
#include <ossim/base/ossimConnectableObjectListener.h>

#include <ossim/imaging/ossimImageSource.h>

%}

%import "../base/ossimConnectableContainerInterface.i"

%feature("notabstract") ossimImageChain;

/* Handling Image chain Assignment operator */
%rename(__getitem__) ossimImageChain::operator[];

/* Importing other dependent files */
%import "ossim/base/ossimConstants.h"

/* Wrapping class ossimImageChain */
%include "ossim/imaging/ossimImageChain.h"

