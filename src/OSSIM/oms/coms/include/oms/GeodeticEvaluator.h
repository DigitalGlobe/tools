#ifndef omsGeodeticEvaluator_HEADER
#define omsGeodeticEvaluator_HEADER
#include <oms/Constants.h>
#include <ossim/base/ossimGpt.h>

namespace oms
{
class OMSDLL GeodeticEvaluator
{
public:
   
   GeodeticEvaluator();
   virtual ~GeodeticEvaluator();

   double getHeightMSL(const ossimGpt& gpt)const;
   double getHeightEllipsoid(const ossimGpt& gpt)const;

   bool computeEllipsoidalDistAz(const ossimGpt& pt1,
                                   const ossimGpt& pt2,
                                   double daArray[])const;
   
protected:
   class PrivateData;
   PrivateData* thePrivateData;
};
}
#endif
