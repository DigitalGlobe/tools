#include <map>
#include <Qt3Support/Q3WidgetStack>
#include <QtGui/QSlider>
#include <Qt3Support/Q3ListBox>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <ossimQt/ossimQtCombinerEditorDialogController.h>
#include <ossimQt/ossimQtCombinerEditorDialog.h>
#include <ossimQt/ossimQtApplicationUtility.h>
#include <ossimQt/ossimQtDisplayUtility.h>
#include <ossimQt/ossimQtLayerEditor.h>
#include <ossimQt/ossimQtLayerEditorController.h>

#include <ossimQt/ossimQtEvent.h>
#include <ossimQt/ossimDataManager.h>
#include <ossim/imaging/ossimBlendMosaic.h>
#include <ossim/imaging/ossimBumpShadeTileSource.h>
#include <ossim/imaging/ossimImageToPlaneNormalFilter.h>
#include <ossim/imaging/ossimTopographicCorrectionFilter.h>
#include <ossim/base/ossimConnectableObject.h>
#include <ossim/imaging/ossimImageChain.h>

ossimQtCombinerEditorDialogController::ossimQtCombinerEditorDialogController(ossimQtCombinerEditorDialog* dialog)
  :theDialog(dialog),
   theCombiner(NULL),
   theParent(NULL)
{
  
   connect(theDialog->theElevationAngleInput, SIGNAL(returnPressed()), this, SLOT(hillshadePropertyChange()));
   connect(theDialog->theAzimuthAngleInput, SIGNAL(returnPressed()), this, SLOT(hillshadePropertyChange()));
   connect(theDialog->theSmoothnessFactorInput, SIGNAL(returnPressed()), this, SLOT(hillshadePropertyChange()));

   connect(theDialog, SIGNAL(apply()), this, SLOT(applyChanges()));
   connect(theDialog, SIGNAL(reset()), this, SLOT(reset()));
   connect(theDialog->theBlendWeightSlider, SIGNAL(sliderReleased()), this, SLOT(blendSliderReleased()));
   connect(theDialog->theInputWeightListBox, SIGNAL(highlighted(const QString & )), this, SLOT(blendSelectionHighlighted(const QString&)));
   connect(theDialog->theLayerEditor->controller(), SIGNAL(inputLayersChanged()),
           this, SLOT(inputsChanged()));
   connect(theDialog->theLayerEditor->controller(), SIGNAL(removingSelectedLayersFromInput(bool&)),
           this, SLOT(removingSelectedLayersFromInput(bool&)));
   connect(theDialog->theLayerEditor->controller(), SIGNAL(addingSelectedLayersToInput(bool&)),
           this, SLOT(addingSelectedLayersToInput(bool&)));
}

void ossimQtCombinerEditorDialogController::initializeDialog()
{
  setAvailableAndInputs();
  setAdditionalInputs();
}

void ossimQtCombinerEditorDialogController::setObject(ossimConnectableObject* obj)
{
   theCombiner = PTR_CAST(ossimImageCombiner, obj);
   
   if(!theCombiner)
   {
      ossimImageChain* chain = PTR_CAST(ossimImageChain, obj);
      if(chain)
      {
         theCombiner = PTR_CAST(ossimImageCombiner, chain->getLastObject());
      }
   }
   
   if(PTR_CAST(ossimImageChain, obj))
   {
      theParent = obj;
   }
   else if(obj)
   {
      theParent = PTR_CAST(ossimConnectableObject, obj->getOwner());
   }
   
   theOriginalState.clear();  
   if(theCombiner)
   {
      theCombiner->saveState(theOriginalState);
   }

   initializeDialog();
}

void ossimQtCombinerEditorDialogController::reset()
{
}

void ossimQtCombinerEditorDialogController::hillshadePropertyChange()
{
  applyChanges(false);
}


void ossimQtCombinerEditorDialogController::blendSelectionHighlighted(const QString& value)
{
  theDialog->theBlendWeightSlider->setValue((int)(theDialog->theBlendWeightSlider->maxValue()*value.toDouble()));
}

