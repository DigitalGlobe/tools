//*******************************************************************
//
// License:  See top level LICENSE.txt.
//
// Author:  David Burken
// 
// Description:
// Main interface between the band selector dialog box and the band selector
// from the image chain.
//
//*************************************************************************
// $Id: ossimQtBandSelectorController.cpp 12288 2008-01-09 16:45:07Z dburken $

#include <vector>
using namespace std;

#include <QtGui/QCheckBox>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
// #include <q3buttongroup.h>
#include <QtGui/QListWidgetItem>
#include <QtGui/QListWidget>
#include <ossimQt/ossimQtBandSelectorController.h>
#include <ossimQt/ossimQtBandSelectorDialog.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/imaging/ossimBandSelector.h>
#include <ossim/base/ossimPropertyEvent.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossimQt/ossimQtDisplayUtility.h>

static ossimTrace traceDebug("ossimQtBandSelectorController:degug");

ossimQtBandSelectorController::ossimQtBandSelectorController(ossimQtBandSelectorDialog* dialog)
   :
      ossimConnectableObjectListener(),
      theDialog(dialog),
      theBandSelector(NULL),
      theTrueColorPosition(0),
      theCurrentMode(ossimQtBandSelectorController::UNKNOWN)
{
}

ossimQtBandSelectorController::~ossimQtBandSelectorController()
{
   if (theBandSelector)
   {
      theBandSelector->removeListener((ossimConnectableObjectListener*)this);
      theBandSelector = NULL;
   }
}

void ossimQtBandSelectorController::objectDestructingEvent(ossimObjectDestructingEvent& event)
{
   if (theBandSelector)
   {
      if(event.getObject() == PTR_CAST(ossimObject, theBandSelector))
      {      
         theDialog->close(true);
      }
   }
}

void ossimQtBandSelectorController::clearButtonClicked() const
{
   theDialog->theSelectedBandsListBox->clear();
}

void ossimQtBandSelectorController::allButtonClicked() const
{
   if (!theBandSelector) return;

   theDialog->theSelectedBandsListBox->clear();
   
   // Update the available list box.
   ossim_uint32 bands = theBandSelector->getNumberOfInputBands();
   for (ossim_uint32 band = 0; band < bands; ++band)
   {
      QListWidgetItem* newItem = new QListWidgetItem;
      newItem->setText(ossimString::toString(band+1).c_str());
      theDialog->theSelectedBandsListBox->addItem(newItem);
   }
}

void ossimQtBandSelectorController::rightButtonClicked() const
{
   // Find the selected items and move them over.
   const int COUNT = theDialog->theAvailableBandsListBox->count();
   for (int i = 0; i < COUNT; ++i)
   {
      QListWidgetItem* item = theDialog->theAvailableBandsListBox->item(i);
      if (theDialog->theAvailableBandsListBox->isItemSelected(item))
      {
         QString s = item->text();
         theDialog->theSelectedBandsListBox->addItem(s);
      }
   }
}

void ossimQtBandSelectorController::leftButtonClicked() const
{
   // Find the selected items and remove them.
   const int COUNT = theDialog->theSelectedBandsListBox->count();
   for (int i = 0; i < COUNT; ++i)
   {
      QListWidgetItem* item = theDialog->theSelectedBandsListBox->item(i);
      if (theDialog->theSelectedBandsListBox->isItemSelected(item))
      {
         item = theDialog->theSelectedBandsListBox->takeItem(i);
         if (item)
         {
            delete item;
            item = NULL;
         }
      }
   }
}

void ossimQtBandSelectorController::upButtonClicked() const
{
   // Find the selected items and move them up.
   const int COUNT = theDialog->theSelectedBandsListBox->count();
   
   for (int i = 1; i < COUNT; ++i) // Skip the top cell...
   {
      QListWidgetItem* item = theDialog->theSelectedBandsListBox->item(i);
      if (theDialog->theSelectedBandsListBox->isItemSelected(item))
      {
         item = theDialog->theSelectedBandsListBox->takeItem(i);
         theDialog->theSelectedBandsListBox->insertItem((i-1), item);
      }
   }
}

void ossimQtBandSelectorController::downButtonClicked() const
{
   // Find the selected items and move them down.
   const int COUNT = theDialog->theSelectedBandsListBox->count();
   
   for (int i = (COUNT-2); i >= 0; --i) // Skip the bottom cell...
   {
      QListWidgetItem* item = theDialog->theSelectedBandsListBox->item(i);
      if (theDialog->theSelectedBandsListBox->isItemSelected(item))
      {
         item = theDialog->theSelectedBandsListBox->takeItem(i);
         theDialog->theSelectedBandsListBox->insertItem((i+1), item);
      }
   }
}
   
void ossimQtBandSelectorController::moveToTopButtonClicked() const
{
   // Find the selected items and move them to the top.
   const int COUNT = theDialog->theSelectedBandsListBox->count();

   int top_index = 0;
   for (int i = 1; i < COUNT; ++i) // Skip the top cell...
   {
      QListWidgetItem* item = theDialog->theSelectedBandsListBox->item(i);
      if (theDialog->theSelectedBandsListBox->isItemSelected(item))
      {
         item = theDialog->theSelectedBandsListBox->takeItem(i);
         theDialog->theSelectedBandsListBox->insertItem(top_index, item);
         ++top_index;
      }
   }
}

