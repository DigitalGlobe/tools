/*!
 * \file  columnsdialog.h
 *
 * \brief Declares class ColumnsDialog.
 */

#ifndef COLUMNSDIALOG_H
#define COLUMNSDIALOG_H

#include "ui_columnsdialog.h"
#include "runnerwindowclient.h"

#include <QBitArray>

namespace QxRunner {

/*!
 * \brief The ColumnsDialog class allows defining the visible columns
 *        in the views.
 *
 * The user can select which columns in the views should be visible. The
 * the available columns are retrieved from the views in the main window.
 * If the dialog is accepted the selection is applied to the views and
 * the columns get reasonable column widths. If the dialog is closed with
 * \c Cancel the columns remain unchanged.
 *
 * The first column in a view is always visible therefore its visibility
 * can't be changed. For a hidden view the column visibilities also can't
 * be changed.
 */

class ColumnsDialog : public QDialog,
                      public RunnerWindowClient
{
    Q_OBJECT

public: // Operations

	/*!
	 * Constructs a columns selection dialog with the given \a parent
	 * for the views contained in the \a window.
	 */
    ColumnsDialog(QWidget* parent, RunnerWindow* window);

	/*!
	 * Destroys this columns selection dialog.
	 */
    ~ColumnsDialog();

private slots:

	/*!
	 * All selections are applied to the views but the dialog remains
	 * open.
	 */
	void apply();

	/*!
	 * All selections are applied to the views and the dialog gets
	 * closed.
	 */
	void accept();

	/*!
	 * The dialog gets closed without applying any selections.
	 */
	void reject();

private: // Operations

	/*!
	 * Disables \a item for selection and changes its color to the
	 * color for disabled GUI elelements.
	 */
	void setItemDisabled(QTreeWidgetItem* item) const;

	/*!
	 * Handles key press events for the tree view. A space mimicks
	 * a mouse click. +/- expand/collapse branches.
	 */
	bool eventFilter(QObject* obj, QEvent* event);

	// Copy and assignment not supported.
	ColumnsDialog(const ColumnsDialog&);
	ColumnsDialog& operator=(const ColumnsDialog&);

private: // Attributes

    Ui::ColumnsDialog ui;

	QTreeWidgetItem* m_runnerTopItem;
	QTreeWidgetItem* m_resultsTopItem;

	QBitArray  m_prevRunnerColumns;
	QBitArray  m_prevResultsColumns;
	QList<int> m_prevRunnerColumnSizes;
	QList<int> m_prevResultsColumnSizes;
};

} // namespace

#endif // COLUMNSDIALOG_H
