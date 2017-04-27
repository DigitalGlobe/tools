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
#include "qscxmlqstates_p.h"
#include "qscxmlstatemachine_p.h"

#undef DUMP_EVENT
#ifdef DUMP_EVENT
#include <QJSEngine>
#include "qscxmlecmascriptdatamodel.h"
#endif

QT_BEGIN_NAMESPACE

static QStringList filterEmpty(const QStringList &events) {
    QStringList res;
    int oldI = 0;
    for (int i = 0; i < events.size(); ++i) {
        if (events.at(i).isEmpty()) {
            res.append(events.mid(oldI, i - oldI));
            oldI = i + 1;
        }
    }
    if (oldI > 0) {
        res.append(events.mid(oldI));
        return res;
    }
    return events;
}

QScxmlStatePrivate::QScxmlStatePrivate()
    : initInstructions(QScxmlExecutableContent::NoInstruction)
    , onEntryInstructions(QScxmlExecutableContent::NoInstruction)
    , onExitInstructions(QScxmlExecutableContent::NoInstruction)
{}

QScxmlStatePrivate::~QScxmlStatePrivate()
{
    qDeleteAll(invokableServiceFactories);
}

QScxmlState::QScxmlState(QState *parent)
    : QState(*new QScxmlStatePrivate, parent)
{}

QScxmlState::QScxmlState(QScxmlStateMachine *parent)
    : QState(*new QScxmlStatePrivate, QScxmlStateMachinePrivate::get(parent)->m_qStateMachine)
{}

QScxmlState::~QScxmlState()
{}

void QScxmlState::setAsInitialStateFor(QScxmlState *state)
{
    state->setInitialState(this);
}

void QScxmlState::setAsInitialStateFor(QScxmlStateMachine *stateMachine)
{
    QScxmlStateMachinePrivate::get(stateMachine)->m_qStateMachine->setInitialState(this);
}

QScxmlStateMachine *QScxmlState::stateMachine() const {
    return qobject_cast<QScxmlInternal::WrappedQStateMachine *>(machine())->stateMachine();
}

QString QScxmlState::stateLocation() const
{
    return QStringLiteral("State %1").arg(objectName());
}

void QScxmlState::setInitInstructions(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlState);
    d->initInstructions = instructions;
}

void QScxmlState::setOnEntryInstructions(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlState);
    d->onEntryInstructions = instructions;
}

void QScxmlState::setOnExitInstructions(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlState);
    d->onExitInstructions = instructions;
}

void QScxmlState::setInvokableServiceFactories(const QVector<QScxmlInvokableServiceFactory *> &factories)
{
    Q_D(QScxmlState);
    d->invokableServiceFactories = factories;
}

void QScxmlState::onEntry(QEvent *event)
{
    Q_D(QScxmlState);

    auto sp = QScxmlStateMachinePrivate::get(stateMachine());
    if (d->initInstructions != QScxmlExecutableContent::NoInstruction) {
        sp->m_executionEngine->execute(d->initInstructions);
        d->initInstructions = QScxmlExecutableContent::NoInstruction;
    }
    QState::onEntry(event);
    auto sm = stateMachine();
    QScxmlStateMachinePrivate::get(sm)->m_executionEngine->execute(d->onEntryInstructions);
    foreach (QScxmlInvokableServiceFactory *f, d->invokableServiceFactories) {
        if (auto service = f->invoke(stateMachine())) {
            d->invokedServices.append(service);
            d->servicesWaitingToStart.append(service);
            sp->addService(service);
        }
    }
    emit didEnter();
}

void QScxmlState::onExit(QEvent *event)
{
    Q_D(QScxmlState);

    emit willExit();
    auto sm = stateMachine();
    QScxmlStateMachinePrivate::get(sm)->m_executionEngine->execute(d->onExitInstructions);
    QState::onExit(event);
}

QScxmlFinalStatePrivate::QScxmlFinalStatePrivate()
    : doneData(QScxmlExecutableContent::NoInstruction)
    , onEntryInstructions(QScxmlExecutableContent::NoInstruction)
    , onExitInstructions(QScxmlExecutableContent::NoInstruction)
{}

QScxmlFinalStatePrivate::~QScxmlFinalStatePrivate()
{}

