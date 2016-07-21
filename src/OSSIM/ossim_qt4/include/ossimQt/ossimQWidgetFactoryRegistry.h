//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for the class ossimQWidgetFactoryRegistry.
// 
//----------------------------------------------------------------------------
// $Id: ossimQWidgetFactoryRegistry.h 12108 2007-12-04 17:42:05Z gpotts $
#ifndef ossimQWidgetFactoryRegistry_HEADER
#define ossimQWidgetFactoryRegistry_HEADER

#include <vector>
#include <QtCore/Qt> /* for Qt::WFlags */

class ossimQWidgetFactoryInterface;
class ossimObject;
class QWidget;

class ossimQWidgetFactoryRegistry
{
public:
   
   virtual ~ossimQWidgetFactoryRegistry();

   static ossimQWidgetFactoryRegistry* instance();
   
   void addFactory(ossimQWidgetFactoryInterface* factory);

   void registerFactory(ossimQWidgetFactoryInterface* factory);

   virtual QWidget* createEditor(ossimObject* obj,
                                 QWidget* parent,
                                 Qt::WFlags f = 0) const;

protected:
   ossimQWidgetFactoryRegistry();
   
private:
   ossimQWidgetFactoryRegistry(const ossimQWidgetFactoryRegistry& rhs);
   const ossimQWidgetFactoryRegistry& operator=(const ossimQWidgetFactoryRegistry& rhs);
   
   void clear();
   
   std::vector<ossimQWidgetFactoryInterface*> theFactoryList;
   static ossimQWidgetFactoryRegistry* theInstance;
};

#endif /* #ifndef ossimQWidgetFactoryRegistry_HEADER */
