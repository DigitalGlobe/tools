//-----------------------------------------------------------------------------
// File:  KeywordlistIterator.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: SWIG readable iterator for Keywordlist wrapper class.
//
//-----------------------------------------------------------------------------
// $Id$

#ifndef omsKeywordlistIterator_HEADER
#define omsKeywordlistIterator_HEADER 1

#include <oms/Constants.h>
#include <map>
#include <string>

namespace OpenThreads
{
   class Mutex;
}

namespace oms
{
   class Keywordlist;
   class StringPair;
   
   /**
    * @class KeywordlistIterator
    *
    * SWIG readable iterator for Keywordlist wrapper class.  Used to iterate
    * through the underlying std::map<std::string, std::string>.
    *
    * See org.ossim.jni.test.InfoTest.java for example of usage.
    */
   class OMSDLL KeywordlistIterator
   {
   public:

      typedef std::map<std::string, std::string> StringPairMap;

      /**
       * @brief default constructor
       * un-initialized...
       */
      KeywordlistIterator();

      /**
       * @brief Constructor
       * Does not own memory.
       */
      KeywordlistIterator(oms::Keywordlist* kwl);

      /**
       * @brief copy constructor
       * @param iter KeywordlistIterator to initialize from.
       */
      KeywordlistIterator(const oms::KeywordlistIterator& iter);

      /**
       * @brief Assignment operator.
       * @param iter KeywordlistIterator to assign from.
       */
      const KeywordlistIterator& operator=(
         const oms::KeywordlistIterator& iter );

      /** @brief destructor */
      ~KeywordlistIterator();

      /**
       * @brief Initialize method.
       * @param kwl Our Keywordlist.
       */
      void initialize(oms::Keywordlist* kwl);

      /** @return Moves iterartor to beginning. */
      void reset();

      /** @brief Increments iterator. */
      void next();

      /** @return true when iterator is at end. */
      bool end() const;

      /** @return Key for current iterator. */
      std::string getKey() const;

      /** @return Value for current iterator. */
      std::string getValue() const;

      /**
       * @brief Get string pair with key and value for current iterator.
       * @param pair Initialized by this call.
       * @note pointer passed in must be valid.*/ 
      void getKeyValue(oms::StringPair* pair) const;
      
   private:
      
      Keywordlist*            m_kwl;
      StringPairMap::iterator m_iter;
      OpenThreads::Mutex*      m_mutex;
   };

} // End of namespace oms.

#endif /* #ifndef omsKeywordlist_HEADER */
