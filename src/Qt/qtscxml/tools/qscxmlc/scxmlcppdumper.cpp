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

#include <QtScxml/private/qscxmlexecutablecontent_p.h>
#include "scxmlcppdumper.h"

#include <algorithm>
#include <functional>
#include <QFileInfo>
#include <QBuffer>

#include "generator.h"

QT_BEGIN_NAMESPACE

static const QString doNotEditComment = QString::fromLatin1(
            "//\n"
            "// Statemachine code from reading SCXML file '%1'\n"
            "// Created by: The Qt SCXML Compiler version %2 (Qt %3)\n"
            "// WARNING! All changes made in this file will be lost!\n"
            "//\n"
            );

static const QString revisionCheck = QString::fromLatin1(
            "#if !defined(Q_QSCXMLC_OUTPUT_REVISION)\n"
            "#error \"The header file '%1' doesn't include <qscxmltabledata.h>.\"\n"
            "#elif Q_QSCXMLC_OUTPUT_REVISION != %2\n"
            "#error \"This file was generated using the qscxmlc from %3. It\"\n"
            "#error \"cannot be used with the include files from this version of Qt.\"\n"
            "#error \"(The qscxmlc has changed too much.)\"\n"
            "#endif\n"
            );

struct StringListDumper {
    StringListDumper &operator <<(const QString &s) {
        text.append(s);
        return *this;
    }

    StringListDumper &operator <<(const QLatin1String &s) {
        text.append(s);
        return *this;
    }
    StringListDumper &operator <<(const char *s) {
        text.append(QLatin1String(s));
        return *this;
    }
    StringListDumper &operator <<(int i) {
        text.append(QString::number(i));
        return *this;
    }
    StringListDumper &operator <<(const QByteArray &s) {
        text.append(QString::fromUtf8(s));
        return *this;
    }

    bool isEmpty() const {
        return text.isEmpty();
    }

    void write(QTextStream &out, const QString &prefix, const QString &suffix, const QString &mainClassName = QString()) const
    {
        foreach (QString line, text) {
            if (!mainClassName.isEmpty() && line.contains(QStringLiteral("%"))) {
                line = line.arg(mainClassName);
            }
            out << prefix << line << suffix;
        }
    }

    void unique()
    {
        text.sort();
        text.removeDuplicates();
    }

    QStringList text;
};

struct Method {
    StringListDumper initializer;
    Method(const QString &decl = QString()): decl(decl) {}
    Method(const StringListDumper &impl): impl(impl) {}
    QString decl; // void f(int i = 0);
    StringListDumper impl; // void f(int i) { m_i = ++i; }
};

struct ClassDump {
    bool needsEventFilter;
    StringListDumper implIncludes;
    QString className;
    QString dataModelClassName;
    StringListDumper classFields;
    StringListDumper tables;
    Method init;
    Method initDataModel;
    StringListDumper dataMethods;
    StringListDumper classMethods;
    Method constructor;
    Method destructor;
    StringListDumper properties;
    StringListDumper signalMethods;
    QList<Method> publicMethods;
    QList<Method> protectedMethods;
    StringListDumper publicSlotDeclarations;
    StringListDumper publicSlotDefinitions;

    QList<Method> dataModelMethods;

    ClassDump()
        : needsEventFilter(false)
    {}

    QByteArray metaData;
};

namespace {

QString cEscape(const QString &str)
{
    QString res;
    int lastI = 0;
    for (int i = 0; i < str.length(); ++i) {
        QChar c = str.at(i);
        if (c < QLatin1Char(' ') || c == QLatin1Char('\\') || c == QLatin1Char('\"')) {
            res.append(str.mid(lastI, i - lastI));
            lastI = i + 1;
            if (c == QLatin1Char('\\')) {
                res.append(QLatin1String("\\\\"));
            } else if (c == QLatin1Char('\"')) {
                res.append(QLatin1String("\\\""));
            } else if (c == QLatin1Char('\n')) {
                res.append(QLatin1String("\\n"));
            } else if (c == QLatin1Char('\r')) {
                res.append(QLatin1String("\\r"));
            } else {
                char buf[6];
                ushort cc = c.unicode();
                buf[0] = '\\';
                buf[1] = 'u';
                for (int i = 0; i < 4; ++i) {
                    ushort ccc = cc & 0xF;
                    buf[5 - i] = ccc <= 9 ? '0' + ccc : 'a' + ccc - 10;
                    cc >>= 4;
                }
                res.append(QLatin1String(&buf[0], 6));
            }
        }
    }
    if (lastI != 0) {
        res.append(str.mid(lastI));
        return res;
    }
    return str;
}

static const char *headerStart =
        "#include <QScxmlStateMachine>\n"
        "#include <QString>\n"
        "#include <QByteArray>\n"
        "\n";

using namespace DocumentModel;

enum class Evaluator
{
    ToVariant,
    ToString,
    ToBool,
    Assignment,
    Foreach,
    Script
};

class DumperVisitor: public QScxmlExecutableContent::Builder
{
    Q_DISABLE_COPY(DumperVisitor)

public:
    DumperVisitor(ClassDump &clazz, TranslationUnit *tu)
        : namespacePrefix(QStringLiteral("::"))
        , clazz(clazz)
        , translationUnit(tu)
        , m_bindLate(false)
        , m_qtMode(false)
    {
        if (!tu->namespaceName.isEmpty()) {
            namespacePrefix += QStringLiteral("%1::").arg(tu->namespaceName);
        }
    }

    void process(ScxmlDocument *doc)
    {
        Q_ASSERT(doc);

        clazz.className = mangleIdentifier(translationUnit->classnameForDocument.value(doc));
        m_qtMode = doc->qtMode;

        doc->root->accept(this);

        addSubStateMachineProperties(doc);
        addEvents();

        generateMetaObject();
        generateTables();
    }

    ~DumperVisitor()
    {
        Q_ASSERT(m_parents.isEmpty());
    }

protected:
    using NodeVisitor::visit;

