// ossimImageCorrelator.cpp
#include <iostream>

#include "ossimImageCorrelator.h"
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimCastTileSourceFilter.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include "ossimHarrisCorners.h"
#include "ossimChipMatch.h"
#include <ossim/imaging/ossimCacheTileSource.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimBandSelector.h>
#include "ossimTieGenerator.h"
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimFilenameProperty.h>
#include <ossim/base/ossimStringProperty.h>

using namespace std;

static ossimTrace traceDebug("ossimImageCorrelator:debug");

RTTI_DEF2(ossimImageCorrelator, "ossimImageCorrelator", ossimOutputSource, ossimProcessInterface);

ossimImageCorrelator::ossimImageCorrelator()
 : ossimOutputSource(NULL, // owner
                        0,
                        0,
                        true,
                        true),
   ossimProcessInterface(),
   theMaster(),
   theSlave(),
   theMasterBand(0),
   theSlaveBand(0),
   theScaleRatio(1.0),
   theSlaveAccuracy(15.0),
   theCornerDensity(0.002),
   theProjectionType("M"),
   theMasterPointProj("G"),
   theSlavePointProj("I"),
   theTemplateRadius(7),
   theMinCorrel(0.8),
   theHasRun(false),
   handlerM(NULL),
   handlerS(NULL),
   theMasterBandSelector(NULL),
   theSlaveBandSelector(NULL),
   theTset()
{
   //default output name : XML
   setOutputName("tiepts.xml");

   //1.add all OSSIM blocks
   // ingredients :
   // 2x ossimImageSource (for master & slave)
   // 2x ossimImagerRenderer
   // 1x ossimHarrisCorners
   // 2x ossimCastTileFilter (to get floating point
   // 1x ossimChipMatch
   // 1x ossimTieGenerator

   // don't create sources (factories will do it)
   caster.push_back(new ossimCastTileSourceFilter());
   caster.push_back(new ossimCastTileSourceFilter());
   cornerDetector      = new ossimHarrisCorners();
   matcher             = new ossimChipMatch();
   generator           = new ossimTieGenerator();
   
}

ossimImageCorrelator::~ossimImageCorrelator()
{
   //TBC : delete handlers created by factory?
   generator = 0;
   if(matcher.valid())
   {
      matcher->disconnect();
      matcher = 0;
   }
   if(cornerDetector.valid())
   {
      cornerDetector->disconnect();
      cornerDetector = 0;
   }
   
   if(caster.size())
   {
      caster[0]->disconnect();
      caster[1]->disconnect();
      caster.clear();
   }
   if(theMChain.valid())
   {
      theMChain->disconnect();
   }
   if(theSChain.valid())
   {
      theSChain->disconnect();
   }
   theMChain = 0;
   theSChain = 0;
}

ossimString ossimImageCorrelator::getRole() const
{
   ossimString role = "unknown";
   
   //use slave or master projection
   if (theProjectionType == "S")
   {
      role="slave";
   }
   else if (theProjectionType == "M")
   {
      role="master";
   }
   else
   {
      cerr<<"ossimImageCorrelator::getRole unknown output projection, need to supply it"<<endl;
   }

   return role;
}

ossimImageHandler*  ossimImageCorrelator::getProjectionHandler()
{
   //use slave or master projection
   ossimImageHandler* projHandler = 0;
   if (theProjectionType == "S")
   {
      projHandler = handlerS.get();
   }
   else if (theProjectionType == "M")
   {
      projHandler = handlerM.get();
   }
   else
   {
      cerr<<"ossimImageCorrelator::getProjectionHandler cannot get handler for " << getRole() <<endl;
   }
   return projHandler;
}

ossimRefPtr<ossimImageGeometry> ossimImageCorrelator::getOutputImageGeometry()
{
   ossimRefPtr<ossimImageGeometry> geom = 0;
   ossimImageHandler* projHandler = getProjectionHandler();
   if(projHandler)
   {
      geom = projHandler->getImageGeometry();
   }
   return geom;
}

