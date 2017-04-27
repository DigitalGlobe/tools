#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#include <oms/omsInit.h>
#include <oms/omsImageInfo.h>
#include <oms/omsInputStream.h>
#include <oms/omsRasterEngine.h>
#include <oms/omsInfo.h>
#include <oms/omsDpt.h>
#include <oms/omsIpt.h>
#include <oms/omsDrect.h>

using namespace std;


int main(int argc, char *argv[])
{
   omsInit::instance()->initialize(argc, argv);

   std::string s = "/data/mer1/image_formats/tiff/point.tif";

   omsImageInfo ii;
   ii.open(s);
   cout << ii << endl;

   omsRasterEngine ore;
   omsImageInfo* iiPtr = ore.getNewImageInfo(s);
   if (iiPtr)
   {
      cout << *iiPtr << endl;
      delete iiPtr;
   }

   std::vector<std::string> list;
   omsInfo::instance()->getWriterList(list);
   std::vector<std::string>::const_iterator i = list.begin();
   cout << "writer list:" << endl;
   while (i != list.end())
   {
      cout << (*i).c_str() << endl;
      ++i;
   }

   omsRasterEngine re;
   oms_uint32 resLevel = 1;
   std::vector<oms_uint32> bandList(0);
   std::string writerType("ossim_png");
   std::string outputFile("/tmp/foo8.png");

   cout << "Calling omsRasterEngine::getImageChip..." << endl;

   if (re.getImageChip(s,
                       omsIrect(omsIpt(25, 25), omsIpt(400, 400)),
                       // omsIrect(omsIpt(0, 0), omsIpt(200, 200)),
                       resLevel,
                       bandList,
                       writerType,
                       outputFile) == true)
   {
      cout << "Wrote file: " << outputFile.c_str() << endl;
   }
   else
   {
      cout << "bummer..." << endl;
   }

   omsInputStream is;
   if (re.getImageStream(s,
                       omsIrect(omsIpt(25, 25), omsIpt(400, 400)),
                       // omsIrect(omsIpt(0, 0), omsIpt(200, 200)),
                       resLevel,
                       bandList,
                       writerType,
                       is) == true)
   {
      std::ofstream os("/tmp/foo10.png");
      if (os)
      {
         char c;
         while(is.get(c)) os.put(c);
         cout << "Wrote file: /tmp/foo10.png" << endl;
      }
      else
      {
         cout << "bummer..." << endl;
      }
   }
   else
   {
      cout << "bummer..." << endl;
   }

   std::string srs = "epsg:4326";
   std::string mimeType = "image/tiff";
   double ulLat = 42.532662240216432;
   double ulLon = -90.700415472789359;
   double lrLat = 42.438248921235356;
   double lrLon = -90.561432219021384;
   omsDpt origin(ulLon, ulLat);
   omsDpt widthHeight(lrLon - ulLon, ulLat - lrLat);
   outputFile = "/tmp/foo9.tif";
   
   omsDrect inBounds(origin, widthHeight);
   omsIpt size(512, 512);

    if (re.getProjectedImageChip(s,
                                 srs,
                                 inBounds,
                                 size,
                                 mimeType,
                                 outputFile))
    {
      cout << "Wrote file: " << outputFile.c_str() << endl;
   }
   else
   {
      cout << "bummer..." << endl;
   }  
   
   return 0;
}
