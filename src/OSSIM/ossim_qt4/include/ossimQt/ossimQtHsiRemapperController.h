//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc. 
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
// Author:  David Burken <dburken@imagelinks.com>
// 
// Description:
// Main interface between the hsi remapper dialog box and the hsi remapper
// from the image chain.
//
//*************************************************************************
// $Id: ossimQtHsiRemapperController.h 9505 2006-09-02 20:36:55Z dburken $
#ifndef ossimQtHsiRemapperController_HEADER
#define ossimQtHsiRemapperController_HEADER

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>


class ossimQtHsiRemapperDialog;
class ossimHsiRemapper;

class ossimQtHsiRemapperController : public ossimConnectableObjectListener
{
public:
   
   ossimQtHsiRemapperController(ossimQtHsiRemapperDialog* dialog);
   virtual ~ossimQtHsiRemapperController();

   enum 
   {
      RED      = 0,
      YELLOW   = 1,
      GREEN    = 2,
      CYAN     = 3,      
      BLUE     = 4,
      MAGENTA  = 5,
      ALL      = 6
   };

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   void setHsiRemapper(ossimHsiRemapper* remapper);

   void redButtonClicked();
   void yellowButtonClicked();
   void greenButtonClicked();
   void cyanButtonClicked();
   void blueButtonClicked();
   void magentaButtonClicked();
   void allButtonClicked();

   void hueOffsetValueChanged(int) const;
   void hueLowRangeValueChanged(int) const;
   void hueHighRangeValueChanged(int) const;
   void hueBlendRangeValueChanged(int) const;
   void saturationOffsetValueChanged(int) const;
   void intensityOffsetValueChanged(int) const;
   void lowIntensityClipValueChanged(int) const;
   void highIntensityClipValueChanged(int) const;
   void whiteObjectClipValueChanged(int) const;
   void enableButtonClicked() const;
   void resetGroupButtonClicked();
   void resetAllButtonClicked();
   
private:

   int  getColorGroupId() const;

   void updateSliders();
   void setHueOffsetSlider() const;
   void setHueLowRangeSlider() const;
   void setHueHighRangeSlider() const;
   void setHueBlendRangeSlider() const;
   void setSaturationOffsetSlider() const;
   void setIntensityOffsetSlider() const;
   void setIntensityLowClipSlider() const;
   void setIntensityHighClipSlider() const;
   void setWhiteObjectClipSlider() const;
   void setEnableCheckBox() const;

   void refresh() const;
   

   // Disallow copy  constructor, assignment operator.
   ossimQtHsiRemapperController( const ossimQtHsiRemapperController&);
   ossimQtHsiRemapperController& operator=( const ossimQtHsiRemapperController&);
   
   ossimQtHsiRemapperDialog* theDialog;
   ossimHsiRemapper*         theHsiRemapper;
   int                       theSelectedGroup;
   bool                      theCallBackDisabled;
};

#endif /* #ifndef ossimQtHsiRemapperController_HEADER */
