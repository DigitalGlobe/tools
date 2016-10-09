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

enum { SpyWaitTime = 8000 };

class tst_StateMachine: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void stateNames_data();
    void stateNames();
    void activeStateNames_data();
    void activeStateNames();
    void connectToFinal();
    void eventOccurred();

    void doneDotStateEvent();
};

void tst_StateMachine::stateNames_data()
{
    QTest::addColumn<QString>("scxmlFileName");
    QTest::addColumn<bool>("compressed");
    QTest::addColumn<QStringList>("expectedStates");

    QTest::newRow("stateNames-compressed") << QString(":/tst_statemachine/statenames.scxml")
                                      << true
                                      << (QStringList() << QString("a1") << QString("a2") << QString("final"));
    QTest::newRow("stateNames-notCompressed") << QString(":/tst_statemachine/statenames.scxml")
                                      << false
                                      << (QStringList() << QString("a") << QString("a1") << QString("a2") << QString("b") << QString("final") << QString("top"));
    QTest::newRow("stateNamesNested-compressed") << QString(":/tst_statemachine/statenamesnested.scxml")
                                      << true
                                      << (QStringList() << QString("a") << QString("b"));
    QTest::newRow("stateNamesNested-notCompressed") << QString(":/tst_statemachine/statenamesnested.scxml")
                                      << false
                                      << (QStringList() << QString("a") << QString("b") << QString("super_top"));

    QTest::newRow("ids1") << QString(":/tst_statemachine/ids1.scxml")
                          << false
                          << (QStringList() << QString("_") << QString("foo-bar")
                              << QString("foo.bar") << QString("foo_bar"));
}

void tst_StateMachine::stateNames()
{
    QFETCH(QString, scxmlFileName);
    QFETCH(bool, compressed);
    QFETCH(QStringList, expectedStates);

    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(scxmlFileName));
    QVERIFY(!stateMachine.isNull());
    QCOMPARE(stateMachine->parseErrors().count(), 0);

    QCOMPARE(stateMachine->stateNames(compressed), expectedStates);
}

void tst_StateMachine::activeStateNames_data()
{
    QTest::addColumn<QString>("scxmlFileName");
    QTest::addColumn<bool>("compressed");
    QTest::addColumn<QStringList>("expectedStates");

    QTest::newRow("stateNames-compressed") << QString(":/tst_statemachine/statenames.scxml")
                                      << true
                                      << (QStringList() << QString("a1") << QString("final"));
    QTest::newRow("stateNames-notCompressed") << QString(":/tst_statemachine/statenames.scxml")
                                      << false
                                      << (QStringList() << QString("a") << QString("a1") << QString("b") << QString("final") << QString("top"));
    QTest::newRow("stateNamesNested-compressed") << QString(":/tst_statemachine/statenamesnested.scxml")
                                      << true
                                      << (QStringList() << QString("a")<< QString("b"));
    QTest::newRow("stateNamesNested-notCompressed") << QString(":/tst_statemachine/statenamesnested.scxml")
                                      << false
                                      << (QStringList() << QString("a") << QString("b") << QString("super_top"));
}

void tst_StateMachine::activeStateNames()
{
    QFETCH(QString, scxmlFileName);
    QFETCH(bool, compressed);
    QFETCH(QStringList, expectedStates);

    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(scxmlFileName));
    QVERIFY(!stateMachine.isNull());

    QSignalSpy stableStateSpy(stateMachine.data(), SIGNAL(reachedStableState()));

    stateMachine->start();

    stableStateSpy.wait(5000);

    QCOMPARE(stateMachine->activeStateNames(compressed), expectedStates);
}

void tst_StateMachine::connectToFinal()
{
    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(QString(":/tst_statemachine/statenames.scxml")));
    QVERIFY(!stateMachine.isNull());

    QState dummy;
    QVERIFY(stateMachine->connectToState(QString("final"), &dummy, SLOT(deleteLater())));
}

void tst_StateMachine::eventOccurred()
{
    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(QString(":/tst_statemachine/eventoccurred.scxml")));
    QVERIFY(!stateMachine.isNull());

    qRegisterMetaType<QScxmlEvent>();
    QSignalSpy finishedSpy(stateMachine.data(), SIGNAL(finished()));
    QSignalSpy eventOccurredSpy(stateMachine.data(), SIGNAL(eventOccurred(QScxmlEvent)));
    QSignalSpy externalEventOccurredSpy(stateMachine.data(), SIGNAL(externalEventOccurred(QScxmlEvent)));

    stateMachine->start();

    finishedSpy.wait(5000);

    QCOMPARE(eventOccurredSpy.count(), 4);
    QCOMPARE(qvariant_cast<QScxmlEvent>(eventOccurredSpy.at(0).at(0)).name(), QLatin1String("internalEvent2"));
    QCOMPARE(qvariant_cast<QScxmlEvent>(eventOccurredSpy.at(1).at(0)).name(), QLatin1String("externalEvent"));
    QCOMPARE(qvariant_cast<QScxmlEvent>(eventOccurredSpy.at(2).at(0)).name(), QLatin1String("timeout"));
    QCOMPARE(qvariant_cast<QScxmlEvent>(eventOccurredSpy.at(3).at(0)).name(), QLatin1String("done.state.top"));


    QCOMPARE(externalEventOccurredSpy.count(), 1);
    QCOMPARE(qvariant_cast<QScxmlEvent>(externalEventOccurredSpy.at(0).at(0)).name(), QLatin1String("externalEvent"));
}

void tst_StateMachine::doneDotStateEvent()
{
    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(QString(":/tst_statemachine/stateDotDoneEvent.scxml")));
    QVERIFY(!stateMachine.isNull());

    QSignalSpy finishedSpy(stateMachine.data(), SIGNAL(finished()));

    stateMachine->start();
    finishedSpy.wait(5000);
    QCOMPARE(finishedSpy.count(), 1);
    QCOMPARE(stateMachine->activeStateNames(true).size(), 1);
    qDebug() << stateMachine->activeStateNames(true);
    QVERIFY(stateMachine->activeStateNames(true).contains(QLatin1String("success")));
}


QTEST_MAIN(tst_StateMachine)

#include "tst_statemachine.moc"