//getOutputProjection() - define output projection
// according to projType
ossimMapProjection* ossimImageCorrelator::getOutputProjection()
{
   ossimMapProjection* mop = 0;

   ossimRefPtr<ossimImageGeometry> geom = getOutputImageGeometry();
   if( geom.valid() )
   {
      if ( geom->getProjection() )
      {
         mop = PTR_CAST(ossimMapProjection, geom->getProjection());
         if( !mop )
         {
            ossimDpt mpp = geom->getMetersPerPixel();
            ossimProjection* outProjection =
               ossimProjectionFactoryRegistry::instance()->
               createProjection(ossimString("ossimEquDistCylProjection"));
            mop = PTR_CAST(ossimMapProjection, outProjection);
         }
         
         if(mop)
         {
            mop->setDatum(ossimDatumFactory::instance()->wgs84());
            mop->update();

            // apply user scale factor (resize)
            // then hopefully overviews can be used
            if ( (theScaleRatio != 1.0) && (theScaleRatio>0) )
            {
               cout << "applying scale ratio on " << getRole() <<
                  ": "<<theScaleRatio << endl; //TBR?

               mop->applyScale(ossimDpt(1.0/theScaleRatio,1.0/theScaleRatio),
                               false);
            }
         }
      }
      else
      {
         cerr << "ossimImageCorrelator::getOutputProjection cannot create projection from " << getRole() <<" geometry." <<endl;
      }
   }
   else
   {
      cerr << "ossimImageCorrelator::getOutputProjection cannot get "
           <<getRole() << " geometry." << endl;
   }

   return mop;
}

// buildRenerer() - builds renderer for an imageSource
// accounts for :
//  -scale factor
//  -required projection
bool ossimImageCorrelator::buildRenderer(
   ossimImageChain* chain,
   ossimMapProjection* outProjection, 
   ossimImageRenderer* renderer,
   const ossimFilterResampler::ossimFilterResamplerType& stype ) const
{
   if(chain)
   {
      chain->add(new ossimCacheTileSource);
      ossimRefPtr<ossimImageGeometry> geom = chain->getImageGeometry();
      if(geom.valid()&&geom->getProjection())
      {       
         ossimImageViewProjectionTransform* transform = new ossimImageViewProjectionTransform;
         transform->setImageGeometry(geom.get());
         transform->setViewGeometry(new ossimImageGeometry(0, outProjection));
         renderer->setImageViewTransform(transform);
         renderer->getResampler()->setFilterType(stype);
         chain->add(renderer);
         chain->add(new ossimCacheTileSource);
      }
      else
      {
         cerr<<"ossimImageCorrelator"<<"::buildRenderer cannot get projection from master/slave"<<endl;
         return false;
      }      
   }
   else
   {
      cerr<<"ossimImageCorrelator"<<"::buildRenderer NULL source"<<endl;
      return false;
   }
   return true;
}


