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

#include "qscxmlstatemachine_p.h"
#include "qscxmlexecutablecontent_p.h"
#include "qscxmlevent_p.h"
#include "qscxmlinvokableservice.h"
#include "qscxmlqstates_p.h"
#include "qscxmldatamodel_p.h"

#include <QAbstractState>
#include <QAbstractTransition>
#include <QFile>
#include <QHash>
#include <QJSEngine>
#include <QLoggingCategory>
#include <QState>
#include <QString>
#include <QTimer>

#include <QtCore/private/qstatemachine_p.h>

#include <functional>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qscxmlLog, "qt.scxml.statemachine")
Q_LOGGING_CATEGORY(scxmlLog, "scxml.statemachine")

namespace QScxmlInternal {
class WrappedQStateMachinePrivate: public QStateMachinePrivate
{
    Q_DECLARE_PUBLIC(WrappedQStateMachine)

public:
    WrappedQStateMachinePrivate(QScxmlStateMachine *stateMachine)
        : m_stateMachine(stateMachine)
        , m_queuedEvents(Q_NULLPTR)
    {}
    ~WrappedQStateMachinePrivate()
    {
        if (m_queuedEvents) {
            foreach (const QueuedEvent &qt, *m_queuedEvents) {
                delete qt.event;
            }

            delete m_queuedEvents;
        }
    }

    int eventIdForDelayedEvent(const QString &sendId);

    QScxmlStateMachine *stateMachine() const
    { return m_stateMachine; }

    QScxmlStateMachinePrivate *stateMachinePrivate() const
    { return QScxmlStateMachinePrivate::get(stateMachine()); }

protected: // overrides for QStateMachinePrivate:
    void noMicrostep() Q_DECL_OVERRIDE;
    void processedPendingEvents(bool didChange) Q_DECL_OVERRIDE;
    void beginMacrostep() Q_DECL_OVERRIDE;
    void endMacrostep(bool didChange) Q_DECL_OVERRIDE;

    void enterStates(QEvent *event, const QList<QAbstractState*> &exitedStates_sorted,
                     const QList<QAbstractState*> &statesToEnter_sorted,
                     const QSet<QAbstractState*> &statesForDefaultEntry,
                     QHash<QAbstractState *, QVector<QPropertyAssignment> > &propertyAssignmentsForState
#  ifndef QT_NO_ANIMATION
                     , const QList<QAbstractAnimation*> &selectedAnimations
#  endif
                     ) Q_DECL_OVERRIDE;
    void exitStates(QEvent *event, const QList<QAbstractState *> &statesToExit_sorted,
                    const QHash<QAbstractState*, QVector<QPropertyAssignment> > &assignmentsForEnteredStates) Q_DECL_OVERRIDE;

    void exitInterpreter() Q_DECL_OVERRIDE;

    void emitStateFinished(QState *forState, QFinalState *guiltyState) Q_DECL_OVERRIDE;
    void startupHook() Q_DECL_OVERRIDE;

public: // fields
    QScxmlStateMachine *m_stateMachine;

    struct QueuedEvent
    {
        QueuedEvent(QEvent *event = Q_NULLPTR, QStateMachine::EventPriority priority = QStateMachine::NormalPriority)
            : event(event)
            , priority(priority)
        {}

        QEvent *event;
        QStateMachine::EventPriority priority;
    };
    QVector<QueuedEvent> *m_queuedEvents;
};

WrappedQStateMachine::WrappedQStateMachine(QScxmlStateMachine *parent)
    : QStateMachine(*new WrappedQStateMachinePrivate(parent), parent)
{}

WrappedQStateMachine::WrappedQStateMachine(WrappedQStateMachinePrivate &dd, QScxmlStateMachine *parent)
    : QStateMachine(dd, parent)
{}

QScxmlStateMachine *WrappedQStateMachine::stateMachine() const
{
    Q_D(const WrappedQStateMachine);

    return d->stateMachine();
}

QScxmlStateMachinePrivate *WrappedQStateMachine::stateMachinePrivate()
{
    Q_D(const WrappedQStateMachine);

    return d->stateMachinePrivate();
}
} // Internal namespace

/*!
 * \class QScxmlEventFilter
 * \brief The QScxmlEventFilter class is an event filter for an SCXML state machine.
 * \since 5.7
 * \inmodule QtScxml
 *
 * An event filter can be used to intercept events generated by an SCXML state machine. By default,
 * the QScxmlStateMachine will have an event filter that will intercept events that are marked as
 * external and that have the type \c qt:signal to emit signals.
 *
 * \sa QScxmlStateMachine
 */

/*!
 * Destroys the SCXML event filter.
 */
QScxmlEventFilter::~QScxmlEventFilter()
{}

/*!
 * \fn QScxmlEventFilter::handle(QScxmlEvent *event, QScxmlStateMachine *stateMachine)
 *
 * Returns \c true if the \a event should be submitted to the \a stateMachine
 * and it was not intercepted inside the event filter, \c false otherwise.
 */

/*!
 * \class QScxmlStateMachine
 * \brief The QScxmlStateMachine class provides an interface to the state machines
 * created from SCXML files.
 * \since 5.7
 * \inmodule QtScxml
 *
 * QScxmlStateMachine is an implementation of the
 * \l{SCXML Specification}{State Chart XML (SCXML)}.
 *
 * All states that are defined in the SCXML file
 * are accessible as properties of QScxmlStateMachine.
 * These properties are boolean values and indicate
 * whether the state is active or inactive.
 *
 * All external signals defined inside the SCXML file
 * that are of the \c qt:signal type, are accessible
 * as signals of QScxmlStateMachine in the \e {Qt mode}.
 * The only argument of these signals
 * is always QVariant, which is of QMap<QString, QVariant>
 * type containing the content of all the \c <param>
 * elements specified as children of a \c <send> element.
 * The name of each QScxmlStateMachine signal
 * corresponds to the value defined in the
 * \e event attribute of one \c <send> tag in the SCXML file.
 */