void ossimQtCombinerEditorDialogController::blendSliderReleased()
{
  double t = ((double)theDialog->theBlendWeightSlider->value())/((double)theDialog->theBlendWeightSlider->maxValue());

  double count = theDialog->theInputWeightListBox->count();
  if(count > 1)
    {
       Q3ListBoxItem* currentSelectedListBoxItem = theDialog->theInputWeightListBox->selectedItem();
       
       if(!currentSelectedListBoxItem) return;
       int idx =  theDialog->theInputWeightListBox->index(currentSelectedListBoxItem);
       theDialog->theInputWeightListBox->changeItem(QString().setNum(t), idx);
       
       theDialog->theInputWeightListBox->setSelected(idx, true);
       if(theDialog->theAutoApplyCheckBox->isOn())
       {
	  applyBlendChanges(true);
       }
    }
}

void ossimQtCombinerEditorDialogController::applyChanges(bool fullRefreshFlag)
{
  ossimQtGetDataManagerEvent event;

  ossimQtApplicationUtility::sendEventToRoot(theDialog,
					     &event);
  

  ossimDataManager* dataManager = event.getDataManager();
  
  if(!dataManager)
  {
     return;
  }
  ossimConnectableObject* parent = theParent;
  
  if(!parent)
  {
     parent = theCombiner;
  }
  int count = theDialog->theLayerEditor->theCurrentInputLayersListBox->count();

  ossimConnectableObject::ConnectableObjectList objList;
  bool okToChangeInputs = true;
  ossimConnectableObject* currentObj = NULL;
  for(int idx = 0; idx < count; ++idx)
  {
     Q3ListBoxItem* item = theDialog->theLayerEditor->theCurrentInputLayersListBox->item(idx);
     if(item)
     {
        QString text = item->text();
        
        if(text !=  theDialog->theLayerEditor->controller()->getEmptySlotString())
        {
           text.truncate(text.find(':'));
           ossimId id(text.toInt());
           
           currentObj = dataManager->getObject(id);
           if(currentObj)
           {
              objList.push_back(currentObj);
//              parent->connectMyInputTo(idx, currentObj);
           }
           else
           {
              okToChangeInputs = false;
           }
        }
     }
  }
  if(okToChangeInputs)
  {
     parent->disconnectAllInputs();
     parent->connectMyInputTo(objList);
  }
  bool bumpShadeDone = false;
  if(PTR_CAST(ossimBlendMosaic, theCombiner))
  {
     applyBlendChanges(false);
  }
  else if(PTR_CAST(ossimBumpShadeTileSource, theCombiner))
  {
    bumpShadeDone = true;
    applyHillshadeChanges(false);
  }
  ossimImageToPlaneNormalFilter* filter = NULL;

  if(bumpShadeDone)
    {
      filter = (ossimImageToPlaneNormalFilter*)theCombiner->findObjectOfType("ossimImageToPlaneNormalFilter",
									   ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT,
									   true);
    }
   if(fullRefreshFlag)
     {
       if(filter)
	 {
	   ossimQtDisplayUtility:: refreshAllOutputs(PTR_CAST(ossimConnectableObject, filter->getOwner()));
	 }
       else
	 {
	   ossimQtDisplayUtility:: refreshAllOutputs(parent);
	 }
     }
   else
     {
       if(filter)
	 {
	   ossimQtDisplayUtility:: flushAllOutputs(PTR_CAST(ossimConnectableObject, filter->getOwner()));
	 }
       else
	 {
	   ossimQtDisplayUtility:: flushAllOutputs(parent);
	 }
     }
}

void ossimQtCombinerEditorDialogController::inputsChanged()
{
   ossimConnectableObject* parent = theParent;
   if(!theParent) parent = theCombiner;
   if(theDialog->theAutoApplyCheckBox->isOn())
   {
      // we will only do a full refresh if bounding rect potentially change
      // this will only occur if inputs are removed or added
      //
      if(theDialog->theLayerEditor->theCurrentInputLayersListBox->count() == parent->getNumberOfInputs())
      {
         applyChanges(false);
      }
      else
      {
         applyChanges(true);
      }
   }
}

