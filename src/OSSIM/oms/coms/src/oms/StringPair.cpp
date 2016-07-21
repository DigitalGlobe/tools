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
// $Id$

#include <oms/StringPair.h>

oms::StringPair::StringPair()
   :
   m_key(),
   m_val()
{
}

oms::StringPair::StringPair( const StringPair& pair )
   :
   m_key( pair.m_key ),
   m_val( pair.m_val )
{
}

const oms::StringPair& oms::StringPair::operator=( const StringPair& rhs )
{
   if ( this != &rhs )
   {
      m_key = rhs.m_key;
      m_val = rhs.m_val;
   }
   return *this;
}

void oms::StringPair::clear()
{
   m_key.clear();
   m_val.clear();
}

std::string oms::StringPair::getKey() const
{
   return m_key;
}

std::string oms::StringPair::getValue() const
{
   return m_val;
}

void oms::StringPair::setKey(const std::string& key)
{
   m_key = key;
}

void oms::StringPair::setValue(const std::string& value)
{
   m_val = value;
}