/*!
    \fn QScxmlStateMachine::eventOccurred(const QScxmlEvent &event)

    This signal is emitted when the SCXML event \a event occurs. This signal is
    emitted for all events.

    \sa externalEventOccurred()
*/

/*!
    \fn QScxmlStateMachine::externalEventOccurred(const QScxmlEvent &event)

    This signal is emitted for each \c <send> element in the SCXML file that
    contains the attribute \c {type="qt:signal"}. The event that occurred is
    specified by \a event.

    \sa eventOccurred()
*/

QAtomicInt QScxmlStateMachinePrivate::m_sessionIdCounter = QAtomicInt(0);


QScxmlStateMachinePrivate::QScxmlStateMachinePrivate()
    : QObjectPrivate()
    , m_sessionId(QScxmlStateMachine::generateSessionId(QStringLiteral("session-")))
    , m_isInvoked(false)
    , m_isInitialized(false)
    , m_dataModel(Q_NULLPTR)
    , m_dataBinding(QScxmlStateMachine::EarlyBinding)
    , m_executionEngine(Q_NULLPTR)
    , m_tableData(Q_NULLPTR)
    , m_qStateMachine(Q_NULLPTR)
    , m_eventFilter(Q_NULLPTR)
    , m_parentStateMachine(Q_NULLPTR)
{}

QScxmlStateMachinePrivate::~QScxmlStateMachinePrivate()
{
    qDeleteAll(m_invokedServices);
    delete m_executionEngine;
}

void QScxmlStateMachinePrivate::init()
{
    Q_Q(QScxmlStateMachine);
    m_executionEngine = new QScxmlExecutableContent::QScxmlExecutionEngine(q);
    setQStateMachine(new QScxmlInternal::WrappedQStateMachine(q));
    QObject::connect(m_qStateMachine, &QStateMachine::runningChanged,
                     q, &QScxmlStateMachine::runningChanged);
    QObject::connect(m_qStateMachine, &QStateMachine::finished,
                     q, &QScxmlStateMachine::finished);

    // The final state is also a stable state.
    QObject::connect(m_qStateMachine, &QStateMachine::finished,
                     q, &QScxmlStateMachine::reachedStableState);
}

void QScxmlStateMachinePrivate::setQStateMachine(QScxmlInternal::WrappedQStateMachine *stateMachine)
{
    m_qStateMachine = stateMachine;
}

static QAbstractState *findState(const QString &scxmlName, QStateMachine *parent)
{
    QList<QObject *> worklist;
    worklist.reserve(parent->children().size() + parent->configuration().size());
    worklist.append(parent);

    while (!worklist.isEmpty()) {
        QObject *obj = worklist.takeLast();
        if (QAbstractState *state = qobject_cast<QAbstractState *>(obj)) {
            if (state->objectName() == scxmlName)
                return state;
        }
        worklist.append(obj->children());
    }

    return Q_NULLPTR;
}

QAbstractState *QScxmlStateMachinePrivate::stateByScxmlName(const QString &scxmlName)
{
    return findState(scxmlName, m_qStateMachine);
}

QScxmlStateMachinePrivate::ParserData *QScxmlStateMachinePrivate::parserData()
{
    if (m_parserData.isNull())
        m_parserData.reset(new ParserData);
    return m_parserData.data();
}

void QScxmlStateMachinePrivate::addService(QScxmlInvokableService *service)
{
    Q_Q(QScxmlStateMachine);
    Q_ASSERT(!m_invokedServices.contains(service));
    m_invokedServices.append(service);
    q->setService(service->name(), service);
}

bool QScxmlStateMachinePrivate::removeService(QScxmlInvokableService *service)
{
    Q_Q(QScxmlStateMachine);
    Q_ASSERT(m_invokedServices.contains(service));
    if (m_invokedServices.removeOne(service)) {
        q->setService(service->name(), Q_NULLPTR);
        return true;
    }
    return false;
}

bool QScxmlStateMachinePrivate::executeInitialSetup()
{
    return m_executionEngine->execute(m_tableData->initialSetup());
}

void QScxmlStateMachinePrivate::routeEvent(QScxmlEvent *event)
{
    Q_Q(QScxmlStateMachine);

    if (!event)
        return;

    QString origin = event->origin();
    if (origin == QStringLiteral("#_parent")) {
        if (auto psm = m_parentStateMachine) {
            qCDebug(qscxmlLog) << q << "routing event" << event->name() << "from" << q->name() << "to parent" << psm->name();
                                 QScxmlStateMachinePrivate::get(psm)->postEvent(event);
        } else {
            qCDebug(qscxmlLog) << this << "is not invoked, so it cannot route a message to #_parent";
            delete event;
        }
    } else if (origin.startsWith(QStringLiteral("#_")) && origin != QStringLiteral("#_internal")) {
        // route to children
        auto originId = origin.midRef(2);
        foreach (QScxmlInvokableService *service, m_invokedServices) {
            if (service->id() == originId) {
                qCDebug(qscxmlLog) << q << "routing event" << event->name()
                                  << "from" << q->name()
                                  << "to parent" << service->id();
                service->postEvent(new QScxmlEvent(*event));
            }
        }
        delete event;
    } else {
        postEvent(event);
    }
}

