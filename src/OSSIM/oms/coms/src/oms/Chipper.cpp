//-----------------------------------------------------------------------------
// File:  Chipper.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for Chipper. Wrapper class for
// ossimChipperUtil with swig readable interfaces.
//
//-----------------------------------------------------------------------------
// $Id$

#include <oms/Chipper.h>
#include <oms/Keywordlist.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/util/ossimChipperUtil.h>
#include <ossim/base/ossimTrace.h>
static ossimTrace traceDebug("oms::Chipper:debug");

oms::Chipper::Chipper() :
   m_chipper( new ossimChipperUtil )
{
}

// Private/hidden from use.
oms::Chipper::Chipper( const Chipper& /* obj */ )
{
}

// Private/hidden from use.
const oms::Chipper& oms::Chipper::operator=(
   const oms::Chipper& /* rhs */)
{
   return *this;
}
      
oms::Chipper::~Chipper()
{
   if (m_chipper)
   {
      delete m_chipper;
      m_chipper = 0;
   }
}

bool oms::Chipper::initialize(int argc, char* argv[])
{
   bool result = false;
   try
   {
      ossimArgumentParser ap(&argc, argv);
      result = m_chipper->initialize(ap);
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
   }
   catch ( ... )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception!" << std::endl;
      }
   }
   return result;
}

bool oms::Chipper::initialize( const oms::Keywordlist& kwl )
{
   bool result = true;
   try
   {
      if ( kwl.getConstKeywordlist() )
      {
         m_chipper->initialize( *(kwl.getConstKeywordlist()) );
      }
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
      result = false;
   }
   catch ( ... )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception!" << std::endl;
      }
      result = false;
   }
   return result;
}

bool oms::Chipper::initialize( const std::map<std::string, std::string>& map )
{
   bool result = true;
   try
   {
      Keywordlist kwl;
      if ( kwl.getKeywordlist() )
      {
         kwl.getKeywordlist()->getMap() = map;
         m_chipper->initialize( *(kwl.getConstKeywordlist()) );
      }
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
      result = false;
   }
   catch ( ... )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception!" << std::endl;
      }
      result = false;
   }
   return result;
}

bool oms::Chipper::execute()
{
   bool result = true;
   try
   {
      m_chipper->execute();
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
      result = false;
   }
   catch ( ... )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception!" << std::endl;
      }
      result = false;
   }
   return result;  
}

void oms::Chipper::abort()
{
   if(m_chipper)
   {
      m_chipper->abort();
   }
}

int oms::Chipper::getChip( ossim_int8* data, bool alpha,const std::map<std::string,std::string>& options)
{
   int status = OSSIM_STATUS_UNKNOWN;
   try
   {   
      if ( data )
      {
         ossimRefPtr<ossimImageData> chip = m_chipper->getChip(options);
         if ( chip.valid() )
         {
            status = chip->getDataObjectStatus();
            ossimIrect rect = chip->getImageRectangle();
            if ( rect.hasNans() == false )
            {
               if ( status != (int)OSSIM_NULL )
               {
                  if ( alpha )
                  {
                     chip->computeAlphaChannel();
                     chip->unloadTileToBipAlpha( (void*)data, rect, rect );
                  }
                  else
                  {
                     chip->unloadTile( (void*)data, rect, OSSIM_BIP );
                  }
               }
            }
         }
      }
   }
   catch ( const ossimException& e )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception: " << e.what() << std::endl;
      }
   }
   catch ( ... )
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "Chipper::initialize caught exception!" << std::endl;
      }
   }
   return status;
}
