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

#include "qscxmlparser_p.h"
#include "qscxmlexecutablecontent_p.h"

#include <QXmlStreamReader>
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include <QString>

#ifndef BUILD_QSCXMLC
#include "qscxmlnulldatamodel.h"
#include "qscxmlecmascriptdatamodel.h"
#include "qscxmlqstates.h"
#include "qscxmldatamodel_p.h"
#include "qscxmlstatemachine_p.h"
#include "qscxmlstatemachine.h"

#include <QState>
#include <QHistoryState>
#include <QEventTransition>
#include <QSignalTransition>
#include <QJSValue>
#include <private/qabstracttransition_p.h>
#include <private/qmetaobjectbuilder_p.h>
#endif // BUILD_QSCXMLC

#include <functional>

namespace {
enum {
    DebugHelper_NameTransitions = 0
};
} // anonymous namespace

QT_BEGIN_NAMESPACE

static QString scxmlNamespace = QStringLiteral("http://www.w3.org/2005/07/scxml");
static QString qtScxmlNamespace = QStringLiteral("http://theqtcompany.com/scxml/2015/06/");

namespace {

class ScxmlVerifier: public DocumentModel::NodeVisitor
{
public:
    ScxmlVerifier(std::function<void (const DocumentModel::XmlLocation &, const QString &)> errorHandler)
        : m_errorHandler(errorHandler)
        , m_doc(Q_NULLPTR)
        , m_hasErrors(false)
    {}

    bool verify(DocumentModel::ScxmlDocument *doc)
    {
        if (doc->isVerified)
            return true;

        doc->isVerified = true;
        m_doc = doc;
        foreach (DocumentModel::AbstractState *state, doc->allStates) {
            if (state->id.isEmpty()) {
                continue;
#ifndef QT_NO_DEBUG
            } else if (m_stateById.contains(state->id)) {
                Q_ASSERT(!"Should be unreachable: the parser should check for this case!");
#endif // QT_NO_DEBUG
            } else {
                m_stateById[state->id] = state;
            }
        }

        if (doc->root)
            doc->root->accept(this);
        return !m_hasErrors;
    }

private:
    bool visit(DocumentModel::Scxml *scxml) Q_DECL_OVERRIDE
    {
        Q_ASSERT(scxml->initialStates.isEmpty());
        if (!scxml->name.isEmpty() && !isValidToken(scxml->name, XmlNmtoken)) {
            error(scxml->xmlLocation,
                  QStringLiteral("scxml name '%1' is not a valid XML Nmtoken").arg(scxml->name));
        }

        if (scxml->initial.isEmpty()) {
            if (auto firstChild = firstAbstractState(scxml)) {
                scxml->initialStates.append(firstChild);
            }
        } else {
            foreach (const QString &initial, scxml->initial) {
                if (DocumentModel::AbstractState *s = m_stateById.value(initial))
                    scxml->initialStates.append(s);
                else
                    error(scxml->xmlLocation, QStringLiteral("initial state '%1' not found for <scxml> element").arg(initial));
            }
        }

        m_parentNodes.append(scxml);

        return true;
    }

    void endVisit(DocumentModel::Scxml *) Q_DECL_OVERRIDE
    {
        m_parentNodes.removeLast();
    }

    bool visit(DocumentModel::State *state) Q_DECL_OVERRIDE
    {
        Q_ASSERT(state->initialStates.isEmpty());

        if (!state->id.isEmpty() && !isValidToken(state->id, XmlNCName)) {
            error(state->xmlLocation, QStringLiteral("'%1' is not a valid XML ID").arg(state->id));
        } else if (state->type != DocumentModel::State::Initial) {
            validateStateName(state->xmlLocation, state->id);
        }

        if (state->initial.isEmpty()) {
            if (auto firstChild = firstAbstractState(state)) {
                state->initialStates += firstChild;
            }
        } else {
            Q_ASSERT(state->type == DocumentModel::State::Normal);
            foreach (const QString &initialState, state->initial) {
                if (DocumentModel::AbstractState *s = m_stateById.value(initialState)) {
                    state->initialStates += s;
                } else {
                    error(state->xmlLocation,
                          QStringLiteral("undefined initial state '%1' for state '%2'")
                          .arg(initialState, state->id));
                }
            }
        }

        switch (state->type) {
        case DocumentModel::State::Normal:
            break;
        case DocumentModel::State::Parallel:
            if (!state->initial.isEmpty()) {
                error(state->xmlLocation, QStringLiteral("parallel states cannot have an initial state"));
            }
            break;
        case DocumentModel::State::Initial:
            if (transitionCount(state) != 1)
                error(state->xmlLocation, QStringLiteral("an initial state can only have one transition, but has '%1'").arg(transitionCount(state)));
            if (DocumentModel::Transition *t = firstTransition(state)) {
                if (!t->events.isEmpty() || !t->condition.isNull()) {
                    error(t->xmlLocation, QStringLiteral("the transition in an initial state cannot have an event or a condition"));
                }
                if (t->targets.isEmpty()) {
                    error(t->xmlLocation, QStringLiteral("the transition in an initial state must have at least one target"));
                }
            }
            foreach (DocumentModel::StateOrTransition *child, state->children) {
                if (DocumentModel::State *s = child->asState()) {
                    error(s->xmlLocation, QStringLiteral("substates are not allowed in initial states"));
                }
            }
            if (parentState() == Q_NULLPTR) {
                error(state->xmlLocation, QStringLiteral("initial states can only occur in a state"));
            }
            break;
        case DocumentModel::State::Final:
            break;
        default:
            Q_UNREACHABLE();
        }

        m_parentNodes.append(state);
        return true;
    }

    void endVisit(DocumentModel::State *) Q_DECL_OVERRIDE
    {
        m_parentNodes.removeLast();
    }

    bool visit(DocumentModel::Transition *transition) Q_DECL_OVERRIDE
    {
        Q_ASSERT(transition->targetStates.isEmpty());

        if (int size = transition->targets.size())
            transition->targetStates.reserve(size);
        foreach (const QString &target, transition->targets) {
            if (DocumentModel::AbstractState *s = m_stateById.value(target)) {
                if (transition->targetStates.contains(s)) {
                    error(transition->xmlLocation, QStringLiteral("duplicate target '%1'").arg(target));
                } else {
                    transition->targetStates.append(s);
                }
            } else if (!target.isEmpty()) {
                error(transition->xmlLocation, QStringLiteral("unknown state '%1' in target").arg(target));
            }
        }
        foreach (const QString &event, transition->events) {
            checkEvent(event, transition->xmlLocation, AllowWildCards);

            if (!DocumentModel::isEventToBeGenerated(event))
                continue;

            validateEventName(transition->xmlLocation, event);
        }

        m_parentNodes.append(transition);
        return true;
    }

    void endVisit(DocumentModel::Transition *) Q_DECL_OVERRIDE
    {
        m_parentNodes.removeLast();
    }

    bool visit(DocumentModel::HistoryState *state) Q_DECL_OVERRIDE
    {
        bool seenTransition = false;
        foreach (DocumentModel::StateOrTransition *sot, state->children) {
            if (DocumentModel::State *s = sot->asState()) {
                error(s->xmlLocation, QStringLiteral("history state cannot have substates"));
            } else if (DocumentModel::Transition *t = sot->asTransition()) {
                if (seenTransition) {
                    error(t->xmlLocation, QStringLiteral("history state can only have one transition"));
                } else {
                    seenTransition = true;
                    m_parentNodes.append(state);
                    t->accept(this);
                    m_parentNodes.removeLast();
                }
            }
        }

        return false;
    }

    bool visit(DocumentModel::Send *node) Q_DECL_OVERRIDE
    {
        checkEvent(node->event, node->xmlLocation, ForbidWildCards);

        if (node->type == QStringLiteral("qt:signal")) {
            validateEventName(node->xmlLocation, node->event);
        }

        checkExpr(node->xmlLocation, QStringLiteral("send"), QStringLiteral("eventexpr"), node->eventexpr);
        return true;
    }

    void visit(DocumentModel::Cancel *node) Q_DECL_OVERRIDE
    {
        checkExpr(node->xmlLocation, QStringLiteral("cancel"), QStringLiteral("sendidexpr"), node->sendidexpr);
    }

    bool visit(DocumentModel::DoneData *node) Q_DECL_OVERRIDE
    {
        checkExpr(node->xmlLocation, QStringLiteral("donedata"), QStringLiteral("expr"), node->expr);
        return false;
    }

    bool visit(DocumentModel::Invoke *node) Q_DECL_OVERRIDE
    {
        if (node->content.isNull()) {
            error(node->xmlLocation, QStringLiteral("no valid content found in <invoke> tag"));
        } else {
            ScxmlVerifier subVerifier(m_errorHandler);
            m_hasErrors = !subVerifier.verify(node->content.data());
        }
        return false;
    }

private:
    enum TokenType {
        XmlNCName,
        XmlNmtoken,
    };

    static bool isValidToken(const QString &id, TokenType tokenType)
    {
        Q_ASSERT(!id.isEmpty());
        int i = 0;
        if (tokenType == XmlNCName) {
            const QChar c = id.at(i++);
            if (!isLetter(c) && c != QLatin1Char('_'))
                return false;
        }
        for (int ei = id.length(); i != ei; ++i) {
            const QChar c = id.at(i);
            if (isLetter(c) || c.isDigit() || c == QLatin1Char('.') || c == QLatin1Char('-')
                    || c == QLatin1Char('_') || isNameTail(c))
                continue;
            else if (tokenType == XmlNmtoken && c == QLatin1Char(':'))
                continue;
            else
                return false;
        }

        return true;
    }

    static bool isLetter(QChar c)
    {
        switch (c.category()) {
        case QChar::Letter_Lowercase:
        case QChar::Letter_Uppercase:
        case QChar::Letter_Other:
        case QChar::Letter_Titlecase:
        case QChar::Number_Letter:
            return true;
        default:
            return false;
        }
    }

    static bool isNameTail(QChar c)
    {
        switch (c.category()) {
        case QChar::Mark_SpacingCombining:
        case QChar::Mark_Enclosing:
        case QChar::Mark_NonSpacing:
        case QChar::Letter_Modifier:
        case QChar::Number_DecimalDigit:
            return true;
        default:
            return false;
        }
    }

    enum WildCardMode {
        ForbidWildCards,
        AllowWildCards
    };

    void checkEvent(const QString &event, const DocumentModel::XmlLocation &loc,
                    WildCardMode wildCardMode)
    {
        if (event.isEmpty())
            return;

        if (!isValidEvent(event, wildCardMode)) {
            error(loc, QStringLiteral("'%1' is not a valid event").arg(event));
        }
    }

    static bool isValidEvent(const QString &event, WildCardMode wildCardMode)
    {
        if (event.isEmpty())
            return false;

        if (wildCardMode == AllowWildCards && event == QLatin1String(".*"))
            return true;

        const QStringList parts = event.split(QLatin1Char('.'));

        for (const QString &part : parts) {
            if (part.isEmpty())
                return false;

            if (wildCardMode == AllowWildCards && part.length() == 1
                    && part.at(0) == QLatin1Char('*')) {
                continue;
            }

            for (int i = 0, ei = part.length(); i != ei; ++i) {
                const QChar c = part.at(i);
                if (!isLetter(c) && !c.isDigit() && c != QLatin1Char('-') && c != QLatin1Char('_')
                        && c != QLatin1Char(':')) {
                    return false;
                }
            }
        }

        return true;
    }

    static int transitionCount(DocumentModel::State *state)
    {
        int count = 0;
        foreach (DocumentModel::StateOrTransition *child, state->children) {
            if (child->asTransition())
                ++count;
        }
        return count;
    }

    static DocumentModel::Transition *firstTransition(DocumentModel::State *state)
    {
        foreach (DocumentModel::StateOrTransition *child, state->children) {
            if (DocumentModel::Transition *t = child->asTransition())
                return t;
        }
        return Q_NULLPTR;
    }

    static DocumentModel::AbstractState *firstAbstractState(DocumentModel::StateContainer *container)
    {
        QVector<DocumentModel::StateOrTransition *> children;
        if (auto state = container->asState())
            children = state->children;
        else if (auto scxml = container->asScxml())
            children = scxml->children;
        else
            Q_UNREACHABLE();
        foreach (DocumentModel::StateOrTransition *child, children) {
            if (DocumentModel::State *s = child->asState())
                return s;
            else if (DocumentModel::HistoryState *h = child->asHistoryState())
                return h;
        }
        return Q_NULLPTR;
    }

    void checkExpr(const DocumentModel::XmlLocation &loc, const QString &tag, const QString &attrName, const QString &attrValue)
    {
        if (m_doc->root->dataModel == DocumentModel::Scxml::NullDataModel && !attrValue.isEmpty()) {
            error(loc, QStringLiteral(
                      "%1 in <%2> cannot be used with data model 'null'").arg(attrName, tag));
        }
    }

    void error(const DocumentModel::XmlLocation &location, const QString &message)
    {
        m_hasErrors = true;
        if (m_errorHandler)
            m_errorHandler(location, message);
    }