void ossimQtCombinerEditorDialogController::removingSelectedLayersFromInput(bool&)
{
   
   ossimBlendMosaic* blend = PTR_CAST(ossimBlendMosaic, theCombiner);

   if(!blend) return;
   
   Q3ListBox* listBox = theDialog->theLayerEditor->theCurrentInputLayersListBox;
   if(blend&&(listBox->count() > 0))
   {
      Q3ListBoxItem* item = listBox->item((int)(listBox->count()-1));
      int idx = listBox->count()-1;
      bool selectionFound = false;
      while(item)
      {
         if(item->isSelected())
         {
            theDialog->theInputWeightListBox->removeItem(idx);
            selectionFound = true;
         }
         --idx;
         item=item->prev();
      }
   }
}

void ossimQtCombinerEditorDialogController::addingSelectedLayersToInput(bool& /*canAddFlag*/)
{
   ossimBlendMosaic* blend = PTR_CAST(ossimBlendMosaic, theCombiner);

   if(!blend) return;
   
   Q3ListBox* listBox = theDialog->theLayerEditor->theAvailableLayersListBox;
   if(blend&&(listBox->count() > 0))
   {
      Q3ListBoxItem* item = listBox->item(0);
      int idx = 0;
      int numberOfSelections = 0;
      double weight = 0.0;
      
      while(item)
      {
         if(item->isSelected())
         {
            ++numberOfSelections;
         }
         ++idx;
         item=item->next();
      }
      if(numberOfSelections > 0)
      {
         weight = 1.0;
         for(int i = 0; i < numberOfSelections; ++i)
         {
            theDialog->theInputWeightListBox->insertItem(QString().setNum(weight,'f',15));
         }
      }
   }
}

void ossimQtCombinerEditorDialogController::setAvailableAndInputs()
{
   ossimConnectableObject* obj = theParent;
   if(!obj) obj = theCombiner;
   
   if(!obj) return;
   
   if(obj->getInputListIsFixedFlag())
   {
      theDialog->theLayerEditor->controller()->setMaxNumberOfInputs(obj->getNumberOfInputs());
   }
   else
   {
      theDialog->theLayerEditor->controller()->setMaxNumberOfInputs(-1);
   }
   
   ossimQtGetDataManagerEvent event;
   ossimQtApplicationUtility::sendEventToRoot(theDialog,
                                              &event);
   
   
   ossimDataManager* dataManager = event.getDataManager();
   
   if(dataManager)
   {
      map<ossim_int32, ossim_int32> theUniqueIdMap;
      
      for(int idx = 0; idx < (int)(obj->getNumberOfInputs()); ++idx)
      {
         ossimConnectableObject* inputObj = obj->getInput(idx);
         if(inputObj)
         {
            ossimString text = ossimString::toString(inputObj->getId().getId());
            text += (": " + inputObj->getDescription());
            theDialog->theLayerEditor->theCurrentInputLayersListBox->insertItem(text.c_str());
            theUniqueIdMap.insert(make_pair((ossim_int32)inputObj->getId().getId(),
                                            (ossim_int32)inputObj->getId().getId()));
         }
         else if(obj->getInputListIsFixedFlag() == true)
         {
            theDialog->theLayerEditor->theCurrentInputLayersListBox->insertItem(theDialog->theLayerEditor->controller()->getEmptySlotString());
         }
      }
      
      ossimConnectableObject* current = dataManager->getFirstObject();
      while(current)
      {
         ossimImageChain* chain = PTR_CAST(ossimImageChain, current);
         if(chain)
         {
            if(chain->getLastObject() != obj)
            {
               if(theUniqueIdMap.find((ossim_int32)(chain->getId().getId())) == theUniqueIdMap.end())
               {
                  ossimString text = ossimString::toString(chain->getId().getId());
                  text += (": " + chain->getDescription());
                  theDialog->theLayerEditor->theAvailableLayersListBox->insertItem(text.c_str());
               }
            }
         }
         current = dataManager->getNextObject();
      }
   }
}

