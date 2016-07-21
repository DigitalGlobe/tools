//*******************************************************************
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author:  David Burken
// 
// Description:
// 
// Controller class for image chain editor dialog.
//
//*************************************************************************
// $Id: ossimQtImageChainEditorController.cpp 19974 2011-08-17 13:51:38Z gpotts $

#include <vector>
#include <algorithm> /* for sort() */

#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QComboBox>
#include <Qt3Support/Q3ListBox>
#include <ossimQt/ossimQtImageChainEditorController.h>
#include <ossimQt/ossimQtImageChainEditorDialog.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossimQt/ossimQtEditorWidgetManager.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/imaging/ossimHistogramRemapper.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossimQt/ossimQWidgetFactoryRegistry.h>
#include <ossimQt/ossimQtPropertyDialog.h>
#include <ossimQt/ossimQtDisplayUtility.h>

static ossimTrace traceDebug("ossimQtImageChainEditorController:degug");

ossimQtImageChainEditorController::ossimQtImageChainEditorController(ossimQtImageChainEditorDialog* dialog)
   :
      ossimConnectableDisplayListener(),
      theImageChain(0),
      theDialog(dialog),
      theWidget(0),
      theCallBackDisabled(false),
      theChainFilterList(),
      theAvailableFilterList()
{
}

ossimQtImageChainEditorController::~ossimQtImageChainEditorController()
{
   theChainFilterList.clear();
   theAvailableFilterList.clear();

   if (theImageChain)
   {
      theImageChain->removeListener(this);
      theImageChain = 0;
   }
}

void ossimQtImageChainEditorController::updateListener(ossimImageChain* ic)
{
   if (ic)
   {
      if (ic != theImageChain)
      {
         if (theImageChain)
         {
            theImageChain->removeListener(this);
         }
         theImageChain = ic;
         theImageChain->addListener(this);
      }
   }
}

void ossimQtImageChainEditorController::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theImageChain)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theImageChain))
      {
         if (theDialog)
         {
            theDialog->close(true);
         }
      }
   }
}

void ossimQtImageChainEditorController::addObjectEvent(ossimContainerEvent& event)
{
   if (theCallBackDisabled) return;
   
   ossimContainerEvent::ObjectList& objects = event.getObjectList();
   if(!objects.empty())
   {
      updateDialog();
   }
}

void ossimQtImageChainEditorController::removeObjectEvent(ossimContainerEvent& event)
{
   if (theCallBackDisabled) return;
   
   ossimContainerEvent::ObjectList& objects = event.getObjectList();
   if(!objects.empty())
   {
      updateDialog();
   }
}

void ossimQtImageChainEditorController::setImageWidget(ossimQtScrollingImageWidget* widget)
{
   theWidget = widget;
   initializeDialog();
}

void ossimQtImageChainEditorController::setAvailableFilterList(const std::vector<ossimString>& list)
{
   // Wipe the original slick.
   theDialog->theAvailableFiltersListBox->clear();
   theAvailableFilterList.clear();
   
   theAvailableFilterList = list;
   std::sort(theAvailableFilterList.begin(), theAvailableFilterList.end());
   std::vector<ossimString>::const_iterator i = theAvailableFilterList.begin();
   while (i != theAvailableFilterList.end())
   {
      // Strip the ossim from the class name...
      ossimString s = (*i).substitute(ossimString("ossim"), ossimString(""));
      QString item = s.c_str();
      theDialog->theAvailableFiltersListBox->insertItem(item);
      ++i;
   } 
}

void ossimQtImageChainEditorController::layerComboBoxActivated()
{
   if (theCallBackDisabled) return;

   updateDialog();
}

