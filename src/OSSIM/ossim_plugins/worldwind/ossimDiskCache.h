#ifndef ossimDiskCache_HEADER
#define ossimDiskCache_HEADER
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/base/ossimObject.h>
#include <OpenThreads/Mutex>
#include <map>

class ossimDiskCache : public ossimObject
{
public:
   // default to 1 gigabyte
   ossimDiskCache(ossim_uint64 sizeInMegabytes=2048)
      :theMaxCacheSize(sizeInMegabytes*1024*1024),
      theMinCacheSize(static_cast<ossim_uint64>(theMaxCacheSize*.75)),
      theCurrentSize(0),
      theLastTimeStamp(0),
      theDirtyFlag(false)
   {
   }
   ~ossimDiskCache();
   bool open(const ossimFilename& directory);
   void close();
   void setDiskCacheSize(ossim_uint64 sizeInMegaBytes)
   {
      theMaxCacheSize = sizeInMegaBytes*1024*1024;
   }
   void setMinCacheSize(ossim_uint64 sizeInMegaBytes)
   {
      theMinCacheSize = sizeInMegaBytes*1024*1024;
   }

   ossimFilename cacheIndexFile()const
   {
      return rootDirectory().dirCat(INDEX_FILE_NAME);
   }
   const ossimFilename& rootDirectory()const
   {
      return theRootDirectory;
   }

   void removeFile(const ossimFilename& relativePath);
   
   ossimFilename fullPath(const ossimFilename& relativePath)const
   {
      return theRootDirectory.dirCat(relativePath);
   }
   /**
    * The input is an input stream and writes the outputFile which is relative to this
    * root location.
    */ 
   bool addStream(std::istream& inputStream,
                  const ossimFilename& outputFile);

   bool addFile(const ossimFilename& relativeFile);
   bool isInCache(const ossimFilename& relativePath)const;
   bool accessFile(const ossimFilename& relativePath);
   
   void clearAll();
   void clear(const ossimFilename& relativeDir);

   void writeIndex();
   void dirty()
   {
      theDirtyFlag = true;
   }
   bool isDirty()const
   {
      return theDirtyFlag;
   }
   
   void freeDiskSpace();
   ossimFilename stripRootDir(const ossimFilename& file)const;
  
   static const ossimString INDEX_FILE_NAME;
protected:
   bool loadIndex();
   
   
   struct ossimFileInfo
   {
      public:
      ossimFileInfo(ossim_uint64 fileSize,
                    ossim_uint64 timeStamp)
         :theFileSize(fileSize),
          theTimeStamp(timeStamp)
         {
         }
      ossim_uint64 theFileSize;
      ossim_uint64 theTimeStamp;
   }; 
   typedef std::map<ossimFilename, ossimFileInfo> ossimFileInfoMap;
   ossim_uint64  theMaxCacheSize;
   ossim_uint64  theMinCacheSize;
   ossim_uint64  theCurrentSize;
   ossimFilename theRootDirectory;
   ossim_uint64  theLastTimeStamp;
   mutable bool  theDirtyFlag;
   /**
    * This holds the relative file name and the general file information
    */
   ossimFileInfoMap theFileMap;

   mutable OpenThreads::Mutex theMutex;
};

#endif
