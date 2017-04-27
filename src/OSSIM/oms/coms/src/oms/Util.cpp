//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Utility class for oms library conversion to from ossim library.
//----------------------------------------------------------------------------
// $Id: Util.cpp 23424 2015-07-14 17:46:02Z dburken $

#include <oms/Util.h>
#include <oms/ImageOutputFormat.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimVisitor.h>
#include <ossim/base/ossim2dTo2dIdentityTransform.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageHandlerFactoryBase.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimScalarRemapper.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimRectangleCutFilter.h>
#include <ossim/projection/ossimImageViewAffineTransform.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimBilinearProjection.h>
#include <vector>


bool oms::Util::isAngularUnit(const ossimUnitType& unitType)
{
   bool result = false;
   switch(unitType)
   {
      case OSSIM_DEGREES:
      case OSSIM_MINUTES:
      case OSSIM_SECONDS:
      {
         result = true;
         break;
      }
      default:
      {
         break;
      }
   }

   return result;
}

void oms::Util::ossimToOms(const ossimKeywordlist& in,
                           std::map<std::string, std::string>& out)
{
   out.clear();

   ossimKeywordlist::KeywordMap::const_iterator i = in.getMap().begin();

   while(i != in.getMap().end())
   {
      std::string key  = i->first;
      std::string data = i->second;
      out.insert( make_pair(key, data) );
      ++i;
   }
}


void oms::Util::mimeToOssimWriter(const std::string& in, ossimString& out)
{
   ossimString os = in;
   os.downcase();
   os = os.substitute(ossimString("image/"), ossimString(""));
   out = "unknown";

   if (os.contains("jpeg"))
   {
      out = "jpeg";
   }
   else if(os.contains("tif"))
   {
      out = "tiff_tiled_band_separate";
   }
   else
   {
      std::vector<ossimString> outputType;
      ossimImageWriterFactoryRegistry::instance()->
         getImageTypeList(outputType);

      std::vector<ossimString>::const_iterator i = outputType.begin();
      while ( i != outputType.end() )
      {
         ossimString s = (*i);
         s.downcase();
         if (s.contains(os))
         {
            out = (*i);
            break;
         }
         ++i;
      }
   }
}

void oms::Util::getOutputFormats( std::vector<oms::ImageOutputFormat>& list)
{

   std::vector<ossimString> outputType;
   ossimImageWriterFactoryRegistry::instance()->
      getImageTypeList(outputType);

   std::vector<ossimString>::const_iterator i = outputType.begin();
   while ( i != outputType.end() )
   {
      ossimString os = (*i);
      os.downcase();

      oms::ImageOutputFormat iof;

      // Set the name to the write type from getImageTypeList.
      iof.setName((*i).c_str());

      // Take a stab at mine types.
      if (os.contains("jpeg"))
      {
         iof.setMimeType("image/jpeg");
      }
      else if (os.contains("png"))
      {
         iof.setMimeType("image/png");
      }
      else if (os.contains("tiff"))
      {
         iof.setMimeType("image/tiff");
      }
      else if (os.contains("bmp"))
      {
         iof.setMimeType("image/bmp");
      }
      else if (os.contains("gif"))
      {
         iof.setMimeType("image/gif");
      }

      // Take a stab at description.
      if ((os == "jpeg") || (os == "ossim_jpeg"))
      {
         iof.setDescription("ossim jpeg writer");
      }
      else if (os == "gdal_jpeg")
      {
         iof.setDescription("gdal jpeg writer");
      }
      else if (os == "ossim_png")
      {
         iof.setDescription("ossim png writer");
      }
      else if (os == "gdal_png")
      {
         iof.setDescription("gdal png writer");
      }
      else if (os == "tiff_strip")
      {
         iof.setDescription("ossim tiff writer strip contiguous");
      }
      else if (os == "tiff_strip_band_separate")
      {
         iof.setDescription("ossim tiff writer strip band separate");
      }
      else if (os == "tiff_tiled")
      {
         iof.setDescription("ossim tiff writer tiled contiguous");
      }
      else if (os == "tiff_tiled_band_separate")
      {
         iof.setDescription("ossim tiff writer tiled band separate");
      }
      else if (os == "gdal_gtiff")
      {
         iof.setDescription("gdal tiff writer");
      }
      else if (os == "gdal_gif")
      {
         iof.setDescription("gdal gif writer");
      }

      // Add it to the list.
      list.push_back(iof);

      // Go to next writer type in the list.
      ++i;

   } // End of "while ( i != outputType.end() )"
}

