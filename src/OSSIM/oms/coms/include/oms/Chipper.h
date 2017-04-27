//-----------------------------------------------------------------------------
// File:  Chipper.h
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Wrapper class for ossimChipperUtil with swig readable
// interfaces.
//
//-----------------------------------------------------------------------------
// $Id$

#ifndef omsChipper_HEADER
#define omsChipper_HEADER 1

#include <oms/Constants.h>
#include <map>
#include <string>

class ossimChipperUtil;

namespace oms
{
   class Keywordlist;
   
   class OMSDLL Chipper
   {
   public:

      /** @brief default constructor. */
      Chipper();
      
      /** @brief destructor */
      ~Chipper();

      /**
       * @brief Initialize from command line args.
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
       * @brief Initialize from Keywordlist.
       * @param ap Arg parser to initialize from.
       * @return true, indicating process should continue with execute.
       */
      bool initialize( const Keywordlist& kwl );

      /**
       * @brief Initialize from a std::map<std::string, std::string>.
       * @param ap Arg parser to initialize from.
       * @return true, indicating process should continue with execute.
       */
      bool initialize( const std::map<std::string, std::string>& map);

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
      * Will call abort on the writer and return.
      */
      void abort();

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
       * @param Addiontal options that can be passed so you can change some
       *                  of the exposed parameters.  You can actually specify
       *                  and change the cut window without re-initializing
       *
       * @return Status of tile data. This maps to ossimDataObjectStatus
       * from ossim library:
       * OSSIM_STATUS_UNKNOWN = 0,
       * OSSIM_NULL           = 1, not initialized
       * OSSIM_EMPTY          = 2, initialized but blank or empty
       * OSSIM_PARTIAL        = 3, contains some null/invalid values
       * OSSIM_FULL           = 4  all valid data
       */
      int getChip(ossim_int8* data, bool alpha, 
            const std::map<std::string,std::string>& options=std::map<std::string, std::string>());

   private:

      /** @brief Hidden from use copy constructor. */
      Chipper( const Chipper& obj );
      
      /** @brief Hidden from use assignment operator. */
      const Chipper& operator=( const Chipper& rhs );
      
      ossimChipperUtil* m_chipper;
   };

} // End of namespace oms.

#endif /* #ifndef omsChipper_HEADER */
