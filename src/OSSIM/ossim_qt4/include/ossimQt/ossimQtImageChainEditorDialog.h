#ifndef OSSIMQTIMAGECHAINEDITORDIALOG_H
#define OSSIMQTIMAGECHAINEDITORDIALOG_H

#include <QtCore/QVariant>
//Added by qt3to4:
#include <Qt3Support/Q3GridLayout>
#include <Qt3Support/Q3HBoxLayout>
#include <Qt3Support/Q3VBoxLayout>
#include <QtGui/QLabel>
#include <Qt3Support/Q3Frame>

class ossimQtImageChainEditorController;
class ossimQtScrollingImageWidget;
class ossimQtDialog;

#include <Qt3Support/Q3ButtonGroup>
#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3ListBox>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <ossim/base/ossimString.h>
#include <Qt3Support/Q3MimeSourceFactory>
#include <vector>

class Ui_ossimQtImageChainEditorDialog
{
public:
    Q3VBoxLayout *vboxLayout;
    Q3HBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QLabel *theLayerTextLabel;
    QComboBox *theLayerComboBox;
    QSpacerItem *spacerItem1;
    Q3Frame *line1;
    Q3HBoxLayout *hboxLayout1;
    Q3GroupBox *theAvailableGroupBox;
    Q3GridLayout *gridLayout;
    Q3ListBox *theAvailableFiltersListBox;
    Q3ButtonGroup *theFilterActionButtonGroup;
    Q3VBoxLayout *vboxLayout1;
    QPushButton *theInsertBeforeButton;
    QPushButton *theInsertAfterPushButton;
    QPushButton *theDeletePushButton;
    QPushButton *theEditPropertiesPushButton;
    QPushButton *theClosePushButton;
    Q3GroupBox *theImageChainGroupBox;
    Q3GridLayout *gridLayout1;
    Q3ListBox *theChainListBox;