bool oms::Util::canOpen(const std::string& file)
{
   // Make ossim filename.
   ossimFilename f = file.c_str();
   if (f.empty())
   {
      return false; // Nothing in it.
   }

   ossimString ext = f.ext(); // Get the extension.
   if (ext.empty())
   {
      return false; // Nothing in it.
   }

   // Downcase it.
   ext.downcase();

   // Get a list of supported extensions.
   ossimImageHandlerFactoryBase::UniqueStringList list;
   ossimImageHandlerRegistry::instance()->getSupportedExtensions(list);

   std::vector<ossimString>::const_iterator i = list.getList().begin();

   while (i != list.getList().end())
   {
      if (ext == (*i))
      {
         return true;
      }
      ++i;
   }
   return false;
}

const ossimProjection* oms::Util::findProjectionConst(ossimConnectableObject* input)
{
   // Find all the views from input.
   ossimTypeNameVisitor visitor( ossimString("ossimViewInterface"),
                                 false, // firstofTypeFlag
                                 (ossimVisitor::VISIT_INPUTS|
                                  ossimVisitor::VISIT_CHILDREN) );
   input->accept( visitor );

   for( ossim_uint32 i = 0; i < visitor.getObjects().size(); ++i )
   {
      ossimViewInterface* viewInterface = visitor.getObjectAs<ossimViewInterface>( i );
      if (viewInterface)
      {
         const ossimProjection* proj =
            dynamic_cast<const ossimProjection*>(viewInterface->getView());
         if(proj)
         {
            return proj;
         }
         ossimImageGeometry* geom =
            dynamic_cast<ossimImageGeometry*> (viewInterface->getView());
         if(geom)
         {
            return geom->getProjection();
         } 
      }
   }
   
#if 0 /* Replaced deprecated code. (drb) */
   ossimConnectableObject::ConnectableObjectList viewList;

   if(PTR_CAST(ossimViewInterface, input))
   {
      viewList.push_back(input);
   }
   input->findAllInputsOfType(viewList,
                              "ossimViewInterface",
                              true,
                              true);

   ossim_uint32 idx = 0;
   for(idx = 0; idx < viewList.size();++idx)
   {
      ossimViewInterface* viewInterface = PTR_CAST(ossimViewInterface, viewList[idx].get());

      ossimProjection* proj = dynamic_cast<ossimProjection*>(viewInterface->getView());
      if(proj)
      {
         return proj;
      }
      ossimImageGeometry* geom = dynamic_cast<ossimImageGeometry*> (viewInterface->getView());
      if(geom)
      {
         return geom->getProjection();
      }
   }
#endif /* Replaced deprecated code. (drb) */

   return 0;
}

ossimProjection* oms::Util::createProjection(const std::string& type,
                                             const ossimGrect& rect)
{
   return createProjection(type, rect.midPoint());
}

ossimProjection* oms::Util::createProjection(const std::string& type,
                                             const ossimGpt& groundPoint)
{
   ossimProjection* result = ossimProjectionFactoryRegistry::instance()->createProjection(type);

   if(result)
   {
      ossimMapProjection* proj = dynamic_cast<ossimMapProjection*>(result);
      if(proj)
      {
         proj->setOrigin(groundPoint);
      }
   }

   return result;
}

ossimProjection* oms::Util::createProjection(ossimImageHandler* handler)
{
   ossimProjection* proj=0;

   if(handler)
   {
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if(geom.valid())
      {
         proj = geom->getProjection();
      }
   }

   return proj;
}

