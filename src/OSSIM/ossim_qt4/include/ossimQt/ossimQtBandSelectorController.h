//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  
// Main interface between the band selector dialog box and the band selector
// from the image chain.
// 
//----------------------------------------------------------------------------
// $Id: ossimQtBandSelectorController.h 9515 2006-09-04 17:13:09Z dburken $
#ifndef ossimQtBandSelectorController_HEADER
#define ossimQtBandSelectorController_HEADER

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>


class ossimQtBandSelectorDialog;
class ossimBandSelector;

class ossimQtBandSelectorController : public ossimConnectableObjectListener
{
public:

   enum ModeId
   {
      UNKNOWN    = -1,
      GREY       =  0,
      TRUE_COLOR =  1,
      NBAND      =  2
   };

   ossimQtBandSelectorController(ossimQtBandSelectorDialog* dialog);
   virtual ~ossimQtBandSelectorController();

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   void clearButtonClicked()        const;
   void allButtonClicked()          const;
   void rightButtonClicked()        const;
   void leftButtonClicked()         const;
   void upButtonClicked()           const;
   void downButtonClicked()         const;
   void moveToTopButtonClicked()    const;
   void moveToBottomButtonClicked() const;
   void applyButtonClicked()        const;
   void greyButtonClicked();
   void trueButtonClicked();
   void nBandButtonClicked();
   void availableBandClicked(QListWidgetItem* item);

   void setBandSelector(ossimBandSelector* bandSelector);
   
private:
   void updateListBoxes() const;
   void setModeId();
   void setModeId(int mode);
   int  getModeId() const;
   void initListBoxes() const;
   void hideButtons() const;
   void showButtons() const;

   // Disallow copy  constructor, assignment operator.
   ossimQtBandSelectorController( const ossimQtBandSelectorController&);
   ossimQtBandSelectorController& operator=( const ossimQtBandSelectorController&);
   
   ossimQtBandSelectorDialog* theDialog;
   ossimBandSelector*         theBandSelector;
   int                        theTrueColorPosition;
   int                        theCurrentMode;
};

#endif
