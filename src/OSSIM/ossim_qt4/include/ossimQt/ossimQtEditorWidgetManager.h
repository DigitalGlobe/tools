//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  Utility class to manage widgets spawned by dialogs.
// 
//
// $Id: ossimQtEditorWidgetManager.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtEditorWidgetManager_HEADER
#define ossimQtEditorWidgetManager_HEADER

#include <vector>
#include <QtCore/QObject>
#include "ossimQtExport.h"

class ossimObject;
class QWidget;
class ossimString;

/**
 * @class ossimQtEditorWidgetManager
 * Utility class to manage widgets spawned by dialogs.  Usages designed to
 * prevent opening duplicate dialogs.
 *
 * Example:
 *  ossimHistogramRemapper* hr = PTR_CAST(ossimHistogramRemapper, con_obj);
 *  QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(obj);
 *  if (w)
 *  {
 *     w->raise();
 *  }
 *  else
 *  {
 *     w = ossimQWidgetFactoryRegistry::instance()->
 *          createEditor(obj, theWidget);
 *     if (w)
 *     {
 *        ossimQtEditorWidgetManager::instance()->addToList(w, obj);
 *        w->show();
 *     }
 *  }   
 */                                             
class OSSIMQT_DLL ossimQtEditorWidgetManager : public QObject
{
   Q_OBJECT
      
public:

   static ossimQtEditorWidgetManager* instance();

   /** destructor */
   virtual ~ossimQtEditorWidgetManager();

   /**
    * @param widget Widget that controls object.
    *
    * @param obj Object that the widget controls.  This can be null for widgets
    * that don't control an ossim object such as an "About Dialog".
    *
    * Example (hr = ossimHistogramRemapper* and hd = ossimQtHistogramDialog*)
    * ossimQtEditorWidgetManager::instance()->addToList(hd, hr);
    * hd->show();
    */
   void addToList(QWidget* widget, const ossimObject* obj=NULL);

   /**
    * @note This method assumes obj is unique in the list.  Use getFromList
    * that takes a widget name if not.
    * 
    * @param obj Usually the object the widget controlls.
    * 
    * @return Controlling widget of obj or NULL if not found in the list.
    *
    * Example (hr = ossimHistogramRemapper*)
    * QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(hr);
    * if (w)
    * {
    *    w->raise();
    *    return;
    * }
    * 
    */
   QWidget* getFromList(const ossimObject* obj) const;

   /**
    * @param widget_name The name of the widget or dialog box.
    *
    * @param obj The object to associate the widget with.  This can be null
    * for widgets that don't control an ossim object such as an
    * "About Dialog".
    *
    * @return Widget belonging to obj with widget name or NULL if not
    * found in the list.
    *
    * @note This should be used if obj has more than one widget in the list.
    */
   QWidget* getFromList(const ossimString& widget_name,
                        const ossimObject* obj=NULL) const;

public slots:

   /**
    * Slot to connect to widget's "destroyed" signal.
    */
   void clearFromList(QObject* widget);
   
private:
   /** Hide default constructor.  Must use instance method. */
   ossimQtEditorWidgetManager();
   
   /** Hide from use copy constructor. */
   ossimQtEditorWidgetManager(const ossimQtEditorWidgetManager& obj);

   /** Hide from use assignment operator. */
   const ossimQtEditorWidgetManager& operator=(const ossimQtEditorWidgetManager& rhs);

   static ossimQtEditorWidgetManager* theInstance;

   // Private constainer class.
   class ossimObjQWidgetPair
   {
   public:
      ossimObjQWidgetPair();
      ossimObjQWidgetPair(const ossimObject* obj,
                          QWidget* widget);
      const ossimObject* theOssimObject;
      QWidget*           theWidget;
   };

   //---
   // Changed from map to vector of container class since the ossimObject
   // can be duplicated.
   //---
   std::vector<ossimObjQWidgetPair> theMap;
};

#endif /* End of "#ifndef ossimQtEditorWidgetManager_HEADER" */

