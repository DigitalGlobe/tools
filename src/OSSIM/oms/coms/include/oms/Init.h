//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description: Singleton class for queries about things like writer strings
// that should stay the same throughout the session.
//
// $Id: Init.h 22314 2013-07-17 19:50:48Z dburken $
//----------------------------------------------------------------------------
#ifndef omsInit_HEADER
#define omsInit_HEADER 1

#include <vector>
#include <string>
#include <oms/Constants.h>

namespace oms
{
   class OMSDLL Init
   {
   public:

      /** @brief destructor */
      ~Init();

      /**
       * @brief Instance method.
       *
       * @return The instance of this class.
       */
      static oms::Init* instance();

      /**
       * @brief Initialize method.
       *
       * @param argc Argument count.
       *
       * @param argv Array of args.
       *
       * @return Remaining argc count.
       */
      int initialize(int argc, char* argv[]);

      /** @brief Initialize method. */
      void initialize();

   private:
      bool theInitCalledFlag;
      /** @brief default constructor - hidden from use */
      Init();

      /** @brief copy constructor - hidden from use */
      Init(const oms::Init& obj);

      /** @brief assignment operator - hidden from use */ 
      const oms::Init& operator=(const oms::Init& obj);

      /** @brief The single static instance of this class. */
      static oms::Init* theInstance;
   };

} // End of namespace oms.

#endif /* #ifndef omsInit_HEADER */