    bool visit(Scxml *node) Q_DECL_OVERRIDE
    {
        // init:
        if (!node->name.isEmpty()) {
            clazz.dataMethods << QStringLiteral("QString name() const Q_DECL_OVERRIDE Q_DECL_FINAL")
                              << QStringLiteral("{ return string(%1); }").arg(addString(node->name))
                              << QString();
            clazz.init.impl << QStringLiteral("stateMachine.setObjectName(string(%1));").arg(addString(node->name));
        } else {
            clazz.dataMethods << QStringLiteral("QString name() const Q_DECL_OVERRIDE Q_DECL_FINAL")
                              << QStringLiteral("{ return QString(); }")
                              << QString();
        }
        if (node->dataModel == Scxml::CppDataModel) {
            // Tell the builder not to generate any script strings when visiting any executable content.
            // We'll take care of the evaluators ourselves.
            setIsCppDataModel(true);
        }

        QString binding;
        switch (node->binding) {
        case Scxml::EarlyBinding:
            binding = QStringLiteral("Early");
            break;
        case Scxml::LateBinding:
            binding = QStringLiteral("Late");
            m_bindLate = true;
            break;
        default:
            Q_UNREACHABLE();
        }
        clazz.init.impl << QStringLiteral("stateMachine.setDataBinding(QScxmlStateMachine::%1Binding);").arg(binding);
        clazz.implIncludes << QStringLiteral("qscxmlexecutablecontent.h");
        clazz.init.impl << QStringLiteral("stateMachine.setTableData(this);");

        foreach (AbstractState *s, node->initialStates) {
            clazz.init.impl << QStringLiteral("%1.setAsInitialStateFor(&stateMachine);")
                               .arg(mangledName(s, StateName));
        }

        // visit the kids:
        m_parents.append(node);
        visit(node->children);
        visit(node->dataElements);

        m_dataElements.append(node->dataElements);
        if (node->script || !m_dataElements.isEmpty() || !node->initialSetup.isEmpty()) {
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::ContainerId initialSetup() const Q_DECL_OVERRIDE Q_DECL_FINAL")
                              << QStringLiteral("{ return %1; }").arg(startNewSequence())
                              << QString();
            generate(m_dataElements);
            if (node->script) {
                node->script->accept(this);
            }
            visit(&node->initialSetup);
            endSequence();
        } else {
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::ContainerId initialSetup() const Q_DECL_OVERRIDE Q_DECL_FINAL")
                              << QStringLiteral("{ return QScxmlExecutableContent::NoInstruction; }")
                              << QString();
        }

        m_parents.removeLast();

        { // the data model:
            switch (node->dataModel) {
            case Scxml::NullDataModel:
                clazz.classFields << QStringLiteral("QScxmlNullDataModel dataModel;");
                clazz.implIncludes << QStringLiteral("QScxmlNullDataModel");
                clazz.init.impl << QStringLiteral("stateMachine.setDataModel(&dataModel);");
                break;
            case Scxml::JSDataModel:
                clazz.classFields << QStringLiteral("QScxmlEcmaScriptDataModel dataModel;");
                clazz.implIncludes << QStringLiteral("QScxmlEcmaScriptDataModel");
                clazz.init.impl << QStringLiteral("stateMachine.setDataModel(&dataModel);");
                break;
            case Scxml::CppDataModel:
                clazz.dataModelClassName = node->cppDataModelClassName;
                clazz.implIncludes << node->cppDataModelHeaderName;
                break;
            default:
                Q_UNREACHABLE();
            }
        }
        return false;
    }

    bool visit(State *node) Q_DECL_OVERRIDE
    {
        QString name = mangledName(node, PlainName);
        QString stateName = mangledName(node, StateName);
        // Property stuff:
        if (isValidQPropertyName(node->id)) {
            clazz.properties << QStringLiteral("Q_PROPERTY(bool %1 READ %2 NOTIFY %3)")
                                .arg(node->id).arg(name)
                                .arg(mangledName(node, SignalName));
        }
        if (m_qtMode) {
            Method getter(QStringLiteral("bool %1() const").arg(name));
            getter.impl << QStringLiteral("bool %2::%1() const").arg(name)
                        << QStringLiteral("{ return data->%1.active(); }").arg(stateName);
            clazz.publicMethods << getter;
        }

        // Declaration:
        if (node->type == State::Final) {
            clazz.classFields << QStringLiteral("QScxmlFinalState ") + stateName + QLatin1Char(';');
        } else {
            clazz.classFields << QStringLiteral("QScxmlState ") + stateName + QLatin1Char(';');
        }

        // Initializer:
        clazz.constructor.initializer << generateInitializer(node);

        // init:
        if (!node->id.isEmpty()) {
            clazz.init.impl << stateName + QStringLiteral(".setObjectName(string(%1));").arg(addString(node->id));
        }
        if (node->type == State::Parallel) {
            clazz.init.impl << stateName + QStringLiteral(".setChildMode(QState::ParallelStates);");
        } else {
            foreach (AbstractState *initialState, node->initialStates) {
                clazz.init.impl << stateName + QStringLiteral(".setInitialState(&")
                                   + mangledName(initialState, StateName)
                                   + QStringLiteral(");");
            }

        }
        if (!node->id.isEmpty()) {
            clazz.init.impl << QStringLiteral("QObject::connect(&")
                               + stateName
                               + QStringLiteral(", SIGNAL(activeChanged(bool)), &stateMachine, SIGNAL(")
                               + mangledName(node, SignalName)
                               + QStringLiteral("(bool)));");
        }

        m_stateNames.append(node->id);
        m_stateFieldNames.append(stateName);

        // visit the kids:
        m_parents.append(node);
        if (!node->dataElements.isEmpty()) {
            if (m_bindLate) {
                clazz.init.impl << stateName + QStringLiteral(".setInitInstructions(%1);").arg(startNewSequence());
                generate(node->dataElements);
                endSequence();
            } else {
                m_dataElements.append(node->dataElements);
            }
        }

        visit(node->children);
        if (!node->onEntry.isEmpty())
            clazz.init.impl << stateName + QStringLiteral(".setOnEntryInstructions(%1);").arg(generate(node->onEntry));
        if (!node->onExit.isEmpty())
            clazz.init.impl << stateName + QStringLiteral(".setOnExitInstructions(%1);").arg(generate(node->onExit));
        if (!node->invokes.isEmpty()) {
            QStringList lines;
            for (int i = 0, ei = node->invokes.size(); i != ei; ++i) {
                Invoke *invoke = node->invokes.at(i);
                QString line = QStringLiteral("new QScxmlInvokeScxmlFactory<%1>(").arg(scxmlClassName(invoke->content.data()));
                line += QStringLiteral("%1, ").arg(Builder::createContext(QStringLiteral("invoke")));
                line += QStringLiteral("%1, ").arg(addString(invoke->id));
                line += QStringLiteral("%1, ").arg(addString(node->id + QStringLiteral(".session-")));
                line += QStringLiteral("%1, ").arg(addString(invoke->idLocation));
                {
                    QStringList l;
                    foreach (const QString &name, invoke->namelist) {
                        l.append(QString::number(addString(name)));
                    }
                    line += QStringLiteral("%1, ").arg(createVector(QStringLiteral("QScxmlExecutableContent::StringId"), l));
                }
                line += QStringLiteral("%1, ").arg(invoke->autoforward ? QStringLiteral("true") : QStringLiteral("false"));
                {
                    QStringList l;
                    foreach (DocumentModel::Param *param, invoke->params) {
                        l += QStringLiteral("QScxmlInvokableServiceFactory::Param(%1, %2, %3)")
                                .arg(addString(param->name))
                                .arg(createEvaluatorVariant(QStringLiteral("param"), QStringLiteral("expr"), param->expr))
                                .arg(addString(param->location));
                    }
                    line += QStringLiteral("%1, ").arg(createVector(QStringLiteral("QScxmlInvokableServiceFactory::Param"), l));
                }
                if (invoke->finalize.isEmpty()) {
                    line += QStringLiteral("QScxmlExecutableContent::NoInstruction");
                } else {
                    line += QString::number(startNewSequence());
                    visit(&invoke->finalize);
                    endSequence();
                }
                line += QLatin1Char(')');
                lines << line;
            }
            clazz.init.impl << stateName + QStringLiteral(".setInvokableServiceFactories(");
            clazz.init.impl << QStringLiteral("    ") + createVector(QStringLiteral("QScxmlInvokableServiceFactory *"), lines);
            clazz.init.impl << QStringLiteral(");");
        }

        if (node->type == State::Final) {
            auto id = generate(node->doneData);
            clazz.init.impl << stateName + QStringLiteral(".setDoneData(%1);").arg(id);
        }

        m_parents.removeLast();
        return false;
    }

