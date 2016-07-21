//-----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Datum factory class.
//
// TODO: Make comma separated value (csv) file reader for create method for
// other datums.
// 
//-----------------------------------------------------------------------------
// $Id$

#ifndef pqe_DatumFactory_HEADER
#define pqe_DatumFactory_HEADER 1

#include <iosfwd>
#include <string>

#include <pqe/Constants.h>

namespace pqe
{
   class Datum;
   
   class PQEDLL DatumFactory
   {
   public:

      /** @brief destructor */
      ~DatumFactory();

      /**
       * @brief Instance method.
       *
       * @return The instance of this class.
       */
      static pqe::DatumFactory* instance();

      /** @return Pointer to wgs 84 datum. */ 
      const pqe::Datum* wgs84() const;

      /**
       * @return Pointer datum matching code.
       * @note Currently only supports WGE.
       */ 
      const pqe::Datum* create(const std::string& code) const;
      
      
   private:

      /** @brief default constructor - hidden from use */
      DatumFactory();

      /** @brief copy constructor - hidden from use */
      DatumFactory(const pqe::DatumFactory& obj);

      /** @brief assignment operator - hidden from use */ 
      const pqe::DatumFactory& operator=(const pqe::DatumFactory& obj);

      /** @brief The single static instance of this class. */
      static pqe::DatumFactory* theInstance;

      /** @brief instance of the wgs 84 datum. */
      static const pqe::Datum* theWgs84Datum;
 
   }; // End of class DatumFactory

} // End of namespace pqe.

#endif /* #ifndef pqe_DatumFactory_HEADER */
