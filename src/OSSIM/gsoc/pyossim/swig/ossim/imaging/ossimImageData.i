/*

*/

%module pyossim

%{
#include <vector>
#include <iosfwd>
#include <ossim/base/ossimRectilinearDataObject.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimRefPtr.h>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA
#endif


%rename(ossimImageData_print) print;
%rename(__set__) ossimImageData::operator=;

%template( ossimImageDataRefPtr ) ossimRefPtr<ossimImageData>;

%extend ossimImageData {
public:

   void copyToBuffer(ossim_uint8* destinationBuffer, int bandIdx)
   {
      memcpy(destinationBuffer, self->getBuf(bandIdx), self->getSizePerBandInBytes());
   }
};
%include "ossim/base/ossimConstants.h"
%include "ossim/imaging/ossimImageData.h"