    bool visit(Transition *node) Q_DECL_OVERRIDE
    {
        const QString tName = transitionName(node);
        if (m_qtMode) {
            foreach (const QString &event, node->events) {
                if (!DocumentModel::isEventToBeGenerated(event))
                    continue;

                // If the event name is not filtered out, is was already validated inside:
                // bool ScxmlVerifier::visit(DocumentModel::Transition *transition)
                // by a call to: validateEventName();
                m_knownEvents.insert(event);
            }
        }

        // Declaration:
        clazz.classFields << QStringLiteral("QScxmlTransition ") + tName + QLatin1Char(';');

        // Initializer:
        QString initializer = tName + QStringLiteral("(");
        QStringList elements;
        foreach (const QString &event, node->events)
            elements.append(qba(event));
        initializer += createList(QStringLiteral("QString"), elements);
        initializer += QStringLiteral(")");
        clazz.constructor.initializer << initializer;

        // init:
        if (node->condition) {
            QString condExpr = *node->condition.data();
            auto cond = createEvaluatorBool(QStringLiteral("transition"), QStringLiteral("cond"), condExpr);
            clazz.init.impl << tName + QStringLiteral(".setConditionalExpression(%1);").arg(cond);
        }

        if (m_parents.last()->asHistoryState()) {
            clazz.init.impl << QStringLiteral("%1.setDefaultTransition(&%2);").arg(parentStateMemberName(), tName);
        } else {
            clazz.init.impl << QStringLiteral("%1.addTransitionTo(&%2);").arg(tName, parentStateMemberName());
        }

        if (node->type == Transition::Internal) {
            clazz.init.impl << tName + QStringLiteral(".setTransitionType(QAbstractTransition::InternalTransition);");
        }
        QStringList targetNames;
        foreach (DocumentModel::AbstractState *s, node->targetStates)
            targetNames.append(QStringLiteral("&") + mangledName(s, StateName));
        QString targets = tName + QStringLiteral(".setTargetStates(") + createList(QStringLiteral("QAbstractState*"), targetNames);
        clazz.init.impl << targets + QStringLiteral(");");

        // visit the kids:
        if (!node->instructionsOnTransition.isEmpty()) {
            m_parents.append(node);
            m_currentTransitionName = tName;
            clazz.init.impl << tName + QStringLiteral(".setInstructionsOnTransition(%1);").arg(startNewSequence());
            visit(&node->instructionsOnTransition);
            endSequence();
            m_parents.removeLast();
            m_currentTransitionName.clear();
        }
        return false;
    }

    bool visit(DocumentModel::HistoryState *node) Q_DECL_OVERRIDE
    {
        // Includes:
        clazz.implIncludes << "QScxmlHistoryState";

        const QString stateName = mangledName(node, StateName);
        // Declaration:
        clazz.classFields << QStringLiteral("QScxmlHistoryState ") + stateName + QLatin1Char(';');

        // Initializer:
        clazz.constructor.initializer << generateInitializer(node);

        // init:
        if (!node->id.isEmpty()) {
            clazz.init.impl << stateName + QStringLiteral(".setObjectName(string(%1));").arg(addString(node->id));
        }
        QString depth;
        switch (node->type) {
        case DocumentModel::HistoryState::Shallow:
            depth = QStringLiteral("Shallow");
            break;
        case DocumentModel::HistoryState::Deep:
            depth = QStringLiteral("Deep");
            break;
        default:
            Q_UNREACHABLE();
        }
        clazz.init.impl << stateName + QStringLiteral(".setHistoryType(QScxmlHistoryState::") + depth + QStringLiteral("History);");

        // visit the kid:
        if (Transition *t = node->defaultConfiguration()) {

            m_parents.append(node);
            t->accept(this);
            m_parents.removeLast();
        }
        return false;
    }

    bool visit(Send *node) Q_DECL_OVERRIDE
    {
        if (m_qtMode && node->type == QStringLiteral("qt:signal")) {
            if (!m_signals.contains(node->event)) {
                m_signals.insert(node->event);
                m_signalNames.append(node->event);
                clazz.signalMethods << QStringLiteral("void %1(const QVariant &data);").arg(node->event);
            }
        }

        return QScxmlExecutableContent::Builder::visit(node);
    }

private:
    enum NameForm {
        PlainName,
        SignalName,
        MachineName,
        StateName
    };

    QString mangledName(const QString &id, NameForm form) const
    {
        QString name = id;
        switch (form) {
        case PlainName: break;
        case SignalName: name.append(QStringLiteral("Changed")); break;
        case StateName: name.prepend(QStringLiteral("state_")); break;
        case MachineName: name.prepend(QStringLiteral("machine_")); break;
        }

        return name.isEmpty() ? name : mangleIdentifier(name);
    }

    QString mangledName(AbstractState *state, NameForm form) const
    {
        Q_ASSERT(state);

        QString name = m_mangledNames.value(state)[form];
        if (!name.isEmpty())
            return name;

        QString id = state->id;
        if (State *s = state->asState()) {
            if (s->type == State::Initial) {
                id = s->parent->asState()->id + QStringLiteral("_initial");
            }
        }

        name = mangledName(id, form);
        m_mangledNames[state][form] = name;
        return name;
    }

    QString transitionName(Transition *t) const
    {
        int idx = 0;
        QString parentName;
        auto parent = m_parents.last();
        if (State *parentState = parent->asState()) {
            parentName = mangledName(parentState, PlainName);
            idx = childIndex(t, parentState->children);
        } else if (HistoryState *historyState = parent->asHistoryState()) {
            parentName = mangledName(historyState, PlainName);
        } else if (Scxml *scxml = parent->asScxml()) {
            parentName = QStringLiteral("stateMachine");
            idx = childIndex(t, scxml->children);
        } else {
            Q_UNREACHABLE();
        }
        return QStringLiteral("transition_%1_%2").arg(parentName, QString::number(idx));
    }

    static int childIndex(StateOrTransition *child, const QVector<StateOrTransition *> &children) {
        int idx = 0;
        foreach (StateOrTransition *sot, children) {
            if (sot == child)
                break;
            else
                ++idx;
        }
        return idx;
    }

    QString createList(const QString &elementType, const QStringList &elements) const
    { return createContainer(QStringLiteral("QList"), elementType, elements); }

    QString createVector(const QString &elementType, const QStringList &elements) const
    { return createContainer(QStringLiteral("QVector"), elementType, elements); }

    QString createContainer(const QString &baseType, const QString &elementType, const QStringList &elements) const
    {
        QString result;
        if (translationUnit->useCxx11) {
            if (elements.isEmpty()) {
                result += QStringLiteral("{}");
            } else {
                result += QStringLiteral("{ ") + elements.join(QStringLiteral(", ")) + QStringLiteral(" }");
            }
        } else {
            result += QStringLiteral("%1<%2>()").arg(baseType, elementType);
            if (!elements.isEmpty()) {
                result += QStringLiteral(" << ") + elements.join(QStringLiteral(" << "));
            }
        }
        return result;
    }

    QString generateInitializer(AbstractState *node) const
    {
        QString init = mangledName(node, StateName) + QStringLiteral("(");
        if (State *parentState = node->parent->asState()) {
            init += QStringLiteral("&") + mangledName(parentState, StateName);
        } else {
            init += QStringLiteral("&stateMachine");
        }
        init += QLatin1Char(')');
        return init;
    }

