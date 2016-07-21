/******************************
 *
 * License: LGPL
 * Author:  Garrett Potts
 *
 *****************************/
#include <oms/WmsMap.h>
#include <oms/SingleImageChain.h>
#include <oms/Util.h>
#include <ossim/base/ossimException.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
#include <ossim/imaging/ossimImageMosaic.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimRLevelFilter.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimImageHistogramSource.h>
#include <ossim/projection/ossimImageViewAffineTransform.h>

class oms::WmsMap::PrivateData
{
public:
   friend class oms::WmsMap;
   PrivateData()
   :cacheFlag(true)
   {
      mosaic = new ossimImageMosaic();
      m_boundingGround.makeNan();
      m_rotation = 0.0;
      eightBitThreeBandChain = new SingleImageChain();
      // make this an image space ony chain and scale to 8 bit 3 band output
      eightBitThreeBandChain->setImageRendererEnableFlag( false );
      eightBitThreeBandChain->getImageChain()->connectMyInputTo( 0, mosaic.get() );
      eightBitThreeBandChain->setToThreeBands();
      eightBitThreeBandChain->setScalarRemapType( OSSIM_UINT8 );
      blankSource = new ossimMemoryImageSource();
      cacheSource = new ossimMemoryImageSource();
      blankSource->setImage( OSSIM_UINT8, 3, 256, 256 );
      wmsView = new WmsView();
   }
   virtual ~PrivateData()
   {
      // since we are managing everything we will need to manually disconnect everything
      // so we insure proper cleanup
      //
      if(mosaic.valid())
      {
         eightBitThreeBandChain->getImageChain()->disconnect();
         eightBitThreeBandChain = 0;
         mosaic->disconnect();
         mosaic = 0;
         blankSource->disconnect();
         cacheSource->disconnect();
         wmsView = 0;
         
         ossim_uint32 idx = 0;
         for(idx = 0; idx < inputHandlers.size();++idx)
         {
            inputHandlers[idx]->disconnect();
            inputHandlers[idx] = 0;
         }
         for(idx = 0; idx < inputChains.size();++idx)
         {
            inputChains[idx]->getImageChain()->disconnect();
            inputChains[idx] = 0;
         }
         blankSource = 0;
         cacheSource = 0;
      }
   }
   void setupViewportStretchIfNeeded()
   {
      eightBitThreeBandChain->setHistogramRemapperEnableFlag(false);
      if(!eightBitThreeBandChain.valid()||
         theViewportStretchType.empty()||
         (theViewportStretchType=="none")) 
      {
         return;
      }
      if(eightBitThreeBandChain->getScalarRemapper()&&wmsView.valid())
      {
         ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
         histoSource->connectMyInputTo(0, eightBitThreeBandChain->getScalarRemapper()->getInput());
         histoSource->enableSource();
         histoSource->setAreaOfInterest(wmsView->getViewImageRect());
         ossimRefPtr<ossimMultiResLevelHistogram> histo = histoSource->getHistogram();
         histoSource->disconnect();
         if(histo.valid())
         {
            eightBitThreeBandChain->setHistogramAndMode(histo.get(), theViewportStretchType);
         }
      }
   }
   ossimImageData* stretchInput(ossimImageData* data)
   {
      ossimImageData* result = data;
      if(theViewportStretchType.empty()||
         (theViewportStretchType=="none"))
      {
         return result;
      }
      if(data&&(data->getDataObjectStatus() != OSSIM_NULL))
      {
         ossimRefPtr<ossimMemoryImageSource> memSource = new ossimMemoryImageSource();
         memSource->setImage(data);
         ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
         histoSource->connectMyInputTo(0, memSource.get());
         histoSource->enableSource();
         ossimRefPtr<ossimMultiResLevelHistogram> histo = histoSource->getHistogram();
         if(histo.valid())
         {
            ossimRefPtr<ossimHistogramRemapper> histoRemapper = new ossimHistogramRemapper();
            histoRemapper->connectMyInputTo(0, memSource.get());
            histoRemapper->setStretchModeAsString(theViewportStretchType);
            histoRemapper->setHistogram(histo.get());

            try
            {
               ossimRefPtr<ossimImageData> output =
                  histoRemapper->getTile(histoRemapper->getBoundingRect());
               result = (ossimImageData*)(output->dup());
            }
            catch( const ossimException& e )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "oms::WmsMap::PrivateData::stretchInput caught exception: "
                  << e.what() << std::endl;
            }
            catch( ... )
            {
               ossimNotify(ossimNotifyLevel_WARN)
                  << "oms::WmsMap::PrivateData::stretchInput caught unknown exception: "
                  << std::endl;
            }
         }
      }
      
