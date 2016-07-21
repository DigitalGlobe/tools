//-----------------------------------------------------------------------------
// File:  Keywordlist.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper for ossimKeywordlist.
//
//-----------------------------------------------------------------------------
// $Id: Keywordlist.h 22359 2013-08-06 23:11:43Z dburken $

#ifndef omsKeywordlist_HEADER
#define omsKeywordlist_HEADER 1

#include <oms/Constants.h>
#include <oms/KeywordlistIterator.h>
#include <oms/StringPair.h>

class ossimKeywordlist;

namespace oms
{
   /**
    * @class Keywordlist
    *
    * Wrapper for ossimKeywordlist.
    */
   class OMSDLL Keywordlist
   {
   public:
      /** @brief default constructor */
      Keywordlist();
      
      /** @brief destructor */
      ~Keywordlist();

      /**
       * @brief copy constructor
       * @param obj Copy this
       */
      Keywordlist( const Keywordlist& obj );

      /**
       * @brief Assignment operator.
       * @param pair StringPair to assign from.
       */
      const Keywordlist& operator=( const Keywordlist& rhs );

      /**
       * @retrurn a const pointer to the ossimKeywordlist.
       * @note getKeywordlist not overloaded for swig warning.
       */
      const ossimKeywordlist* getConstKeywordlist() const;

      /** @retrurn a pointer to the ossimKeywordlist. */
      ossimKeywordlist* getKeywordlist();

      /** @retrurn a const pointer to the ossimKeywordlist. */
      const ossimKeywordlist* getKeywordlist() const;

      /** @return An iterator for this. */
      oms::KeywordlistIterator getIterator();

      /** @return true if m_kwl is initialized, false if not. */
      bool valid() const;

      /** @return Size of map. */
      int size() const;

      /**
       * @brief Adds key, value to keyword list.
       * @param key
       * @param valud
       */
      void addPair(const std::string& key,
                   const std::string& value);

      /*!
       *  @brief Find method that takes a key and intializes value.
       *  @param key Key to find.  e.g.: "tiff.image0.samples_per_pixel"
       *  @return value
       */
      std::string findKey( const std::string& key ) const;
      
      /*!
       *  @brief Find method that takes a prefix and key and intializes value.
       *  @param prefix e.g.: "tiff.image0."
       *  @param key e.g.:  "samples_per_pixel"
       *  @return value
       */
      std::string findKey( const std::string& prefix,
                           const std::string& key ) const;
      
      /**
       * @brief Calls ossimKeywordlist::clear method in turn clears the 
       * std::map<std::string, std::string>
       */
      void clearMap();

   private:
      ossimKeywordlist* m_kwl;
   };

} // End of namespace oms.

#endif /* #ifndef omsKeywordlist_HEADER */
