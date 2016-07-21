/***************************************************************************
    qgsphotowidgetwrapper.h
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

#ifndef QGSPHOTOWIDGETWRAPPER_H
#define QGSPHOTOWIDGETWRAPPER_H

#include "qgseditorwidgetwrapper.h"
#include "qgspixmaplabel.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#ifdef WITH_QTWEBKIT
#include <QWebView>
#endif


/** \ingroup gui
 * Wraps a photo widget. Will show a picture and a file chooser to change the picture.
 *
 * Options:
 *
 * <ul>
 * <li><b>Width</b> <i>The width of the picture widget. If 0 and "Height" &gt; 0 will be determined automatically.</i></li>
 * <li><b>Height</b> <i>The height of the picture widget. If 0 and "Width" &gt; 0 will be determined automatically.</i></li>
 * </ul>
 * \note not available in Python bindings
 */

class GUI_EXPORT QgsPhotoWidgetWrapper : public QgsEditorWidgetWrapper
{
    Q_OBJECT
  public:
    explicit QgsPhotoWidgetWrapper( QgsVectorLayer* vl, int fieldIdx, QWidget* editor = nullptr, QWidget* parent = nullptr );

    // QgsEditorWidgetWrapper interface
  public:
    QVariant value() const override;
    void showIndeterminateState() override;

  protected:
    QWidget* createWidget( QWidget* parent ) override;
    void initWidget( QWidget* editor ) override;
    bool valid() const override;

  public slots:
    void setValue( const QVariant& value ) override;
    void setEnabled( bool enabled ) override;

  private slots:
    void selectFileName();
    void loadPixmap( const QString& fileName );

  private:
    void updateConstraintWidgetStatus( bool constraintValid ) override;

    //! This label is used as a container to display the picture
    QLabel* mPhotoLabel;
    //! This label is used as a container to display a picture that scales with the dialog layout.
    //! It will always point to the same label as mPhotoLabel, but may be NULL if the widget is of type QLabel.
    QgsPixmapLabel* mPhotoPixmapLabel;
#ifdef WITH_QTWEBKIT
    //! This webview is used as a container to display the picture
    QWebView* mWebView;
#endif
    //! The line edit containing the path to the picture
    QLineEdit* mLineEdit;
    //! The button to open the file chooser dialog
    QPushButton* mButton;

    void clearPicture();
};

#endif // QGSPHOTOWIDGETWRAPPER_H
