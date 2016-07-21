//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Hdf Info object.
// 
//----------------------------------------------------------------------------
// $Id: ossimHdfInfo.cpp 2645 2011-05-26 15:21:34Z oscar.kramer $

//ossim includes
#include "ossimHdfInfo.h"
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimKeywordlist.h>

// Static trace for debugging
static ossimTrace traceDebug("ossimHdfInfo:debug");

ossimHdfInfo::ossimHdfInfo()
: ossimInfoBase(),
theFile(),
m_hdf4Reader(0),
m_driverName()
{
}

ossimHdfInfo::~ossimHdfInfo()
{
   if (m_hdf4Reader.valid())
   {
      m_hdf4Reader = 0;
   }
   m_metaInfos.clear();
   std::map<ossim_uint32, std::vector<ossimString> >::iterator it = m_subMetaInfos.begin();
   while (it != m_subMetaInfos.end())
   {
      it->second.clear();
      it++;
   }
   m_subMetaInfos.clear();
}

bool ossimHdfInfo::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimHdfInfo::open";
   if (traceDebug())
   {    
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "file: " << file << "\n";
   }

   bool result = false;

   ossimString ext = file.ext();
   ext.downcase();

   if ( ext == "hdf" || ext == "h4" || ext == "hdf4" || 
        ext == "he4" || ext == "l1r" )
   {
      m_metaInfos.clear();
      m_bandVector.clear();

      m_hdf4Reader = new ossimHdf4Reader;
      m_hdf4Reader->setFilename(file);

      if ( m_hdf4Reader->open() )
      {
         m_metaInfos = m_hdf4Reader->getGlobalMeta();

         ossim_uint32 entryNum = m_hdf4Reader->getNumberOfEntries();
         for (ossim_uint32 i = 0; i < entryNum; i++)
         {
            m_hdf4Reader->setCurrentEntry(i);
            m_bandVector.push_back(m_hdf4Reader->getNumberOfOutputBands());
            m_subMetaInfos[i] = m_hdf4Reader->getSubDatasetMeta();
         }
         m_driverName = "hdf4";
         theFile = file;
         result = true;
      }
      else
      {
         m_hdf4Reader = 0;
      }
   }

   if (traceDebug())
   {    
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true\n":"false\n");
   }

   return result;
}

std::ostream& ossimHdfInfo::print(std::ostream& out) const
{
   static const char MODULE[] = "ossimHdfInfo::print";
   if (traceDebug())
   {    
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   //display global meta first
   ossimString prefix = ossimString(m_driverName + ".").downcase();
   for (ossim_uint32 i = 0; i < m_metaInfos.size(); i++)
   {
      out << prefix << m_metaInfos[i] << "\n";
   }

   ossim_uint32 index = 0;
   std::map<ossim_uint32, std::vector<ossimString> >::const_iterator it = m_subMetaInfos.begin();
   while (it != m_subMetaInfos.end())
   {
      ossimString imagePrefix = prefix + "image" + ossimString::toString(index) + ".";
      std::vector<ossimString> subMeta = it->second;
      ossim_uint32 numOfBands = m_bandVector[index];
      for (ossim_uint32 i = 0; i < numOfBands; i++)
      {
         ossimString bandPrefix = "band" + ossimString::toString(i) + ".";
         ossimString prefixStr = imagePrefix + bandPrefix;
         for (ossim_uint32 j = 0; j < subMeta.size(); j++)
         {
            out << prefixStr << subMeta[j] << "\n";
         }
      }
      index++;
      it++;
   }

   if (traceDebug())
   {    
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " exited...\n";
   }

   return out;
}