      return result;
   }
   void insertAffineTransform(ossimImageGeometry* geom)
   {
      if(!ossim::almostEqual(m_rotation, 0.0)&&eightBitThreeBandChain.valid()&&mosaic.valid()&&geom)
      {
         ossimGpt midGpt = m_boundingGround.midPoint();
         ossimDpt midPt;
         geom->worldToLocal(midGpt, midPt);
            
         ossimRefPtr<ossimImageViewAffineTransform> transform = new ossimImageViewAffineTransform(
            -m_rotation,
            1.0,1.0,  // image scale x and y
            1.0,1.0,  //scale x and y
            0.0, 0.0, // translate x,y
            midPt.x, midPt.y); // pivot point
         geom->setTransform(transform.get());
      }
   }
   
   double rotation()const
   {
      return m_rotation;
   }
   
   void combineBoundingGround(ossimRefPtr<ossimImageHandler> handler)
   {
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      ossimIrect bounds = handler->getBoundingRect();
      std::vector<ossimGpt> points(4);
      geom->localToWorld(bounds.ul(), points[0]);
      geom->localToWorld(bounds.ur(), points[1]);
      geom->localToWorld(bounds.lr(), points[2]);
      geom->localToWorld(bounds.ll(), points[3]);
      ossimGrect grect(points);
      if(m_boundingGround.isNan())
      {
         m_boundingGround = grect;
      }
      else 
      {
         m_boundingGround.combine(grect);
      }
   }
   
protected:
   std::vector<ossimRefPtr<ossimImageHandler> > inputHandlers;
   std::vector<ossimRefPtr<SingleImageChain> >  inputChains;
   ossimRefPtr<SingleImageChain>                eightBitThreeBandChain;
   ossimRefPtr<ossimImageSource>                mosaic;
   ossimRefPtr<ossimMemoryImageSource>          blankSource;
   ossimRefPtr<ossimMemoryImageSource>          cacheSource;
   
   ossim_float64                                m_rotation;
   ossimGrect                                   m_boundingGround;
   // set this to true if you want any call to getMAp to go ahead and
   // retrieve the image and return it as a cached memory object
   //
   bool cacheFlag;
   
   ossimString theViewportStretchType;
private:
   ossimRefPtr<oms::WmsView> wmsView;
};


oms::WmsMap::WmsMap()
:thePrivateData(new PrivateData())
{
}

oms::WmsMap::~WmsMap()
{
   if(thePrivateData)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
}

void oms::WmsMap::cleanUp()
{
   if(thePrivateData)
   {
      delete thePrivateData;
      thePrivateData = 0;
   }
}

bool oms::WmsMap::addFile(const std::string& filename,
                          int entryIdx,
                          ossimImageGeometry* geomOverride)