void QScxmlStateMachinePrivate::postEvent(QScxmlEvent *event)
{
    Q_Q(QScxmlStateMachine);

    QStateMachine::EventPriority priority =
            event->eventType() == QScxmlEvent::ExternalEvent ? QStateMachine::NormalPriority
                                                             : QStateMachine::HighPriority;

    if (m_qStateMachine->isRunning()) {
        qCDebug(qscxmlLog) << q << "posting event" << event->name();
        m_qStateMachine->postEvent(event, priority);
    } else {
        qCDebug(qscxmlLog) << q << "queueing event" << event->name();
        m_qStateMachine->queueEvent(event, priority);
    }
}

/*!
 * \internal
 * \brief Submits an error event to the external event queue of this state machine.
 *
 * \param type The error message type, e.g. "error.execution". The type has to start with "error.".
 * \param msg A string describing the nature of the error. This is passed to the event as the
 *            errorMessage
 * \param sendid The sendid of the message causing the error, if it has one.
 */
void QScxmlStateMachinePrivate::submitError(const QString &type, const QString &msg, const QString &sendid)
{
    Q_Q(QScxmlStateMachine);
    qCDebug(qscxmlLog) << q << "had error" << type << ":" << msg;
    if (!type.startsWith(QStringLiteral("error.")))
        qCWarning(qscxmlLog) << q << "Message type of error message does not start with 'error.'!";
    q->submitEvent(QScxmlEventBuilder::errorEvent(q, type, msg, sendid));
}

/*!
 * Creates a state machine from the SCXML file specified by \a fileName.
 *
 * This method will always return a state machine. If errors occur while reading the SCXML file,
 * the state machine cannot be started. The errors can be retrieved by calling the parseErrors()
 * method.
 *
 * \sa parseErrors()
 */
QScxmlStateMachine *QScxmlStateMachine::fromFile(const QString &fileName)
{
    QFile scxmlFile(fileName);
    if (!scxmlFile.open(QIODevice::ReadOnly)) {
        auto stateMachine = new QScxmlStateMachine;
        QScxmlError err(scxmlFile.fileName(), 0, 0, QStringLiteral("cannot open for reading"));
        QScxmlStateMachinePrivate::get(stateMachine)->parserData()->m_errors.append(err);
        return stateMachine;
    }

    QScxmlStateMachine *stateMachine = fromData(&scxmlFile, fileName);
    scxmlFile.close();
    return stateMachine;
}

/*!
 * Creates a state machine by reading from the QIODevice specified by \a data.
 *
 * This method will always return a state machine. If errors occur while reading the SCXML file,
 * \a fileName, the state machine cannot be started. The errors can be retrieved by calling the
 * parseErrors() method.
 *
 * \sa parseErrors()
 */
QScxmlStateMachine *QScxmlStateMachine::fromData(QIODevice *data, const QString &fileName)
{
    QXmlStreamReader xmlReader(data);
    QScxmlParser parser(&xmlReader);
    parser.setFileName(fileName);
    parser.parse();
    auto stateMachine = parser.instantiateStateMachine();
    parser.instantiateDataModel(stateMachine);
    return stateMachine;
}

/*!
 * Returns the list of parse errors that occurred while creating a state machine from an
 *         SCXML file.
 */
QVector<QScxmlError> QScxmlStateMachine::parseErrors() const
{
    Q_D(const QScxmlStateMachine);
    return d->m_parserData ? d->m_parserData->m_errors : QVector<QScxmlError>();
}

QScxmlStateMachine::QScxmlStateMachine(QObject *parent)
    : QObject(*new QScxmlStateMachinePrivate, parent)
{
    Q_D(QScxmlStateMachine);
    d->init();
}

QScxmlStateMachine::QScxmlStateMachine(QScxmlStateMachinePrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
    dd.init();
}

/*!
    \property QScxmlStateMachine::running

    \brief the running state of this state machine

    \sa start()
 */

/*!
    \property QScxmlStateMachine::dataModel

    \brief The data model to be used for this state machine.

    SCXML data models are described in
    \l {SCXML Specification - 5 Data Model and Data Manipulation}. For more
    information about supported data models, see \l {SCXML Compliance}.

    Changing the data model when the state machine has been \c initialized is
    not specified in the SCXML standard and leads to undefined behavior.

    \sa QScxmlDataModel, QScxmlNullDataModel, QScxmlEcmaScriptDataModel,
        QScxmlCppDataModel
*/

/*!
    \property QScxmlStateMachine::initialized

    \brief Whether the state machine has been initialized.

    It is \c true if the state machine has been initialized, \c false otherwise.

    \sa QScxmlStateMachine::init(), QScxmlDataModel
*/

/*!
    \property QScxmlStateMachine::initialValues

    \brief The initial values to be used for setting up the data model.

    \sa QScxmlStateMachine::init(), QScxmlDataModel
*/

/*!
    \enum QScxmlStateMachine::BindingMethod

    This enum specifies the binding method. The binding method controls the point in time
    when the initial values are assigned to the data elements.

    \value EarlyBinding All data elements are created and initialized at data-model initialization.
           This is the default.
    \value LateBinding All data elements are created at initialization, but the initial values are
           assigned only when the containing state is entered for the first time. This is done
           before any executable content is executed.
 */