ossimProjection* oms::Util::createViewProjection(ossimImageSource* inputSource,
                                                 const std::string& projectionType)
{
   ossimKeywordlist kwl;
   bool needsAdjusting = true;
   ossimRefPtr<ossimImageGeometry> geom = inputSource->getImageGeometry();
   if(!geom->getProjection())
   {
      return 0;
   }
   ossimRefPtr<ossimProjection> inputSourceProjection = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
   if(!inputSourceProjection) return 0;
   ossimProjection* result = ossimProjectionFactoryRegistry::instance()->createProjection(projectionType);

   if(!result)
   {
      if(!dynamic_cast<ossimMapProjection*>(inputSourceProjection.get()))
      {
         result = ossimProjectionFactoryRegistry::instance()->createProjection(ossimString("ossimEquDistCylProjection"));
      }
      else
      {
         result = (ossimProjection*)inputSourceProjection->dup();
         needsAdjusting = false;
      }
   }
   if(result&&needsAdjusting)
   {
      if(result)
      {
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(result);

         // for now, can only adjust map type projecitons
         if(mapProj)
         {
            if(inputSourceProjection.valid())
            {
               ossimDrect rect = inputSource->getBoundingRect();
               if(!rect.hasNans())
               {
                  ossimGpt gpt;
                  inputSourceProjection->lineSampleToWorld(rect.midPoint(), gpt);
                  mapProj->setOrigin(gpt);
                  mapProj->setMetersPerPixel(inputSourceProjection->getMetersPerPixel());
                  mapProj->update();
                  mapProj->setUlTiePoints(gpt);
               }
            }
         }
      }
   }
   return result;
}

ossimProjection* oms::Util::createViewProjection(ossimConnectableObject* inputSource,
                                                 const std::string& projectionType)
{
   ossimImageSource* imageSource = dynamic_cast<ossimImageSource*>(inputSource);
   if(imageSource)
   {
      return createViewProjection(imageSource, projectionType);
   }

   return 0;
}

void oms::Util::setAllViewProjections(ossimConnectableObject* input,
                                      ossimProjection* proj,
                                      bool cloneViewFlag)
{
   // Find all the views from input.
   ossimTypeNameVisitor visitor( ossimString("ossimViewInterface"),
                                 false, // firstofTypeFlag
                                 (ossimVisitor::VISIT_INPUTS|
                                  ossimVisitor::VISIT_CHILDREN) );
   input->accept( visitor );

   // Update each view projection.
   ossim_uint32 idx = 0;
   const ossim_uint32 SIZE = visitor.getObjects().size();
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimViewInterface* viewInterface = visitor.getObjectAs<ossimViewInterface>( idx );
      if (viewInterface)
      {
         if ( cloneViewFlag )
         {
            ossimRefPtr<ossimProjection> tempProj = (ossimProjection*)proj->dup();  
            viewInterface->setView(tempProj.get());
         }
         else
         {
            viewInterface->setView(proj);
         }
      }
   }

   // Lets initialize everyone else after we set all views just incase there are dependencies.
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimConnectableObject* obj = visitor.getObjectAs<ossimConnectableObject>( idx );
      if ( obj )
      {
         ossimRefreshEvent evt( obj );
         obj->fireEvent(evt);
         obj->propagateEventToOutputs(evt);
      }
   }
   
#if 0 /* Replaced deprecated code. (drb) */

   ossimConnectableObject::ConnectableObjectList viewList;
   
   input->findAllInputsOfType(viewList,
                              "ossimViewInterface",
                              true,
                              true);
   if(PTR_CAST(ossimViewInterface, input))
   {
      viewList.push_back(input);
   }
   if(viewList.size())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = viewList.size();
      for(idx = 0; idx < size;++idx)
      {
         ossimViewInterface* interface = PTR_CAST(ossimViewInterface, viewList[idx].get());
         if(interface)
         {
            if(cloneViewFlag)
            {
               ossimRefPtr<ossimProjection> tempProj = (ossimProjection*)proj->dup();
               
               interface->setView(tempProj.get());
            }
            else
            {
               interface->setView(proj);
            }
         }
      }
      // lets initialize everyone else after we set all views just incase there are dependencies
      //
      for(idx = 0; idx < size;++idx)
      {
         ossimRefreshEvent evt(viewList[idx].get());
         viewList[idx]->fireEvent(evt);
         viewList[idx]->propagateEventToOutputs(evt);
      }
   }
