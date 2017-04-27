// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "platform/v8_inspector/V8InjectedScriptHost.h"

#include "platform/inspector_protocol/String16.h"
#include "platform/v8_inspector/InjectedScriptNative.h"
#include "platform/v8_inspector/V8Compat.h"
#include "platform/v8_inspector/V8DebuggerImpl.h"
#include "platform/v8_inspector/V8StringUtil.h"
#include "platform/v8_inspector/V8ValueCopier.h"
#include "platform/v8_inspector/public/V8DebuggerClient.h"

namespace blink {

namespace {

void setFunctionProperty(v8::Local<v8::Context> context, v8::Local<v8::Object> obj, const char* name, v8::FunctionCallback callback, v8::Local<v8::External> external)
{
    v8::Local<v8::String> funcName = toV8StringInternalized(context->GetIsolate(), name);
    v8::Local<v8::Function> func;
    if (!v8::Function::New(context, callback, external, 0, v8::ConstructorBehavior::kThrow).ToLocal(&func))
        return;
    func->SetName(funcName);
    createDataProperty(context, obj, funcName, func);
}

V8DebuggerImpl* unwrapDebugger(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK(!info.Data().IsEmpty());
    DCHECK(info.Data()->IsExternal());
    V8DebuggerImpl* debugger = static_cast<V8DebuggerImpl*>(info.Data().As<v8::External>()->Value());
    DCHECK(debugger);
    return debugger;
}

} // namespace

v8::Local<v8::Object> V8InjectedScriptHost::create(v8::Local<v8::Context> context, V8DebuggerImpl* debugger)
{
    v8::Isolate* isolate = debugger->isolate();
    v8::Local<v8::Object> injectedScriptHost = v8::Object::New(isolate);
    bool success = injectedScriptHost->SetPrototype(context, v8::Null(isolate)).FromMaybe(false);
    DCHECK(success);
    v8::Local<v8::External> debuggerExternal = v8::External::New(isolate, debugger);
    setFunctionProperty(context, injectedScriptHost, "internalConstructorName", V8InjectedScriptHost::internalConstructorNameCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "formatAccessorsAsProperties", V8InjectedScriptHost::formatAccessorsAsProperties, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "isTypedArray", V8InjectedScriptHost::isTypedArrayCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "subtype", V8InjectedScriptHost::subtypeCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "getInternalProperties", V8InjectedScriptHost::getInternalPropertiesCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "suppressWarningsAndCallFunction", V8InjectedScriptHost::suppressWarningsAndCallFunctionCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "bind", V8InjectedScriptHost::bindCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "proxyTargetValue", V8InjectedScriptHost::proxyTargetValueCallback, debuggerExternal);
    setFunctionProperty(context, injectedScriptHost, "prototype", V8InjectedScriptHost::prototypeCallback, debuggerExternal);
    return injectedScriptHost;
}

v8::Local<v8::Private> V8InjectedScriptHost::internalEntryPrivate(v8::Isolate* isolate)
{
    return v8::Private::ForApi(isolate, toV8StringInternalized(isolate, "V8InjectedScriptHost#internalEntry"));
}

void V8InjectedScriptHost::internalConstructorNameCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1 || !info[0]->IsObject())
        return;

    v8::Local<v8::Object> object = info[0].As<v8::Object>();
    info.GetReturnValue().Set(object->GetConstructorName());
}

void V8InjectedScriptHost::formatAccessorsAsProperties(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK_EQ(info.Length(), 2);
    info.GetReturnValue().Set(false);
    if (!info[1]->IsFunction())
        return;
    // Check that function is user-defined.
    if (info[1].As<v8::Function>()->ScriptId() != v8::UnboundScript::kNoScriptId)
        return;
    info.GetReturnValue().Set(unwrapDebugger(info)->client()->formatAccessorsAsProperties(info[0]));
}

void V8InjectedScriptHost::isTypedArrayCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1)
        return;

    info.GetReturnValue().Set(info[0]->IsTypedArray());
}