    void addSubStateMachineProperties(ScxmlDocument *doc)
    {
        foreach (ScxmlDocument *subDocs, doc->allSubDocuments) {
            QString name = subDocs->root->name;
            if (name.isEmpty())
                continue;
            auto plainName = mangledName(name, PlainName);
            auto qualifiedName = namespacePrefix + plainName;
            if (m_serviceProps.contains(qMakePair(plainName, qualifiedName)))
                continue;
            m_serviceProps.append(qMakePair(name, qualifiedName));
            clazz.classFields << QStringLiteral("%1 *%2;").arg(qualifiedName, plainName);
            clazz.constructor.initializer << QStringLiteral("%1(Q_NULLPTR)").arg(plainName);
            if (isValidQPropertyName(name)) {
                clazz.properties << QStringLiteral("Q_PROPERTY(%1%2 *%3 READ %2 NOTIFY %4)")
                                    .arg(namespacePrefix, plainName, name,
                                         mangledName(name, SignalName));
            }
            if (m_qtMode) {
                Method getter(QStringLiteral("%1 *%2() const").arg(qualifiedName, plainName));
                getter.impl << QStringLiteral("%1 *%2::%3() const").arg(qualifiedName,
                                                                        clazz.className, plainName)
                            << QStringLiteral("{ return data->%1; }").arg(plainName);
                clazz.publicMethods << getter;
                clazz.signalMethods << QStringLiteral("void %1(%2 *statemachine);")
                                       .arg(mangledName(name, SignalName), qualifiedName);
            }

            clazz.dataMethods << QStringLiteral("%1 *%2() const")
                                 .arg(qualifiedName, mangledName(name, MachineName))
                              << QStringLiteral("{ return %1; }").arg(plainName)
                              << QString();
        }
    }

    void addEvents()
    {
        QStringList knownEventsList = m_knownEvents.toList();
        std::sort(knownEventsList.begin(), knownEventsList.end());
        if (m_qtMode) {
            foreach (const QString &event, knownEventsList) {
                clazz.publicSlotDeclarations << QStringLiteral("void ") + event + QStringLiteral("(const QVariant &eventData = QVariant());");
                clazz.publicSlotDefinitions << QStringLiteral("void ") + clazz.className
                                               + QStringLiteral("::")
                                               + event
                                               + QStringLiteral("(const QVariant &eventData)\n{ submitEvent(data->") + qba(event)
                                               + QStringLiteral(", eventData); }");
            }
        }

        if (!m_signalNames.isEmpty()) {
            clazz.needsEventFilter = true;
            clazz.init.impl << QStringLiteral("stateMachine.setScxmlEventFilter(this);");
            auto &dm = clazz.dataMethods;
            dm << QStringLiteral("bool handle(QScxmlEvent *event, QScxmlStateMachine *stateMachine) Q_DECL_OVERRIDE  {");
            if (m_qtMode) {
                dm << QStringLiteral("    if (event->originType() != QStringLiteral(\"qt:signal\")) { return true; }")
                   << QStringLiteral("    %1 *m = static_cast<%1 *>(stateMachine);").arg(clazz.className);
                foreach (const QString &signalName, m_signalNames) {
                    dm << QStringLiteral("    if (event->name() == %1) { emit m->%2(event->data()); return false; }")
                          .arg(qba(signalName), mangleIdentifier(signalName));
                }
            }
            dm << QStringLiteral("    return true;")
               << QStringLiteral("}")
               << QString();
        }
    }

    QString createContextString(const QString &instrName) const Q_DECL_OVERRIDE
    {
        if (!m_currentTransitionName.isEmpty()) {
            QString state = parentStateName();
            return QStringLiteral("<%1> instruction in transition of state '%2'").arg(instrName, state);
        } else {
            return QStringLiteral("<%1> instruction in state '%2'").arg(instrName, parentStateName());
        }
    }

    QString createContext(const QString &instrName, const QString &attrName, const QString &attrValue) const Q_DECL_OVERRIDE
    {
        QString location = createContextString(instrName);
        return QStringLiteral("%1 with %2=\"%3\"").arg(location, attrName, attrValue);
    }

    QString parentStateName() const
    {
        for (int i = m_parents.size() - 1; i >= 0; --i) {
            Node *node = m_parents.at(i);
            if (State *s = node->asState())
                return s->id;
            else if (HistoryState *h = node->asHistoryState())
                return h->id;
            else if (Scxml *l = node->asScxml())
                return l->name;
        }

        return QString();
    }

    QString parentStateMemberName() const
    {
        Node *parent = m_parents.last();
        if (State *s = parent->asState())
            return mangledName(s, StateName);
        else if (HistoryState *h = parent->asHistoryState())
            return mangledName(h, StateName);
        else if (parent->asScxml())
            return QStringLiteral("stateMachine");
        else
            Q_UNIMPLEMENTED();
        return QString();
    }

    static void generateList(StringListDumper &t, std::function<QString(int)> next)
    {
        const int maxLineLength = 80;
        QString line;
        for (int i = 0; ; ++i) {
            QString nr = next(i);
            if (nr.isNull())
                break;

            if (i != 0)
                line += QLatin1Char(',');

            if (line.length() + nr.length() + 1 > maxLineLength) {
                t << line;
                line.clear();
            } else if (i != 0) {
                line += QLatin1Char(' ');
            }
            line += nr;
        }
        if (!line.isEmpty())
            t << line;
    }