    DocumentModel::Node *parentState() const
    {
        for (int i = m_parentNodes.size() - 1; i >= 0; --i) {
            if (DocumentModel::State *s = m_parentNodes.at(i)->asState())
                return s;
        }

        return Q_NULLPTR;
    }

    void validateEventName(const DocumentModel::XmlLocation &location, const QString &name)
    {
        if (!m_doc->qtMode)
            return;

        if (!isValidCppIdentifier(name))
            error(location, QStringLiteral(
                      "event name '%1' is not a valid C++ identifier in Qt mode").arg(name));

        if (!isValidQtIdentifier(name))
            error(location, QStringLiteral(
                      "event name '%1' is not a valid Qt identifier in Qt mode").arg(name));

        if (m_stateById.contains(name))
            error(location, QStringLiteral(
                      "event name '%1' collides with a state name '%1' in Qt mode").arg(name));

        const QString changedSuffix(QStringLiteral("Changed"));
        if (name.endsWith(changedSuffix)) {
            const QString baseName = name.left(name.count() - changedSuffix.count());
            if (m_stateById.contains(baseName))
                error(location, QStringLiteral(
                          "event name '%1' collides with a state name '%2' in Qt mode").arg(name).arg(baseName));
        }
    }

    void validateStateName(const DocumentModel::XmlLocation &location, const QString &name)
    {
        if (!m_doc->qtMode)
            return;

        if (!isValidCppIdentifier(name))
            error(location, QStringLiteral(
                      "state name '%1' is not a valid C++ identifier in Qt mode").arg(name));

        if (!isValidQtIdentifier(name))
            error(location, QStringLiteral(
                      "state name '%1' is not a valid Qt identifier in Qt mode").arg(name));

        const QString changedSuffix(QStringLiteral("Changed"));
        if (name.endsWith(changedSuffix)) {
            const QString baseName = name.left(name.count() - changedSuffix.count());
            if (m_stateById.contains(baseName))
                error(location, QStringLiteral(
                          "state name '%1' collides with a state name '%2' in Qt mode").arg(name).arg(baseName));
        }
    }

    static bool isValidCppIdentifier(const QString &str)
    {
        static const QStringList keywords = QStringList()
                << QStringLiteral("alignas")
                << QStringLiteral("alignof")
                << QStringLiteral("asm")
                << QStringLiteral("auto")
                << QStringLiteral("bool")
                << QStringLiteral("break")
                << QStringLiteral("case")
                << QStringLiteral("catch")
                << QStringLiteral("char")
                << QStringLiteral("char16_t")
                << QStringLiteral("char32_t")
                << QStringLiteral("class")
                << QStringLiteral("const")
                << QStringLiteral("constexpr")
                << QStringLiteral("const_cast")
                << QStringLiteral("continue")
                << QStringLiteral("decltype")
                << QStringLiteral("default")
                << QStringLiteral("delete")
                << QStringLiteral("double")
                << QStringLiteral("do")
                << QStringLiteral("dynamic_cast")
                << QStringLiteral("else")
                << QStringLiteral("enum")
                << QStringLiteral("explicit")
                << QStringLiteral("export")
                << QStringLiteral("extern")
                << QStringLiteral("false")
                << QStringLiteral("float")
                << QStringLiteral("for")
                << QStringLiteral("friend")
                << QStringLiteral("goto")
                << QStringLiteral("if")
                << QStringLiteral("inline")
                << QStringLiteral("int")
                << QStringLiteral("long")
                << QStringLiteral("mutable")
                << QStringLiteral("namespace")
                << QStringLiteral("new")
                << QStringLiteral("noexcept")
                << QStringLiteral("nullptr")
                << QStringLiteral("operator")
                << QStringLiteral("private")
                << QStringLiteral("protected")
                << QStringLiteral("public")
                << QStringLiteral("register")
                << QStringLiteral("reinterpret_cast")
                << QStringLiteral("return")
                << QStringLiteral("short")
                << QStringLiteral("signed")
                << QStringLiteral("sizeof")
                << QStringLiteral("static")
                << QStringLiteral("static_assert")
                << QStringLiteral("static_cast")
                << QStringLiteral("struct")
                << QStringLiteral("switch")
                << QStringLiteral("template")
                << QStringLiteral("this")
                << QStringLiteral("thread_local")
                << QStringLiteral("throw")
                << QStringLiteral("true")
                << QStringLiteral("try")
                << QStringLiteral("typedef")
                << QStringLiteral("typeid")
                << QStringLiteral("typename")
                << QStringLiteral("union")
                << QStringLiteral("unsigned")
                << QStringLiteral("using")
                << QStringLiteral("virtual")
                << QStringLiteral("void")
                << QStringLiteral("volatile")
                << QStringLiteral("wchar_t")
                << QStringLiteral("while");

        if (keywords.contains(str)) {
            return false;
        }

        auto isNonDigit = [](QChar ch) -> bool {
            return (ch >= QLatin1Char('A') && ch <= QLatin1Char('Z'))
                    || (ch >= QLatin1Char('a') && ch <= QLatin1Char('z'))
                    || (ch == QLatin1Char('_'));
        };

        QChar ch = str.at(0);
        if (!isNonDigit(ch)) {
            return false;
        }
        for (int i = 1, ei = str.size(); i != ei; ++i) {
            ch = str.at(i);
            if (!isNonDigit(ch) && !ch.isDigit()) {
                return false;
            }
        }

        if (str.startsWith(QLatin1Char('_')) && str.size() > 1) {
            QChar ch = str.at(1);
            if (ch == QLatin1Char('_')
                    || (ch >= QLatin1Char('A') && ch <= QLatin1Char('Z'))) {
                return false;
            }
        }

        return true;
    }

    static bool isValidQtIdentifier(const QString &str)
    {
        static const QStringList keywords = QStringList()
                << QStringLiteral("QObject")
                << QStringLiteral("event")
                << QStringLiteral("eventFilter")
                << QStringLiteral("tr")
                << QStringLiteral("trUtf8")
                << QStringLiteral("metaObject")
                << QStringLiteral("staticMetaObject")
                << QStringLiteral("objectName")
                << QStringLiteral("setObjectName")
                << QStringLiteral("isWidgetType")
                << QStringLiteral("isWindowType")
                << QStringLiteral("signalsBlocked")
                << QStringLiteral("blockSignals")
                << QStringLiteral("thread")
                << QStringLiteral("moveToThread")
                << QStringLiteral("startTimer")
                << QStringLiteral("killTimer")
                << QStringLiteral("findChild")
                << QStringLiteral("findChildren")
                << QStringLiteral("children")
                << QStringLiteral("setParent")
                << QStringLiteral("installEventFilter")
                << QStringLiteral("removeEventFilter")
                << QStringLiteral("connect")
                << QStringLiteral("connect_functor")
                << QStringLiteral("disconnect")
                << QStringLiteral("dumpObjectTree")
                << QStringLiteral("dumpObjectInfo")
                << QStringLiteral("setProperty")
                << QStringLiteral("property")
                << QStringLiteral("dynamicPropertyNames")
                << QStringLiteral("registerUserData")
                << QStringLiteral("setUserData")
                << QStringLiteral("userData")
                << QStringLiteral("destroyed")
                << QStringLiteral("objectNameChanged")
                << QStringLiteral("parent")
                << QStringLiteral("inherits")
                << QStringLiteral("deleteLater")
                << QStringLiteral("sender")
                << QStringLiteral("senderSignalIndex")
                << QStringLiteral("receivers")
                << QStringLiteral("isSignalConnected")
                << QStringLiteral("timerEvent")
                << QStringLiteral("childEvent")
                << QStringLiteral("customEvent")
                << QStringLiteral("connectNotify")
                << QStringLiteral("disconnectNotify")
                << QStringLiteral("d_ptr")
                << QStringLiteral("staticQtMetaObject")
                << QStringLiteral("d_func")
                << QStringLiteral("connectImpl")
                << QStringLiteral("disconnectImpl")

                << QStringLiteral("QScxmlStateMachine")
                << QStringLiteral("running")
                << QStringLiteral("BindingMethod")
                << QStringLiteral("EarlyBinding")
                << QStringLiteral("LateBinding")
                << QStringLiteral("fromFile")
                << QStringLiteral("fromData")
                << QStringLiteral("parseErrors")
                << QStringLiteral("sessionId")
                << QStringLiteral("setSessionId")
                << QStringLiteral("generateSessionId")
                << QStringLiteral("isInvoked")
                << QStringLiteral("dataModel")
                << QStringLiteral("dataBinding")
                << QStringLiteral("init")
                << QStringLiteral("isRunning")
                << QStringLiteral("name")
                << QStringLiteral("stateNames")
                << QStringLiteral("activeStateNames")
                << QStringLiteral("isActive")
                << QStringLiteral("scxmlEventFilter")
                << QStringLiteral("setScxmlEventFilter")
                << QStringLiteral("submitEvent")
                << QStringLiteral("cancelDelayedEvent")
                << QStringLiteral("isDispatchableTarget")
                << QStringLiteral("runningChanged")
                << QStringLiteral("log")
                << QStringLiteral("reachedStableState")
                << QStringLiteral("finished")
                << QStringLiteral("eventOccurred")
                << QStringLiteral("start")
                << QStringLiteral("setDataBinding")
                << QStringLiteral("setService")
                << QStringLiteral("tableData");

        if (keywords.contains(str))
            return false;

        return true;
    }

private:
    std::function<void (const DocumentModel::XmlLocation &, const QString &)> m_errorHandler;
    DocumentModel::ScxmlDocument *m_doc;
    bool m_hasErrors;
    QHash<QString, DocumentModel::AbstractState *> m_stateById;
    QVector<DocumentModel::Node *> m_parentNodes;
};

#ifndef BUILD_QSCXMLC
class QStateMachineBuilder;
class DynamicStateMachine: public QScxmlStateMachine, public QScxmlEventFilter
{
    // Manually expanded from Q_OBJECT macro:
public:
    Q_OBJECT_CHECK

    const QMetaObject *metaObject() const Q_DECL_OVERRIDE
    { return m_metaObject; }

    int qt_metacall(QMetaObject::Call _c, int _id, void **_a) Q_DECL_OVERRIDE
    {
        _id = QScxmlStateMachine::qt_metacall(_c, _id, _a);
        if (_id < 0)
            return _id;
        int ownMethodCount = m_metaObject->methodCount() - m_metaObject->methodOffset();
        if (_c == QMetaObject::InvokeMetaMethod) {
            if (_id < ownMethodCount)
                qt_static_metacall(this, _c, _id, _a);
            _id -= ownMethodCount;
        } else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
                   || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
            qt_static_metacall(this, _c, _id, _a);
            _id -= m_metaObject->propertyCount();
        }
        return _id;
    }

private:
    static void qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
    {
        if (_c == QMetaObject::InvokeMetaMethod) {
            DynamicStateMachine *_t = static_cast<DynamicStateMachine *>(_o);
            if (_id >= _t->m_eventNamesByIndex.size() || _id < 0) {
                // out of bounds
                return;
            }
            if (_id >= _t->m_firstSubStateMachineSignal && _id < _t->m_firstSlot) {
                // these signals are only emitted, not activated by another signal
                return;
            }
            if (_id >= _t->m_firstStateChangedSignal && _id < _t->m_firstSubStateMachineSignal) {
                // re-propagate QAbstractState::activeChanged as stateChanged
                QMetaObject::activate(_t, _t->m_metaObject, _id, _a);
                return;
            }
            // We have 1 kind of slots: those to submit events.
            const QString &event = _t->m_eventNamesByIndex.at(_id);
            if (!event.isEmpty()) {
                if (_id < _t->m_firstSlotWithoutData) {
                    QVariant data = *reinterpret_cast< QVariant(*)>(_a[1]);
                    if (data.canConvert<QJSValue>()) {
                        data = data.value<QJSValue>().toVariant();
                    }
                    _t->submitEvent(event, data);
                } else {
                    _t->submitEvent(event, QVariant());
                }
            }
        } else if (_c == QMetaObject::RegisterPropertyMetaType) {
            DynamicStateMachine *_t = static_cast<DynamicStateMachine *>(_o);
            if (_id < _t->m_firstSubStateMachineProperty) {
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType<bool>();
            } else {
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType<QScxmlStateMachine *>();
            }
        } else if (_c == QMetaObject::ReadProperty) {
            DynamicStateMachine *_t = static_cast<DynamicStateMachine *>(_o);
            void *_v = _a[0];
            if (_id >= 0 && _id < _t->m_propertyNamesByIndex.size()) {
                if (_id < _t->m_firstSubStateMachineProperty) {
                    // getter for the state
                    auto smp = QScxmlStateMachinePrivate::get(_t);
                    auto name = _t->m_propertyNamesByIndex.at(_id);
                    *reinterpret_cast<bool*>(_v) = smp->stateByScxmlName(name)->active();
                } else {
                    // getter for a child statemachine
                    int idx = _id - _t->m_firstSubStateMachineProperty;
                    *reinterpret_cast<QScxmlStateMachine **>(_v) = _t->m_subStateMachines.at(idx);
                }
            }
        }
    }
    // end of Q_OBJECT macro

private:
    friend QStateMachineBuilder;
    DynamicStateMachine()
        : m_metaObject(Q_NULLPTR)
        , m_firstSubStateMachineSignal(0)
        , m_firstSlot(0)
        , m_firstSlotWithoutData(0)
        , m_firstSubStateMachineProperty(0)
    {
        // Temporarily wire up the QMetaObject, because qobject_cast needs it while building MyQStateMachine.
        QMetaObjectBuilder b;
        b.setClassName("DynamicStateMachine");
        b.setSuperClass(&QScxmlStateMachine::staticMetaObject);
        b.setStaticMetacallFunction(qt_static_metacall);
        m_metaObject = b.toMetaObject();

        setScxmlEventFilter(this);
    }