//                          const std::string& histogramFile,
//                          const std::string& stretchType)
{
	ossimRefPtr<ossimImageGeometry> retainGeom = geomOverride;
	ossimRefPtr<ossimImageHandler> input = ossimImageHandlerRegistry::instance()->open( ossimFilename(filename));
	bool result = false;
	if ( input.valid() )
	{
		if ( input->setCurrentEntry( (ossim_uint32)entryIdx ) )
		{
         if(geomOverride)
         {
            ossimKeywordlist kwl;
            geomOverride->saveState(kwl);
            input->setImageGeometry(geomOverride);
         }
         thePrivateData->combineBoundingGround(input.get());
			ossimRefPtr<SingleImageChain> reprojectionChain = new SingleImageChain();
			reprojectionChain->setResamplerType( "bilinear" );
			// connect the chain to the source
			ossimImageChain* chain1 = reprojectionChain->getImageChain();
			
			chain1->connectMyInputTo( input.get() );
         reprojectionChain->setCacheEnableFlag(true);
			// add to the mosaic
			thePrivateData->mosaic->connectMyInputTo( chain1 );

#if 0
			if((stretchType != "none")&&(!stretchType.empty()))
			{
				if(histogramFile.empty())
				{
					reprojectionChain->setHistogramFileToDefaultAndMode(stretchType);
				}
				else
				{
					reprojectionChain->setHistogramFileAndMode(histogramFile, stretchType);
				}
			}
#endif
			// retain the pointers
			thePrivateData->inputHandlers.push_back( input );
			thePrivateData->inputChains.push_back( reprojectionChain );
			
			thePrivateData->eightBitThreeBandChain->getImageChain()->connectMyInputTo( 0, thePrivateData->mosaic.get() );			
			thePrivateData->eightBitThreeBandChain->setToThreeBands();
			thePrivateData->eightBitThreeBandChain->setScalarRemapType( OSSIM_UINT8 );
			thePrivateData->eightBitThreeBandChain->getImageChain()->initialize();
			
			result = true;
		}
	}
	
	return result;
}

