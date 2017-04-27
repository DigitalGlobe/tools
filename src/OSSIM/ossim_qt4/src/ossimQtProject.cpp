//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimQtProject.cpp 9114 2006-06-14 17:42:07Z gpotts $

#include <QtGui/QApplication>
#include <QtGui/QMessageBox>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossimQt/ossimQtProject.h>
#include <ossimQt/ossimQtMainWindow.h>
#include <ossimQt/ossimQtImageWindow.h>
  // fix drb #include <ossimQt/ossimQtVceWindow.h>
#include <ossimQt/ossimQtMainWindowWorkspace.h>
#include <ossimQt/ossimDataManager.h>
#include <ossimQt/ossimQtEvent.h>

const ossimString ossimQtProject::EXTENSION="prj";

int ossimQtProject::theUnamedFileCounter = 0;

ossimQtProject::ossimQtProject(QWidget* parent)
{
   theDataManager = new ossimDataManager;
   theWorkspace   = new ossimQtMainWindowWorkspace(parent);
   theWorkspace->hide();
   theWorkspace->setScrollBarsEnabled(true);
   theFilename = "unnamed";
   theFilename += ossimString::toString(theUnamedFileCounter);
   theFilename += ".";
   theFilename +=  ossimQtProject::EXTENSION.c_str();
   theDescription = "";
   ++theUnamedFileCounter;
   theDirtyFlag = true;
}

ossimQtProject::~ossimQtProject()
{
   if(theDataManager)
   {
      delete theDataManager;
      theDataManager = NULL;
   }
   if(theWorkspace)
   {
      delete theWorkspace;
      theWorkspace = NULL;
   }
}

void ossimQtProject::show()
{
   theWorkspace->show(); 
}

void ossimQtProject::hide()
{
   theWorkspace->hide(); 
}

ossimFilename  ossimQtProject::getFilename()const
{
   return theFilename;
}

void ossimQtProject::setFilename(const ossimFilename& filename)
{
   theFilename = filename;
   theFilename = ossimFilename(theFilename.setExtension(ossimQtProject::EXTENSION));
   theDirtyFlag = true;
}

bool ossimQtProject::save()
{
   if (theFilename.exists())
   {
      QString caption("Question:");
      QString text = "Overwrite existing file:  ";
      text += theFilename;
      int answer = QMessageBox::question( theWorkspace->activeWindow(),
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::No)
      {
         return true;
      }
   }

   ossimKeywordlist kwl;
   
   saveState(kwl, "");

   ossimRefPtr<ossimXmlDocument> xmlDocument = new ossimXmlDocument;

   xmlDocument->fromKwl(kwl);
//   kwl.write(theFilename);
//   kwl.write(theFilename);
   xmlDocument->write(theFilename);
   
   theDirtyFlag = false;

   return true;
}


bool ossimQtProject::open(const ossimFilename& project)
{
   bool result = false;
   close();
   theFilename = project;
   ossimRefPtr<ossimXmlDocument> xmlDocument = new ossimXmlDocument;
   ossimKeywordlist kwl;
   bool openedFlag = false;
   
   if(xmlDocument->openFile(theFilename))
   {
      xmlDocument->toKwl(kwl);
      openedFlag = true;
   }
   else if(kwl.addFile(project.c_str()))
   {
      openedFlag = true;
   }
   if(openedFlag)
   {
      result = loadState(kwl, "");
      theDataManager->makeUniqueIds();
   }
   return false;
}

bool ossimQtProject::close()
{
   theWorkspace->closeAll();
   theDataManager->removeAll();
   theFilename = "unnamed";
   theFilename += ossimString::toString(theUnamedFileCounter);
   theFilename += ".";
   theFilename +=  ossimQtProject::EXTENSION;
   theDescription = "";
   ++theUnamedFileCounter;
   theDirtyFlag = false;
   return true;
}

void ossimQtProject::setDirtyFlag(bool flag)
{
   theDirtyFlag = flag;
}

bool ossimQtProject::isDirty()const
{
   return theDirtyFlag;
}

bool ossimQtProject::saveState(ossimKeywordlist& kwl,
                               const ossimString& prefix)const
{
   bool result = true;
   kwl.add(prefix,
	   ossimKeywordNames::TYPE_KW,
	   "ossimQtProject",
	   true);
   kwl.add(prefix,
	   ossimKeywordNames::DESCRIPTION_KW,
	   theDescription,
	   true);
   if(theDataManager)
   {
      if(!theDataManager->saveState(kwl,
                                    prefix + "data."))
      {
         result = false;
      }
   }
   ossimQtStateEvent event(ossimQtStateEvent::ossimQtStateEventType_SAVE);
   
   QWidgetList widgetList = theWorkspace->windowList();
   
   for(int i = 0; i < (int)widgetList.count(); ++i)
   {
      event.setPrefix(prefix+ "display" + ossimString::toString(i) + ".");
      
      QApplication::sendEvent(widgetList.at(i), &event);
   }

   kwl.add(event.getKeywordlist());
   
   return result;
}

bool ossimQtProject::loadState(ossimKeywordlist& kwl,
                               const ossimString& prefix)
{
   bool result = true;
   
   const char* description = kwl.find(prefix,
				      ossimKeywordNames::DESCRIPTION_KW);
   if(description)
     {
       theDescription = description;
     }

   if(theDataManager)
   {
      if(!theDataManager->loadState(kwl,
                                    prefix + "data."))
      {
         result = false;
      }
   }

   ossimString regExpr( prefix+"display[0-9]+" );
   vector<ossimString> splitArray;
   vector<ossimString> keys = kwl.getSubstringKeyList( regExpr );
   ossimQtStateEvent event(ossimQtStateEvent::ossimQtStateEventType_LOAD);
   event.setKeywordlist(kwl);
   
   for(int i = 0; i < (int)keys.size();++i)
   {
      const char* type = kwl.find(keys[i]+".",
                                  ossimKeywordNames::TYPE_KW);
      event.setPrefix(keys[i]+".");
      QObject* window = (QObject*)NULL;
      if(ossimString(type) == "ossimQtImageWindow")
      {
		  ossimQtImageWindow* w = new ossimQtImageWindow(0, 0);
         //window = new ossimQtImageWindow(theWorkspace, 0, Qt::WDestructiveClose);
        window = w;
         theWorkspace->addWindow(w);
       w->setAttribute(Qt::WA_DeleteOnClose);      
     }
      else if(ossimString(type) == "ossimQtVceWindow")
      {
         // fix drb
         // window = new ossimQtVceWindow(theWorkspace, 0, Qt::WDestructiveClose);
      }
      if(window)
      {
         QApplication::sendEvent(window, &event);
      }
   }

   return result;
}