    void initDynamicParts(const QSet<QString> &eventSignals,
                          const QSet<QString> &eventSlots,
                          const QList<QString> &stateNames,
                          const QList<QString> &subStateMachineNames)
    {
        // Release the temporary QMetaObject.
        Q_ASSERT(m_metaObject);
        free(m_metaObject);

        m_eventNamesByIndex.reserve(eventSignals.size() + subStateMachineNames.size() + eventSlots.size());

        // Build the real one.
        QMetaObjectBuilder b;
        b.setClassName("DynamicStateMachine");
        b.setSuperClass(&QScxmlStateMachine::staticMetaObject);
        b.setStaticMetacallFunction(qt_static_metacall);

        // signals
        foreach (const QString &eventName, eventSignals) {
            QByteArray signalName = eventName.toUtf8() + "(const QVariant &)";
            QMetaMethodBuilder signalBuilder = b.addSignal(signalName);
            signalBuilder.setParameterNames(init("data"));
            int idx = signalBuilder.index();
            m_eventNamesByIndex.resize(std::max(idx + 1, m_eventNamesByIndex.size()));
            m_eventNamesByIndex[idx] = eventName;
        }

        m_firstStateChangedSignal = m_eventNamesByIndex.size();
        foreach (const QString &stateName, stateNames) {
            auto name = stateName.toUtf8();
            QByteArray signalName = name + "Changed(bool)";
            QMetaMethodBuilder signalBuilder = b.addSignal(signalName);
            signalBuilder.setParameterNames(init("active"));
            int idx = signalBuilder.index();
            m_eventNamesByIndex.resize(std::max(idx + 1, m_eventNamesByIndex.size()));
        }

        m_firstSubStateMachineSignal = m_eventNamesByIndex.size();
        foreach (const QString &machineName, subStateMachineNames) {
            auto name = machineName.toUtf8();
            QByteArray signalName = name + "Changed(QScxmlStateMachine *)";
            QMetaMethodBuilder signalBuilder = b.addSignal(signalName);
            signalBuilder.setParameterNames(init("statemachine"));
            int idx = signalBuilder.index();
            m_eventNamesByIndex.resize(std::max(idx + 1, m_eventNamesByIndex.size()));
        }

        // slots
        m_firstSlot = m_eventNamesByIndex.size();
        foreach (const QString &eventName, eventSlots) {
            QByteArray slotName = eventName.toUtf8() + "(const QVariant &)";
            QMetaMethodBuilder slotBuilder = b.addSlot(slotName);
            slotBuilder.setParameterNames(init("data"));
            int idx = slotBuilder.index();
            m_eventNamesByIndex.resize(std::max(idx + 1, m_eventNamesByIndex.size()));
            m_eventNamesByIndex[idx] = eventName;
        }

        m_firstSlotWithoutData = m_eventNamesByIndex.size();
        foreach (const QString &eventName, eventSlots) {
            QByteArray slotName = eventName.toUtf8() + "()";
            QMetaMethodBuilder slotBuilder = b.addSlot(slotName);
            int idx = slotBuilder.index();
            m_eventNamesByIndex.resize(std::max(idx + 1, m_eventNamesByIndex.size()));
            m_eventNamesByIndex[idx] = eventName;
        }

        // properties
        int stateNotifier = m_firstStateChangedSignal;
        foreach (const QString &stateName, stateNames) {
            QMetaPropertyBuilder prop = b.addProperty(stateName.toUtf8(), "bool", stateNotifier);
            prop.setWritable(false);
            int idx = prop.index();
            m_propertyNamesByIndex.resize(std::max(idx + 1, m_propertyNamesByIndex.size()));
            m_propertyNamesByIndex[idx] = stateName;
            ++stateNotifier;
        }

        m_firstSubStateMachineProperty = m_propertyNamesByIndex.size();
        int notifier = m_firstSubStateMachineSignal;
        foreach (const QString &machineName, subStateMachineNames) {
            QMetaPropertyBuilder prop = b.addProperty(machineName.toUtf8(), "QScxmlStateMachine *", notifier);
            prop.setWritable(false);
            int idx = prop.index();
            m_propertyNamesByIndex.resize(std::max(idx + 1, m_propertyNamesByIndex.size()));
            m_propertyNamesByIndex[idx] = machineName;
            ++notifier;
        }
        m_subStateMachines.resize(subStateMachineNames.size());

        // And we're done
        m_metaObject = b.toMetaObject();
    }

public:
    ~DynamicStateMachine()
    { if (m_metaObject) free(m_metaObject); }

    bool handle(QScxmlEvent *event, QScxmlStateMachine *stateMachine) Q_DECL_OVERRIDE {
        Q_UNUSED(stateMachine);

        if (event->originType() != QStringLiteral("qt:signal")) {
            return true;
        }

        auto eventName = event->name();
        for (int i = 0; i < m_firstSubStateMachineSignal; ++i) {
            if (m_eventNamesByIndex.at(i) == eventName) {
                QVariant data = event->data();
                void *argv[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&data)) };
                QMetaObject::activate(this, metaObject(), i, argv);
                return false;
            }
        }

        return true;
    }

protected:
    void setService(const QString &id, QScxmlInvokableService *service) Q_DECL_OVERRIDE
    {
        int idx = -1;
        for (int i = m_firstSubStateMachineProperty, ei = m_propertyNamesByIndex.size(); i != ei; ++i) {
            if (m_propertyNamesByIndex.at(i) == id) {
                idx = i - m_firstSubStateMachineProperty;
                break;
            }
        }
        if (idx < 0)
            return;
        auto scxml = service ? dynamic_cast<QScxmlInvokableScxml *>(service) : Q_NULLPTR;
        auto machine = scxml ? scxml->stateMachine() : Q_NULLPTR;
        if (m_subStateMachines.at(idx) != machine) {
            m_subStateMachines[idx] = machine;
            // emit changed signal:
            void *argv[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&machine)) };
            QMetaObject::activate(this, metaObject(), m_firstSubStateMachineSignal + idx, argv);
        }
    }

private:
    static QList<QByteArray> init(const char *s)
    {
#ifdef Q_COMPILER_INITIALIZER_LISTS
        return QList<QByteArray>({ QByteArray::fromRawData(s, int(strlen(s))) });
#else // insane compiler:
        return QList<QByteArray>() << QByteArray::fromRawData(s, int(strlen(s)));
#endif
    }

private:
    QMetaObject *m_metaObject;
    QVector<QString> m_eventNamesByIndex;
    QVector<QString> m_propertyNamesByIndex;
    QVector<QScxmlStateMachine *> m_subStateMachines;
    int m_firstSubStateMachineSignal;
    int m_firstStateChangedSignal;
    int m_firstSlot;
    int m_firstSlotWithoutData;
    int m_firstSubStateMachineProperty;
};

class InvokeDynamicScxmlFactory: public QScxmlInvokableScxmlServiceFactory
{
public:
    InvokeDynamicScxmlFactory(QScxmlExecutableContent::StringId invokeLocation,
                              QScxmlExecutableContent::StringId id,
                              QScxmlExecutableContent::StringId idPrefix,
                              QScxmlExecutableContent::StringId idlocation,
                              const QVector<QScxmlExecutableContent::StringId> &namelist,
                              bool autoforward,
                              const QVector<Param> &params,
                              QScxmlExecutableContent::ContainerId finalize)
        : QScxmlInvokableScxmlServiceFactory(invokeLocation, id, idPrefix, idlocation, namelist, autoforward, params, finalize)
    {}

    void setContent(const QSharedPointer<DocumentModel::ScxmlDocument> &content)
    { m_content = content; }

    QScxmlInvokableService *invoke(QScxmlStateMachine *child) Q_DECL_OVERRIDE;

private:
    QSharedPointer<DocumentModel::ScxmlDocument> m_content;
};

class QStateMachineBuilder: public QScxmlExecutableContent::Builder
{
public:
    QStateMachineBuilder()
        : m_stateMachine(Q_NULLPTR)
        , m_currentTransition(Q_NULLPTR)
        , m_bindLate(false)
        , m_qtMode(false)
    {}

    QScxmlStateMachine *build(DocumentModel::ScxmlDocument *doc)
    {
        m_stateMachine = Q_NULLPTR;
        m_parents.reserve(32);
        m_allTransitions.reserve(doc->allTransitions.size());
        m_docStatesToQStates.reserve(doc->allStates.size());
        m_qtMode = doc->qtMode;

        doc->root->accept(this);
        wireTransitions();
        applyInitialStates();

        QScxmlExecutableContent::DynamicTableData *td = tableData();
        td->setParent(m_stateMachine);
        m_stateMachine->setTableData(td);
        m_stateMachine->initDynamicParts(m_eventSignals, m_eventSlots, m_stateNames.keys(), m_subStateMachineNames.toList());

        const auto signalCode = QByteArray::number(QSIGNAL_CODE);
        for (auto it = m_stateNames.constBegin(), eit = m_stateNames.constEnd(); it != eit; ++it) {
            QByteArray signal = signalCode + it.key().toUtf8() + "Changed(bool)";
            QObject::connect(it.value(), SIGNAL(activeChanged(bool)), m_stateMachine, signal.constData());
        }

        m_parents.clear();
        m_allTransitions.clear();
        m_docStatesToQStates.clear();
        m_currentTransition = Q_NULLPTR;

        return m_stateMachine;
    }

private:
    using NodeVisitor::visit;
    using QScxmlExecutableContent::Builder::createContext;

    bool visit(DocumentModel::Scxml *node) Q_DECL_OVERRIDE
    {
        m_stateMachine = new DynamicStateMachine;

        switch (node->binding) {
        case DocumentModel::Scxml::EarlyBinding:
            m_stateMachine->setDataBinding(QScxmlStateMachine::EarlyBinding);
            break;
        case DocumentModel::Scxml::LateBinding:
            m_stateMachine->setDataBinding(QScxmlStateMachine::LateBinding);
            m_bindLate = true;
            break;
        default:
            Q_UNREACHABLE();
        }

        setName(node->name);

        m_parents.append(QScxmlStateMachinePrivate::get(m_stateMachine)->m_qStateMachine);
        visit(node->children);

        m_dataElements.append(node->dataElements);
        if (node->script || !m_dataElements.isEmpty() || !node->initialSetup.isEmpty()) {
            setInitialSetup(startNewSequence());
            generate(m_dataElements);
            if (node->script) {
                node->script->accept(this);
            }
            visit(&node->initialSetup);
            endSequence();
        }

        m_parents.removeLast();

        foreach (auto initialState, node->initialStates) {
            Q_ASSERT(initialState);
            m_initialStates.append(qMakePair(QScxmlStateMachinePrivate::get(m_stateMachine)->m_qStateMachine, initialState));
        }

        return false;
    }

    bool visit(DocumentModel::State *node) Q_DECL_OVERRIDE
    {
        QAbstractState *newState = Q_NULLPTR;
        switch (node->type) {
        case DocumentModel::State::Normal: {
            auto s = new QScxmlState(currentParent());
            newState = s;
            foreach (DocumentModel::AbstractState *initialState, node->initialStates) {
                m_initialStates.append(qMakePair(s, initialState));
            }
        } break;
        case DocumentModel::State::Parallel: {
            auto s = new QScxmlState(currentParent());
            s->setChildMode(QState::ParallelStates);
            newState = s;
        } break;
        case DocumentModel::State::Initial: {
            auto s = new QScxmlState(currentParent());
            currentParent()->setInitialState(s);
            newState = s;
        } break;
        case DocumentModel::State::Final: {
            auto s = new QScxmlFinalState(currentParent());
            newState = s;
            s->setDoneData(generate(node->doneData));
        } break;
        default:
            Q_UNREACHABLE();
        }

        newState->setObjectName(node->id);
        m_stateNames.insert(node->id, newState);

        m_docStatesToQStates.insert(node, newState);
        m_parents.append(newState);

        if (!node->dataElements.isEmpty()) {
            if (m_bindLate) {
                qobject_cast<QScxmlState *>(newState)->setInitInstructions(startNewSequence());
                generate(node->dataElements);
                endSequence();
            } else {
                m_dataElements.append(node->dataElements);
            }
        }

        QScxmlExecutableContent::ContainerId onEntry = generate(node->onEntry);
        QScxmlExecutableContent::ContainerId onExit = generate(node->onExit);
        if (QScxmlState *s = qobject_cast<QScxmlState *>(newState)) {
            s->setOnEntryInstructions(onEntry);
            s->setOnExitInstructions(onExit);
            if (!node->invokes.isEmpty()) {
                QVector<QScxmlInvokableServiceFactory *> factories;
                foreach (DocumentModel::Invoke *invoke, node->invokes) {
                    auto ctxt = createContext(QStringLiteral("invoke"));
                    QVector<QScxmlExecutableContent::StringId> namelist;
                    foreach (const QString &name, invoke->namelist)
                        namelist += addString(name);
                    QVector<QScxmlInvokableServiceFactory::Param> params;
                    foreach (DocumentModel::Param *param, invoke->params) {
                        QScxmlInvokableServiceFactory::Param p;
                        p.name = addString(param->name);
                        p.expr = createEvaluatorVariant(QStringLiteral("param"), QStringLiteral("expr"), param->expr);
                        p.location = addString(param->location);
                        params.append(p);
                    }
                    QScxmlExecutableContent::ContainerId finalize = QScxmlExecutableContent::NoInstruction;
                    if (!invoke->finalize.isEmpty()) {
                        finalize = startNewSequence();
                        visit(&invoke->finalize);
                        endSequence();
                    }
                    auto factory = new InvokeDynamicScxmlFactory(ctxt,
                                                                 addString(invoke->id),
                                                                 addString(node->id + QStringLiteral(".session-")),
                                                                 addString(invoke->idLocation),
                                                                 namelist,
                                                                 invoke->autoforward,
                                                                 params,
                                                                 finalize);
                    factory->setContent(invoke->content);
                    factories.append(factory);
                    QString name = invoke->content->root->name;
                    if (!name.isEmpty()) {
                        m_subStateMachineNames.insert(name);
                    }
                }
                s->setInvokableServiceFactories(factories);
            }
        } else if (QScxmlFinalState *f = qobject_cast<QScxmlFinalState *>(newState)) {
            f->setOnEntryInstructions(onEntry);
            f->setOnExitInstructions(onExit);
        } else {
            Q_UNREACHABLE();
        }

        visit(node->children);

        m_parents.removeLast();
        return false;
    }

