#ifndef ossimPlanetQtAlertLayer_HEADER
#define ossimPlanetQtAlertLayer_HEADER

#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/Image>
#include <ossimPlanet/ossimPlanetLayer.h>
#include <ossimPlanet/ossimPlanetId.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimRefPtr.h>
#include <OpenThreads/Mutex>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtAlertIconCallout : public osg::Drawable
{
   
public:   
   struct AlertInfo
   {
      ossimPlanetId theId;
      osg::Vec3d    theLandPosition;
   };
   ossimPlanetQtAlertIconCallout();
   
   /** Copy constructor using CopyOp to manage deep vs shallow copy.*/
   ossimPlanetQtAlertIconCallout(const ossimPlanetQtAlertIconCallout& iconCallout,
                                 const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);
            
   virtual osg::Object* cloneType()const
   {
      return new ossimPlanetQtAlertIconCallout();
   }

   virtual Object* clone(const osg::CopyOp& copyop) const
   {
      return new ossimPlanetQtAlertIconCallout(*this, copyop);
   }
        
   virtual bool isSameKindAs(const Object* obj)const
   {
      return dynamic_cast<const ossimPlanetQtAlertIconCallout*>(obj) != NULL;
   }
   
   virtual const char* libraryName()const
   {
      return "ossimPlanetQt";
   }
   
   virtual const char* className()const
   {
      return "ossimPlanetQtAlertIconCallout";
   }
   virtual void drawImplementation(osg::RenderInfo& state)const;

   virtual osg::BoundingBox computeBound()const;
   virtual void addOrUpdateAlert(const AlertInfo& info);
protected:
   
   ossimPlanetQtAlertIconCallout& operator = (const ossimPlanetQtAlertIconCallout&)
      {
         return *this;
      }
   
   virtual ~ossimPlanetQtAlertIconCallout();

   osg::ref_ptr<osg::Image>           theImage;
   unsigned short                     theStemHeight; 
   std::map<ossimPlanetId, AlertInfo> theAlerts;
   std::vector<unsigned char>         theLinePixelBuffer; 
};

class ossimPlanet;
class ossimPlanetQtAlertLayer : public ossimPlanetLayer
{
public:
   ossimPlanetQtAlertLayer();
   virtual void execute(const ossimPlanetAction& action)
   {
      ossimPlanetLayer::execute(action);
   }
   
   void removeAlert(const ossimPlanetId& id);
   void updateAlert(const ossimPlanetId& id,
                    const ossimGpt& gpt);
   ossimPlanetId addAlert(const ossimGpt& gpt);
   virtual void traverse(osg::NodeVisitor& nv);
protected:
   OpenThreads::Mutex                theMutex;
   std::map<ossimPlanetId, ossimGpt> theAlertsToAddOrUpdate;
   //ossimPlanetLand*                  theLand;
   ossimPlanetQtAlertIconCallout*    theDrawable;
   osg::ref_ptr<osg::Geode>          theGeode;
   ossimPlanet*                      thePlanet;
   ossimRefPtr<ossimGeoid >          theGeoid;
};
#endif
