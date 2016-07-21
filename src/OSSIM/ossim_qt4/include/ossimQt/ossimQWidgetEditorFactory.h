//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
//
// Contains class declaration for ossimQWidgetEditorFactory.
// 
//----------------------------------------------------------------------------
// $Id: ossimQWidgetEditorFactory.h 9508 2006-09-03 16:07:00Z dburken $
#ifndef ossimQWidgetEditorFactory_HEADER
#define ossimQWidgetEditorFactory_HEADER

#include "ossimQWidgetFactoryInterface.h"

class ossimQWidgetEditorFactory : public ossimQWidgetFactoryInterface
{
public:
   
   virtual ~ossimQWidgetEditorFactory();
   
   static ossimQWidgetEditorFactory* instance();

   virtual QWidget* createEditor(ossimObject* obj,
                                 QWidget* parent,
                                 Qt::WFlags f = 0) const;

 protected:
   ossimQWidgetEditorFactory();
   
private:
   ossimQWidgetEditorFactory(const ossimQWidgetEditorFactory&);
   void operator=(const ossimQWidgetEditorFactory&);

   static ossimQWidgetEditorFactory* theInstance;
};

#endif /* #ifndef ossimQWidgetEditorFactory_HEADER */
