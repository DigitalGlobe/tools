//*******************************************************************
// Copyright (C) 2004 ImageLinks Inc.  All rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  David Burken
// 
// Description:
// 
// Controller class for image chain editor dialog.
//
//*************************************************************************
// $Id: ossimQtImageChainEditorController.h 9510 2006-09-03 18:13:41Z dburken $
#ifndef ossimQtImageChainEditorController_HEADER
#define ossimQtImageChainEditorController_HEADER

#include <vector>

#include <ossim/base/ossimConnectableDisplayListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimFilename.h>

class ossimQtImageChainEditorDialog;
class ossimImageChain;
class ossimImageChainRemapper;
class ossimQtScrollingImageWidget;
class ossimConnectableObject;
class ossimImageHandler;
class ossimQtPropertyDialog;
class ossimQtDialog;

class ossimQtImageChainEditorController : public ossimConnectableDisplayListener
{
public:
   
   ossimQtImageChainEditorController(ossimQtImageChainEditorDialog* dialog);
   virtual ~ossimQtImageChainEditorController();

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   /**
    * Method to listen to "theChain" for adds of objects so we can,
    * for instance, rebuild the reduced res menu if one is added.
    */
   virtual void addObjectEvent(ossimContainerEvent& event);

   /**
    * Method to listen to "theChain" for adds of objects so we can,
    * for instance, rebuild the reduced res menu if one is removed.
    */
   virtual void removeObjectEvent(ossimContainerEvent& event);

   void setImageWidget(ossimQtScrollingImageWidget* widget);

   /**
    * @param list The list of ossim filters the user wants to be available
    * to insert into the chain.
    * 
    * @note This only needs to be called is the default list from
    * "ossimImageSourceFactoryRegistry::instance()->getTypeNameList(list)"
    * is not good enough.
    */
   void setAvailableFilterList(const std::vector<ossimString>& list);

   /**
    * Methods called by dialog slots.
    */
   void layerComboBoxActivated();
   void insertBeforeButtonClicked();
   void insertAfterPushButtonClicked();
   void editPropertiesPushButtonClicked();
   void deletePushButtonClicked();

private:
   
   class FilterNameList
   {
   public:
      ossimString theClassName;
      ossimString theShortName;
   };
   
   // Disallow copy  constructor, assignment operator.
   ossimQtImageChainEditorController(const ossimQtImageChainEditorController&);
   ossimQtImageChainEditorController& operator=( const ossimQtImageChainEditorController&);

   void initializeDialog();
   void updateDialog();
   void buildLayerMenu();
   void buildImageChainMenu(ossim_uint32 layer);
   void buildAvailableFiltersMenu();
   
   ossim_int32             getLayerIndex() const;
   ossim_int32             getSelectedChainFilterIndex() const;
   ossim_int32             getSelectedAvailableFilterIndex() const;   
   ossimImageChain*        getChain() const;
   ossimImageChain*        getChain(ossim_uint32 layer) const;
   ossimConnectableObject* getSelectedChainFilter() const;
   ossimConnectableObject* getSelectedAvailableFilter() const;
   ossimImageHandler*      getImageHandler() const;
   ossimImageHandler*      getImageHandler(ossim_uint32 layer) const;

   bool isImageHandler(const ossimConnectableObject* obj) const;

   /**
    * This performs three things.
    * 1) If theImageChain was previously initialized it removes it
    *    as a listener.
    * 2) Captures the pointer.
    * 3) Adds this as a listerner to the image chain destruction.
    */
   void updateListener(ossimImageChain* ic);

   /**
    * Holds the pointer to the image chain the dialog was built from.
    * This will be the chain the dialog will listen to for a destruct
    * event.
    */
   ossimImageChain*               theImageChain;
   
   ossimQtImageChainEditorDialog* theDialog;
   ossimQtScrollingImageWidget*   theWidget;
   bool                           theCallBackDisabled;
   std::vector<FilterNameList>    theChainFilterList;
   std::vector<ossimString>       theAvailableFilterList;
};

#endif /* #ifndef ossimQtImageChainEditorRemapperController_HEADER */

