//-----------------------------------------------------------------------------
// File:  Keywordlist.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimKeywordlist.
//
//-----------------------------------------------------------------------------
// $Id: Keywordlist.cpp 22359 2013-08-06 23:11:43Z dburken $

#include <oms/Keywordlist.h>
#include <ossim/base/ossimKeywordlist.h>

oms::Keywordlist::Keywordlist() :
   m_kwl( new ossimKeywordlist() )
{
}

oms::Keywordlist::Keywordlist(  const oms::Keywordlist& obj ) :
   m_kwl( new ossimKeywordlist() )
{
   if ( obj.getConstKeywordlist() )
   {
      m_kwl->addList( *(obj.getConstKeywordlist()), true );
   }
}

const oms::Keywordlist& oms::Keywordlist::operator=( const oms::Keywordlist& rhs )
{
   if ( this != &rhs )
   {
      m_kwl->clear();

      if ( rhs.getConstKeywordlist() )
      {
         m_kwl->addList( *(rhs.getConstKeywordlist()), true );
      }
   }
   return *this;
}

oms::Keywordlist::~Keywordlist()
{
   if (m_kwl)
   {
      delete m_kwl;
      m_kwl = 0;
   }
}

const ossimKeywordlist* oms::Keywordlist::getConstKeywordlist() const
{
   return m_kwl;
}

ossimKeywordlist* oms::Keywordlist::getKeywordlist()
{
   return m_kwl;
}

const ossimKeywordlist* oms::Keywordlist::getKeywordlist() const
{
   return m_kwl;
}

oms::KeywordlistIterator oms::Keywordlist::getIterator()
{
   return KeywordlistIterator(this);
}

bool oms::Keywordlist::valid() const
{
   return m_kwl ? true : false;
}

int oms::Keywordlist::size() const
{
   int result = 0;
   if ( m_kwl )
   {
      result = static_cast<int>( m_kwl->getSize() );
   }
   return result;
}

void oms::Keywordlist::addPair(const std::string& key,
                               const std::string& value)
{
   if ( m_kwl )
   {
      m_kwl->addPair( key, value );
   }
}

std::string oms::Keywordlist::findKey( const std::string& key ) const
{
   std::string result;
   if ( m_kwl )
   {
      result = m_kwl->findKey(key);
   }
   return result;
}

std::string oms::Keywordlist::findKey( const std::string& prefix,
                                       const std::string& key ) const
{
   std::string result;
   {
      result = m_kwl->findKey(prefix, key);
   }
   return result;
}

void oms::Keywordlist::clearMap()
{
   if ( m_kwl )
   {
      m_kwl->clear();
   }
}