void ossimQtCombinerEditorDialogController::setAdditionalInputs()
{
   int stackId = getStackId();
   
   theDialog->theInputWeightListBox->clear();
   if(stackId < 0)
   {
      theDialog->theWidgetStack->hide();
   }
   else
   {
      
      theDialog->theWidgetStack->raiseWidget(stackId);
      theDialog->theWidgetStack->show();
      
      switch(stackId)
      {
         case combinerEditorStackId_BLEND:
         {
            ossimBlendMosaic* blend = PTR_CAST(ossimBlendMosaic, theCombiner);
            for(int idx = 0; idx < (int)blend->getNumberOfInputs();++idx)
            {
               theDialog->theInputWeightListBox->insertItem(QString().setNum(blend->getWeight(idx),'f',15));
            }
            theDialog->theInputWeightListBox->setSelected(theDialog->theInputWeightListBox->topItem(), true);
            theDialog->theBlendWeightSlider->setValue((int)(theDialog->theBlendWeightSlider->maxValue()*blend->getWeight(0)));
            break;
         }
         case combinerEditorStackId_HILLSHADE:
         {
            ossimBumpShadeTileSource* hillshade = PTR_CAST(ossimBumpShadeTileSource, theCombiner);
            if(hillshade)
            {
               theDialog->theElevationAngleInput->setText(ossimString::toString(hillshade->getElevationAngle()).c_str());
               theDialog->theAzimuthAngleInput->setText(ossimString::toString(hillshade->getAzimuthAngle()).c_str());
               ossimImageToPlaneNormalFilter* filter = 
                  (ossimImageToPlaneNormalFilter*)hillshade->findObjectOfType("ossimImageToPlaneNormalFilter",
                                                                              ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT,
                                                                              true);
               if(filter)
               {
                  theDialog->theSmoothnessFactorInput->setText(ossimString::toString(filter->getSmoothnessFactor(), 6).c_str());
               }
               
            }
            break;
         }
      }
   }
}

void ossimQtCombinerEditorDialogController::applyHillshadeChanges(bool repaint)
{
   ossimBumpShadeTileSource* hillshade = PTR_CAST(ossimBumpShadeTileSource, theCombiner);
   if(hillshade)
   {
     ossimImageToPlaneNormalFilter* filter = 
       (ossimImageToPlaneNormalFilter*)hillshade->findObjectOfType("ossimImageToPlaneNormalFilter",
								   ossimConnectableObject::CONNECTABLE_DIRECTION_INPUT,
								   true);
     double  smoothnessFactor = ossimString(theDialog->theSmoothnessFactorInput->text().ascii()).toDouble();
      double azimuthAngle = ossimString((theDialog->theAzimuthAngleInput->text().ascii())).toDouble();
      double elevAngle    = ossimString((theDialog->theElevationAngleInput->text().ascii())).toDouble();
      
     if(filter)
       {
	 filter->setSmoothnessFactor(smoothnessFactor);
       }
      hillshade->setAzimuthAngle(azimuthAngle);
      hillshade->setElevationAngle(elevAngle);
      ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theCombiner->getOwner());
      if(!obj)
	{
	  obj = theCombiner;
	}
      if(filter)
	{
	  ossimConnectableObject* obj2 = PTR_CAST(ossimConnectableObject, filter->getOwner());
	  ossimQtDisplayUtility::flushAllOutputs(filter, false);
	  ossimQtDisplayUtility::flushAllOutputs(obj2, false);
	}
      ossimQtDisplayUtility::flushAllOutputs(obj, repaint);
   }
}

void ossimQtCombinerEditorDialogController::applyBlendChanges(bool repaint)
{
  ossimBlendMosaic* blend = PTR_CAST(ossimBlendMosaic, theCombiner);
  if(blend)
    {
      int currentIdx = 0;
      Q3ListBoxItem* current = theDialog->theInputWeightListBox->firstItem();
      while(current)
	{
	  blend->setWeight(currentIdx, 
			   current->text().toDouble());
	  current = current->next();
	  ++currentIdx;
	}
      ossimConnectableObject* obj = PTR_CAST(ossimConnectableObject, theCombiner->getOwner());
      if(!obj)
	{
	  obj = theCombiner;
	}
      ossimQtDisplayUtility::flushAllOutputs(obj, repaint);
    }
}

int ossimQtCombinerEditorDialogController::getStackId()const
{
   if(PTR_CAST(ossimBlendMosaic, theCombiner))
   {
      return (int)combinerEditorStackId_BLEND;
   }
   else if(PTR_CAST(ossimBumpShadeTileSource, theCombiner))
   {
      return (int)combinerEditorStackId_HILLSHADE;
   }

   return -1;
}
