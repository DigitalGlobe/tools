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
#include <QXmlStreamReader>
#include <QtScxml/qscxmlparser.h>
#include <QtScxml/qscxmlstatemachine.h>

Q_DECLARE_METATYPE(QScxmlError);

class tst_Parser: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void error_data();
    void error();
};

void tst_Parser::error_data()
{
    QTest::addColumn<QString>("scxmlFileName");
    QTest::addColumn<QVector<QScxmlError> >("expectedErrors");

    QVector<QScxmlError> errors;
    QString filename;

    filename = QLatin1String(":/tst_parser/data/test1.scxml");
    errors.clear();
    errors << QScxmlError(filename, 34, 46,
                          QLatin1String("unknown state 'b' in target"));
    QTest::newRow("test1") << filename << errors;

    filename = QLatin1String(":/tst_parser/data/namespaces1.scxml");
    errors.clear();
    QTest::newRow("namespaces 1") << filename << errors;

    filename = QLatin1String(":/tst_parser/data/ids1.scxml");
    errors.clear();
    QTest::newRow("IDs 1") << filename << errors;

    filename = QLatin1String(":/tst_parser/data/ids2.scxml");
    errors.clear();
    errors << QScxmlError(filename, 33, 25,
                          QLatin1String("state name 'foo.bar' is not a valid C++ identifier in Qt mode"));
    errors << QScxmlError(filename, 34, 25,
                          QLatin1String("state name 'foo-bar' is not a valid C++ identifier in Qt mode"));
    errors << QScxmlError(filename, 36, 19,
                          QLatin1String("'1' is not a valid XML ID"));

    QTest::newRow("IDs 2") << filename << errors;

    filename = QLatin1String(":/tst_parser/data/eventnames.scxml");
    errors.clear();
    errors << QScxmlError(filename, 50, 38,
                          QLatin1String("'.invalid' is not a valid event"));
    errors << QScxmlError(filename, 51, 38,
                          QLatin1String("'invalid.' is not a valid event"));
    errors << QScxmlError(filename, 39, 36,
                          QLatin1String("'.invalid' is not a valid event"));
    errors << QScxmlError(filename, 40, 36,
                          QLatin1String("'invalid.' is not a valid event"));
    errors << QScxmlError(filename, 41, 36,
                          QLatin1String("'in valid' is not a valid event"));
    QTest::newRow("eventnames") << filename << errors;

    filename = QString(":/tst_parser/data/qtmode.scxml");
    errors.clear();
    errors << QScxmlError(filename, 35, 31,
                          QLatin1String("event name 'a' collides with a state name 'a' in Qt mode"));
    errors << QScxmlError(filename, 36, 34,
                          QLatin1String("event name 'void' is not a valid C++ identifier in Qt mode"));
    errors << QScxmlError(filename, 37, 38,
                          QLatin1String("event name 'aChanged' collides with a state name 'a' in Qt mode"));
    errors << QScxmlError(filename, 38, 38,
                          QLatin1String("event name 'finished' is not a valid Qt identifier in Qt mode"));
    errors << QScxmlError(filename, 42, 21,
                          QLatin1String("state name 'int' is not a valid C++ identifier in Qt mode"));
    errors << QScxmlError(filename, 43, 28,
                          QLatin1String("state name 'objectName' is not a valid Qt identifier in Qt mode"));
    errors << QScxmlError(filename, 45, 28,
                          QLatin1String("state name 'fooChanged' collides with a state name 'foo' in Qt mode"));
    QTest::newRow("qtmode") << filename << errors;

    filename = QString(":/tst_parser/data/scxml1.scxml");
    errors.clear();
    errors << QScxmlError(filename, 32, 36,
                          QLatin1String("Unsupported data model 'foo' in scxml"));
    errors << QScxmlError(filename, 34, 30,
                          QLatin1String("Unexpected element scxml"));
    QTest::newRow("scxml1") << filename << errors;

    filename = QString(":/tst_parser/data/scxml2.scxml");
    errors.clear();
    errors << QScxmlError(filename, 32, 34,
                          QLatin1String("Unsupperted binding type 'foo'"));
    QTest::newRow("scxml2") << filename << errors;
}

void tst_Parser::error()
{
    QFETCH(QString, scxmlFileName);
    QFETCH(QVector<QScxmlError>, expectedErrors);

    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(scxmlFileName));
    QVERIFY(!stateMachine.isNull());

    QVector<QScxmlError> errors = stateMachine->parseErrors();
    if (errors.count() != expectedErrors.count()) {
        foreach (const QScxmlError &error, errors) {
            qDebug() << error.toString();
        }
    }
    QCOMPARE(errors.count(), expectedErrors.count());

    for (int i = 0; i < errors.count(); ++i) {
        QScxmlError error = errors.at(i);
        QScxmlError expectedError = expectedErrors.at(i);
        QCOMPARE(error.fileName(), expectedError.fileName());
        QCOMPARE(error.line(), expectedError.line());
        QCOMPARE(error.column(), expectedError.column());
        QCOMPARE(error.description(), expectedError.description());
    }
}

QTEST_MAIN(tst_Parser)

#include "tst_parser.moc"


