//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License: MIT
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  A QTProgressDialog and an ossimProcessListener.
//
// $Id: ossimQtProgressDialog.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtProgressDialog_HEADER
#define ossimQtProgressDialog_HEADER

#include <Qt3Support/Q3ProgressDialog>
#include <ossim/base/ossimProcessListener.h>
#include <ossim/base/ossimProcessInterface.h>
/**
 * A QTProgressDialog and an ossimProcessListener.
 *
 * @note By default all three public constructors do "setAutoReset(false)"
 * so that "wasCanceled()" will return true if the cancel button is hit.
 */
class ossimQtProgressDialog : public Q3ProgressDialog,
                              public ossimProcessListener
{
  Q_OBJECT;
public:

   /** Constructor, mirrors qt's public constructor. */
   ossimQtProgressDialog(QWidget * creator = 0,
                         const char * name = 0,
                         bool modal = FALSE,
                         Qt::WFlags f = 0 );

   /** Constructor, mirrors qt's public constructor. */
   ossimQtProgressDialog(const QString& labelText,
                         const QString& cancelButtonText,
                         int totalSteps,
                         QWidget * creator = 0,
                         const char * name = 0,
                         bool modal = FALSE,
                         Qt::WFlags f = 0 );

   /**
    * Constructor, for typical usage with:
    * - text label set to your text label like "Processing file..."
    * - cancel button text = "cancel".
    * - total steps = 100
    * - modal = true
    * - progess starting at 0
    * - auto reset set to false so wasCanceled will not be cleared on cancel.
    */
   ossimQtProgressDialog(const QString& labelText);

   /** Default destructor. */
   virtual ~ossimQtProgressDialog();

   /**
    * Catches event and sets the progress bar.
    *
    * @param Event with a getPercentComplete() passed to listener
    *
    * @note This method calls QApplication::processEvents so that things
    * like the painting of the cancel button, sending canceled signal
    * are handled.
    */
   virtual void processProgressEvent(ossimProcessProgressEvent& event);

   void setProcessInterface(ossimProcessInterface* processInterface);

private:
   /** Hidden from use copy constructor. */
   ossimQtProgressDialog( const ossimQtProgressDialog&);

   /** Hidden from use assignment operator. */
   ossimQtProgressDialog& operator=( const ossimQtProgressDialog&);

   void init();

   ossimProcessInterface* theProcessInterface;

public slots:
   void cancelClicked();
};

#endif /* #ifndef #ifndef ossimQtProgressDialog_HEADER */

