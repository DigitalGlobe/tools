/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/
#include <oms/SingleImageChain.h>
#include <oms/Util.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimNullPixelFlip.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimImageSharpenFilter.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimPolyCutter.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimGeoPolyCutter.h>
#include <ossim/imaging/ossimFilterResampler.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimRefreshEvent.h>
#include <ossim/base/ossimMultiResLevelHistogram.h>

class oms::SingleImageChain::EventListener : public ossimConnectableObjectListener
{
public:
   EventListener(oms::SingleImageChain* owner)
   :theOwner(owner)
   {
   }
   virtual void disconnectInputEvent(ossimConnectionEvent& event)
   {
   }
   //virtual void disconnectOutputEvent(ossimConnectionEvent& event);
   virtual void connectInputEvent(ossimConnectionEvent& event)
   {
      if(event.getObject() == theOwner->theImageChain.get())
      {
         ossimKeywordlist kwl;
         ossimImageSource* input = dynamic_cast<ossimImageSource*>(theOwner->theImageChain->getInput(0));
         if(input)
         {
            theOwner->theImageGeometry = input->getImageGeometry();
            theOwner->theImageChain->initialize();
         }
      }
   }
   //virtual void connectOutputEvent(ossimConnectionEvent& event);
   //virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);
   
protected:
   oms::SingleImageChain* theOwner;
};

oms::SingleImageChain::SingleImageChain()
{
   theEventListener     = new EventListener(this);
   theImageChain        = new ossimImageChain;
   theBandSelector      = new ossimBandSelector;
   theNullPixelFlip     = new ossimNullPixelFlip;
   theHistogramRemapper = new ossimHistogramRemapper;
   theImageCutter       = new ossimPolyCutter;
   theCache             = new ossimCacheTileSource;
   theRenderer          = new ossimImageRenderer;
   theCache2            = new ossimCacheTileSource;
   theViewCutter        = new ossimGeoPolyCutter;
   theViewImageCutter   = new ossimPolyCutter;

   theScalarRemapper    = new ossimScalarRemapper;
   theImageSharpenFilter = new ossimImageSharpenFilter;
   theBandSelector->setEnableFlag(false);
   theNullPixelFlip->setEnableFlag(false);
   theImageSharpenFilter->setEnableFlag(false);
   theImageCutter->setEnableFlag(false);
   theHistogramRemapper->setEnableFlag(false);
   theCache->setEnableFlag(false);
   theRenderer->setEnableFlag(true);
   theCache2->setEnableFlag(false);
   theScalarRemapper->setEnableFlag(false);
   theViewCutter->setEnableFlag(false);
   theViewImageCutter->setEnableFlag(false);
	
	theImageChain->add(theBandSelector);
   theImageChain->add(theNullPixelFlip);
	theImageChain->add(theImageSharpenFilter);
	theImageChain->add(theHistogramRemapper);
	theImageChain->add(theImageCutter);
	theImageChain->add(theCache);
	theImageChain->add(theRenderer);
	theImageChain->add(theCache2);
	theImageChain->add(theViewCutter);
	theImageChain->add(theViewImageCutter);
	theImageChain->add(theScalarRemapper);
   
   theImageChain->addListener(theEventListener);
}

oms::SingleImageChain::~SingleImageChain()
{
   if(theEventListener)
   {
      if(theImageChain.valid())
      {
         theImageChain->removeListener(theEventListener);
      }
      delete theEventListener;
      theEventListener = 0;
   }
   if(theImageChain.valid())
   {
      theBandSelector=0;
      theNullPixelFlip = 0;
      theImageSharpenFilter=0;
      theImageCutter = 0;
      theHistogramRemapper = 0;
      theCache = 0;
      theRenderer = 0;
      theCache2 = 0;
      theViewCutter = 0;
      theViewImageCutter = 0;
      theScalarRemapper = 0;
      theImageChain->disconnect();
      theImageChain = 0;
         
   }
   theImageGeometry = 0;
}

