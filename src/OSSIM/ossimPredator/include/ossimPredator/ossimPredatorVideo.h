#ifndef ossimPredatorVideo_HEADER
#define ossimPredatorVideo_HEADER
#include "ffmpegApi.h"
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimReferenced.h>
#include <ossim/base/ossimRefPtr.h>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
#include <queue>
#include <iostream>
#include <ossimPredator/ossimPredatorExport.h>
#include <ossimPredator/ossimPredatorKlvTable.h>


class OSSIMPREDATOR_DLL ossimPredatorVideo : public ossimReferenced
{
public:
   enum SeekType
   {
      SEEK_NONE     = 0,
      SEEK_RELATIVE = 1,
      SEEK_ABSOLUTE = 2
   };
   class KlvInfo : public ossimReferenced
   {
   public: 
      KlvInfo()
      :theTime(0.0)
      {
         
      }
      KlvInfo(const KlvInfo& src)
      :theTime(0.0),
      theTable(src.theTable.valid()?src.theTable->dup():0)
      {
         
      }
      ~KlvInfo()
      {
         theTable = 0;
      }
      KlvInfo* dup()const
      {
         return new KlvInfo(*this);
      }
      void setTable(ossimPredatorKlvTable* value)
      {
         theTable = value;
      }
      void setTime(ossim_float64 t)
      {
         theTime = t;
      }
      const ossimPredatorKlvTable* table()const
      {
         return theTable.get();
      }
      ossimPredatorKlvTable* table()
      {
         return theTable.get();
      }
      ossim_float64 time()const
      {
         return theTime;
      }
   protected:
      ossim_float64                      theTime; // seconds since first Klv
      ossimRefPtr<ossimPredatorKlvTable> theTable;
   };
   class FrameInfo : public ossimReferenced
   {
   public:
      FrameInfo()
         :theVideoFrameTime(0.0),
          theKlvInfo(new KlvInfo())
      {
      }
      FrameInfo(const FrameInfo& src)
         :theRgbBuffer(src.theRgbBuffer),
         theVideoFrameTime(src.theVideoFrameTime),
         theImageRect(src.theImageRect),
         theKlvInfo(src.theKlvInfo->dup())
      {
      }
      ~FrameInfo()
      {
      }
      FrameInfo* dup()const
      {
         return new FrameInfo(*this);
      }
      ossimIrect imageRect()const
      {
         return theImageRect;
      }
      void setImageRect(const ossimIrect& rect)
      {
         theImageRect = rect;
      }
      void allocate()
      {
         ossim_uint32 size = theImageRect.width()*theImageRect.height()*3;

         theRgbBuffer.resize(size);
      }
      void setKlvTable(ossimPredatorKlvTable* table)
      {
         theKlvInfo->setTable(table);
      }
      ossimPredatorKlvTable* klvTable()
      {
         return theKlvInfo->table();
      }
      const ossimPredatorKlvTable* klvTable()const
      {
         return theKlvInfo->table();
      }
      ossim_uint8* rgbBuffer()
      {
         return &theRgbBuffer.front();
      }
      const ossim_uint8* rgbBuffer()const
      {
         return &theRgbBuffer.front();
      }
      ossim_uint32 rgbBufferSizeInBytes()const
      {
         return theRgbBuffer.size();
      }
      void setVideoFrameTime(ossim_float64 t)
      {
         theVideoFrameTime = t;
      }
      ossim_float64 videoTime()const
      {
         return theVideoFrameTime;
      }
      void setKlvTime(ossim_float64 t)
      {
         theKlvInfo->setTime(t);
      }
      ossim_float64 klvTime()const
      {
         return theKlvInfo->time();
      }
      ossimRefPtr<KlvInfo> klvInfo()
      {
         return theKlvInfo.get();
      }
      
      
   protected:
      std::vector<ossim_uint8>           theRgbBuffer;
      ossim_float64                      theVideoFrameTime;// seconds sice first frame 
      ossimIrect                         theImageRect;
      ossimRefPtr<KlvInfo>               theKlvInfo;
   };
   ossimPredatorVideo();
   ~ossimPredatorVideo();

   bool open(const ossimFilename& videoFile);
   void close();
   bool isOpen()const;

