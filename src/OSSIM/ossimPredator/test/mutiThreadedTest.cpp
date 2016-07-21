#include <ossimPredator/ossimPredatorVideo.h>
#include <ossimPredator/ossimPredatorInit.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimFilename.h>
#include <OpenThreads/Thread>

class TempThread : public OpenThreads::Thread
{
public:
   bool openVideo(ossimPredatorVideo* video)
   {
      return video->open(file);
   }
   virtual void run()
   {
      ossimRefPtr<ossimPredatorVideo> video;
      while(true)
      {
         video = new ossimPredatorVideo();
         if(openVideo(video.get()))
         {
            ossimRefPtr<ossimPredatorVideo::FrameInfo> frameInfo = video->nextFrame();
            if(!frameInfo.valid())
            {
               std::cout << "Unable to get the next frame!\n";
               exit(1);
            }
            else
            {
              // std::cout << "Got it!\n";
            }
            
         }
         else
         {
//            m.unlock();
            std::cout << "Unable to open file = " << file << "\n";
            exit(1);
         }
      }
   }
   ossimFilename file;
};

int main(int argc, char* argv[])
{
   TempThread t[5];
   for(int i = 0; i < 5; ++i)
   {
      t[i].file =  ossimFilename(argv[1]);
      t[i].start();
   }
   while(true);
   
   return 0;
}