bool 
ossimImageCorrelator::execute()
{
   bool result=true;

   if(theMChain.valid())
   {
      theMChain->disconnect();
   }
   if(theSChain.valid())
   {
      theSChain->disconnect();
   }
   theMChain = new ossimImageChain;
   theSChain = new ossimImageChain;

   // -- 1 -- create source handlers
   handlerM = ossimImageHandlerRegistry::instance()->open(theMaster);
   if (!handlerM)
   {
      cerr<<"ossimImageCorrelator"<<"::execute can't create handler for master image "<< theMaster <<endl;
      return false;
   }
   theMChain->add(handlerM.get());
   handlerS = ossimImageHandlerRegistry::instance()->open(theSlave);
   if (!handlerS)
   {
      cerr<<"ossimImageCorrelator"<<"::execute can't create handler for slave image  "<< theSlave <<endl;
      return false;
   }
   theSChain->add(handlerS.get());
   ossimRefPtr<ossimImageSource> theMasterSource = handlerM.get();

   // select only one band (if multiple)
   ossim_uint32 mbc = theMChain->getNumberOfOutputBands();//handlerM->getNumberOfOutputBands();
   if (mbc>1)
   {
      //add a band selector
      ossim_uint32 mb = getMasterBand();
      if (mb>=mbc) 
      {
         cerr<<"ossimImageCorrelator"<<"::execute Warning not enough bands in master, only "<< mbc <<endl;
         mb=0;
      }
      cout<<"Using band "<<mb<<" for master"<<endl; //TBR
      theMasterBandSelector = new ossimBandSelector;
      theMasterBandSelector->connectMyInputTo(0, handlerM.get());
      theMasterBandSelector->setOutputBandList(vector<ossim_uint32>(1,mb));

      theMasterSource = theMasterBandSelector.get();
      theMChain->add(theMasterBandSelector.get());
   }
   ossim_uint32 sbc = theSChain->getNumberOfOutputBands();//handlerS->getNumberOfOutputBands();
   if (sbc>1)
   {
      //add a band selector
      ossim_uint32 sb = getSlaveBand();
      if (sb>=sbc) 
      {
         cerr<<"ossimImageCorrelator"<<"::execute Warning not enough bands in slave, only "<< sbc <<endl;
         sb=0;
      }
      cout<<"Using band "<<sb<<" for slave"<<endl; //TBR
      theSlaveBandSelector = new ossimBandSelector;
      theSlaveBandSelector->connectMyInputTo(0, handlerS.get());
      theSlaveBandSelector->setOutputBandList(vector<ossim_uint32>(1,sb));
//      theSlaveSource = theSlaveBandSelector;
      theSChain->add(theSlaveBandSelector.get());
   }

   // -- 2 -- initialize blocks
   //init renderers
   ossimMapProjection* outProj = getOutputProjection();
   if (outProj)
   {
      rendererM = new ossimImageRenderer;
      
      result = buildRenderer(theMChain.get(), outProj, rendererM.get()); //TBD : update method //index 0 is for master
      if (!result) return result;
      
      rendererS = new ossimImageRenderer;  
      result = buildRenderer(theSChain.get(), outProj, rendererS.get()); //index 1 is for slave
      if (!result) return result;
   }
   else
   {
      return false;
   }
   //init corner Detector
   cornerDetector->setK(0.05); //hardcoded
   cornerDetector->setGaussStd(getTemplateRadius() / 2.0); //TBC : hardcoded ratio
   cornerDetector->setDensity(getCornerDensity());

   //init casters
   caster[0]->setOutputScalarType(OSSIM_FLOAT64);
   caster[1]->setOutputScalarType(OSSIM_FLOAT64);

   //init matcher
   matcher->setSlaveAccuracy(getSlaveAccuracy() * getScaleRatio()); //adapt pixel radius according to scale
   matcher->setMasterRadius(getTemplateRadius()); //in sync with GaussStd for corners
   matcher->setBias(ossimDpt(0.0,0.0));
   matcher->setMinNCC(getMinCorrel());

   generator->close();
   generator->open();
   //init gen
   generator->setOutputName("difftiepts.txt"); //create on the fly image diff text file //TBR
   generator->setStoreFlag(true); //also keep all tie points in memory

   //TBD : set area of interest to buffer around slave?

   // -- 3 -- tie blocks, from sources to outputs

   caster[0]->connectMyInputTo(0, theMChain.get());
   caster[1]->connectMyInputTo(0, theSChain.get());
   cornerDetector->connectMyInputTo(0,theMChain.get());
   matcher->connectMyInputTo(0,cornerDetector.get());
   matcher->connectMyInputTo(1,caster[0].get()); // master
   matcher->connectMyInputTo(2,caster[1].get()); // slave
   generator->connectMyInputTo(0,matcher.get());

   // -- 4 -- run
   result = generator->execute();

   const vector<ossimTDpt>& tp = generator->getTiePoints();

   computeDispStats(tp); //TBR ?

   // -- 7 -- convert "Image to Image" to "Ground to Image" tie points    //TBC : use more generic tie points
   theTset.clearTiePoints();

   //get image<->original image transform for slave
   ossimImageViewTransform* st = rendererS->getImageViewTransform();

   ossimRefPtr<ossimImageGeometry> geom = getOutputImageGeometry();
   if ( geom.valid() )
   {
      for(vector<ossimTDpt>::const_iterator it = tp.begin();
          it != tp.end() ;
          ++it)
      {
         ossimRefPtr<ossimTieGpt> tgi(new ossimTieGpt);
         //set master ground pos
         geom->localToWorld( it->getMasterPoint() , *tgi ); //TBC : is it always lon/lat WGS84?
         //set slave image position
         st->viewToImage( it->getMasterPoint() + it->getSlavePoint() , tgi->refImagePoint() );
         //set score
         tgi->setScore(it->score);
         
         //add to list
         theTset.addTiePoint(tgi);
      }
   }
   
   theTset.setMasterPath(theMaster);
   theTset.setSlavePath(theSlave);

   //slave image accuracy (pixel)
   NEWMAT::SymmetricMatrix icov(2);
   icov=0.0;
   icov(1,1)= std::pow(0.5 * st->getOutputMetersPerPixel().x / st->getInputMetersPerPixel().x, 2);
   icov(2,2)= std::pow(0.5 * st->getOutputMetersPerPixel().y / st->getInputMetersPerPixel().y, 2);
   theTset.setImageCov(icov);

   //master ground precision (degrees)
   NEWMAT::SymmetricMatrix gcov(3);
   gcov=0.0;
   gcov(1,1) = std::pow(outProj->getDecimalDegreesPerPixel().x*0.5, 2);
   gcov(2,2) = std::pow(outProj->getDecimalDegreesPerPixel().y*0.5, 2);
   gcov(3,3) = std::pow(30.0, 2); //elevation accuracy, meters^2, harcoded TBC
   theTset.setGroundCov(gcov);

   // -- 8 -- export as XML/GML
   ossimRefPtr<ossimXmlNode> set_node = theTset.exportAsGmlNode();
   ossimXmlDocument gmlDoc;
   gmlDoc.initRoot(set_node); //need namespaces etc...
   gmlDoc.write(getOutputName());

   theHasRun = true;
   return true;
}

