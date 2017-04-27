#ifndef ossimSPOTNatural_HEADER
#define ossimSPOTNatural_HEADER
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/base/ossimString.h>
#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimSPOTNatural : public ossimImageSourceFilter
{
public:
   ossimSPOTNatural(ossimObject* owner=NULL);
   ossimSPOTNatural(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   ossimSPOTNatural(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   virtual ~ossimSPOTNatural();
   ossimString getShortName()const
      {
         return ossimString("SPOTNatural");
      }
   
   ossimString getLongName()const
      {
         return ossimString("SPOT False Natural Colour Generation");
      }
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual void allocate();
   
   ossim_uint32 getNumberOfOutputBands() const;
 
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
   void runNatColour(T,
                     ossimRefPtr<ossimImageData> inputData,
                     ossimRefPtr<ossimImageData> outputData);
                     
   double theC1;
   double theC2;
   double theC3;

TYPE_DATA
};

#endif