QScxmlFinalState::QScxmlFinalState(QState *parent)
    : QFinalState(*new QScxmlFinalStatePrivate, parent)
{}

QScxmlFinalState::QScxmlFinalState(QScxmlStateMachine *parent)
    : QFinalState(*new QScxmlFinalStatePrivate, QScxmlStateMachinePrivate::get(parent)->m_qStateMachine)
{}

QScxmlFinalState::~QScxmlFinalState()
{}

void QScxmlFinalState::setAsInitialStateFor(QScxmlState *state)
{
    state->setInitialState(this);
}

void QScxmlFinalState::setAsInitialStateFor(QScxmlStateMachine *stateMachine)
{
    QScxmlStateMachinePrivate::get(stateMachine)->m_qStateMachine->setInitialState(this);
}

QScxmlStateMachine *QScxmlFinalState::stateMachine() const {
    return qobject_cast<QScxmlInternal::WrappedQStateMachine *>(machine())->stateMachine();
}

QScxmlHistoryState::QScxmlHistoryState(QState *parent)
    : QHistoryState(parent)
{
}

QScxmlHistoryState::~QScxmlHistoryState()
{
}

void QScxmlHistoryState::setAsInitialStateFor(QScxmlState *state)
{
    state->setInitialState(this);
}

void QScxmlHistoryState::setAsInitialStateFor(QScxmlStateMachine *stateMachine)
{
    QScxmlStateMachinePrivate::get(stateMachine)->m_qStateMachine->setInitialState(this);
}

QScxmlStateMachine *QScxmlHistoryState::stateMachine() const
{
    return qobject_cast<QScxmlInternal::WrappedQStateMachine *>(machine())->stateMachine();
}

QScxmlExecutableContent::ContainerId QScxmlFinalState::doneData() const
{
    Q_D(const QScxmlFinalState);
    return d->doneData;
}

void QScxmlFinalState::setDoneData(QScxmlExecutableContent::ContainerId doneData)
{
    Q_D(QScxmlFinalState);
    d->doneData = doneData;
}

void QScxmlFinalState::setOnEntryInstructions(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlFinalState);
    d->onEntryInstructions = instructions;
}

void QScxmlFinalState::setOnExitInstructions(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlFinalState);
    d->onExitInstructions = instructions;
}

void QScxmlFinalState::onEntry(QEvent *event)
{
    Q_D(QScxmlFinalState);

    QFinalState::onEntry(event);
    auto smp = QScxmlStateMachinePrivate::get(stateMachine());
    smp->m_executionEngine->execute(d->onEntryInstructions);
}

void QScxmlFinalState::onExit(QEvent *event)
{
    Q_D(QScxmlFinalState);

    QFinalState::onExit(event);
    QScxmlStateMachinePrivate::get(stateMachine())->m_executionEngine->execute(d->onExitInstructions);
}

QScxmlBaseTransition::QScxmlBaseTransition(QState *sourceState, const QStringList &eventSelector)
    : QAbstractTransition(*new QScxmlBaseTransitionPrivate, sourceState)
{
    Q_D(QScxmlBaseTransition);
    d->eventSelector = eventSelector;
}

QScxmlBaseTransition::QScxmlBaseTransition(QScxmlBaseTransitionPrivate &dd, QState *parent,
                                           const QStringList &eventSelector)
    : QAbstractTransition(dd, parent)
{
    Q_D(QScxmlBaseTransition);
    d->eventSelector = eventSelector;
}

QScxmlBaseTransition::~QScxmlBaseTransition()
{}

QScxmlStateMachine *QScxmlBaseTransition::stateMachine() const {
    if (QScxmlInternal::WrappedQStateMachine *t = qobject_cast<QScxmlInternal::WrappedQStateMachine *>(parent()))
        return t->stateMachine();
    if (QState *s = sourceState())
        return qobject_cast<QScxmlInternal::WrappedQStateMachine *>(s->machine())->stateMachine();
    qCWarning(qscxmlLog) << "could not find Scxml::StateMachine in " << transitionLocation();
    return 0;
}

QString QScxmlBaseTransition::transitionLocation() const {
    if (QState *state = sourceState()) {
        QString stateName = state->objectName();
        int transitionIndex = state->transitions().indexOf(const_cast<QScxmlBaseTransition *>(this));
        return QStringLiteral("transition #%1 in state %2").arg(transitionIndex).arg(stateName);
    }
    return QStringLiteral("unbound transition @%1").arg(reinterpret_cast<quintptr>(this));
}

