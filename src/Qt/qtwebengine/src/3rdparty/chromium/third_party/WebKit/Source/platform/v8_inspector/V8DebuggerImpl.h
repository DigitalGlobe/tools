/*
 * Copyright (c) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef V8DebuggerImpl_h
#define V8DebuggerImpl_h

#include "platform/inspector_protocol/Collections.h"
#include "platform/inspector_protocol/Maybe.h"
#include "platform/inspector_protocol/Platform.h"
#include "platform/v8_inspector/JavaScriptCallFrame.h"
#include "platform/v8_inspector/V8DebuggerScript.h"
#include "platform/v8_inspector/protocol/Debugger.h"
#include "platform/v8_inspector/public/V8Debugger.h"

#include <v8-debug.h>
#include <v8.h>
#include <vector>

#include <vector>

namespace blink {

using protocol::Maybe;

struct ScriptBreakpoint;
class InspectedContext;
class V8ConsoleMessageStorage;
class V8DebuggerAgentImpl;
class V8InspectorSessionImpl;
class V8RuntimeAgentImpl;
class V8StackTraceImpl;

class V8DebuggerImpl : public V8Debugger {
    PROTOCOL_DISALLOW_COPY(V8DebuggerImpl);
public:
    V8DebuggerImpl(v8::Isolate*, V8DebuggerClient*);
    ~V8DebuggerImpl() override;

    static int contextId(v8::Local<v8::Context>);

    bool enabled() const;

    String16 setBreakpoint(const String16& sourceID, const ScriptBreakpoint&, int* actualLineNumber, int* actualColumnNumber, bool interstatementLocation);
    void removeBreakpoint(const String16& breakpointId);
    void setBreakpointsActivated(bool);
    bool breakpointsActivated() const { return m_breakpointsActivated; }

    enum PauseOnExceptionsState {
        DontPauseOnExceptions,
        PauseOnAllExceptions,
        PauseOnUncaughtExceptions
    };
    PauseOnExceptionsState getPauseOnExceptionsState();
    void setPauseOnExceptionsState(PauseOnExceptionsState);
    void setPauseOnNextStatement(bool);
    bool canBreakProgram();
    void breakProgram();
    void continueProgram();
    void stepIntoStatement();
    void stepOverStatement();
    void stepOutOfFunction();
    void clearStepping();

    bool setScriptSource(const String16& sourceID, v8::Local<v8::String> newSource, bool preview, ErrorString*, Maybe<protocol::Debugger::SetScriptSourceError>*, JavaScriptCallFrames* newCallFrames, Maybe<bool>* stackChanged);
    JavaScriptCallFrames currentCallFrames(int limit = 0);

    // Each script inherits debug data from v8::Context where it has been compiled.
    // Only scripts whose debug data matches |contextGroupId| will be reported.
    // Passing 0 will result in reporting all scripts.
    void getCompiledScripts(int contextGroupId, std::vector<std::unique_ptr<V8DebuggerScript>>&);
    void debuggerAgentEnabled();
    void debuggerAgentDisabled();

    bool isPaused() override;
    v8::Local<v8::Context> pausedContext() { return m_pausedContext; }
    int maxAsyncCallChainDepth() { return m_maxAsyncCallStackDepth; }
    V8StackTraceImpl* currentAsyncCallChain();
    void setAsyncCallStackDepth(V8DebuggerAgentImpl*, int);

    v8::MaybeLocal<v8::Value> functionScopes(v8::Local<v8::Context>, v8::Local<v8::Function>);
    v8::MaybeLocal<v8::Array> internalProperties(v8::Local<v8::Context>, v8::Local<v8::Value>);
    v8::Local<v8::Value> generatorObjectDetails(v8::Local<v8::Context>, v8::Local<v8::Object>&);

    v8::Isolate* isolate() const { return m_isolate; }
    V8DebuggerClient* client() { return m_client; }

    v8::MaybeLocal<v8::Value> runCompiledScript(v8::Local<v8::Context>, v8::Local<v8::Script>);
    v8::MaybeLocal<v8::Value> callFunction(v8::Local<v8::Function>, v8::Local<v8::Context>, v8::Local<v8::Value> receiver, int argc, v8::Local<v8::Value> info[]);
    v8::MaybeLocal<v8::Value> compileAndRunInternalScript(v8::Local<v8::Context>, v8::Local<v8::String>);
    v8::Local<v8::Script> compileInternalScript(v8::Local<v8::Context>, v8::Local<v8::String>, const String16& fileName);
    v8::Local<v8::Context> regexContext();

    void enableStackCapturingIfNeeded();
    void disableStackCapturingIfNeeded();
    V8ConsoleMessageStorage* ensureConsoleMessageStorage(int contextGroupId);
    bool hasConsoleMessageStorage(int contextGroupId);

    // V8Debugger implementation
    std::unique_ptr<V8InspectorSession> connect(int contextGroupId, protocol::FrontendChannel*, V8InspectorSessionClient*, const String16* state) override;
    void contextCreated(const V8ContextInfo&) override;
    void contextDestroyed(v8::Local<v8::Context>) override;
    void resetContextGroup(int contextGroupId) override;
    void willExecuteScript(v8::Local<v8::Context>, int scriptId) override;
    void didExecuteScript(v8::Local<v8::Context>) override;
    void idleStarted() override;
    void idleFinished() override;
    bool addConsoleMessage(int contextGroupId, MessageSource, MessageLevel, const String16& message, const String16& url, unsigned lineNumber, unsigned columnNumber, std::unique_ptr<V8StackTrace>, int scriptId, const String16& requestIdentifier) override;
    void logToConsole(v8::Local<v8::Context>, const String16& message, v8::Local<v8::Value> arg1, v8::Local<v8::Value> arg2) override;
    unsigned promiseRejected(v8::Local<v8::Context>, const String16& errorMessage, v8::Local<v8::Value> reason, const String16& url, unsigned lineNumber, unsigned columnNumber, std::unique_ptr<V8StackTrace>, int scriptId) override;
    void promiseRejectionRevoked(v8::Local<v8::Context>, unsigned promiseRejectionId) override;
    void consoleMessagesCount(int contextGroupId, unsigned* total, unsigned* withArguments) override;
    std::unique_ptr<V8StackTrace> createStackTrace(v8::Local<v8::StackTrace>) override;
    std::unique_ptr<V8StackTrace> captureStackTrace(bool fullStack) override;
    void asyncTaskScheduled(const String16& taskName, void* task, bool recurring) override;
    void asyncTaskCanceled(void* task) override;
    void asyncTaskStarted(void* task) override;
    void asyncTaskFinished(void* task) override;
    void allAsyncTasksCanceled() override;
    void muteConsole() override { m_muteConsoleCount++; }
    void unmuteConsole() override { m_muteConsoleCount--; }

    using ContextByIdMap = protocol::HashMap<int, std::unique_ptr<InspectedContext>>;
    void discardInspectedContext(int contextGroupId, int contextId);
    const ContextByIdMap* contextGroup(int contextGroupId);
    void disconnect(V8InspectorSessionImpl*);
    V8InspectorSessionImpl* sessionForContextGroup(int contextGroupId);
    InspectedContext* getContext(int groupId, int contextId) const;

private:
    void enable();
    void disable();
    V8DebuggerAgentImpl* findEnabledDebuggerAgent(int contextGroupId);
    V8DebuggerAgentImpl* findEnabledDebuggerAgent(v8::Local<v8::Context>);

    void compileDebuggerScript();
    v8::MaybeLocal<v8::Value> callDebuggerMethod(const char* functionName, int argc, v8::Local<v8::Value> argv[]);
    v8::Local<v8::Context> debuggerContext() const;
    void clearBreakpoints();

    static void breakProgramCallback(const v8::FunctionCallbackInfo<v8::Value>&);
    void handleProgramBreak(v8::Local<v8::Context> pausedContext, v8::Local<v8::Object> executionState, v8::Local<v8::Value> exception, v8::Local<v8::Array> hitBreakpoints, bool isPromiseRejection = false);
    static void v8DebugEventCallback(const v8::Debug::EventDetails&);
    v8::Local<v8::Value> callInternalGetterFunction(v8::Local<v8::Object>, const char* functionName);
    void handleV8DebugEvent(const v8::Debug::EventDetails&);

    v8::Local<v8::String> v8InternalizedString(const char*) const;

    void handleV8AsyncTaskEvent(v8::Local<v8::Context>, v8::Local<v8::Object> executionState, v8::Local<v8::Object> eventData);

    using ContextsByGroupMap = protocol::HashMap<int, std::unique_ptr<ContextByIdMap>>;

    v8::Local<v8::Value> collectionEntries(v8::Local<v8::Context>, v8::Local<v8::Object>);

    v8::Isolate* m_isolate;
    V8DebuggerClient* m_client;
    ContextsByGroupMap m_contexts;
    using SessionMap = protocol::HashMap<int, V8InspectorSessionImpl*>;
    SessionMap m_sessions;
    using ConsoleStorageMap = protocol::HashMap<int, std::unique_ptr<V8ConsoleMessageStorage>>;
    ConsoleStorageMap m_consoleStorageMap;
    int m_capturingStackTracesCount;
    int m_muteConsoleCount;
    unsigned m_lastConsoleMessageId;
    int m_enabledAgentsCount;
    bool m_breakpointsActivated;
    v8::Global<v8::Object> m_debuggerScript;
    v8::Global<v8::Context> m_debuggerContext;
    v8::Local<v8::Object> m_executionState;
    v8::Local<v8::Context> m_pausedContext;
    bool m_runningNestedMessageLoop;
    v8::Global<v8::Context> m_regexContext;

    using AsyncTaskToStackTrace = protocol::HashMap<void*, std::unique_ptr<V8StackTraceImpl>>;
    AsyncTaskToStackTrace m_asyncTaskStacks;
    protocol::HashSet<void*> m_recurringTasks;
    int m_maxAsyncCallStackDepth;
    std::vector<void*> m_currentTasks;
    std::vector<std::unique_ptr<V8StackTraceImpl>> m_currentStacks;
    protocol::HashMap<V8DebuggerAgentImpl*, int> m_maxAsyncCallStackDepthMap;
};

} // namespace blink


#endif // V8DebuggerImpl_h
