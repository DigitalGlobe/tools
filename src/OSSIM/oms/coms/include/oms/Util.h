//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Utility class for oms library conversion to from ossim library.
//----------------------------------------------------------------------------
// $Id: Util.h 20144 2011-10-13 15:46:00Z gpotts $
#ifndef omsUtil_HEADER
#define omsUtil_HEADER 1

#include <oms/Constants.h>
#include <ossim/base/ossimGrect.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <map>
#include <string>
#include <vector>


//---
// Forward class declarations outside of namespace oms.
//---
class ossimIpt;
class ossimIrect;
class ossimString;
class ossimKeywordlist;
class ossimImageSource;
namespace oms
{
   //---
   // Forward class declarations inside of namespace oms.
   //---
   class ImageOutputFormat;
   class Ipt;
   class Irect;

   class OMSDLL Util
   {
   public:

      static bool isAngularUnit(const ossimUnitType& unitType);

      /**
       * @brief ossimKeywordlist to a std::map<std::string, std::string>
       * @param in ossimKeywordlist to convert
       * @param out converted std::map<std::string, std::string>
       */
      static void ossimToOms(const ossimKeywordlist& in,
                             std::map<std::string, std::string>& out);



      /**
       * @brief Convert mime types like "image/jpeg" to ossim writer string.
       * @param in String to convert
       * @param out converted string.
       */
      static void mimeToOssimWriter(const std::string& in, ossimString& out);

      /**
       * @brief Appends an omsImageOutputFormat object to the list for each
       * image writer.
       * @param list The list to append to.
       * @note This does not clear the list passed to it, only adds to it.
       */
      static void getOutputFormats(std::vector<oms::ImageOutputFormat>& list);

      /**
       * @bried Returns true if ossim can open the file.
       * @return true if ossim can open, false if not.
       */
      static bool canOpen(const std::string& file);

      static const ossimProjection* findProjectionConst(ossimConnectableObject* input);
      
      /**
       * Will use the passed in ground rectangle to create a projection.
       * For example, if it's UTM it will align the zone that best fits the region.
       */
      static ossimProjection* createProjection(const std::string& type,
                                               const ossimGrect& rect);

      /**
       * Will use the passed in ground point to create a projection.
       * For example, if it's UTM then the best zone is picked from  the ground point
       */
      static ossimProjection* createProjection(const std::string& type,
                                        const ossimGpt& groundPoint);

      static ossimProjection* createProjection(ossimImageHandler* handler);


      /**
       * Will set the gsd and origin of the new projection by using the input source information
       */
      static ossimProjection* createViewProjection(ossimImageSource* inputSource,
                                                   const std::string& projectionType=std::string(""));
      static ossimProjection* createViewProjection(ossimConnectableObject* inputSource,
                                                   const std::string& projectionType=std::string(""));

      /**
       * Set all view interfaces to the common view.  If you want all views to clone the view then a copy
       * of the passed in view is made and each view interface will receive their own copy.
       */
      static void setAllViewProjections(ossimConnectableObject* input,
                                        ossimProjection* proj,
                                        bool cloneViewFlag=true);
      static void setAllViewGeometries(ossimConnectableObject* input,
                                       ossimObject* geom,
                                       bool cloneViewFlag=true);
      
      static void updateProjectionToFitOutputDimensions(ossimProjection* projectionToUpdate,
                                                        const ossimIrect& inputBounds,
                                                        ossim_uint32 outputWidth,
                                                        ossim_uint32 outputHeight,
                                                        bool keepAspectFlag=true);

      static void computeGroundPointsFromCenterRadius(std::vector<ossimGpt>& result,
                                                      const ossimProjection* proj,
                                                      const ossimGpt& gpt,
                                                      const double& radius,
                                                      const ossimUnitType& radiusUnits = OSSIM_METERS);

      static void computeGroundRect(ossimGrect& result,
                                    const ossimProjection* proj,
                                    const ossimDrect& rect);

      static void computeGroundRect(ossimGrect& result,
                                    const ossimProjection* proj,
                                    const ossimIrect& rect);

      static void computeCenterGroundPoint(ossimGpt& result,
                                           const ossimProjection* proj,
                                           const ossimIrect& rect);

      static void computeCenterGroundPoint(ossimGpt& result,
                                           const ossimProjection* proj,
                                           const ossimDrect& rect);

      static void computeGroundPoints(std::vector<ossimGpt>& result,
                                      const ossimProjection* proj,
                                      const ossimIrect& rect);

      static void computeGroundPoints(std::vector<ossimGpt>& result,
                                      const ossimProjection* proj,
                                      const ossimDrect& rect);

      static ossimImageSource* newEightBitImageSpaceThumbnailChain(ossimImageSource* inputSource,
                                                                   int xRes,
                                                                   int yRes,
                                                                   const std::string& histogramFile,
                                                                   const std::string& stretchType,
                                                                   bool keepAspectFlag);

      static bool writeImageSpaceThumbnail(const std::string& inputFile,
                                           int entryId,
                                           const std::string& outFile,
                                           const std::string& writerType,
                                           int xRes,
                                           int yRes,
                                           const std::string& histogramFile,
                                           const std::string& stretchType,
                                           bool keepAspectFlag);
      
      static void getEntryList(std::vector<ossim_uint32>& entryIds, 
                               const std::string& filename);
      
      /**
       * Given a relative point from center normalized it will calculate a rotation relative
       * to the positive Y axis.  So if you have realtive point = <1, 0> it will return 90.0 degrees
       * relative to the Y positive axis.
       */
      //static double calculateRotationRelativeToY(const ossimDpt& relativePoint);
      /**
       * Given a relative point from center normalized it will calculate a rotation relative
       * to the positive Y axis.  So if you have realtive point = <1, 0> it will return 0.0 degrees
       * relative to the Y positive axis.
       */
      //static double calculateRotationRelativeToX(const ossimDpt& relativePoint);
      
      /**
       *  Will calculate the heading using the images input projection.  It will give you
       * a positive rotation from 0 to 360 relative to the Y axis.
       */
      static double imageHeading(const std::string& filename, ossim_int32 entryId=-1);
      
      static ossimRefPtr<ossimImageGeometry> createBilinearModel(std::vector<ossimDpt>& imagePoints,
                                                                 std::vector<ossimGpt>& groundPoints);
   };
} // End of namespace oms.

#endif /* End of #ifndef omsUtil_HEADER */