    void generateTables()
    {
        StringListDumper &t = clazz.tables;
        clazz.classFields << QString();
        QScopedPointer<QScxmlExecutableContent::DynamicTableData> td(tableData());

        { // instructions
            clazz.classFields << QStringLiteral("static qint32 theInstructions[];");
            t << QStringLiteral("qint32 %1::Data::theInstructions[] = {").arg(clazz.className);
            auto instr = td->instructionTable();
            generateList(t, [&instr](int idx) -> QString {
                if (instr.isEmpty() && idx == 0) // prevent generation of empty array
                    return QStringLiteral("-1");
                if (idx < instr.size())
                    return QString::number(instr.at(idx));
                else
                    return QString();
            });
            t << QStringLiteral("};") << QStringLiteral("");
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::Instructions instructions() const Q_DECL_OVERRIDE Q_DECL_FINAL")
                              << QStringLiteral("{ return theInstructions; }")
                              << QString();
        }

        { // dataIds
            int count;
            auto dataIds = td->dataNames(&count);
            clazz.classFields << QStringLiteral("static QScxmlExecutableContent::StringId dataIds[];");
            t << QStringLiteral("QScxmlExecutableContent::StringId %1::Data::dataIds[] = {").arg(clazz.className);
            if (isCppDataModel()) {
                t << QStringLiteral("-1");
            } else  {
                generateList(t, [&dataIds, count](int idx) -> QString {
                    if (count == 0 && idx == 0) // prevent generation of empty array
                        return QStringLiteral("-1");
                    if (idx < count)
                        return QString::number(dataIds[idx]);
                    else
                        return QString();
                });
            }
            t << QStringLiteral("};") << QStringLiteral("");
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::StringId *dataNames(int *count) const Q_DECL_OVERRIDE Q_DECL_FINAL");
            clazz.dataMethods << QStringLiteral("{ *count = %1; return dataIds; }").arg(count);
            clazz.dataMethods << QString();
        }

        { // evaluators
            auto evaluators = td->evaluators();
            clazz.classFields << QStringLiteral("static QScxmlExecutableContent::EvaluatorInfo evaluators[];");
            t << QStringLiteral("QScxmlExecutableContent::EvaluatorInfo %1::Data::evaluators[] = {").arg(clazz.className);
            if (isCppDataModel()) {
                t << QStringLiteral("{ -1, -1 }");
            } else {
                generateList(t, [&evaluators](int idx) -> QString {
                    if (evaluators.isEmpty() && idx == 0) // prevent generation of empty array
                        return QStringLiteral("{ -1, -1 }");
                    if (idx >= evaluators.size())
                        return QString();

                    auto eval = evaluators.at(idx);
                    return QStringLiteral("{ %1, %2 }").arg(eval.expr).arg(eval.context);
                });
            }
            t << QStringLiteral("};") << QStringLiteral("");
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::EvaluatorInfo evaluatorInfo(QScxmlExecutableContent::EvaluatorId evaluatorId) const Q_DECL_OVERRIDE Q_DECL_FINAL");
            clazz.dataMethods << QStringLiteral("{ Q_ASSERT(evaluatorId >= 0); Q_ASSERT(evaluatorId < %1); return evaluators[evaluatorId]; }").arg(evaluators.size());
            clazz.dataMethods << QString();

            if (isCppDataModel()) {
                {
                    StringListDumper stringEvals;
                    stringEvals << QStringLiteral("QString %1::evaluateToString(QScxmlExecutableContent::EvaluatorId id, bool *ok) {").arg(clazz.dataModelClassName)
                              << QStringLiteral("    *ok = true;")
                              << QStringLiteral("    switch (id) {");
                    auto evals = stringEvaluators();
                    for (auto it = evals.constBegin(), eit = evals.constEnd(); it != eit; ++it) {
                        stringEvals << QStringLiteral("    case %1:").arg(it.key())
                                  << QStringLiteral("        return [this]()->QString{ return %1; }();").arg(it.value());
                    }
                    stringEvals << QStringLiteral("    default:")
                              << QStringLiteral("        Q_UNREACHABLE();")
                              << QStringLiteral("        *ok = false;")
                              << QStringLiteral("        return QString();")
                              << QStringLiteral("    }");
                    stringEvals << QStringLiteral("}");
                    clazz.dataModelMethods.append(Method(stringEvals));
                }

                {
                    StringListDumper boolEvals;
                    boolEvals << QStringLiteral("bool %1::evaluateToBool(QScxmlExecutableContent::EvaluatorId id, bool *ok) {").arg(clazz.dataModelClassName)
                              << QStringLiteral("    *ok = true;")
                              << QStringLiteral("    switch (id) {");
                    auto evals = boolEvaluators();
                    for (auto it = evals.constBegin(), eit = evals.constEnd(); it != eit; ++it) {
                        boolEvals << QStringLiteral("    case %1:").arg(it.key())
                                  << QStringLiteral("        return [this]()->bool{ return %1; }();").arg(it.value());
                    }
                    boolEvals << QStringLiteral("    default:")
                              << QStringLiteral("        Q_UNREACHABLE();")
                              << QStringLiteral("        *ok = false;")
                              << QStringLiteral("        return false;")
                              << QStringLiteral("    }");
                    boolEvals << QStringLiteral("}");
                    clazz.dataModelMethods.append(Method(boolEvals));
                }

                {
                    StringListDumper variantEvals;
                    variantEvals << QStringLiteral("QVariant %1::evaluateToVariant(QScxmlExecutableContent::EvaluatorId id, bool *ok) {").arg(clazz.dataModelClassName)
                                 << QStringLiteral("    *ok = true;")
                                 << QStringLiteral("    switch (id) {");
                    auto evals = variantEvaluators();
                    for (auto it = evals.constBegin(), eit = evals.constEnd(); it != eit; ++it) {
                        variantEvals << QStringLiteral("    case %1:").arg(it.key())
                                     << QStringLiteral("        return [this]()->QVariant{ return %1; }();").arg(it.value());
                    }
                    variantEvals << QStringLiteral("    default:")
                                 << QStringLiteral("        Q_UNREACHABLE();")
                                 << QStringLiteral("        *ok = false;")
                                 << QStringLiteral("        return QVariant();")
                                 << QStringLiteral("    }");
                    variantEvals << QStringLiteral("}");
                    clazz.dataModelMethods.append(Method(variantEvals));
                }

                {
                    StringListDumper voidEvals;
                    voidEvals << QStringLiteral("void %1::evaluateToVoid(QScxmlExecutableContent::EvaluatorId id, bool *ok) {").arg(clazz.dataModelClassName)
                                 << QStringLiteral("    *ok = true;")
                                 << QStringLiteral("    switch (id) {");
                    auto evals = voidEvaluators();
                    for (auto it = evals.constBegin(), eit = evals.constEnd(); it != eit; ++it) {
                        voidEvals << QStringLiteral("    case %1:").arg(it.key())
                                     << QStringLiteral("        [this]()->void{ %1 }();").arg(it.value())
                                     << QStringLiteral("        break;");
                    }
                    voidEvals << QStringLiteral("    default:")
                                 << QStringLiteral("        Q_UNREACHABLE();")
                                 << QStringLiteral("        *ok = false;")
                                 << QStringLiteral("    }");
                    voidEvals << QStringLiteral("}");
                    clazz.dataModelMethods.append(Method(voidEvals));
                }
            }
        }

        { // assignments
            auto assignments = td->assignments();
            clazz.classFields << QStringLiteral("static QScxmlExecutableContent::AssignmentInfo assignments[];");
            t << QStringLiteral("QScxmlExecutableContent::AssignmentInfo %1::Data::assignments[] = {").arg(clazz.className);
            if (isCppDataModel()) {
                t << QStringLiteral("{ -1, -1, -1 }");
            } else {
                generateList(t, [&assignments](int idx) -> QString {
                    if (assignments.isEmpty() && idx == 0) // prevent generation of empty array
                        return QStringLiteral("{ -1, -1, -1 }");
                    if (idx >= assignments.size())
                        return QString();

                    auto ass = assignments.at(idx);
                    return QStringLiteral("{ %1, %2, %3 }").arg(ass.dest).arg(ass.expr).arg(ass.context);
                });
            }
            t << QStringLiteral("};") << QStringLiteral("");
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::AssignmentInfo assignmentInfo(QScxmlExecutableContent::EvaluatorId assignmentId) const Q_DECL_OVERRIDE Q_DECL_FINAL");
            clazz.dataMethods << QStringLiteral("{ Q_ASSERT(assignmentId >= 0); Q_ASSERT(assignmentId < %1); return assignments[assignmentId]; }").arg(assignments.size());
            clazz.dataMethods << QString();
        }

        { // foreaches
            auto foreaches = td->foreaches();
            clazz.classFields << QStringLiteral("static QScxmlExecutableContent::ForeachInfo foreaches[];");
            t << QStringLiteral("QScxmlExecutableContent::ForeachInfo %1::Data::foreaches[] = {").arg(clazz.className);
            generateList(t, [&foreaches](int idx) -> QString {
                if (foreaches.isEmpty() && idx == 0) // prevent generation of empty array
                    return QStringLiteral("{ -1, -1, -1, -1 }");
                if (idx >= foreaches.size())
                    return QString();

                auto foreach = foreaches.at(idx);
                return QStringLiteral("{ %1, %2, %3, %4 }").arg(foreach.array).arg(foreach.item).arg(foreach.index).arg(foreach.context);
            });
            t << QStringLiteral("};") << QStringLiteral("");
            clazz.dataMethods << QStringLiteral("QScxmlExecutableContent::ForeachInfo foreachInfo(QScxmlExecutableContent::EvaluatorId foreachId) const Q_DECL_OVERRIDE Q_DECL_FINAL");
            clazz.dataMethods << QStringLiteral("{ Q_ASSERT(foreachId >= 0); Q_ASSERT(foreachId < %1); return foreaches[foreachId]; }").arg(foreaches.size());
        }

        { // strings
            t << QStringLiteral("#define STR_LIT(idx, ofs, len) \\")
              << QStringLiteral("    Q_STATIC_STRING_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \\")
              << QStringLiteral("    qptrdiff(offsetof(Strings, stringdata) + ofs * sizeof(qunicodechar) - idx * sizeof(QArrayData)) \\")
              << QStringLiteral("    )");

            t << QStringLiteral("%1::Data::Strings %1::Data::strings = {{").arg(clazz.className);
            auto strings = td->stringTable();
            if (strings.isEmpty()) // prevent generation of empty array
                strings.append(QStringLiteral(""));
            int ucharCount = 0;
            generateList(t, [&ucharCount, &strings](int idx) -> QString {
                if (idx >= strings.size())
                    return QString();

                int length = strings.at(idx).size();
                QString str = QStringLiteral("STR_LIT(%1, %2, %3)").arg(QString::number(idx),
                                                                        QString::number(ucharCount),
                                                                        QString::number(length));
                ucharCount += length + 1;
                return str;
            });
            t << QStringLiteral("},{");
            for (int i = 0, ei = strings.size(); i < ei; ++i) {
                const QString &string  = strings.at(i);
                QString result;
                for (int charPos = 0, eCharPos = string.size(); charPos < eCharPos; ++charPos) {
                    result.append(QStringLiteral("0x%1,")
                                  .arg(QString::number(string.at(charPos).unicode(), 16)));
                }
                result.append(QStringLiteral("0%1 // %2: %3")
                              .arg(QLatin1String(i < ei - 1 ? "," : ""), QString::number(i),
                                   cEscape(string)));
                t << result;
            }
            t << QStringLiteral("}};") << QStringLiteral("");

            clazz.classFields << QStringLiteral("static struct Strings {")
                              << QStringLiteral("    QArrayData data[%1];").arg(strings.size())
                              << QStringLiteral("    qunicodechar stringdata[%1];").arg(ucharCount + 1)
                              << QStringLiteral("} strings;");

            clazz.dataMethods << QStringLiteral("QString string(QScxmlExecutableContent::StringId id) const Q_DECL_OVERRIDE Q_DECL_FINAL");
            clazz.dataMethods << QStringLiteral("{");
            clazz.dataMethods << QStringLiteral("    Q_ASSERT(id >= QScxmlExecutableContent::NoString); Q_ASSERT(id < %1);").arg(strings.size());
            clazz.dataMethods << QStringLiteral("    if (id == QScxmlExecutableContent::NoString) return QString();");
            if (translationUnit->useCxx11) {
                clazz.dataMethods << QStringLiteral("    return QString({static_cast<QStringData*>(strings.data + id)});");
            } else {
                clazz.dataMethods << QStringLiteral("    QStringDataPtr data;");
                clazz.dataMethods << QStringLiteral("    data.ptr = static_cast<QStringData*>(strings.data + id);");
                clazz.dataMethods << QStringLiteral("    return QString(data);");
            }
            clazz.dataMethods << QStringLiteral("}");
            clazz.dataMethods << QString();
        }
    }

