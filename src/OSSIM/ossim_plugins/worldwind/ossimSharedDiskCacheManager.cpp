#include "ossimSharedDiskCacheManager.h"

ossimSharedDiskCacheManager* ossimSharedDiskCacheManager::theInstance = 0;
ossimSharedDiskCacheManager::ossimSharedDiskCacheManager()
{
   theInstance = this;
}

ossimSharedDiskCacheManager::~ossimSharedDiskCacheManager()
{
   theInstance = 0;
}

ossimSharedDiskCacheManager* ossimSharedDiskCacheManager::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimSharedDiskCacheManager;
   }

   return theInstance;
}

ossimRefPtr<ossimDiskCache> ossimSharedDiskCacheManager::getDiskCache(const ossimFilename& file)const
{
//    std::cout << "ossimSharedDiskCacheManager::getDiskCache" << std::endl;
   ossim_uint32 idx = 0;


   if(!file.isDir())
   {
      return 0;
   }

//    std::cout << 1 << std::endl;
   for(idx = 0; idx < theDiskCaches.size(); ++idx)
   {
      if(theDiskCaches[idx]->rootDirectory() == file)
      {
         return theDiskCaches[idx];
      }
   }

   ossimRefPtr<ossimDiskCache> tempCache = new ossimDiskCache;

   if(tempCache->open(file))
   {
      theDiskCaches.push_back(tempCache);
      return tempCache;
   }
   else
   {
      // notify error
   }

   return 0;
}

void ossimSharedDiskCacheManager::releaseDiskCache(const ossimFilename& file)const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theDiskCaches.size(); ++idx)
   {
//       std::cout << "Testing root directory = " << theDiskCaches[idx]->rootDirectory() << " == " << file << std::endl;
      if(theDiskCaches[idx]->rootDirectory() == file)
      {
         if(theDiskCaches[idx]->referenceCount() == 1)
         {
            theDiskCaches[idx] = 0;
            theDiskCaches.erase(theDiskCaches.begin() + idx);
            return;
         }
      }
   }
}
