#ifndef ossimDensityReducer_HEADER
#define ossimDensityReducer_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include "ossimRegistrationExports.h"

/**
 * class ossimDensityReducer
 *
 * orders NON NULL elements in tile
 * and nullify weaker ones so that a maximum density of non NULL pixel is reached
 * MaxDensity is a real number, which should between 0 and 1 (unit: pixel^-2)
 * 
 * TBD:
 * -so far all bands are processed in parallel (slow) : define multi-band density reduction
 *   (I only use it for single band data so far)
 * -dealing with NULL values
 * -random pick for too small density values (to avoid no corners at all)
 */
class OSSIM_REGISTRATION_DLL ossimDensityReducer : public ossimImageSourceFilter
{
public:
   /**
    * own methods
    */
   ossimDensityReducer(ossimObject* owner=NULL);
   ossimDensityReducer(ossimImageSource* inputSource);
   ossimDensityReducer(ossimObject* owner, ossimImageSource* inputSource);
  
   inline const ossim_float64& getMaxDensity()const { return theMaxDensity; }
   inline void                 setMaxDensity(const ossim_float64& aMaxDensity) { theMaxDensity = aMaxDensity; }

   /**
    * inherited methods
    */
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix=0);
   
protected:
   ossimRefPtr<ossimImageData> theTile;
   ossim_float64                theMaxDensity; //! between 0 and 1 included
   
   template <class T>
   void runFilter(T dummyVariable, ossimRefPtr<ossimImageData> inputData);
   
TYPE_DATA
};


#endif
