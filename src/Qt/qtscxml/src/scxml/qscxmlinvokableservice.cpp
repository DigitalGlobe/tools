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

#include "qscxmlglobals_p.h"
#include "qscxmlinvokableservice.h"
#include "qscxmlstatemachine_p.h"

QT_BEGIN_NAMESPACE

class QScxmlInvokableService::Data
{
public:
    Data(QScxmlInvokableServiceFactory *service, QScxmlStateMachine *parent)
        : service(service)
        , parent(parent)
    {}

    QScxmlInvokableServiceFactory *service;
    QScxmlStateMachine *parent;
};

QScxmlInvokableService::QScxmlInvokableService(QScxmlInvokableServiceFactory *service,
                                               QScxmlStateMachine *parent)
    : d(new Data(service, parent))
{
    qRegisterMetaType<QScxmlInvokableService *>();
}

QScxmlInvokableService::~QScxmlInvokableService()
{
    delete d;
}

bool QScxmlInvokableService::autoforward() const
{
    return d->service->autoforward();
}

QScxmlStateMachine *QScxmlInvokableService::parentStateMachine() const
{
    return d->parent;
}

void QScxmlInvokableService::finalize()
{
    QScxmlExecutableContent::ContainerId finalize = d->service->finalizeContent();

    if (finalize != QScxmlExecutableContent::NoInstruction) {
        auto psm = parentStateMachine();
        qCDebug(qscxmlLog) << psm << "running finalize on event";
        auto smp = QScxmlStateMachinePrivate::get(psm);
        smp->m_executionEngine->execute(finalize);
    }
}

QScxmlInvokableServiceFactory *QScxmlInvokableService::service() const
{
    return d->service;
}

class QScxmlInvokableServiceFactory::Data
{
public:
    Data(QScxmlExecutableContent::StringId invokeLocation, QScxmlExecutableContent::StringId id,
         QScxmlExecutableContent::StringId idPrefix, QScxmlExecutableContent::StringId idlocation,
         const QVector<QScxmlExecutableContent::StringId> &namelist, bool autoforward,
         const QVector<QScxmlInvokableServiceFactory::Param> &params,
         QScxmlExecutableContent::ContainerId finalize)
        : invokeLocation(invokeLocation)
        , id(id)
        , idPrefix(idPrefix)
        , idlocation(idlocation)
        , namelist(namelist)
        , params(params)
        , finalize(finalize)
        , autoforward(autoforward)
    {}

    QScxmlExecutableContent::StringId invokeLocation;
    QScxmlExecutableContent::StringId id;
    QScxmlExecutableContent::StringId idPrefix;
    QScxmlExecutableContent::StringId idlocation;
    QVector<QScxmlExecutableContent::StringId> namelist;
    QVector<QScxmlInvokableServiceFactory::Param> params;
    QScxmlExecutableContent::ContainerId finalize;
    bool autoforward;
};

QScxmlInvokableServiceFactory::QScxmlInvokableServiceFactory(
        QScxmlExecutableContent::StringId invokeLocation,  QScxmlExecutableContent::StringId id,
        QScxmlExecutableContent::StringId idPrefix, QScxmlExecutableContent::StringId idlocation,
        const QVector<QScxmlExecutableContent::StringId> &namelist, bool autoforward,
        const QVector<Param> &params, QScxmlExecutableContent::ContainerId finalize)
    : d(new Data(invokeLocation, id, idPrefix, idlocation, namelist, autoforward, params, finalize))
{}

QScxmlInvokableServiceFactory::~QScxmlInvokableServiceFactory()
{
    delete d;
}

QString QScxmlInvokableServiceFactory::calculateId(QScxmlStateMachine *parent, bool *ok) const
{
    Q_ASSERT(ok);
    *ok = true;
    auto stateMachine = parent->tableData();

    if (d->id != QScxmlExecutableContent::NoString) {
        return stateMachine->string(d->id);
    }

    QString id = QScxmlStateMachine::generateSessionId(stateMachine->string(d->idPrefix));

    if (d->idlocation != QScxmlExecutableContent::NoString) {
        auto idloc = stateMachine->string(d->idlocation);
        auto ctxt = stateMachine->string(d->invokeLocation);
        *ok = parent->dataModel()->setScxmlProperty(idloc, id, ctxt);
        if (!*ok)
            return QString();
    }

    return id;
}