void ossimQtBandSelectorController::moveToBottomButtonClicked() const
{
   // Find the selected items and move them to the bottom.
   const int COUNT = theDialog->theSelectedBandsListBox->count();

   int bottom_index = COUNT-1;
   for (int i = (COUNT-2); i >= 0; --i) // Skip the bottom cell...
   {
               
      QListWidgetItem* item = theDialog->theSelectedBandsListBox->item(i);
      if (theDialog->theSelectedBandsListBox->isItemSelected(item))
      {
         item = theDialog->theSelectedBandsListBox->takeItem(i);
         theDialog->theSelectedBandsListBox->insertItem(bottom_index, item);
         --bottom_index;
      }
   } 
}

void ossimQtBandSelectorController::applyButtonClicked() const
{
   if (!theBandSelector) return;

   const ossim_uint32 BANDS = theBandSelector->getNumberOfInputBands();
   vector<ossim_uint32> bandList;
   ossim_uint32 count = theDialog->theSelectedBandsListBox->count();
   
   for (ossim_uint32 i = 0; i < count; ++i)
   {
      ossim_uint32 band = 0;
      ossimString s = (const char*)theDialog->theSelectedBandsListBox->
         item(i)->text();
      if (!s.empty())
      {
         band = (ossim_uint32)atoi(s.c_str());
         if (band > 0 && band <= BANDS)
         {
            bandList.push_back(band - 1);
         }
         else
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimQtBandSelectorController::applyButtonClicked "
               << "band index is not within range of 0 to " << BANDS << "!"
               << std::endl;
            return;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimQtBandSelectorController::applyButtonClicked: "
            << "No bands available, the band list is empty!" << std::endl;
         return;
      }
   }

   theBandSelector->enableSource();
   theBandSelector->setOutputBandList(bandList);

   // since no bounding rects change just iuue a repaint by flushing the
   // display cache else we would issue a flushAllOutputs.
   //
   ossimQtDisplayUtility::flushAllOutputs(theBandSelector, true);
}

void ossimQtBandSelectorController::greyButtonClicked()
{
   setModeId(ossimQtBandSelectorController::GREY);
}

void ossimQtBandSelectorController::trueButtonClicked()
{
   setModeId(ossimQtBandSelectorController::TRUE_COLOR);
}

void ossimQtBandSelectorController::nBandButtonClicked()
{
   setModeId(ossimQtBandSelectorController::NBAND);
}

void ossimQtBandSelectorController::availableBandClicked(QListWidgetItem* item)
{
   QString s = item->text();
      
   switch(getModeId())
   {
      case ossimQtBandSelectorController::GREY:
      {
         theDialog->theSelectedBandsListBox->clear();
         theDialog->theSelectedBandsListBox->addItem(s);
         break;
      }
      case ossimQtBandSelectorController::TRUE_COLOR:
      {
         QListWidgetItem* item = theDialog->theSelectedBandsListBox->
            takeItem(theTrueColorPosition);
         if (item)
         {
            delete item;
            item = NULL;
         }
         theDialog->theSelectedBandsListBox->insertItem(theTrueColorPosition,
                                                        s);
         ++theTrueColorPosition;
         if (theTrueColorPosition > 2)
         {
            theTrueColorPosition = 0;
         }

         // Highlight the next band in the selected list.
         item = theDialog->theSelectedBandsListBox->item(theTrueColorPosition);
         theDialog->theSelectedBandsListBox->setItemSelected(item, true);
         break;
      }      
      default:
         break;
   }
}

void ossimQtBandSelectorController::initListBoxes() const
{
   if (!theBandSelector) return;

   
   theDialog->theAvailableBandsListBox->clear();

   const ossim_uint32 BANDS = theBandSelector->getNumberOfInputBands();
   ossim_uint32 band;

   for (band = 0; band < BANDS; ++band)
   {
      QString item = ossimString::toString(band+1).c_str();
      theDialog->theAvailableBandsListBox->addItem(item);
   }

   // Update the selected list box.
   theDialog->theSelectedBandsListBox->clear();
   vector<ossim_uint32> bandList;
   theBandSelector->getOutputBandList(bandList);

   if (getModeId() == ossimQtBandSelectorController::GREY)
   {
      theDialog->theSelectedBandsListBox->
         setSelectionMode(QAbstractItemView::NoSelection);

      QString band1 = "1";

      if (bandList.size() == 1)
      {
         band1 = ossimString::toString(bandList[0]+1).c_str();
      }

      theDialog->theSelectedBandsListBox->addItem(band1);
   }
   else if (getModeId() == ossimQtBandSelectorController::TRUE_COLOR)
   {
      theDialog->theSelectedBandsListBox->
         setSelectionMode(QAbstractItemView::SingleSelection);

      QString band1;
      QString band2;
      QString band3;

      if (bandList.size() > 2)
      {
         band1 = ossimString::toString(bandList[0]+1).c_str();
         band2 = ossimString::toString(bandList[1]+1).c_str();
         band3 = ossimString::toString(bandList[2]+1).c_str();
      }
      else if (theBandSelector->getNumberOfInputBands() > 2)
      {
         band1 = "1";
         band2 = "2";
         band3 = "3";
      }
      else
      {
         band1 = "1";
         band2 = "1";
         band3 = "1";
      }

      theDialog->theSelectedBandsListBox->addItem(band1);
      theDialog->theSelectedBandsListBox->addItem(band2);
      theDialog->theSelectedBandsListBox->addItem(band3);

      QListWidgetItem* item = theDialog->theSelectedBandsListBox->
         item(theTrueColorPosition);
      theDialog->theSelectedBandsListBox->setItemSelected(item, true);
   }
   else // NBand
   {
      theDialog->theSelectedBandsListBox->
         setSelectionMode(QAbstractItemView::SingleSelection);

      for (band = 0; band < bandList.size(); ++band)
      {
         QString item = ossimString::toString(bandList[band]+1).c_str();
         theDialog->theSelectedBandsListBox->addItem(item);
      }
   }
}