void oms::SingleImageChain::disableAll()
{
   if(theBandSelector)
      theBandSelector->setEnableFlag(false);
   if(theHistogramRemapper)
      theHistogramRemapper->setEnableFlag(false);
   if(theImageCutter)
      theImageCutter->setEnableFlag(false);
   if(theCache)
      theCache->setEnableFlag(false);
   if(theRenderer)
      theRenderer->setEnableFlag(false);
   if(theCache2)
      theCache2->setEnableFlag(false);
   if(theScalarRemapper)
      theScalarRemapper->setEnableFlag(false);
   if(theViewCutter)
      theViewCutter->setEnableFlag(false);
   if(theViewImageCutter)
      theViewImageCutter->setEnableFlag(false);
}

void oms::SingleImageChain::setNullPixelFlipEnableFlag(bool flag)
{
   if(theNullPixelFlip)
   {
      theNullPixelFlip->setEnableFlag(flag);
   }
}

void oms::SingleImageChain::setHistogramRemapperEnableFlag(bool flag)
{
   if(theHistogramRemapper)
      theHistogramRemapper->setEnableFlag(flag);
}

void oms::SingleImageChain::setImageRendererEnableFlag(bool flag)
{
   if(theRenderer)
      theRenderer->setEnableFlag(flag);
}

void oms::SingleImageChain::setImageCutterEnableFlag(bool flag)
{
	if(theImageCutter)
      theImageCutter->setEnableFlag(flag);
}

void oms::SingleImageChain::setViewCutterEnableFlag(bool flag)
{
	if(theViewCutter)
      theViewCutter->setEnableFlag(flag);
}

void oms::SingleImageChain::setCacheEnableFlag(bool flag)
{
	if(theCache) theCache->setEnableFlag(flag);
	if(theCache2) theCache2->setEnableFlag(flag);
}

void oms::SingleImageChain::setBandSelectorEnableFlag(bool flag)
{
	if(theBandSelector) theBandSelector->setEnableFlag(flag);
}

void oms::SingleImageChain::setScalarRemapperEnableFlag(bool flag)
{
	if(theScalarRemapper) theScalarRemapper->setEnableFlag(flag);
}

void oms::SingleImageChain::setSharpenEnableFlag(bool flag)
{
   if(theImageSharpenFilter)
   {
      theImageSharpenFilter->setEnableFlag(flag);
   }
}

void oms::SingleImageChain::setResamplerType(const std::string& resamplerType)
{
	if(theRenderer) theRenderer->getResampler()->setFilterType(resamplerType);
}

void oms::SingleImageChain::setScalarRemapType(ossimScalarType scalarType)
{
   if(theScalarRemapper) 
   {
      theScalarRemapper->setEnableFlag(true);
      theScalarRemapper->setOutputScalarType(scalarType);
   }
}

void oms::SingleImageChain::setPropertyGivenClassName(const std::string& className,
                                                      const std::string& propertyName,
                                                      const std::string& propertyValue)
{
   ossimConnectableObject* obj = theImageChain->findFirstObjectOfType(className);
   if(obj)
   {
      ((ossimPropertyInterface*)obj)->setProperty(propertyName, propertyValue);
   }
}

ossimConnectableObject* oms::SingleImageChain::getConnectableObject(const std::string& className)
{
   return  theImageChain->findFirstObjectOfType(className);
}

void oms::SingleImageChain::setToSingleBand(ossim_int32 band)
{
   std::vector<ossim_uint32> bandList;
   bandList.push_back(band);
   setBandSelection(bandList);
}

void oms::SingleImageChain::setToThreeBands()
{
   if(!theImageChain.valid()) return;
   ossim_uint32 bands = 0;
   if(theImageChain->getInput())
   {
      ossimImageSource* is = dynamic_cast<ossimImageSource*>(theImageChain->getInput());
      bands = is->getNumberOfOutputBands();
   }
   std::vector<ossim_uint32> bandList;
   if(bands >=3)
   {
      bandList.push_back(0);
      bandList.push_back(1);
      bandList.push_back(2);
      setBandSelection(bandList);
   }
   else if((bands < 3)&&(bands>0))
   {
      bandList.push_back(0);
      bandList.push_back(0);
      bandList.push_back(0);
      setBandSelection(bandList);
   }
}

