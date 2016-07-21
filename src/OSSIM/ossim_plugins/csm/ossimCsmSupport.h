//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  Bill Watkins
//
// Description: //
//----------------------------------------------------------------------------
// $Id: ossimPngPluginInit.cpp 11046 2007-05-25 18:03:03Z gpotts $
#ifndef ossimCsmSupport_HEADER
#define ossimCsmSupport_HEADER
#include <ossim/base/ossimFilename.h>
class ossimCsmSupport
{
public:
   static ossimCsmSupport* instance();
   const ossimFilename& csmPluginDirectory();
   void init();
protected:
   ossimCsmSupport();
   static ossimCsmSupport* m_instance;
   ossimFilename m_pluginDirectory;
};
#endif
