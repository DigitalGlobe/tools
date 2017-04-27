#ifndef ossimPlanetQtDropDownDelegate_HEADER
#define ossimPlanetQtDropDownDelegate_HEADER
#ifdef check
#undef check
#endif
#include <QtGui/QItemDelegate>
#include <ossimPlanetQt/ossimPlanetQtExport.h>


class OSSIMPLANETQT_DLL ossimPlanetQtDropDownDelegate : public QItemDelegate
{
public:
   ossimPlanetQtDropDownDelegate(QObject* parent=0);

   virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const;
   
   virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
   virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                     const QModelIndex &index) const;
   
   virtual void updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option, const QModelIndex &index) const;

   void setChoices(const QStringList& choices);
protected:
   QStringList theChoices;
};

#endif
