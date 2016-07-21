//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  A QTProgressDialog and an ossimProcessListener.
//
// $Id: ossimQtProgressDialog.cpp 23664 2015-12-14 14:17:27Z dburken $
//----------------------------------------------------------------------------
#include <ossimQt/ossimQtProgressDialog.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <QtCore/QString>
#include <QtGui/QApplication>

ossimQtProgressDialog::ossimQtProgressDialog(QWidget * creator,
                                             const char * name,
                                             bool modal,
                                             Qt::WFlags f)
   : Q3ProgressDialog(creator, name, modal, f),
     ossimProcessListener()
                                            
{
   init();
   connect( this, SIGNAL(canceled()), this, SLOT(cancelClicked()) );
   theProcessInterface = 0;
}

ossimQtProgressDialog::ossimQtProgressDialog(const QString& labelText,
                                             const QString& cancelButtonText,
                                             int totalSteps,
                                             QWidget * creator,
                                             const char * name,
                                             bool modal,
                                             Qt::WFlags f)
   : Q3ProgressDialog(labelText,
                     cancelButtonText,
                     totalSteps,
                     creator,
                     name,
                     modal,
                     f),
     ossimProcessListener()
                                            
{
   init();
   connect(this, SIGNAL(canceled()), this, SLOT(cancelClicked()) );
   theProcessInterface = 0;
}

ossimQtProgressDialog::ossimQtProgressDialog(const QString& labelText)
   : Q3ProgressDialog(labelText,
                     "cancel",
                     100,
                     0,
                     0,
                     true,
                     0),
     ossimProcessListener()
                                            
{
   init();
   connect( this, SIGNAL(canceled()), this, SLOT(cancelClicked()) );
   theProcessInterface = 0;
}

void ossimQtProgressDialog::init()
{
   // Start at 0 by default.
   setProgress(0);

   //---
   // Set this to false so that "wasCanceled" will return true if canceled.
   // Users can change this as "setAutoReset" is a public method.
   //---
   setAutoReset(false);
   theProcessInterface = 0;
}

ossimQtProgressDialog::~ossimQtProgressDialog()
{
}

void ossimQtProgressDialog::processProgressEvent(ossimProcessProgressEvent& event)
{
   if (event.getOutputMessageFlag())
   {
      ossimString s;
      event.getMessage(s);
      if (!s.empty())
      {
         setLabelText(s.c_str());
      }
   }

   int progress = static_cast<int>(event.getPercentComplete());
   setProgress(progress);
   
   qApp->processEvents();
}

// Private to disallow copy constructor, assignment operator use...
ossimQtProgressDialog::ossimQtProgressDialog(const ossimQtProgressDialog&)
   : Q3ProgressDialog(),
     ossimProcessListener()
{
}

ossimQtProgressDialog& ossimQtProgressDialog::operator=(const ossimQtProgressDialog&)
{
   return *this;
}

void ossimQtProgressDialog::setProcessInterface(ossimProcessInterface* processInterface)
{
  theProcessInterface = processInterface;
}

void ossimQtProgressDialog::cancelClicked()
{
  if(theProcessInterface)
  {
    theProcessInterface->abort();
  }
}

