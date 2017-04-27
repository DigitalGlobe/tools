//----------------------------------------------------------------------------
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
//----------------------------------------------------------------------------
// $Id: ossimQtPropertyDialogController.cpp 23664 2015-12-14 14:17:27Z dburken $

#include <ossimQt/ossimQtPropertyDialogController.h>

#include <ossimQt/ossimQtPropertyListView.h>
#include <ossimQt/ossimQtPropertyDialog.h>

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimListenerManager.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimProcessInterface.h>

#include <QMessageBox>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>

class ossimQtPropertyDialogControllerPrivateListener : public ossimConnectableObjectListener
{
public:
   ossimQtPropertyDialogControllerPrivateListener(ossimQtPropertyDialogController* controller)
      :theController(controller)
      {}
   virtual void objectDestructingEvent(ossimObjectDestructingEvent& /*event*/);
   
protected:
   ossimQtPropertyDialogController* theController;
};

void ossimQtPropertyDialogControllerPrivateListener::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if(event.getObject() == theController->theObject)
   {
      theController->setObject(NULL);
   }
}


ossimQtPropertyDialogController::ossimQtPropertyDialogController(ossimQtPropertyDialog* dialog)
   :theDialog(dialog),
    theObject(NULL)
{
   connect(theDialog->theCloseButton,
           SIGNAL(clicked()),
           this,
           SLOT(closeButton()));

   connect(theDialog->theResetButton,
           SIGNAL(clicked()),
           this,
           SLOT(resetButton()));
   connect(theDialog->theApplyButton,
           SIGNAL(clicked()),
           this,
           SLOT(applyButton()));

   connect(theDialog->theAutoApplyCheckBox,
           SIGNAL(toggled(bool)),
           this,
           SLOT(autoApplyToggledButton(bool)));
   thePrivateListener = new ossimQtPropertyDialogControllerPrivateListener(this);
   
   theDialog->thePropertyListView->setAutoApplyFlag(theDialog->theAutoApplyCheckBox->isChecked());
}

ossimQtPropertyDialogController::~ossimQtPropertyDialogController()
{
   ossimListenerManager* listenerManager = PTR_CAST(ossimListenerManager,
                                                    theObject);
   if(listenerManager)
   {
      listenerManager->removeListener(thePrivateListener);
   }
   if(thePrivateListener)
   {
      delete thePrivateListener;
      thePrivateListener = NULL;
   }
   theDialog = NULL;
}

void ossimQtPropertyDialogController::setObject(ossimObject* obj)
{
   if(obj&&theDialog)
   {
      if(theObject!=obj)
      {
         ossimListenerManager* listenerManager = PTR_CAST(ossimListenerManager,
                                                          obj);
         if(listenerManager)
         {
            listenerManager->addListener(thePrivateListener);
            theObject = obj;
            /*add special case cast for execute() button here */
            ossimProcessInterface* prcInf = dynamic_cast<ossimProcessInterface*>( obj );
            if ( prcInf )
            {
               // add execute button
               connect(theDialog->theExecuteButton, SIGNAL(clicked()), this, SLOT(executeButton()));
               theDialog->showExecuteButton();
            }
            theDialog->thePropertyListView->setObject(theObject);
         }
         else
         {
            closeButton();
         }
      }
   }
   else
   {
      closeButton();
   }
}

void ossimQtPropertyDialogController::executeButton()
{
   ossimProcessInterface* prcInf = dynamic_cast<ossimProcessInterface*>( theObject );
   if ( prcInf )
   {

      try
      {
         if ( prcInf->execute() )
         {
            ; // do nothing for now
         }
      }
      catch( const ossimException& e )
      {
         QString caption = "Exception caught!\n";
         QString text = e.what();
         QMessageBox::information( theDialog,
                                   caption,
                                   text,
                                   QMessageBox::Ok );
      }
   }
}

void ossimQtPropertyDialogController::closeButton()
{
   if(theDialog)
   {
      theDialog->close(true);
   }
}

void ossimQtPropertyDialogController::resetButton()
{
   theDialog->thePropertyListView->resetAllProperties();
}

void ossimQtPropertyDialogController::applyButton()
{
   theDialog->thePropertyListView->applyAllProperties();
}

void ossimQtPropertyDialogController::autoApplyToggledButton(bool value)
{
   theDialog->thePropertyListView->setAutoApplyFlag(value);
}

