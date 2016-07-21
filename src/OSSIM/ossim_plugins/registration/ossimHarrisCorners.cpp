#include "ossimHarrisCorners.h"
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimConvolutionFilter1D.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageGaussianFilter.h>
#include <ossim/imaging/ossimEquationCombiner.h>
#include "ossimExtremaFilter.h"
#include "ossimDensityReducer.h"
#include "ossimSquareFunction.h"
#include "ossimMultiplier.h"

RTTI_DEF1(ossimHarrisCorners, "ossimHarrisCorners", ossimImageSourceFilter);

//! property names
static const char* PROPERTYNAME_K             = "K";
static const char* PROPERTYNAME_GAUSSSTD      = "GaussStd";
static const char* PROPERTYNAME_MINCORNERNESS = "MinCornerness";
static const char* PROPERTYNAME_DENSITY       = "Density";

static double DER_KERNEL[3] = {-0.5,0.0,0.5};

ossimHarrisCorners::ossimHarrisCorners()
   : ossimImageSourceFilter(),
     theK(0.05),
     theGaussStd(1.0),
     theMinCornerness(0.0),
     theDensity(0.003)
{
   //create sub-processes
   // ingredients: 
   // 2x  3x3ConvolutionFilter
   // 3x  EquationCombiner - for 2nd order products  (a square and a product)
   // 3x  EquationCombiner - for gaussian smoothing  (clones)
   // 1x  EquationCombiner - for combining everything
   // 1x  ExtremaFilter    - for local maxima, 8 neighbours, strict
   // 1x  DensityReducer   - for spatially filtering local maxima
   
   theCastP       = new ossimCastTileSourceFilter();
   theDxP         = new ossimConvolutionFilter1D();
   theDyP         = new ossimConvolutionFilter1D();
   theProductP    = new ossimMultiplier();
   theSquaresP.push_back(new ossimSquareFunction);
   theSquaresP.push_back(new ossimSquareFunction);
   theSmoothP.push_back(new ossimImageGaussianFilter);
   theSmoothP.push_back(new ossimImageGaussianFilter);
   theSmoothP.push_back(new ossimImageGaussianFilter);
   theCornernessP = new ossimEquationCombiner();
   theLocalMaxP   = new ossimExtremaFilter();
   theFilterP     = new ossimDensityReducer();

   //initialize parameters for sub-processes
   theCastP->setOutputScalarType(OSSIM_FLOAT64); //float64 or double??

   theDxP->setKernel(vector<ossim_float64>(DER_KERNEL,DER_KERNEL+3)); //hard coded
   theDxP->setCenterOffset(1); //hard coded
   theDxP->setIsHorizontal(true); //hard coded
   theDxP->setStrictNoData(true);

   theDyP->setKernel(vector<ossim_float64>(DER_KERNEL,DER_KERNEL+3));
   theDyP->setCenterOffset(1); //hard coded
   theDyP->setIsHorizontal(false); //hard coded
   theDyP->setStrictNoData(true);
      
   for(int i=0;i<3;++i)
   {
     theSmoothP[i]->setGaussStd(theGaussStd);
     theSmoothP[i]->setStrictNoData(true);
   }

   theCornernessP->setEquation(getCornernessEquation());
   theCornernessP->setOutputScalarType(OSSIM_FLOAT64);

   theLocalMaxP->setIsMaximum(true);
   theLocalMaxP->setIsStrict(true);

   theFilterP->setMaxDensity(getDensity());

   //tie them all up
   theDxP->connectMyInputTo(0,theCastP.get());
   theDyP->connectMyInputTo(0,theCastP.get());

   theSquaresP[0]->connectMyInputTo(0,theDxP.get());
   theSquaresP[1]->connectMyInputTo(0,theDyP.get());

   theProductP->connectMyInputTo(0,theDxP.get());
   theProductP->connectMyInputTo(1,theDyP.get());

   theSmoothP[0]->connectMyInputTo(0,theSquaresP[0].get());
   theSmoothP[1]->connectMyInputTo(0,theProductP.get());
   theSmoothP[2]->connectMyInputTo(0,theSquaresP[1].get());

   for (int i=0;i<3;++i)
   {   
      theCornernessP->connectMyInputTo(i,theSmoothP[i].get());
   }

   theLocalMaxP->connectMyInputTo(0,theCornernessP.get());

   theFilterP->connectMyInputTo(0,theLocalMaxP.get());
}

