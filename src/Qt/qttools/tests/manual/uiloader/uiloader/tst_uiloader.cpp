/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include "uiloader.h"

#include <QtTest/QtTest>
#include <QApplication>
#include <QDir>

class uiLoaderAutotest: public QObject
{

Q_OBJECT

public slots:
    void init();

private slots:
    void imageDiffTest();

private:
    QString currentDir;

};



void uiLoaderAutotest::init()
{
    currentDir = QDir::currentPath();
#ifndef Q_OS_IRIX
    QDir::setCurrent(QString(SRCDIR) + QString("/.."));
#endif
}

void uiLoaderAutotest::imageDiffTest()
{
    //QApplication app(argc, argv);

    QString pathToProgram   = currentDir + "/tst_screenshot/tst_screenshot";

#ifdef Q_WS_MAC
    pathToProgram += ".app/Contents/MacOS/tst_screenshot";
#endif

#ifdef Q_WS_WIN
    pathToProgram += ".exe";
#endif
    uiLoader wrapper(pathToProgram);
    QString errorMessage;
    switch(wrapper.runAutoTests(&errorMessage)) {
        case uiLoader::TestRunDone:
        break;
        case uiLoader::TestConfigError:
        QVERIFY2(false, qPrintable(errorMessage));
        break;
        case uiLoader::TestNoConfig:
        QSKIP(qPrintable(errorMessage));
        break;
    }
}

QTEST_MAIN(uiLoaderAutotest)
#include "tst_uiloader.moc"