void ossimQtImageChainEditorController::insertBeforeButtonClicked()
{
   ossimRefPtr<ossimConnectableObject> left_obj = getSelectedAvailableFilter();
   if (!left_obj)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING ossimQtImageChainEditorController::insertBeforeButtonClicked(): getSelectedAvailableFilter failed, returning..."
         << std::endl;
      return;
   }
   
   ossimRefPtr<ossimConnectableObject> right_obj = getSelectedChainFilter();
   if (!right_obj)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "WARNING ossimQtImageChainEditorController::insertBeforeButtonClicked(): getSelectedChainFilter failed, returning..."
         << std::endl;
      
      left_obj = 0;
      return;
   }
   
   if ( isImageHandler(right_obj.get()) )
   {
      left_obj = 0;
      QString caption("Sorry:");
      QString text =
         "You cannot insert this filter before an image handler...";
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok );
      return;
   }

   ossimImageChain* ic = getChain();
   if (ic)
   {
      QString caption("Question:");
      QString text = "Are you sure you want to insert filter:  ";
      text += left_obj->getShortName().c_str();
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         if (ic->insertLeft(left_obj.get(), right_obj.get()))
         {
            theWidget->refresh();
         }
         else
         {
            left_obj = 0;
            QString caption("Error:");
            QString text = "Filter insertion failed!";
            QMessageBox::information( theDialog,
                                      caption,
                                      text,
                                      QMessageBox::Ok);
         }
      }
      else
      {
         left_obj = 0;
      }
   }
}

void ossimQtImageChainEditorController::insertAfterPushButtonClicked()
{
   ossimConnectableObject* right_obj = getSelectedAvailableFilter();
   if (!right_obj)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimQtImageChainEditorController::insertAfterPushButtonClicked(): getSelectedAvailableFilter failed, returning..."
         << std::endl;
      return;
   }
   ossimConnectableObject* left_obj = getSelectedChainFilter();
   if (!left_obj)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimQtImageChainEditorController::insertAfterPushButtonClicked(): getSelectedChainFilter failed, returning..."
         << std::endl;
      right_obj = 0;
      return;
   }

   ossimImageChain* ic = getChain();
   if (ic)
   {
      QString caption("Question:");
      QString text = "Are you sure you want to insert filter:  ";
      text += right_obj->getShortName().c_str();
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No );
      if (answer == QMessageBox::Yes)
      {
         if (ic->insertRight(right_obj, left_obj))
         {
            theWidget->refresh();
         }
         else
         {
            right_obj = 0;
            QString caption("Error:");
            QString text = "Filter insertion failed!";
            QMessageBox::information( theDialog,
                                      caption,
                                      text,
                                      QMessageBox::Ok );
         }
      }
      else
      {
         right_obj = 0;
      }
   }
}

void ossimQtImageChainEditorController::editPropertiesPushButtonClicked()
{
   if (theCallBackDisabled) return;

   ossimConnectableObject* con_obj = getSelectedChainFilter();

   if ( PTR_CAST(ossimHistogramRemapper, con_obj) )
   {
      // Special case (hack) for histogram stuff...
      ossimHistogramRemapper* remapper = PTR_CAST(ossimHistogramRemapper,
                                                  con_obj);
      //---
      // See if the remapper has a histogram yet, if not:
      // - First try to open it.
      // - If that fails prompt the user.
      //---
      ossimRefPtr<ossimHistogram> h = remapper->getHistogram(0);
      if (!h.valid())
      {
         ossimImageHandler* handler = getImageHandler();
         if (handler)
         {
            ossimFilename file = handler->getFilename();
            file.setExtension("his");
            remapper->openHistogram(file);
         }
      }
   }
   
   if (con_obj)
   {
      ossimObject* obj = PTR_CAST(ossimObject, con_obj);

      // First see if we already have a dialog open for this.
      QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(obj);
      if (w)
      {
         w->show();
         w->raise();
         return;
      }

      w = (QWidget*)ossimQWidgetFactoryRegistry::instance()->
         createEditor(obj, theWidget);
      if (w)
      {
         ossimQtEditorWidgetManager::instance()->addToList(w, obj);
         w->show();
      }
   }
}

void ossimQtImageChainEditorController::updateDialog()
{
   if (!theDialog || !theWidget) return;

   theCallBackDisabled = true;

   // Get the current layer.
   ossim_uint32 layer = theDialog->theLayerComboBox->currentItem();

   buildLayerMenu();
   buildImageChainMenu(layer);
   
   theCallBackDisabled = false;
}

void ossimQtImageChainEditorController::initializeDialog()
{
   if (!theDialog || !theWidget) return;

   theCallBackDisabled = true;

   buildLayerMenu();
   buildImageChainMenu(0);  // Layer 0...
   buildAvailableFiltersMenu();
   
   theCallBackDisabled = false;
}

