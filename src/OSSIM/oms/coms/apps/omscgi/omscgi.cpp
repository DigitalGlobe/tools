//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: main Common Gateway Interface (CGI) application for
// OSSIM Mapping Service (OMS.
//
// $Id: ossimQtSingleImageWindow.cpp 9525 2006-09-06 16:17:09Z dburken $
//----------------------------------------------------------------------------

#define A 0
#define B 1


#if A
// #include <cstring>
#include <iostream>
// #include <sstream>
#include <fstream>
#include <cstdlib> /* for getenv */

// #include <iomanip>

#include <oms/WmsRequest.h>
#include <oms/WmsServer.h>
#include <oms/Init.h>

int main(int argc, char *argv[])
{

   cout
   oms::Init::instance()->initialize(argc, argv);

   // Output a response header.
   // cout << "Content-type: image/png\n" << endl;

   // oms::WmsRequest request;
   // request.loadQueryString();

   oms::WmsServer wmsServer;
   wmsServer.loadQueryString();
   // wmsServer.setOutputStream(std::cout);
   wmsServer.execute();

   return 0;
}

#endif /* A */

#if B
#include <oms/Init.h>
#include <oms/DataInfo.h>
#include <oms/InputStream.h>
#include <oms/RasterEngine.h>
#include <oms/Info.h>
#include <oms/Dpt.h>
#include <oms/Ipt.h>
#include <oms/Irect.h>
#include <oms/Drect.h>
#include <ossim/base/ossimNotify.h>

using namespace std;

int main(int argc, char *argv[])
{

   // cout << "a..." << endl;

   oms::Init::instance()->initialize(argc, argv);

   // cout << "b..." << endl;
   
   // ossimNotify(ossimNotifyLevel_DEBUG) << "argc: " << argc << endl;
   
//    for (int i = 0; i < argc; ++i)
//    {
//       ossimNotify(ossimNotifyLevel_DEBUG)
//          << "argv[" << i << "]: " << argv[i] << endl;
//    }

//    std::string qs = getenv("QUERY_STRING");
//    ossimNotify(ossimNotifyLevel_DEBUG)
//          << "QUERY_STRING" << qs << endl;
   
   
   // Output a response header.
   // cout << "Content-type: image/png\n" << endl;

   // std::streambuf ob;
   // std::ofstream out;

   // ossimRefPtr<ossimOStream> out = new ossimOStream1();
   
//    out.copyfmt(std::cout);
//    out.clear(std::cout.rdstate());
//    out.basic_ios<char>::rdbuf(std::cout.rdbuf());
   
   // (std::cout.rdbuf(), ios_base::binary);

   // ios_base::fmtflags myOpts = ios_base::binary;
   // ios_base::fmtflags origFlags = out.flags();
   // cout.flags(origFlags

   std::string s = "/data/image_formats/tiff/point.tif";

   oms::RasterEngine re;
   oms_uint32 resLevel = 1;
   std::vector<oms_uint32> bandList(0);
   std::string writerType("ossim_png");
   std::string outputFile("/tmp/foo8.png");
   std::ofstream os(outputFile.c_str());

   std::cout << "Content-type: image/png\n" << endl;
   re.writeImageToStream(s,
                         oms::Irect(oms::Ipt(25, 25), oms::Ipt(400, 400)),
                         // Irect(omsIpt(25, 25), omsIpt(50, 50)),
                         resLevel,
                         bandList,
                         writerType,
                         std::cout);
   // out);

   // cout.flags(origFlags);
   

   // cout << outputFile.c_str();

   // cout << "Calling omsRasterEngine::getImageChip..." << endl;

//    if (re.getImageChip(s,
//                        omsIrect(omsIpt(25, 25), omsIpt(400, 400)),
//                        // omsIrect(omsIpt(0, 0), omsIpt(200, 200)),
//                        resLevel,
//                        bandList,
//                        writerType,
//                        outputFile) == true)
//    {
//       cout << outputFile.c_str();

//       // cout << "Wrote file: " << outputFile.c_str() << endl;
//    }

   
//    cout << "<HTML><BODY>" << endl;
//    cout << "<p>" << endl;
//    omsInit::instance()->initialize(argc, argv);
//    cout << "</p>" << endl;
        

//    // cout << "Content-Type: text/plain\n\nhello world!" << endl;

//    cout << "</BODY></HTML>" << endl;

   return 0;
}
#endif /* B */