void oms::SingleImageChain::setToThreeBandsReverse()
{
   if(!theImageChain.valid()) return;
   ossim_uint32 bands = theImageChain->getNumberOfInputBands();
   std::vector<ossim_uint32> bandList;
   if(bands >3)
   {
      bandList.push_back(2);
      bandList.push_back(1);
      bandList.push_back(0);
      setBandSelection(bandList);
   }
   else if((bands < 3)&&(bands>0))
   {
      bandList.push_back(0);
      bandList.push_back(0);
      bandList.push_back(0);
      setBandSelection(bandList);
   }
}

void oms::SingleImageChain::setupSurfaceNormalCalculations()
{
   ossimRefPtr<ossimConnectableObject> surfaceNormals = theImageChain->findFirstObjectOfType("ossimImageToPlaneNormalFilter");
   if(!surfaceNormals)
   {
      surfaceNormals = ossimImageSourceFactoryRegistry::instance()->createImageSource(ossimString("ossimImageToPlaneNormalFilter"));
      if(surfaceNormals.valid())
      {
         if(!theImageChain->insertLeft(surfaceNormals.get(),
                                        theCache))
         {
            surfaceNormals = 0;
         }
         else
         {
            theImageChain->initialize();
         }
      }
   }
}

void oms::SingleImageChain::setBandSelection(const int* bandList, int bandListSize, bool zeroBased)
{
	setBandSelection(std::vector<ossim_uint32>(bandList, bandList+bandListSize));
}

void oms::SingleImageChain::setBandSelection(const std::vector<ossim_uint32>& bandList, bool zeroBased)
{
   std::vector<ossim_uint32> bandZeroBased(bandList);
   if(!zeroBased)
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < bandZeroBased.size(); ++idx)
      {
         if(bandZeroBased[idx] > 0)
         {
            --bandZeroBased[idx];
         }
      }
   }
   theBandSelector->setEnableFlag(true);
   theBandSelector->setOutputBandList(bandZeroBased);
   theHistogramRemapper->initialize();
}

void oms::SingleImageChain::setBandSelection(const std::vector<ossimString>& bandList, bool zeroBased)
{
   std::vector<ossim_uint32> bands(bandList.size());
   ossim_uint32 idx = 0;
   for(idx = 0; idx < bands.size(); ++idx)
   {
      bands[idx] = bandList[idx].toUInt32();
   }
   setBandSelection(bands, zeroBased);
}

void oms::SingleImageChain::setHistogramFile(const std::string& file)
{
   if(theHistogramRemapper)
   {
      theHistogramRemapper->setEnableFlag(true);
      theHistogramRemapper->openHistogram(ossimFilename(file));
   }
}

void oms::SingleImageChain::setHistogram(ossimMultiResLevelHistogram* histogram)
{
   if(theHistogramRemapper)
   {
      theHistogramRemapper->setEnableFlag(true);
      theHistogramRemapper->setHistogram(histogram);
   }
}

void oms::SingleImageChain::setHistogramStretchMode(const std::string& mode)
{
   if(theHistogramRemapper)
   {
      theHistogramRemapper->setEnableFlag(true);
      theHistogramRemapper->setStretchModeAsString(mode);
   }
}

void oms::SingleImageChain::setHistogramFileAndMode(const std::string& file, const std::string& mode)
{
   if(theHistogramRemapper&&ossimFilename(file).exists())
   {
      setHistogramFile(file);
      setHistogramStretchMode(mode);
      ossimRefreshEvent evt(theHistogramRemapper);
      theHistogramRemapper->propagateEventToOutputs(evt);
   }
}
void oms::SingleImageChain::setHistogramAndMode(ossimMultiResLevelHistogram* histogram, const std::string& mode)
{
   if(theHistogramRemapper)
   {
      setHistogram(histogram);
      setHistogramStretchMode(mode);
      ossimRefreshEvent evt(theHistogramRemapper);
      theHistogramRemapper->propagateEventToOutputs(evt);
   }
}

