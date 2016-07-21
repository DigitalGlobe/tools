/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/
#ifndef omsWmsMap_HEADER
#define omsWmsMap_HEADER 1
#include <oms/Constants.h>
#include <oms/WmsView.h>
#include <ossim/base/ossimReferenced.h>

class ossimImageGeometry;
namespace oms
{
   class OMSDLL WmsMap : public ossimReferenced
   {
   public:
      WmsMap();
      virtual ~WmsMap();
//      void addFile( const std::string& filename );
//      void addFile( const std::string& filename, const std::string& entryId );
//      void addFile( const std::string& filename, int entryNumber );
//      void getMap(const std::vector<std::string>& filename,
//                  const std::string& srsCode,
//                  double minX, double minY,
//                  double maxX, double maxY,
//                  int width, int height, ossim_int8* buffer );
	   
	   bool addFile(const std::string& filename,
                   int entryIdx,
                   ossimImageGeometry* geomOverride=0);
//					const std::string& histogramFile,
//					const std::string& stretchType);
  
      /**
       * Sequence of name value pairs.
       * 
       * stretch_mode  
       *    values - linear_auto_min_max, linear_1std_from_mean, linear_2std_from_mean, linear_3std_from_mean
       * stretch_region
       *    values - viewport, global
       * sharpen_width
       *    values -  double precision value that indicates the kernel width of the stretch
       * sharpen_sigma
       *    values - double precision to use in the gaussian for the sharpen.
       */
      void setChainParameters(const ossimKeywordlist& values, int idx = -1);
      void setChainParametersPerImage(const std::vector<ossimKeywordlist>& paramsPerChain);
//      void setGlobalStretchType(const std::string& stretchType);
//	   void setViewportStretchType(const std::string& stretchType);

      void getMap(const std::string& srsCode,
                  double minX, double minY,
                  double maxX, double maxY,
                  int width,
                  int height,
                  ossim_int8* buffer);
      
/*      
	   static bool createMap(const std::string& csvFilenames,
                            const std::string& csvEntries,
                            const std::string& csvHistogramFile,
                            const std::string& csvStretchType,
                            const std::string& srsCode,
                            const std::string& viewportStretchType,
                            double minX, double minY,
                            double maxX, double maxY,
                            int width,
                            int height,
                            ossim_int8* buffer );
 */
      static void getUnprojectedMap(
                                    const std::string& filename,
                                    int entryIdx,
                                    double scale,
                                    double startSample, double endSample,
                                    double startLine, double endLine,
                                    ossim_int8* buffer,
                                    const ossimKeywordlist& options);
      void cleanUp();
   protected:
      class PrivateData;
      friend class PrivateData;

      bool setupView( const std::string& srsCode,
                     double minX, double minY,
                     double maxX, double maxY,
                     int width, int height );

      PrivateData* thePrivateData;

   };
}
#endif

