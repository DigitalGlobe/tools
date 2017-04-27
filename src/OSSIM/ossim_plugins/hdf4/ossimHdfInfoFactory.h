//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Factory for hdf info objects.
// 
//----------------------------------------------------------------------------
// $Id: ossimHdfInfoFactory.h 2645 2011-05-26 15:21:34Z oscar.kramer $
#ifndef ossimHdfInfoFactory_HEADER
#define ossimHdfInfoFactory_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoFactoryInterface.h>

class ossimFilename;
class ossimInfoBase;

/**
 * @brief Info factory.
 */
class ossimHdfInfoFactory : public ossimInfoFactoryInterface
{
public:

   /** virtual destructor */
   virtual ~ossimHdfInfoFactory();

   static ossimHdfInfoFactory* instance();

   /**
    * @brief create method.
    *
    * @param file Some file you want info for.
    *
    * @return ossimInfoBase* on success 0 on failure.  Caller is responsible
    * for memory.
    */
   virtual ossimInfoBase* create(const ossimFilename& file) const;
   
private:
   
   /** hidden from use default constructor */
   ossimHdfInfoFactory();

   /** hidden from use copy constructor */
   ossimHdfInfoFactory(const ossimHdfInfoFactory& obj);

   /** hidden from use operator = */
   const ossimHdfInfoFactory& operator=(const ossimHdfInfoFactory& rhs);
};

#endif /* End of "#ifndef ossimInfoFactory_HEADER" */


