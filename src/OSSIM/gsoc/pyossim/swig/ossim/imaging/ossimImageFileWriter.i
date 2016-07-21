/*-----------------------------------------------------------------------------
 * Filename        : ossimImageWriter.i
 * Author          : Vipul Raheja
 * License         : See top level LICENSE.txt file.
 * Description     : Contains SWIG-Python of class ossimImageWriter
 * -----------------------------------------------------------------------------*/

%module pyossim

%{

#include <fstream>
#include <ossim/base/ossimIoStream.h>
#include <ossim/imaging/ossimImageWriter.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimObjectEvents.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimViewController.h>


%}

#ifndef TYPE_DATA
#define TYPE_DATA
#endif
/**
 * Pure virtual base class for image file writers.
 *
 * There is normally one subclass of this class for each format supported
 * for writing.  This class roughly corresponds to the ImageHandler class
 * used for reading images.  Format specific ossimImageWriter's are normally
 * instantiated by the ossimImageWriterFactory::createWriterInstance() method.
 * ossimImageWriters should not be directly instantiated by application code.
 */
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageFileWriter.h"