bool
ossimImageCorrelator::isOpen() const
{
   if(generator.valid())
   {
      return generator->isOpen();
   }
   return false;
}

bool
ossimImageCorrelator::open()
{
   if(generator.valid())
   {
      return generator->open();
   }

   return false;
}

void
ossimImageCorrelator::close()
{
   if(generator.valid())
   {
      return generator->close();
   }
}

bool
ossimImageCorrelator::addListener(ossimListener* listener)
{
   if(generator.valid())
   {
      return generator->addListener(listener);
   }

   return false;
}

bool
ossimImageCorrelator::removeListener(ossimListener* listener)
{
   if(generator.valid())
   {
      return generator->removeListener(listener);
   }

   return false;
}

void
ossimImageCorrelator::computeDispStats(const vector<ossimTDpt>& tp)const
{
   // -- 5 -- 2D statistics for tie points displacements
   // get 1st and 2nd moments

   cout<<"Statistics for tie point displacements"<<endl;
   cout<<"number of tie points="<<tp.size()<<endl;
   vector<ossimTDpt>::const_iterator it;

   double isr  = 1.0/getScaleRatio();
   double isr2 = isr*isr;
   //mean
   double sx=0.0, sy=0.0;
   for (it = tp.begin() ; it != tp.end() ; ++it)
   {
      sx += it->tie.x;
      sy += it->tie.y;
   }
   sx /= tp.size(); //mean values
   sy /= tp.size();
   cout <<"Mean displacements: mx="<<sx*isr<<" my="<<sy*isr<<endl;

   //x,y correlation
   double dx,dy;
   double sx2=0.0, sy2=0.0, sxy=0.0;
   for (it = tp.begin() ; it != tp.end() ; ++it)
   {
      dx = it->tie.x - sx;
      dy = it->tie.y - sy;
      sx2 += dx*dx;
      sxy += dx*dy;
      sy2 += dy*dy;
   }
   sx2 /= tp.size();
   sy2 /= tp.size();
   sxy /= tp.size();

   cout <<"Correlation coeffs: Cx2="<<sx2*isr2<<" Cxy="<<sxy*isr2<<" Cy2="<<sy2*isr2<<endl;
   //calculate eigenvalues + angle for major axis
   double delta = pow(sx2-sy2,2) + 4*pow(sxy,2);
   double lmax = 0.5*( sx2+sy2 + sqrt(delta) ); //>=0
   double lmin = 0.5*( sx2+sy2 - sqrt(delta) ); //>=0
   double dirx = sx2 - lmax, diry = -sxy; //direction vector for major axis
   double angle = atan2(-diry, dirx); //invert y since image is oriented downwards and we want map oriented angles
   cout<<"RMS along eigen axes : rmax="<<sqrt(lmax)*isr<<" rmin="<<sqrt(lmin)*isr<<endl;
   cout<<"Major axis angle with x axis (degrees): "<< angle / M_PI * 180.0<<endl; 
}