bool QScxmlBaseTransition::eventTest(QEvent *event)
{
    Q_D(QScxmlBaseTransition);

    if (d->eventSelector.isEmpty())
        return true;
    if (event->type() == QEvent::None)
        return false;
    Q_ASSERT(stateMachine());
    QString eventName = QScxmlStateMachinePrivate::get(stateMachine())->m_event.name();
    bool selected = false;
    foreach (QString eventStr, d->eventSelector) {
        if (eventStr == QStringLiteral("*")) {
            selected = true;
            break;
        }
        if (eventStr.endsWith(QStringLiteral(".*")))
            eventStr.chop(2);
        if (eventName.startsWith(eventStr)) {
            QChar nextC = QLatin1Char('.');
            if (eventName.size() > eventStr.size())
                nextC = eventName.at(eventStr.size());
            if (nextC == QLatin1Char('.') || nextC == QLatin1Char('(')) {
                selected = true;
                break;
            }
        }
    }
    return selected;
}

void QScxmlBaseTransition::onTransition(QEvent *event)
{
    Q_UNUSED(event);
}

QScxmlTransitionPrivate::QScxmlTransitionPrivate()
    : conditionalExp(QScxmlExecutableContent::NoEvaluator)
    , instructionsOnTransition(QScxmlExecutableContent::NoInstruction)
{}

QScxmlTransitionPrivate::~QScxmlTransitionPrivate()
{}

QScxmlTransition::QScxmlTransition(QState *sourceState, const QStringList &eventSelector)
    : QScxmlBaseTransition(*new QScxmlTransitionPrivate, sourceState, filterEmpty(eventSelector))
{}

QScxmlTransition::QScxmlTransition(const QStringList &eventSelector)
    : QScxmlBaseTransition(*new QScxmlTransitionPrivate, Q_NULLPTR, filterEmpty(eventSelector))
{}

QScxmlTransition::~QScxmlTransition()
{}

void QScxmlTransition::addTransitionTo(QScxmlState *state)
{
    state->addTransition(this);
}

void QScxmlTransition::addTransitionTo(QScxmlStateMachine *stateMachine)
{
    QScxmlStateMachinePrivate::get(stateMachine)->m_qStateMachine->addTransition(this);
}

bool QScxmlTransition::eventTest(QEvent *event)
{
    Q_D(QScxmlTransition);

#ifdef DUMP_EVENT
    if (auto edm = dynamic_cast<QScxmlEcmaScriptDataModel *>(stateMachine()->dataModel()))
        qCDebug(qscxmlLog) << qPrintable(edm->engine()->evaluate(QLatin1String("JSON.stringify(_event)")).toString());
#endif

    if (QScxmlBaseTransition::eventTest(event)) {
        bool ok = true;
        if (d->conditionalExp != QScxmlExecutableContent::NoEvaluator)
            return stateMachine()->dataModel()->evaluateToBool(d->conditionalExp, &ok) && ok;
        return true;
    }

    return false;
}

void QScxmlTransition::onTransition(QEvent *)
{
    Q_D(QScxmlTransition);

    QScxmlStateMachinePrivate::get(stateMachine())->m_executionEngine->execute(d->instructionsOnTransition);
}

QScxmlStateMachine *QScxmlTransition::stateMachine() const {
    // work around a bug in QStateMachine
    if (QScxmlInternal::WrappedQStateMachine *t = qobject_cast<QScxmlInternal::WrappedQStateMachine *>(sourceState()))
        return t->stateMachine();
    return qobject_cast<QScxmlInternal::WrappedQStateMachine *>(machine())->stateMachine();
}

void QScxmlTransition::setInstructionsOnTransition(QScxmlExecutableContent::ContainerId instructions)
{
    Q_D(QScxmlTransition);
    d->instructionsOnTransition = instructions;
}

void QScxmlTransition::setConditionalExpression(QScxmlExecutableContent::EvaluatorId evaluator)
{
    Q_D(QScxmlTransition);
    d->conditionalExp = evaluator;
}

QT_END_NAMESPACE
