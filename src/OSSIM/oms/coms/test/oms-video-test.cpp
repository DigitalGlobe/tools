#include <oms/Video.h>
#include <ossim/init/ossimInit.h>
#include <ossimPredator/ossimPredatorVideo.h>
#include <ossimPredator/ossimPredatorKlvTable.h>
int main(int argc, char* argv[])
{
   ossimInit::instance()->initialize(argc, argv);
   
   ossimRefPtr<oms::Video> video = new oms::Video();
   
   if(video->open(argv[1]))
   {
      while(video->nextKlv())
      {
         ossimPredatorVideo::KlvInfo* info = video->currentKlv();
         std::cout << "______________________________\n" << info->table()->print(std::cout) << std::endl;;
      }
   }
   
   return 0;
}