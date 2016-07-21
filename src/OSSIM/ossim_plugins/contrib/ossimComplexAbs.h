#ifndef ossimComplexAbs_HEADER
#define ossimComplexAbs_HEADER
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimComplexAbs : public ossimImageSourceFilter
{
public:
   ossimComplexAbs(ossimObject* owner=NULL);
   ossimComplexAbs(ossimImageSource* inputSource,unsigned int outputbands=1);
   ossimComplexAbs(ossimObject* owner,
                        ossimImageSource* inputSource,unsigned int outputbands=1);
                      
   virtual ~ossimComplexAbs();
   ossimString getShortName()const
      {
         return ossimString("ComplexAbs");
      }
   
   ossimString getLongName()const
      {
         return ossimString("Get absolute value for Complex data");
      }
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual void allocate();
   
   ossim_uint32 getNumberOfOutputBands() const;
   virtual bool isSourceEnabled()const;
 
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   
   template<class T>
   void runAbsCalc(T,
                     ossimRefPtr<ossimImageData> inputData,
                     ossimRefPtr<ossimImageData> outputData);
                     
   ossim_uint32 theOutputBandCount;
   ossim_uint32 theInputBandCount; 
TYPE_DATA
};

#endif