    void setupUi(QDialog *ossimQtImageChainEditorDialog)
    {
    ossimQtImageChainEditorDialog->setObjectName(QString::fromUtf8("ossimQtImageChainEditorDialog"));
    ossimQtImageChainEditorDialog->resize(QSize(529, 282).expandedTo(ossimQtImageChainEditorDialog->minimumSizeHint()));
    vboxLayout = new Q3VBoxLayout(ossimQtImageChainEditorDialog);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    hboxLayout = new Q3HBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(156, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    theLayerTextLabel = new QLabel(ossimQtImageChainEditorDialog);
    theLayerTextLabel->setObjectName(QString::fromUtf8("theLayerTextLabel"));

    hboxLayout->addWidget(theLayerTextLabel);

    theLayerComboBox = new QComboBox(ossimQtImageChainEditorDialog);
    theLayerComboBox->setObjectName(QString::fromUtf8("theLayerComboBox"));

    hboxLayout->addWidget(theLayerComboBox);

    spacerItem1 = new QSpacerItem(155, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem1);


    vboxLayout->addLayout(hboxLayout);

    line1 = new Q3Frame(ossimQtImageChainEditorDialog);
    line1->setObjectName(QString::fromUtf8("line1"));
    line1->setFrameShape(Q3Frame::HLine);
    line1->setFrameShadow(Q3Frame::Sunken);

    vboxLayout->addWidget(line1);

    hboxLayout1 = new Q3HBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theAvailableGroupBox = new Q3GroupBox(ossimQtImageChainEditorDialog);
    theAvailableGroupBox->setObjectName(QString::fromUtf8("theAvailableGroupBox"));
    theAvailableGroupBox->setAlignment(Qt::AlignHCenter);
    theAvailableGroupBox->setColumnLayout(0, Qt::Vertical);
    theAvailableGroupBox->layout()->setSpacing(6);
    theAvailableGroupBox->layout()->setMargin(11);
    gridLayout = new Q3GridLayout(theAvailableGroupBox->layout());
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    theAvailableFiltersListBox = new Q3ListBox(theAvailableGroupBox);
    theAvailableFiltersListBox->setObjectName(QString::fromUtf8("theAvailableFiltersListBox"));
    theAvailableFiltersListBox->setMinimumSize(QSize(160, 160));

    gridLayout->addWidget(theAvailableFiltersListBox, 0, 0, 1, 1);


    hboxLayout1->addWidget(theAvailableGroupBox);

    theFilterActionButtonGroup = new Q3ButtonGroup(ossimQtImageChainEditorDialog);
    theFilterActionButtonGroup->setObjectName(QString::fromUtf8("theFilterActionButtonGroup"));
    theFilterActionButtonGroup->setAlignment(Qt::AlignHCenter);
    theFilterActionButtonGroup->setColumnLayout(0, Qt::Vertical);
    theFilterActionButtonGroup->layout()->setSpacing(6);
    theFilterActionButtonGroup->layout()->setMargin(11);
    vboxLayout1 = new Q3VBoxLayout(theFilterActionButtonGroup->layout());
    vboxLayout1->setAlignment(Qt::AlignTop);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    theInsertBeforeButton = new QPushButton(theFilterActionButtonGroup);
    theInsertBeforeButton->setObjectName(QString::fromUtf8("theInsertBeforeButton"));

    vboxLayout1->addWidget(theInsertBeforeButton);

    theInsertAfterPushButton = new QPushButton(theFilterActionButtonGroup);
    theInsertAfterPushButton->setObjectName(QString::fromUtf8("theInsertAfterPushButton"));

    vboxLayout1->addWidget(theInsertAfterPushButton);

    theDeletePushButton = new QPushButton(theFilterActionButtonGroup);
    theDeletePushButton->setObjectName(QString::fromUtf8("theDeletePushButton"));

    vboxLayout1->addWidget(theDeletePushButton);

    theEditPropertiesPushButton = new QPushButton(theFilterActionButtonGroup);
    theEditPropertiesPushButton->setObjectName(QString::fromUtf8("theEditPropertiesPushButton"));

    vboxLayout1->addWidget(theEditPropertiesPushButton);

    theClosePushButton = new QPushButton(theFilterActionButtonGroup);
    theClosePushButton->setObjectName(QString::fromUtf8("theClosePushButton"));

    vboxLayout1->addWidget(theClosePushButton);


    hboxLayout1->addWidget(theFilterActionButtonGroup);

    theImageChainGroupBox = new Q3GroupBox(ossimQtImageChainEditorDialog);
    theImageChainGroupBox->setObjectName(QString::fromUtf8("theImageChainGroupBox"));
    theImageChainGroupBox->setAlignment(Qt::AlignHCenter);
    theImageChainGroupBox->setColumnLayout(0, Qt::Vertical);
    theImageChainGroupBox->layout()->setSpacing(6);
    theImageChainGroupBox->layout()->setMargin(11);
    gridLayout1 = new Q3GridLayout(theImageChainGroupBox->layout());
    gridLayout1->setAlignment(Qt::AlignTop);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    theChainListBox = new Q3ListBox(theImageChainGroupBox);
    theChainListBox->setObjectName(QString::fromUtf8("theChainListBox"));
    theChainListBox->setMinimumSize(QSize(160, 160));

    gridLayout1->addWidget(theChainListBox, 0, 0, 1, 1);


    hboxLayout1->addWidget(theImageChainGroupBox);


    vboxLayout->addLayout(hboxLayout1);

    retranslateUi(ossimQtImageChainEditorDialog);

    QMetaObject::connectSlotsByName(ossimQtImageChainEditorDialog);
    } // setupUi

    void retranslateUi(QDialog *ossimQtImageChainEditorDialog)
    {
    ossimQtImageChainEditorDialog->setWindowTitle(QApplication::translate("ossimQtImageChainEditorDialog", "Image Chain Editor", 0, QApplication::UnicodeUTF8));
    theLayerTextLabel->setText(QApplication::translate("ossimQtImageChainEditorDialog", "Image Layer:", 0, QApplication::UnicodeUTF8));
    theAvailableGroupBox->setTitle(QApplication::translate("ossimQtImageChainEditorDialog", "Available Filters", 0, QApplication::UnicodeUTF8));
    theAvailableFiltersListBox->clear();
    theAvailableFiltersListBox->insertItem(QApplication::translate("ossimQtImageChainEditorDialog", "New Item", 0, QApplication::UnicodeUTF8));
    theFilterActionButtonGroup->setTitle(QApplication::translate("ossimQtImageChainEditorDialog", "Filter Action", 0, QApplication::UnicodeUTF8));
    theInsertBeforeButton->setText(QApplication::translate("ossimQtImageChainEditorDialog", "Insert Before", 0, QApplication::UnicodeUTF8));
    theInsertAfterPushButton->setText(QApplication::translate("ossimQtImageChainEditorDialog", "Insert After", 0, QApplication::UnicodeUTF8));
    theDeletePushButton->setText(QApplication::translate("ossimQtImageChainEditorDialog", "Delete", 0, QApplication::UnicodeUTF8));
    theEditPropertiesPushButton->setText(QApplication::translate("ossimQtImageChainEditorDialog", "Edit Properties", 0, QApplication::UnicodeUTF8));
    theClosePushButton->setText(QApplication::translate("ossimQtImageChainEditorDialog", "close", 0, QApplication::UnicodeUTF8));
    theImageChainGroupBox->setTitle(QApplication::translate("ossimQtImageChainEditorDialog", "Image Chain", 0, QApplication::UnicodeUTF8));
    theChainListBox->clear();
    theChainListBox->insertItem(QApplication::translate("ossimQtImageChainEditorDialog", "New Item", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtImageChainEditorDialog);
    } // retranslateUi

};

namespace Ui {
    class ossimQtImageChainEditorDialog: public Ui_ossimQtImageChainEditorDialog {};
} // namespace Ui

class ossimQtImageChainEditorDialog : public QDialog, public Ui::ossimQtImageChainEditorDialog
{
    Q_OBJECT

public:
    ossimQtImageChainEditorDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WFlags fl = 0);
    ~ossimQtImageChainEditorDialog();

    virtual void setImageWidget( ossimQtScrollingImageWidget * widget );
    virtual void setAvailableFilterList( const std::vector<ossimString> & list );

public slots:
    virtual void closePushButton_clicked();
    virtual void insertBeforeButton_clicked();
    virtual void insertAfterPushButton_clicked();
    virtual void layerComboBox_activated( int );
    virtual void editPropertiesPushButton_clicked();
    virtual void deletePushButton_clicked();

protected slots:
    virtual void languageChange();

private:
    ossimQtImageChainEditorController* theController;

    virtual void init();
    virtual void destroy();

};

#endif // OSSIMQTIMAGECHAINEDITORDIALOG_H
