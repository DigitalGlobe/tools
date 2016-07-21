/*-----------------------------------------------------------------------------
 * Filename        : ossimDtedHandler.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimDtedHandler
 * -----------------------------------------------------------------------------*/

%module pyossim

%{
#include <fstream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/elevation/ossimElevCellHandler.h>
#include <OpenThreads/Mutex>
#include <ossim/support_data/ossimDtedVol.h>
#include <ossim/support_data/ossimDtedHdr.h>
#include <ossim/support_data/ossimDtedUhl.h>
#include <ossim/support_data/ossimDtedDsi.h>
#include <ossim/support_data/ossimDtedAcc.h>
#include <ossim/support_data/ossimDtedRecord.h>
%}

/* Handling operators */
%rename(__set__) ossimDtedHandler::operator=;

%include "ossim/base/ossimConstants.h"

/* Handling class ossimDtedHandler */
%include "ossim/elevation/ossimElevCellHandler.h"
%include "ossim/elevation/ossimDtedHandler.h"
