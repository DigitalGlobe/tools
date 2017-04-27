#include <ossimPlanetQt/ossimPlanetQtAlertLayer.h>
#include <OpenThreads/ScopedLock>
#include <ossimPlanet/ossimPlanet.h>
//#include <ossimPlanet/ossimPlanetLand.h>
#include <ossimPlanet/ossimPlanetGeoRefModel.h>
#include <ossimPlanet/ossimPlanetIdManager.h>
#include <ossim/elevation/ossimElevManager.h>

#include <iostream>
#define _MAX_STEM_LENGTH 100
#define _STEM_R 246
#define _STEM_G 185
#define _STEM_B 0
// static unsigned char theLinePixelBuffer[_MAX_STEM_LENGTH * 3];

ossimPlanetQtAlertIconCallout::ossimPlanetQtAlertIconCallout()
{
   setSupportsDisplayList(false);
   theLinePixelBuffer.resize(100*3);
   theStemHeight = 30;
   for (int i = 0; i < _MAX_STEM_LENGTH; i +=3)
   {
      theLinePixelBuffer[i]   = _STEM_R;
      theLinePixelBuffer[i+1] = _STEM_G;
      theLinePixelBuffer[i+2] = _STEM_B;
   }
}

ossimPlanetQtAlertIconCallout::ossimPlanetQtAlertIconCallout(const ossimPlanetQtAlertIconCallout& iconCallout,
                                                             const osg::CopyOp& copyop)
   : osg::Drawable(iconCallout, copyop),
     theImage(iconCallout.theImage),
     theStemHeight(iconCallout.theStemHeight),
     theLinePixelBuffer(iconCallout.theLinePixelBuffer)
{
//    theLinePixelBuffer.resize(100*3);
}

ossimPlanetQtAlertIconCallout::~ossimPlanetQtAlertIconCallout()
{
   
}

void ossimPlanetQtAlertIconCallout::drawImplementation(osg::RenderInfo& /*state*/)const
{
   std::map<ossimPlanetId, AlertInfo>::const_iterator iter = theAlerts.begin();

   while(iter!=theAlerts.end())
   {
      glPushMatrix();
      glTranslated(iter->second.theLandPosition[0], iter->second.theLandPosition[1], iter->second.theLandPosition[2]);
      glRasterPos3d(0.0,0.0,0.0);
      glDrawPixels(1, theStemHeight, GL_RGB, GL_UNSIGNED_BYTE, &theLinePixelBuffer.front());
      glBitmap(0, 0, 0, 0, -2, theStemHeight, NULL);
      glDrawPixels(10, 10, GL_RGB, GL_UNSIGNED_BYTE, &theLinePixelBuffer.front());
      glPopMatrix();
      ++iter;
   }
}

void ossimPlanetQtAlertIconCallout::addOrUpdateAlert(const AlertInfo& info)
{
   std::map<ossimPlanetId, AlertInfo>::iterator iter = theAlerts.find(info.theId);

   if(iter == theAlerts.end())
   {
      theAlerts.insert(std::make_pair(info.theId, info));
   }
   else
   {
      iter->second = info; 
   }
}

osg::BoundingBox ossimPlanetQtAlertIconCallout::computeBound()const
{
   osg::BoundingBox bbox;

   return bbox;
}

ossimPlanetQtAlertLayer::ossimPlanetQtAlertLayer()
   :thePlanet(0),
    theGeoid(ossimGeoidManager::instance()->findGeoidByShortName("geoid1996"))
{
   theDrawable = new ossimPlanetQtAlertIconCallout;
   theGeode = new osg::Geode;
   theGeode->addDrawable(theDrawable);
   theGeode->setCullingActive(false);
   theDrawable->setSupportsDisplayList(false);
   addChild(theGeode.get());
   setCullingActive(false);
   
}

void ossimPlanetQtAlertLayer::removeAlert(const ossimPlanetId& /*id*/)
{
   
}

void ossimPlanetQtAlertLayer::updateAlert(const ossimPlanetId& /*id*/,
                                          const ossimGpt& /*gpt*/)
{
   
}

ossimPlanetId ossimPlanetQtAlertLayer::addAlert(const ossimGpt& gpt)
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   ossimPlanetId id = ossimPlanetIdManager::nextId();
   theAlertsToAddOrUpdate.insert(std::make_pair(id, gpt));
   return id;
}
 
void ossimPlanetQtAlertLayer::traverse(osg::NodeVisitor& nv)
{  
   switch(nv.getVisitorType())
   {
      case osg::NodeVisitor::UPDATE_VISITOR:
      {
         if(!thePlanet)
         {
            thePlanet = ossimPlanet::findPlanet(this);
         }

         if(thePlanet&&!theAlertsToAddOrUpdate.empty())
         {
            std::map<ossimPlanetId, ossimGpt>::iterator iter = theAlertsToAddOrUpdate.begin();
            ossimPlanetId id = iter->first;
            double h = ossimElevManager::instance()->getHeightAboveMSL(iter->second);
            if(!ossim::isnan(h))
            {
               if(theGeoid.valid()) h +=theGeoid->offsetFromEllipsoid(iter->second);
            }
            else
            {
               if(theGeoid.valid()) h =theGeoid->offsetFromEllipsoid(iter->second);
            }
            if(!ossim::isnan(h))
            {
               h = 0;
            }
            h /= thePlanet->model()->getNormalizationScale();
            osg::Vec3d llh(iter->second.latd(),
                           iter->second.lond(),
                           h);
            osg::Vec3d out;
            thePlanet->model()->forward(llh, out);
            ossimPlanetQtAlertIconCallout::AlertInfo info;
            info.theId = iter->first;
            info.theLandPosition = out;
            theDrawable->addOrUpdateAlert(info);
            theAlertsToAddOrUpdate.erase(iter);
         }
         break;
      }
      default:
      {
         break;
      }
   }
   
   osg::Group::traverse(nv);
}
