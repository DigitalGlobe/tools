// $Id: omsIcpTest.cpp 10407 2007-01-31 15:35:42Z dburken $

#include <iostream>
#include <string>
using namespace std;

#include <oms/omsIcp.h>
#include <oms/omsIOException.h>


using namespace std;


int main(int argc, char *argv[])
{
   omsIcp icp;
   icp.setWriterType(std::string("tiff_tiled_band_separate"));

   //---
   // Cut from R1 test.
   //---
   icp.setInputFile(std::string("/data/mer1/image_formats/test/oms_icp/1024x1024.tif"));
   icp.setOutputFile(std::string("/data/mer1/image_formats/test/oms_icp/512x512.tif"));
   icp.setReducedResolutionLevel(1);
   try
   {
      icp.execute();
   }
   catch (omsIOException)
   {
      cerr << "bummer... cut from r1 failed..." << endl;
   }
   cout << "finished cut from r1..." << endl;

   //---
   // Thumbnail test.
   //---
   icp.setReducedResolutionLevel(0);
   icp.setThumbnailSize(128);
   icp.setOutputFile(std::string("/data/mer1/image_formats/test/oms_icp/thumbnail.tif"));
   try
   {
      icp.execute();
   }
   catch (omsIOException)
   {
      cerr << "bummer... thumbnail failed..." << endl;
   }
   cout << "finished thumbnail..." << endl;


   //---
   // Cut test.
   //---
   icp.setReducedResolutionLevel(0);
   icp.setThumbnailSize(0);
   icp.setStartLine(100);
   icp.setStopLine(355);
   icp.setStartSample(100);
   icp.setStopSample(355);
   icp.setOutputFile(std::string("/data/mer1/image_formats/test/oms_icp/cut.tif"));
   try
   {
      icp.execute();
   }
   catch (omsIOException)
   {
      cerr << "bummer... cut failed..." << endl;
   }
   cout << "finished cut..." << endl;
      
   return 0;
}
