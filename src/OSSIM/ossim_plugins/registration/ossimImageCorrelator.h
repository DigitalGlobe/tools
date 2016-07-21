// ossimImageCorrelator
// class for getting tie-points from a master/slave image pair
//
// TODO : generate one file only : XML or Tabulated Text
// TODO : change TieGPtSet to a generic TiePtSet
// TODO : increase speed

#ifndef ossimImageCorrelator_HEADER
#define ossimImageCorrelator_HEADER

#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/base/ossimTDpt.h>
#include <ossim/base/ossimTieGptSet.h>
#include "ossimRegistrationExports.h"
#include "ossimHarrisCorners.h"
#include "ossimTieGenerator.h"
#include "ossimChipMatch.h"
#include <vector>

class ossimImageGeometry;
class ossimMapProjection;
class ossimListenerManager;

class OSSIM_REGISTRATION_DLL ossimImageCorrelator :
    public ossimOutputSource,
    public ossimProcessInterface
{
public:
   ossimImageCorrelator();
   virtual ~ossimImageCorrelator();

   //accessors to parms
   inline void               setMaster(const ossimFilename& m) { theMaster=m; }
   inline const ossimFilename& getMaster()const { return theMaster; }
   inline void               setSlave(const ossimFilename& s) { theSlave=s; }
   inline const ossimFilename& getSlave()const { return theSlave; }
   inline void               setMasterBand(ossim_uint32 b) { theMasterBand=b; }
   inline ossim_uint32       getMasterBand()const { return theMasterBand; }
   inline void               setSlaveBand(ossim_uint32 b) { theSlaveBand=b; }
   inline ossim_uint32       getSlaveBand()const { return theSlaveBand; }
   inline void               setScaleRatio(const ossim_float64& r) { theScaleRatio=r; }
   inline ossim_float64      getScaleRatio()const { return theScaleRatio; }
   inline void               setSlaveAccuracy(const ossim_float64& a) { theSlaveAccuracy=a; }
   inline ossim_float64      getSlaveAccuracy()const { return theSlaveAccuracy; }
   inline void               setProjectionType(const ossimString& p) { theProjectionType=p; }
   inline const ossimString& getProjectionType()const { return theProjectionType; }
   inline void               setCornerDensity(const ossim_float64& a) { theCornerDensity=a; }
   inline ossim_float64      getCornerDensity()const { return theCornerDensity; }
   inline void               setMasterPointProj(const ossimString& p) { theMasterPointProj=p; }
   inline const ossimString& getMasterPointProj()const { return theMasterPointProj; }
   inline void               setSlavePointProj(const ossimString& p) { theSlavePointProj=p; }
   inline const ossimString& getSlavePointProj()const { return theSlavePointProj; }
   inline void               setTemplateRadius(const ossim_uint32& r) { theTemplateRadius = r; }
   inline ossim_uint32       getTemplateRadius()const { return theTemplateRadius; }
   inline void               setMinCorrel(const ossim_float64& c) { theMinCorrel=c; }
   inline ossim_float64      getMinCorrel()const { return theMinCorrel; }   
   
   inline bool hasRun()const { return theHasRun; }

   // inherited methods
   virtual bool isOpen() const;
   virtual bool open();
   virtual void close();

   virtual bool  execute(); //also creates tie point file
   virtual       ossimObject* getObject()      { return this; }
   virtual const ossimObject* getObject()const { return this; }
   virtual       ossimObject* getObjectInterface() { return this; }

   virtual ossimListenerManager* getListenerManagerInterface() { return this; }
   virtual bool addListener(ossimListener *listener);
   virtual bool removeListener(ossimListener *listener);

   virtual bool canConnectMyInputTo(ossim_int32 inputIndex,const ossimConnectableObject* object)const { return false; } //TBC : so far no input

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:  

   ossimString         getRole() const;
   ossimImageHandler*  getProjectionHandler();
   
   ossimRefPtr<ossimImageGeometry> getOutputImageGeometry();

   ossimMapProjection* getOutputProjection();
   
   bool buildRenderer(
      ossimImageChain* chain,
//         ossimImageSource* source, 
         ossimMapProjection* outProjection, 
         ossimImageRenderer* renderer,
         const ossimFilterResampler::ossimFilterResamplerType& stype =  ossimFilterResampler::ossimFilterResampler_CUBIC
         )const;

   void computeDispStats(const vector<ossimTDpt>& tp)const;
   void rejectOutliersMedian(const vector<ossimTDpt>& tp, vector<ossimTDpt>& ftp, const ossim_float64& relError);
   void getMedianInlier1D(const vector<double>& sd, const ossim_float64& relError, ossim_float64& median, ossim_uint32& inlierCount);

   ossimFilename   theMaster;
   ossimFilename   theSlave;
   ossim_uint32  theMasterBand;
   ossim_uint32  theSlaveBand;
   ossim_float64 theScaleRatio;
   ossim_float64 theSlaveAccuracy;
   ossim_float64 theCornerDensity;
   ossimString   theProjectionType;
   ossimString   theMasterPointProj;
   ossimString   theSlavePointProj;
   ossim_uint32  theTemplateRadius; //in pixels
   ossim_float64 theMinCorrel;

   bool theHasRun; //to know whether execute has been run

   ossimRefPtr<ossimImageChain> theMChain;
   ossimRefPtr<ossimImageChain> theSChain;
   
   ossimRefPtr<ossimImageHandler>   handlerM;
   ossimRefPtr<ossimImageHandler>   handlerS;
   ossimRefPtr<ossimBandSelector>   theMasterBandSelector;
   ossimRefPtr<ossimBandSelector>   theSlaveBandSelector;
   ossimRefPtr<ossimImageRenderer>  rendererM;
   ossimRefPtr<ossimImageRenderer>  rendererS;
   ossimRefPtr<ossimHarrisCorners>  cornerDetector;
   std::vector<ossimRefPtr<ossimCastTileSourceFilter> > caster;
   ossimRefPtr<ossimChipMatch>      matcher;
   ossimRefPtr<ossimTieGenerator>   generator;
   ossimTieGptSet       theTset;

   //! Disallow operator=
   const ossimImageCorrelator& operator=(const ossimImageCorrelator& rhs) {return rhs;}

TYPE_DATA
};

#endif //ossimImageCorrelator_HEADER
