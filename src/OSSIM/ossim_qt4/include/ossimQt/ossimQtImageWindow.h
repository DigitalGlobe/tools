#ifndef OSSIMQTIMAGEWINDOW_H
#define OSSIMQTIMAGEWINDOW_H

#include <QMainWindow>

class ossimConnectableObject;
class ossimObject;
class ossimQtImageWindowController;
class ossimQtSwipeScrollingImageWidget;
class QAction;
class QComboBox;
class QMenu;
class QPushButton;
class QToolBar;
class QToolButton;
class QVBoxLayout;
class QWidget;

class Ui_ossimQtImageWindow
{
public:
   QAction *fileSaveAsAction;
   QAction *filePrintAction;
   QAction *fileExitAction;
   QAction *editUndoAction;
   QAction *editRedoAction;
   QAction *helpContentsAction;
   QAction *helpIndexAction;
   QAction *helpAboutAction;
   QAction *editViewToolAction;
   QAction *enhancementsHistogramOperationsAction;
   QAction *enhancementsHsiAction;
   QAction *enhancementsBrightnessContrastAction;
   QAction *igenExportAction;
   QAction *enhancementsBandSelectorAction;
   QAction *swipeAddLayerAction;
   QAction *swipeRemoveLayerAction;
   QAction *swipeHorizontalAction;
   QAction *swipeVerticalAction;
   QAction *swipeBoxAction;
   QAction *swipeCircleAction;
   QAction *editImageChainAction;
   QAction *editImageInfoAction;
   QAction *editLayersAction;
   QAction *editParameterAdjustmentAction;
   QAction *editVectorSourceAction;
   QAction *enhancementsFusionAction;
   QAction *editPositionInformationAction;
   QAction *editPositionQualityAction;
   QAction *enhancementTopographicCorrection;
   QAction *resamplerPropagateAction;
   QWidget *widget;
   QVBoxLayout *vboxLayout;
   ossimQtSwipeScrollingImageWidget *theImageWidget;
   QToolBar *Toolbar;
   QToolButton *theZoomToolButton;
   QToolButton *thePanToToolButton;
   QPushButton *theFitToWindowButton;
   QPushButton *theFullResButton;
   QPushButton *thePropagateButton;
   QPushButton *theRefreshButton;
   QComboBox *theResamplerSelection;
   QMenu *fileMenu;
   QMenu *editMenu;
   QMenu *swipeMenu;
   QMenu *enhanceMenu;
   QMenu *helpMenu;

   void setupUi(QMainWindow *ossimQtImageWindow);

   void retranslateUi(QMainWindow *ossimQtImageWindow);

protected:
   enum IconID
   {
      image0_ID,
      unknown_ID
   };
   static QPixmap icon(IconID id)
   {
      static const char* const image0_data[] = {"0 0 0 1"};
       
       
      switch (id) {
         case image0_ID: return QPixmap((const char**)image0_data);
         default: return QPixmap();
      } // switch
   } // icon
};

namespace Ui {
    class ossimQtImageWindow: public Ui_ossimQtImageWindow {};
} // namespace Ui

class ossimQtImageWindow : public QMainWindow, public Ui::ossimQtImageWindow
{
    Q_OBJECT

public:
    ossimQtImageWindow(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~ossimQtImageWindow();

    virtual ossimQtSwipeScrollingImageWidget * getImageWidget();
    virtual ossimQtImageWindowController * controller();
    virtual bool event( QEvent * e );
public slots:
    virtual void enterEvent( QEvent * evt );
    virtual void leaveEvent( QEvent * evt );
    virtual void addInputSource( ossimConnectableObject * obj );
    virtual void editImageGeometryAction_activated();
    virtual void editRedoAction_activated();
    virtual void editUndoAction_activated();
    virtual void editViewToolAction_activated();
    virtual void enhancementsBandSelectorAction_activated();
    virtual void enhancementsBrightnessContrastAction_activated();
    virtual void enhancementsHistogramOperationsAction_activated();
    virtual void enhancementsHsiAction_activated();
    virtual void fileExitAction_activated();
    virtual void fileSaveAsAction_activated();
    virtual void closeEvent( QCloseEvent * event );
    virtual void resamplerActionGroup_selected( QAction * action );
    virtual void thePropagateButton_clicked();
    virtual void theFullResButton_clicked();
    virtual void theFitToWindowButton_clicked();
    virtual void thePanToTool_toggled( bool value );
    virtual void theZoomToolButton_toggled( bool value );
    virtual void resamplerPropagateAction_activated();
    virtual void swipeAddLayerAction_activated();
    virtual void swipeRemoveLayerAction_activated();
    virtual void swipeBoxAction_activated();
    virtual void swipeCircleAction_activated();
    virtual void swipeVerticalAction_activated();
    virtual void swipeHorizontalAction_activated();
    virtual void editImageChainAction_activated();
    virtual void editImageInfoAction_activated();
    virtual void editLayersAction_activated();
    virtual void editParameterAdjustmentAction_activated();
    virtual void editVectorSourceAction_activated();
    virtual void enhancementsFusionAction_activated();
    virtual void theResamplerSelection_activated( const QString & value );
    virtual void editPositionInformationActionActivated();
    virtual void editPositionQualityActionActivated();
    virtual void refreshButtonClicked();
    virtual void enhancementTopographicCorrection_activated();

protected:
    ossimQtImageWindowController* theController;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // OSSIMQTIMAGEWINDOW_H
