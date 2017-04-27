#include <ossimPlanetQt/ossimPlanetQtDropDownDelegate.h>
#include <QtGui/QComboBox>

ossimPlanetQtDropDownDelegate::ossimPlanetQtDropDownDelegate(QObject* parent)
   :QItemDelegate(parent)
{
}

QWidget *ossimPlanetQtDropDownDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                                     const QModelIndex &index) const
{
   QComboBox *editor = new QComboBox(parent);
   editor->insertItems(0, theChoices);
   
   return editor;
 }

void ossimPlanetQtDropDownDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QString value = index.model()->data(index, Qt::DisplayRole).toString();
   
   QComboBox *comboBox = static_cast<QComboBox*>(editor);
   comboBox->setEditText(value);
}

void ossimPlanetQtDropDownDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                                 const QModelIndex &index) const
{
   QComboBox *comboBox = static_cast<QComboBox*>(editor);
   QString value = comboBox->currentText();
   
   model->setData(index, value);
}

void ossimPlanetQtDropDownDelegate::updateEditorGeometry(QWidget *editor,
                                                         const QStyleOptionViewItem &option, const QModelIndex &index) const
{
     editor->setGeometry(option.rect);
}

void ossimPlanetQtDropDownDelegate::setChoices(const QStringList& choices)
{
   theChoices = choices;
}
