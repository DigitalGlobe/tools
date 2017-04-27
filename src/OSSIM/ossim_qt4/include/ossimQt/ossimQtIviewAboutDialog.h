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
// $Id: ossimQtIviewAboutDialog.h 9508 2006-09-03 16:07:00Z dburken $
//----------------------------------------------------------------------------
#ifndef ossimQtIviewAboutDialog_HEADER
#define ossimQtIviewAboutDialog_HEADER

#include <ossimQt/ossimQtAboutDialogBase.h>

class ossimQtIviewAboutDialog : public ossimQtAboutDialogBase
{

public:

   /** public constructor */
   ossimQtIviewAboutDialog(
      QWidget* parent = 0,
      const char* name = "ossimQtIviewAboutDialog",
      bool modal = false,
      Qt::WFlags f = 0 );

   /** virtual destructor */
   virtual ~ossimQtIviewAboutDialog();

private:
   /** Hide from use copy constructor. */
   ossimQtIviewAboutDialog(const ossimQtIviewAboutDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtIviewAboutDialog& operator=(const ossimQtIviewAboutDialog& rhs);

};

#endif /* End of "#ifndef ossimQtIviewAboutDialog_HEADER" */

