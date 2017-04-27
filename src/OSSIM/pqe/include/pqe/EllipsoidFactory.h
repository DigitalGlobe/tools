//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Ellipsoid factory class.
//
// TODO: Make comma separated value (csv) file reader for create method for
// other ellipsoids.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef pqe_EllipsoidFactory_HEADER
#define pqe_EllipsoidFactory_HEADER 1

#include <pqe/Constants.h>

namespace pqe
{
   class Ellipsoid;
   
   class PQEDLL EllipsoidFactory
   {
   public:

      /** @brief destructor */
      ~EllipsoidFactory();

      /**
       * @brief Instance method.
       *
       * @return The instance of this class.
       */
      static pqe::EllipsoidFactory* instance();

      /** @return Pointer to wgs 84 ellipsoid. */ 
      const pqe::Ellipsoid* wgs84() const;
      
   private:

      /** @brief default constructor - hidden from use */
      EllipsoidFactory();

      /** @brief copy constructor - hidden from use */
      EllipsoidFactory(const pqe::EllipsoidFactory& obj);

      /** @brief assignment operator - hidden from use */ 
      const pqe::EllipsoidFactory& operator=(const pqe::EllipsoidFactory& obj);

      /** @brief The single static instance of this class. */
      static pqe::EllipsoidFactory* theInstance;

      /** @brief instance of the wgs 84 ellipsiod. */
      static const Ellipsoid* theWgs84Ellipsoid;
      
   }; // End of class EllipsoidFactory.

} // End of namespace pqe.

#endif /* #ifndef pqe_EllipsoidFactory_HEADER */
