#include <ossimPredator/ossimPredatorInit.h>
#include <ossimPredator/ossimPredatorApi.h>
#include <OpenThreads/Mutex>
ossimPredatorInit* ossimPredatorInit::theInstance = 0;

extern "C"
{
int ffmpeg_lock_callback(void **mutex, enum AVLockOp op);
}

int ffmpeg_lock_callback(void **mutex, enum AVLockOp op)
{
   static OpenThreads::Mutex m;

   switch(op)
   {
      case AV_LOCK_CREATE:
      {
         *mutex = &m;
          break;
      }
      case AV_LOCK_OBTAIN:
      {
         ((OpenThreads::Mutex*)(*mutex))->lock();
         break;
      }
      case AV_LOCK_RELEASE:
      {
         ((OpenThreads::Mutex*)(*mutex))->unlock();
        break;      
      }
      case AV_LOCK_DESTROY:
      {
        *mutex = 0;
        break;
      }

   }

   return 0;
}

ossimPredatorInit::ossimPredatorInit()
{
   theInstance = this;
}

ossimPredatorInit* ossimPredatorInit::instance()
{
   // synch the entry of instance
   static OpenThreads::Mutex m;
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
   if(!theInstance)
   {
      theInstance = new ossimPredatorInit;
      av_lockmgr_register(&ffmpeg_lock_callback);
      // now synch the critcal section
      //
     avcodec_register_all();
     avdevice_register_all();
     av_register_all();

      av_log_set_level(AV_LOG_QUIET);
   }

   return theInstance;
}
