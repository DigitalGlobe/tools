//----------------------------------------------------------------------------
//
// License:   See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Dialog for building image overviews.
//
// $Id: ossimQtOverviewBuilderDialog.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtOverviewBuilderDialog_HEADER
#define ossimQtOverviewBuilderDialog_HEADER

#include <QtGui/QDialog>
#include "ossimQtExport.h"

// Forward class declarations.
class ossimQtWindow;
class ossimImageHandler;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QGroupBox;
class QHGroupBox;
class QPushButton;
class ossimTiffOverviewBuilder;
class QSpacerItem;

class OSSIMQT_DLL ossimQtOverviewBuilderDialog : public QDialog
{
   Q_OBJECT

public:

   /**
    * public constructor
    *
    * @note This does not contruct with destructive close flag so that
    * "exec()" can be called.  Same with all close calls do not use
    * destructive close(true).
    *
    * Callers should delete after using if a new was performed.
    */
   ossimQtOverviewBuilderDialog(QWidget* parent,
                                ossimImageHandler* imageHandler);
   
   /** virtual destructor */
   virtual ~ossimQtOverviewBuilderDialog();
   
public slots:

   /**
    * Slot connected to theBuildButton clicked signal.
    */
   virtual void buildClicked();

   /**
    * Slot connected to the progress dialogs cancel clicked signal.
    */
   virtual void abortClicked();

   /**
    * Slot connected to theCloseButton clicked signal.
    */
   virtual void closeClicked();
   
protected:
   
   ossimImageHandler*        theImageHandler;
   ossimTiffOverviewBuilder* theWriter;

   QVBoxLayout*     theMainVBox;

   QHBoxLayout*     theHBox1;
   QGroupBox*       theSourceImageGroupBox;         
   QLabel*          theSourceImageLabel;

   QHBoxLayout*     theHBox2;
   QSpacerItem*     theSpacer1;
   QPushButton*     theBuildButton;
   QPushButton*     theCloseButton;

private:
   /** Hide from use copy constructor. */
   ossimQtOverviewBuilderDialog(const ossimQtOverviewBuilderDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtOverviewBuilderDialog& operator=(const ossimQtOverviewBuilderDialog& rhs);
};

#endif /* End of "#ifndef ossimQtOverviewBuilderDialog_HEADER" */
