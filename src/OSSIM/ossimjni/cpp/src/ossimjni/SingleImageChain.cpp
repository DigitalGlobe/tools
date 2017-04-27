//-----------------------------------------------------------------------------
// File:  SingleImageChain.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for SingleImageChain.
//
//-----------------------------------------------------------------------------
// $Id: SingleImageChain.cpp 22121 2013-01-26 16:34:32Z dburken $

#include <ossimjni/SingleImageChain.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimSingleImageChain.h>

ossimjni::SingleImageChain::SingleImageChain() :
   m_img(new ossimSingleImageChain)
{
}

ossimjni::SingleImageChain::~SingleImageChain()
{
   if (m_img)
   {
      delete m_img;
      m_img = 0;
   }
}

bool ossimjni::SingleImageChain::open(const std::string& file)
{
   return m_img->open(ossimFilename(file));
}

bool ossimjni::SingleImageChain::open(const std::string& file, ossimjni_int32 entry)
{
   bool result = false;
   if ( m_img->open(ossimFilename(file)) )
   {
      ossimRefPtr<ossimImageHandler> ih = m_img->getImageHandler();
      if ( ih.valid() )
      {
         result = ih->setCurrentEntry( static_cast<ossim_uint32>(entry) );
      }
   }
   return result;
}

bool ossimjni::SingleImageChain::isOpen() const
{
   return m_img->isOpen();
}

void ossimjni::SingleImageChain::close()
{
   m_img->close();
}

std::vector<ossimjni_int32> ossimjni::SingleImageChain::getEntryList() const
{
   std::vector<ossimjni_int32> result(0);
   ossimRefPtr<ossimImageHandler> ih = m_img->getImageHandler();
   if ( ih.valid() )
   {
      std::vector<ossim_uint32> list;
      ih->getEntryList(list);
      std::vector<ossim_uint32>::const_iterator i = list.begin();
      while ( i != list.end() )
      {
         result.push_back(static_cast<ossimjni_int32>(*i));
         ++i;
      }
   }
   return result;
}

std::string ossimjni::SingleImageChain::getFilename() const
{
   std::string result = m_img->getFilename().string();
   return result;
}

void ossimjni::SingleImageChain::createRenderedChain()
{
   m_img->createRenderedChain();
}

bool ossimjni::SingleImageChain::selectBands(const std::vector<ossimjni_uint32>& bands)
{
   bool result = verifyBandRange(bands);
   if ( result )
   {
      const ossim_uint32 IMG_BANDS = getNumberOfBands();
      if (IMG_BANDS > 1)
      {
         // This will add a band selector to chain if not there yet.
         m_img->setBandSelection(bands);
      }
   }
   return result;
}

std::vector<ossimjni_uint32> ossimjni::SingleImageChain::getBandSelection() const
{
   std::vector<ossimjni_uint32> result(0);
   const ossim_uint32 IMG_BANDS = getNumberOfBands();
   if ( IMG_BANDS )
   {
      if ( IMG_BANDS == 1 )
      {
         result.push_back(1);
      }
      else
      {
         ossimRefPtr<ossimBandSelector> bs = m_img->getBandSelector();
         if ( bs.valid() == false )
         {
            m_img->addBandSelector();
            bs =  m_img->getBandSelector();
         }
         if ( bs.valid() )
         {
            bs->getOutputBandList(result);
         }
      }
   }
   return result;
}

ossimjni_uint32 ossimjni::SingleImageChain::getNumberOfBands() const
{
   ossimjni_uint32 result = 0;
   if ( isOpen() )
   {
      ossimRefPtr<ossimImageHandler> ih = m_img->getImageHandler();
      if ( ih.valid() )
      {
         result = ih->getNumberOfOutputBands();
      }
   }
   return result;
}

bool ossimjni::SingleImageChain::setHistogram(const std::string& his)
{
   bool result = false;
   if ( isOpen() )
   {
      ossimRefPtr<ossimHistogramRemapper> hr = m_img->getHistogramRemapper();
      if ( hr.valid() == false )
      {
         //---
         // If the chain is not created check for the need for a band selector as
         // it should be in front of the histogram in the chain in case bands are
         // filtered out.
         //---
         const ossim_uint32 IMG_BANDS = getNumberOfBands();
         if ( IMG_BANDS > 1 )
         {
            ossimRefPtr<ossimBandSelector> bs = m_img->getBandSelector();
            if ( bs.valid() == false )
            {
               m_img->addBandSelector();
            }
         }
         m_img->addHistogramRemapper();
         hr = m_img->getHistogramRemapper();
      }
      if ( hr.valid() )
      {
         result = hr->openHistogram( ossimFilename(his) );
      }
   }
   return result;
}

std::string ossimjni::SingleImageChain::getHistogramFile() const
{
   std::string result;
   ossimRefPtr<ossimHistogramRemapper> hr = m_img->getHistogramRemapper();
   if ( hr.valid() )
   {
      result = hr->getHistogramFile().c_str();
   }
   return result;
}

bool ossimjni::SingleImageChain::setOverview(const std::string& ovr)
{
   bool result = false;
   if ( isOpen() )
   {
      ossimRefPtr<ossimImageHandler> ih = m_img->getImageHandler();
      if ( ih.valid() )
      {
         ossimFilename f(ovr);
         ossimRefPtr<const ossimImageHandler> ovrIh = ih->getOverview();
         if ( ovrIh.valid() )
         {
            ossimFilename currentOvrFilename = ovrIh->getFilename();
            if ( ( f == currentOvrFilename) || (f.expand() == currentOvrFilename) )
            {
               result = true; // Already has open.
            }
         }
         if ( !result )
         {
            result = ih->openOverview(f);
         }
      }
   }
   return result;
}

std::string ossimjni::SingleImageChain::getOverviewFile() const
{
   std::string result;
   if ( isOpen() )
   {
      ossimRefPtr<ossimImageHandler> ih = m_img->getImageHandler();
      if ( ih.valid() )
      {
         ossimRefPtr<const ossimImageHandler> ovrIh = ih->getOverview();
         if ( ovrIh.valid() )
         {
            result = ovrIh->getFilename().string();
         }
      }
   }
   return result;
}

bool ossimjni::SingleImageChain::verifyBandRange(const std::vector<ossimjni_uint32>& bands) const
{
   bool result = true;
   if ( isOpen() && bands.size() )
   {
      const ossim_uint32 IMG_BANDS = getNumberOfBands();
      std::vector<ossimjni_uint32>::const_iterator i = bands.begin();
      while ( i != bands.end() )
      {
         if ( *i >= IMG_BANDS )
         {
            result = false;
            break;
         }
         ++i;
      }
   }
   else
   {
      result = false; // not open or no bands.
   }
   return result;
}