    void generateMetaObject()
    {
        ClassDef classDef;
        classDef.classname = clazz.className.toUtf8();
        classDef.qualified = classDef.classname;
        classDef.superclassList << qMakePair(QByteArray("QScxmlStateMachine"), FunctionDef::Public);
        classDef.hasQObject = true;

        // Event signals:
        foreach (const QString &signalName, m_signalNames) {
            FunctionDef signal;
            signal.type.name = "void";
            signal.type.rawName = signal.type.name;
            signal.normalizedType = signal.type.name;
            signal.name = signalName.toUtf8();
            signal.access = FunctionDef::Public;
            signal.isSignal = true;

            ArgumentDef arg;
            arg.type.name = "const QVariant &";
            arg.type.rawName = arg.type.name;
            arg.normalizedType = "QVariant";
            arg.name = "data";
            arg.typeNameForCast = arg.normalizedType + "*";
            signal.arguments << arg;

            classDef.signalList << signal;
        }

        // stateNames:
        foreach (const QString &stateName, m_stateNames) {
            if (stateName.isEmpty())
                continue;

            const QByteArray mangledStateName = mangledName(stateName, StateName).toUtf8();
            const QString mangledSignalName = mangledName(stateName, SignalName);

            FunctionDef signal;
            signal.type.name = "void";
            signal.type.rawName = signal.type.name;
            signal.normalizedType = signal.type.name;
            signal.name = mangledSignalName.toUtf8();
            signal.access = FunctionDef::Private;
            signal.isSignal = true;
            if (!m_qtMode) {
                signal.implementation = "QMetaObject::activate(_o, &staticMetaObject, %d, _a);";
            } else {
                clazz.signalMethods << QStringLiteral("void %1(bool active);")
                                       .arg(mangledSignalName);
            }
            ArgumentDef arg;
            arg.type.name = "bool";
            arg.type.rawName = arg.type.name;
            arg.normalizedType = arg.type.name;
            arg.name = "active";
            arg.typeNameForCast = arg.type.name + "*";
            signal.arguments << arg;
            classDef.signalList << signal;

            ++classDef.notifyableProperties;
            PropertyDef prop;
            prop.name = stateName.toUtf8();
            prop.type = "bool";
            prop.read = "data->" + mangledStateName + ".active";
            prop.notify = mangledSignalName.toUtf8();
            prop.notifyId = classDef.signalList.size() - 1;
            prop.gspec = PropertyDef::ValueSpec;
            prop.scriptable = "true";
            classDef.propertyList << prop;
        }

        // event slots:
        foreach (const QString &eventName, m_knownEvents) {
            FunctionDef slot;
            slot.type.name = "void";
            slot.type.rawName = slot.type.name;
            slot.normalizedType = slot.type.name;
            slot.name = eventName.toUtf8();
            slot.access = FunctionDef::Public;
            slot.isSlot = true;

            classDef.slotList << slot;

            ArgumentDef arg;
            arg.type.name = "const QVariant &";
            arg.type.rawName = arg.type.name;
            arg.normalizedType = "QVariant";
            arg.name = "data";
            arg.typeNameForCast = arg.normalizedType + "*";
            slot.arguments << arg;

            classDef.slotList << slot;
        }

        // sub-statemachines:
        QHash<QByteArray, QByteArray> knownQObjectClasses;
        knownQObjectClasses.insert(QByteArray("QScxmlStateMachine"), QByteArray());
        Method reg(QStringLiteral("void setService(const QString &id, QScxmlInvokableService *service) Q_DECL_OVERRIDE Q_DECL_FINAL"));
        reg.impl << QStringLiteral("void %1::setService(const QString &id, QScxmlInvokableService *service) {").arg(clazz.className);
        if (m_serviceProps.isEmpty()) {
            reg.impl << QStringLiteral("    Q_UNUSED(id);")
                     << QStringLiteral("    Q_UNUSED(service);");
        }
        for (const auto &service : m_serviceProps) {
            auto serviceName = service.first;
            const QString mangledServiceName = mangledName(serviceName, PlainName);
            const QString fqServiceClass = service.second;
            const QByteArray serviceClass = fqServiceClass.toUtf8();
            knownQObjectClasses.insert(serviceClass, "");

            reg.impl << QStringLiteral("    SET_SERVICE_PROP(%1, %2, %3%2, %4)")
                        .arg(addString(serviceName))
                        .arg(mangledServiceName, namespacePrefix).arg(classDef.signalList.size());

            const QByteArray mangledMachineName = mangledName(serviceName, MachineName).toUtf8();
            const QByteArray mangledSignalName = mangledName(serviceName, SignalName).toUtf8();

            FunctionDef signal;
            signal.type.name = "void";
            signal.type.rawName = signal.type.name;
            signal.normalizedType = signal.type.name;
            signal.name = mangledSignalName;
            signal.access = FunctionDef::Private;
            signal.isSignal = true;
            if (!m_qtMode) {
                signal.implementation = "QMetaObject::activate(_o, &staticMetaObject, %d, _a);";
            }
            ArgumentDef arg;
            arg.type.name = serviceClass + " *";
            arg.type.rawName = arg.type.name;
            arg.type.referenceType = Type::Pointer;
            arg.normalizedType = serviceClass + "*(*)";
            arg.name = "statemachine";
            arg.typeNameForCast = arg.type.name + "*";
            signal.arguments << arg;
            classDef.signalList << signal;

            ++classDef.notifyableProperties;
            PropertyDef prop;
            prop.name = serviceName.toUtf8();
            prop.type = serviceClass + "*";
            prop.read = "data->" + mangledMachineName;
            prop.notify = mangledSignalName;
            prop.notifyId = classDef.signalList.size() - 1;
            prop.gspec = PropertyDef::ValueSpec;
            prop.scriptable = "true";
            classDef.propertyList << prop;
        }
        reg.impl << QStringLiteral("}");
        clazz.protectedMethods.append(reg);

        QBuffer buf(&clazz.metaData);
        buf.open(QIODevice::WriteOnly);
        Generator(&classDef, QList<QByteArray>(), knownQObjectClasses,
                  QHash<QByteArray, QByteArray>(), buf).generateCode();
        buf.close();
    }