/*!
 * Returns the session ID for the current state machine.
 *
 * The session ID is used for message routing between parent and child state machines. If a state
 * machine is started by an \c <invoke> element, any event it sends will have the \c invokeid field
 * set to the session ID. The state machine will use the origin of an event (which is set by the
 * \e target or \e targetexpr attribute in a \c <send> element) to dispatch messages to the correct
 * child state machine.
 *
 * \sa setSessionId() QScxmlEvent::invokeId()
 */
QString QScxmlStateMachine::sessionId() const
{
    Q_D(const QScxmlStateMachine);

    return d->m_sessionId;
}

/*!
  Sets the session ID for the current state machine to \a id.

  \sa sessionId()
 */
void QScxmlStateMachine::setSessionId(const QString &id)
{
    Q_D(QScxmlStateMachine);
    d->m_sessionId = id;
}

/*!
 * Generates a unique ID by appending a unique number to the \a prefix.
 *
 * The number is only unique within a single run of an application. This method is used when an
 * invoked service does not have an ID set (the \e id attribute in \c <invoke>).
 */
QString QScxmlStateMachine::generateSessionId(const QString &prefix)
{
    int id = ++QScxmlStateMachinePrivate::m_sessionIdCounter;
    return prefix + QString::number(id);
}

/*!
 * Returns \c true when the state machine was started as a service with the \c <invoke> element,
 * \c false otherwise.
 */
bool QScxmlStateMachine::isInvoked() const
{
    Q_D(const QScxmlStateMachine);
    return d->m_isInvoked;
}

bool QScxmlStateMachine::isInitialized() const
{
    Q_D(const QScxmlStateMachine);
    return d->m_isInitialized;
}

/*!
 * Sets the data model for this state machine to \a model. There is a 1:1
 * relation between state machines and models. After setting the model once you
 * cannot change it anymore. Any further attempts to set the model using this
 * method will be ignored.
 */
void QScxmlStateMachine::setDataModel(QScxmlDataModel *model)
{
    Q_D(QScxmlStateMachine);

    if (d->m_dataModel == Q_NULLPTR && model != Q_NULLPTR) {
        d->m_dataModel = model;
        if (model)
            model->setStateMachine(this);
        emit dataModelChanged(model);
    }
}

/*!
 * Returns the data model used by the state machine.
 */
QScxmlDataModel *QScxmlStateMachine::dataModel() const
{
    Q_D(const QScxmlStateMachine);

    return d->m_dataModel;
}

/*!
 * \internal
 * Sets the binding method to the specified value.
 */
void QScxmlStateMachine::setDataBinding(QScxmlStateMachine::BindingMethod bindingMethod)
{
    Q_D(QScxmlStateMachine);

    d->m_dataBinding = bindingMethod;
}

/*!
 * Returns the binding method used by the state machine.
 */
QScxmlStateMachine::BindingMethod QScxmlStateMachine::dataBinding() const
{
    Q_D(const QScxmlStateMachine);

    return d->m_dataBinding;
}

/*!
 * \internal
 *
 * This is used internally in order to execute the executable content.
 *
 * \return the data tables used by the state machine.
 */
QScxmlTableData *QScxmlStateMachine::tableData() const
{
    Q_D(const QScxmlStateMachine);

    return d->m_tableData;
}

/*!
 * \internal
 * This is used when generating C++ from an SCXML file. The class implementing the
 * state machine will use this method to pass in the table data (which is also generated).
 *
 * \return the data tables used by the state machine.
 */
void QScxmlStateMachine::setTableData(QScxmlTableData *tableData)
{
    Q_D(QScxmlStateMachine);
    Q_ASSERT(tableData);

    d->m_tableData = tableData;
    if (objectName().isEmpty()) {
        setObjectName(tableData->name());
    }
}

void QScxmlInternal::WrappedQStateMachine::beginSelectTransitions(QEvent *event)
{
    Q_D(WrappedQStateMachine);

    if (event && event->type() == QScxmlEvent::scxmlEventType) {
        stateMachinePrivate()->m_event = *static_cast<QScxmlEvent *>(event);
        d->stateMachine()->dataModel()->setScxmlEvent(stateMachinePrivate()->m_event);

        auto scxmlEvent = static_cast<QScxmlEvent *>(event);
        auto smp = stateMachinePrivate();

        foreach (QScxmlInvokableService *service, smp->invokedServices()) {
            if (scxmlEvent->invokeId() == service->id()) {
                service->finalize();
            }
            if (service->autoforward()) {
                qCDebug(qscxmlLog) << this << "auto-forwarding event" << scxmlEvent->name()
                                  << "from" << stateMachine()->name() << "to service" << service->id();
                service->postEvent(new QScxmlEvent(*scxmlEvent));
            }
        }

        if (scxmlEvent->eventType() == QScxmlEvent::ExternalEvent) {
            emit d->stateMachine()->eventOccurred(*scxmlEvent);
        }

        if (scxmlEvent->originType() == QLatin1String("qt:signal")) {
            emit d->stateMachine()->externalEventOccurred(*scxmlEvent);
        }

        if (smp->m_eventFilter && !smp->m_eventFilter->handle(scxmlEvent, d->stateMachine())) {
            scxmlEvent->makeIgnorable();
            scxmlEvent->clear();
            smp->m_event.clear();
            return;
        }
    } else {
        stateMachinePrivate()->m_event.clear();
        d->stateMachine()->dataModel()->setScxmlEvent(stateMachinePrivate()->m_event);
    }
}

