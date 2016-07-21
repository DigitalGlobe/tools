//----------------------------------------------------------------------------
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimQWidgetFactorRegistry.
//
// $Id: ossimQWidgetFactoryRegistry.cpp 12141 2007-12-07 18:05:13Z gpotts $
//----------------------------------------------------------------------------

#include <iostream>
using namespace std;

#include <QtGui/QWidget>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossimQt/ossimQWidgetFactoryRegistry.h>
#include <ossimQt/ossimQWidgetEditorFactory.h>
#include <ossimQt/ossimQWidgetFactoryInterface.h>

ossimQWidgetFactoryRegistry* ossimQWidgetFactoryRegistry::theInstance = NULL;

ossimQWidgetFactoryRegistry::ossimQWidgetFactoryRegistry()
   :
      theFactoryList()
{
}

ossimQWidgetFactoryRegistry* ossimQWidgetFactoryRegistry::instance()
{
   if (theInstance == NULL)
   {
      theInstance = new ossimQWidgetFactoryRegistry();
      theInstance->registerFactory(ossimQWidgetEditorFactory::instance());
   }
   return theInstance;
}

ossimQWidgetFactoryRegistry::~ossimQWidgetFactoryRegistry()
{
   clear();
   if (theInstance)
   {
      delete theInstance;
      theInstance = NULL;
   }
}

void ossimQWidgetFactoryRegistry::addFactory(ossimQWidgetFactoryInterface* factory)
{
   registerFactory(factory);
}

void ossimQWidgetFactoryRegistry::registerFactory(ossimQWidgetFactoryInterface* factory)
{
   if(factory)
   {
      theFactoryList.push_back(factory);
   }
}

QWidget* ossimQWidgetFactoryRegistry::createEditor(ossimObject* obj,
                                                   QWidget* parent,
                                                   Qt::WFlags f) const
{
   QWidget* result = NULL;
   
   if (!obj)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimQWidgetEditorFactory::createEditor: NULL obj passed in!" << std::endl;
      return result;
   }
   if (!parent)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimQWidgetEditorFactory::createEditor: NULL parent passed in!" << std::endl;
      return result;
   }

   vector<ossimQWidgetFactoryInterface*>::const_iterator factory;
   factory = theFactoryList.begin();
   while((factory != theFactoryList.end()) && !result)
   {
     result = (*factory)->createEditor(obj, parent, f);
     ++factory;
   }
   
   return result;
}

void ossimQWidgetFactoryRegistry::clear()
{
   theFactoryList.clear();
}

ossimQWidgetFactoryRegistry::ossimQWidgetFactoryRegistry(const ossimQWidgetFactoryRegistry&)
{}

const ossimQWidgetFactoryRegistry&
ossimQWidgetFactoryRegistry::operator=(const ossimQWidgetFactoryRegistry& rhs)
{ return rhs; }
