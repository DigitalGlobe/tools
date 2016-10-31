/*!
 * \file  aboutdialog.h
 *
 * \brief Declares class AboutDialog.
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_aboutdialog.h"

namespace QxRunner {

class RunnerModel;

/*!
 * \brief The AboutDialog class displays a simple message box about
 *        QxRunner.
 *       
 *
 * The message box shows the version of QxRunner, information about
 * the model in use and the version number of Qt being used.
 */

class AboutDialog : public QDialog
{
    Q_OBJECT

public: // Operations

	/*!
	 * Constructs an about dialog with the given \a parent
	 * and \a model.
	 */
    AboutDialog(QWidget* parent, RunnerModel* model);

	/*!
	 * Destroys this about dialog.
	 */
    ~AboutDialog();

private: // Operations

	// Copy and assignment not supported.
	AboutDialog(const AboutDialog&);
	AboutDialog& operator=(const AboutDialog&);

private: // Attributes

    Ui::AboutDialog ui;
};

} // namespace

#endif // ABOUTDIALOG_H
