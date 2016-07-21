#include <ossimQt/ossimQtLayerEditor.h>

#include <QtCore/QVariant>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include <ossimQt/ossimQtLayerEditorController.h>

/*
 *  Constructs a ossimQtLayerEditor as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ossimQtLayerEditor::ossimQtLayerEditor(QWidget* parent, const char* name, Qt::WFlags fl)
    : QWidget(parent, name, fl)
{
    setupUi(this);


    // signals and slots connections
    connect(theUpButton, SIGNAL(clicked()), this, SLOT(theUpButton_clicked()));
    connect(theDownButton, SIGNAL(clicked()), this, SLOT(theDownButton_clicked()));
    connect(theAddLayerButton, SIGNAL(clicked()), this, SLOT(theAddLayerButton_clicked()));
    connect(theRemoveLayerButton, SIGNAL(clicked()), this, SLOT(theRemoveLayerButton_clicked()));
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ossimQtLayerEditor::~ossimQtLayerEditor()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ossimQtLayerEditor::languageChange()
{
    retranslateUi(this);
}

void ossimQtLayerEditor::init()
{
    theController = new ossimQtLayerEditorController(this);
}

void ossimQtLayerEditor::theUpButton_clicked()
{
    theController->upButtonClicked();
}


void ossimQtLayerEditor::theDownButton_clicked()
{
    theController->downButtonClicked();
}

void ossimQtLayerEditor::theAddLayerButton_clicked()
{
    theController->addLayerButtonClicked();
}


void ossimQtLayerEditor::theRemoveLayerButton_clicked()
{
    theController->removeLayerButtonClicked();
}

ossimQtLayerEditorController* ossimQtLayerEditor::controller()
{
    return theController;
}
