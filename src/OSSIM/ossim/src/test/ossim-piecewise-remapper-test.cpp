//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Test application for ossimPiecewiseRemapper class.
// 
//----------------------------------------------------------------------------
// $Id: ossim-single-image-chain-test.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimTimer.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimPiecewiseRemapper.h>
#include <ossim/imaging/ossimSingleImageChain.h>
#include <ossim/imaging/ossimTiffWriter.h>
#include <ossim/init/ossimInit.h>
#include <iomanip>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
   ossimTimer::instance()->setStartTick();
   
   ossimInit::instance()->initialize(argc, argv);

   ossimTimer::Timer_t t1 = ossimTimer::instance()->tick();
   
   cout << "elapsed time after initialize(ms): "
        << std::setiosflags(ios::fixed)
        << std::setprecision(3)
        << ossimTimer::instance()->time_s() << "\n";

   if (argc < 2)
   {
      cout << argv[0] << "<image_file> <optional_output_file>"
           << "\nOpens up single image chain and dumps the state to keyword"
           << " list." << endl;
      return 0;
   }

   ossimRefPtr<ossimSingleImageChain> sic1 = new ossimSingleImageChain();
   if ( sic1->open( ossimFilename(argv[1]) ) )
   {
      cout << "Opened: " << argv[1] << endl;
      
      ossimRefPtr<ossimPiecewiseRemapper> pwr = new ossimPiecewiseRemapper();

      // Add to the end of the chain. So it's image_handler->piecewise_remapper.
      sic1->addFirst( pwr.get() );

      ossimKeywordlist kwl;
      std::string key;
      std::string val;
      
      key = "number_bands";
      val = "1";
      kwl.addPair(key, val);

      key = "remap_type";
      val = "linear_native";
      kwl.addPair(key, val);

      // ((<min_in> <max_in> <min_out> <max_out>),(<min_in> <max_in> <min_out> <max_out>))
      key = "band0.remap0";
      val = "((0, 127, 0, 127), (128, 255, 128, 382))";
      kwl.addPair(key, val);
      key = "band0.remap1";
      val = "((0, 382, 0, 255))";
      kwl.addPair(key, val);

      key = "scalar_type";
      val = "OSSIM_UINT8";
      kwl.addPair(key, val);

      key = "type";
      val = "ossimPiecewiseRemapper";
      kwl.addPair(key, val);

      cout << "kwl:\n" << kwl << "\n";

      pwr->loadState( kwl, 0 );
      // pwr->initialize();

      // sic1->addScalarRemapper();
      sic1->addCache();

      sic1->initialize();
      
      // Set up chain:
      if ( sic1->getImageHandler()->getNumberOfOutputBands() == 4 )
      {
         // Just guessing...
         // sic1->setThreeBandReverseFlag(true);
      }
               
      // Always have resampler cache.
      // sic1->setAddResamplerCacheFlag(true);

      // Histogram:
      // sic1->setAddHistogramFlag(true);

      // sic1->createRenderedChain();

      kwl.clear();
      sic1->saveState(kwl, 0);

      cout << "sic1 state: " << kwl << endl;
   }

   if (argc == 3)
   {
      // Write image:
      ossimRefPtr<ossimImageFileWriter> writer = new ossimTiffWriter();
      if ( writer->open( ossimFilename(argv[2]) ) )
      {
         cout << "Outputting file: " << ossimFilename(argv[2]) << endl;
         
         // Add a listener to get percent complete.
         ossimStdOutProgress prog(0, true);
         writer->addListener(&prog);
         
         writer->connectMyInputTo(0, sic1.get());
         writer->execute();
         ossimTimer::Timer_t t2 = ossimTimer::instance()->tick();
         cout << "elapsed time after write(ms): "
              << std::setiosflags(ios::fixed)
              << std::setprecision(3)
              << ossimTimer::instance()->time_s() << "\n";
         
         cout << "write time minus initialize: "
              << std::setiosflags(ios::fixed)
              << std::setprecision(3)
              << ossimTimer::instance()->delta_s(t1, t2) << "\n";
      }
   }

   return 0;
}

#if 0
      // Histogram stretch:
      if ( sic1->openHistogram(ossimHistogramRemapper::LINEAR_AUTO_MIN_MAX) == false )
      {
         cout << "Could not do histogram stretch!" << endl;
      }
      
      if (argc == 3)
      {
         // Write image:
         ossimRefPtr<ossimImageFileWriter> writer = new ossimTiffWriter();
         if ( writer->open( ossimFilename(argv[2]) ) )
         {
            cout << "Outputting file: " << ossimFilename(argv[2]) << endl;
            
            // Add a listener to get percent complete.
            ossimStdOutProgress prog(0, true);
            writer->addListener(&prog);

            writer->connectMyInputTo(0, sic1.get());
            writer->execute();
            ossimTimer::Timer_t t2 = ossimTimer::instance()->tick();
            cout << "elapsed time after write(ms): "
                 << std::setiosflags(ios::fixed)
                 << std::setprecision(3)
                 << ossimTimer::instance()->time_s() << "\n";

            cout << "write time minus initialize: "
                 << std::setiosflags(ios::fixed)
                 << std::setprecision(3)
                 << ossimTimer::instance()->delta_s(t1, t2) << "\n";
         }
      }
#endif

#if 0 
      ossimRefPtr<ossimImageGeometry> geom = sic1->getImageGeometry();
      if (geom.valid())
      {
         geom->print(cout);
      }

      // Test the load state.
      // ossimKeywordlist kwl;
      kwl.clear();
      sic1->saveState(kwl, 0);

      ossimSingleImageChain* sic2 = new ossimSingleImageChain();
      sic2->loadState(kwl, 0);

      kwl.clear();
      sic2->saveState(kwl, 0);

      cout << "\n\nSingle image chain from load state kwl\n" << kwl;
   }

#endif

#if 0

   // Create a normal chain.
   sic1 = new ossimSingleImageChain(true,  // addHistogramFlag
                                    true,  // addResamplerCacheFlag
                                    true,  // addChainCacheFlag
                                    false, // remapToEightBitFlag
                                    false, // threeBandFlag
                                    false); // threeBandReverseFlag
   
   if ( sic1->open( ossimFilename(argv[1]) ) )
   {
      sic1->createRenderedChain();
      ossimKeywordlist kwl;
      sic1->saveState(kwl, 0);
      cout << "\n\nNormal single image chain kwl\n" << kwl;
   }

   // Create a stripped down chain.
   sic1 = new ossimSingleImageChain();
   if ( sic1->open( ossimFilename(argv[1]) ) )
   {
      sic1->createRenderedChain();
      ossimKeywordlist kwl;
      sic1->saveState(kwl, 0);
      cout << "\n\nSingle image chain stripped down kwl\n" << kwl;
   }

   // Create a rgb reversed chain.
   sic1 = new ossimSingleImageChain();
   if ( sic1->open( ossimFilename(argv[1]) ) )
   {
      sic1->setThreeBandReverseFlag(true);
      sic1->createRenderedChain();
      ossimKeywordlist kwl;
      sic1->saveState(kwl, 0);
      cout << "\n\nSingle image chain rgb reversed kwl\n" << kwl;
   }

   cout << "constness test:\n";
   ossimRefPtr<const ossimSingleImageChain> consSic = sic1.get();
   ossimRefPtr<const ossimImageHandler> ihConst =  consSic->getImageHandler().get();
   cout << "image handler bands: " << ihConst->getNumberOfOutputBands() << endl;


   
   return 0;
}
#endif