void QScxmlInternal::WrappedQStateMachine::beginMicrostep(QEvent *event)
{
    Q_D(WrappedQStateMachine);

    qCDebug(qscxmlLog) << d->m_stateMachine
                      << "started microstep from state" << d->m_stateMachine->activeStateNames()
                      << "with event" << stateMachinePrivate()->m_event.name()
                      << "and event type" << event->type();
}

void QScxmlInternal::WrappedQStateMachine::endMicrostep(QEvent *event)
{
    Q_D(WrappedQStateMachine);
    Q_UNUSED(event);

    qCDebug(qscxmlLog) << d->m_stateMachine
                      << "finished microstep in state (" << d->m_stateMachine->activeStateNames() << ")";
}

// This is a slightly modified copy of QStateMachinePrivate::event()
// Instead of postExternalEvent and processEvents
// we route event first to the appropriate state machine instance.
bool QScxmlInternal::WrappedQStateMachine::event(QEvent *e)
{
    Q_D(QScxmlInternal::WrappedQStateMachine);
    if (e->type() == QEvent::Timer) {
        QTimerEvent *te = static_cast<QTimerEvent*>(e);
        int tid = te->timerId();
        if (d->state != QStateMachinePrivate::Running) {
            // This event has been cancelled already
            QMutexLocker locker(&d->delayedEventsMutex);
            Q_ASSERT(!d->timerIdToDelayedEventId.contains(tid));
            return true;
        }
        d->delayedEventsMutex.lock();
        int id = d->timerIdToDelayedEventId.take(tid);
        QStateMachinePrivate::DelayedEvent ee = d->delayedEvents.take(id);
        if (ee.event != 0) {
            Q_ASSERT(ee.timerId == tid);
//          killTimer(tid);
//          d->delayedEventIdFreeList.release(id);
            d->delayedEventsMutex.unlock();
            d->_q_killDelayedEventTimer(id, tid);
            // route here
            if (ee.event->type() == QScxmlEvent::scxmlEventType)
                QScxmlStateMachinePrivate::get(stateMachine())->routeEvent(static_cast<QScxmlEvent *>(ee.event));
//          d->postExternalEvent(ee.event);
//          d->processEvents(QStateMachinePrivate::DirectProcessing);
            return true;
        } else {
            d->delayedEventsMutex.unlock();
        }
    }
    return QState::event(e);
}

void QScxmlInternal::WrappedQStateMachinePrivate::noMicrostep()
{
    qCDebug(qscxmlLog) << m_stateMachine
                      << "had no transition, stays in state (" << m_stateMachine->activeStateNames() << ")";
}

void QScxmlInternal::WrappedQStateMachinePrivate::processedPendingEvents(bool didChange)
{
    qCDebug(qscxmlLog) << m_stateMachine << "finishedPendingEvents" << didChange << "in state ("
                      << m_stateMachine->activeStateNames() << ")";
    emit m_stateMachine->reachedStableState();
}

void QScxmlInternal::WrappedQStateMachinePrivate::beginMacrostep()
{
}

void QScxmlInternal::WrappedQStateMachinePrivate::endMacrostep(bool didChange)
{
    qCDebug(qscxmlLog) << m_stateMachine << "endMacrostep" << didChange
                      << "in state (" << m_stateMachine->activeStateNames() << ")";

    { // handle <invoke>s
        QVector<QScxmlState*> &sti = stateMachinePrivate()->m_statesToInvoke;
        std::sort(sti.begin(), sti.end(), WrappedQStateMachinePrivate::stateEntryLessThan);
        foreach (QScxmlState *s, sti) {
            auto sp = QScxmlStatePrivate::get(s);
            foreach (QScxmlInvokableService *s, sp->servicesWaitingToStart) {
                s->start();
            }
            sp->servicesWaitingToStart.clear();
        }
        sti.clear();
    }
}

void QScxmlInternal::WrappedQStateMachinePrivate::enterStates(
        QEvent *event,
        const QList<QAbstractState*> &exitedStates_sorted,
        const QList<QAbstractState*> &statesToEnter_sorted,
        const QSet<QAbstractState*> &statesForDefaultEntry,
        QHash<QAbstractState *, QVector<QPropertyAssignment> > &propertyAssignmentsForState
#  ifndef QT_NO_ANIMATION
        , const QList<QAbstractAnimation*> &selectedAnimations
#  endif
        )
{
    QStateMachinePrivate::enterStates(event, exitedStates_sorted, statesToEnter_sorted,
                                      statesForDefaultEntry, propertyAssignmentsForState
#  ifndef QT_NO_ANIMATION
                                      , selectedAnimations
#  endif
                                      );
    foreach (QAbstractState *s, statesToEnter_sorted) {
        if (QScxmlState *qss = qobject_cast<QScxmlState *>(s)) {
            if (!QScxmlStatePrivate::get(qss)->invokableServiceFactories.isEmpty()) {
                if (!stateMachinePrivate()->m_statesToInvoke.contains(qss)) {
                    stateMachinePrivate()->m_statesToInvoke.append(qss);
                }
            }
        }
    }
}