void V8InjectedScriptHost::subtypeCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1)
        return;

    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Value> value = info[0];
    if (value->IsArray() || value->IsTypedArray() || value->IsArgumentsObject()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "array"));
        return;
    }
    if (value->IsDate()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "date"));
        return;
    }
    if (value->IsRegExp()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "regexp"));
        return;
    }
    if (value->IsMap() || value->IsWeakMap()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "map"));
        return;
    }
    if (value->IsSet() || value->IsWeakSet()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "set"));
        return;
    }
    if (value->IsMapIterator() || value->IsSetIterator()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "iterator"));
        return;
    }
    if (value->IsGeneratorObject()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "generator"));
        return;
    }
    if (value->IsNativeError()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "error"));
        return;
    }
    if (value->IsProxy()) {
        info.GetReturnValue().Set(toV8StringInternalized(isolate, "proxy"));
        return;
    }
    if (value->IsObject()) {
        v8::Local<v8::Object> obj = value.As<v8::Object>();
        if (obj->HasPrivate(isolate->GetCurrentContext(), internalEntryPrivate(isolate)).FromMaybe(false)) {
            info.GetReturnValue().Set(toV8StringInternalized(isolate, "internal#entry"));
            return;
        }
    }
    String16 subtype = unwrapDebugger(info)->client()->valueSubtype(value);
    if (!subtype.isEmpty()) {
        info.GetReturnValue().Set(toV8String(isolate, subtype));
        return;
    }
}

void V8InjectedScriptHost::getInternalPropertiesCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 1)
        return;
    v8::Local<v8::Array> properties;
    if (unwrapDebugger(info)->internalProperties(info.GetIsolate()->GetCurrentContext(), info[0]).ToLocal(&properties))
        info.GetReturnValue().Set(properties);
}

void V8InjectedScriptHost::suppressWarningsAndCallFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 2 || info.Length() > 3 || !info[0]->IsFunction()) {
        NOTREACHED();
        return;
    }
    if (info.Length() > 2 && (!info[2]->IsArray() && !info[2]->IsUndefined())) {
        NOTREACHED();
        return;
    }

    v8::Isolate* isolate = info.GetIsolate();
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    v8::Local<v8::Function> function = info[0].As<v8::Function>();
    v8::Local<v8::Value> receiver = info[1];
    std::unique_ptr<v8::Local<v8::Value>[]> argv = nullptr;
    size_t argc = 0;

    if (info.Length() > 2 && info[2]->IsArray()) {
        v8::Local<v8::Array> arguments = info[2].As<v8::Array>();
        argc = arguments->Length();
        argv.reset(new v8::Local<v8::Value>[argc]);
        for (size_t i = 0; i < argc; ++i) {
            if (!arguments->Get(context, i).ToLocal(&argv[i]))
                return;
        }
    }

    V8DebuggerImpl* debugger = unwrapDebugger(info);
    debugger->client()->muteWarningsAndDeprecations();
    debugger->muteConsole();

    v8::MicrotasksScope microtasks(isolate, v8::MicrotasksScope::kDoNotRunMicrotasks);
    v8::Local<v8::Value> result;
    if (function->Call(context, receiver, argc, argv.get()).ToLocal(&result))
        info.GetReturnValue().Set(result);

    debugger->unmuteConsole();
    debugger->client()->unmuteWarningsAndDeprecations();
}

void V8InjectedScriptHost::bindCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() < 2 || !info[1]->IsString())
        return;
    InjectedScriptNative* injectedScriptNative = InjectedScriptNative::fromInjectedScriptHost(info.Holder());
    if (!injectedScriptNative)
        return;

    v8::Local<v8::String> v8groupName = info[1]->ToString(info.GetIsolate());
    String16 groupName = toProtocolStringWithTypeCheck(v8groupName);
    int id = injectedScriptNative->bind(info[0], groupName);
    info.GetReturnValue().Set(id);
}

void V8InjectedScriptHost::proxyTargetValueCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    if (info.Length() != 1 || !info[0]->IsProxy()) {
        NOTREACHED();
        return;
    }
    v8::Local<v8::Object> target = info[0].As<v8::Proxy>();
    while (target->IsProxy())
        target = v8::Local<v8::Proxy>::Cast(target)->GetTarget();
    info.GetReturnValue().Set(target);
}

void V8InjectedScriptHost::prototypeCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK(info.Length() > 0 && info[0]->IsObject());
    info.GetReturnValue().Set(info[0].As<v8::Object>()->GetPrototype());
}

} // namespace blink
