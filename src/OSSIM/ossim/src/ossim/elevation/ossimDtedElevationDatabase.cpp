#include <ossim/elevation/ossimDtedElevationDatabase.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <sstream>
#include <iomanip>
#include <cstdlib> /* for abs(int) */
#include <OpenThreads/Thread> //### TODO: for debug, remove

static ossimTrace traceDebug("ossimDtedElevationDatabase:debug");
static OpenThreads::Mutex d_mutex; // for debug
RTTI_DEF1(ossimDtedElevationDatabase, "ossimDtedElevationDatabase", ossimElevationCellDatabase);

double ossimDtedElevationDatabase::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(!isSourceEnabled())
      return ossim::nan();

   double result = ossim::nan();
   m_mutex.lock();
   if(m_lastHandler.valid() && m_lastHandler->pointHasCoverage(gpt))
   {
      result = m_lastHandler->getHeightAboveMSL(gpt);
   }
   else
   {
      m_lastHandler = getOrCreateCellHandler(gpt);
      if(m_lastHandler.valid())
         result = m_lastHandler->getHeightAboveMSL(gpt);
   }

#if 0
   d_mutex.lock();
   cout << "THREAD ID: "<<OpenThreads::Thread::CurrentThread()->getThreadId()<<"  "
         <<"ossimDtedElevationDatabase @ "<<(unsigned long)this<<"  m_lastHandler @ "
         <<(unsigned long)m_lastHandler.get()<<endl; //### TODO: for debug, remove
   d_mutex.unlock();
#endif

   m_mutex.unlock();
   return result;
}

double ossimDtedElevationDatabase::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   double h = getHeightAboveMSL(gpt);
   if(h != ossim::nan())
   {
      double offset = getOffsetFromEllipsoid(gpt);
      
      h += offset;
   }
   
   return h;
}
bool ossimDtedElevationDatabase::open(const ossimString& connectionString)
{
   bool result = false;
   ossimFilename file = ossimFilename(connectionString);
   
   result = openDtedDirectory(file);

   return result;
}

bool ossimDtedElevationDatabase::openDtedDirectory(const ossimFilename& dir)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimDtedElevationDatabase::open entered ...\n";
   }
   bool result = dir.isDir();
   if(result)
   {
      ossim_uint32 count = 0;
      ossim_uint32 maxCount = 10;
      ossimDirectory od;
      result = od.open(dir);
      if(result)
      {
         result = false;
         ossimFilename f;
         // Get the first directory.
         od.getFirst(f, ossimDirectory::OSSIM_DIR_DIRS);
         
         do
         {
            ++count;
            // Must be a directory.
            if (f.isDir())
            {
               // Discard any full path.
               ossimFilename fileOnly = f.file();
               
               // Downcase it.
              fileOnly.downcase();
               // Must start with 'e' or 'w'.
               bool foundCell = ( ((fileOnly.c_str()[0] == 'e') || ( fileOnly.c_str()[0] == 'w')) &&
                         (fileOnly.size() == 4));
               if(foundCell)
               {
                  ossim_uint32 maxCount2 = 10;
                  ossim_uint32 count2 = 0;
                  ossimDirectory d2;
                  if(d2.open(f))
                  {
                     d2.getFirst(f, ossimDirectory::OSSIM_DIR_FILES);
                     do
                     {
                        ossimRefPtr<ossimDtedHandler> dtedHandler = new ossimDtedHandler();
                        if(dtedHandler->open(f, false))
                        {
                           if(traceDebug())
                           {
                              ossimNotify(ossimNotifyLevel_DEBUG) << "ossimDtedElevationDatabase::open: Found dted file " << f << "\n";
                           }
                           result = true;
                           m_extension = "."+f.ext();
                           m_connectionString = dir;
                           m_meanSpacing = dtedHandler->getMeanSpacingMeters();
                       }
                        dtedHandler->close();
                        dtedHandler = 0;
                        ++count2;
                     }while(!result&&d2.getNext(f)&&(count2 < maxCount2));
                  }
               }
            }
         }while(!result&&(od.getNext(f))&&(count < maxCount));
      }
   }
   
   if(result)
   {
      if(!m_geoid.valid())
      {
         m_geoid = ossimGeoidManager::instance()->findGeoidByShortName("geoid1996", false);
      }
      
      if(!m_geoid.valid()&&traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimDtedElevationDatabase::open: Unable to load goeid grid 1996 for DTED database\n";
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimDtedElevationDatabase::open leaving ...\n";
   }
   return result;
}

bool ossimDtedElevationDatabase::getAccuracyInfo(ossimElevationAccuracyInfo& info, const ossimGpt& gpt) const
{
   bool result = false;
   
   m_mutex.lock();
   ossimDtedElevationDatabase* thisPtr = const_cast<ossimDtedElevationDatabase*>(this);
   ossimRefPtr<ossimElevCellHandler> tempHandler = thisPtr->getOrCreateCellHandler(gpt);
   m_mutex.unlock();

   if(tempHandler.valid())
   {
      result = tempHandler->getAccuracyInfo(info, gpt);
   }
  return result;
}

void ossimDtedElevationDatabase::createRelativePath(ossimFilename& file, const ossimGpt& gpt)const
{
   ossimFilename lon, lat;
   int ilon = static_cast<int>(floor(gpt.lond()));
   
   if (ilon < 0)
   {
      lon = "w";
   }
   else
   {
      lon = "e";
   }
   
   ilon = abs(ilon);
   std::ostringstream  s1;
   s1 << std::setfill('0') << std::setw(3)<< ilon;
   
   lon += s1.str().c_str();//ossimString::toString(ilon);
   
   int ilat =  static_cast<int>(floor(gpt.latd()));
   if (ilat < 0)
   {
      lat += "s";
   }
   else
   {
      lat += "n";
   }
   
   ilat = abs(ilat);
   std::ostringstream  s2;
   
   s2<< std::setfill('0') << std::setw(2)<< ilat;
   
   lat += s2.str().c_str();
   
   file = lon.dirCat(lat+m_extension);
}
ossimRefPtr<ossimElevCellHandler> ossimDtedElevationDatabase::createCell(const ossimGpt& gpt)
{
  ossimRefPtr<ossimElevCellHandler> result = 0;
  ossimFilename f;
  createFullPath(f, gpt);
  if(f.exists())
  {
     ossimRefPtr<ossimDtedHandler> h = new ossimDtedHandler(f, m_memoryMapCellsFlag);
     if (!(h->getErrorStatus()))
     {
        result = h.get();
     }
  }

  return result;
}

bool ossimDtedElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix )
{
   bool result = ossimElevationCellDatabase::loadState(kwl, prefix);
   if(result)
   {
      if(!m_connectionString.empty()&&ossimFilename(m_connectionString).exists())
      {
         result = open(m_connectionString);
      }
      else
      {
         // can't open the connection because it does not exists or empty
         result = false;
      }
   }
   
   return result;
}

bool ossimDtedElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   bool result = ossimElevationCellDatabase::saveState(kwl, prefix);
   
   return result;
}

std::ostream& ossimDtedElevationDatabase::print(ostream& out) const
{
   ossimKeywordlist kwl;
   saveState(kwl);
   out << "\nossimDtedElevationDatabase @ "<< (ossim_uint64) this << "\n"
         << kwl <<ends;
   return out;
}

