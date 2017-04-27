/***************************************************************************
    qgssmartgroupeditordialog.cpp
    -----------------------------
    begin                : July 2012
    copyright            : (C) 2012 by Arunmozhi
    email                : aruntheguy at gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgssmartgroupeditordialog.h"

#include "qgsstylev2.h"
#include "qgsapplication.h"

#include <QVariant>
#include <QMessageBox>

// -------------------------- //
// Condition Widget functions //
// -------------------------- //
QgsSmartGroupCondition::QgsSmartGroupCondition( int id, QWidget* parent ) : QWidget( parent )
{
  setupUi( this );

  mConditionId = id;

  mCondCombo->addItem( tr( "has the tag" ), QVariant( "tag" ) );
  mCondCombo->addItem( tr( "is a member of group" ), QVariant( "group" ) );
  mCondCombo->addItem( tr( "has a part of name matching" ), QVariant( "name" ) );
  mCondCombo->addItem( tr( "does NOT have the tag" ), QVariant( "!tag" ) );
  mCondCombo->addItem( tr( "is NOT a member of group" ), QVariant( "!group" ) );
  mCondCombo->addItem( tr( "has NO part of name matching" ), QVariant( "!name" ) );

  mRemoveBtn->setIcon( QIcon( QgsApplication::iconPath( "symbologyRemove.svg" ) ) );

  connect( mRemoveBtn, SIGNAL( clicked() ), this, SLOT( destruct() ) );
}

void QgsSmartGroupCondition::destruct()
{
  emit removed( mConditionId );
}

QString QgsSmartGroupCondition::constraint()
{
  return mCondCombo->itemData( mCondCombo->currentIndex() ).toString();
}

QString QgsSmartGroupCondition::parameter()
{
  return mCondLineEdit->text();
}

void QgsSmartGroupCondition::setConstraint( const QString& constraint )
{
  mCondCombo->setCurrentIndex( mCondCombo->findData( QVariant( constraint ) ) );
}

void QgsSmartGroupCondition::setParameter( const QString& param )
{
  mCondLineEdit->setText( param );
}

void QgsSmartGroupCondition::hideRemoveButton( bool hide )
{
  mRemoveBtn->setVisible( !hide );
}


// ------------------------ //
// Editor Dialog Functions  //
// ------------------------ //
QgsSmartGroupEditorDialog::QgsSmartGroupEditorDialog( QgsStyleV2* style, QWidget* parent )
    : QDialog( parent )
    , mStyle( style )
{
  setupUi( this );

  mCondCount = 0;

  mAndOrCombo->addItem( tr( "ALL the constraints" ), QVariant( "AND" ) );
  mAndOrCombo->addItem( tr( "any ONE of the constraints" ), QVariant( "OR" ) );

  mLayout = new QGridLayout( mConditionsBox );
  addCondition();

  connect( mAddConditionBtn, SIGNAL( clicked() ), this, SLOT( addCondition() ) );
}

QgsSmartGroupEditorDialog::~QgsSmartGroupEditorDialog()
{
}

QString QgsSmartGroupEditorDialog::smartgroupName()
{
  return mNameLineEdit->text();
}

void QgsSmartGroupEditorDialog::addCondition()
{
  // enable the remove buttons when 2nd condition is added
  if ( mConditionMap.count() == 1 )
  {
    Q_FOREACH ( QgsSmartGroupCondition *condition, mConditionMap )
    {
      condition->hideRemoveButton( false );
    }
  }
  QgsSmartGroupCondition *cond = new QgsSmartGroupCondition( mCondCount, this );
  mLayout->addWidget( cond, mCondCount, 0, 1, 1 );

  connect( cond, SIGNAL( removed( int ) ), this, SLOT( removeCondition( int ) ) );
  if ( mConditionMap.isEmpty() )
  {
    cond->hideRemoveButton( true );
  }
  mConditionMap.insert( mCondCount, cond );
  ++mCondCount;
}

void QgsSmartGroupEditorDialog::removeCondition( int id )
{
  // hide the remove button of the last condition when 2nd last is removed
  if ( mConditionMap.count() == 2 )
  {
    Q_FOREACH ( QgsSmartGroupCondition* condition, mConditionMap )
    {
      condition->hideRemoveButton( true );
    }
  }

  QgsSmartGroupCondition *cond = mConditionMap.take( id );
  delete cond;
}

QgsSmartConditionMap QgsSmartGroupEditorDialog::conditionMap()
{
  QgsSmartConditionMap conditions;

  Q_FOREACH ( QgsSmartGroupCondition* condition, mConditionMap )
  {
    conditions.insert( condition->constraint(), condition->parameter() );
  }

  return conditions;
}

QString QgsSmartGroupEditorDialog::conditionOperator()
{
  return mAndOrCombo->itemData( mAndOrCombo->currentIndex() ).toString();
}

void QgsSmartGroupEditorDialog::setConditionMap( const QgsSmartConditionMap& map )
{
  QStringList constraints;
  constraints << "tag" << "group" << "name" << "!tag" << "!group" << "!name";

  // clear any defaults
  Q_FOREACH ( int id, mConditionMap.keys() )
  {
    QgsSmartGroupCondition *cond = mConditionMap.take( id );
    delete cond;
  }

  //set the constraints
  Q_FOREACH ( const QString &constr, constraints )
  {
    QStringList params = map.values( constr );
    Q_FOREACH ( const QString &param, params )
    {
      QgsSmartGroupCondition *cond = new QgsSmartGroupCondition( mCondCount, this );
      mLayout->addWidget( cond, mCondCount, 0, 1, 1 );

      cond->setConstraint( constr );
      cond->setParameter( param );

      connect( cond, SIGNAL( removed( int ) ), this, SLOT( removeCondition( int ) ) );

      mConditionMap.insert( mCondCount, cond );
      ++mCondCount;
    }
  }
}

void QgsSmartGroupEditorDialog::setOperator( const QString& op )
{
  mAndOrCombo->setCurrentIndex( mAndOrCombo->findData( QVariant( op ) ) );
}

void QgsSmartGroupEditorDialog::setSmartgroupName( const QString& name )
{
  mNameLineEdit->setText( name );
}

void QgsSmartGroupEditorDialog::on_buttonBox_accepted()
{
  if ( mNameLineEdit->text().isEmpty() )
  {
    QMessageBox::critical( this, tr( "Invalid name" ), tr( "The smart group name field is empty. Kindly provide a name" ) );
    return;
  }
  accept();
}