    bool visit(DocumentModel::Transition *node) Q_DECL_OVERRIDE
    {
        if (m_qtMode) {
            m_eventSlots.unite(node->events.toSet());
        }

        auto newTransition = new QScxmlTransition(node->events);
        if (QHistoryState *parent = qobject_cast<QHistoryState*>(m_parents.last())) {
            parent->setDefaultTransition(newTransition);
        } else {
            currentParent()->addTransition(newTransition);
        }

        if (node->condition) {
            auto cond = createEvaluatorBool(QStringLiteral("transition"), QStringLiteral("cond"), *node->condition.data());
            newTransition->setConditionalExpression(cond);
        }

        switch (node->type) {
        case DocumentModel::Transition::External:
            newTransition->setTransitionType(QAbstractTransition::ExternalTransition);
            break;
        case DocumentModel::Transition::Internal:
            newTransition->setTransitionType(QAbstractTransition::InternalTransition);
            break;
        default:
            Q_UNREACHABLE();
        }

        m_allTransitions.insert(newTransition, node);
        if (!node->instructionsOnTransition.isEmpty()) {
            m_currentTransition = newTransition;
            newTransition->setInstructionsOnTransition(startNewSequence());
            visit(&node->instructionsOnTransition);
            endSequence();
            m_currentTransition = 0;
        }
        Q_ASSERT(newTransition->stateMachine());
        return false;
    }

    bool visit(DocumentModel::HistoryState *state) Q_DECL_OVERRIDE
    {
        QHistoryState *newState = new QScxmlHistoryState(currentParent());
        switch (state->type) {
        case DocumentModel::HistoryState::Shallow:
            newState->setHistoryType(QHistoryState::ShallowHistory);
            break;
        case DocumentModel::HistoryState::Deep:
            newState->setHistoryType(QHistoryState::DeepHistory);
            break;
        default:
            Q_UNREACHABLE();
        }

        newState->setObjectName(state->id);
        m_docStatesToQStates.insert(state, newState);
        m_parents.append(newState);
        return true;
    }

    void endVisit(DocumentModel::HistoryState *) Q_DECL_OVERRIDE
    {
        m_parents.removeLast();
    }

    bool visit(DocumentModel::Send *node) Q_DECL_OVERRIDE
    {
        if (m_qtMode && node->type == QStringLiteral("qt:signal")) {
            m_eventSignals.insert(node->event);
        }

        return QScxmlExecutableContent::Builder::visit(node);
    }

private: // Utility methods
    QState *currentParent() const
    {
        if (m_parents.isEmpty())
            return Q_NULLPTR;

        QState *parent = qobject_cast<QState*>(m_parents.last());
        Q_ASSERT(parent);
        return parent;
    }

    void wireTransitions()
    {
        for (QHash<QAbstractTransition *, DocumentModel::Transition*>::const_iterator i = m_allTransitions.begin(), ei = m_allTransitions.end(); i != ei; ++i) {
            QList<QAbstractState *> targets;
            targets.reserve(i.value()->targets.size());
            foreach (DocumentModel::AbstractState *targetState, i.value()->targetStates) {
                QAbstractState *target = m_docStatesToQStates.value(targetState);
                Q_ASSERT(target);
                targets.append(target);
            }
            i.key()->setTargetStates(targets);

            if (DebugHelper_NameTransitions)
                i.key()->setObjectName(QStringLiteral("%1 -> %2").arg(i.key()->parent()->objectName(), i.value()->targets.join(QStringLiteral(","))));
        }
    }

    void applyInitialStates()
    {
        foreach (const auto &init, m_initialStates) {
            Q_ASSERT(init.second);
            auto initialState = m_docStatesToQStates.value(init.second);
            Q_ASSERT(initialState);
            init.first->setInitialState(initialState);
        }
    }

    QString createContextString(const QString &instrName) const Q_DECL_OVERRIDE
    {
        if (m_currentTransition) {
            QString state;
            if (QState *s = m_currentTransition->sourceState()) {
                state = QStringLiteral(" of state '%1'").arg(s->objectName());
            }
            return QStringLiteral("%1 instruction in transition %2 %3").arg(instrName, m_currentTransition->objectName(), state);
        } else {
            return QStringLiteral("%1 instruction in state %2").arg(instrName, m_parents.last()->objectName());
        }
    }

    QString createContext(const QString &instrName, const QString &attrName, const QString &attrValue) const Q_DECL_OVERRIDE
    {
        QString location = createContextString(instrName);
        return QStringLiteral("%1 with %2=\"%3\"").arg(location, attrName, attrValue);
    }

private:
    DynamicStateMachine *m_stateMachine;
    QVector<QAbstractState *> m_parents;
    QHash<QAbstractTransition *, DocumentModel::Transition*> m_allTransitions;
    QHash<DocumentModel::AbstractState *, QAbstractState *> m_docStatesToQStates;
    QAbstractTransition *m_currentTransition;
    QVector<QPair<QState *, DocumentModel::AbstractState *>> m_initialStates;
    bool m_bindLate;
    bool m_qtMode;
    QVector<DocumentModel::DataElement *> m_dataElements;
    QSet<QString> m_eventSignals;
    QSet<QString> m_eventSlots;
    QHash<QString, QAbstractState *> m_stateNames;
    QSet<QString> m_subStateMachineNames;
};

inline QScxmlInvokableService *InvokeDynamicScxmlFactory::invoke(QScxmlStateMachine *parent)
{
    auto child = QStateMachineBuilder().build(m_content.data());

    auto dm = QScxmlDataModelPrivate::instantiateDataModel(m_content->root->dataModel);
    dm->setParent(child);
    child->setDataModel(dm);

    return finishInvoke(child, parent);
}
#endif // BUILD_QSCXMLC

} // anonymous namespace

/*!
 * \class QScxmlParser
 * \brief The QScxmlParser class is a parser for SCXML files.
 * \since 5.7
 * \inmodule QtScxml
 *
 * Parses an \l{SCXML Specification}{SCXML} file. It can also dynamically instantiate a
 * state machine for a successfully parsed SCXML file. If parsing failed and
 * instantiateStateMachine() is called, the new state machine cannot start. All errors are
 * returned by QScxmlStateMachine::parseErrors().
 *
 * To load an SCXML file, QScxmlStateMachine::fromFile or QScxmlStateMachine::fromData should be
 * used. Using QScxmlParser directly is only needed when the parser needs to use a custom
 * QScxmlParser::Loader.
 */

/*!
    \enum QScxmlParser::State

    This enum specifies the state the parser is in.

    \value  StartingParsing
            The state before parse() is called.
    \value  ParsingScxml
            The state when parsing, before any errors have occurred.
    \value  ParsingError
            The final state, indicating that an error occurred while parsing.
    \value  FinishedParsing
            The final state, indicating that the file was successfully parsed.
 */

/*!
    \enum QScxmlParser::QtMode

    This enum specifies if the document should be parsed in Qt mode. In Qt
    mode, event and state names have to be valid C++ identifiers. If that is
    the case some additional convenience methods are generated. If not, the
    parser will reject the document. Qt mode can be enabled in the document
    itself by adding an XML comment of the form:

    \c {<!-- enable-qt-mode: yes -->}

    \value QtModeDisabled
           Ignore the XML comment and do not generate additional methods.
    \value QtModeEnabled
           Force parsing in Qt mode and try to generate the additional methods,
           no matter if the XML comment is present.
    \value QtModeFromInputFile
           Enable Qt mode only if the XML comment is present in the document.
 */

/*!
 * Creates a new SCXML parser for the specified \a reader.
 */
QScxmlParser::QScxmlParser(QXmlStreamReader *reader)
    : d(new QScxmlParserPrivate(this, reader))
{ }

/*!
 * Destroys the SCXML parser.
 */
QScxmlParser::~QScxmlParser()
{
    delete d;
}

/*!
 * Returns the file name associated with the current input.
 *
 * \sa setFileName()
 */
QString QScxmlParser::fileName() const
{
    return d->fileName();
}

/*!
 * Sets the file name for the current input to \a fileName.
 *
 * The file name is used for error reporting and for resolving relative path URIs.
 *
 * \sa fileName()
 */
void QScxmlParser::setFileName(const QString &fileName)
{
    d->setFileName(fileName);
}

/*!
 * Returns the loader that is currently used to resolve and load URIs.
 *
 * \sa setLoader()
 */
QScxmlParser::Loader *QScxmlParser::loader() const
{
    return d->loader();
}

/*!
 * Sets \a newLoader to be used for resolving and loading URIs.
 *
 * \sa loader()
 */
void QScxmlParser::setLoader(QScxmlParser::Loader *newLoader)
{
    d->setLoader(newLoader);
}

/*!
 * Parses an SCXML file.
 */
void QScxmlParser::parse()
{
    d->readDocument();
    d->verifyDocument();
}

/*!
 * Instantiates a new state machine from the parsed SCXML.
 *
 * If parsing is successful, the returned state machine can be initialized and started. If
 * parsing fails, QScxmlStateMachine::parseErrors() can be used to retrieve a list of errors.
 *
 * \note The instantiated state machine will not have an associated data model set.
 * \sa QScxmlParser::instantiateDataModel
 */
QScxmlStateMachine *QScxmlParser::instantiateStateMachine() const
{
#ifdef BUILD_QSCXMLC
    return Q_NULLPTR;
#else // BUILD_QSCXMLC
    DocumentModel::ScxmlDocument *doc = d->scxmlDocument();
    if (doc && doc->root) {
        return QStateMachineBuilder().build(doc);
    } else {
        class InvalidStateMachine: public QScxmlStateMachine {
        public:
            InvalidStateMachine()
            {}
        };

        auto stateMachine = new InvalidStateMachine;
        QScxmlStateMachinePrivate::get(stateMachine)->parserData()->m_errors = errors();
        return stateMachine;
    }
#endif // BUILD_QSCXMLC
}

/*!
 * Instantiates the data model as described in the SCXML file.
 *
 * After instantiation, the \a stateMachine takes ownership of the data model.
 */
void QScxmlParser::instantiateDataModel(QScxmlStateMachine *stateMachine) const
{
#ifdef BUILD_QSCXMLC
    Q_UNUSED(stateMachine)
#else
    auto doc = d->scxmlDocument();
    auto root = doc ? doc->root : Q_NULLPTR;
    if (root == Q_NULLPTR) {
        qWarning() << "SCXML document has no root element";
    } else {
        QScxmlDataModel *dm = QScxmlDataModelPrivate::instantiateDataModel(root->dataModel);
        QScxmlStateMachinePrivate::get(stateMachine)->parserData()->m_ownedDataModel.reset(dm);
        stateMachine->setDataModel(dm);
        if (dm == Q_NULLPTR)
            qWarning() << "No data-model instantiated";
    }
#endif // BUILD_QSCXMLC
}

/*!
 * Returns the list of parse errors.
 */
QVector<QScxmlError> QScxmlParser::errors() const
{
    return d->errors();
}

/*!
 * Adds the error message \a msg.
 *
 * The line and column numbers for the error message are the current line and
 * column numbers of the QXmlStreamReader.
 */
void QScxmlParser::addError(const QString &msg)
{
    d->addError(msg);
}

/*!
 * Returns how the parser decides if the SCXML document should conform to Qt
 * mode.
 *
 * \sa QtMode
 */
QScxmlParser::QtMode QScxmlParser::qtMode() const
{
    return d->qtMode();
}

