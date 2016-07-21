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
// Main interface between the histogram remapper dialog box and the histogram
// remapper from the image chain.
//
//*************************************************************************
// $Id: ossimQtHistogramController.h 9505 2006-09-02 20:36:55Z dburken $
#ifndef ossimQtHistogramController_HEADER
#define ossimQtHistogramController_HEADER

#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimObjectDestructingEvent.h>
#include <ossim/base/ossimFilename.h>

class ossimQtHistogramDialog;
class ossimHistogram;
class ossimHistogramRemapper;

class ossimQtHistogramController : public ossimConnectableObjectListener
{
public:

   enum
   {
      MASTER = 999999
   };
   
   ossimQtHistogramController(ossimQtHistogramDialog* dialog);
   virtual ~ossimQtHistogramController();

   virtual void objectDestructingEvent(ossimObjectDestructingEvent& event);

   void setHistogramRemapper(ossimHistogramRemapper* remapper);
   void setHistogramFile(const ossimFilename& file);
   
   void applyButtonClicked() const;
   void resetButtonClicked();
   void enableCheckBoxClicked() const;
   void bandComboBoxActivated(int item);
   void stretchModeComboBoxActivated(int item);
   void lowClipPercentReturnPressed();
   void highClipPercentReturnPressed();
   void lowClipValueReturnPressed();
   void highClipValueReturnPressed();
   void midPointReturnPressed();
   void outputMinValueReturnPressed();
   void outputMaxValueReturnPressed();

   static ossimFilename getLatestOutputFilePath();

private:
   void updateDialog();
   void setModeComboBox() const;
   void setEnableCheckBox() const;
   void setLowClipPercentage() const;
   void setHighClipPercentage() const;
   void setLowClipValue() const;
   void setHighClipValue() const;
   void setMidPoint() const;
   void setOutputMinValue() const;
   void setOutputMaxValue() const;
   void setHistogramFile() const;

   void refresh() const;

   /*!
    * Returns the current band.  If the band is the master, then the
    * enumeration MASTER is returned.
    */
   ossim_uint32 getBand() const;
   

   // Disallow copy  constructor, assignment operator.
   ossimQtHistogramController( const ossimQtHistogramController&);
   ossimQtHistogramController& operator=( const ossimQtHistogramController&);
   
   ossimQtHistogramDialog* theDialog;
   ossimHistogramRemapper* theHistogramRemapper;
   bool                    theCallBackDisabled;
   ossim_int32             theStretchModeComboBoxItem;
   ossim_int32             theBandComboBoxItem;

   // Static data member to capture the last directory.
   static ossimFilename theLatestOutputFilePath;
};

#endif /* #ifndef ossimQtHistogramRemapperController_HEADER */