    QString qba(const QString &bytes)
    {
        return QStringLiteral("string(%1)").arg(addString(bytes));
    }

    QString scxmlClassName(DocumentModel::ScxmlDocument *doc)
    {
        QString name = mangleIdentifier(translationUnit->classnameForDocument.value(doc));
        Q_ASSERT(!name.isEmpty());
        return namespacePrefix + name;
    }

    /*!
     * \internal
     * Mangles \a str to be a unique C++ identifier. Characters that are invalid for C++ identifiers
     * are replaced by the pattern \c _0x<hex>_ where <hex> is the hexadecimal unicode
     * representation of the character. As identifiers with leading underscores followed by either
     * another underscore or a capital letter are reserved in C++, we also escape those, by escaping
     * the first underscore, using the above method.
     *
     * We keep track of all identifiers we have used so far and if we find two different names that
     * map to the same mangled identifier by the above method, we append underscores to the new one
     * until the result is unique.
     *
     * \note
     * Although C++11 allows for non-ascii (unicode) characters to be used in identifiers,
     * many compilers forgot to read the spec and do not implement this. Some also do not
     * implement C99 identifiers, because that is \e {at the implementation's discretion}. So,
     * we are stuck with plain old boring identifiers.
     */
    QString mangleIdentifier(const QString &str) const
    {
        auto isNonDigit = [](QChar c) -> bool {
            return (c >= QLatin1Char('a') && c <= QLatin1Char('z')) ||
                    (c >= QLatin1Char('A') && c <= QLatin1Char('Z')) ||
                    c == QLatin1Char('_');
        };

        Q_ASSERT(!str.isEmpty());

        QString mangled;
        mangled.reserve(str.size());

        int i = 0;
        if (str.startsWith(QLatin1Char('_')) && str.size() > 1) {
            QChar ch = str.at(1);
            if (ch == QLatin1Char('_')
                    || (ch >= QLatin1Char('A') && ch <= QLatin1Char('Z'))) {
                mangled += QLatin1String("_0x5f_");
                ++i;
            }
        }

        for (int ei = str.length(); i != ei; ++i) {
            auto c = str.at(i).unicode();
            if ((c >= QLatin1Char('0') && c <= QLatin1Char('9')) || isNonDigit(c)) {
                mangled += c;
            } else {
                mangled += QLatin1String("_0x") + QString::number(c, 16) + QLatin1Char('_');
            }
        }

        while (true) {
            auto it = m_mangledToOriginal.constFind(mangled);
            if (it == m_mangledToOriginal.constEnd()) {
                m_mangledToOriginal.insert(mangled, str);
                break;
            } else if (it.value() == str) {
                break;
            }
            mangled += QStringLiteral("_"); // append underscores until we get a unique name
        }

        return mangled;
    }

private:
    QString namespacePrefix;
    ClassDump &clazz;
    TranslationUnit *translationUnit;
    mutable QHash<AbstractState *, QHash<NameForm, QString> > m_mangledNames;
    mutable QHash<QString, QString> m_mangledToOriginal;
    QVector<Node *> m_parents;
    QList<QPair<QString, QString>> m_serviceProps;
    QSet<QString> m_knownEvents;
    QSet<QString> m_signals;
    QStringList m_signalNames;
    QStringList m_stateNames;
    QStringList m_stateFieldNames;
    QString m_currentTransitionName;
    bool m_bindLate;
    bool m_qtMode;
    QVector<DocumentModel::DataElement *> m_dataElements;
};
} // anonymous namespace


void CppDumper::dump(TranslationUnit *unit)
{
    Q_ASSERT(unit);
    Q_ASSERT(unit->mainDocument);

    m_translationUnit = unit;

    QStringList classDecls;
    QVector<ClassDump> clazzes;
    auto docs = m_translationUnit->otherDocuments();
    clazzes.resize(docs.size() + 1);
    DumperVisitor(clazzes[0], m_translationUnit).process(unit->mainDocument);
    for (int i = 0, ei = docs.size(); i != ei; ++i) {
        auto doc = docs.at(i);
        ClassDump &clazz = clazzes[i + 1];
        DumperVisitor(clazz, m_translationUnit).process(doc);
        classDecls.append(clazz.className);
    }

    QString headerName = QFileInfo(unit->outHFileName).fileName();
    const QString headerGuard = headerName.toUpper()
            .replace(QLatin1Char('.'), QLatin1Char('_'))
            .replace(QLatin1Char('-'), QLatin1Char('_'));
    writeHeaderStart(headerGuard, classDecls);
    writeImplStart(clazzes);

    foreach (const ClassDump &clazz, clazzes) {
        writeClass(clazz);
        writeImplBody(clazz);
    }

    classDecls.append(clazzes.at(0).className);
    writeHeaderEnd(headerGuard, classDecls);
    writeImplEnd();
}

void CppDumper::writeHeaderStart(const QString &headerGuard, const QStringList &forwardDecls)
{
    h << doNotEditComment.arg(m_translationUnit->scxmlFileName, QString::number(Q_QSCXMLC_OUTPUT_REVISION), QString::fromLatin1(QT_VERSION_STR))
      << endl;

    h << QStringLiteral("#ifndef ") << headerGuard << endl
      << QStringLiteral("#define ") << headerGuard << endl
      << endl;
    h << l(headerStart);
    if (!m_translationUnit->namespaceName.isEmpty())
        h << l("namespace ") << m_translationUnit->namespaceName << l(" {") << endl << endl;

    if (!forwardDecls.isEmpty()) {
        foreach (const QString &name, forwardDecls) {
            h << QStringLiteral("class %1;").arg(name) << endl;
        }
        h << endl;
    }
}

