// class ossimHarrisCorner : tile source
// computes harris corners using an image chain
// calcultes x and y derivatives, then forms three 2nd order products, smoothg them and combines to form a cornerness map
// then selects local maxima above a certain threshold
//
// INPUT:  one band image
// OUTPUT: one band, ossim_float64
//         corners are the only non NULL pixels (non OSSIM_DBL_NAN). value=cornerness
// PROPERTIES
//      K  = harris cornerness parameter
//         min_cornerness = minimum cornerness
//         smoooth_radius = gaussian smoothing standard deviation
//

#ifndef ossimHarrisCorners_HEADER
#define ossimHarrisCorners_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/imaging/ossimEquationCombiner.h>
#include "ossimExtremaFilter.h"
#include "ossimDensityReducer.h"
#include "ossimMultiplier.h"
#include "ossimSquareFunction.h"
#include "ossimRegistrationExports.h"


class OSSIM_REGISTRATION_DLL ossimHarrisCorners : public ossimImageSourceFilter
{
public:
  /**
   *own methods
   */
   ossimHarrisCorners();
   virtual ~ossimHarrisCorners();

   inline ossim_float64 getK()const { return theK; }
   void setK(const ossim_float64& v);

   inline ossim_float64 getGaussStd()const { return theGaussStd; }
   void setGaussStd(const ossim_float64& v);

   inline ossim_float64 getMinCornerness()const { return theMinCornerness; }
   void setMinCornerness(const ossim_float64& v);

   inline ossim_float64 getDensity()const { return theDensity; }
   void setDensity(const ossim_float64& d);

  /**
   * inherited methods re-implemented due to type change in output
   */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;
   virtual ossimScalarType getOutputScalarType() const;
   virtual double getNullPixelValue(ossim_uint32 band=0)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
  /**
   * inherited methods
   */
   virtual void initialize();
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect &tileRect,ossim_uint32 resLevel=0);
   
   //! implemented to pass events to inner processes
   virtual void connectInputEvent(ossimConnectionEvent &event);
   virtual void disconnectInputEvent(ossimConnectionEvent &event);

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

   virtual bool loadState(const ossimKeywordlist& kwl,const char* prefix = 0);   
   virtual bool saveState(ossimKeywordlist& kwl,const char* prefix = 0)const;
   
protected:
   /**
    * protected methods
    */
   void initializeProcesses();
   ossimString getCornernessEquation()const;

  /**
   *parameters
   */
   ossim_float64 theK;
   ossim_float64 theGaussStd;
   ossim_float64 theMinCornerness; //not used now
   ossim_float64 theDensity; //corner density in pixel^-2

  /** 
   * subprocesses
   */
   ossimRefPtr<ossimCastTileSourceFilter> theCastP;
   ossimRefPtr<ossimConvolutionFilter1D>  theDxP;
   ossimRefPtr<ossimConvolutionFilter1D>  theDyP;
   ossimRefPtr<ossimMultiplier>           theProductP;
   std::vector<ossimRefPtr<ossimSquareFunction> >      theSquaresP;
   std::vector<ossimRefPtr<ossimImageGaussianFilter> >  theSmoothP;
   ossimRefPtr<ossimEquationCombiner>     theCornernessP;
   ossimRefPtr<ossimExtremaFilter>        theLocalMaxP;
   ossimRefPtr<ossimDensityReducer>       theFilterP;

TYPE_DATA
};

#endif /* #ifndef ossimHarrisCorners_HEADER */
