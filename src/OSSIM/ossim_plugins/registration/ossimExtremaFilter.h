
#ifndef ossimExtremaFilter_HEADER
#define ossimExtremaFilter_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include "ossimRegistrationExports.h"

/**
 * class ossimExtremaFilter
 *
 * returns local extrema unchanged (can be maxima or minima, strict or not)
 * nullify other pixels
 * uses a 8 pixel neighborhood
 *
 * ToBeDone:
 *  -add vector tiles support
 *  -add different kind of neighborhoods  - mostly larger
 */
class OSSIM_REGISTRATION_DLL ossimExtremaFilter : public ossimImageSourceFilter
{
public:
  /**
   * own methods
   */
   ossimExtremaFilter(ossimObject* owner=NULL);
   ossimExtremaFilter(ossimImageSource* inputSource);
   ossimExtremaFilter(ossimObject* owner, ossimImageSource* inputSource);

   inline bool isMaximum()const { return theIsMaximum; }
   inline void setIsMaximum(bool aMax) { theIsMaximum = aMax; }
   
   inline bool isStrict()const { return theIsStrict; }
   inline void setIsStrict(bool aStrict) { theIsStrict = aStrict; }

  /**
   *inherited methods
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
   bool                        theIsMaximum;
   bool                        theIsStrict;
   
   void adjustRequestRect(ossimIrect& requestRect)const;
   
   template <class T>
   void runFilter(T dummyVariable,
                  ossimRefPtr<ossimImageData> inputData);
   
TYPE_DATA
};

#endif