void oms::WmsMap::setChainParameters(const ossimKeywordlist& value, int idx)
{
   if(!thePrivateData)
   {
      return;
   }
   ossimString viewable_bands = value.find("viewable_bands");
   ossimString sharpen_width = value.find("sharpen_width");
   ossimString bands = value.find("bands");
   ossimString sharpen_sigma = value.find("sharpen_sigma");
   ossimString stretch_mode = value.find("stretch_mode");
   ossimString stretch_region = value.find("stretch_region");
   ossimString null_flip = value.find("null_flip");
   ossimString rotateString = value.find("rotate");
   
   if(!rotateString.empty())
   {
      thePrivateData->m_rotation = rotateString.toDouble();
   }
   ossim_uint32 chainIdx = 0;
   bool sharpenFlag = (!sharpen_width.empty()&&
                       !sharpen_sigma.empty());
   bool globalStretch = stretch_region == "global";
   bool viewportStretch = stretch_region == "viewport";
   bool validStretch  = (!stretch_mode.empty() && 
                         (stretch_mode != "none"));
   bool nullFlipEnabled = (!null_flip.empty()&&
                           (null_flip == "min"));
   bands = bands.trim();
   std::vector<ossimString> bandSplit;
   std::vector<ossim_uint32> bandList;
   bands.split(bandSplit, ",");
   bandList.resize(bandSplit.size());
   ossim_uint32 bandIdx = 0;
   for(;bandIdx < bandList.size();++bandIdx)
   {
      bandList[bandIdx] = bandSplit[bandIdx].toUInt32();
#if 0 // this is to make zero based, for now we will take it out and expect things 0 based
      if(bandList[bandIdx] > 0)
      {
         --bandList[bandIdx];
      }
#endif
   }
   ossim_uint32 inputs = thePrivateData->inputChains.size();
   if(idx < 0)
   {
      for(chainIdx = 0; chainIdx < inputs; ++chainIdx)
      {
         std::vector<ossim_uint32> bandListForChain(bandList);
         ossim_uint32 numberOfBands = thePrivateData->inputChains[chainIdx]->getImageChain()->getNumberOfInputBands();
         
         for(bandIdx = 0;bandIdx < bandListForChain.size();++bandIdx)
         {
            if(bandListForChain[bandIdx] >= numberOfBands)
            {
               bandListForChain[bandIdx] = numberOfBands-1;
            }
         }
         if(!bands.empty())
         {
            thePrivateData->inputChains[chainIdx]->setBandSelection(bandListForChain);
         }
         if(globalStretch)
         {
            if(validStretch)
            {
               thePrivateData->inputChains[chainIdx]->setHistogramFileToDefaultAndMode(stretch_mode);
            }
            else
            {
               thePrivateData->inputChains[chainIdx]->setHistogramRemapperEnableFlag(false);
            }
            
            thePrivateData->inputChains[chainIdx]->getImageChain()->initialize();
         }
         thePrivateData->inputChains[chainIdx]->setNullPixelFlipEnableFlag(nullFlipEnabled);
      }
   }
   else if(idx < thePrivateData->inputChains.size())
   {
      std::vector<ossim_uint32> bandListForChain(bandList);
      ossim_uint32 numberOfBands = thePrivateData->inputChains[idx]->getImageChain()->getNumberOfInputBands();
      for(bandIdx = 0;bandIdx < bandListForChain.size();++bandIdx)
      {
         if(bandListForChain[bandIdx] >= numberOfBands)
         {
            bandListForChain[bandIdx] = numberOfBands-1;
         }
      }
      if(bandSplit.size()&&!bands.empty())
      {
         thePrivateData->inputChains[idx]->setBandSelection(bandListForChain);
      }
      if(globalStretch)
      {
         if(validStretch)
         {
            thePrivateData->inputChains[idx]->setHistogramFileToDefaultAndMode(stretch_mode);
         }
         else
         {
            thePrivateData->inputChains[idx]->setHistogramRemapperEnableFlag(false);
         }
         thePrivateData->inputChains[idx]->getImageChain()->initialize();
      }
      thePrivateData->inputChains[idx]->setNullPixelFlipEnableFlag(nullFlipEnabled);
      
   }

   if(viewportStretch)
   {
      thePrivateData->theViewportStretchType = stretch_mode;
   }
   else
   {
      thePrivateData->theViewportStretchType = "";
   }
   if(inputs==1)
   {
      if(sharpenFlag)
      {
         thePrivateData->inputChains[0]->setSharpen(sharpen_width.toUInt32(), sharpen_sigma.toDouble());
      }
      else
      {
         thePrivateData->inputChains[0]->setSharpenEnableFlag(false);
      }
      
      thePrivateData->inputChains[0]->getImageChain()->initialize();
   }
   else
   {
      if(thePrivateData->eightBitThreeBandChain.valid())
      {
         if(sharpenFlag)
         {
            thePrivateData->eightBitThreeBandChain->setSharpen(sharpen_width.toUInt32(), sharpen_sigma.toDouble());
         }
         else
         {
            thePrivateData->eightBitThreeBandChain->setSharpenEnableFlag(false);
         }
      }
   }

   if(thePrivateData->mosaic.valid())
   {
      thePrivateData->eightBitThreeBandChain->getImageChain()->connectMyInputTo(thePrivateData->mosaic.get());
      thePrivateData->mosaic->initialize();
   }
   ossim_uint32 viewableBands = 3;
   if(!viewable_bands.empty())
   {
      viewableBands = viewable_bands.toUInt32();
   }
   if(viewableBands == 3)
   {
      
      thePrivateData->eightBitThreeBandChain->setToThreeBands();
   }
   else 
   {
      thePrivateData->eightBitThreeBandChain->setToSingleBand();
   }

   thePrivateData->eightBitThreeBandChain->setScalarRemapType( OSSIM_UINT8 );
   thePrivateData->eightBitThreeBandChain->getImageChain()->initialize();
   
}

void oms::WmsMap::setChainParametersPerImage(const std::vector<ossimKeywordlist>& value)
{
   if(!thePrivateData)
   {
      return;
   }
 
   ossim_uint32 idx = 0;
   for(idx = 0; idx < value.size();++idx)
   {
      setChainParameters(value[idx], (int)idx);
   }
}