#endif /* Replaced deprecated code. (drb) */
}

void oms::Util::setAllViewGeometries(ossimConnectableObject* input,
                                     ossimObject* geom,
                                     bool cloneViewFlag)
{
   // Find all the views from input.
   ossimTypeNameVisitor visitor( ossimString("ossimViewInterface"),
                                 false, // firstofTypeFlag
                                 (ossimVisitor::VISIT_INPUTS|
                                  ossimVisitor::VISIT_CHILDREN) );
   input->accept( visitor );
   
   // Update each view projection.
   ossim_uint32 idx = 0;
   const ossim_uint32 SIZE = visitor.getObjects().size();
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimViewInterface* viewInterface = visitor.getObjectAs<ossimViewInterface>( idx );
      if (viewInterface)
      {
         if ( cloneViewFlag )
         {
            ossimRefPtr<ossimObject> tempProj = (ossimProjection*)geom->dup();  
            viewInterface->setView( tempProj.get() );
         }
         else
         {
            viewInterface->setView( geom );
         }
      }
   }

   // Lets initialize everyone else after we set all views just incase there are dependencies.
   for( idx = 0; idx < SIZE; ++idx )
   {
      ossimRefPtr<ossimConnectableObject> obj = visitor.getObjectAs<ossimConnectableObject>( idx );
      if ( obj.valid() )
      {
         ossimRefreshEvent evt( obj.get() );
         obj->fireEvent(evt);
         obj->propagateEventToOutputs(evt);
      }
   }
   
#if 0 /* Replaced deprecated code. (drb) */ 
   ossimConnectableObject::ConnectableObjectList viewList;
   
   input->findAllInputsOfType(viewList,
                              "ossimViewInterface",
                              true,
                              true);
   if(PTR_CAST(ossimViewInterface, input))
   {
      viewList.push_back(input);
   }
   if(viewList.size())
   {
      ossim_uint32 idx = 0;
      ossim_uint32 size = viewList.size();
      for(idx = 0; idx < size;++idx)
      {
         ossimViewInterface* interface = PTR_CAST(ossimViewInterface, viewList[idx].get());
         if(interface)
         {
            if(cloneViewFlag)
            {
               ossimRefPtr<ossimObject> tempProj = geom->dup();
               interface->setView(tempProj.get());
            }
            else
            {
               interface->setView(geom);
            }
         }
      }
      // lets initialize everyone else after we set all views just incase there are dependencies
      //
      for(idx = 0; idx < size;++idx)
      {
         ossimRefreshEvent evt(viewList[idx].get());
         viewList[idx]->fireEvent(evt);
         viewList[idx]->propagateEventToOutputs(evt);
      }
   }
#endif /* Replaced deprecated code. (drb) */
}

void oms::Util::updateProjectionToFitOutputDimensions(ossimProjection* projectionToUpdate,
                                                      const ossimIrect& inputBounds,
                                                      ossim_uint32 outputWidth,
                                                      ossim_uint32 outputHeight,
                                                      bool keepAspectFlag)
{
   if(keepAspectFlag)
   {
      ossim_uint32 maxRes = ossim::max(inputBounds.width(), inputBounds.height());
      ossim_uint32 maxOutputRes = ossim::max(outputWidth, outputHeight);
      if(maxRes > maxOutputRes)
      {
         double scale = 0.0;

         if(maxOutputRes > 1)
         {
            scale = (double)maxRes/(double)(maxOutputRes-1);
         }
         else
         {
            scale = (double)maxRes/(double)(maxOutputRes);
         }
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(projectionToUpdate);
         if(mapProj)
         {
            mapProj->applyScale(ossimDpt(scale,scale), true);
         }
      }
   }
   else
   {
      if((outputWidth > 1)&&
         (outputHeight > 1))
      {
         double xScale = (double)inputBounds.width()/((double)outputWidth-1);
         double yScale = (double)inputBounds.height()/((double)outputHeight-1);
         ossimMapProjection* mapProj = dynamic_cast<ossimMapProjection*>(projectionToUpdate);
         if(mapProj)
         {
            mapProj->applyScale(ossimDpt(xScale, yScale), true);
         }
      }
   }
}


