//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License: MIT
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//
//*******************************************************************
//  $Id: ossimStreamFactory.h 23664 2015-12-14 14:17:27Z dburken $
//
#ifndef ossimStreamFactory_HEADER
#define ossimStreamFactory_HEADER
#include <ossim/base/ossimStreamFactoryBase.h>
#include <ossim/base/ossimIoStream.h>

class OSSIM_DLL ossimStreamFactory : public ossimStreamFactoryBase
{
public:
   static ossimStreamFactory* instance();
   virtual ~ossimStreamFactory();
 
   virtual ossimRefPtr<ossimIFStream>
      createNewIFStream(const ossimFilename& file,
                        std::ios_base::openmode openMode) const;

   
protected:
   ossimStreamFactory();
   ossimStreamFactory(const ossimStreamFactory&);
   static ossimStreamFactory* theInstance;
   
};

#endif

