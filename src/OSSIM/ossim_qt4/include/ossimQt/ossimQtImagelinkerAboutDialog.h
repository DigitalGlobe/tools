//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: ImageLinker about dialog.
//
// $Id: ossimQtImagelinkerAboutDialog.h 9508 2006-09-03 16:07:00Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimQtImagelinkerAboutDialog_HEADER
#define ossimQtImagelinkerAboutDialog_HEADER

#include <ossimQt/ossimQtAboutDialogBase.h>

class ossimQtImagelinkerAboutDialog : public ossimQtAboutDialogBase
{

public:

   /** public constructor */
   ossimQtImagelinkerAboutDialog(
      QWidget* parent = 0,
      const char* name = "ossimQtImagelinkerAboutDialog",
      bool modal = false,
      Qt::WFlags f = 0 );

   /** virtual destructor */
   virtual ~ossimQtImagelinkerAboutDialog();

private:
   /** Hide from use copy constructor. */
   ossimQtImagelinkerAboutDialog(const ossimQtImagelinkerAboutDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtImagelinkerAboutDialog& operator=(const ossimQtImagelinkerAboutDialog& rhs);

};

#endif /* End of "#ifndef ossimQtImagelinkerAboutDialog_HEADER" */

