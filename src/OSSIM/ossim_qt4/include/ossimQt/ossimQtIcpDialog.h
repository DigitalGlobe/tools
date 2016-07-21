//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Image Copy interface. 
//
// $Id: ossimQtIcpDialog.h 12108 2007-12-04 17:42:05Z gpotts $
//----------------------------------------------------------------------------
#ifndef ossimQtIcpDialog_HEADER
#define ossimQtIcpDialog_HEADER

#include <QtGui/QDialog>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimImageChain.h>
#include <ossim/imaging/ossimImageFileWriter.h>

// Forward class declarations.
class ossimQtWindow;
class ossimImageHandler;
class QLabel;
class QLineEdit;
class QPushButton;
class QComboBox;

class ossimQtIcpDialog : public QDialog
{
   Q_OBJECT

public:

   /** public constructor */
   ossimQtIcpDialog(QWidget* parent, ossimImageChain* image_chain);

   /** virtual destructor */
   virtual ~ossimQtIcpDialog();

public slots:
   /**
    * Slot connected to theStartLineField returnPressed signal.
    * Performs range check and resets field if out of range.
    */
   virtual void startLinePressed();

   /**
    * Slot connected to theStopLineField returnPressed signal.
    * Performs range check and resets field if out of range.
    */
   virtual void stopLinePressed();

   /**
    * Slot connected to theStartSampleGroupBox returnPressed signal.
    * Performs range check and resets field if out of range.
    */
   virtual void startSamplePressed();

   /**
    * Slot connected to theStopSampleField returnPressed signal.
    * Performs range check and resets field if out of range.
    */
   virtual void stopSamplePressed();

   /**
    * Slot connected to theOutputImageField returnPressed signal.
    * Performs range check and resets field if out of range.
    */
   virtual void outputImagePressed();

   /**
    * Slot connected to theOutputImageButton clicked signal.
    */
   virtual void outputImageClicked();

   /**
    * Slot connected to theOutputImageButton clicked signal.
    */
   virtual void outputImageWriterMenuActivated(const QString& qs);

   /**
    * Slot connected to theEditOutputImageWriteButton cliced signal.
    */
   virtual void editOutputImageWriterClicked();

   /**
    * Slot connected to theSaveButton clicked signal.
    */
   virtual void saveClicked();

   /**
    * Slot connected to the progress dialogs cancel clicked signal.
    */
   virtual void abortClicked();

   /**
    * Slot connected to theCancelButton clicked signal.
    */
   virtual void closeClicked();
   
protected:

   /**
    * @return The image handler from "theChain".
    *
    * @note This can return null so users should check return.
    */
   ossimImageHandler* getImageHandler();

   /**
    * @return The file name from the image chain's image handler.
    */
   QString getImageFileName();

   /**
    * Performs the initial setting of start/stop line/sample.
    */
   void initializeDialog();

   /**
    * Builds the image writer menu.
    */
   void buildWriterMenu();

   /**
    * Sets the start/stop line/sample to that of "theOutputImageRect".
    */
   void setStartStopFields();

   /**
    * Updates theOutputImageRect from the start/stop line/sample fields.
    * @return true on success, false on error.
    */
   bool updateOuputRect();

   /**
    * Updates:  theOutputImageRect.ul().y
    *
    * @return true on success, false on range error.
    */
   bool updateStartLine();

   /**
    * Updates:  theOutputImageRect.lr().y
    *
    * @return true on success, false on range error.
    */   
   bool updateStopline();

   /**
    * Updates: theOutputImageRect.ul().x
    *
    * @return true on success, false on range error.
    */
   bool updateStartSample();
   
   /**
    * Updates:  theOutputImageRect.lr().x
    *
    * @return true on success, false on range error.
    */
   bool updateStopSample();

   /**
    * This will set theOutputImageField to that of the writer output filename
    * provided it is not empty and not the same as the source file.
    */
   void updateOutputFilenameFromWriter();
   
   ossimRefPtr<ossimImageChain>      theChain;
   ossimRefPtr<ossimImageFileWriter> theWriter;

   QLabel*          theSourceImageLabel;

   QLineEdit*       theStartLineField;
   QLineEdit*       theStopLineField;

   QLineEdit*       theStartSampleField;
   QLineEdit*       theStopSampleField;
   
   QLineEdit*       theOutputImageField;
   QPushButton*     theOutputImageButton;

   QComboBox*       theOutputImageWriterMenu;
   QPushButton*     theEditOutputImageWriterButton;

   QPushButton*     theSaveButton;
   QPushButton*     theCloseButton;

   ossimIrect       theSourceImageRect;
   ossimIrect       theOutputImageRect;

private:
   /** Hide from use copy constructor. */
   ossimQtIcpDialog(const ossimQtIcpDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtIcpDialog& operator=(const ossimQtIcpDialog& rhs);
};

#endif /* End of "#ifndef ossimQtIcpDialog_HEADER" */
