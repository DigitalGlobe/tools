/***************************************************************************
   qgsfieldexpressionwidget.h
    --------------------------------------
   Date                 : 01.04.2014
   Copyright            : (C) 2014 Denis Rouzaud
   Email                : denis.rouzaud@gmail.com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef QGSFIELDEXPRESSIONWIDGET_H
#define QGSFIELDEXPRESSIONWIDGET_H

#include <QSharedPointer>
#include <QWidget>
#include <QToolButton>
#include <QComboBox>
#include <QColor>

#include "qgsdistancearea.h"
#include "qgsfieldproxymodel.h"

class QgsMapLayer;
class QgsVectorLayer;


/** \ingroup gui
 * @brief The QgsFieldExpressionWidget class reates a widget to choose fields and edit expressions
 * It contains a combo boxto display the fields and expression and a button to open the expression dialog.
 * The combo box is editable, allowing expressions to be edited inline.
 * The validity of the expression is checked live on key press, invalid expressions are displayed in red.
 * The expression will be added to the model (and the fieldChanged signals emitted)
 * only when editing in the line edit is finished (focus lost, enter key pressed).
 */
class GUI_EXPORT QgsFieldExpressionWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QString expressionDialogTitle READ expressionDialogTitle WRITE setExpressionDialogTitle )
    Q_FLAGS( QgsFieldProxyModel::Filters )
    Q_PROPERTY( QgsFieldProxyModel::Filters filters READ filters WRITE setFilters )

  public:
    /**
     * @brief QgsFieldExpressionWidget creates a widget with a combo box to display the fields and expression and a button to open the expression dialog
     */
    explicit QgsFieldExpressionWidget( QWidget *parent = nullptr );

    //! define the title used in the expression dialog
    void setExpressionDialogTitle( const QString& title );

    //! return the title used for the expression dialog
    const QString expressionDialogTitle() { return mExpressionDialogTitle; }

    //! setFilters allows fitering according to the type of field
    void setFilters( const QgsFieldProxyModel::Filters& filters );

    void setLeftHandButtonStyle( bool isLeft );

    //! currently used filter on list of fields
    QgsFieldProxyModel::Filters filters() const { return mFieldProxyModel->filters(); }

    //! set the geometry calculator used in the expression dialog
    void setGeomCalculator( const QgsDistanceArea &da );

    /**
     * @brief currentField returns the currently selected field or expression if allowed
     * @param isExpression determines if the string returned is the name of a field or an expression
     * @param isValid determines if the expression (or field) returned is valid
     */
    QString currentField( bool *isExpression = nullptr, bool *isValid = nullptr ) const;

    /**
      * Return true if the current expression is valid
      */
    bool isValidExpression( QString *expressionError = nullptr ) const;

    /**
     * If the content is not just a simple field this method will return true.
     */
    bool isExpression() const;

    /**
      * Return the current text that is set in the expression area
      */
    QString currentText() const;

    /** Returns the currently selected field or expression. If a field is currently selected, the returned
     * value will be converted to a valid expression referencing this field (ie enclosing the field name with
     * appropriate quotations).
     * @note added in QGIS 2.14
     */
    QString asExpression() const;

    //! Returns the currently used layer
    QgsVectorLayer* layer() const;

    //! Callback function for retrieving the expression context for the expression
    typedef QgsExpressionContext( *ExpressionContextCallback )( const void* context );

    /** Register callback function for retrieving the expression context for the expression
     * @param fnGetExpressionContext call back function, will be called when the widget requires
     * the current expression context
     * @param context context for callback function
     * @note added in QGIS 2.12
     * @note not available in Python bindings
     */
    void registerGetExpressionContextCallback( ExpressionContextCallback fnGetExpressionContext, const void* context );

  signals:
    //! the signal is emitted when the currently selected field changes
    void fieldChanged( const QString& fieldName );

    //! fieldChanged signal with indication of the validity of the expression
    void fieldChanged( const QString& fieldName, bool isValid );

//    void returnPressed();

  public slots:
    //! set the layer used to display the fields and expression
    void setLayer( QgsVectorLayer* layer );

    //! convenience slot to connect QgsMapLayerComboBox layer signal
    void setLayer( QgsMapLayer* layer );

    //! sets the current row in the widget
    void setRow( int row ) { mCombo->setCurrentIndex( row ); }

    //! sets the current field or expression in the widget
    void setField( const QString &fieldName );

  protected slots:
    //! open the expression dialog to edit the current or add a new expression
    void editExpression();

    //! when expression is edited by the user in the line edit, it will be checked for validity
    void expressionEdited( const QString& expression );

    //! when expression has been edited (finished) it will be added to the model
    void expressionEditingFinished();

    void currentFieldChanged();

    /**
     * @brief updateLineEditStyle will re-style (color/font) the line edit depending on content and status
     * @param expression if expression is given it will be evaluated for the given string, otherwise it takes
     * current expression from the model
     */
    void updateLineEditStyle( const QString& expression = QString() );

    bool isExpressionValid( const QString& expressionStr );

  protected:
    void changeEvent( QEvent* event ) override;

  private slots:
    void reloadLayer();

    void beforeResetModel();
    void afterResetModel();

  private:
    QComboBox* mCombo;
    QToolButton* mButton;
    QgsFieldProxyModel* mFieldProxyModel;
    QString mExpressionDialogTitle;
    QSharedPointer<const QgsDistanceArea> mDa;
    QScopedPointer< QgsExpressionContext > mExpressionContext;
    ExpressionContextCallback mExpressionContextCallback;
    const void* mExpressionContextCallbackContext;
    QString mBackupExpression;

    friend class TestQgsFieldExpressionWidget;
};

#endif // QGSFIELDEXPRESSIONWIDGET_H
