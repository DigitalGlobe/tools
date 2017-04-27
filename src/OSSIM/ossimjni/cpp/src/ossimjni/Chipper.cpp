//-----------------------------------------------------------------------------
// File:  Chipper.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for Chipper.
//
//-----------------------------------------------------------------------------
// $Id: Chipper.cpp 22320 2013-07-19 15:21:03Z dburken $

#include <ossimjni/Chipper.h>
#include <ossimjni/ImageData.h>
#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/util/ossimChipperUtil.h>

ossimjni::Chipper::Chipper() :
   m_chipper( new ossimChipperUtil )
{
}

// Private/hidden from use.
ossimjni::Chipper::Chipper( const Chipper& /* obj */ )
{
}

// Private/hidden from use.
const ossimjni::Chipper& ossimjni::Chipper::operator=(
   const ossimjni::Chipper& /* rhs */)
{
   return *this;
}
      
ossimjni::Chipper::~Chipper()
{
   if (m_chipper)
   {
      delete m_chipper;
      m_chipper = 0;
   }
}

bool ossimjni::Chipper::initialize(int argc, char* argv[])
{
   bool result = false;
   try
   {
      ossimArgumentParser ap(&argc, argv);
      result = m_chipper->initialize(ap);
   }
   catch ( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Chipper::initialize caught exception: " << e.what() << std::endl;
      result = false;
   }
   return result;
}

bool ossimjni::Chipper::initialize( const ossimjni::Keywordlist& kwl )
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
      ossimNotify(ossimNotifyLevel_WARN)
         << "Chipper::initialize caught exception: " << e.what() << std::endl;
      result = false;
   }
   return result;
}

bool ossimjni::Chipper::execute()
{
   bool result = true;
   try
   {
      m_chipper->execute();
   }
   catch ( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Chipper::initialize caught exception: " << e.what() << std::endl;
      result = false;
   }
   return result;  
}

ossimjni_int32 ossimjni::Chipper::getChip( ImageData* imageData )
{
   ossimjni_int32 status = OSSIM_STATUS_UNKNOWN;
   try
   {   
      if ( imageData )
      {
         ossimRefPtr<ossimImageData> chip = m_chipper->getChip();
         if ( chip.valid() )
         {
            // Chip status for return:
            status = chip->getDataObjectStatus();
            
            //---
            // Must call ossimRefPtr::release so chip does not self destruct
            // when it goes out of scope.
            //---
            imageData->setImageData( chip.release() );
         }
         else
         {
            imageData->setImageData( (ossimImageData*)0 );
         }
      }
   }
   catch ( const ossimException& e )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Chipper::getChip caught exception: " << e.what() << std::endl;
   }
   return status;
}

ossimjni_int32 ossimjni::Chipper::getChip( ossimjni_int8* data, bool alpha )
{
   ossimjni_int32 status = OSSIM_STATUS_UNKNOWN;
   try
   {   
      if ( data )
      {
         ossimRefPtr<ossimImageData> chip = m_chipper->getChip();
         if ( chip.valid() )
         {
            // Chip status for return:
            status = chip->getDataObjectStatus();

            ossimIrect rect = chip->getImageRectangle();
            if ( rect.hasNans() == false )
            {
               if ( status != (ossimjni_int32)OSSIM_NULL )
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
      ossimNotify(ossimNotifyLevel_WARN)
         << "Chipper::getChip caught exception: " << e.what() << std::endl;
   }
   return status;
}
