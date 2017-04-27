#ifndef ossimSharedDiskCacheManager_HEADER
#define ossimSharedDiskCacheManager_HEADER
#include <ossim/base/ossimRefPtr.h>
#include "ossimDiskCache.h"
#include <vector>

class ossimSharedDiskCacheManager
{
public:
   ossimSharedDiskCacheManager();
   ~ossimSharedDiskCacheManager();
   static ossimSharedDiskCacheManager* instance();

   ossimRefPtr<ossimDiskCache> getDiskCache(const ossimFilename& file)const;

   void releaseDiskCache(const ossimFilename& file)const;
   
protected:
   mutable std::vector<ossimRefPtr<ossimDiskCache> > theDiskCaches;
   static ossimSharedDiskCacheManager* theInstance;
};

#endif
