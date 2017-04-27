//-----------------------------------------------------------------------------
// File:  Info.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for Info.
//
//-----------------------------------------------------------------------------
// $Id: Info.cpp 23591 2015-10-21 13:14:26Z dburken $

#include <ossimjni/Info.h>
#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/util/ossimInfo.h>

ossimjni::Info::Info() :
   m_info(new ossimInfo)
{
}
      
ossimjni::Info::~Info()
{
   if (m_info)
   {
      delete m_info;
      m_info = 0;
   }
}

bool ossimjni::Info::initialize(int argc, char* argv[])
{
   ossimArgumentParser ap(&argc, argv);
   return m_info->initialize(ap);
}

void ossimjni::Info::execute()
{
   m_info->execute();
}

void ossimjni::Info::getImageInfo( const std::string& file,
                                   bool dumpFlag,
                                   bool dnoFlag,
                                   bool imageGeomFlag,
                                   bool imageInfoFlag,
                                   bool metaDataFlag,
                                   bool paletteFlag,
                                   ossimjni::Keywordlist* kwl) const
{
   if ( kwl )
   {
      m_info->getImageInfo(ossimFilename(file),
                           dumpFlag,
                           dnoFlag,
                           imageGeomFlag,
                           imageInfoFlag,
                           metaDataFlag,
                           paletteFlag,
                           *(kwl->getKeywordlist()) );
   }
}

bool ossimjni::Info::getImageInfo( const std::string& file,
                                   ossimjni_int32 entry,
                                   ossimjni::Keywordlist* kwl) const
{
   bool result = false;
   
   if ( kwl )
   {
      ossim_uint32 entryIndex = 0;
      if ( entry > -1 )
      {
         entryIndex = (ossim_uint32)entry;
      }

      result = m_info->getImageInfo( ossimFilename(file),
                                     entryIndex,
                                     *(kwl->getKeywordlist()) );
   }

   return result;
}

void ossimjni::Info::closeImage()
{
   m_info->closeImage();
}

std::string ossimjni::Info::getOssimBuildDate() const
{
   std::string s;
   m_info->getBuildDate( s );
   return s;
}

std::string ossimjni::Info::getOssimRevisionNumber() const
{
   std::string s;
   m_info->getRevisionNumber( s );
   return s;
}

std::string ossimjni::Info::getOssimVersion() const
{
   std::string s;
   m_info->getVersion( s );
   return s;
}
