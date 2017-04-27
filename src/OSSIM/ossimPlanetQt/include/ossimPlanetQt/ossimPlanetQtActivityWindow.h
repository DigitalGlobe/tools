#ifndef ossimPlanetQtActivityWindow_HEADER
#define ossimPlanetQtActivityWindow_HEADER
#include <ossimPlanetQt/ui_ossimPlanetQtActivityWindow.h>
#include <QtGui/QDialog>
#include <ossimPlanetQt/ossimPlanetQtExport.h>

class OSSIMPLANETQT_DLL ossimPlanetQtActivityWindow : public QDialog, public Ui::ossimPlanetQtActivityWindow
{
   Q_OBJECT
public:
   ossimPlanetQtActivityWindow(QWidget* parent = 0);

   
};

#endif
