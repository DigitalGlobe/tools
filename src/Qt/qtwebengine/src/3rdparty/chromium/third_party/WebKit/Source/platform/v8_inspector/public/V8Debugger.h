// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8Debugger_h
#define V8Debugger_h

#include "platform/inspector_protocol/Collections.h"
#include "platform/inspector_protocol/Platform.h"
#include "platform/inspector_protocol/String16.h"
#include "platform/v8_inspector/public/V8ConsoleTypes.h"

#include <v8.h>
#include <vector>

namespace blink {

class V8ContextInfo;
class V8DebuggerClient;
class V8InspectorSession;
class V8InspectorSessionClient;
class V8StackTrace;

namespace protocol {
class DictionaryValue;
class FrontendChannel;
}

class PLATFORM_EXPORT V8Debugger {
public:
    static std::unique_ptr<V8Debugger> create(v8::Isolate*, V8DebuggerClient*);
    virtual ~V8Debugger() { }

    virtual void contextCreated(const V8ContextInfo&) = 0;
    virtual void contextDestroyed(v8::Local<v8::Context>) = 0;
    // TODO(dgozman): remove this one.
    virtual void resetContextGroup(int contextGroupId) = 0;
    virtual void willExecuteScript(v8::Local<v8::Context>, int scriptId) = 0;
    virtual void didExecuteScript(v8::Local<v8::Context>) = 0;
    virtual void idleStarted() = 0;
    virtual void idleFinished() = 0;
    // TODO(dgozman): remove requestIdentifier.
    virtual bool addConsoleMessage(int contextGroupId, MessageSource, MessageLevel, const String16& message, const String16& url, unsigned lineNumber, unsigned columnNumber, std::unique_ptr<V8StackTrace>, int scriptId, const String16& requestIdentifier) = 0;
    // TODO(dgozman): can we remove this method?
    virtual void logToConsole(v8::Local<v8::Context>, const String16& message, v8::Local<v8::Value> arg1, v8::Local<v8::Value> arg2) = 0;
    virtual unsigned promiseRejected(v8::Local<v8::Context>, const String16& errorMessage, v8::Local<v8::Value> reason, const String16& url, unsigned lineNumber, unsigned columnNumber, std::unique_ptr<V8StackTrace>, int scriptId) = 0;
    virtual void promiseRejectionRevoked(v8::Local<v8::Context>, unsigned promiseRejectionId) = 0;
    virtual void consoleMessagesCount(int contextGroupId, unsigned* total, unsigned* withArguments) = 0;
    // TODO(dgozman): remove mute methods.
    virtual void muteConsole() = 0;
    virtual void unmuteConsole() = 0;

    virtual std::unique_ptr<V8InspectorSession> connect(int contextGroupId, protocol::FrontendChannel*, V8InspectorSessionClient*, const String16* state) = 0;
    virtual bool isPaused() = 0;

    virtual std::unique_ptr<V8StackTrace> createStackTrace(v8::Local<v8::StackTrace>) = 0;
    virtual std::unique_ptr<V8StackTrace> captureStackTrace(bool fullStack) = 0;

    // API to report async call stacks.
    virtual void asyncTaskScheduled(const String16& taskName, void* task, bool recurring) = 0;
    virtual void asyncTaskCanceled(void* task) = 0;
    virtual void asyncTaskStarted(void* task) = 0;
    virtual void asyncTaskFinished(void* task) = 0;
    virtual void allAsyncTasksCanceled() = 0;
};

} // namespace blink


#endif // V8Debugger_h
