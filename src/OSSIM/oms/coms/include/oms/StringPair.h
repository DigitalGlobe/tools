//-----------------------------------------------------------------------------
// File:  StringPair.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Container for a pair of std::strings. Data members public.
//
//-----------------------------------------------------------------------------
// $Id: StringPair.h 20264 2011-11-17 22:57:15Z dburken $

#ifndef omsStringPair_HEADER
#define omsStringPair_HEADER 1

#include <oms/Constants.h>
#include <string>

namespace oms
{
   /**
    * @class StringPair
    *
    * Container for a pair of std::strings. Data members public.
    */
   class OMSDLL StringPair
   {
   public:
      /** @brief default constructor */
      StringPair();

      /**
       * @brief copy constructor
       * @param pair Copy this
       */
      StringPair( const StringPair& pair );

      /**
       * @brief Assignment operator.
       * @param pair StringPair to assign from.
       */
      const StringPair& operator=( const StringPair& rhs );

      /** @brief Call std::string::clear on key and value.*/
      void clear();

      /**
       * @brief Gets key.
       * @return Key
       */
      std::string getKey()   const;

      /**
       * @brief Gets value.
       * @return Value
       */
      std::string getValue() const;

      /**
       * @brief Sets key.
       * @param key
       */
      void setKey(const std::string& key);

      /**
       * @brief Sets value.
       * @param value
       */
      void setValue(const std::string& value);

   private:
      
      std::string m_key;
      std::string m_val;
   };

} // End of namespace oms.

#endif /* #ifndef omsStringPair_HEADER */