void oms::Util::computeGroundPointsFromCenterRadius(std::vector<ossimGpt>& result,
                                                  const ossimProjection* proj,
                                                  const ossimGpt& gpt,
                                                  const double& radius,
                                                  const ossimUnitType& radiusUnits)
{
   const ossimMapProjection* mapProj = dynamic_cast<const ossimMapProjection*>(proj);
   ossimUnitConversionTool unitConversion(radius, radiusUnits);
   double meterRadius = unitConversion.getMeters();
   double angularDegreeRadius = unitConversion.getDegrees();
   ossimGpt topMost, bottomMost, leftMost, rightMost;

   if(mapProj)
   {
      ossimDpt centerEastingNorthing = proj->forward(gpt);
      leftMost = proj->inverse(ossimDpt(centerEastingNorthing.x-meterRadius,
                                        centerEastingNorthing.y));
      rightMost = proj->inverse(ossimDpt(centerEastingNorthing.x+meterRadius,
                                        centerEastingNorthing.y));
      topMost = proj->inverse(ossimDpt(centerEastingNorthing.x,
                                       centerEastingNorthing.y + meterRadius));
      bottomMost = proj->inverse(ossimDpt(centerEastingNorthing.x,
                                          centerEastingNorthing.y - meterRadius));

   }
   else
   {
      leftMost = ossimGpt(gpt.latd(),
                          gpt.lond() - angularDegreeRadius,
                          gpt.height(),
                          gpt.datum());
      rightMost = ossimGpt(gpt.latd(),
                           gpt.lond() + angularDegreeRadius,
                           gpt.height(),
                           gpt.datum());

      topMost = ossimGpt(gpt.latd() + angularDegreeRadius,
                         gpt.lond(),
                         gpt.height(),
                         gpt.datum());

      bottomMost = ossimGpt(gpt.latd() - angularDegreeRadius,
                            gpt.lond(),
                            gpt.height(),
                            gpt.datum());

   }
   ossimGrect grect(leftMost, rightMost, topMost, bottomMost);
   result.push_back(grect.ul());
   result.push_back(grect.ur());
   result.push_back(grect.lr());
   result.push_back(grect.ll());
}

void oms::Util::computeGroundRect(ossimGrect& result,
                                  const ossimProjection* proj,
                                  const ossimDrect& rect)
{
   ossimGpt pt1;
   ossimGpt pt2;
   ossimGpt pt3;
   ossimGpt pt4;

   proj->lineSampleToWorld(rect.ul(), pt1);
   proj->lineSampleToWorld(rect.ur(), pt2);
   proj->lineSampleToWorld(rect.lr(), pt3);
   proj->lineSampleToWorld(rect.ll(), pt4);

   result = ossimGrect(pt1, pt2, pt3, pt4);

}

void oms::Util::computeGroundRect(ossimGrect& result,
                                  const ossimProjection* proj,
                                  const ossimIrect& rect)
{
   ossimGpt pt1;
   ossimGpt pt2;
   ossimGpt pt3;
   ossimGpt pt4;

   proj->lineSampleToWorld(rect.ul(), pt1);
   proj->lineSampleToWorld(rect.ur(), pt2);
   proj->lineSampleToWorld(rect.lr(), pt3);
   proj->lineSampleToWorld(rect.ll(), pt4);

   result = ossimGrect(pt1, pt2, pt3, pt4);
}

void oms::Util::computeCenterGroundPoint(ossimGpt& result,
                                         const ossimProjection* proj,
                                         const ossimIrect& rect)
{
   computeCenterGroundPoint(result, proj, ossimDrect(rect));
}

