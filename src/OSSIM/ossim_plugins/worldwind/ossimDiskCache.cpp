#include "ossimDiskCache.h"
#include <ossim/base/ossimDirectoryTree.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimRegExp.h>
#include <OpenThreads/ScopedLock>
#include <algorithm>

const ossimString ossimDiskCache::INDEX_FILE_NAME = "cache.idx";

ossimDiskCache::~ossimDiskCache()
{
   close();
}

bool ossimDiskCache::open(const ossimFilename& directory)
{
   close();
   
   bool result = true;
   if(directory.isDir())
   {
      theRootDirectory = directory;
      loadIndex();
   }
   else
   {
      result = false;
   }

   if(result)
   {
      dirty();
      writeIndex();
      if(!cacheIndexFile().exists())
      {
         result = false;
      }
   }
   
   return result;
}

void ossimDiskCache::close()
{
   OpenThreads::ScopedLock<OpenThreads::Mutex> lock(theMutex);
   
   // need to write access table out to disk
   writeIndex();
   theCurrentSize    = 0;
   theLastTimeStamp  = 0;
   theRootDirectory  = "";
   theFileMap.clear();
}

bool ossimDiskCache::addStream(std::istream& inputStream,
                               const ossimFilename& outputFile)
{
   ossimFilename file = fullPath(outputFile);
   if(!file.path().exists())
   {
      file.path().createDirectory();
   }
   std::ofstream out(file.c_str(),
                     std::ios::out|std::ios::binary);

   if(!out.fail())
   {
      char buf[1024];

      while(!inputStream.fail())
      {
         inputStream.read(buf, 1024);
         if(inputStream.gcount() > 0)
         {
            out.write(buf, inputStream.gcount());
         }
         else
         {
            break;
         }
      }
      out.close();
      
      accessFile(outputFile);
   }
   return true;
}

bool ossimDiskCache::addFile(const ossimFilename& relativePath)
{
   if(!relativePath.path().exists())
   {
      relativePath.path().createDirectory();
   }
   accessFile(relativePath);
   freeDiskSpace(); // free disk space if need be
   return true;
}

bool ossimDiskCache::isInCache(const ossimFilename& relativePath)const
{
   ossimFileInfoMap::const_iterator iter = theFileMap.find(relativePath);
   return(iter != theFileMap.end());
}

bool ossimDiskCache::accessFile(const ossimFilename& relativePath)
{
   ossimFilename file = fullPath(relativePath);

   ossimFileInfoMap::iterator iter = theFileMap.find(relativePath);
   
   if(iter != theFileMap.end())
   {
      iter-> second.theTimeStamp = theLastTimeStamp;
      ++theLastTimeStamp;
   }
   else
   {
      ossim_uint64 fileSize = 0;
      if(file.exists())
      {
         fileSize = file.fileSize();
         theCurrentSize += file.fileSize();
      }
      theFileMap.insert(std::make_pair(relativePath,
                                       ossimFileInfo(fileSize,
                                                     theLastTimeStamp)));
      ++theLastTimeStamp;
   }

   dirty();
   return true;
}

void ossimDiskCache::clearAll()
{
   ossimFileInfoMap::iterator iter = theFileMap.begin();

   while(iter!=theFileMap.end())
   {
      ossimFilename file = fullPath(iter->first);

      file.remove();
      theCurrentSize-=iter->second.theFileSize;
      
      ++iter;
   }
}

void ossimDiskCache::removeFile(const ossimFilename& relativePath)
{
   ossimFileInfoMap::const_iterator iter = theFileMap.find(relativePath);
   ossimFilename file = fullPath(relativePath);
   if(iter != theFileMap.end())
   {
      file.remove();
      theCurrentSize-=iter->second.theFileSize;
   }
}

void ossimDiskCache::clear(const ossimFilename& relativeDir)
{
   ossimFileInfoMap::iterator iter = theFileMap.begin();
   ossimRegExp regExp;

   regExp.compile(("^"+relativeDir).c_str());

   while(iter!=theFileMap.end())
   {
      if(regExp.find(iter->first.c_str()))
      {
         ossimFilename file = fullPath(iter->first);
         file.remove();
         theCurrentSize-=iter->second.theFileSize;
      }
      ++iter;
   }

   fullPath(relativeDir).remove();
}

void ossimDiskCache::writeIndex()
{
   if(!isDirty()) return;
   
   ossimFileInfoMap::iterator iter = theFileMap.begin();
   ossimKeywordlist kwl;
   ossim_uint64 idx=0;
   
   while(iter!=theFileMap.end())
   {
      ossimString prefix = "f" + ossimString::toString(idx) + ".";
      
      kwl.add(prefix,
              "n",
              iter->first,
              true);
      kwl.add(prefix,
              "s",
              ossimString::toString(iter->second.theFileSize),
              true);
      kwl.add(prefix,
              "t",
              ossimString::toString(iter->second.theTimeStamp),
              true);
      ++iter;
      ++idx;
   }
   kwl.add("cachesize",
           ossimString::toString(theCurrentSize),
           true);
   kwl.add("last_time_stamp",
           ossimString::toString(theLastTimeStamp),
           true);
   kwl.add("maxsize",
           ossimString::toString(theMaxCacheSize));
   kwl.add("minsize",
           ossimString(theMinCacheSize));
   
   ossimFilename indexFileName = fullPath(ossimDiskCache::INDEX_FILE_NAME);

   kwl.write(indexFileName);
   if(indexFileName.exists())
   {
      theDirtyFlag = false;
   }
}