/*
bool oms::WmsMap::createMap(const std::string& csvFilenames,
                            const std::string& csvEntries,
                            const std::string& csvHistogramFile,
                            const std::string& csvStretchType,
                            const std::string& srsCode,
                            const std::string& viewportStretchType,
                            double minX, double minY,
                            double maxX, double maxY,
                            int width,
                            int height,
                            ossim_int8* buffer )
{
	bool result = false;
	std::vector<ossimString> files;
	std::vector<ossimString> entries;
	std::vector<ossimString> histograms;
	std::vector<ossimString> stretchType;
	
#if 0	
	std::cout << "FILES = " << csvFilenames << std::endl;
	std::cout << "ENTRIES = " << csvEntries << std::endl;
	std::cout << "HISTS = " << csvHistogramFile << std::endl;
	std::cout << "STRETCH = " << csvStretchType << std::endl;
#endif	
	
	ossimString(csvFilenames).split(files, ",");
	ossimString(csvEntries).split(entries, ",");
	ossimString(csvHistogramFile).split(histograms, ",");
	ossimString(csvStretchType).split(stretchType, ",");
	ossim_uint32 size = files.size();
	if((size == entries.size())&&
	   (size == histograms.size())&&
	   (size == stretchType.size()))
	{
		ossimRefPtr<WmsMap> map = new WmsMap();
//      map->setViewportStretchType(viewportStretchType);
		ossim_uint32 idx = 0;
		result = true;
		for(idx = 0; idx < size; ++idx)
		{
			if(!map->addFile(files[idx].trim(),
                          entries[idx].trim().toInt32()))
//                          histograms[idx].trim(),
//                          stretchType[idx].trim()))
			{
				result = false;
			}
			map->getMap(srsCode, 
                     minX, minY, maxX, maxY, 
                     width, 
                     height, buffer);
		}
      map = 0;
	}
	
	return result;
}
*/

void oms::WmsMap::getMap(const std::string& srsCode,
                          double minX, double minY,
                          double maxX, double maxY,
                          int width,
                          int height,
                          ossim_int8* buffer)
{
   bool blankOutFlag = false;
   if( setupView(srsCode, minX, minY, maxX, maxY, width, height) )
   {
      thePrivateData->setupViewportStretchIfNeeded();
      // now cut the rectangle and put in the buffer
      ossimIrect rect = thePrivateData->eightBitThreeBandChain->getImageChain()->getBoundingRect();
      ossimDpt midPoint = rect.midPoint();
      ossimIpt origin = ossimDpt(midPoint.x - width/2.0, midPoint.y - height/2.0);
      ossimIrect rectCut = ossimDrect(0.0,
                                      0.0,
                                      (width-1),
                                      (height-1));

      try
      {
         ossimRefPtr<ossimImageData> data =
            thePrivateData->eightBitThreeBandChain->getImageChain()->getTile(rectCut);
         if(data.valid())
         {
            ossimDataObjectStatus status = data->getDataObjectStatus();
            if(status != OSSIM_NULL)
            {
               data->unloadTile(buffer, rectCut, OSSIM_BIP);
            }
            else
            {
               blankOutFlag = true;
            }
         }
         else
         {
            blankOutFlag = true;
         }
      }
      catch( const ossimException& e )
      {
         blankOutFlag = true;
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::stretchInput caught exception: "
            << e.what() << std::endl;
      }
      catch( ... )
      {
         blankOutFlag = true;
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::stretchInput caught unknown exception: "
            << std::endl;
      }
   }
   else
   {
      blankOutFlag = true;
   }
   
   if(blankOutFlag)
   {
      ossimRefPtr<ossimMemoryImageSource> blankSource = new ossimMemoryImageSource();
      blankSource->setImage( OSSIM_UINT8, thePrivateData->eightBitThreeBandChain->getImageChain()->getNumberOfOutputBands(), width, height );
      ossimRefPtr<ossimImageData> data = new ossimImageData(0, OSSIM_UINT8, thePrivateData->eightBitThreeBandChain->getImageChain()->getNumberOfOutputBands(), width, height);
      data->initialize();
      data->unloadTile(buffer, data->getImageRectangle(), OSSIM_BIP);
   }
}