ossimHarrisCorners::~ossimHarrisCorners()
{
   if(theCastP.valid())
   {
      ossim_uint32 idx = 0;
      theCastP->disconnect();
      theDxP->disconnect();
      theDyP->disconnect();
      theProductP->disconnect();
      for(idx = 0; idx < theSquaresP.size(); ++idx)
      {
         theSquaresP[idx]->disconnect();
         theSquaresP[idx] = 0;
      }
      for(idx = 0; idx < theSmoothP.size(); ++idx)
      {
         theSmoothP[idx]->disconnect();
         theSmoothP[idx] = 0;
      }
      theCornernessP->disconnect();
      theLocalMaxP->disconnect();
      theFilterP->disconnect();
      theSmoothP.clear();
      theCastP = 0;
      theDxP = 0;
      theDyP = 0;
      theCornernessP = 0;
      theLocalMaxP = 0;
      theFilterP = 0;
   }
}

void ossimHarrisCorners::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property) return;

   if (property->getName() == PROPERTYNAME_K) {
      ossimNumericProperty* sProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(sProperty)
      {
         setK(sProperty->asFloat64());
      }
   } else if (property->getName() == PROPERTYNAME_GAUSSSTD) {
      ossimNumericProperty* sProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(sProperty)
      {
         setGaussStd(sProperty->asFloat64());
      }
   } else if(property->getName() == PROPERTYNAME_MINCORNERNESS) {
      ossimNumericProperty* sProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(sProperty)
      {
         setMinCornerness(sProperty->asFloat64());
      }
   } else if(property->getName() == PROPERTYNAME_DENSITY) {
      ossimNumericProperty* sProperty = PTR_CAST(ossimNumericProperty,
                                                     property.get());
      if(sProperty)
      {
         setDensity(sProperty->asFloat64());
      }
   } else {
      ossimImageSourceFilter::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimHarrisCorners::getProperty(const ossimString& name)const
{
   if(name == PROPERTYNAME_K) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(getK()),0,0.25);
      property->setCacheRefreshBit();
      return property;
   } else if (name == PROPERTYNAME_GAUSSSTD) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(getGaussStd()),0.001,15);
      property->setCacheRefreshBit();
      return property;
   } else if (name == PROPERTYNAME_MINCORNERNESS) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(getMinCornerness()));
      property->setCacheRefreshBit();
      return property;
   } else if (name == PROPERTYNAME_DENSITY) {
      ossimNumericProperty* property = new ossimNumericProperty(name,ossimString::toString(getDensity()),0.0,1.0);
      property->setCacheRefreshBit();
      return property;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimHarrisCorners::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back(PROPERTYNAME_K);
   propertyNames.push_back(PROPERTYNAME_GAUSSSTD);
   propertyNames.push_back(PROPERTYNAME_MINCORNERNESS);
   propertyNames.push_back(PROPERTYNAME_DENSITY);
}

