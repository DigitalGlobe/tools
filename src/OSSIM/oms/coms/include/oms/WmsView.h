#ifndef omsWmsView_HEADER
#define omsWmsView_HEADER 1
#include <oms/Constants.h>
#include <string>
#include <ossim/base/ossimDrect.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/imaging/ossimImageGeometry.h>
class ossimMapProjection;
namespace oms
{
   class OMSDLL WmsView : public ossimReferenced
   {
   public:
      WmsView();
      virtual ~WmsView();
      
      bool setProjection(const std::string& srsCode);
      ossimRefPtr<ossimImageGeometry> getImageGeometry();
      
      /**
       * This is a BBOX from WMS.  The min max extents are assumed to be within the units of the projector.  So
       * if EPSG 4326 then it is degrees.  If its a UTM srs then the extends should be in meters.
       *
       * The width and height are used to set the GSD to match the output image required and then the extents are used 
       * to adjust the image space rect to map to 0,0 upper left corner
       */
      bool setViewDimensionsAndImageSize(double minX,
                                         double minY,
                                         double maxX,
                                         double maxY,
                                         int width,
                                         int height);
      
      ossimDrect getViewImageRect()const;
      
      double getScaleChangeFromInputToView(const ossimImageGeometry* inputGeometry)const;
   protected:
      ossimRefPtr<ossimImageGeometry> m_geometry;
      ossimDrect m_viewRect;
      
   };
}
#endif

