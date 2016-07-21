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
// $Id: KeywordlistIterator.cpp 22320 2013-07-19 15:21:03Z dburken $

#include <ossimjni/KeywordlistIterator.h>
#include <ossimjni/Keywordlist.h>
#include <ossimjni/StringPair.h>
#include <ossim/base/ossimKeywordlist.h>

ossimjni::KeywordlistIterator::KeywordlistIterator()
   :
   m_kwl(0),
   m_iter()
{
}

ossimjni::KeywordlistIterator::KeywordlistIterator(const ossimjni::KeywordlistIterator& iter)
   :
   m_kwl(iter.m_kwl),
   m_iter(iter.m_iter)
{
}

ossimjni::KeywordlistIterator::KeywordlistIterator(ossimjni::Keywordlist* kwl)
   :
   m_kwl(kwl),
   m_iter()
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
}

ossimjni::KeywordlistIterator::~KeywordlistIterator()
{
}

const ossimjni::KeywordlistIterator& ossimjni::KeywordlistIterator::operator=(
   const ossimjni::KeywordlistIterator& iter )
{
   if ( this != &iter )
   {
      m_kwl         = iter.m_kwl;
      m_iter        = iter.m_iter;
   }
   return *this;
}

void ossimjni::KeywordlistIterator::initialize(ossimjni::Keywordlist* kwl)
{
   m_kwl = kwl;
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
}

void ossimjni::KeywordlistIterator::reset()
{
   if ( m_kwl && m_kwl->valid() )
   {
      m_iter = m_kwl->getKeywordlist()->getMap().begin();
   }
}

bool ossimjni::KeywordlistIterator::end() const
{
   bool result = false;
   if ( m_kwl  && m_kwl->valid() )
   {
      result = (m_iter == m_kwl->getKeywordlist()->getMap().end());
   }
   return result;
}

void ossimjni::KeywordlistIterator::next()
{
   if ( !end() )
   {
      ++m_iter;
   }
}

std::string ossimjni::KeywordlistIterator::getKey() const
{
   return (*m_iter).first;
}

std::string ossimjni::KeywordlistIterator::getValue() const
{
   return (*m_iter).second;
}

void ossimjni::KeywordlistIterator::getKeyValue( ossimjni::StringPair* pair ) const
{
   if ( pair )
   {
      pair->setKey( (*m_iter).first );
      pair->setValue( (*m_iter).second );
   }
}
