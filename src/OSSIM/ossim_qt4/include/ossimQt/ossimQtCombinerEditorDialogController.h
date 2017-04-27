#ifndef ossimQtCombinerEditorDialogController_HEADER
#define ossimQtCombinerEditorDialogController_HEADER
#include <QtCore/QObject>
#include <ossim/base/ossimKeywordlist.h>

class ossimQtCombinerEditorDialog;
class ossimConnectableObject;
class ossimQtCombinerEditorDialogController : public QObject
{
  Q_OBJECT
public:
  enum combinerEditorStackId
      {
         combinerEditorStackId_BLEND = 0,
         combinerEditorStackId_HILLSHADE
      };
   
  ossimQtCombinerEditorDialogController(ossimQtCombinerEditorDialog* dialog);

   virtual void initializeDialog();
   virtual void setObject(ossimConnectableObject* obj);
   
public slots:
   virtual void applyChanges(bool fullRefreshFlag=true);
   virtual void inputsChanged();
   virtual void reset();
   virtual void blendSliderReleased();
   virtual void blendSelectionHighlighted(const QString& value);
   virtual void removingSelectedLayersFromInput(bool& canRemoveFlag);
   virtual void addingSelectedLayersToInput(bool& canAddFlag);
   virtual void hillshadePropertyChange();
protected:
   ossimQtCombinerEditorDialog* theDialog;
   ossimConnectableObject*      theCombiner;
   ossimConnectableObject*      theParent;
   ossimKeywordlist             theOriginalState;
   virtual void setAvailableAndInputs();
   virtual void setAdditionalInputs();
   virtual void applyBlendChanges(bool repaint=false);
   virtual void applyHillshadeChanges(bool repaint=false);

   int getStackId()const;
};

#endif