bool ossimDiskCache::loadIndex()
{
   if(isDirty())
   {
      writeIndex();
   }
   bool result = true;
   theFileMap.clear();
   theCurrentSize = 0;
   ossimFilename indexFileName = fullPath(ossimDiskCache::INDEX_FILE_NAME);
   if(indexFileName.exists())
   {
      ossimKeywordlist kwl;

      if(!kwl.addFile(indexFileName))
      {
         return false;
      }
      
      ossimString regExpression =  ossimString("^(") +"f[0-9]+.)";
      vector<ossimString> keys =
         kwl.getSubstringKeyList( regExpression );
      ossim_uint64 n = keys.size();
      ossim_uint64 offset = 1;
      ossim_uint64 idx = 0;
      std::vector<int> theNumberList(n);
      for(idx = 0; idx < theNumberList.size();++idx)
      {
         ossimString numberStr(keys[idx].begin() + offset,
                               keys[idx].end());
         theNumberList[idx] = numberStr.toInt();
     }
     std::sort(theNumberList.begin(), theNumberList.end());
     for(idx=0;idx < theNumberList.size();++idx)
     {
        ossimString newPrefix = ossimString("f");
        newPrefix += ossimString::toString(theNumberList[idx]);
        newPrefix += ossimString(".");

        ossimString name(kwl.find(newPrefix,
                                  "n"));
        ossimString size(kwl.find(newPrefix,
                                  "s"));
        ossimString timeStamp(kwl.find(newPrefix,
                                  "t"));
        ossimString cachesize(kwl.find("cachesize"));
        ossimString lastTimeStamp(kwl.find("last_time_stamp"));
        ossimString maxSize(kwl.find("maxsize"));
        ossimString minSize(kwl.find("minsize"));
        
        theCurrentSize   = cachesize.toUInt32();
        theLastTimeStamp = lastTimeStamp.toUInt64();
        theMaxCacheSize  = maxSize.toUInt32();
        theMinCacheSize  = minSize.toUInt32();
        theFileMap.insert(std::make_pair(name,
                                         ossimFileInfo(size.toUInt64(),
                                                       timeStamp.toUInt64())));  
     } 
   }
//    else
//    {
//       ossimDirectoryTree dirTree;
//       theLastTimeStamp = 0;
//       if(dirTree.open(rootDirectory()))
//       {
//          ossimFilename file;
         
//          if(dirTree.getFirst(file))
//          {
//             do
//             {
//                if(!file.isDir())
//                {
//                   ossimFilename fileTmp = file;
//                   fileTmp.convertBackToForwardSlashes();

                  
//                   ossimFilename relativePath = stripRootDir(fileTmp);
                  
//                   theCurrentSize += file.fileSize();
//                   theFileMap.insert(std::make_pair(relativePath,
//                                                    ossimFileInfo(file.fileSize(),
//                                                                  theLastTimeStamp)));
//                }
//             }while(dirTree.getNext(file));

//             // we will start everyone with equal status if nothing is available.
//             // so all have same access count
//             //
//             ++theLastTimeStamp;
//          }
//       }

//       dirty();
//    }
   return result;
}

void ossimDiskCache::freeDiskSpace()
{
   if(theCurrentSize < theMaxCacheSize) return;
   while((!theFileMap.empty())&&
         (theCurrentSize > theMinCacheSize))
   {
      ossimFileInfoMap::iterator iter = theFileMap.begin();
      ossimFileInfoMap::iterator current = iter;
      ossim_uint64 oldestTime = theLastTimeStamp + 1;
      while(iter != theFileMap.end())
      {
         if(iter->second.theTimeStamp < oldestTime)
         {
            current = iter;
         }
         
         ++iter;
      }
      if(current != theFileMap.end())
      {
         fullPath(current->first).remove();
         theCurrentSize -= current->second.theFileSize;
         theFileMap.erase(current);
         dirty();
      }
   }
}

ossimFilename ossimDiskCache::stripRootDir(const ossimFilename& file)const
{
   ossimFilename tempF = file;
   tempF.convertBackToForwardSlashes();
   tempF = tempF.substitute(rootDirectory(), "");
   if(*tempF.begin() == '/')
   {
      tempF = ossimFilename(ossimString(tempF.begin()+1,
                                        tempF.end()));
   }

   return tempF;
}