/*!
 * Sets the \c qtMode to \a mode. This property overrides the XML comment. You
 * can force Qt mode to be used by setting it to \c QtModeEnabled or force any
 * XML comments to be ignored and Qt mode to be used by setting it to
 * \c QtModeDisabled. The default is \c QtModeFromInputFile, which will switch
 * Qt mode on if the XML comment is present in the source file.
 *
 * \sa QtMode
 */
void QScxmlParser::setQtMode(QScxmlParser::QtMode mode)
{
    d->setQtMode(mode);
}

bool QScxmlParserPrivate::ParserState::collectChars() {
    switch (kind) {
    case Content:
    case Data:
    case Script:
        return true;
    default:
        break;
    }
    return false;
}

bool QScxmlParserPrivate::ParserState::validChild(ParserState::Kind child) const {
    return validChild(kind, child);
}

bool QScxmlParserPrivate::ParserState::validChild(ParserState::Kind parent, ParserState::Kind child)
{
    switch (parent) {
    case ParserState::Scxml:
        switch (child) {
        case ParserState::State:
        case ParserState::Parallel:
        case ParserState::Final:
        case ParserState::DataModel:
        case ParserState::Script:
        case ParserState::Transition:
            return true;
        default:
            break;
        }
        return false;
    case ParserState::State:
        switch (child) {
        case ParserState::OnEntry:
        case ParserState::OnExit:
        case ParserState::Transition:
        case ParserState::Initial:
        case ParserState::State:
        case ParserState::Parallel:
        case ParserState::Final:
        case ParserState::History:
        case ParserState::DataModel:
        case ParserState::Invoke:
            return true;
        default:
            break;
        }
        return false;
    case ParserState::Parallel:
        switch (child) {
        case ParserState::OnEntry:
        case ParserState::OnExit:
        case ParserState::Transition:
        case ParserState::State:
        case ParserState::Parallel:
        case ParserState::History:
        case ParserState::DataModel:
        case ParserState::Invoke:
            return true;
        default:
            break;
        }
        return false;
    case ParserState::Transition:
        return isExecutableContent(child);
    case ParserState::Initial:
        return (child == ParserState::Transition);
    case ParserState::Final:
        switch (child) {
        case ParserState::OnEntry:
        case ParserState::OnExit:
        case ParserState::DoneData:
            return true;
        default:
            break;
        }
        return false;
    case ParserState::OnEntry:
    case ParserState::OnExit:
        return isExecutableContent(child);
    case ParserState::History:
        return child == ParserState::Transition;
    case ParserState::Raise:
        return false;
    case ParserState::If:
        return child == ParserState::ElseIf || child == ParserState::Else
                || isExecutableContent(child);
    case ParserState::ElseIf:
    case ParserState::Else:
        return false;
    case ParserState::Foreach:
        return isExecutableContent(child);
    case ParserState::Log:
        return false;
    case ParserState::DataModel:
        return (child == ParserState::Data);
    case ParserState::Data:
        return false;
    case ParserState::Assign:
        return false;
    case ParserState::DoneData:
    case ParserState::Send:
        return child == ParserState::Content || child == ParserState::Param;
    case ParserState::Content:
        return child == ParserState::Scxml || isExecutableContent(child);
    case ParserState::Param:
    case ParserState::Cancel:
        return false;
    case ParserState::Finalize:
        return isExecutableContent(child);
    case ParserState::Invoke:
        return child == ParserState::Content || child == ParserState::Finalize
                || child == ParserState::Param;
    case ParserState::Script:
    case ParserState::None:
        break;
    }
    return false;
}

bool QScxmlParserPrivate::ParserState::isExecutableContent(ParserState::Kind kind) {
    switch (kind) {
    case Raise:
    case Send:
    case Log:
    case Script:
    case Assign:
    case If:
    case Foreach:
    case Cancel:
    case Invoke:
        return true;
    default:
        break;
    }
    return false;
}

QScxmlParserPrivate::ParserState::Kind QScxmlParserPrivate::ParserState::nameToParserStateKind(const QStringRef &name)
{
    static QMap<QString, ParserState::Kind> nameToKind;
    if (nameToKind.isEmpty()) {
        nameToKind.insert(QLatin1String("scxml"),       Scxml);
        nameToKind.insert(QLatin1String("state"),       State);
        nameToKind.insert(QLatin1String("parallel"),    Parallel);
        nameToKind.insert(QLatin1String("transition"),  Transition);
        nameToKind.insert(QLatin1String("initial"),     Initial);
        nameToKind.insert(QLatin1String("final"),       Final);
        nameToKind.insert(QLatin1String("onentry"),     OnEntry);
        nameToKind.insert(QLatin1String("onexit"),      OnExit);
        nameToKind.insert(QLatin1String("history"),     History);
        nameToKind.insert(QLatin1String("raise"),       Raise);
        nameToKind.insert(QLatin1String("if"),          If);
        nameToKind.insert(QLatin1String("elseif"),      ElseIf);
        nameToKind.insert(QLatin1String("else"),        Else);
        nameToKind.insert(QLatin1String("foreach"),     Foreach);
        nameToKind.insert(QLatin1String("log"),         Log);
        nameToKind.insert(QLatin1String("datamodel"),   DataModel);
        nameToKind.insert(QLatin1String("data"),        Data);
        nameToKind.insert(QLatin1String("assign"),      Assign);
        nameToKind.insert(QLatin1String("donedata"),    DoneData);
        nameToKind.insert(QLatin1String("content"),     Content);
        nameToKind.insert(QLatin1String("param"),       Param);
        nameToKind.insert(QLatin1String("script"),      Script);
        nameToKind.insert(QLatin1String("send"),        Send);
        nameToKind.insert(QLatin1String("cancel"),      Cancel);
        nameToKind.insert(QLatin1String("invoke"),      Invoke);
        nameToKind.insert(QLatin1String("finalize"),    Finalize);
    }
    QMap<QString, ParserState::Kind>::ConstIterator it = nameToKind.constBegin();
    const QMap<QString, ParserState::Kind>::ConstIterator itEnd = nameToKind.constEnd();
    while (it != itEnd) {
        if (it.key() == name)
            return it.value();
        ++it;
    }
    return None;
}

QStringList QScxmlParserPrivate::ParserState::requiredAttributes(QScxmlParserPrivate::ParserState::Kind kind)
{
    switch (kind) {
    case Scxml:      return QStringList() << QStringLiteral("version");
    case State:      return QStringList();
    case Parallel:   return QStringList();
    case Transition: return QStringList();
    case Initial:    return QStringList();
    case Final:      return QStringList();
    case OnEntry:    return QStringList();
    case OnExit:     return QStringList();
    case History:    return QStringList();
    case Raise:      return QStringList() << QStringLiteral("event");
    case If:         return QStringList() << QStringLiteral("cond");
    case ElseIf:     return QStringList() << QStringLiteral("cond");
    case Else:       return QStringList();
    case Foreach:    return QStringList() << QStringLiteral("array")
                                          << QStringLiteral("item");
    case Log:        return QStringList();
    case DataModel:  return QStringList();
    case Data:       return QStringList() << QStringLiteral("id");
    case Assign:     return QStringList() << QStringLiteral("location");
    case DoneData:   return QStringList();
    case Content:    return QStringList();
    case Param:      return QStringList() << QStringLiteral("name");
    case Script:     return QStringList();
    case Send:       return QStringList();
    case Cancel:     return QStringList();
    case Invoke:     return QStringList();
    case Finalize:   return QStringList();
    default:         return QStringList();
    }
    return QStringList();
}

QStringList QScxmlParserPrivate::ParserState::optionalAttributes(QScxmlParserPrivate::ParserState::Kind kind)
{
    switch (kind) {
    case Scxml:      return QStringList() << QStringLiteral("initial")
                                          << QStringLiteral("datamodel")
                                          << QStringLiteral("binding")
                                          << QStringLiteral("name");
    case State:      return QStringList() << QStringLiteral("id")
                                          << QStringLiteral("initial");
    case Parallel:   return QStringList() << QStringLiteral("id");
    case Transition: return QStringList() << QStringLiteral("event")
                                          << QStringLiteral("cond")
                                          << QStringLiteral("target")
                                          << QStringLiteral("type");
    case Initial:    return QStringList();
    case Final:      return QStringList() << QStringLiteral("id");
    case OnEntry:    return QStringList();
    case OnExit:     return QStringList();
    case History:    return QStringList() << QStringLiteral("id")
                                          << QStringLiteral("type");
    case Raise:      return QStringList();
    case If:         return QStringList();
    case ElseIf:     return QStringList();
    case Else:       return QStringList();
    case Foreach:    return QStringList() << QStringLiteral("index");
    case Log:        return QStringList() << QStringLiteral("label")
                                          << QStringLiteral("expr");
    case DataModel:  return QStringList();
    case Data:       return QStringList() << QStringLiteral("src")
                                          << QStringLiteral("expr");
    case Assign:     return QStringList() << QStringLiteral("expr");
    case DoneData:   return QStringList();
    case Content:    return QStringList() << QStringLiteral("expr");
    case Param:      return QStringList() << QStringLiteral("expr")
                                          << QStringLiteral("location");
    case Script:     return QStringList() << QStringLiteral("src");
    case Send:       return QStringList() << QStringLiteral("event")
                                          << QStringLiteral("eventexpr")
                                          << QStringLiteral("id")
                                          << QStringLiteral("idlocation")
                                          << QStringLiteral("type")
                                          << QStringLiteral("typeexpr")
                                          << QStringLiteral("namelist")
                                          << QStringLiteral("delay")
                                          << QStringLiteral("delayexpr")
                                          << QStringLiteral("target")
                                          << QStringLiteral("targetexpr");
    case Cancel:     return QStringList() << QStringLiteral("sendid")
                                          << QStringLiteral("sendidexpr");
    case Invoke:     return QStringList() << QStringLiteral("type")
                                          << QStringLiteral("typeexpr")
                                          << QStringLiteral("src")
                                          << QStringLiteral("srcexpr")
                                          << QStringLiteral("id")
                                          << QStringLiteral("idlocation")
                                          << QStringLiteral("namelist")
                                          << QStringLiteral("autoforward");
    case Finalize:   return QStringList();
    default:         return QStringList();
    }
    return QStringList();
}

DocumentModel::Node::~Node()
{
}

void DocumentModel::DataElement::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::Param::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::DoneData::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        foreach (Param *param, params)
            param->accept(visitor);
    }
    visitor->endVisit(this);
}

void DocumentModel::Send::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(params);
    }
    visitor->endVisit(this);
}

void DocumentModel::Invoke::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(params);
        visitor->visit(&finalize);
    }
    visitor->endVisit(this);
}

void DocumentModel::Raise::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::Log::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::Script::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::Assign::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::If::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(blocks);
    }
    visitor->endVisit(this);
}

void DocumentModel::Foreach::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(&block);
    }
    visitor->endVisit(this);
}

void DocumentModel::Cancel::accept(DocumentModel::NodeVisitor *visitor)
{
    visitor->visit(this);
}

void DocumentModel::State::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(dataElements);
        visitor->visit(children);
        visitor->visit(onEntry);
        visitor->visit(onExit);
        if (doneData)
            doneData->accept(visitor);
        foreach (Invoke *invoke, invokes)
            invoke->accept(visitor);
    }
    visitor->endVisit(this);
}

void DocumentModel::Transition::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(&instructionsOnTransition);
    }
    visitor->endVisit(this);
}

void DocumentModel::HistoryState::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        if (Transition *t = defaultConfiguration())
            t->accept(visitor);
    }
    visitor->endVisit(this);
}

void DocumentModel::Scxml::accept(DocumentModel::NodeVisitor *visitor)
{
    if (visitor->visit(this)) {
        visitor->visit(children);
        visitor->visit(dataElements);
        if (script)
            script->accept(visitor);
        visitor->visit(&initialSetup);
    }
    visitor->endVisit(this);
}

DocumentModel::NodeVisitor::~NodeVisitor()
{}

bool DocumentModel::isValidQPropertyName(const QString &str)
{
    return !str.isEmpty() && !str.contains(QLatin1Char('(')) && !str.contains(QLatin1Char(')'));
}

bool DocumentModel::isEventToBeGenerated(const QString &event)
{
    return !event.contains(QLatin1Char('.'));
}

/*!
 * \class QScxmlParser::Loader
 * \brief The Loader class is a URI resolver and resource loader for an SCXML parser.
 * \since 5.7
 * \inmodule QtScxml
 */

/*!
 * Creates a new loader for the specified \a parser.
 */
QScxmlParser::Loader::Loader(QScxmlParser *parser)
    : m_parser(parser)
{
    Q_ASSERT(parser);
}

/*!
 * Destroys the loader.
 */
QScxmlParser::Loader::~Loader()
{}

/*!
 * Returns the parser that is associated with this loader.
 */
QScxmlParser *QScxmlParser::Loader::parser() const
{
    return m_parser;
}

/*!
 * \fn QScxmlParser::Loader::load(const QString &name, const QString &baseDir, bool *ok)
 * Resolves the URI \a name and loads an SCXML file from the directory
 * specified by \a baseDir. The boolean parameter \a ok indicates whether
 * the loading was successful.
 *
 * Returns a QByteArray that stores the contents of the file.
 */

