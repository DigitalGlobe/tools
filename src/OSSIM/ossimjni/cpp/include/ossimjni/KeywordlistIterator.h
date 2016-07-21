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
// $Id: KeywordlistIterator.h 22320 2013-07-19 15:21:03Z dburken $

#ifndef ossimjniKeywordlistIterator_HEADER
#define ossimjniKeywordlistIterator_HEADER 1

#include <ossimjni/Constants.h>
#include <map>
#include <string>

namespace ossimjni
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
   class OSSIMJNIDLL KeywordlistIterator
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
      KeywordlistIterator(ossimjni::Keywordlist* kwl);

      /**
       * @brief copy constructor
       * @param iter KeywordlistIterator to initialize from.
       */
      KeywordlistIterator(const ossimjni::KeywordlistIterator& iter);

      /**
       * @brief Assignment operator.
       * @param iter KeywordlistIterator to assign from.
       */
      const KeywordlistIterator& operator=(
         const ossimjni::KeywordlistIterator& iter );

      /** @brief destructor */
      ~KeywordlistIterator();

      /**
       * @brief Initialize method.
       * @param kwl Our Keywordlist.
       */
      void initialize(ossimjni::Keywordlist* kwl);

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
      void getKeyValue(ossimjni::StringPair* pair) const;
      
   private:
      
      Keywordlist*            m_kwl;
      StringPairMap::iterator m_iter;
   };

} // End of namespace ossimjni.

#endif /* #ifndef ossimjniKeywordlist_HEADER */
