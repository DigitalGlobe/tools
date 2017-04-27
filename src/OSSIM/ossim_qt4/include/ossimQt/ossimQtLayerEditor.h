#ifndef OSSIMQTLAYEREDITOR_H
#define OSSIMQTLAYEREDITOR_H

#include <QtCore/QVariant>

class Q3ListBoxItem;
class ossimQtLayerEditorController;

#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3ListBox>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>


class Ui_ossimQtLayerEditor
{
public:
    QHBoxLayout *hboxLayout;
    Q3GroupBox *theAvailableInputsGroupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    Q3ListBox *theAvailableLayersListBox;
    QVBoxLayout *vboxLayout1;
    QSpacerItem *spacerItem;
    QPushButton *theAddLayerButton;
    QPushButton *theRemoveLayerButton;
    QSpacerItem *spacerItem1;
    Q3GroupBox *theInputsGroupBox;
    QVBoxLayout *vboxLayout2;
    Q3ListBox *theCurrentInputLayersListBox;
    QHBoxLayout *hboxLayout1;
    QPushButton *theUpButton;
    QPushButton *theDownButton;

    void setupUi(QWidget *ossimQtLayerEditor)
    {
    ossimQtLayerEditor->setObjectName(QString::fromUtf8("ossimQtLayerEditor"));
    ossimQtLayerEditor->resize(QSize(588, 230).expandedTo(ossimQtLayerEditor->minimumSizeHint()));
    hboxLayout = new QHBoxLayout(ossimQtLayerEditor);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(11);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    theAvailableInputsGroupBox = new Q3GroupBox(ossimQtLayerEditor);
    theAvailableInputsGroupBox->setObjectName(QString::fromUtf8("theAvailableInputsGroupBox"));
    theAvailableInputsGroupBox->setColumnLayout(0, Qt::Vertical);
    theAvailableInputsGroupBox->layout()->setSpacing(6);
    theAvailableInputsGroupBox->layout()->setMargin(11);
    gridLayout = new QGridLayout(theAvailableInputsGroupBox->layout());
    gridLayout->setAlignment(Qt::AlignTop);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    theAvailableLayersListBox = new Q3ListBox(theAvailableInputsGroupBox);
    theAvailableLayersListBox->setObjectName(QString::fromUtf8("theAvailableLayersListBox"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(theAvailableLayersListBox->sizePolicy().hasHeightForWidth());
    theAvailableLayersListBox->setSizePolicy(sizePolicy);

    vboxLayout->addWidget(theAvailableLayersListBox);


    gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);


    hboxLayout->addWidget(theAvailableInputsGroupBox);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem);

    theAddLayerButton = new QPushButton(ossimQtLayerEditor);
    theAddLayerButton->setObjectName(QString::fromUtf8("theAddLayerButton"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(theAddLayerButton->sizePolicy().hasHeightForWidth());
    theAddLayerButton->setSizePolicy(sizePolicy1);

    vboxLayout1->addWidget(theAddLayerButton);

    theRemoveLayerButton = new QPushButton(ossimQtLayerEditor);
    theRemoveLayerButton->setObjectName(QString::fromUtf8("theRemoveLayerButton"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(theRemoveLayerButton->sizePolicy().hasHeightForWidth());
    theRemoveLayerButton->setSizePolicy(sizePolicy2);

    vboxLayout1->addWidget(theRemoveLayerButton);

    spacerItem1 = new QSpacerItem(20, 66, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacerItem1);


    hboxLayout->addLayout(vboxLayout1);

    theInputsGroupBox = new Q3GroupBox(ossimQtLayerEditor);
    theInputsGroupBox->setObjectName(QString::fromUtf8("theInputsGroupBox"));
    theInputsGroupBox->setColumnLayout(0, Qt::Vertical);
    theInputsGroupBox->layout()->setSpacing(6);
    theInputsGroupBox->layout()->setMargin(11);
    vboxLayout2 = new QVBoxLayout(theInputsGroupBox->layout());
    vboxLayout2->setAlignment(Qt::AlignTop);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    theCurrentInputLayersListBox = new Q3ListBox(theInputsGroupBox);
    theCurrentInputLayersListBox->setObjectName(QString::fromUtf8("theCurrentInputLayersListBox"));
    QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(theCurrentInputLayersListBox->sizePolicy().hasHeightForWidth());
    theCurrentInputLayersListBox->setSizePolicy(sizePolicy3);
    theCurrentInputLayersListBox->setMinimumSize(QSize(0, 0));

    vboxLayout2->addWidget(theCurrentInputLayersListBox);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    theUpButton = new QPushButton(theInputsGroupBox);
    theUpButton->setObjectName(QString::fromUtf8("theUpButton"));

    hboxLayout1->addWidget(theUpButton);

    theDownButton = new QPushButton(theInputsGroupBox);
    theDownButton->setObjectName(QString::fromUtf8("theDownButton"));

    hboxLayout1->addWidget(theDownButton);


    vboxLayout2->addLayout(hboxLayout1);


    hboxLayout->addWidget(theInputsGroupBox);

    retranslateUi(ossimQtLayerEditor);

    QMetaObject::connectSlotsByName(ossimQtLayerEditor);
    } // setupUi

    void retranslateUi(QWidget *ossimQtLayerEditor)
    {
    ossimQtLayerEditor->setWindowTitle(QApplication::translate("ossimQtLayerEditor", "ossimQtLayerEditor", 0, QApplication::UnicodeUTF8));
    theAvailableInputsGroupBox->setTitle(QApplication::translate("ossimQtLayerEditor", "Available", 0, QApplication::UnicodeUTF8));
    theAddLayerButton->setText(QApplication::translate("ossimQtLayerEditor", "-->", 0, QApplication::UnicodeUTF8));
    theRemoveLayerButton->setText(QApplication::translate("ossimQtLayerEditor", "<--", 0, QApplication::UnicodeUTF8));
    theInputsGroupBox->setTitle(QApplication::translate("ossimQtLayerEditor", "Inputs", 0, QApplication::UnicodeUTF8));
    theUpButton->setText(QApplication::translate("ossimQtLayerEditor", "Up", 0, QApplication::UnicodeUTF8));
    theDownButton->setText(QApplication::translate("ossimQtLayerEditor", "Down", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ossimQtLayerEditor);
    } // retranslateUi


protected:
    enum IconID
    {
        image0_ID,
        unknown_ID
    };
    static QPixmap icon(IconID id)
    {
    static const char* const image0_data[] = { 
"0 0 0 1"};


    switch (id) {
        case image0_ID: return QPixmap((const char**)image0_data);
        default: return QPixmap();
    } // switch
    } // icon

};

namespace Ui {
    class ossimQtLayerEditor: public Ui_ossimQtLayerEditor {};
} // namespace Ui

class ossimQtLayerEditor : public QWidget, public Ui::ossimQtLayerEditor
{
    Q_OBJECT

public:
    ossimQtLayerEditor(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0);
    ~ossimQtLayerEditor();

    virtual ossimQtLayerEditorController * controller();

public slots:
    virtual void theUpButton_clicked();
    virtual void theDownButton_clicked();
    virtual void theAddLayerButton_clicked();
    virtual void theRemoveLayerButton_clicked();

signals:
    void addingSelectedLayersToInput(bool& canAddAvailableToInput);
    void removingSelectedLayersFromInput(bool& canRemoveLayerFromInput);
    void movingSelectedLayersUp(bool& canMoveLayersUpFlag);
    void movingSelectedLayersDown(bool& canMoveSelectedLayersDownFlag);
    void inputLayersChanged();

protected:
    ossimQtLayerEditorController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // OSSIMQTLAYEREDITOR_H