QScxmlParserPrivate *QScxmlParserPrivate::get(QScxmlParser *parser)
{
    return parser->d;
}

QScxmlParserPrivate::QScxmlParserPrivate(QScxmlParser *parser, QXmlStreamReader *reader)
    : m_currentState(Q_NULLPTR)
    , m_defaultLoader(parser)
    , m_loader(&m_defaultLoader)
    , m_reader(reader)
    , m_qtMode(QScxmlParser::QtModeFromInputFile)
{}

bool QScxmlParserPrivate::verifyDocument()
{
    if (!m_doc)
        return false;

    auto handler = [this](const DocumentModel::XmlLocation &location, const QString &msg) {
        this->addError(location, msg);
    };

    if (ScxmlVerifier(handler).verify(m_doc.data()))
        return true;
    else
        return false;
}

DocumentModel::ScxmlDocument *QScxmlParserPrivate::scxmlDocument() const
{
    return m_doc && m_errors.isEmpty() ? m_doc.data() : Q_NULLPTR;
}

QString QScxmlParserPrivate::fileName() const
{
    return m_fileName;
}

void QScxmlParserPrivate::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

QScxmlParser::Loader *QScxmlParserPrivate::loader() const
{
    return m_loader;
}

void QScxmlParserPrivate::setLoader(QScxmlParser::Loader *loader)
{
    m_loader = loader;
}

void QScxmlParserPrivate::parseSubDocument(DocumentModel::Invoke *parentInvoke, QXmlStreamReader *reader, const QString &fileName)\
{
    QScxmlParser p(reader);
    p.setFileName(fileName);
    p.d->readDocument();
    parentInvoke->content.reset(p.d->m_doc.take());
    m_doc->allSubDocuments.append(parentInvoke->content.data());
    m_errors.append(p.errors());
}

bool QScxmlParserPrivate::parseSubElement(DocumentModel::Invoke *parentInvoke, QXmlStreamReader *reader, const QString &fileName)
{
    QScxmlParser p(reader);
    p.setFileName(fileName);
    p.d->resetDocument();
    bool ok = p.d->readElement();
    parentInvoke->content.reset(p.d->m_doc.take());
    m_doc->allSubDocuments.append(parentInvoke->content.data());
    m_errors.append(p.errors());
    parentInvoke->content->qtMode = m_doc->qtMode;
    return ok;
}

static bool isWordEnd(const QStringRef &str, int start)
{
    if (str.size() <= start) {
        return true;
    }

    QChar ch = str.at(start);
    return ch.isSpace();
}

bool QScxmlParserPrivate::preReadElementScxml()
{
    if (m_doc->root) {
        addError(QLatin1String("Doc root already allocated"));
        return false;
    }
    m_doc->root = new DocumentModel::Scxml(xmlLocation());

    auto scxml = m_doc->root;
    const QXmlStreamAttributes attributes = m_reader->attributes();
    if (attributes.hasAttribute(QStringLiteral("initial"))) {
        const QString initial = attributes.value(QStringLiteral("initial")).toString();
        scxml->initial += initial.split(QChar::Space, QString::SkipEmptyParts);
    }

    const QStringRef datamodel = attributes.value(QLatin1String("datamodel"));
    if (datamodel.isEmpty() || datamodel == QLatin1String("null")) {
        scxml->dataModel = DocumentModel::Scxml::NullDataModel;
    } else if (datamodel == QLatin1String("ecmascript")) {
        scxml->dataModel = DocumentModel::Scxml::JSDataModel;
    } else if (datamodel.startsWith(QLatin1String("cplusplus"))) {
        scxml->dataModel = DocumentModel::Scxml::CppDataModel;
        int firstColon = datamodel.indexOf(QLatin1Char(':'));
        if (firstColon == -1) {
            scxml->cppDataModelClassName = attributes.value(QStringLiteral("name")).toString() + QStringLiteral("DataModel");
            scxml->cppDataModelHeaderName = scxml->cppDataModelClassName + QStringLiteral(".h");
        } else {
            int lastColon = datamodel.lastIndexOf(QLatin1Char(':'));
            if (lastColon == -1) {
                lastColon = datamodel.length();
            } else {
                scxml->cppDataModelHeaderName = datamodel.mid(lastColon + 1).toString();
            }
            scxml->cppDataModelClassName = datamodel.mid(firstColon + 1, lastColon - firstColon - 1).toString();
        }
    } else {
        addError(QStringLiteral("Unsupported data model '%1' in scxml")
                 .arg(datamodel.toString()));
    }
    const QStringRef binding = attributes.value(QLatin1String("binding"));
    if (binding.isEmpty() || binding == QLatin1String("early")) {
        scxml->binding = DocumentModel::Scxml::EarlyBinding;
    } else if (binding == QLatin1String("late")) {
        scxml->binding = DocumentModel::Scxml::LateBinding;
    } else {
        addError(QStringLiteral("Unsupperted binding type '%1'")
                 .arg(binding.toString()));
        return false;
    }
    const QStringRef name = attributes.value(QLatin1String("name"));
    if (!name.isEmpty()) {
        scxml->name = name.toString();
    }
    m_currentState = m_doc->root;
    current().instructionContainer = &m_doc->root->initialSetup;
    return true;
}


bool QScxmlParserPrivate::preReadElementState()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto newState = m_doc->newState(m_currentState, DocumentModel::State::Normal, xmlLocation());
    if (!maybeId(attributes, &newState->id))
        return false;

    if (attributes.hasAttribute(QStringLiteral("initial"))) {
        const QString initial = attributes.value(QStringLiteral("initial")).toString();
        newState->initial += initial.split(QChar::Space, QString::SkipEmptyParts);
    }
    m_currentState = newState;
    return true;
}

bool QScxmlParserPrivate::preReadElementParallel()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto newState = m_doc->newState(m_currentState, DocumentModel::State::Parallel, xmlLocation());
    if (!maybeId(attributes, &newState->id))
        return false;

    m_currentState = newState;
    return true;
}

bool QScxmlParserPrivate::preReadElementInitial()
{
    DocumentModel::AbstractState *parent = currentParent();
    if (!parent) {
        addError(QStringLiteral("<initial> found outside a state"));
        return false;
    }

    DocumentModel::State *parentState = parent->asState();
    if (!parentState) {
        addError(QStringLiteral("<initial> found outside a state"));
        return false;
    }

    if (parentState->type == DocumentModel::State::Parallel) {
        addError(QStringLiteral("Explicit initial state for parallel states not supported (only implicitly through the initial states of its substates)"));
        return false;
    }
    auto newState = m_doc->newState(m_currentState, DocumentModel::State::Initial, xmlLocation());
    m_currentState = newState;
    return true;
}

bool QScxmlParserPrivate::preReadElementTransition()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto transition = m_doc->newTransition(m_currentState, xmlLocation());
    transition->events = attributes.value(QLatin1String("event")).toString().split(QLatin1Char(' '), QString::SkipEmptyParts);
    transition->targets = attributes.value(QLatin1String("target")).toString().split(QLatin1Char(' '), QString::SkipEmptyParts);
    if (attributes.hasAttribute(QStringLiteral("cond")))
        transition->condition.reset(new QString(attributes.value(QLatin1String("cond")).toString()));
    QStringRef type = attributes.value(QLatin1String("type"));
    if (type.isEmpty() || type == QLatin1String("external")) {
        transition->type = DocumentModel::Transition::External;
    } else if (type == QLatin1String("internal")) {
        transition->type = DocumentModel::Transition::Internal;
    } else {
        addError(QStringLiteral("invalid transition type '%1', valid values are 'external' and 'internal'").arg(type.toString()));
        return true; // TODO: verify me
    }
    current().instructionContainer = &transition->instructionsOnTransition;
    return true;
}

bool QScxmlParserPrivate::preReadElementFinal()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto newState = m_doc->newState(m_currentState, DocumentModel::State::Final, xmlLocation());
    if (!maybeId(attributes, &newState->id))
        return false;
    m_currentState = newState;
    return true;
}

bool QScxmlParserPrivate::preReadElementHistory()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();

    DocumentModel::AbstractState *parent = currentParent();
    if (!parent) {
        addError(QStringLiteral("<history> found outside a state"));
        return false;
    }
    auto newState = m_doc->newHistoryState(parent, xmlLocation());
    if (!maybeId(attributes, &newState->id))
        return false;

    const QStringRef type = attributes.value(QLatin1String("type"));
    if (type.isEmpty() || type == QLatin1String("shallow")) {
        newState->type = DocumentModel::HistoryState::Shallow;
    } else if (type == QLatin1String("deep")) {
        newState->type = DocumentModel::HistoryState::Deep;
    } else {
        addError(QStringLiteral("invalid history type %1, valid values are 'shallow' and 'deep'").arg(type.toString()));
        return false;
    }
    m_currentState = newState;
    return true;
}

bool QScxmlParserPrivate::preReadElementOnEntry()
{
    const ParserState::Kind previousKind = previous().kind;
    switch (previousKind) {
    case ParserState::Final:
    case ParserState::State:
    case ParserState::Parallel:
        if (DocumentModel::State *s = m_currentState->asState()) {
            current().instructionContainer = m_doc->newSequence(&s->onEntry);
            break;
        }
        // intentional fall-through
    default:
        addError(QStringLiteral("unexpected container state for onentry"));
        break;
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementOnExit()
{
    ParserState::Kind previousKind = previous().kind;
    switch (previousKind) {
    case ParserState::Final:
    case ParserState::State:
    case ParserState::Parallel:
        if (DocumentModel::State *s = m_currentState->asState()) {
            current().instructionContainer = m_doc->newSequence(&s->onExit);
            break;
        }
        // intentional fall-through
    default:
        addError(QStringLiteral("unexpected container state for onexit"));
        break;
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementRaise()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto raise = m_doc->newNode<DocumentModel::Raise>(xmlLocation());
    raise->event = attributes.value(QLatin1String("event")).toString();
    current().instruction = raise;
    return true;
}

bool QScxmlParserPrivate::preReadElementIf()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto *ifI = m_doc->newNode<DocumentModel::If>(xmlLocation());
    current().instruction = ifI;
    ifI->conditions.append(attributes.value(QLatin1String("cond")).toString());
    current().instructionContainer = m_doc->newSequence(&ifI->blocks);
    return true;
}

bool QScxmlParserPrivate::preReadElementElseIf()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();

    DocumentModel::If *ifI = lastIf();
    if (!ifI)
        return false;

    ifI->conditions.append(attributes.value(QLatin1String("cond")).toString());
    previous().instructionContainer = m_doc->newSequence(&ifI->blocks);
    return true;
}

bool QScxmlParserPrivate::preReadElementElse()
{
    DocumentModel::If *ifI = lastIf();
    if (!ifI)
        return false;

    previous().instructionContainer = m_doc->newSequence(&ifI->blocks);
    return true;
}

bool QScxmlParserPrivate::preReadElementForeach()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto foreachI = m_doc->newNode<DocumentModel::Foreach>(xmlLocation());
    foreachI->array = attributes.value(QLatin1String("array")).toString();
    foreachI->item = attributes.value(QLatin1String("item")).toString();
    foreachI->index = attributes.value(QLatin1String("index")).toString();
    current().instruction = foreachI;
    current().instructionContainer = &foreachI->block;
    return true;
}

bool QScxmlParserPrivate::preReadElementLog()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto logI = m_doc->newNode<DocumentModel::Log>(xmlLocation());
    logI->label = attributes.value(QLatin1String("label")).toString();
    logI->expr = attributes.value(QLatin1String("expr")).toString();
    current().instruction = logI;
    return true;
}

bool QScxmlParserPrivate::preReadElementDataModel()
{
    return true;
}

bool QScxmlParserPrivate::preReadElementData()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto data = m_doc->newNode<DocumentModel::DataElement>(xmlLocation());
    data->id = attributes.value(QLatin1String("id")).toString();
    data->src = attributes.value(QLatin1String("src")).toString();
    data->expr = attributes.value(QLatin1String("expr")).toString();
    if (DocumentModel::Scxml *scxml = m_currentState->asScxml()) {
        scxml->dataElements.append(data);
    } else if (DocumentModel::State *state = m_currentState->asState()) {
        state->dataElements.append(data);
    } else {
        Q_UNREACHABLE();
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementAssign()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto assign = m_doc->newNode<DocumentModel::Assign>(xmlLocation());
    assign->location = attributes.value(QLatin1String("location")).toString();
    assign->expr = attributes.value(QLatin1String("expr")).toString();
    current().instruction = assign;
    return true;
}

