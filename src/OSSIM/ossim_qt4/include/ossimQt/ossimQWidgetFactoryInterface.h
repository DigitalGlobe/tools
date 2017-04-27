//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// Base class interface declaration for QWidget factory.
//
//----------------------------------------------------------------------------
// $Id: ossimQWidgetFactoryInterface.h 12122 2007-12-06 13:27:09Z gpotts $
#ifndef ossimQWidgetFactoryInterface_HEADER
#define ossimQWidgetFactoryInterface_HEADER

#include <QtCore/Qt> /* for Qt::WFlags */

class ossimObject;
class QWidget;

class ossimQWidgetFactoryInterface
{
public:
   ossimQWidgetFactoryInterface();
   
   virtual ~ossimQWidgetFactoryInterface();
   
   virtual QWidget* createEditor(ossimObject* obj,
                                 QWidget* parent,
                                 Qt::WFlags f = 0) const = 0;
};

#endif /* #ifndef ossimQWidgetFactoryInterface_HEADER */