void oms::WmsMap::getUnprojectedMap(
                                    const std::string& filename,
                                    int entryIdx,
                                    double scale,
                                    double startSample, double endSample,
                                    double startLine, double endLine,
                                    ossim_int8* buffer,
                                    const ossimKeywordlist& options)
{
   ossimString viewable_bands = options.find("viewable_bands");
   ossimString sharpen_width = options.find("sharpen_width");
   ossimString bands = options.find("bands");
   ossimString sharpen_sigma = options.find("sharpen_sigma");
   ossimString stretch_mode = options.find("stretch_mode");
   ossimString stretch_region = options.find("stretch_mode_region");
   ossimString null_flip = options.find("null_flip");
   ossimString histogramFile = options.find("histogram_file");
   ossimString rotateString = options.find("rotate");
   double rotate = 0.0;
   if(!rotateString.empty())
   {
      rotate = rotateString.toDouble();
   }
   bool blankOut = false;
   ossimRefPtr<SingleImageChain>                imageSpaceChain;
   ossimRefPtr<SingleImageChain>                viewableBandChain;
   ossimRefPtr<ossimMemoryImageSource>          blankSource;
   ossimRefPtr<ossimImageHandler> input = ossimImageHandlerRegistry::instance()->open( ossimFilename(filename));
   ossimImageSource* inputSrc = input.get();
   ossimIrect areaOfInterest = ossimIrect(startSample, startLine, endSample, endLine);
   bool viewportStretchFlag = ((!stretch_region.empty())&&
                               (stretch_region=="viewport"));
   bool sharpenFlag = (!sharpen_width.empty()&&
                       !sharpen_sigma.empty());
   ossim_uint32 viewableBands = viewable_bands.empty()?3:viewable_bands.toUInt32();
   if(inputSrc)
   {
      bands = bands.trim();
      std::vector<ossimString> bandSplit;
      std::vector<ossim_uint32> bandList;
      bands.split(bandSplit, ",");
      bandList.resize(bandSplit.size());
      ossim_uint32 bandIdx = 0;
      for(;bandIdx < bandList.size();++bandIdx)
      {
         bandList[bandIdx] = bandSplit[bandIdx].toUInt32();
      }
      ossim_uint32 numberOfBands = inputSrc->getNumberOfOutputBands();
      
      for(bandIdx = 0;bandIdx < bandList.size();++bandIdx)
      {
         if(bandList[bandIdx] >= numberOfBands)
         {
            bandList[bandIdx] = numberOfBands-1;
         }
      }
      input->setCurrentEntry(entryIdx);
		
      imageSpaceChain = new SingleImageChain();
      viewableBandChain = new SingleImageChain();
      ossimImageChain* imageChain = imageSpaceChain->getImageChain();
      imageSpaceChain->disableAll();
      viewableBandChain->disableAll();
      if(!bandList.empty())
      {
         imageSpaceChain->setBandSelection(bandList);
      }
      ossimIrect bounds = input->getBoundingRect();
      ossimDpt boundsCenter = bounds.midPoint();
      imageSpaceChain->setImageViewTransform(new ossimImageViewAffineTransform(
                                                -rotate,
                                                1.0, 1.0,     // image scale x and y
                                                scale, scale, // scale x and y
                                                0.0,0.0, boundsCenter.x, boundsCenter.y));
      imageSpaceChain->setResamplerType( "bilinear" );
      imageChain->connectMyInputTo(0, inputSrc);
      viewableBandChain->getImageChain()->connectMyInputTo(0, imageSpaceChain->getImageChain());
      ossimIrect areaOfInterest(ossimIpt(startSample, startLine) + bounds.ul(), 
                                ossimIpt(endSample, endLine)     + bounds.ul());
      // make this an image space only chain and scale to 8 bit 3 band output
      viewableBandChain->setScalarRemapType( OSSIM_UINT8 );
      if(viewableBands == 3)
      {
         viewableBandChain->setToThreeBands();
      }
      else 
      {
         viewableBandChain->setToSingleBand();
      }
      imageSpaceChain->setViewCut(areaOfInterest, true); // we will say it's in pixel space
      ossimFilename histoFile;
		
      if(sharpenFlag)
      {
         imageSpaceChain->setSharpen(sharpen_width.toUInt32(), sharpen_sigma.toDouble());
      }
      if(!viewportStretchFlag)
      {
         if((histogramFile != "none")&&(!histogramFile.empty()))
         {
            histoFile = ossimFilename(histogramFile);
         }
         else
         {
            histoFile = input->createDefaultHistogramFilename();
         }
      }
		
      if(!viewportStretchFlag&&(!stretch_mode.empty())&&(stretch_mode!="none")&&histoFile.exists())
      {
         imageSpaceChain->setHistogramFileAndMode(histoFile, stretch_mode);
      }
      else if(viewportStretchFlag&&(stretch_mode!="none")&&(!stretch_mode.empty()))
      {
         ossimRefPtr<ossimImageHistogramSource> histoSource = new ossimImageHistogramSource;
         histoSource->connectMyInputTo(0, imageSpaceChain->getScalarRemapper()->getInput());
         histoSource->enableSource();
         ossimRefPtr<ossimMultiResLevelHistogram> histo = histoSource->getHistogram();
         histoSource->disconnect();
         if(histo.valid())
         {
            imageSpaceChain->setHistogramAndMode(histo.get(), stretch_mode);
         }
      }

      // now cut the rectangle and put in the buffer
      try
      {
         ossimRefPtr<ossimImageData> data =
            viewableBandChain->getImageChain()->getTile(areaOfInterest);
         if(data.valid())
         {
            ossimDataObjectStatus status = data->getDataObjectStatus();
            if(status != OSSIM_NULL)
            {
               data->unloadTile(buffer, areaOfInterest, OSSIM_BIP);
            }
            else
            {
               blankOut = true;
            }
         }
         else
         {
            blankOut = true;
         }
         imageSpaceChain->getImageChain()->disconnect();
         viewableBandChain->getImageChain()->disconnect();
         imageChain->disconnect();
         input->close();
         input = 0;
         //      renderer = 0;
         //		rLevelFilter = 0;
         viewableBandChain = 0;
         imageSpaceChain = 0;
      }
      catch( const ossimException& e )
      {
         blankOut = true;
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::getUnprojectedMap caught exception: "
            << e.what() << std::endl;
      }
      catch( ... )
      {
         blankOut = true;
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::getUnprojectedMap caught unknown exception: "
            << std::endl;
      }
   }
   else
   {
      blankOut = true;
   }
   
   if(blankOut)
   {
//		blankSource = new ossimMemoryImageSource();
//		blankSource->setImage( OSSIM_UINT8, viewableBands, areaOfInterest.width(), areaOfInterest.height() );
      try
      {
         ossimRefPtr<ossimImageData> data =
            new ossimImageData(0, OSSIM_UINT8, viewableBands, areaOfInterest.width(), areaOfInterest.height());
         
         data->initialize();
         data->unloadTile(buffer, data->getImageRectangle(), OSSIM_BIP);
      }
      catch( const ossimException& e )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::getUnprojectedMap caught exception: "
            << e.what() << std::endl;
      }
      catch( ... )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::WmsMap::getUnprojectedMap caught unknown exception: "
            << std::endl;
      }
   }
	
}

