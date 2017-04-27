#include <ossimQt/ossimQtIgenDialog.h>

#include <QtCore/QVariant>
#include <QtGui/QFileDialog>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtIgenController.h>
#include <ossimQt/ossimQtScrollingImageWidget.h>
#include <ossim/base/ossimImageTypeLut.h>

/*
 *  Constructs a ossimQtIgenDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
ossimQtIgenDialog::ossimQtIgenDialog(QWidget* parent, const char* name, bool modal, Qt::WFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theCloseButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
    connect(theOutputFilePushButton, SIGNAL(clicked()), this, SLOT(outputFilePushButtonClicked()));
    connect(theSaveSpecFilePushButton, SIGNAL(clicked()), this, SLOT(saveSpecFilePushButtonClicked()));
    connect(theRunIgenPushButton, SIGNAL(clicked()), this, SLOT(runIgenPushButtonClicked()));
    connect(theGsdLineEdit, SIGNAL(returnPressed()), this, SLOT(gsdLineEditReturnPressed()));
    connect(theLinesLineEdit, SIGNAL(returnPressed()), this, SLOT(linesLineEditReturnPressed()));
    connect(theSamplesLineEdit, SIGNAL(returnPressed()), this, SLOT(samplesLineEditReturnPressed()));
    connect(theOutputFileLineEdit, SIGNAL(returnPressed()), this, SLOT(outputFileLineEditReturnPressed()));
    connect(theSceneRectPushButton, SIGNAL(clicked()), this, SLOT(sceneRectPushButtonClicked()));
    connect(theEditWriterPushButton, SIGNAL(clicked()), this, SLOT(editWriterPushButtonClicked()));
    connect(theOutputTypeComboBox, SIGNAL(activated(const QString&)), this, SLOT(outputTypeComboBoxActivated(const QString&)));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtIgenDialog::~ossimQtIgenDialog()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtIgenDialog::languageChange()
{
    retranslateUi(this);
}

void ossimQtIgenDialog::init()
{   
   theController = new ossimQtIgenController(this);
}	

void ossimQtIgenDialog::destroy()
{
   if (theController)
   {
      delete theController;
      theController = NULL;
   }
   if (theImageWidget)
   {
      theImageWidget->refreshGraphics();
      theImageWidget = NULL;
   }
}

void ossimQtIgenDialog::closeButtonClicked()
{
   close(true);
}

void ossimQtIgenDialog::setImageWidget( ossimQtScrollingImageWidget * widget )
{
   theImageWidget = widget;

   if (theController)
   {
      theController->setImageWidget(widget);
      
      connect( theImageWidget,
               SIGNAL(destroyed()),
               this,
               SLOT(imageWidgetDestroyed()) );
   }
}

void ossimQtIgenDialog::setView(const ossimObject* view)
{
   if (theController)
   {
      theController->setView(view);
   }
}

void ossimQtIgenDialog::outputFilePushButtonClicked()
{
   if (theController)
   {
      QFileDialog* fd = new QFileDialog( this );
      fd->setMode(QFileDialog::AnyFile);
      fd->setDir(ossimQtIgenController::getLatestOutputFilePath().c_str());
      
      QString file;
      if (fd->exec() == QDialog::Accepted )
      {
         file = fd->selectedFile();
         theController->setOutputFile(file);
      }
      
      // Delete fd???
      delete fd;
      fd = NULL;
   }
}

void ossimQtIgenDialog::saveSpecFilePushButtonClicked()
{
   if (theController)
   {
      QFileDialog* fd = new QFileDialog( this );
      fd->setMode(QFileDialog::AnyFile);
      fd->setDir(ossimQtIgenController::getLatestOutputFilePath().c_str());
      
      QString file;
      if (fd->exec() == QDialog::Accepted )
      {
         file = fd->selectedFile();
         theController->saveSpecFile(file);
      }
      
      // Delete fd???
      delete fd;
      fd = NULL;
   }
}

void ossimQtIgenDialog::runIgenPushButtonClicked()
{
   if (theController)
   {
      theController->runIgen();
   }
}

void ossimQtIgenDialog::gsdLineEditReturnPressed()
{
   if (theController)
   {
      theController->gsdLineEditReturnPressed();
   }
}

void ossimQtIgenDialog::linesLineEditReturnPressed()
{
   if (theController)
   {
      theController->linesLineEditReturnPressed();
   }
}

void ossimQtIgenDialog::samplesLineEditReturnPressed()
{
   if (theController)
   {
      theController->samplesLineEditReturnPressed();
   }
}

void ossimQtIgenDialog::outputFileLineEditReturnPressed()
{
   if (theController)
   {
      theController->outputFileLineEditReturnPressed();
   }
}

void ossimQtIgenDialog::sceneRectPushButtonClicked()
{
   if (theController)
   {
      theController->setSceneBoundingRect();
   }
}

ossimQtIgenController* ossimQtIgenDialog::controller()
{
    return theController;
}

void ossimQtIgenDialog::imageWidgetDestroyed()
{
   theImageWidget = NULL;
}


void ossimQtIgenDialog::editWriterPushButtonClicked()
{
   if (theController)
   {
      theController->editWriter();
   }
}

void ossimQtIgenDialog::outputTypeComboBoxActivated( const QString & type)
{
    if (theController)
    {
	theController->setOutputType(type);
    }
}
