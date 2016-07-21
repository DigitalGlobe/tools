/***************************************************************************
    qgshiddenwidgetwrapper.h
     --------------------------------------
    Date                 : 5.1.2014
    Copyright            : (C) 2014 Matthias Kuhn
    Email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSHIDDENWIDGETWRAPPER_H
#define QGSHIDDENWIDGETWRAPPER_H

#include "qgseditorwidgetwrapper.h"

/** \ingroup gui
 * Wraps a hidden widget. Fields with this widget type will not be visible.
 * \note not available in Python bindings
 */

class GUI_EXPORT QgsHiddenWidgetWrapper : public QgsEditorWidgetWrapper
{
    Q_OBJECT
  public:
    explicit QgsHiddenWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* editor = nullptr, QWidget* parent = nullptr );

    // QgsEditorWidgetWrapper interface
  public:
    QVariant value() const override;

  protected:
    QWidget* createWidget( QWidget* parent ) override;
    void initWidget( QWidget* editor ) override;
    bool valid() const override;

  public slots:
    void setValue( const QVariant& value ) override;

  private:
    QVariant mValue;
};

#endif // QGSHIDDENWIDGETWRAPPER_H
