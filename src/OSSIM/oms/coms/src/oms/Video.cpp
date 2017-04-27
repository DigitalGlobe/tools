#include <oms/Video.h>


#ifdef OSSIM_PREDATOR_VIDEO_ENABLED
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimMemoryImageSource.h>
#include <ossim/imaging/ossimImageRenderer.h>
#include <ossim/imaging/ossimImageFileWriter.h>
#include <ossim/imaging/ossimImageWriterFactoryRegistry.h>
#include <ossim/projection/ossimImageViewAffineTransform.h>
#include <ossimPredator/ossimPredatorVideo.h>

class oms::Video::Info
{
public:
   std::string                     theInputFile;
   ossimRefPtr<ossimPredatorVideo> theSource;
   ossimRefPtr<ossimPredatorVideo::FrameInfo> theCurrentFrame;
   ossimRefPtr<ossimPredatorVideo::KlvInfo> theCurrentKlv;
};

oms::Video::Video()
   :theVideoInfo(new oms::Video::Info())
{
}

oms::Video::~Video()
{
   if(theVideoInfo)
   {
      delete theVideoInfo;
      theVideoInfo = 0;
   }
}

void oms::Video::close()
{
   if(theVideoInfo)
   {
      theVideoInfo->theSource       = 0;
      theVideoInfo->theCurrentFrame = 0;
   }
}

bool oms::Video::open(const std::string& file)
{
   close();
   ossimRefPtr<ossimPredatorVideo> video = new ossimPredatorVideo;
	
   if(video->open(ossimFilename(file)))
   {
      theVideoInfo->theInputFile = file;
      theVideoInfo->theSource = video.get();
   }
	
   return theVideoInfo->theSource.valid();
}

bool oms::Video::nextFrame()
{
   if(!theVideoInfo->theSource.valid()) return false;
   theVideoInfo->theCurrentFrame = theVideoInfo->theSource->nextFrame();
   if(theVideoInfo->theCurrentFrame.valid())
   {
      if(theVideoInfo->theCurrentFrame->klvInfo().valid())
      {
         theVideoInfo->theCurrentKlv = theVideoInfo->theCurrentFrame->klvInfo();
      }
   }
   return theVideoInfo->theCurrentFrame.valid();
}

bool oms::Video::nextKlv()
{
   if(!theVideoInfo->theSource.valid()) return false;
   
   theVideoInfo->theCurrentKlv = theVideoInfo->theSource->nextKlv();
   return theVideoInfo->theCurrentKlv.valid();
}

ossimPredatorVideo::FrameInfo* oms::Video::currentFrame()
{
   return theVideoInfo->theCurrentFrame.get();
}

ossimPredatorVideo::KlvInfo*   oms::Video::currentKlv()
{
   return theVideoInfo->theCurrentKlv.get();
}

bool oms::Video::writeCurrentFrameToFile(const std::string& file,
                                         ossim_uint32 resolution,
                                         const std::string& mimeType)
{
   if(!theVideoInfo->theSource.valid())
   {
      return false;
   }

   if(!theVideoInfo->theCurrentFrame->rgbBuffer())
   {
      return false;
   }
   ossimFilename filename(file);
   ossimString type = mimeType;
	
   if(type.empty())
   {
      type = "image/"+filename.ext();
   }
   ossimRefPtr<ossimImageFileWriter> writer = ossimImageWriterFactoryRegistry::instance()->createWriter(type);
   if(!writer.valid())
   {
      return false;
   }
	
   ossimIrect rect = theVideoInfo->theCurrentFrame->imageRect();
   ossimRefPtr<ossimImageData> data = new ossimImageData(0,
                                                         OSSIM_UINT8,
                                                         3,
                                                         rect.width(),
                                                         rect.height());
   data->loadTile(theVideoInfo->theCurrentFrame->rgbBuffer(),
                  ossimIrect(0,
                             0,
                             rect.width() - 1,
                             rect.height() - 1),
                  OSSIM_BIP);
   ossimRefPtr<ossimMemoryImageSource> memSource = new ossimMemoryImageSource();
   memSource->setImage(data.get());
   ossimRefPtr<ossimImageRenderer> source;
   if(resolution > 0)
   {
      double scale = (double)resolution/(double)ossim::max(rect.width(), rect.height());
      if(scale < 1.0)
      {
         source = new ossimImageRenderer;
         ossimImageViewAffineTransform* transform = new ossimImageViewAffineTransform(0,
                                                                                      1.0, 1.0,      // image scale x and y
                                                                                      scale, scale); // scale x and y
         source->setImageViewTransform(transform);
         source->connectMyInputTo(0, memSource.get());
      }
   }
   if(source.valid())
   {
      writer->connectMyInputTo(0, source.get());
   }
   else
   {
      writer->connectMyInputTo(0, memSource.get());
   }
	
   writer->setOutputName(filename);
   writer->setWriteExternalGeometryFlag(false);
   writer->setWriteOverviewFlag(false);
   writer->setWriteHistogramFlag(false);
   writer->execute();	
						
   writer->disconnect();
   memSource->disconnect();
   if(source.valid())
   {
      source->disconnect();
   }
   return filename.exists();
}
#else
oms::Video::Video()
   :theVideoInfo(0)
{
   
}

oms::Video::~Video()
{
   
}

void oms::Video::close()
{
   
}

bool oms::Video::open(const std::string& file)
{
   return false;
}

bool oms::Video::nextFrame()
{
   return false;
}

bool oms::Video::nextKlv()
{
   return false;
}

bool oms::Video::writeCurrentFrameToFile(const std::string& file,
                                         ossim_uint32 resolution,
                                         const std::string& mimeType)
{
   return false;
}

#endif /* #ifdef OSSIM_PREDATOR_VIDEO_ENABLED ... #else ... */
