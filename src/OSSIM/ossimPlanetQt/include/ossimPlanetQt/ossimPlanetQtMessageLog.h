#ifndef ossimPlanetQtMessageLog_HEADER
#define ossimPlanetQtMessageLog_HEADER
#include <ossimPlanetQt/ui_ossimPlanetQtMessageLog.h>
#include <QtGui/QDialog>
#include <ossimPlanetQt/ossimPlanetQtExport.h>
#include <ossim/base/ossimConstants.h>
class OSSIMPLANETQT_DLL ossimPlanetQtMessageLog : public QDialog, public Ui::ossimPlanetQtMessageLog
{
   Q_OBJECT
public:
	enum Type
	{
		LOG_ERROR = 0,
		LOG_WARNING,
		LOG_INFO,
		LOG_DEBUG
	};	
   ossimPlanetQtMessageLog(QWidget* parent = 0);
	
	void addMessage(const QString& message,
						 ossimPlanetQtMessageLog::Type type);
	void showLastMessage();
	
public slots:
	void closeButtonClicked(bool checked = false);
	void clearButtonClicked(bool checked = false);
	
protected:
	void adjustItemCount();
	
	ossim_uint32 theNumberOfItems;
	ossim_uint32 theMaxNumberOfItems;
};
#endif