void ossimQtBandSelectorController::hideButtons() const
{
   if (!theDialog) return;
   
   theDialog->theClearButton->hide();
   theDialog->theAllButton->hide();
   theDialog->theRightButton->hide();
   theDialog->theLeftButton->hide();
   theDialog->theUpButton->hide();
   theDialog->theDownButton->hide();
   theDialog->theMoveToTopButton->hide();
   theDialog->theMoveToBottomButton->hide();
}

void ossimQtBandSelectorController::showButtons() const
{
   theDialog->theClearButton->show();
   theDialog->theAllButton->show();
   theDialog->theRightButton->show();
   theDialog->theLeftButton->show();
   theDialog->theUpButton->show();
   theDialog->theDownButton->show();
   theDialog->theMoveToTopButton->show();
   theDialog->theMoveToBottomButton->show();
}

void ossimQtBandSelectorController::setBandSelector(ossimBandSelector* bandSelector)
{
   if(theBandSelector)
   {
      theBandSelector->removeListener((ossimConnectableObjectListener*)this);
   }
   
   theBandSelector = bandSelector;
   
   if(theBandSelector)
   {
      theBandSelector->addListener((ossimConnectableObjectListener*)this);
   }

   // This will set up the list boxes.
   setModeId();
}

void ossimQtBandSelectorController::setModeId()
{
   if (!theBandSelector) return;

   vector<ossim_uint32> bandList;
   theBandSelector->getOutputBandList(bandList);
   const ossim_uint32 BANDS = bandList.size();
   
   int mode = ossimQtBandSelectorController::GREY;
   if (BANDS == 3)
   {
      mode = ossimQtBandSelectorController::TRUE_COLOR;
      
   }
   else if (BANDS == 2 || BANDS > 3)
   {
      mode = ossimQtBandSelectorController::NBAND;
   }
   setModeId(mode);
}

void ossimQtBandSelectorController::setModeId(int id)
{
   if (!theDialog) return;

   if (theCurrentMode != id)
   {
      // If grey or true hide the control buttons.
      if ( id == ossimQtBandSelectorController::GREY ||
           id == ossimQtBandSelectorController::TRUE_COLOR )
      {
         hideButtons();
      }
      else
      {
         showButtons();
      }

      if (getModeId() != id)
      {
         if (id == ossimQtBandSelectorController::GREY)
         {
            theDialog->theGreyButton->setChecked(true);
            
         }
         else if (id == ossimQtBandSelectorController::TRUE_COLOR)
         {
            theDialog->theTrueButton->setChecked(true);
         }
         else
         {
            theDialog->theNBandButton->setChecked(true);
         }
      }
      
      initListBoxes();

      theCurrentMode = id;
   }
}

int ossimQtBandSelectorController::getModeId() const
{
   if (!theDialog) return ossimQtBandSelectorController::UNKNOWN;

   if (theDialog->theGreyButton->isChecked())
   {
      return ossimQtBandSelectorController::GREY;
   }
   else if(theDialog->theTrueButton->isChecked())
   {
      return ossimQtBandSelectorController::TRUE_COLOR;
   }
   else if (theDialog->theNBandButton->isChecked())
   {
      return ossimQtBandSelectorController::NBAND;
   }

   return ossimQtBandSelectorController::UNKNOWN;
}

// Private to disallow copy constructor, assignment operator use...
ossimQtBandSelectorController::ossimQtBandSelectorController(const ossimQtBandSelectorController&)
   :
      ossimConnectableObjectListener(),
      theDialog(NULL),
      theBandSelector(NULL),
      theTrueColorPosition(0),
      theCurrentMode(ossimQtBandSelectorController::UNKNOWN)
{
}

ossimQtBandSelectorController& ossimQtBandSelectorController::operator=(const ossimQtBandSelectorController&)
{
   return *this;
}