   /**
    * This will cycle and read the first KLV.  Sense we have to incorporate time we will
    * have to look at the video frame to do a clock time.  If no time is set then we do
    * not need to.
    *
    * @param maxTimeToSearchInSeconds The maximum time to search in seconds.  If this is
    *                                 larger then 0.0 then it will tell the method
    *                                 to use the timing in the video packet.  If not set
    *                                 it will search until it hits the first klv or end of file
    *
    * @return The first valid klv in the sequence
    */
   ossimRefPtr<ossimPredatorVideo::KlvInfo> firstKlv(double maxTimeToSearchInSeconds=0);
   
   /**
    * This allows you to seq1uence to the next klv only.  If you want vide info and klv
    * information then you should use the nextFrame call.
    */
   ossimRefPtr<ossimPredatorVideo::KlvInfo> nextKlv();
   
   ossimRefPtr<ossimPredatorVideo::FrameInfo> nextFrame();
   
   /**
    * Will rewind all active streams
    */
   void rewind();
   
   /**
    *
    */
   void seek(ossim_float64 reference, ossimPredatorVideo::SeekType seekType);
   ossim_float64 duration()const
   {
      return theDurationInSeconds;
   }
   ossim_float64 referenceTime()const;
/*    void seek(ossim_float64 timeInSeconds, */
/*              ossimPredatorVideo::SeekType seekType=CURRENT_POSITION); */
   
/*    void seek(ossim_int64 */
/*    ossimRefPtr<ossimPredatorVideo::GeometryInfo> currentGeometry(); */
/*    const ossimRefPtr<ossimPredatorVideo::GeometryInfo> currentGeometry()const; */

   ossimFilename filename()const;
   ossim_float64 videoFrameRate()const;
   ossim_float64 videoClock()const;
   
   ossim_uint32 imageWidth()const
   {
      return theImageWidth;
   }
   ossim_uint32 imageHeight()const
   {
      return theImageHeight;
   }
   bool seekingFlag()const;
   void setFirstFrameFlag(bool flag);
   bool firstFrameFlag()const;
   /**
    * Will scan the current stream and print out meta information
    * such as timing, ... etc.
    */ 
   void debugScan();

   static ossim_int64 MY_NOPTS_VALUE;
  
protected:
   void adjustSeek();
   ossim_float64 synchronizeVideo(AVFrame *src_frame, ossim_float64 pts);
   void flushQueues();

   class TimestampKlv : public ossimReferenced
   {
   public:
      ossim_float64                      theOffsetInSeconds; // offset in seconds from the first time stamp
      ossimRefPtr<ossimPredatorKlvTable> theKlvInfo;         // geometry information 
   };
   ossimFilename    theFilename;
   AVFormatContext *theFormatCtx;
   ossim_int32      theVideoStreamIndex;
   ossim_int32      theKlvStreamIndex;
   ossim_uint32     theImageWidth;
   ossim_uint32     theImageHeight;
   AVFrame         *theVideoFrame; 
   AVFrame         *theRgbFrame; 
   AVCodecContext  *theVideoCodecCtx;
   AVCodec         *theVideoDecoder;
   ossim_uint8     *theBuffer;
   ossim_uint32     theBufferSizeInBytes;
   ossim_float64    theVideoFrameRate;
   ossim_float64    theVideoClock;
   ossim_uint64     theKlvTimestamp;

   bool             theFirstFrameFlag;
   bool             theFirstKlvTimeSetFlag;
   ossim_float64    theFirstFrameTime; // seconds
   ossim_float64    theFirstKlvTime;   // seconds

   ossim_float64    theDurationInSeconds;
   std::queue<ossimRefPtr<ossimPredatorVideo::TimestampKlv> > theKlvQueue;
   ossimRefPtr<ossimPredatorVideo::FrameInfo> theCurrentFrameInfo;

   mutable OpenThreads::Mutex           theSeekMutex;
   ossim_float64                theSeekTarget;
   ossimPredatorVideo::SeekType theSeekTargetType;
   bool                         theSeekingFlag;
   
   struct SwsContext *theImageConvertContext;
};
#endif // ossimPredatorVideo_HEADER
