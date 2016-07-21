/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/
#include <oms/WmsView.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/imaging/ossimImageGeometry.h>

oms::WmsView::WmsView()
:m_geometry(new ossimImageGeometry())
{
}

oms::WmsView::~WmsView()
{
   m_geometry = 0;
}

bool oms::WmsView::setProjection(const std::string& srsCode)
{
   m_geometry->setProjection(ossimProjectionFactoryRegistry::instance()->createProjection(srsCode));
   m_geometry->setTransform(0);
   return (m_geometry->getProjection() != 0);
}


ossimRefPtr<ossimImageGeometry> oms::WmsView::getImageGeometry()
{
   return m_geometry.get();
}

bool oms::WmsView::setViewDimensionsAndImageSize(double minX,
                                                 double minY,
                                                 double maxX,
                                                 double maxY,
                                                 int width,
                                                 int height)
{
   bool result = false;
   ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(m_geometry->getProjection());
   if(mapProj)
   {
      result = true;
      double gsdX = (maxX-minX)/(width);
      double gsdY = (maxY-minY)/(height);
      
      if(mapProj->isGeographic())
      {
         ossimGpt origin = mapProj->origin();
         origin.latd(maxY);
         origin.lond(minX);
         mapProj->setDecimalDegreesPerPixel(ossimDpt(gsdX, gsdY));
         mapProj->update();
         
         mapProj->setUlTiePoints(origin);
         ossimDpt ul;
         
         mapProj->worldToLineSample(origin, ul);
         
         m_viewRect = ossimDrect(ul.x,
                                 ul.y ,
                                 ul.x + width-1,
                                 ul.y + height-1);
      }
      else
      {
         // for now I assume meter units, but later I will update to take on the units of the SRS code.  Right now, they typically
         // are meters.
         //
         mapProj->setMetersPerPixel(ossimDpt(gsdX, gsdY));
         mapProj->update();
         mapProj->setUlTiePoints(ossimDpt(minX, maxY));
         m_viewRect = ossimDrect(0,0,width-1.0, height-1.0);
      }
   }
   
   return result;
}

double oms::WmsView::getScaleChangeFromInputToView(const ossimImageGeometry* inputGeometry)const
{
   double result = 0.0;
   if(m_geometry.valid()&&!m_viewRect.hasNans()&&inputGeometry)
   {
      
      ossimGpt gp1, gp2, gp3, gp4;
      ossimDpt p1, p2, p3,p4;
      
      m_geometry->localToWorld(m_viewRect.ul(), gp1);
      m_geometry->localToWorld(m_viewRect.ur(), gp2);
      m_geometry->localToWorld(m_viewRect.lr(), gp3);
      m_geometry->localToWorld(m_viewRect.ll(), gp4);
      
      inputGeometry->worldToLocal(gp1, p1);
      inputGeometry->worldToLocal(gp2, p2);
      inputGeometry->worldToLocal(gp3, p3);
      inputGeometry->worldToLocal(gp4, p4);
      
      ossimIrect inputRect(p1, p2, p3, p4);
                           
      ossim_float64 maxInput = ossim::max(inputRect.width(), inputRect.height());
      ossim_float64 maxView  = ossim::max(m_viewRect.width(), m_viewRect.height());
      

      result = maxView/maxInput;
   }
   
   return result;
}

ossimDrect oms::WmsView::getViewImageRect()const
{
   return m_viewRect;
}