void QScxmlInternal::WrappedQStateMachinePrivate::exitStates(
        QEvent *event,
        const QList<QAbstractState *> &statesToExit_sorted,
        const QHash<QAbstractState*, QVector<QPropertyAssignment> > &assignmentsForEnteredStates)
{
    QStateMachinePrivate::exitStates(event, statesToExit_sorted, assignmentsForEnteredStates);

    auto smp = stateMachinePrivate();
    for (int i = 0; i < smp->m_statesToInvoke.size(); ) {
        if (statesToExit_sorted.contains(smp->m_statesToInvoke.at(i))) {
            smp->m_statesToInvoke.removeAt(i);
        } else {
            ++i;
        }
    }

    foreach (QAbstractState *s, statesToExit_sorted) {
        if (QScxmlState *qss = qobject_cast<QScxmlState *>(s)) {
            auto ssp = QScxmlStatePrivate::get(qss);
            ssp->servicesWaitingToStart.clear();
            QVector<QScxmlInvokableService *> &services = ssp->invokedServices;
            foreach (QScxmlInvokableService *service, services) {
                qCDebug(qscxmlLog) << stateMachine() << "schedule service cancellation" << service->id();
                QMetaObject::invokeMethod(q_func(),
                                          "removeAndDestroyService",
                                          Qt::QueuedConnection,
                                          Q_ARG(QScxmlInvokableService *,service));
            }
            services.clear();
        }
    }
}

void QScxmlInternal::WrappedQStateMachinePrivate::exitInterpreter()
{
    Q_Q(WrappedQStateMachine);

    foreach (QAbstractState *s, configuration) {
        QScxmlExecutableContent::ContainerId onExitInstructions = QScxmlExecutableContent::NoInstruction;
        if (QScxmlFinalState *finalState = qobject_cast<QScxmlFinalState *>(s)) {
            stateMachinePrivate()->m_executionEngine->execute(finalState->doneData(), QVariant());
            onExitInstructions = QScxmlFinalStatePrivate::get(finalState)->onExitInstructions;
        } else if (QScxmlState *state = qobject_cast<QScxmlState *>(s)) {
            onExitInstructions = QScxmlStatePrivate::get(state)->onExitInstructions;
        }

        if (onExitInstructions != QScxmlExecutableContent::NoInstruction) {
            stateMachinePrivate()->m_executionEngine->execute(onExitInstructions);
        }

        if (QScxmlFinalState *finalState = qobject_cast<QScxmlFinalState *>(s)) {
            if (finalState->parent() == q) {
                if (auto psm = stateMachinePrivate()->m_parentStateMachine) {
                    auto done = new QScxmlEvent;
                    done->setName(QStringLiteral("done.invoke.") + m_stateMachine->sessionId());
                    done->setInvokeId(m_stateMachine->sessionId());
                    qCDebug(qscxmlLog) << "submitting event" << done->name() << "to" << psm->name();
                    psm->submitEvent(done);
                }
            }
        }
    }
}

void QScxmlInternal::WrappedQStateMachinePrivate::emitStateFinished(QState *forState, QFinalState *guiltyState)
{
    Q_Q(WrappedQStateMachine);

    if (QScxmlFinalState *finalState = qobject_cast<QScxmlFinalState *>(guiltyState)) {
        if (!q->isRunning())
            return;
        stateMachinePrivate()->m_executionEngine->execute(finalState->doneData(), forState->objectName());
    }

    QStateMachinePrivate::emitStateFinished(forState, guiltyState);
}

void QScxmlInternal::WrappedQStateMachinePrivate::startupHook()
{
    Q_Q(WrappedQStateMachine);

    q->submitQueuedEvents();
}

int QScxmlInternal::WrappedQStateMachinePrivate::eventIdForDelayedEvent(const QString &sendId)
{
    QMutexLocker locker(&delayedEventsMutex);

    QHash<int, DelayedEvent>::const_iterator it;
    for (it = delayedEvents.constBegin(); it != delayedEvents.constEnd(); ++it) {
        if (QScxmlEvent *e = dynamic_cast<QScxmlEvent *>(it->event)) {
            if (e->sendId() == sendId) {
                return it.key();
            }
        }
    }

    return -1;
}

/*!
 * Retrieves a list of state names of all states.
 *
 * When \a compress is \c true (the default), the states that contain child states
 * will be filtered out and only the \e {leaf states} will be returned.
 * When it is \c false, the full list of all states will be returned.
 *
 * The returned list does not contain the states of possible nested state machines.
 */
