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

#include <oms/Info.h>
#include <oms/Keywordlist.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/util/ossimInfo.h>

oms::Info::Info() :
   m_info(new ossimInfo)
{
}
      
oms::Info::~Info()
{
   if (m_info)
   {
      delete m_info;
      m_info = 0;
   }
}

bool oms::Info::initialize(int argc, char* argv[])
{
   bool result = false;
   try
   {
      ossimArgumentParser ap(&argc, argv);
      result = m_info->initialize(ap);
   }
   catch ( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "omsInfo::initialize caught exception: " << e.what() << std::endl;
      result = false;
   }
   catch( ... )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "omsInfo::initialize caught exception!" << std::endl;
      result = false;
   }
   return result;
}

void oms::Info::execute()
{
   try
   {
      m_info->execute();
   }
   catch ( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "oms::Info::execute caught exception: " << e.what() << std::endl;
   }
   catch( ... )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "omsInfo::execute caught exception!" << std::endl;
   }
}

void oms::Info::getImageInfo( const std::string& file,
                              bool dumpFlag,
                              bool dnoFlag,
                              bool imageGeomFlag,
                              bool imageInfoFlag,
                              bool metaDataFlag,
                              bool paletteFlag,
                              oms::Keywordlist* kwl) const
{
   if ( kwl )
   {
      try
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
      catch ( const ossimException& e )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::Info::getImageInfo caught exception: " << e.what() << std::endl;
      }
      catch ( ... )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::Info::getImageInfo caught exception!" << std::endl;
      }
   }
}

bool oms::Info::getImageInfo( const std::string& file,
                              int entry,
                              oms::Keywordlist* kwl) const
{
   bool result = false;
   
   if ( kwl )
   {
      ossim_uint32 entryIndex = 0;
      if ( entry > -1 )
      {
         entryIndex = (ossim_uint32)entry;
      }

      try
      {
         result = m_info->getImageInfo( ossimFilename(file),
                                        entryIndex,
                                        *(kwl->getKeywordlist()) );
      }
      catch( const ossimException& e )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::Info::getImageInfo caught exception: " << e.what() << std::endl;
         result = false;
      }
      catch( ... )
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "oms::Info::getImageInfo caught exception!" << std::endl;
         result = false;
      }
   }

   return result;
}

void oms::Info::closeImage()
{
   try
   {
      m_info->closeImage();
   }
   catch( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "oms::Info::closeImage caught exception: " << e.what() << std::endl;
   }
   catch( ... )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "oms::Info::closeImage caught exception!" << std::endl;
   }
}

std::string oms::Info::getOssimBuildDate() const
{
   std::string s;
   m_info->getBuildDate( s );
   return s;
}

std::string oms::Info::getOssimRevisionNumber() const
{
   std::string s;
   m_info->getRevisionNumber( s );
   return s;
}

std::string oms::Info::getOssimVersion() const
{
   std::string s;
   m_info->getVersion( s );
   return s;
}

