
#include "ossimTieGenerator.h"
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <fstream>
#include <iostream> //TBR DEBUG
using namespace std;

static ossimTrace traceDebug("ossimTieGenerator:debug");

RTTI_DEF2(ossimTieGenerator, "ossimTieGenerator",
          ossimOutputSource, ossimProcessInterface);

ossimTieGenerator::ossimTieGenerator(ossimImageSource* inputSource)
      :
      ossimOutputSource(NULL, // owner
                        1,
                        0,
                        true,
                        true),
      ossimProcessInterface(),
      theAreaOfInterest(),
      theFilename(ossimFilename::NIL),
      theFileStream(),
      theStoreFlag(false)
{
   connectMyInputTo(0, inputSource);
   theAreaOfInterest.makeNan();
}

ossimTieGenerator::~ossimTieGenerator()
{
}

bool ossimTieGenerator::execute()
{
   bool status = true;
   static const char MODULE[] = "ossimTieGenerator::execute";

   if (traceDebug()) CLOG << " Entered..." << endl;
   
   ossimImageSource* src = reinterpret_cast<ossimImageSource*>(getInput(0));
   if (theAreaOfInterest.isNan())
   {
      // ChipMatch will provide correct ROI
      theAreaOfInterest = src->getBoundingRect(0);
   }

   //open stream
   open();

   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_EXECUTING);
   
   //write out projection info
   if (theFilename != ossimFilename::NIL)
   {
      ossimRefPtr<ossimImageGeometry> geom = src->getImageGeometry();
      ossimKeywordlist mpk;
      if(geom.valid())
      {
         geom->saveState(mpk);
      }
      theFileStream<<mpk.toString(); //writeToStream(theFileStream);
   }

   //do the actual work there
   if (status) status = getAllFeatures();  
   
   setProcessStatus(ossimProcessInterface::PROCESS_STATUS_NOT_EXECUTING);

   if (traceDebug()) CLOG << " Exited..." << endl;

   //close stream
   close();

   return status;
}

bool ossimTieGenerator::getAllFeatures()
{
   static const char MODULE[] = "ossimTieGenerator::getAllFeatures";

   if (traceDebug()) CLOG << " Entered..." << endl;

   ossimChipMatch* src = reinterpret_cast<ossimChipMatch*>(getInput(0));
   if (!src)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARN ossimTieGenerator::scanForEdges():"
         << "\nInput source is not a ossimImageChip.  Returning..." << std::endl;
      return false;
   }

   // Some constants needed throughout...
   const ossim_int32 TILE_HEIGHT    = src->getTileHeight();
   const ossim_int32 TILE_WIDTH     = src->getTileWidth();
   const ossim_int32 START_LINE = theAreaOfInterest.ul().y;
   const ossim_int32 STOP_LINE  = theAreaOfInterest.lr().y;
   const ossim_int32 START_SAMP = theAreaOfInterest.ul().x;
   const ossim_int32 STOP_SAMP  = theAreaOfInterest.lr().x;

   // For percent complete status.
   ossim_int32 tilerows=(STOP_LINE-START_LINE+TILE_HEIGHT) / TILE_HEIGHT; //ceil : (stop-start+1+size-1)/size
   ossim_int32 tilecols=(STOP_SAMP-START_SAMP+TILE_WIDTH) / TILE_WIDTH;
   double total_tiles = ((double)tilerows)*tilecols;
   double tiles_processed = 0.0;

   // Set the status message to be "scanning source for edges..."
   ossimNotify(ossimNotifyLevel_INFO) << "Getting tie points..." << std::endl;
   
   // Start off with a percent complete at 0...
   setPercentComplete(0.0);

   // loop through all tiles
   // need to use a sequencer for parallelism in the future TBD
   theTiePoints.clear();
   ossim_int32 line=START_LINE;
   ossim_int32 i,j;
   for (i=0;(i<tilerows)&&!needsAborting();++i)
   {
      ossim_int32 samp=START_SAMP;
      for (j=0;(j<tilecols)&&!needsAborting();++j )
      {
         // Get the tie points
         //TBC : can we go further than source edges with edge tiles?? TBD
         const vector<ossimTDpt>& tp = src->getFeatures(ossimIrect(ossimIpt(samp, line),ossimIpt(samp+TILE_WIDTH-1,line+TILE_HEIGHT-1)));

         if (theFilename != ossimFilename::NIL)
         {
            //write on stream
            writeTiePoints(tp);
         }
         
         if (getStoreFlag())
         {
            //store them : insert at the end (constant time) //TBD : conditional store
            theTiePoints.insert(theTiePoints.end(),tp.begin(),tp.end());
         }

         samp+=TILE_WIDTH;
         // Set the percent complete.
         tiles_processed += 1.0;
         setPercentComplete(tiles_processed/total_tiles*100.0);

//DEBUG TBR TBC
// std::cout<<"p="<<tiles_processed/total_tiles*100.0<<std::endl;

      }
      line+=TILE_HEIGHT;
   }

   setPercentComplete(100.0);
   if (traceDebug()) CLOG << " Exited." << endl;
   return true;
}

void ossimTieGenerator::writeTiePoints(const vector<ossimTDpt>& tp)
{
   for (vector<ossimTDpt>::const_iterator it=tp.begin();it!=tp.end();++it)
   {
      it->printTab(theFileStream);
      theFileStream<<endl;
   }
}

void ossimTieGenerator::setOutputName(const ossimString& filename)
{
   ossimOutputSource::setOutputName(filename);

   if (isOpen()) close();
   
   if (filename != "")
   {
      theFilename = filename;
   }
}

void ossimTieGenerator::setAreaOfInterest(const ossimIrect& rect)
{
   theAreaOfInterest = rect;
}

bool ossimTieGenerator::isOpen()const
{
   return const_cast<ofstream*>(&theFileStream)->is_open();
}

bool ossimTieGenerator::open()
{
   if(isOpen())
   {
      close();
   }

   if (theFilename == ossimFilename::NIL)
   {
      return false;
   }
   
   theFileStream.open(theFilename.c_str());

   return theFileStream.good();
}

void ossimTieGenerator::close()
{
   if (isOpen()) theFileStream.close();
}
