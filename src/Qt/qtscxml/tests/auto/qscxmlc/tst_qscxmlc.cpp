/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtScxml module of the Qt Toolkit.
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

#include <QtTest>
#include <QObject>
#include <QCoreApplication>
#include "qscxmlc.h"

class tst_Qscxmlc: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void parsing_data();
    void parsing();
};

void tst_Qscxmlc::parsing_data()
{
    QTest::addColumn<QString>("scxmlFileName");
    QDir dir(QLatin1String(":/tst_qscxmlc/data/"));
    foreach (const QString &entry, dir.entryList()) {
        QTest::newRow(entry.toLatin1().constData()) << dir.filePath(entry);
    }
}

void tst_Qscxmlc::parsing()
{
    QFETCH(QString, scxmlFileName);
    QVERIFY(run(QStringList() << QLatin1String("qcsxmlc") << scxmlFileName));
}

QTEST_MAIN(tst_Qscxmlc)

#include "tst_qscxmlc.moc"