void CppDumper::writeClass(const ClassDump &clazz)
{
    h << l("class ") << clazz.className << QStringLiteral(": public QScxmlStateMachine\n{") << endl;
    h << QStringLiteral("public:") << endl
      << QStringLiteral("    /* qmake ignore Q_OBJECT */") << endl
      << QStringLiteral("    Q_OBJECT") << endl;
    clazz.properties.write(h, QStringLiteral("    "), QStringLiteral("\n"));

    h << endl
      << QStringLiteral("public:") << endl;
    h << l("    ") << clazz.className << l("(QObject *parent = 0);") << endl;
    h << l("    ~") << clazz.className << "();" << endl;

    if (!clazz.publicMethods.isEmpty()) {
        h << endl;
        foreach (const Method &m, clazz.publicMethods) {
            h << QStringLiteral("    ") << m.decl << QLatin1Char(';') << endl;
        }
    }

    if (!clazz.protectedMethods.isEmpty()) {
        h << endl
          << QStringLiteral("protected:") << endl;
        foreach (const Method &m, clazz.protectedMethods) {
            h << QStringLiteral("    ") << m.decl << QLatin1Char(';') << endl;
        }
    }

    if (!clazz.signalMethods.isEmpty()) {
        h << endl
          << QStringLiteral("signals:") << endl;
        clazz.signalMethods.write(h, QStringLiteral("    "), QStringLiteral("\n"));
    }

    if (!clazz.publicSlotDeclarations.isEmpty()) {
        h << endl
          << QStringLiteral("public slots:") << endl;
        clazz.publicSlotDeclarations.write(h, QStringLiteral("    "), QStringLiteral("\n"));
    }

    h << endl
      << l("private:") << endl
      << l("    struct Data;") << endl
      << l("    friend struct Data;") << endl
      << l("    struct Data *data;") << endl
      << l("};") << endl << endl;
}

void CppDumper::writeHeaderEnd(const QString &headerGuard, const QStringList &metatypeDecls)
{
    QString ns;
    if (!m_translationUnit->namespaceName.isEmpty()) {
        h << QStringLiteral("} // %1 namespace ").arg(m_translationUnit->namespaceName) << endl
          << endl;
        ns = QStringLiteral("::%1").arg(m_translationUnit->namespaceName);
    }

    foreach (const QString &name, metatypeDecls) {
        h << QStringLiteral("Q_DECLARE_METATYPE(%1::%2*);").arg(ns, name) << endl;
    }
    h << endl;

    h << QStringLiteral("#endif // ") << headerGuard << endl;
}

void CppDumper::writeImplStart(const QVector<ClassDump> &allClazzes)
{
    cpp << doNotEditComment.arg(m_translationUnit->scxmlFileName, QString::number(Q_QSCXMLC_OUTPUT_REVISION), QString::fromLatin1(QT_VERSION_STR))
        << endl;

    StringListDumper includes;
    foreach (const ClassDump &clazz, allClazzes) {
        includes.text += clazz.implIncludes.text;
    }
    includes.unique();

    QString headerName = QFileInfo(m_translationUnit->outHFileName).fileName();
    cpp << l("#include \"") << headerName << l("\"") << endl;
    cpp << endl
        << QStringLiteral("#include <qscxmlqstates.h>") << endl
        << QStringLiteral("#include <qscxmltabledata.h>") << endl;
    if (!includes.isEmpty()) {
        includes.write(cpp, QStringLiteral("#include <"), QStringLiteral(">\n"));
        cpp << endl;
    }
    cpp << endl
        << revisionCheck.arg(m_translationUnit->scxmlFileName, QString::number(Q_QSCXMLC_OUTPUT_REVISION), QString::fromLatin1(QT_VERSION_STR))
        << endl;
    if (!m_translationUnit->namespaceName.isEmpty())
        cpp << l("namespace ") << m_translationUnit->namespaceName << l(" {") << endl << endl;
}

void CppDumper::writeImplBody(const ClassDump &clazz)
{
    cpp << l("struct ") << clazz.className << l("::Data: private QScxmlTableData");
    if (clazz.needsEventFilter) {
        cpp << QStringLiteral(", public QScxmlEventFilter");
    }
    cpp << l(" {") << endl;

    cpp << QStringLiteral("    Data(%1 &stateMachine)").arg(clazz.className) << endl
        << QStringLiteral("        : stateMachine(stateMachine)") << endl;
    clazz.constructor.initializer.write(cpp, QStringLiteral("        , "), QStringLiteral("\n"));
    cpp << l("    {") << endl;
    clazz.constructor.impl.write(cpp, QStringLiteral("        "), QStringLiteral("\n"));
    cpp << l("    }") << endl;

    cpp << endl;
    cpp << l("    void init() {\n");
    clazz.init.impl.write(cpp, QStringLiteral("        "), QStringLiteral("\n"));
    cpp << l("    }") << endl;
    cpp << endl;
    clazz.dataMethods.write(cpp, QStringLiteral("    "), QStringLiteral("\n"));

    cpp << endl
        << QStringLiteral("    %1 &stateMachine;").arg(clazz.className) << endl;
    clazz.classFields.write(cpp, QStringLiteral("    "), QStringLiteral("\n"));

    cpp << l("};") << endl
        << endl;
    clazz.classMethods.write(cpp, QStringLiteral(""), QStringLiteral("\n"));

    cpp << clazz.className << l("::") << clazz.className << l("(QObject *parent)") << endl
        << QStringLiteral("    : QScxmlStateMachine(parent)") << endl
        << QStringLiteral("    , data(new Data(*this))") << endl
        << QStringLiteral("{ qRegisterMetaType<%1 *>(); data->init(); }").arg(clazz.className) << endl
        << endl;
    cpp << clazz.className << l("::~") << clazz.className << l("()") << endl
        << l("{ delete data; }") << endl
        << endl;
    foreach (const Method &m, clazz.publicMethods) {
        m.impl.write(cpp, QStringLiteral(""), QStringLiteral("\n"), clazz.className);
        cpp << endl;
    }
    if (!clazz.protectedMethods.isEmpty()) {
        cpp << "#define SET_SERVICE_PROP(s, n, fq, sig) \\\n"
               "    if (id == data->string(s)) { \\\n"
               "        QScxmlInvokableScxml *machine = service ? dynamic_cast<QScxmlInvokableScxml *>(service) : Q_NULLPTR; \\\n"
               "        fq *casted = machine ? dynamic_cast<fq*>(machine->stateMachine()) : Q_NULLPTR; \\\n"
               "        if (data->n != casted) { \\\n"
               "            data->n = casted; \\\n"
               "            void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&casted)) }; \\\n"
               "            QMetaObject::activate(this, &staticMetaObject, sig, _a); \\\n"
               "        } \\\n"
               "        return; \\\n"
               "    }\n"
            << endl;
        foreach (const Method &m, clazz.protectedMethods) {
            m.impl.write(cpp, QStringLiteral(""), QStringLiteral("\n"), clazz.className);
            cpp << endl;
        }
        cpp << QStringLiteral("#undef SET_SERVICE_PROP") << endl
            << endl;
    }
    clazz.publicSlotDefinitions.write(cpp, QStringLiteral("\n"), QStringLiteral("\n"));
    cpp << endl;

    clazz.tables.write(cpp, QStringLiteral(""), QStringLiteral("\n"));

    if (!clazz.dataModelMethods.isEmpty()) {
        bool first = true;
        foreach (const Method &m, clazz.dataModelMethods) {
            if (first) {
                first = false;
            } else {
                cpp << endl;
            }
            m.impl.write(cpp, QStringLiteral(""), QStringLiteral("\n"));
        }
    }

    cpp << endl << clazz.metaData;
}

void CppDumper::writeImplEnd()
{
    if (!m_translationUnit->namespaceName.isEmpty()) {
        cpp << endl
            << QStringLiteral("} // %1 namespace").arg(m_translationUnit->namespaceName) << endl;
    }
}

QT_END_NAMESPACE
