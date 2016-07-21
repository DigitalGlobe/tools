#include "ossimUnitTestPolyCutter.h"
#include <ossim/base/ossimStringProperty.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimUnitTestPolyCutter, "ossimUnitTestPolyCutter", ossimPolyCutter);

ossimUnitTestPolyCutter::ossimUnitTestPolyCutter()
   :theSeed(0),
    theNumberOfPoints(10)
{
}

void ossimUnitTestPolyCutter::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString tempName = property->getName();
   if(tempName == "seed")
   {
      
      theSeed = property->valueToString().toUInt32();

      recompute();
      return;
   }
   else if(tempName == "number_of_points")
   {
      theNumberOfPoints = property->valueToString().toUInt32();
      recompute();
      return;
   }
   
   ossimPolyCutter::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimUnitTestPolyCutter::getProperty(const ossimString& name)const
{
   if(name == "seed")
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name,
                                                            ossimString::toString(theSeed));
      prop->setNumericType(ossimNumericProperty::ossimNumericPropertyType_UINT);
      prop->setReadOnlyFlag(false);
      prop->setFullRefreshBit();
      return prop;
   }
   if(name == "number_of_points")
   {
      ossimNumericProperty* prop = new ossimNumericProperty(name,
                                                            ossimString::toString(theNumberOfPoints),
                                                            10,
                                                            500);
      prop->setNumericType(ossimNumericProperty::ossimNumericPropertyType_UINT);
      prop->setReadOnlyFlag(false);
      prop->setFullRefreshBit();
      return prop;
   }
   
   return ossimPolyCutter::getProperty(name);
}

void ossimUnitTestPolyCutter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimPolyCutter::getPropertyNames(propertyNames);
   propertyNames.push_back("seed");
   propertyNames.push_back("number_of_points");
}

void ossimUnitTestPolyCutter::recompute()
{
   thePolygonList.clear();
   computeBoundingRect();
   
   
   ossimPolygon poly;
   
   ossimIrect rect = getBoundingRect();
   ossimIpt midPoint = rect.midPoint();
   
   double radius = ossimMin(rect.width(),
                            rect.height())/2.0;
   
   srand(theSeed);
   //
   // calculate a random circle radius
   // x = r*cos(theta);
   // y = r*sin(theta);
   //
   ossim_float32 idxDegree = 0;
   ossim_float32 increment = 360.0/theNumberOfPoints;
   for(idxDegree = 0; idxDegree < 360; idxDegree+=increment)
   {
      double tr = ((rand()/(double)RAND_MAX)*(radius/2.0));
      
      ossimDpt pt((tr+(radius/2.0))*cosd(idxDegree),
                  (tr+(radius/2.0))*sind(idxDegree));
      
      pt += midPoint;
      poly.addPoint(pt);
   }
   poly.checkOrdering();
   if(poly.getOrdering()==OSSIM_COUNTERCLOCKWISE_ORDER)
   {
      poly.reverseOrder();
   }
   addPolygon(poly);
}

bool ossimUnitTestPolyCutter::saveState(ossimKeywordlist& kwl,
                                        const char* prefix)const
{
   return ossimPolyCutter::saveState(kwl, prefix);
}
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
bool ossimUnitTestPolyCutter::loadState(const ossimKeywordlist& kwl,
                                        const char* prefix)
{
   return ossimPolyCutter::loadState(kwl, prefix);
   
}
