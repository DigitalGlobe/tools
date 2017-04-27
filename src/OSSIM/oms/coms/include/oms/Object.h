//----------------------------------------------------------------------------
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Base class for oms objects.
//
//----------------------------------------------------------------------------
// $Id: Object.h 13837 2008-11-03 20:26:10Z gpotts $
#ifndef omsObject_HEADER
#define omsObject_HEADER 1

#include <iosfwd>
#include <oms/Constants.h>
#include <ossim/base/ossimReferenced.h>

namespace oms
{
   class OMSDLL Object : public ossimReferenced
   {
   public:
      
      /** default constuctor */
      Object();
      
      /** virtual destructor */
      virtual ~Object();
   };
} // End of namespace std.

#endif /* End of #ifndef omsObject_HEADER */


