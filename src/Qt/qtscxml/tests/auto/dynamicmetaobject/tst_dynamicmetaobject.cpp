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
#include <QtScxml/qscxmlstatemachine.h>

class tst_DynamicMetaObject: public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void dynamicPartCheck_data();
    void dynamicPartCheck();
};

void tst_DynamicMetaObject::dynamicPartCheck_data()
{
    QTest::addColumn<QString>("scxmlFileName");
    QTest::addColumn<QStringList>("expectedProperties");
    QTest::addColumn<QStringList>("expectedMethods");
    QTest::addColumn<QStringList>("expectedSignals");
    QTest::addColumn<QStringList>("expectedSlots");

    { // test1.scxml

        const QStringList expectedProperties = { QLatin1String("a"), QLatin1String("b") };
        const QStringList expectedMethods = { };
        const QStringList expectedSignals = { QLatin1String("aChanged(bool)"),
                                              QLatin1String("bChanged(bool)") };
        const QStringList expectedSlots = { };

        QTest::newRow("test1") << QString(":/tst_dynamicmetaobject/test1.scxml")
                               << expectedProperties
                               << expectedMethods
                               << expectedSignals
                               << expectedSlots;
    }

    { // mediaplayer.scxml

        const QStringList expectedProperties = { QLatin1String("stopped"), QLatin1String("playing") };
        const QStringList expectedMethods = { };
        const QStringList expectedSignals = { QLatin1String("stoppedChanged(bool)"),
                                              QLatin1String("playingChanged(bool)"),
                                              QLatin1String("playbackStarted(QVariant)"),
                                              QLatin1String("playbackStopped(QVariant)") };
        const QStringList expectedSlots = { QLatin1String("tap()"),
                                            QLatin1String("tap(QVariant)") };

        QTest::newRow("mediaplayer") << QString(":/tst_dynamicmetaobject/mediaplayer.scxml")
                                     << expectedProperties
                                     << expectedMethods
                                     << expectedSignals
                                     << expectedSlots;
    }
}

void tst_DynamicMetaObject::dynamicPartCheck()
{
    QFETCH(QString, scxmlFileName);
    QFETCH(QStringList, expectedProperties);
    QFETCH(QStringList, expectedMethods);
    QFETCH(QStringList, expectedSignals);
    QFETCH(QStringList, expectedSlots);

    QScopedPointer<QScxmlStateMachine> stateMachine(QScxmlStateMachine::fromFile(scxmlFileName));
    QVERIFY(!stateMachine.isNull());
    QVERIFY(!stateMachine->parseErrors().count());

    const QMetaObject *metaObject = stateMachine->metaObject();

    QStringList dynamicProperties;
    for (int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); i++) {
        QMetaProperty metaProperty = metaObject->property(i);
        dynamicProperties << metaProperty.name();
    }

    QStringList dynamicMethods, dynamicSignals, dynamicSlots;
    for (int i = metaObject->methodOffset(); i < metaObject->methodCount(); i++) {
        QMetaMethod metaMethod = metaObject->method(i);
        switch (metaMethod.methodType()) {
        case QMetaMethod::Method:
            dynamicMethods << metaMethod.methodSignature();
            break;
        case QMetaMethod::Signal:
            dynamicSignals << metaMethod.methodSignature();
            break;
        case QMetaMethod::Slot:
            dynamicSlots << metaMethod.methodSignature();
            break;
        default:
            break;
        }
    }

    // TODO: remove sorting when we drop QSet internally
    expectedProperties.sort();
    expectedMethods.sort();
    expectedSignals.sort();
    expectedSlots.sort();
    dynamicProperties.sort();
    dynamicMethods.sort();
    dynamicSignals.sort();
    dynamicSlots.sort();

    QCOMPARE(dynamicProperties, expectedProperties);
    QCOMPARE(dynamicMethods, expectedMethods);
    QCOMPARE(dynamicSignals, expectedSignals);
    QCOMPARE(dynamicSlots, expectedSlots);
}

QTEST_MAIN(tst_DynamicMetaObject)

#include "tst_dynamicmetaobject.moc"