bool ossimHarrisCorners::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{   
   kwl.add(prefix,
           PROPERTYNAME_K,
           getK(),
           true);
   kwl.add(prefix,
          PROPERTYNAME_GAUSSSTD,
           getGaussStd(),
           true);
   kwl.add(prefix,
          PROPERTYNAME_MINCORNERNESS,
           getMinCornerness(),
           true); 
   kwl.add(prefix,
          PROPERTYNAME_DENSITY,
           getDensity(),
           true); 

   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimHarrisCorners::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* k = kwl.find(prefix, PROPERTYNAME_K);
   if(k)
   {      
      setK(ossimString(k).toDouble());
   } else {
      cerr<<"ossimHarrisCorners::loadState : missing property "<< PROPERTYNAME_K<<endl;
   }

   const char* gs = kwl.find(prefix, PROPERTYNAME_GAUSSSTD);
   if(gs)
   {      
      setGaussStd(ossimString(gs).toDouble());
   } else {
      cerr<<"ossimHarrisCorners::loadState : missing property "<< PROPERTYNAME_GAUSSSTD<<endl;
   }

   const char* mc = kwl.find(prefix, PROPERTYNAME_MINCORNERNESS);
   if(mc)
   {      
      setMinCornerness(ossimString(mc).toDouble());
   } else {
      cerr<<"ossimHarrisCorners::loadState : missing property "<< PROPERTYNAME_MINCORNERNESS<<endl;
   }
   const char* dd = kwl.find(prefix, PROPERTYNAME_DENSITY);
   if(dd)
   {      
      setDensity(ossimString(dd).toDouble());
   } else {
      cerr<<"ossimHarrisCorners::loadState : missing property "<< PROPERTYNAME_DENSITY<<endl;
   }
   
   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimHarrisCorners::setK(const ossim_float64& v)
{
   theK=v;
   //update processes
   theCornernessP->setEquation(getCornernessEquation());
}

void ossimHarrisCorners::setGaussStd(const ossim_float64& v)
{
   theGaussStd = v;
   //update processes
   for(int i=0;i<3;++i)
   {
      theSmoothP[i]->setGaussStd(theGaussStd);
   }
}

void ossimHarrisCorners::setMinCornerness(const ossim_float64& v)
{
   theMinCornerness = v;
   //TBC : no impact, not used
}

void ossimHarrisCorners::setDensity(const ossim_float64& v)
{
   theDensity = v;
   //update process
   theFilterP->setMaxDensity(getDensity());
}

void
ossimHarrisCorners::initialize()
{
   ossimImageSourceFilter::initialize();
   initializeProcesses();
}

ossimRefPtr<ossimImageData>
ossimHarrisCorners::getTile(const ossimIrect &tileRect,ossim_uint32 resLevel)
{
    if(theInputConnection)
    {
       if(isSourceEnabled())
       {
         return theFilterP->getTile(tileRect, resLevel);
       }
       return theInputConnection->getTile(tileRect, resLevel);
    }
    return ossimRefPtr<ossimImageData>();
}

void
ossimHarrisCorners::initializeProcesses()
{
   theCastP->initialize();
   theDxP->initialize();
   theDyP->initialize();
   theProductP->initialize();
   theSquaresP[0]->initialize();
   theSquaresP[1]->initialize();
   for (int i=0;i<3;++i)
   {
      theSmoothP[i]->initialize();
   }
   theCornernessP->initialize();
   theLocalMaxP->initialize();
   theFilterP->initialize();
}

void
ossimHarrisCorners::connectInputEvent(ossimConnectionEvent &event)
{
    ossimImageSourceFilter::connectInputEvent(event);
    if(getInput())
    {
       theCastP->connectMyInputTo(0, getInput());
       initializeProcesses();
    }
    else
    {
       theCastP->disconnectMyInput(0, false, false);
       initializeProcesses();
    }
}


void
ossimHarrisCorners::disconnectInputEvent(ossimConnectionEvent &event)
{
    ossimImageSourceFilter::disconnectInputEvent(event);
    if(getInput())
    {
       theCastP->connectMyInputTo(0, getInput());
       initializeProcesses();
    }
    else
    {
       theCastP->disconnectMyInput(0, false, false);
       initializeProcesses();
    }
}

ossimString
ossimHarrisCorners::getCornernessEquation()const
{
   return ossimString("(in[0]*in[2]-in[1]^2)-") + ossimString::toString(theK) + "*(in[0]+in[2])^2";
}

void
ossimHarrisCorners::getOutputBandList(std::vector<ossim_uint32>& bandList) const
{
   theFilterP->getOutputBandList(bandList);
}

ossimScalarType
ossimHarrisCorners::getOutputScalarType() const
{
   return theFilterP->getOutputScalarType();
}

double
ossimHarrisCorners::getNullPixelValue(ossim_uint32 band)const
{
   return theFilterP->getNullPixelValue(band);
}

double
ossimHarrisCorners::getMinPixelValue(ossim_uint32 band)const
{
   return theFilterP->getMinPixelValue(band);
}

double
ossimHarrisCorners::getMaxPixelValue(ossim_uint32 band)const
{
   return theFilterP->getMaxPixelValue(band);
}
