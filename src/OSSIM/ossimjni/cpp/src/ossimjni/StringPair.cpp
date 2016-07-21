//-----------------------------------------------------------------------------
// File:  StringPair.cpp
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Class definition for StringPair.
//
//-----------------------------------------------------------------------------
// $Id: StringPair.cpp 20264 2011-11-17 22:57:15Z dburken $

#include <ossimjni/StringPair.h>

ossimjni::StringPair::StringPair()
   :
   m_key(),
   m_val()
{
}

ossimjni::StringPair::StringPair( const StringPair& pair )
   :
   m_key( pair.m_key ),
   m_val( pair.m_val )
{
}

const ossimjni::StringPair& ossimjni::StringPair::operator=( const StringPair& rhs )
{
   if ( this != &rhs )
   {
      m_key = rhs.m_key;
      m_val = rhs.m_val;
   }
   return *this;
}

void ossimjni::StringPair::clear()
{
   m_key.clear();
   m_val.clear();
}

std::string ossimjni::StringPair::getKey() const
{
   return m_key;
}

std::string ossimjni::StringPair::getValue() const
{
   return m_val;
}

void ossimjni::StringPair::setKey(const std::string& key)
{
   m_key = key;
}

void ossimjni::StringPair::setValue(const std::string& value)
{
   m_val = value;
}
