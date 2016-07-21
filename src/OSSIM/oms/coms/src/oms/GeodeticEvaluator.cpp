#include <oms/GeodeticEvaluator.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimGeodeticEvaluator.h>


class oms::GeodeticEvaluator::PrivateData
{
public:
   virtual ~PrivateData()
   {
   }
};


oms::GeodeticEvaluator::GeodeticEvaluator()
:thePrivateData(new PrivateData())
{
}


oms::GeodeticEvaluator::~GeodeticEvaluator()
{
   if(thePrivateData)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
}

double oms::GeodeticEvaluator::getHeightEllipsoid(const ossimGpt& gpt)const
{
   return ossimElevManager::instance()->getHeightAboveEllipsoid(gpt);
}

double oms::GeodeticEvaluator::getHeightMSL(const ossimGpt& gpt)const
{
   return ossimElevManager::instance()->getHeightAboveMSL(gpt);
}


//=============================================================================
// Input:  pt1     ground point 1
//         pt2     ground point 2
// Output: daArray [0] distance (m)
//                 [1] azimuth 1->2
//                 [2] azimuth 2->1  (back azimuth)
//=============================================================================
bool oms::GeodeticEvaluator::computeEllipsoidalDistAz(const ossimGpt& pt1,
                                                        const ossimGpt& pt2,
                                                        double daArray[])const
{
   bool computationOK = false;
   for (int k=0; k<3; ++k)
      daArray[k] = 0.0;

   if(!thePrivateData)
      return computationOK;

   double d;
   double az1;
   double az2;
   
   ossimGeodeticEvaluator geoEv;

   if (geoEv.inverse(pt1, pt2, d, az1, az2))
   {
      daArray[0] = d;
      daArray[1] = az1;
      daArray[2] = az2;
      computationOK = true;
   }

 
   return computationOK; 
}