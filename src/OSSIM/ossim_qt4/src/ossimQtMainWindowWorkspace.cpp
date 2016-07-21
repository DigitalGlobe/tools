//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
//*************************************************************************
// $Id: ossimQtMainWindowWorkspace.cpp 5757 2003-12-12 19:50:32Z gpotts $
#include <QtGui/QApplication>
#include <ossimQt/ossimQtEvent.h>

#include <ossimQt/ossimQtMainWindowWorkspace.h>

void ossimQtMainWindowWorkspace::sendEventToAll(QEvent* evt)
{
  QWidgetList widgetList = windowList();
  
  for(int i = 0; i < (int)widgetList.count(); ++i)
    {
      QApplication::sendEvent(widgetList.at(i), evt);
    }
}

void ossimQtMainWindowWorkspace::minimizeAll()
{
    QWidgetList widgetList = windowList();
    
    for(int i = 0; i < (int)widgetList.count(); ++i)
    {
	widgetList.at(i)->showMinimized();
    }
}

void ossimQtMainWindowWorkspace::restoreAll()
{
    QWidgetList widgetList = windowList();
    
    for(int i = 0; i < (int)widgetList.count(); ++i)
    {
      if((!widgetList.at(i)->isShown())||
	 (widgetList.at(i)->isMinimized()))
	{
	  widgetList.at(i)->showNormal();
	}
    }
}

void ossimQtMainWindowWorkspace::refreshAll()
{
    QWidgetList widgetList = windowList();
 
    for(int i = 0; i < (int)widgetList.count(); ++i)
    {
      ossimQtRefreshDisplayEvent* evt = new ossimQtRefreshDisplayEvent;
      
      QApplication::postEvent(widgetList.at(i), evt);
    }
  
}

void ossimQtMainWindowWorkspace::closeAll()
{
    QCloseEvent evt;
    sendEventToAll(&evt);
    
//     QWidgetList widgetList = windowList();
    
//     for(int i = 0; i < (int)widgetList.count(); ++i)
//     {
//        QCloseEvent* evt = new QCloseEvent;
//        QApplication::postEvent(widgetList.at(i), evt);
//     }
}