bool oms::WmsMap::setupView(const std::string& srsCode,
                            double minX, double minY,
                            double maxX, double maxY,
                            int width, int height )
{
   if(!thePrivateData)
   {
      return false;
   }
   // first let's create a reprojection for the epsg code defined
   //
   if ( !thePrivateData->wmsView->setProjection( srsCode ) )
   {
      // technically we need to do an WMS XML exception or whatever is
      // defined by OGC WMS for an exception
      return false;
   }
   
   thePrivateData->wmsView->setViewDimensionsAndImageSize( minX, minY, maxX, maxY, width, height);
   
   if(!ossim::almostEqual(thePrivateData->rotation(), 0.0))
   {
      thePrivateData->insertAffineTransform(thePrivateData->wmsView->getImageGeometry().get());
   }
   Util::setAllViewGeometries( thePrivateData->mosaic.get(), thePrivateData->wmsView->getImageGeometry().get() );
   thePrivateData->eightBitThreeBandChain->setViewCut(thePrivateData->wmsView->getViewImageRect(), true);
   thePrivateData->eightBitThreeBandChain->getImageChain()->initialize();
   
   
//   if(!ossim::almostEqual(thePrivateData->rotation(), 0.0))
//   {
//      thePrivateData->insertAffineTransform(thePrivateData->wmsView->getImageGeometry().get());
//      Util::setAllViewGeometries( thePrivateData->mosaic.get(), thePrivateData->wmsView->getImageGeometry().get() );
//      thePrivateData->eightBitThreeBandChain->getImageChain()->initialize();
//   }
   return true;
}
