#ifndef omsAdjustmentModel_HEADER
#define omsAdjustmentModel_HEADER
#include <oms/Constants.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimPointObservation.h>

namespace oms
{
class OMSDLL AdjustmentModel
{
public:
   AdjustmentModel(const std::string& report);
   virtual ~AdjustmentModel();

   bool addObservation(ossimPointObservation& obs);
   
   bool addMeasurement(const std::string& idm,
                       const ossimDpt& iPt,
                       const std::string& imgFile);

   bool initAdjustment();

   bool runAdjustment();

   bool isValid();
   
protected:
   class PrivateData;
   PrivateData* thePrivateData;
};
}
#endif
