//-----------------------------------------------------------------------------
// File:  KeywordlistIterator.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimKeywordlistIterator.
//
//-----------------------------------------------------------------------------
// $Id: KeywordlistIterator.cpp 22121 2013-01-26 16:34:32Z dburken $

#include <oms/KeywordlistIterator.h>
#include <oms/Keywordlist.h>
#include <oms/StringPair.h>
#include <ossim/base/ossimKeywordlist.h>
#include <OpenThreads/Mutex>

oms::KeywordlistIterator::KeywordlistIterator()
   :
   m_kwl(0),
   m_iter(),
   m_mutex(new OpenThreads::Mutex())
{
}

oms::KeywordlistIterator::KeywordlistIterator(const oms::KeywordlistIterator& iter)
   :
   m_kwl(iter.m_kwl),
   m_iter(iter.m_iter),
   m_mutex(new OpenThreads::Mutex())
{
}

oms::KeywordlistIterator::KeywordlistIterator(oms::Keywordlist* kwl)
   :
   m_kwl(kwl),
   m_iter(),
   m_mutex(new OpenThreads::Mutex())
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_mutex->lock();
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
      m_mutex->unlock();
   }
}

oms::KeywordlistIterator::~KeywordlistIterator()
{
   delete m_mutex;
   m_mutex = 0;
}

const oms::KeywordlistIterator& oms::KeywordlistIterator::operator=(
   const oms::KeywordlistIterator& iter )
{
   if ( this != &iter )
   {
      m_mutex->lock();
      m_kwl  = iter.m_kwl;
      m_iter = iter.m_iter;
      m_mutex->unlock();
   }
   return *this;
}

void oms::KeywordlistIterator::initialize(oms::Keywordlist* kwl)
{
   m_mutex->lock();
   m_kwl = kwl;
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
   m_mutex->unlock();
}

void oms::KeywordlistIterator::reset()
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_mutex->lock();
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
      m_mutex->unlock();
   }
}

bool oms::KeywordlistIterator::end() const
{
   bool result = false;
   m_mutex->lock();

   if ( m_kwl  && m_kwl->valid() )
   {
      result = (m_iter == m_kwl->getKeywordlist()->getMap().end());
   }
   m_mutex->unlock();

   return result;
}

void oms::KeywordlistIterator::next()
{
   if ( !end() )
   {
      m_mutex->lock();
      ++m_iter;
      m_mutex->unlock();
   }
}

std::string oms::KeywordlistIterator::getKey() const
{
   std::string result;

   m_mutex->lock();
   result= (*m_iter).first;
   m_mutex->unlock();

   return  result;
}

std::string oms::KeywordlistIterator::getValue() const
{
   std::string result;

   m_mutex->lock();
   result= (*m_iter).second;
   m_mutex->unlock();

   return result;
}

void oms::KeywordlistIterator::getKeyValue( oms::StringPair* pair ) const
{
   m_mutex->lock();

   if ( pair )
   {
      pair->setKey( (*m_iter).first );
      pair->setValue( (*m_iter).second );
   }

   m_mutex->unlock();

}