void oms::SingleImageChain::setHistogramFileToDefaultAndMode(const std::string& mode)
{
   if(theBandSelector)
   {
      ossimImageHandler* handler = (ossimImageHandler*)(theBandSelector->findInputObjectOfType("ossimImageHandler"));
      if(handler)
      {
         ossimFilename histoFile = handler->createDefaultHistogramFilename();
         setHistogramFileAndMode(histoFile, mode);
      }
   }
}

void oms::SingleImageChain::setViewCut(const ossimGpt* pointList, int pointListSize)
{
   setViewCut(std::vector<ossimGpt> (pointList, pointList + pointListSize));
}

void oms::SingleImageChain::setViewCut(const std::vector<ossimGpt>& pointList)
{
   theViewImageCutter->setEnableFlag(false);
   theViewCutter->setEnableFlag(true);
   theViewCutter->setPolygon(pointList);
   ossimPropertyEvent evt(theViewCutter);
   theViewCutter->propagateEventToOutputs(evt);
}

void oms::SingleImageChain::setViewCut(const ossimGrect& grect)
{
   std::vector<ossimGpt> pointList(4);
   pointList[0] = grect.ul();
   pointList[1] = grect.ur();
   pointList[2] = grect.lr();
   pointList[3] = grect.ll();
   setViewCut(pointList);
};

void oms::SingleImageChain::setViewCut(const ossimDrect& rect, bool imageSpaceFlag)
{
   if(!imageSpaceFlag)
   {
      
      const ossimProjection* proj = getViewProjection();
      
      if(proj)
      {
         std::vector<ossimGpt> pointList(4);
         proj->lineSampleToWorld(rect.ul(), pointList[0]);
         proj->lineSampleToWorld(rect.ur(), pointList[1]);
         proj->lineSampleToWorld(rect.lr(), pointList[2]);
         proj->lineSampleToWorld(rect.ll(), pointList[3]);
         setViewCut(pointList);
      }
   }
   else
   {
      std::vector<ossimDpt> pointList(4);
      pointList[0] = rect.ul();
      pointList[1] = rect.ur();
      pointList[2] = rect.lr();
      pointList[3] = rect.ll();
      theViewCutter->setEnableFlag(false);
      theViewImageCutter->setEnableFlag(true);
      theViewImageCutter->setPolygon(pointList);
      theViewImageCutter->initialize();
      ossimPropertyEvent evt(theViewImageCutter);
      theViewImageCutter->propagateEventToOutputs(evt);
   }
}

void oms::SingleImageChain::setViewCut(const ossimIrect& rect, bool imageSpaceFlag)
{
   setViewCut(ossimDrect(rect), imageSpaceFlag);
}

void oms::SingleImageChain::setImageCut(const ossimDpt* pointList, int pointListSize)
{
   setImageCut(std::vector<ossimDpt>(pointList, pointList + pointListSize));
}

void oms::SingleImageChain::setImageCut(const std::vector<ossimDpt>& pointList)
{
   theImageCutter->setEnableFlag(true);
   theImageCutter->setPolygon(pointList);
   theImageCutter->initialize();
   ossimPropertyEvent evt(theImageCutter);
   theImageCutter->propagateEventToOutputs(evt);
}

void oms::SingleImageChain::setImageCut(const ossimIrect& rect)
{
   std::vector<ossimDpt> pointList(4);
   pointList[0] = rect.ul();
   pointList[1] = rect.ur();
   pointList[2] = rect.lr();
   pointList[3] = rect.ll();
   
   setImageCut(pointList);
}

void oms::SingleImageChain::setImageCut(const ossimDrect& rect)
{
   std::vector<ossimDpt> pointList(4);
   pointList[0] = rect.ul();
   pointList[1] = rect.ur();
   pointList[2] = rect.lr();
   pointList[3] = rect.ll();
   
   setImageCut(pointList);
}

