/*!
 * \file  statuswidget.h
 *
 * \brief Declares class StatusWidget.
 */

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include "ui_statuswidget.h"

namespace QxRunner {

/*!
 * \brief The StatusWidget class presents QxRunner status information.
 *       
 * The information mainly consists of counters such as number of items,
 * number of errrors, execution progress and the like. Actual values
 * must be inserted by clients. The status widget is placed in the
 * statusbar of the main window.
 *
 * \sa \ref main_window
 */

class StatusWidget : public QWidget
{
    Q_OBJECT

public: // Operations

	/*!
	 * Constructs a status widget with the given \a parent.
	 */
    StatusWidget(QWidget* parent);

	/*!
	 * Destroys this status widget.
	 */
    ~StatusWidget();

public: // Attributes

    Ui::StatusWidget ui;

private: // Operations

	// Copy and assignment not supported.
	StatusWidget(const StatusWidget&);
	StatusWidget& operator=(const StatusWidget&);
};

} // namespace

#endif // STATUSWIDGET_H
