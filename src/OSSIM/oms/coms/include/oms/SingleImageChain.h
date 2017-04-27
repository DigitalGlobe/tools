/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/

#ifndef omsSingleImageChain_HEADER
#define omsSingleImageChain_HEADER
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/projection/ossimImageViewTransform.h>
#include <oms/Constants.h>
#include <oms/Object.h>

class ossimProjection;
class ossimNullPixelFlip;
class ossimBandSelector;
class ossimImageSharpenFilter;
class ossimPolyCutter;
class ossimHistogramRemapper;
class ossimImageRenderer;
class ossimCacheTileSource;
class ossimGeoPolyCutter;
class ossimScalarRemapper;
class ossimMultiResLevelHistogram;
namespace oms
{
   class OMSDLL SingleImageChain : public oms::Object
	{
	public:
		/**
		 *
		 * Construct a projection chain where everything is disabled except for the Renderer which handles reprojections
		 *
		 */
		SingleImageChain();
		virtual ~SingleImageChain();
		
		/**
		 * Returns the index of the source added.  -1 if unsuccessful.
		 */
      
      void disableAll();
      void setNullPixelFlipEnableFlag(bool flag);
		void setHistogramRemapperEnableFlag(bool flag);
		void setImageRendererEnableFlag(bool flag);
		void setCacheEnableFlag(bool flag);
		void setImageCutterEnableFlag(bool flag);
		void setViewCutterEnableFlag(bool flag);
		void setBandSelectorEnableFlag(bool flag);
		void setScalarRemapperEnableFlag(bool flag);

      void setSharpenEnableFlag(bool flag);
      
      void setPropertyGivenClassName(const std::string& className,
                                     const std::string& propertyName,
                                     const std::string& propertyValue);
      ossimConnectableObject* getConnectableObject(const std::string& className);
    
		void setResamplerType(const std::string& resamplerType);
		void setScalarRemapType(ossimScalarType scalarType);
     
      /**
       * Utility for setting to a single band output
       */
      void setToSingleBand(ossim_int32 band=0);
      /**
       * Utility method to force 3 band output
       */
      void setToThreeBands();
      
      /**
       * Utility method to set to 3 bandsand reverse them.  This is mainly used by NITF and Lndsat color data
       * where the bands are in bgr format and you want it in rgb combination.
       */
      void setToThreeBandsReverse();
      
      /**
       * Utility to set a grey scale to normal calculations.  It will use Blinns bumb shade equations
       * to create surface normals.  Typically this is applied to elevation data but you can use any single band 
       * input.
       */
      void setupSurfaceNormalCalculations();
      
		void setBandSelection(const int* bandList, int bandListSize, bool zeroBased=true);
		void setBandSelection(const std::vector<ossim_uint32>& bandList, bool zeroBased=true);
		void setBandSelection(const std::vector<ossimString>& bandList, bool zeroBased=true);
		void setHistogramFile(const std::string& file);
		
		/**
		 * mode can be one of the following types:
		 *
       *   linear_one_piece
		 *   linear_1std_from_mean
		 *   linear_2std_from_mean
		 *   linear_3std_from_mean
		 *   linear_auto_min_max
		 */
		void setHistogramStretchMode(const std::string& mode);
      void setHistogram(ossimMultiResLevelHistogram* histogram);
      /**
       * Stretch mode values can be linear_one_piece, linear_1std_from_mean, linear_2std_from_mean, linear_3std_from_mean, linear_auto_min_max
       */
		void setHistogramFileAndMode(const std::string& file, const std::string& mode);
      void setHistogramAndMode(ossimMultiResLevelHistogram* histogram, const std::string& mode);

      /**
       * Stretch mode values can be linear_one_piece, linear_1std_from_mean, linear_2std_from_mean, linear_3std_from_mean, linear_auto_min_max
       */
		void setHistogramFileToDefaultAndMode(const std::string& mode);
		
		void setViewCut(const ossimGpt* pointList, int pointListSize);
		void setViewCut(const std::vector<ossimGpt>& pointList);
		void setViewCut(const ossimGrect& groundRect);
		void setViewCut(const ossimDrect& rect, bool imageSpaceFlag=false);
		void setViewCut(const ossimIrect& rect, bool imageSpaceFlag=false);
      
      void setImageCut(const ossimDpt* pointList, int pointListSize);
      void setImageCut(const std::vector<ossimDpt>& pointList);
      void setImageCut(const ossimIrect& rect);
      void setImageCut(const ossimDrect& rect);

      void setSharpen(ossim_uint32 kernelWidth,
                      ossim_float64 sigmaParameter);
      
      ossimImageSource* getImageRenderer();
      const ossimProjection* getViewProjection();

     // virtual void disconnectInputEvent(ossimConnectionEvent& event);
     // virtual void connectInputEvent(ossimConnectionEvent& event);
      
      ossimImageChain* getImageChain();
      
      ossimImageSource* getBandSelector();
      ossimImageSource* getImageCutter();
      ossimImageSource* getViewCutter();
      ossimImageSource* getScalarRemapper();
      
      /**
       * This is in image space before any transformations occur.  If the image has a projection model 
       * associated with it it will take the image center and project it to the ground coordinate.
       */
      bool getInputGroundCenter(ossimGpt& result)const;
      bool getInputCornerGroundPoints(ossimGpt& ul,
                                      ossimGpt& ur,
                                      ossimGpt& lr,
                                      ossimGpt& ll)const;
      /**
       * Returns the chains input bounding rect.
       */
      bool getInputBoundingRect(ossimDrect& rect)const;
      bool getInputBoundingRect(ossimIrect& rect)const;
      
      /**
       * This will check to see if its an image space transformation or not and if it is
       * will set the chain to image space only and will se the renderer to the new 
       * image view transformation.
       */
      void setImageViewTransform(ossimRefPtr<ossimImageViewTransform> imageViewTransform);
      
      void saveState(ossimKeywordlist& kwl,
                     const ossimString& prefix="");
   private:
      class EventListener;
      
      friend class EventListener;
     
      ossimBandSelector*      theBandSelector;
      ossimNullPixelFlip*      theNullPixelFlip;
      ossimImageSharpenFilter* theImageSharpenFilter;
      ossimPolyCutter*        theImageCutter;
      ossimHistogramRemapper* theHistogramRemapper;
      ossimCacheTileSource*   theCache;
      ossimImageRenderer*     theRenderer;  
      ossimCacheTileSource*   theCache2;
      ossimGeoPolyCutter*     theViewCutter;
      ossimPolyCutter*        theViewImageCutter;
      ossimScalarRemapper*    theScalarRemapper;
      
      ossimRefPtr<ossimImageChain> theImageChain;
      EventListener*               theEventListener;
      
      ossimRefPtr<ossimImageGeometry> theImageGeometry;
      
      /**
       * We will allow the filter to be sharpen in image or view space
       * depending on how it is set and used.
       */
	};
}
#endif
