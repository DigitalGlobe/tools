#ifndef ossimSquareFunction_HEADER
#define ossimSquareFunction_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include "ossimRegistrationExports.h"

/**
 * ossimSquareFunction : filter to raise to power of 2
 */
class OSSIM_REGISTRATION_DLL ossimSquareFunction : public ossimImageSourceFilter
{
public:
   ossimSquareFunction(ossimObject* owner=NULL);
   virtual ~ossimSquareFunction();

   virtual ossimString getShortName()const{return ossimString("SquareFunction");}
   virtual ossimString getLongName()const{return ossimString("raises to power of two");}

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);

   virtual void initialize();

   virtual double getNullPixelValue(ossim_uint32 band=0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0)  const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)  const;

protected:

   template<class T>
   void applyFunction(T,
                      ossimRefPtr<ossimImageData> inputData,
                      ossimRefPtr<ossimImageData> outputData);

   void allocate();
   void clearNullMinMax();
   void computeNullMinMax();
   
   vector<double> theNullPixValue;
   vector<double> theMinPixValue;
   vector<double> theMaxPixValue;   
   
   ossimRefPtr<ossimImageData> theTile;                       

TYPE_DATA
};

#endif /* #ifndef ossimSquareFunction_HEADER */
