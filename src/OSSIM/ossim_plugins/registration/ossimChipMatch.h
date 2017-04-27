// class ossimChipMatch : Image Combiner
//
// INPUTS:
// - input image (slave) : one band
// - reference image (master) : one band
// - feature centers map
//    = image of centers of chips (center= non NULL, > 2.0, pixels), e.g. harris corner map
//   that image should be co-registered with the master image
// OPERATION:
//  matches master chips to slave chips and stores score + displacement.
//  uses normalized cross correlation
// PARAMETERS:
//  accuracy of slave positions (meters)
//  chip window size(pixels)
// OUTPUT:
//  normalized floating point image (0.0<->1.0) with chip matching intensity
//
// FUTURE:
//  handle NULL pixels
//  add matching on boundaries 
//  use vector features for matching positions
//  use vector tiles in Ossim for parallelism
//
// created by Frederic Claudel, EORU - CSIR - Aug 2005

#ifndef ossimChipMatch_HEADER
#define ossimChipMatch_HEADER

#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimTDpt.h>
#include <vector>
#include "ossimRegistrationExports.h"

#define OSSIM_CHIPMATCH_SLAVEACCURACY_PROPNAME "SlaveAccuracy"
#define OSSIM_CHIPMATCH_PIXELRADIUS_PROPNAME "PixelRadius"
#define OSSIM_CHIPMATCH_MINNCC_PROPNAME "MinimumNCC"

class ossimNCC_FFTW;

class OSSIM_REGISTRATION_DLL ossimChipMatch : public ossimImageCombiner
{
public:
   //own public methods
   ossimChipMatch();
   ossimChipMatch(ossimObject *owner,         
                               ossimImageSource *centers,
                               ossimImageSource *master, 
                               ossimImageSource *slave   );
   virtual ~ossimChipMatch();

   inline void setSlaveAccuracy(ossim_float64 acc) { theSlaveAccuracy=acc; } //unit: meters
   inline ossim_float64 getSlaveAccuracy()const { return theSlaveAccuracy; }

   inline void setMasterRadius(ossim_uint32 r) { theMRadius=r; } //unit : pixels
   inline ossim_uint32 getMasterRadius()const { return theMRadius; }

   inline void setBias(const ossimDpt& aBias) { theBias=aBias; } //using current projection unit & axes
   inline const ossimDpt& getBias()const { return theBias; }

   inline void setMinNCC(ossim_float64 m) { theMinNCC=m; } //unitless (between -1.0 and 1.0)
   inline ossim_float64 getMinNCC()const { return theMinNCC; }
   
   virtual const std::vector<ossimTDpt>& getFeatures(const ossimIrect &rect, ossim_uint32 resLevel=0); //vector method for getTile
   
   //inherited public methods
   virtual void                        initialize();
   virtual bool                        canConnectMyInputTo(ossim_int32 index,const ossimConnectableObject* object)const;
   virtual double                      getNullPixelValue(ossim_uint32 band)const;
   virtual double                      getMinPixelValue(ossim_uint32 band=0)const;
   virtual double                      getMaxPixelValue(ossim_uint32 band=0)const;
   virtual ossimScalarType 	         getOutputScalarType()const;
   virtual ossim_uint32                getNumberOfOutputBands()const;
   virtual ossimIrect                  getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual ossim_uint32 getTileWidth() const;
   virtual ossim_uint32 getTileHeight() const;

   virtual void getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result)const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;

   virtual ossimRefPtr<ossimImageData> getTile (const ossimIrect &rect, ossim_uint32 resLevel=0);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

protected:
   bool runMatch(const ossimIrect &rect, ossim_uint32 resLevel=0);
   void getMaxCorrelation(ossimRefPtr<ossimImageData> Mchip, ossimRefPtr<ossimImageData> Schip, 
                                  double* pdispx, double* pdispy, double* pcor);

   std::vector<ossimTDpt>      theTies;
   ossim_float64               theSlaveAccuracy;
   ossim_uint32                theMRadius;
   ossimDpt                    theBias;
   ossim_float64               theMinNCC;
   ossimNCC_FFTW*              theNCCengine;
   ossimRefPtr<ossimImageData> theTile;
   //static data
   static double theLMS[6*9];

TYPE_DATA
};
#endif