bool QScxmlParserPrivate::preReadElementDoneData()
{
    DocumentModel::State *s = m_currentState->asState();
    if (s && s->type == DocumentModel::State::Final) {
        if (s->doneData) {
            addError(QLatin1String("state can only have one donedata"));
        } else {
            s->doneData = m_doc->newNode<DocumentModel::DoneData>(xmlLocation());
        }
    } else {
        addError(QStringLiteral("donedata can only occur in a final state"));
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementContent()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    ParserState::Kind previousKind = previous().kind;
    switch (previousKind) {
    case ParserState::DoneData: {
        DocumentModel::State *s = m_currentState->asState();
        Q_ASSERT(s);
        s->doneData->expr = attributes.value(QLatin1String("expr")).toString();
    } break;
    case ParserState::Send: {
        DocumentModel::Send *s = previous().instruction->asSend();
        Q_ASSERT(s);
        s->content = attributes.value(QLatin1String("expr")).toString();
    } break;
    case ParserState::Invoke: {
        DocumentModel::Invoke *i = previous().instruction->asInvoke();
        Q_ASSERT(i);
        Q_UNUSED(i);
        if (attributes.hasAttribute(QStringLiteral("expr"))) {
            addError(QStringLiteral("expr attribute in content of invoke is not supported"));
            break;
        }
    } break;
    default:
        addError(QStringLiteral("unexpected parent of content %1").arg(previous().kind));
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementParam()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto param = m_doc->newNode<DocumentModel::Param>(xmlLocation());
    param->name = attributes.value(QLatin1String("name")).toString();
    param->expr = attributes.value(QLatin1String("expr")).toString();
    param->location = attributes.value(QLatin1String("location")).toString();

    ParserState::Kind previousKind = previous().kind;
    switch (previousKind) {
    case ParserState::DoneData: {
        DocumentModel::State *s = m_currentState->asState();
        Q_ASSERT(s);
        Q_ASSERT(s->doneData);
        s->doneData->params.append(param);
    } break;
    case ParserState::Send: {
        DocumentModel::Send *s = previous().instruction->asSend();
        Q_ASSERT(s);
        s->params.append(param);
    } break;
    case ParserState::Invoke: {
        DocumentModel::Invoke *i = previous().instruction->asInvoke();
        Q_ASSERT(i);
        i->params.append(param);
    } break;
    default:
        addError(QStringLiteral("unexpected parent of param %1").arg(previous().kind));
    }
    return true;
}

bool QScxmlParserPrivate::preReadElementScript()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto *script = m_doc->newNode<DocumentModel::Script>(xmlLocation());
    script->src = attributes.value(QLatin1String("src")).toString();
    current().instruction = script;
    return true;
}

bool QScxmlParserPrivate::preReadElementSend()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto *send = m_doc->newNode<DocumentModel::Send>(xmlLocation());
    send->event = attributes.value(QLatin1String("event")).toString();
    send->eventexpr = attributes.value(QLatin1String("eventexpr")).toString();
    send->delay = attributes.value(QLatin1String("delay")).toString();
    send->delayexpr = attributes.value(QLatin1String("delayexpr")).toString();
    send->id = attributes.value(QLatin1String("id")).toString();
    send->idLocation = attributes.value(QLatin1String("idlocation")).toString();
    send->type = attributes.value(QLatin1String("type")).toString();
    send->typeexpr = attributes.value(QLatin1String("typeexpr")).toString();
    send->target = attributes.value(QLatin1String("target")).toString();
    send->targetexpr = attributes.value(QLatin1String("targetexpr")).toString();
    if (attributes.hasAttribute(QLatin1String("namelist")))
        send->namelist = attributes.value(QLatin1String("namelist")).toString().split(QLatin1Char(' '), QString::SkipEmptyParts);
    current().instruction = send;
    return true;
}

bool QScxmlParserPrivate::preReadElementCancel()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    auto *cancel = m_doc->newNode<DocumentModel::Cancel>(xmlLocation());
    cancel->sendid = attributes.value(QLatin1String("sendid")).toString();
    cancel->sendidexpr = attributes.value(QLatin1String("sendidexpr")).toString();
    current().instruction = cancel;
    return true;
}

bool QScxmlParserPrivate::preReadElementInvoke()
{
    const QXmlStreamAttributes attributes = m_reader->attributes();
    DocumentModel::State *parentState = m_currentState->asState();
    if (!parentState ||
            (parentState->type != DocumentModel::State::Normal && parentState->type != DocumentModel::State::Parallel)) {
        addError(QStringLiteral("invoke can only occur in <state> or <parallel>"));
        return true; // TODO: verify me
    }
    auto *invoke = m_doc->newNode<DocumentModel::Invoke>(xmlLocation());
    parentState->invokes.append(invoke);
    invoke->src = attributes.value(QLatin1String("src")).toString();
    invoke->srcexpr = attributes.value(QLatin1String("srcexpr")).toString();
    invoke->id = attributes.value(QLatin1String("id")).toString();
    invoke->idLocation = attributes.value(QLatin1String("idlocation")).toString();
    invoke->type = attributes.value(QLatin1String("type")).toString();
    invoke->typeexpr = attributes.value(QLatin1String("typeexpr")).toString();
    QStringRef autoforwardS = attributes.value(QLatin1String("autoforward"));
    if (QStringRef::compare(autoforwardS, QLatin1String("true"), Qt::CaseInsensitive) == 0
            || QStringRef::compare(autoforwardS, QLatin1String("yes"), Qt::CaseInsensitive) == 0
            || QStringRef::compare(autoforwardS, QLatin1String("t"), Qt::CaseInsensitive) == 0
            || QStringRef::compare(autoforwardS, QLatin1String("y"), Qt::CaseInsensitive) == 0
            || autoforwardS == QLatin1String("1"))
        invoke->autoforward = true;
    else
        invoke->autoforward = false;
    invoke->namelist = attributes.value(QLatin1String("namelist")).toString().split(QLatin1Char(' '), QString::SkipEmptyParts);
    current().instruction = invoke;
    return true;
}

bool QScxmlParserPrivate::preReadElementFinalize()
{
    auto instr = previous().instruction;
    if (!instr) {
        addError(QStringLiteral("no previous instruction found for <finalize>"));
        return false;
    }
    auto invoke = instr->asInvoke();
    if (!invoke) {
        addError(QStringLiteral("instruction before <finalize> is not <invoke>"));
        return false;
    }
    current().instructionContainer = &invoke->finalize;
    return true;
}

bool QScxmlParserPrivate::postReadElementScxml()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementState()
{
    currentStateUp();
    return true;
}

bool QScxmlParserPrivate::postReadElementParallel()
{
    currentStateUp();
    return true;
}

bool QScxmlParserPrivate::postReadElementInitial()
{
    currentStateUp();
    return true;
}

bool QScxmlParserPrivate::postReadElementTransition()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementFinal()
{
    currentStateUp();
    return true;
}

bool QScxmlParserPrivate::postReadElementHistory()
{
    currentStateUp();
    return true;
}

bool QScxmlParserPrivate::postReadElementOnEntry()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementOnExit()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementRaise()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementIf()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementElseIf()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementElse()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementForeach()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementLog()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementDataModel()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementData()
{
    const ParserState parserState = current();
    DocumentModel::DataElement *data = Q_NULLPTR;
    if (auto state = m_currentState->asState()) {
        data = state->dataElements.last();
    } else if (auto scxml = m_currentState->asNode()->asScxml()) {
        data = scxml->dataElements.last();
    } else {
        Q_UNREACHABLE();
    }
    if (!data->src.isEmpty() && !data->expr.isEmpty()) {
        addError(QStringLiteral("data element with both 'src' and 'expr' attributes"));
        return false;
    }
    if (!parserState.chars.trimmed().isEmpty()) {
        if (!data->src.isEmpty()) {
            addError(QStringLiteral("data element with both 'src' attribute and CDATA"));
            return false;
        } else if (!data->expr.isEmpty()) {
            addError(QStringLiteral("data element with both 'expr' attribute and CDATA"));
            return false;
        } else {
            // w3c-ecma/test558 - "if a child element of <data> is not a XML,
            // treat it as a string with whitespace normalization"
            // We've modified the test, so that a string is enclosed with quotes.
            data->expr = parserState.chars;
        }
    } else if (!data->src.isEmpty()) {
        if (!m_loader) {
            addError(QStringLiteral("cannot parse a document with external dependencies without a loader"));
        } else {
            bool ok;
            const QByteArray ba = load(data->src, &ok);
            if (!ok) {
                addError(QStringLiteral("failed to load external dependency"));
            } else {
                // w3c-ecma/test558 - "if XML is loaded via "src" attribute,
                // treat it as a string with whitespace normalization"
                // We've enclosed the text in file with quotes.
                data->expr = QString::fromUtf8(ba);
            }
        }
    }
    return true;
}

bool QScxmlParserPrivate::postReadElementAssign()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementDoneData()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementContent()
{
    const ParserState parserState = current();
    if (!parserState.chars.trimmed().isEmpty()) {

        switch (previous().kind) {
        case ParserState::DoneData: // see test529
            m_currentState->asState()->doneData->contents = parserState.chars.simplified();
            break;
        case ParserState::Send: // see test179
            previous().instruction->asSend()->content = parserState.chars.simplified();
            break;
        default:
            break;
        }
    }
    return true;
}

bool QScxmlParserPrivate::postReadElementParam()
{
    return true;
}