QStringList QScxmlStateMachine::stateNames(bool compress) const
{
    Q_D(const QScxmlStateMachine);

    QList<QObject *> worklist;
    worklist.reserve(d->m_qStateMachine->children().size() + d->m_qStateMachine->configuration().size());
    worklist.append(d->m_qStateMachine->children());

    QStringList res;
    while (!worklist.isEmpty()) {
        QObject *obj = worklist.takeLast();
        if (QAbstractState *state = qobject_cast<QAbstractState *>(obj)) {
            if (!compress || !obj->children().count())
                res.append(state->objectName());
            worklist.append(obj->children());
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

/*!
 * Retrieves a list of state names of all active states.
 *
 * When a state is active, all its parent states are active by definition. When \a compress
 * is \c true (the default), the parent states will be filtered out and only the \e {leaf states}
 * will be returned. When it is \c false, the full list of active states will be returned.
 */
QStringList QScxmlStateMachine::activeStateNames(bool compress) const
{
    Q_D(const QScxmlStateMachine);

    QSet<QAbstractState *> config = QStateMachinePrivate::get(d->m_qStateMachine)->configuration;
    if (compress)
        foreach (const QAbstractState *s, config)
            config.remove(s->parentState());
    QStringList res;
    foreach (const QAbstractState *s, config) {
        QString id = s->objectName();
        if (!id.isEmpty()) {
            res.append(id);
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

/*!
 * Returns \c true if the state specified by \a scxmlStateName is active, \c false otherwise.
 */
bool QScxmlStateMachine::isActive(const QString &scxmlStateName) const
{
    Q_D(const QScxmlStateMachine);
    QSet<QAbstractState *> config = QStateMachinePrivate::get(d->m_qStateMachine)->configuration;
    foreach (QAbstractState *s, config) {
        if (s->objectName() == scxmlStateName) {
            return true;
        }
    }
    return false;
}

/*!
 * Creates a connection of the given \a type from the state identified by \a scxmlStateName
 * to the \a method in the \a receiver object. The receiver's \a method
 * may contain a boolean argument that indicates whether the state connected
 * became active or inactive.
 *
 * Returns a handle to the connection, which can be used later to disconnect.
 */
QMetaObject::Connection QScxmlStateMachine::connectToState(const QString &scxmlStateName,
                                            const QObject *receiver, const char *method,
                                            Qt::ConnectionType type)
{
    Q_D(QScxmlStateMachine);
    QAbstractState *state = findState(scxmlStateName, d->m_qStateMachine);
    return QObject::connect(state, SIGNAL(activeChanged(bool)), receiver, method, type);
}

/*!
 * Returns the SCXML event filter if one is set, otherwise returns null.
 */
QScxmlEventFilter *QScxmlStateMachine::scxmlEventFilter() const
{
    Q_D(const QScxmlStateMachine);
    return d->m_eventFilter;
}

/*!
 * Sets the \a newFilter as the SCXML event filter. Passing null will remove the current filter.
 */
void QScxmlStateMachine::setScxmlEventFilter(QScxmlEventFilter *newFilter)
{
    Q_D(QScxmlStateMachine);
    d->m_eventFilter = newFilter;
}

/*!
 * Initializes the state machine.
 *
 * State machine initialization consists of calling QScxmlDataModel::setup(), setting the initial
 * values for \c <data> elements, and executing any \c <script> tags of the \c <scxml> tag. The
 * initial data values are taken from the \c initialValues property.
 *
 * Returns \c false if parse errors occur or if any of the initialization steps fail.
 * Returns \c true otherwise.
 */
bool QScxmlStateMachine::init()
{
    Q_D(QScxmlStateMachine);

    if (d->m_isInitialized)
        return false;

    if (!parseErrors().isEmpty())
        return false;

    if (!dataModel() || !dataModel()->setup(d->m_initialValues))
        return false;

    if (!d->executeInitialSetup())
        return false;

    d->m_isInitialized = true;
    emit initializedChanged(true);
    return true;
}

/*!
 * Returns \c true if the state machine is running, \c false otherwise.
 *
 * \sa setRunning(), runningChanged()
 */
bool QScxmlStateMachine::isRunning() const
{
    Q_D(const QScxmlStateMachine);

    return d->m_qStateMachine->isRunning();
}

/*!
 * Starts the state machine if \a running is \c true, or stops it otherwise.
 *
 * \sa start(), stop(), isRunning(), runningChanged()
 */
void QScxmlStateMachine::setRunning(bool running)
{
    if (running)
        start();
    else
        stop();
}

QVariantMap QScxmlStateMachine::initialValues()
{
    Q_D(const QScxmlStateMachine);
    return d->m_initialValues;
}

void QScxmlStateMachine::setInitialValues(const QVariantMap &initialValues)
{
    Q_D(QScxmlStateMachine);
    if (initialValues != d->m_initialValues) {
        d->m_initialValues = initialValues;
        emit initialValuesChanged(initialValues);
    }
}

/*!
 * Returns the name of the state machine as set by the \e name attribute of the \c <scxml> tag.
 */
QString QScxmlStateMachine::name() const
{
    return tableData()->name();
}

/*!
 * Submits the SCXML event \a event to the internal or external event queue depending on the
 * priority of the event.
 *
 * When a delay is set, the event will be queued for delivery after the timeout has passed.
 */
void QScxmlStateMachine::submitEvent(QScxmlEvent *event)
{
    Q_D(QScxmlStateMachine);

    if (!event)
        return;

    if (event->delay() > 0) {
        qCDebug(qscxmlLog) << this << "submitting event" << event->name()
                           << "with delay" << event->delay() << "ms:"
                           << QScxmlEventPrivate::debugString(event).constData();

        Q_ASSERT(event->eventType() == QScxmlEvent::ExternalEvent);
        int id = d->m_qStateMachine->postDelayedEvent(event, event->delay());

        qCDebug(qscxmlLog) << this << ": delayed event" << event->name() << "(" << event << ") got id:" << id;
    } else {
        qCDebug(qscxmlLog) << this << "submitting event" << event->name()
                           << ":" << QScxmlEventPrivate::debugString(event).constData();

        d->routeEvent(event);
    }
}

/*!
 * A utility method to create and submit an external event with the specified
 * \a eventName as the name.
 */
void QScxmlStateMachine::submitEvent(const QString &eventName)
{
    QScxmlEvent *e = new QScxmlEvent;
    e->setName(eventName);
    e->setEventType(QScxmlEvent::ExternalEvent);
    submitEvent(e);
}

/*!
 * A utility method to create and submit an external event with the specified
 * \a eventName as the name and \a data as the payload data.
 */
void QScxmlStateMachine::submitEvent(const QString &eventName, const QVariant &data)
{
    QVariant incomingData = data;
    if (incomingData.canConvert<QJSValue>()) {
        incomingData = incomingData.value<QJSValue>().toVariant();
    }

    QScxmlEvent *e = new QScxmlEvent;
    e->setName(eventName);
    e->setEventType(QScxmlEvent::ExternalEvent);
    e->setData(incomingData);
    submitEvent(e);
}

/*!
 * Cancels a delayed event with the specified \a sendId.
 */
void QScxmlStateMachine::cancelDelayedEvent(const QString &sendId)
{
    Q_D(QScxmlStateMachine);

    int id = d->m_qStateMachine->eventIdForDelayedEvent(sendId);

    qCDebug(qscxmlLog) << this << "canceling event" << sendId << "with id" << id;

    if (id != -1)
        d->m_qStateMachine->cancelDelayedEvent(id);
}

void QScxmlInternal::WrappedQStateMachine::queueEvent(QScxmlEvent *event, EventPriority priority)
{
    Q_D(WrappedQStateMachine);

    if (!d->m_queuedEvents)
        d->m_queuedEvents = new QVector<WrappedQStateMachinePrivate::QueuedEvent>();
    d->m_queuedEvents->append(WrappedQStateMachinePrivate::QueuedEvent(event, priority));
}

void QScxmlInternal::WrappedQStateMachine::submitQueuedEvents()
{
    Q_D(WrappedQStateMachine);

    qCDebug(qscxmlLog) << d->m_stateMachine << ": submitting queued events";

    if (d->m_queuedEvents) {
        foreach (const WrappedQStateMachinePrivate::QueuedEvent &e, *d->m_queuedEvents)
            postEvent(e.event, e.priority);
        delete d->m_queuedEvents;
        d->m_queuedEvents = Q_NULLPTR;
    }
}

int QScxmlInternal::WrappedQStateMachine::eventIdForDelayedEvent(const QString &sendId)
{
    Q_D(WrappedQStateMachine);
    return d->eventIdForDelayedEvent(sendId);
}

void QScxmlInternal::WrappedQStateMachine::removeAndDestroyService(QScxmlInvokableService *service)
{
    Q_D(WrappedQStateMachine);
    qCDebug(qscxmlLog) << stateMachine() << "canceling service" << service->id();
    if (d->stateMachinePrivate()->removeService(service)) {
        delete service;
    }
}

/*!
 * Returns \c true if a message to \a target can be dispatched by this state machine.
 *
 * Valid targets are:
 * \list
 * \li  \c #_parent for the parent state machine if the current state machine is started by
 *      \c <invoke>
 * \li  \c #_internal for the current state machine
 * \li  \c #_scxml_sessionid, where \c sessionid is the session ID of the current state machine
 * \li  \c #_servicename, where \c servicename is the ID or name of a service started with
 *      \c <invoke> by this state machine
 * \endlist
 */
bool QScxmlStateMachine::isDispatchableTarget(const QString &target) const
{
    Q_D(const QScxmlStateMachine);

    if (isInvoked() && target == QStringLiteral("#_parent"))
        return true; // parent state machine, if we're <invoke>d.
    if (target == QStringLiteral("#_internal")
            || target == QStringLiteral("#_scxml_%1").arg(sessionId()))
        return true; // that's the current state machine

    if (target.startsWith(QStringLiteral("#_"))) {
        QStringRef targetId = target.midRef(2);
        foreach (QScxmlInvokableService *service, d->m_invokedServices) {
            if (service->id() == targetId) {
                return true;
            }
        }
    }

    return false;
}

/*!
  \fn QScxmlStateMachine::runningChanged(bool running)

  This signal is emitted when the \c running property is changed with \a running as argument.
*/

/*!
  \fn QScxmlStateMachine::log(const QString &label, const QString &msg)

  This signal is emitted if a \c <log> tag is used in the SCXML. \a label is the value of the
  \e label attribute of the \c <log> tag. \a msg is the value of the evaluated \e expr attribute
  of the \c <log> tag. If there is no \e expr attribute, a null string will be returned.
*/

/*!
  \fn QScxmlStateMachine::reachedStableState()

  This signal is emitted when the event queue is empty at the end of a macro step or when a final
  state is reached.
*/

/*!
  \fn QScxmlStateMachine::finished()

  This signal is emitted when the state machine reaches a top-level final state.

  \sa running
*/


/*!
  Starts this state machine. The machine will reset its configuration and
  transition to the initial state. When a final top-level state
  is entered, the machine will emit the finished() signal.

  \note A state machine will not run without a running event loop, such as
  the main application event loop started with QCoreApplication::exec() or
  QApplication::exec().

  \sa runningChanged(), setRunning(), stop(), finished()
*/
void QScxmlStateMachine::start()
{
    Q_D(QScxmlStateMachine);

    if (!parseErrors().isEmpty())
        return;

    // Failure to initialize doesn't prevent start(). See w3c-ecma/test487 in the scion test suite.
    if (!isInitialized() && !init())
        qCDebug(qscxmlLog) << this << "cannot be initialized on start(). Starting anyway ...";

    d->m_qStateMachine->start();
}

/*!
  Stops this state machine. The machine will not execute any further state
  transitions. Its \c running property is set to \c false.

  \sa runningChanged(), start(), setRunning()
 */
void QScxmlStateMachine::stop()
{
    Q_D(QScxmlStateMachine);
    d->m_qStateMachine->stop();
}

/*!
 * \internal
 */
void QScxmlStateMachine::setService(const QString &id, QScxmlInvokableService *service)
{
    Q_UNUSED(id);
    Q_UNUSED(service);
}

QT_END_NAMESPACE