void ossimImageCorrelator::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid())
   {
      return;
   }
   
   ossimString name = property->getName();
   
   if(name == "master_filename")
   {
      setMaster(ossimFilename(property->valueToString()));
   }
   else if(name == "slave_filename")
   {
      setSlave(ossimFilename(property->valueToString()));
   }
   else if(name == "master_band")
   {
      setMasterBand(property->valueToString().toUInt32());
   }
   else if(name == "slave_band")
   {
      setSlaveBand(property->valueToString().toUInt32());
   }
   else if(name == "scale_ratio")
   {
      setScaleRatio(property->valueToString().toFloat64());
   }
   else if(name == "corner_density")
   {
      setCornerDensity(property->valueToString().toFloat64());
   }
   else if(name == "min_correl")
   {
      setMinCorrel(property->valueToString().toFloat64());
   }
   else if(name == "template_radius")
   {
      setTemplateRadius(property->valueToString().toUInt32());
   }
   else if(name == "slave_accuracy")
   {
      setSlaveAccuracy(property->valueToString().toFloat64());
   }
   else if(name == "projection_type")
   {
      setProjectionType(property->valueToString());
   }
   else if(name == "output_filename")
   {
      setOutputName(property->valueToString());
   }
   else
   {
      ossimOutputSource::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimImageCorrelator::getProperty(const ossimString& name)const
{
   ossimRefPtr<ossimProperty> result = 0;
   
   if(name == "master_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, getMaster());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      
      return filenameProp;
   }
   else if(name == "slave_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, getSlave());
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_INPUT);
      
      return filenameProp;
   }
   else if(name == "master_band")
   {
      return new ossimStringProperty(name, ossimString::toString(getMasterBand()));
   }
   else if(name == "slave_band")
   {
      return new ossimStringProperty(name, ossimString::toString(getSlaveBand()));
   }
   else if(name == "scale_ratio")
   {
      return new ossimStringProperty(name, ossimString::toString(getScaleRatio()));
   }
   else if(name == "corner_density")
   {
      return new ossimStringProperty(name, ossimString::toString(getCornerDensity()));
   }
   else if(name == "min_correl")
   {
      return new ossimStringProperty(name, ossimString::toString(getMinCorrel()));
   }
   else if(name == "template_radius")
   {
      return new ossimStringProperty(name, ossimString::toString(getTemplateRadius()));
   }
   else if(name == "slave_accuracy")
   {
      return new ossimStringProperty(name, ossimString::toString(getSlaveAccuracy()));
   }
   else if(name == "projection_type")
   {
      return new ossimStringProperty(name, getProjectionType());
   }
   else if(name == "output_filename")
   {
      ossimFilenameProperty* filenameProp =
         new ossimFilenameProperty(name, theOutputName);
      
      filenameProp->setIoType(ossimFilenameProperty::ossimFilenamePropertyIoType_OUTPUT);
      
      return filenameProp;
   }
   else
   {
      return ossimOutputSource::getProperty(name);
   }

   return result;
}

void ossimImageCorrelator::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   
   propertyNames.push_back("master_filename");
   propertyNames.push_back("slave_filename");
   propertyNames.push_back("master_band");
   propertyNames.push_back("slave_band");
   propertyNames.push_back("scale_ratio");
   propertyNames.push_back("corner_density");
   propertyNames.push_back("min_correl");
   propertyNames.push_back("template_radius");
   propertyNames.push_back("slave_accuracy");
   propertyNames.push_back("projection_type");
   propertyNames.push_back("output_filename");
}
