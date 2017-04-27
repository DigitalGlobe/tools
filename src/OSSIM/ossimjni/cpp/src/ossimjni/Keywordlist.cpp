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

#include <ossimjni/Keywordlist.h>
#include <ossim/base/ossimKeywordlist.h>

ossimjni::Keywordlist::Keywordlist() :
   m_kwl( new ossimKeywordlist() )
{
}

ossimjni::Keywordlist::Keywordlist(  const ossimjni::Keywordlist& obj ) :
   m_kwl( new ossimKeywordlist() )
{
   if ( obj.getConstKeywordlist() )
   {
      m_kwl->addList( *(obj.getConstKeywordlist()), true );
   }
}

const ossimjni::Keywordlist& ossimjni::Keywordlist::operator=( const ossimjni::Keywordlist& rhs )
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

ossimjni::Keywordlist::~Keywordlist()
{
   if (m_kwl)
   {
      delete m_kwl;
      m_kwl = 0;
   }
}

const ossimKeywordlist* ossimjni::Keywordlist::getConstKeywordlist() const
{
   return m_kwl;
}

ossimKeywordlist* ossimjni::Keywordlist::getKeywordlist()
{
   return m_kwl;
}

ossimjni::KeywordlistIterator ossimjni::Keywordlist::getIterator()
{
   return KeywordlistIterator(this);
}

bool ossimjni::Keywordlist::valid() const
{
   return m_kwl ? true : false;
}

int ossimjni::Keywordlist::size() const
{
   int result = 0;
   if ( m_kwl )
   {
      result = static_cast<int>( m_kwl->getSize() );
   }
   return result;
}

void ossimjni::Keywordlist::addPair(const std::string& key,
                                    const std::string& value)
{
   if ( m_kwl )
   {
      m_kwl->addPair( key, value );
   }
}

std::string ossimjni::Keywordlist::findKey( const std::string& key ) const
{
   std::string result;
   if ( m_kwl )
   {
      result = m_kwl->findKey(key);
   }
   return result;
}

std::string ossimjni::Keywordlist::findKey( const std::string& prefix,
                                            const std::string& key ) const
{
   std::string result;
   {
      result = m_kwl->findKey(prefix, key);
   }
   return result;
}

void ossimjni::Keywordlist::clearMap()
{
   if ( m_kwl )
   {
      m_kwl->clear();
   }
}

