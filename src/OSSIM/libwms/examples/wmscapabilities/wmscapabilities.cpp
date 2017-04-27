#include <iostream>
#include <wms/wmsNotify.h>
#include <curl/curl.h>
#include <wms/wms.h>
#include <wms/wmsClient.h>
#include <wms/wmsUrl.h>
#include <wms/wmsRefPtr.h>
#include <wms/wmsMemoryStream.h>
#include <wms/wmsCapabilitiesRoot.h>
#include <wms/wmsCapabilitiesState.h>
#include <wms/wmsCapabilitiesParser.h>

int main(int argc, char* argv[])
{
   if(argc != 2)
   {
      std::cout << "Please supply a url you wish to get the capabilities of" << std::endl;
      return 1;
   }
   // initilize communication environment
   wmsInitialize();
   
   wmsClient client;

   wmsNotify(wmsNotifyLevel_INFO)<<"Getting capabilitites from url " << wmsUrl(argv[1]) << std::endl;
   wmsRefPtr<wmsMemoryStream> memoryStream;
   if(client.getCapabilities(wmsUrl(argv[1])))
   {
      memoryStream = client.getStream();
      std::cout <<"______________________CAPABILITIES IN RAW XML FORM_______________________\n";
      std::cout.write(memoryStream->getBuffer(),
                      memoryStream->getBufferSize());
      std::cout <<"______________________END CAPABILITIES IN RAW XML FORM_______________________\n";
      
      wmsRefPtr<wmsCapabilitiesParser> parser = new wmsCapabilitiesParser;
      wmsRefPtr<wmsCapabilitiesRoot> root = parser->parse(*memoryStream);
      
      if(root.valid())
      {
         std::cout << "__________________________PARSED CAPABILITIES____________________________\n"
                   << *root << std::endl
                   << "________________________END PARSED CAPABILITIES__________________________\n";
      }
      else
      {
         std::cout << "__________________________UNABLE TO PARSE CAPABILITIES____________________________\n";
         
      }
   }
   else
   {
      std::cout << "Unable to get capabilities from url = " << argv[1] << std::endl;
   }
   // finalize communication environment
   wmsFinalize();
   
   return 0;
}
