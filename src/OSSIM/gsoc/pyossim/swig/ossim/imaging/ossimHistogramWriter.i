/*-----------------------------------------------------------------------------
Filename        : ossimHistogramWriter.i
Author          : Vipul Raheja
License         : See top level LICENSE.txt file.
Description     : Contains SWIG-Python of class ossimHistogramWriter
-----------------------------------------------------------------------------*/

%module pyossim

%{
        
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimConnectionEvent.h>
#include <ossim/base/ossimHistogramSource.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimHistogramWriter.h>
#include <ossim/imaging/ossimImageHistogramSource.h>

#include <fstream>

%}

#ifndef TYPE_DATA
#define TYPE_DATA

/* Include the required header files */
%import "ossim/base/ossimConstants.h";

/* Wrapping the class ossimHistogramWriter */
%include "ossim/imaging/ossimHistogramWriter.h"

#endif        
