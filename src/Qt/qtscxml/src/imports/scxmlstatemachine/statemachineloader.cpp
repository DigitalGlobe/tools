/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtScxml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "statemachineloader.h"

#include <QtScxml/qscxmlstatemachine.h>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlInfo>
#include <QQmlFile>
#include <QBuffer>

/*!
    \qmltype StateMachineLoader
    \instantiates QScxmlStateMachineLoader
    \inqmlmodule Scxml

    \brief Dynamically loads an SCXML file and instantiates the state machine.

    \since QtScxml 5.7
 */

/*!
    \qmlsignal StateMachineLoader::filenameChanged()
    This signal is emitted when the user changes the filename.
*/

/*!
    \qmlsignal StateMachineLoader::stateMachineChanged()
*/

QScxmlStateMachineLoader::QScxmlStateMachineLoader(QObject *parent)
    : QObject(parent)
    , m_dataModel(Q_NULLPTR)
    , m_implicitDataModel(Q_NULLPTR)
    , m_stateMachine(Q_NULLPTR)
{
}

/*!
    \qmlproperty QObject StateMachineLoader::stateMachine

    The state machine instance.
 */
QT_PREPEND_NAMESPACE(QScxmlStateMachine) *QScxmlStateMachineLoader::stateMachine() const
{
    return m_stateMachine;
}

/*!
    \qmlproperty string StateMachineLoader::filename

    The name of the SCXML file to load.
 */
QUrl QScxmlStateMachineLoader::filename()
{
    return m_filename;
}

void QScxmlStateMachineLoader::setFilename(const QUrl &filename)
{
    if (!filename.isValid())
        return;

    QUrl oldFilename = m_filename;
    if (m_stateMachine) {
        delete m_stateMachine;
        m_stateMachine = Q_NULLPTR;
        m_implicitDataModel = Q_NULLPTR;
    }

    if (parse(filename)) {
        m_filename = filename;
        emit filenameChanged();
    } else {
        m_filename.clear();
        if (!oldFilename.isEmpty()) {
            emit filenameChanged();
        }
    }
}

QVariantMap QScxmlStateMachineLoader::initialValues() const
{
    return m_initialValues;
}

void QScxmlStateMachineLoader::setInitialValues(const QVariantMap &initialValues)
{
    if (initialValues != m_initialValues) {
        m_initialValues = initialValues;
        if (m_stateMachine)
            m_stateMachine->setInitialValues(initialValues);
        emit initialValuesChanged();
    }
}

QScxmlDataModel *QScxmlStateMachineLoader::dataModel() const
{
    return m_dataModel;
}

void QScxmlStateMachineLoader::setDataModel(QScxmlDataModel *dataModel)
{
    if (dataModel != m_dataModel) {
        m_dataModel = dataModel;
        if (m_stateMachine) {
            if (dataModel)
                m_stateMachine->setDataModel(dataModel);
            else
                m_stateMachine->setDataModel(m_implicitDataModel);
        }
        emit dataModelChanged();
    }
}

bool QScxmlStateMachineLoader::parse(const QUrl &filename)
{
    if (!QQmlFile::isSynchronous(filename)) {
        qmlInfo(this) << QStringLiteral("ERROR: cannot open '%1' for reading: only synchronous file access is supported.").arg(filename.fileName());
        return false;
    }
    QQmlFile scxmlFile(QQmlEngine::contextForObject(this)->engine(), filename);
    if (scxmlFile.isError()) {
        // the synchronous case can only fail when the file is not found (or not readable).
        qmlInfo(this) << QStringLiteral("ERROR: cannot open '%1' for reading.").arg(filename.fileName());
        return false;
    }

    QByteArray data(scxmlFile.dataByteArray());
    QBuffer buf(&data);
    if (!buf.open(QIODevice::ReadOnly)) {
        qmlInfo(this) << QStringLiteral("ERROR: cannot open input buffer for reading");
        return false;
    }

    m_stateMachine = QScxmlStateMachine::fromData(&buf, filename.toString());
    m_stateMachine->setParent(this);
    m_implicitDataModel = m_stateMachine->dataModel();

    if (m_stateMachine->parseErrors().isEmpty()) {
        if (m_dataModel)
            m_stateMachine->setDataModel(m_dataModel);
        m_stateMachine->setInitialValues(m_initialValues);
        emit stateMachineChanged();

        // as this is deferred any pending property updates to m_dataModel and m_initialValues
        // should still occur before start().
        QMetaObject::invokeMethod(m_stateMachine, "start", Qt::QueuedConnection);
        return true;
    } else {
        qmlInfo(this) << QStringLiteral("Something went wrong while parsing '%1':").arg(filename.fileName()) << endl;
        foreach (const QScxmlError &msg, m_stateMachine->parseErrors()) {
            qmlInfo(this) << msg.toString();
        }

        emit stateMachineChanged();
        return false;
    }
}
