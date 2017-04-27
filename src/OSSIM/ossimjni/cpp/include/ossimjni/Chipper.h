//-----------------------------------------------------------------------------
// File:  Chipper.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper class for ossimChipper with swig readable interfaces.
//
//-----------------------------------------------------------------------------
// $Id: Chipper.h 22320 2013-07-19 15:21:03Z dburken $

#ifndef ossimjniChipper_HEADER
#define ossimjniChipper_HEADER 1

#include <ossimjni/Constants.h>

class ossimChipperUtil;

namespace ossimjni
{
   class ImageData;
   class Keywordlist;
   
   class OSSIMJNIDLL Chipper
   {
   public:

      /** @brief default constructor. */
      Chipper();
      
      /** @brief destructor */
      ~Chipper();

      /**
       * @brief Initial method.
       *
       * Typically called from application prior to execute.  This parses
       * all options and put in keyword list m_kwl.
       * 
       * @param ap Arg parser to initialize from.
       * @return true, indicating process should continue with execute.
       * @note A throw with an error message of "usage" is used to get out when
       * a usage is printed.
       */
      bool initialize(int argc, char* argv[]);

      /**
       * @brief Initial method.
       *
       * Typically called from application prior to execute.  This parses
       * all options and put in keyword list m_kwl.
       * 
       * @param ap Arg parser to initialize from.
       * @return true, indicating process should continue with execute.
       * @note A throw with an error message of "usage" is used to get out when
       * a usage is printed.
       */
      bool initialize( const Keywordlist& kwl );

      /**
       * @brief execute method.
       *
       * Performs the actual dump of information.  This executes any options
       * set including image operations, i.e. -i -p --dno and so on.
       * 
       * @note Throws ossimException on error.
       */
      bool execute();

      /**
       * @brief Gets initialized area of interest(aoi) from chain.
       * @return Pointer to ossimImageData holding chip.  Pointer
       * can be null if not initialized properly so caller should
       * check.
       *
       * @return Status of tile data. This maps to ossimDataObjectStatus
       * from ossim library:
       * OSSIM_STATUS_UNKNOWN = 0,
       * OSSIM_NULL           = 1, not initialized
       * OSSIM_EMPTY          = 2, initialized but blank or empty
       * OSSIM_PARTIAL        = 3, contains some null/invalid values
       * OSSIM_FULL           = 4  all valid data
       */
      ossimjni_int32 getChip( ImageData* imageData );

      /**
       * @brief Initializes data with area of interest(aoi) from chain. If
       * alpha flag is true the alpha channel is computed and added. Has
       * has coded interleave by pixel or BIP.
       * 
       * @param data Buffer to initialize.   Assumed "data" to be large
       * enough to hold aoi and alpha channel if turned on.
       *
       * @param alpha If true the alpha channel is computed and added.
       *
       * @return Status of tile data. This maps to ossimDataObjectStatus
       * from ossim library:
       * OSSIM_STATUS_UNKNOWN = 0,
       * OSSIM_NULL           = 1, not initialized
       * OSSIM_EMPTY          = 2, initialized but blank or empty
       * OSSIM_PARTIAL        = 3, contains some null/invalid values
       * OSSIM_FULL           = 4  all valid data
       */
      ossimjni_int32 getChip( ossimjni_int8* data, bool alpha );

   private:

      /** @brief Hidden from use copy constructor. */
      Chipper( const Chipper& obj );
      
      /** @brief Hidden from use assignment operator. */
      const Chipper& operator=( const Chipper& rhs );
      
      ossimChipperUtil* m_chipper;
   };

} // End of namespace ossimjni.

#endif /* #ifndef ossimjniChipper_HEADER */
