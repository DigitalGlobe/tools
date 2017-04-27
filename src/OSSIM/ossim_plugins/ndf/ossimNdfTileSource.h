//***
// Written by: Kenneth Melero (kmelero@sanz.com)
//
// $Id$
// ***
#ifndef ossimNdfTileSource_HEADER
#define ossimNdfTileSource_HEADER

#include "../ossimPluginConstants.h"
#include <ossim/imaging/ossimGeneralRasterTileSource.h>

class OSSIM_PLUGINS_DLL ossimNdfTileSource : public ossimGeneralRasterTileSource
{
public:
   ossimNdfTileSource();
   ossimNdfTileSource(const ossimKeywordlist& kwl, const char* prefix=0);
   virtual ~ossimNdfTileSource();
   
   virtual bool open();
   
   virtual ossimString getShortName() const;
   virtual ossimString getLongName()  const;
   virtual ossimString className()    const;
   
   virtual double getNullPixelValue(ossim_uint32 band=0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0) const;
   virtual double getMaxPixelValue(ossim_uint32 band=0) const;
   virtual ossimScalarType getOutputScalarType() const;

   /**
    * Returns the image geometry object associated with this tile source or
    * NULL if non defined.  The geometry contains full-to-local image
    * transform as well as projection (image-to-world).
    */
   virtual ossimRefPtr<ossimImageGeometry> getImageGeometry();

   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * Sets the filename
    */
   virtual void setFilename(const ossimFilename& filename);

   
private:
   bool open(const ossimGeneralRasterInfo& info);

   /**
    * Because all raster band files for multispectral, thermal and pan have
    * the same filename base we must make a unique name for file like the
    * overview, histogram, omd and such.  So is header file is
    * "LE7116034000502150.H1" this will return
    * "LE7116034000502150_H1" so that things like the overviews will come
    * out LE7116034000502150_H1.ovr.
    */
   // virtual ossimFilename getFilenameBase() const;

   ossimFilename theHeaderFile;

   
   
   TYPE_DATA
};

#endif /* #ifndef ossimNdfTileSource_HEADER */