QVariantMap QScxmlInvokableServiceFactory::calculateData(QScxmlStateMachine *parent, bool *ok) const
{
    Q_ASSERT(ok);

    QVariantMap result;
    auto dataModel = parent->dataModel();
    auto tableData = parent->tableData();

    foreach (const Param &param, d->params) {
        auto name = tableData->string(param.name);

        if (param.expr != QScxmlExecutableContent::NoEvaluator) {
            *ok = false;
            auto v = dataModel->evaluateToVariant(param.expr, ok);
            if (!*ok)
                return QVariantMap();
            result.insert(name, v);
        } else {
            QString loc;
            if (param.location != QScxmlExecutableContent::NoString) {
                loc = tableData->string(param.location);
            }

            if (loc.isEmpty()) {
                // TODO: error message?
                *ok = false;
                return QVariantMap();
            }

            auto v = dataModel->scxmlProperty(loc);
            result.insert(name, v);
        }
    }

    foreach (QScxmlExecutableContent::StringId locid, d->namelist) {
        QString loc;
        if (locid != QScxmlExecutableContent::NoString) {
            loc = tableData->string(locid);
        }
        if (loc.isEmpty()) {
            // TODO: error message?
            *ok = false;
            return QVariantMap();
        }
        if (dataModel->hasScxmlProperty(loc)) {
            auto v = dataModel->scxmlProperty(loc);
            result.insert(loc, v);
        } else {
            *ok = false;
            return QVariantMap();
        }
    }

    return result;
}

bool QScxmlInvokableServiceFactory::autoforward() const
{
    return d->autoforward;
}

QScxmlExecutableContent::ContainerId QScxmlInvokableServiceFactory::finalizeContent() const
{
    return d->finalize;
}

QScxmlInvokableScxml::QScxmlInvokableScxml(QScxmlInvokableServiceFactory *service,
                                           QScxmlStateMachine *stateMachine,
                                           QScxmlStateMachine *parent)
    : QScxmlInvokableService(service, parent)
    , m_stateMachine(stateMachine)
{
    QScxmlStateMachinePrivate::get(stateMachine)->m_parentStateMachine = parent;
}

QScxmlInvokableScxml::~QScxmlInvokableScxml()
{
    delete m_stateMachine;
}

bool QScxmlInvokableScxml::start()
{
    qCDebug(qscxmlLog) << parentStateMachine() << "preparing to start" << m_stateMachine;

    bool ok = false;
    auto id = service()->calculateId(parentStateMachine(), &ok);
    if (!ok)
        return false;
    auto data = service()->calculateData(parentStateMachine(), &ok);
    if (!ok)
        return false;

    m_stateMachine->setSessionId(id);
    m_stateMachine->setInitialValues(data);
    if (m_stateMachine->init()) {
        qCDebug(qscxmlLog) << parentStateMachine() << "starting" << m_stateMachine;
        m_stateMachine->start();
        return true;
    }

    qCDebug(qscxmlLog) << parentStateMachine() << "failed to start" << m_stateMachine;
    return false;
}

QString QScxmlInvokableScxml::id() const
{
    return m_stateMachine->sessionId();
}

QString QScxmlInvokableScxml::name() const
{
    return m_stateMachine->name();
}

void QScxmlInvokableScxml::postEvent(QScxmlEvent *event)
{
    QScxmlStateMachinePrivate::get(m_stateMachine)->postEvent(event);
}

QScxmlStateMachine *QScxmlInvokableScxml::stateMachine() const
{
    return m_stateMachine;
}

QScxmlInvokableScxmlServiceFactory::QScxmlInvokableScxmlServiceFactory(
        QScxmlExecutableContent::StringId invokeLocation,
        QScxmlExecutableContent::StringId id,
        QScxmlExecutableContent::StringId idPrefix,
        QScxmlExecutableContent::StringId idlocation,
        const QVector<QScxmlExecutableContent::StringId> &namelist,
        bool doAutoforward,
        const QVector<QScxmlInvokableServiceFactory::Param> &params,
        QScxmlExecutableContent::ContainerId finalize)
    : QScxmlInvokableServiceFactory(invokeLocation, id, idPrefix, idlocation, namelist,
                                   doAutoforward, params, finalize)
{}

QScxmlInvokableService *QScxmlInvokableScxmlServiceFactory::finishInvoke(QScxmlStateMachine *child, QScxmlStateMachine *parent)
{
    QScxmlStateMachinePrivate::get(child)->setIsInvoked(true);
    return new QScxmlInvokableScxml(this, child, parent);
}

QT_END_NAMESPACE