void oms::SingleImageChain::setSharpen(ossim_uint32 kernelWidth,
                                       ossim_float64 sigmaParameter)
{
   theImageSharpenFilter->setWidthAndSigma(kernelWidth, sigmaParameter);
   theImageSharpenFilter->setEnableFlag(true);
   ossimPropertyEvent evt(theImageCutter);
   theImageSharpenFilter->propagateEventToOutputs(evt);
}

const ossimProjection* oms::SingleImageChain::getViewProjection()
{
   return oms::Util::findProjectionConst(theViewCutter);
}

ossimImageChain* oms::SingleImageChain::getImageChain()
{
   return theImageChain.get();
}

ossimImageSource* oms::SingleImageChain::getImageRenderer()
{
   return theRenderer;
}

ossimImageSource* oms::SingleImageChain::getBandSelector()
{
   return theBandSelector;
}

ossimImageSource* oms::SingleImageChain::getImageCutter()
{   
   return theImageCutter;
}

ossimImageSource* oms::SingleImageChain::getViewCutter()
{
   return theViewCutter;
}

ossimImageSource* oms::SingleImageChain::getScalarRemapper()
{
   return theScalarRemapper;
}

bool oms::SingleImageChain::getInputGroundCenter(ossimGpt& result)const
{
   bool resultFlag = false;
   
   if(theImageGeometry.valid()&&theImageGeometry->getProjection())
   {
      ossimDrect inputRect;
      if(getInputBoundingRect(inputRect))
      {
         resultFlag = true;
         ossimDpt imageCenterPoint = inputRect.midPoint();
         theImageGeometry->localToWorld(imageCenterPoint, result);
      }
   }
   
   return resultFlag;
}

bool oms::SingleImageChain::getInputCornerGroundPoints(ossimGpt& ul,
                                                      ossimGpt& ur,
                                                      ossimGpt& lr,
                                                      ossimGpt& ll)const
{
   bool resultFlag = false;
   
   if(theImageGeometry.valid()&&theImageGeometry->getProjection())
   {
      ossimDrect inputRect;
      if(getInputBoundingRect(inputRect))
      {
         resultFlag = true;
         theImageGeometry->localToWorld(inputRect.ul(), ul);
         theImageGeometry->localToWorld(inputRect.ur(), ur);
         theImageGeometry->localToWorld(inputRect.lr(), lr);
         theImageGeometry->localToWorld(inputRect.ll(), ll);
      }
   }
   
   return resultFlag;
}

bool oms::SingleImageChain::getInputBoundingRect(ossimDrect& result)const
{
   bool resultFlag = false;
   const ossimImageSource* inputSource = dynamic_cast<const ossimImageSource*>(theImageChain->getInput(0));
   if(inputSource)
   {
      result = inputSource->getBoundingRect();
      resultFlag = true;
   }
   
   return resultFlag;
}

bool oms::SingleImageChain::getInputBoundingRect(ossimIrect& result)const
{
   bool resultFlag = false;
   const ossimImageSource* inputSource = dynamic_cast<const ossimImageSource*>(theImageChain->getInput(0));
   if(inputSource)
   {
      result = inputSource->getBoundingRect();
      resultFlag = true;
   }
   
   return resultFlag;
}

void oms::SingleImageChain::setImageViewTransform(ossimRefPtr<ossimImageViewTransform> imageViewTransform)
{
   theViewCutter->setEnableFlag(false);
   theViewImageCutter->setEnableFlag(false);
   theRenderer->setImageViewTransform(imageViewTransform.get());
   theRenderer->setEnableFlag(true);
   ossimRefreshEvent evt(theRenderer);
   theRenderer->propagateEventToOutputs(evt);
}

void oms::SingleImageChain::saveState(ossimKeywordlist& kwl,
                                      const ossimString& prefix)
{
   if(theImageChain.valid())
   {
      theImageChain->saveState(kwl, prefix.c_str());
   }
}
