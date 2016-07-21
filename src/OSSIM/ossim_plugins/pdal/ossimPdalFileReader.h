//**************************************************************************************************
//
// OSSIM (http://trac.osgeo.org/ossim/)
//
// License: MIT
//
//**************************************************************************************************
// $Id: ossimPdalFileReader.h 23664 2015-12-14 14:17:27Z dburken $

#ifndef ossimPdalFileReader_HEADER
#define ossimPdalFileReader_HEADER 1

#include "ossimPdalReader.h"
#include "../ossimPluginConstants.h"
#include <pdal/pdal.hpp>

class ossimPointRecord;
class Stage;

#define USE_FULL_POINT_CLOUD_BUFFERING

class OSSIM_PLUGINS_DLL ossimPdalFileReader : public ossimPdalReader
{
public:

   /** default constructor */
   ossimPdalFileReader();

   /** virtual destructor */
   virtual ~ossimPdalFileReader();

   /** Accepts the name of a point cloud file. Returns TRUE if successful.  */
   virtual bool open(const ossimFilename& fname);

   /**
    *  Fetches up to maxNumPoints points at the given dataset <offset> in the order they
    * appear in the data file. Thread-safe implementation accepts a previously-allocated vector
    * from the caller. The offset is set to offset + block.size (or EOF) upon exit, to
    * permit sequencing with getNextBlock() implmented in base class.
    */
   virtual void getFileBlock(ossim_uint32 offset,
                             ossimPointBlock& block,
                             ossim_uint32 maxNumPoints=0xFFFFFFFF) const;

   virtual ossim_uint32 getNumPoints() const;

private:
   virtual void establishMinMax();


TYPE_DATA
};

#endif /* #ifndef ossimPdalFileReader_HEADER */