void oms::Util::computeCenterGroundPoint(ossimGpt& result,
                                         const ossimProjection* proj,
                                         const ossimDrect& rect)
{
   proj->lineSampleToWorld(rect.midPoint(), result);
}

void oms::Util::computeGroundPoints(std::vector<ossimGpt>& result,
                                    const ossimProjection* proj,
                                    const ossimIrect& rect)
{
   computeGroundPoints(result, proj, ossimDrect(rect));
}

void oms::Util::computeGroundPoints(std::vector<ossimGpt>& result,
                                    const ossimProjection* proj,
                                    const ossimDrect& rect)
{
   result.resize(4);

   proj->lineSampleToWorld(rect.ul(), result[0]);
   proj->lineSampleToWorld(rect.ur(), result[1]);
   proj->lineSampleToWorld(rect.lr(), result[2]);
   proj->lineSampleToWorld(rect.ll(), result[3]);

}

ossimImageSource* oms::Util::newEightBitImageSpaceThumbnailChain(ossimImageSource* inputSource,
                                                                 int xRes,
                                                                 int yRes,
                                                                 const std::string& histogramFile,
                                                                 const std::string& stretchType,
                                                                 bool keepAspectFlag)
{
   ossimImageChain* result = new ossimImageChain();
   result->connectMyInputTo(inputSource);

   ossimIrect rect = inputSource->getBoundingRect();
   std::vector<ossim_uint32> bandList;
   if(inputSource->getNumberOfOutputBands() < 3)
   {
      bandList.push_back(0);
      bandList.push_back(0);
      bandList.push_back(0);
   }
   else if(inputSource->getNumberOfOutputBands() > 3)
   {
      bandList.push_back(0);
      bandList.push_back(1);
      bandList.push_back(2);
   }
   if(bandList.size())
   {
      ossimBandSelector* bandSelector = new ossimBandSelector();
      result->add(bandSelector);
      bandSelector->setOutputBandList(bandList);
   }

   ossimFilename histoFile(histogramFile);
   if(histoFile.exists()&&!stretchType.empty())
   {
      ossimHistogramRemapper* histoRemapper = new ossimHistogramRemapper();
      result->add(histoRemapper);
      histoRemapper->setEnableFlag(true);
      histoRemapper->openHistogram(ossimFilename(histoFile));
      histoRemapper->setStretchModeAsString(stretchType, false);
   }
   double imageScaleX=1.0;
   double imageScaleY=1.0;
   double scaleX=1.0;
   double scaleY=1.0;
   ossim_uint32 desiredXRes = xRes;
   ossim_uint32 desiredYRes = yRes;
   if(keepAspectFlag)
   {
      double srcRes = rect.width() > rect.height()?rect.width():rect.height();
      double destRes = xRes < yRes?xRes:yRes;
      desiredXRes = destRes;
      desiredYRes = destRes;
      scaleX = destRes/(srcRes);
      scaleY = scaleX;
   }
   else
   {
      scaleX = (double)xRes/((double)rect.width());
      scaleY = (double)yRes/((double)rect.height());
   }
   ossimImageRenderer* renderer = new ossimImageRenderer();
   renderer->setImageViewTransform(new ossimImageViewAffineTransform(0.0, imageScaleX, imageScaleY, scaleX, scaleY, 0.0, 0.0, 0.0, 0.0));
   if(scaleX > 1.0 || scaleY > 1.0)
   {
      renderer->getResampler()->setFilterType("lanczos");
   }
   result->add(renderer);

   if(inputSource->getOutputScalarType() != OSSIM_UINT8)
   {
      ossimScalarRemapper* remapper = new ossimScalarRemapper();
      remapper->setOutputScalarType(OSSIM_UINT8);
      result->add(remapper);
   }
   result->initialize();
   ossimIrect orect = result->getBoundingRect();
   ossimRectangleCutFilter* cut = new ossimRectangleCutFilter;
   cut->setRectangle(ossimIrect(orect.ul().x,
                                orect.ul().y,
                                orect.ul().x + ossim::min(desiredXRes-1, orect.width()),
                                orect.ul().y + ossim::min(desiredYRes-1, orect.height())));
   result->add(cut);

   return result;
}

