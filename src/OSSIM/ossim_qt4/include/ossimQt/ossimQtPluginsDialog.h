//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:  Interface to plugins. 
//
// $Id$
//----------------------------------------------------------------------------
#ifndef ossimQtPluginsDialog_HEADER
#define ossimQtPluginsDialog_HEADER

#include <QtGui/QDialog>

// Forward class declarations.
class ossimPluginLibrary;
class QListWidget;
class QPushButton;
class QTextEdit;


class ossimQtPluginsDialog : public QDialog
{
   Q_OBJECT

public:

   /** public constructor */
   ossimQtPluginsDialog(QWidget* parent);

   /** virtual destructor */
   virtual ~ossimQtPluginsDialog();

public slots:

   /**
    * Slot connected to thePluginList QListWidget::itemSelectionChanged signal.
    */
   void pluginSelectionChanged();

   /**
    * Slot connected theAddButton QPushButton::pressed() signal.
    */
   void addButtonPressed();

   /**
    * Slot connected theRemoveButton QPushButton::pressed() signal.
    */
   void removeButtonPressed();

   /**
    * Slot connected theCloseButton QPushButton::pressed() signal.
    */
   void closeButtonPressed();
   
private:
   /** Hide from use copy constructor. */
   ossimQtPluginsDialog(const ossimQtPluginsDialog& obj);

   /** Hide from use assignment operator. */
   const ossimQtPluginsDialog& operator=(const ossimQtPluginsDialog& rhs);

   /**
    * Convenience method, calls  updatePluginList() then updateDescription().
    */
   void updateDialog();

   /** Updates the plugin list. */
   void updatePluginList();

   /** Updates the description to that of the selected plugin. */
   void updateDescription();

   /**
    * Gets the selected plugin from the plugin registry.
    * @return ossimPluginLibrary* or 0 if no plugins loaded.
    */
   ossimPluginLibrary* getSelectedPlugin() const;

   QListWidget*   thePluginList;
   QTextEdit*     theDescription;
   QPushButton*   theAddButton;
   QPushButton*   theRemoveButton;
   QPushButton*   theCloseButton;
};

#endif /* End of "#ifndef ossimQtPluginsDialog_HEADER" */