bool QScxmlParserPrivate::postReadElementScript()
{
    const ParserState parserState = current();
    DocumentModel::Script *scriptI = parserState.instruction->asScript();
    if (!parserState.chars.trimmed().isEmpty()) {
        scriptI->content = parserState.chars.trimmed();
        if (!scriptI->src.isEmpty())
            addError(QStringLiteral("both src and source content given to script, will ignore external content"));
    } else if (!scriptI->src.isEmpty()) {
        if (!m_loader) {
            addError(QStringLiteral("cannot parse a document with external dependencies without a loader"));
        } else {
            bool ok;
            const QByteArray data = load(scriptI->src, &ok);
            if (!ok) {
                addError(QStringLiteral("failed to load external dependency"));
            } else {
                scriptI->content = QString::fromUtf8(data);
            }
        }
    }
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementSend()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementCancel()
{
    return flushInstruction();
}

bool QScxmlParserPrivate::postReadElementInvoke()
{
    DocumentModel::Invoke *i = current().instruction->asInvoke();
    const QString fileName = i->src;
    if (!i->content.data()) {
        if (!fileName.isEmpty()) {
            bool ok = true;
            const QByteArray data = load(fileName, &ok);
            if (!ok) {
                addError(QStringLiteral("failed to load external dependency"));
            } else {
                QXmlStreamReader reader(data);
                parseSubDocument(i, &reader, fileName);
            }
        } else { // invoke always expects sub document
            DocumentModel::ScxmlDocument *doc = new DocumentModel::ScxmlDocument(m_fileName);
            doc->root = new DocumentModel::Scxml(DocumentModel::XmlLocation(0, 0));
            i->content.reset(doc);
            m_doc->allSubDocuments.append(i->content.data());
        }
    } else if (!fileName.isEmpty()) {
        addError(QStringLiteral("both src and content given to invoke"));
    }

    return true;
}

bool QScxmlParserPrivate::postReadElementFinalize()
{
    return true;
}

void QScxmlParserPrivate::resetDocument()
{
    m_doc.reset(new DocumentModel::ScxmlDocument(fileName()));
}

bool QScxmlParserPrivate::readDocument()
{
    resetDocument();
    if (m_qtMode == QScxmlParser::QtModeEnabled)
        m_doc->qtMode = true;
    else if (m_qtMode == QScxmlParser::QtModeDisabled)
        m_doc->qtMode = false;
    m_currentState = m_doc->root;
    for (bool finished = false; !finished && !m_reader->hasError();) {
        switch (m_reader->readNext()) {
        case QXmlStreamReader::StartElement : {
            const QStringRef newTag = m_reader->name();
            const ParserState::Kind newElementKind = ParserState::nameToParserStateKind(newTag);

            auto ns = m_reader->namespaceUri();

            if (ns != scxmlNamespace) {
                m_reader->skipCurrentElement();
            } else if (newElementKind == ParserState::None) {
                addError(QStringLiteral("Unknown element %1").arg(newTag.toString()));
                m_reader->skipCurrentElement();
            } else if (newElementKind == ParserState::Scxml) {
                if (readElement() == false)
                    return false;
            } else {
                addError(QStringLiteral("Unexpected element %1").arg(newTag.toString()));
                m_reader->skipCurrentElement();
            }
        }
            break;
        case QXmlStreamReader::EndElement :
            finished = true;
            break;
        case QXmlStreamReader::Comment:
            parseComment();
            break;
        default :
            break;
        }
    }
    if (!m_doc->root) {
        addError(QStringLiteral("Missing root element"));
        return false;
    }

    if (m_reader->hasError() && m_reader->error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        addError(QStringLiteral("Error parsing SCXML file: %1").arg(m_reader->errorString()));
        return false;
    }

    return true;
}

bool QScxmlParserPrivate::readElement()
{
    const QStringRef currentTag = m_reader->name();
    const QXmlStreamAttributes attributes = m_reader->attributes();

    const ParserState::Kind elementKind = ParserState::nameToParserStateKind(currentTag);

    if (!checkAttributes(attributes, elementKind))
        return false;

    if (elementKind == ParserState::Scxml && m_doc->root) {
        if (!hasPrevious()) {
            addError(QStringLiteral("misplaced scxml"));
            return false;
        }

        DocumentModel::Invoke *i = previous().instruction->asInvoke();
        if (!i) {
            addError(QStringLiteral("misplaced scxml"));
            return false;
        }

        return parseSubElement(i, m_reader, m_fileName);
    }

    if (elementKind != ParserState::Scxml && !m_stack.count()) {
        addError(QStringLiteral("misplaced %1").arg(currentTag.toString()));
        return false;
    }

    ParserState pNew = ParserState(elementKind);

    m_stack.append(pNew);

    switch (elementKind) {
    case ParserState::Scxml:      if (!preReadElementScxml())      return false; break;
    case ParserState::State:      if (!preReadElementState())      return false; break;
    case ParserState::Parallel:   if (!preReadElementParallel())   return false; break;
    case ParserState::Initial:    if (!preReadElementInitial())    return false; break;
    case ParserState::Transition: if (!preReadElementTransition()) return false; break;
    case ParserState::Final:      if (!preReadElementFinal())      return false; break;
    case ParserState::History:    if (!preReadElementHistory())    return false; break;
    case ParserState::OnEntry:    if (!preReadElementOnEntry())    return false; break;
    case ParserState::OnExit:     if (!preReadElementOnExit())     return false; break;
    case ParserState::Raise:      if (!preReadElementRaise())      return false; break;
    case ParserState::If:         if (!preReadElementIf())         return false; break;
    case ParserState::ElseIf:     if (!preReadElementElseIf())     return false; break;
    case ParserState::Else:       if (!preReadElementElse())       return false; break;
    case ParserState::Foreach:    if (!preReadElementForeach())    return false; break;
    case ParserState::Log:        if (!preReadElementLog())        return false; break;
    case ParserState::DataModel:  if (!preReadElementDataModel())  return false; break;
    case ParserState::Data:       if (!preReadElementData())       return false; break;
    case ParserState::Assign:     if (!preReadElementAssign())     return false; break;
    case ParserState::DoneData:   if (!preReadElementDoneData())   return false; break;
    case ParserState::Content:    if (!preReadElementContent())    return false; break;
    case ParserState::Param:      if (!preReadElementParam())      return false; break;
    case ParserState::Script:     if (!preReadElementScript())     return false; break;
    case ParserState::Send:       if (!preReadElementSend())       return false; break;
    case ParserState::Cancel:     if (!preReadElementCancel())     return false; break;
    case ParserState::Invoke:     if (!preReadElementInvoke())     return false; break;
    case ParserState::Finalize:   if (!preReadElementFinalize())   return false; break;
    default: addError(QStringLiteral("Unknown element %1").arg(currentTag.toString())); return false;
    }

    for (bool finished = false; !finished && !m_reader->hasError();) {
        switch (m_reader->readNext()) {
        case QXmlStreamReader::StartElement : {
            const QStringRef newTag = m_reader->name();
            const ParserState::Kind newElementKind = ParserState::nameToParserStateKind(newTag);

            auto ns = m_reader->namespaceUri();

            if (ns != scxmlNamespace) {
                m_reader->skipCurrentElement();
            } else if (newElementKind == ParserState::None) {
                addError(QStringLiteral("Unknown element %1").arg(newTag.toString()));
                m_reader->skipCurrentElement();
            } else if (pNew.validChild(newElementKind)) {
                if (readElement() == false)
                    return false;
            } else {
                addError(QStringLiteral("Unexpected element %1").arg(newTag.toString()));
                m_reader->skipCurrentElement();
            }
        }
            break;
        case QXmlStreamReader::EndElement :
            finished = true;
            break;
        case QXmlStreamReader::Characters :
            if (m_stack.isEmpty())
                break;
            if (current().collectChars())
                current().chars.append(m_reader->text());
            break;
        case QXmlStreamReader::Comment:
            parseComment();
            break;
        default :
            break;
        }
    }

    switch (elementKind) {
    case ParserState::Scxml:      if (!postReadElementScxml())      return false; break;
    case ParserState::State:      if (!postReadElementState())      return false; break;
    case ParserState::Parallel:   if (!postReadElementParallel())   return false; break;
    case ParserState::Initial:    if (!postReadElementInitial())    return false; break;
    case ParserState::Transition: if (!postReadElementTransition()) return false; break;
    case ParserState::Final:      if (!postReadElementFinal())      return false; break;
    case ParserState::History:    if (!postReadElementHistory())    return false; break;
    case ParserState::OnEntry:    if (!postReadElementOnEntry())    return false; break;
    case ParserState::OnExit:     if (!postReadElementOnExit())     return false; break;
    case ParserState::Raise:      if (!postReadElementRaise())      return false; break;
    case ParserState::If:         if (!postReadElementIf())         return false; break;
    case ParserState::ElseIf:     if (!postReadElementElseIf())     return false; break;
    case ParserState::Else:       if (!postReadElementElse())       return false; break;
    case ParserState::Foreach:    if (!postReadElementForeach())    return false; break;
    case ParserState::Log:        if (!postReadElementLog())        return false; break;
    case ParserState::DataModel:  if (!postReadElementDataModel())  return false; break;
    case ParserState::Data:       if (!postReadElementData())       return false; break;
    case ParserState::Assign:     if (!postReadElementAssign())     return false; break;
    case ParserState::DoneData:   if (!postReadElementDoneData())   return false; break;
    case ParserState::Content:    if (!postReadElementContent())    return false; break;
    case ParserState::Param:      if (!postReadElementParam())      return false; break;
    case ParserState::Script:     if (!postReadElementScript())     return false; break;
    case ParserState::Send:       if (!postReadElementSend())       return false; break;
    case ParserState::Cancel:     if (!postReadElementCancel())     return false; break;
    case ParserState::Invoke:     if (!postReadElementInvoke())     return false; break;
    case ParserState::Finalize:   if (!postReadElementFinalize())   return false; break;
    default: break;
    }

    m_stack.removeLast();

    if (m_reader->hasError()/* && m_reader->error() != QXmlStreamReader::PrematureEndOfDocumentError*/) {
        addError(QStringLiteral("Error parsing SCXML file: %1").arg(m_reader->errorString()));
        return false;
    }

    return true;
}

void QScxmlParserPrivate::parseComment()
{
    static const QString qtModeSwitch = QStringLiteral("enable-qt-mode:");
    const QStringRef commentText = m_reader->text();
    int qtModeIdx = commentText.indexOf(qtModeSwitch);
    if (qtModeIdx != -1) {
        qtModeIdx += qtModeSwitch.size();
        while (qtModeIdx < commentText.size()) {
            if (commentText.at(qtModeIdx).isSpace()) {
                ++qtModeIdx;
            } else {
                break;
            }
        }
        const QStringRef value = commentText.mid(qtModeIdx);
        if (value.startsWith(QStringLiteral("yes")) && isWordEnd(value, 3)) {
            if (m_qtMode == QScxmlParser::QtModeFromInputFile)
                m_doc->qtMode = true;
        } else if (value.startsWith(QStringLiteral("no")) && isWordEnd(value, 2)) {
            if (m_qtMode == QScxmlParser::QtModeFromInputFile)
                m_doc->qtMode = false;
        } else {
            addError(QStringLiteral("expected 'yes' or 'no' after enable-qt-mode in comment"));
        }
    }
}

void QScxmlParserPrivate::currentStateUp()
{
    Q_ASSERT(m_currentState->parent);
    m_currentState = m_currentState->parent;
}

bool QScxmlParserPrivate::flushInstruction()
{
    if (!hasPrevious()) {
        addError(QStringLiteral("missing instructionContainer"));
        return false;
    }
    DocumentModel::InstructionSequence *instructions = previous().instructionContainer;
    if (!instructions) {
        addError(QStringLiteral("got executable content within an element that did not set instructionContainer"));
        return false;
    }
    instructions->append(current().instruction);
    return true;
}


QByteArray QScxmlParserPrivate::load(const QString &name, bool *ok) const
{
    return m_loader->load(name, m_fileName.isEmpty() ?
                              QString() : QFileInfo(m_fileName).path(), ok);
}

QVector<QScxmlError> QScxmlParserPrivate::errors() const
{
    return m_errors;
}

void QScxmlParserPrivate::addError(const QString &msg)
{
    m_errors.append(QScxmlError(m_fileName, m_reader->lineNumber(), m_reader->columnNumber(), msg));
}

void QScxmlParserPrivate::addError(const DocumentModel::XmlLocation &location, const QString &msg)
{
    m_errors.append(QScxmlError(m_fileName, location.line, location.column, msg));
}

QScxmlParser::QtMode QScxmlParserPrivate::qtMode() const
{
    return m_qtMode;
}

void QScxmlParserPrivate::setQtMode(QScxmlParser::QtMode mode)
{
    m_qtMode = mode;
}

DocumentModel::AbstractState *QScxmlParserPrivate::currentParent() const
{
    return m_currentState ? m_currentState->asAbstractState() : Q_NULLPTR;
}

DocumentModel::XmlLocation QScxmlParserPrivate::xmlLocation() const
{
    return DocumentModel::XmlLocation(m_reader->lineNumber(), m_reader->columnNumber());
}

bool QScxmlParserPrivate::maybeId(const QXmlStreamAttributes &attributes, QString *id)
{
    Q_ASSERT(id);
    QString idStr = attributes.value(QLatin1String("id")).toString();
    if (!idStr.isEmpty()) {
        if (m_allIds.contains(idStr)) {
            addError(xmlLocation(), QStringLiteral("duplicate id '%1'").arg(idStr));
        } else {
            m_allIds.insert(idStr);
            *id = idStr;
        }
    }
    return true;
}

DocumentModel::If *QScxmlParserPrivate::lastIf()
{
    if (!hasPrevious()) {
        addError(QStringLiteral("No previous instruction found for else block"));
        return Q_NULLPTR;
    }

    DocumentModel::Instruction *lastI = previous().instruction;
    if (!lastI) {
        addError(QStringLiteral("No previous instruction found for else block"));
        return Q_NULLPTR;
    }
    DocumentModel::If *ifI = lastI->asIf();
    if (!ifI) {
        addError(QStringLiteral("Previous instruction for else block is not an 'if'"));
        return Q_NULLPTR;
    }
    return ifI;
}

QScxmlParserPrivate::ParserState &QScxmlParserPrivate::current()
{
    return m_stack.last();
}

QScxmlParserPrivate::ParserState &QScxmlParserPrivate::previous()
{
    return m_stack[m_stack.count() - 2];
}

bool QScxmlParserPrivate::hasPrevious() const
{
    return m_stack.count() > 1;
}

bool QScxmlParserPrivate::checkAttributes(const QXmlStreamAttributes &attributes,
                                          QScxmlParserPrivate::ParserState::Kind kind)
{
    return checkAttributes(attributes,
                           ParserState::requiredAttributes(kind),
                           ParserState::optionalAttributes(kind));
}

bool QScxmlParserPrivate::checkAttributes(const QXmlStreamAttributes &attributes,
                                          const QStringList &requiredNames,
                                          const QStringList &optionalNames)
{
    QStringList required = requiredNames;
    foreach (const QXmlStreamAttribute &attribute, attributes) {
        const QStringRef ns = attribute.namespaceUri();
        if (!ns.isEmpty() && ns != scxmlNamespace && ns != qtScxmlNamespace)
            continue;

        const QString name = attribute.name().toString();
        if (!required.removeOne(name) && !optionalNames.contains(name)) {
            addError(QStringLiteral("Unexpected attribute '%1'").arg(name));
            return false;
        }
    }
    if (!required.isEmpty()) {
        addError(QStringLiteral("Missing required attributes: '%1'")
                 .arg(required.join(QLatin1String("', '"))));
        return false;
    }
    return true;
}

QScxmlParserPrivate::DefaultLoader::DefaultLoader(QScxmlParser *parser)
    : Loader(parser)
{}

QByteArray QScxmlParserPrivate::DefaultLoader::load(const QString &name, const QString &baseDir, bool *ok)
{
    Q_ASSERT(ok != nullptr);

    *ok = false;
#ifdef BUILD_QSCXMLC
    QString cleanName = name;
    if (name.startsWith(QStringLiteral("file:")))
        cleanName = name.mid(5);
    QFileInfo fInfo(cleanName);
#else
    const QUrl url(name);
    if (!url.isLocalFile() && !url.isRelative())
        parser()->addError(QStringLiteral("src attribute is not a local file (%1)").arg(name));
    QFileInfo fInfo = url.isLocalFile() ? url.toLocalFile() : name;
#endif // BUILD_QSCXMLC
    if (fInfo.isRelative())
        fInfo = QFileInfo(QDir(baseDir).filePath(fInfo.filePath()));

    if (!fInfo.exists()) {
        parser()->addError(QStringLiteral("src attribute resolves to non existing file (%1)").arg(fInfo.filePath()));
    } else {
        QFile f(fInfo.filePath());
        if (f.open(QFile::ReadOnly)) {
            *ok = true;
            return f.readAll();
        } else {
            parser()->addError(QStringLiteral("Failure opening file %1: %2")
                               .arg(fInfo.filePath(), f.errorString()));
        }
    }
    return QByteArray();
}

QScxmlParserPrivate::ParserState::ParserState(QScxmlParserPrivate::ParserState::Kind someKind)
    : kind(someKind)
    , instruction(0)
    , instructionContainer(0)
{}

QT_END_NAMESPACE