bool oms::Util::writeImageSpaceThumbnail(const std::string& inputFile,
                                         int entryId,
                                         const std::string& outFile,
                                         const std::string& writerType,
                                         int xRes,
                                         int yRes,
                                         const std::string& histogramFile,
                                         const std::string& stretchType,
                                         bool keepAspectFlag)
{
   bool result = false;
   ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(inputFile));
   ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(ossimString(writerType));
   if(handler.valid()&&writer.valid())
   {
	   handler->setCurrentEntry(entryId);

      std::string histFile = histogramFile;
      if(histFile.empty()&&!stretchType.empty())
      {
         histFile = handler->createDefaultHistogramFilename().string();
      }
      ossimRefPtr<ossimImageSource> chain = newEightBitImageSpaceThumbnailChain(handler.get(),
                                                                                xRes,
                                                                                yRes,
                                                                                histFile,
                                                                                stretchType,
                                                                                keepAspectFlag);
      
      if(chain.valid())
      {
         writer->connectMyInputTo(chain.get());
         writer->setFilename(ossimFilename(outFile));
         writer->setWriteExternalGeometryFlag(false);
         result = writer->execute();
         writer->close();
         writer->disconnect();
         writer = 0;
         
     }
      chain->disconnect();
     chain = 0;
   }
   if(handler.valid())
   {
      handler->disconnect();
      handler->close();
      handler = 0;
   }
   return result;
}

void oms::Util::getEntryList(std::vector<ossim_uint32>& entryIds, const std::string& filename)
{
   ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(filename));
   if(handler.valid())
   {
      handler->getEntryList(entryIds);
   }
}
#if 0
double oms::Util::calculateRotationRelativeToY(const ossimDpt& relativePoint)
{
   // apply a -90 degree rotation and then solve along x vector
   //
   return calculateRotationRelativeToX(ossimDpt(relativePoint.y, relativePoint.x));
}

double oms::Util::calculateRotationRelativeToX(const ossimDpt& relativePoint)
{
   double rotation = 0.0;
   ossimDpt pt(relativePoint.x, relativePoint.y);
   
   // now look at x and y component to determin quadrant
   //
   if(relativePoint.y < 0.0) // along southern Axis
   {
      rotation = 180.0 + ossim::radiansToDegrees(acos(-pt.x));
   }
   else
   {
      rotation = ossim::radiansToDegrees(acos(pt.x));
   }
   return rotation;
}
#endif
double oms::Util::imageHeading(const std::string& filename, ossim_int32 entryId)
{
   ossimRefPtr<ossimImageHandler> handler = ossimImageHandlerRegistry::instance()->open(ossimFilename(filename));
   double heading = 0.0;
   if(handler.valid())
   {
      if(entryId >= 0)
      {
         handler->setCurrentEntry(entryId);
      }
      ossimDrect rect = handler->getBoundingRect();
      ossimRefPtr<ossimImageGeometry> geom = handler->getImageGeometry();
      if(geom.valid())
      {
         ossimGpt centerGpt;
         ossimGpt upGpt;
         ossimDpt centerDpt(rect.midPoint());
         ossimDpt upDpt = centerDpt + ossimDpt(0,-rect.height()*.5);
         geom->localToWorld(centerDpt, centerGpt);
         geom->localToWorld(upDpt, upGpt);
         heading = centerGpt.azimuthTo(upGpt);
      }
   }
   
   return heading;
}

ossimRefPtr<ossimImageGeometry> oms::Util::createBilinearModel(std::vector<ossimDpt>& imagePoints,
                                                               std::vector<ossimGpt>& groundPoints)
{
   ossimImageGeometry* result = new ossimImageGeometry();
   ossimBilinearProjection* proj = new ossimBilinearProjection();
   proj->setTiePoints(imagePoints, groundPoints);
   result->setProjection(proj);
   result->setTransform(new ossim2dTo2dIdentityTransform());
   return result;
}