void ossimQtImageChainEditorController::buildLayerMenu()
{
   //---
   // Setup the layer menu.
   //---
   const ossim_uint32 LAYERS = theWidget->getNumberOfInputs();
   theDialog->theLayerComboBox->clear();
   for (ossim_uint32 layer = 0; layer < LAYERS; ++layer)
   {
      ossimImageHandler* ih = getImageHandler(layer);
      if (ih)
      {
         ossimString s = "layer ";
         s += ossimString::toString(layer + 1);
         s += ":image chain:";
         s += ih->getFilename();
         QString item = s.c_str();
         theDialog->theLayerComboBox->insertItem(item);
      }
   }
}

void ossimQtImageChainEditorController::buildImageChainMenu(ossim_uint32 layer)
{
   theDialog->theChainListBox->clear();
   theChainFilterList.clear();

   ossimImageChain* ic = getChain(layer);
   if (ic)
   {
      updateListener(ic);
      
      const ossim_uint32 NUMBER_OF_LINKS = ic->getNumberOfObjects(false);

      theChainFilterList.resize(NUMBER_OF_LINKS);
      if (NUMBER_OF_LINKS)
      {
         for (ossim_uint32 idx = NUMBER_OF_LINKS; idx > 0; --idx) 
         {
            ossimConnectableObject* filter = (*ic)[(idx-1)];
            if (filter)
            {
               FilterNameList fnl;
               fnl.theClassName = filter->getClassName();
               fnl.theShortName =  filter->getShortName();
               theChainFilterList[(idx-1)] = fnl;
               QString item = fnl.theShortName.c_str();
               theDialog->theChainListBox->insertItem(item);
            }
         }
      }
   }
}

void ossimQtImageChainEditorController::buildAvailableFiltersMenu()
{
   std::vector<ossimString> list;
   ossimImageSourceFactoryRegistry::instance()->getTypeNameList(list);
   setAvailableFilterList(list);
}

void ossimQtImageChainEditorController::deletePushButtonClicked()
{
   if (theCallBackDisabled) return;

   ossimRefPtr<ossimConnectableObject> obj = getSelectedChainFilter();
   if (obj.valid())
   {
      if (obj == getImageHandler())
      {
         QString caption = "Sorry:";
         QString text = "You are not allowed to delete the image handler!";
         QMessageBox::information( theDialog,
                                   caption,
                                   text,
                                   QMessageBox::Ok );
         return;
      }
      
      QString caption("Question:");
      QString text = "Are you sure you want to delete filter:  ";
      text += obj->getShortName().c_str();
      
      // Give the user an already open
      int answer = QMessageBox::question( theDialog,
                                          caption,
                                          text,
                                          QMessageBox::Yes,
                                          QMessageBox::No);
      if (answer == QMessageBox::Yes)
      {
         //---
         // Check for a dialog being open and close it if it is.
         //---
         ossimObject* oo = PTR_CAST(ossimObject, obj.get());
         QWidget* w = ossimQtEditorWidgetManager::instance()->getFromList(oo);
         if (w)
         {
            w->close();
         }
         
         //---
         // The call to remove child is only to fire an event to image chain
         // listeners that an object is being removed.
         // The "delete obj" will actually signal the chain that it is being
         // removed and all connections would be fixed up from that.
         //---
         ossimImageChain* ic = getChain();
         if (ic)
         {
            //---
            // Bypass removeObjectEvent call so that we don't update the
            // dialog before obj is deleted.
            //---
            theCallBackDisabled = true;
            ic->removeChild(obj.get());
            theCallBackDisabled = false;
         }
         updateDialog();
         theWidget->refresh();
      }
   }
}

ossim_int32 ossimQtImageChainEditorController::getLayerIndex() const
{
   return theDialog->theLayerComboBox->currentItem();
}

ossim_int32 ossimQtImageChainEditorController::getSelectedChainFilterIndex() const
{
   return theDialog->theChainListBox->currentItem();
}

ossim_int32 ossimQtImageChainEditorController::getSelectedAvailableFilterIndex() const
{
  return theDialog->theAvailableFiltersListBox->currentItem(); 
}

