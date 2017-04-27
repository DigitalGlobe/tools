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
// $Id: ossimQtEditorWidgetManager.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------

#include <QtGui/QWidget>

#include <ossimQt/ossimQtEditorWidgetManager.h>
#include <ossim/base/ossimString.h>


ossimQtEditorWidgetManager* ossimQtEditorWidgetManager::theInstance = NULL;

ossimQtEditorWidgetManager::ossimObjQWidgetPair::ossimObjQWidgetPair()
   : theOssimObject(NULL),
     theWidget(NULL)
{
}

ossimQtEditorWidgetManager::ossimObjQWidgetPair::ossimObjQWidgetPair(const ossimObject* obj, QWidget* widget)
   : theOssimObject(obj),
     theWidget(widget)
{
}

ossimQtEditorWidgetManager::~ossimQtEditorWidgetManager()
{
   theMap.clear();
}

void ossimQtEditorWidgetManager::addToList(QWidget* widget,
                                           const ossimObject* obj)
{
   if (!widget)
   {
      return;  // Should never happen...
   }

   ossimObjQWidgetPair map(obj, widget);
   
   theMap.push_back(map);

   connect( widget, SIGNAL ( destroyed(QObject*) ),
            this,   SLOT   ( clearFromList(QObject*) ) );
}

QWidget* ossimQtEditorWidgetManager::getFromList(const ossimObject* obj) const
{
   QWidget* widget = NULL;

   if (!obj)
   {
      return widget;
   }

   std::vector<ossimObjQWidgetPair>::const_iterator i = theMap.begin();
   while (i != theMap.end())
   {
      if ( (*i).theOssimObject == obj)
      {
         widget = (*i).theWidget;
         break;
      }
      ++i;
   }   

   return widget;
}

QWidget*
ossimQtEditorWidgetManager::getFromList(const ossimString& widget_name,
                                        const ossimObject* obj) const
{
   QWidget* widget = NULL;

   if (widget_name.empty())
   {
      return widget;
   }
   
   std::vector<ossimObjQWidgetPair>::const_iterator i = theMap.begin();
   while (i != theMap.end())
   {
      if ( (*i).theOssimObject == obj)
      {
         ossimString s = (*i).theWidget->name();
         if (s == widget_name)
         {
            widget = (*i).theWidget;
            break;
         }
      }
      ++i;
   }
   
   return widget;
}

void ossimQtEditorWidgetManager::clearFromList(QObject* widget)
{
   if (!widget)
   {
      return;
   }

   QWidget* w = (QWidget*)widget;
   
   std::vector<ossimObjQWidgetPair>::iterator i = theMap.begin();
   while (i != theMap.end())
   {
      if ( (*i).theWidget == w)
      {
         theMap.erase(i);
         break;
      }
      ++i;
   }
}

ossimQtEditorWidgetManager* ossimQtEditorWidgetManager::instance()
{
   if (!theInstance)
   {
      theInstance = new ossimQtEditorWidgetManager();
   }
   return theInstance;
}

ossimQtEditorWidgetManager::ossimQtEditorWidgetManager()
   : QObject(),
     theMap()
{
}

ossimQtEditorWidgetManager::ossimQtEditorWidgetManager(const ossimQtEditorWidgetManager& /* obj */ )
   : QObject()
{
}

const ossimQtEditorWidgetManager& ossimQtEditorWidgetManager::operator=(const ossimQtEditorWidgetManager& /* rhs */ )
{
   return *this;
}

