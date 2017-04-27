%module pyossim

%{
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimRgbToIndexFilter.h>
#include <vector>
%}

#ifndef TYPE_DATA   
#define TYPE_DATA

%include "ossim/base/ossimConstants.h"

/*!
 * It will map the input data to an 8-bit index table.  The table
 * <pre>
 *
 * Example keyword list:
 *
 * type:  ossimRgbToIndexFilter
 *
 * If you want to load from a file then just do:
 *
 * lut.filename: <full path to lut file>
 *
 * If you have the lut table in line then it must look like this:
 *
 * lut.entry0:  204 102 1
 * lut.entry1:  255 204 153
 * lut.entry2:  51 204 204
 * lut.number_of_entries:  3
 * lut.type:  ossimRgbLutDataObject
 *
 * </pre>
 */
%include "ossim/imaging/ossimRgbToIndexFilter.h"

#endif