ossimImageChain* ossimQtImageChainEditorController::getChain() const
{
   return getChain(getLayerIndex());
}

ossimImageChain* ossimQtImageChainEditorController::getChain(ossim_uint32 layer) const
{
   ossimImageChain* ic = 0;
   ossimConnectableObject* obj = theWidget->getInput(layer);
   if (obj)
   {
      ic = PTR_CAST(ossimImageChain, obj);
   }
   else
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimQtImageChainEditorController::getChain: getInput failed!" << std::endl;
   }
   return ic;
}

ossimConnectableObject* ossimQtImageChainEditorController::getSelectedChainFilter() const
{
   ossimConnectableObject* filter = 0;

   if (getSelectedChainFilterIndex() < 0)
   {
      QString caption("Notice:");
      QString text = "You must select an image chain filter!";
      
      // Give the user an already open
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok);
      return filter;
   }

   ossim_uint32 selection =
      static_cast<ossim_uint32>(getSelectedChainFilterIndex());
   
   ossimImageChain* ic = getChain();
   if (ic)
   {
      const ossim_uint32 NUMBER_OF_LINKS = ic->getNumberOfObjects(false);
      const ossim_uint32 INDEX = (NUMBER_OF_LINKS - selection) - 1;
      if (INDEX < NUMBER_OF_LINKS)
      {
         filter = (*ic)[INDEX];
         if ( filter && (INDEX < theChainFilterList.size()) )
         {
            if (filter->getClassName() != theChainFilterList[INDEX].theClassName)
            {
               filter = 0;
               ossimNotify(ossimNotifyLevel_FATAL)
                  << "FATAL ossimQtImageChainEditorController::getSelectedChainFilter "
                  << "\nDialog chain and filter chain mismatch!"
                  << std::endl;
            }
         }
      }
   }
   return filter;
}

ossimConnectableObject* ossimQtImageChainEditorController::getSelectedAvailableFilter() const
{
   ossimConnectableObject* filter = 0;

   if (getSelectedAvailableFilterIndex() < 0)
   {
      QString caption("Notice:");
      QString text = "You must select a filter to insert!";
      
      // Give the user an already open
      QMessageBox::information( theDialog,
                                caption,
                                text,
                                QMessageBox::Ok);
      return filter;
   }

   ossim_uint32 selection =
      static_cast<ossim_uint32>(getSelectedAvailableFilterIndex());

   if (selection < theAvailableFilterList.size())
   {
      ossimObject* obj = ossimImageSourceFactoryRegistry::instance()->createObject(theAvailableFilterList[selection]);
      filter = PTR_CAST(ossimConnectableObject, obj);
   }

   return filter;
}

ossimImageHandler* ossimQtImageChainEditorController::getImageHandler() const
{
   return getImageHandler(getLayerIndex());
}

ossimImageHandler* ossimQtImageChainEditorController::getImageHandler(ossim_uint32 layer) const
{
   ossimImageHandler* handler = 0;
   ossimImageChain* ic = getChain(layer);
   if (ic)
   {
      // Image handler usually at the end of chain so start there...
      const ossim_uint32 NUMBER_OF_LINKS = ic->getNumberOfObjects(false);
      for (ossim_uint32 idx = NUMBER_OF_LINKS; idx > 0; --idx) 
      { 
         ossimConnectableObject* filter = (*ic)[(idx-1)];
         if (filter)
         {
            if ( PTR_CAST(ossimImageHandler, filter) )
            {
               handler = PTR_CAST(ossimImageHandler, filter);
               break;
            }
         }
      }
   }
   return handler;
}

bool ossimQtImageChainEditorController::isImageHandler(const ossimConnectableObject* obj) const
{
   return ( (PTR_CAST(ossimImageHandler, obj) == 0) ? false : true);
}

// Private to disallow copy constructor, assignment operator use...
ossimQtImageChainEditorController::ossimQtImageChainEditorController(const ossimQtImageChainEditorController&)
   :
      ossimConnectableDisplayListener(),
      theDialog(0),
      theCallBackDisabled(false)
{
}

ossimQtImageChainEditorController& ossimQtImageChainEditorController::operator=(const ossimQtImageChainEditorController&)
{
   return *this;
}
