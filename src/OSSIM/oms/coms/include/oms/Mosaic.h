#ifndef omsMosaic_HEADER
#define omsMosaic_HEADER
#include <ossim/base/ossimConstants.h>
#include <oms/Constants.h>
#include <string>
namespace oms
{
   class MosaicPrivateData;
   class OMSDLL Mosaic
      {
      public:
         Mosaic();
         virtual ~Mosaic();
         
         
         /**
          * Will write the current state of the Mosaic class to an image file. The type
          * is a mime type such as image/jpeg or image/png, ..... etc.
          *
          * If the second argument is empty then the extension is used on the filename 
          * to determine an output type.
          * 
          * The histogramAutoStretchModeType can be any of the auto stretch modes
          *       linear_1std_from_mean
          *       linear_2std_from_mean
          *       linear_3std_from_mean
          *       linear_auto_min_max
          *
          * By default all external files are turned off.  So it should not generate histograms, 
          * geoms, or overviews, ... etc.
          */
         void writeImageToFile(const std::string& filename,
                               const std::string& optionalType);
         
         
         void setupOrthoThumbnail(const std::string& inputFile,
                                  const std::string& entryId,
                                  const std::string& srsCode,
                                  ossim_uint32 resolution,
                                  const std::string& histogramAutoStretchModeType);
      protected:
         /**
          *
          * Returns an id of the image added.  Right now the id is
          * built up using the filename and entry number.  WIll return an empty string
          * if the image was not able to be opened.
          *
          */
         std::string addInputFile(const std::string& filename,
                                  const std::string& entryId);
         
         MosaicPrivateData* thePrivateData;
      };
}

#endif

