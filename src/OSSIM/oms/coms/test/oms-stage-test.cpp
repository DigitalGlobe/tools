#include <ossim/init/ossimInit.h>
#include <oms/ImageStager.h>
#include <ossim/base/ossimRefPtr.h>
#include <iostream>
int main(int argc, char* argv[])
{
   ossimInit::instance()->initialize(argc, argv);
   
   ossimRefPtr<oms::ImageStager> stager = new oms::ImageStager();
   if(stager->open(argv[1]))
   {
      std::cout << "Opened! " << argv[1] << std::endl;
      if(stager->buildHistograms()&&
         stager->buildOverviews())
      {
         std::cout << "Staged! " << argv[1] << std::endl;
      }
   }
   
   return 0;
}