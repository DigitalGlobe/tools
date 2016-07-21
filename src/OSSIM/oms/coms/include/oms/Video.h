#ifndef omsVideo_HEADER
#define omsVideo_HEADER 1

#include <oms/Constants.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <string>
#ifdef OSSIM_PREDATOR_VIDEO_ENABLED
#  include <ossimPredator/ossimPredatorVideo.h>
#endif

namespace oms
{
   class OMSDLL Video : public ossimReferenced
   {
   public:
      class Info;
      Video();
      virtual ~Video();
      void close();
      bool open(const std::string& file);
      bool nextFrame();
      bool nextKlv();
#ifdef OSSIM_PREDATOR_VIDEO_ENABLED
      
      ossimPredatorVideo::FrameInfo* currentFrame();
      ossimPredatorVideo::KlvInfo*   currentKlv();
#endif      
      bool writeCurrentFrameToFile(const std::string& file,
                                   ossim_uint32 resolution,
                                   const std::string& mimeType="");
      
   protected:
      Info* theVideoInfo;
      
   };
}

#endif
