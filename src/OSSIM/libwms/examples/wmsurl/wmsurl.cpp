#include <wms/wmsUrl.h>

int main(int argc, char* argv[])
{
   if(argc != 2)
   {
      std::cout << "Please supply a url as an argument and it will be split into it's parts" << std::endl;
      return 1;
   }
   wmsUrl url;

   url.url() = argv[1];

   std::cout << "URL:"<< url << std::endl;
   std::cout << "SPLIT INTO ITS PARTS" << std::endl
             << "protocol: " << url.protocol() << std::endl
             << "server:   " << url.server() << std::endl
             << "path:     " << url.path() << std::endl
             << "options:  " << url.options() << std::endl;

   std::cout << "MERGING http://a.b.c/cgi-bin/mycgi?f=g" << std::endl;
   url.mergeUrl("http",
                "a.b.c",
                "/cgi-bin/mycgi",
                "f=g");
                
   std::cout << "MERGED URL: " << url << std::endl;
                
}
