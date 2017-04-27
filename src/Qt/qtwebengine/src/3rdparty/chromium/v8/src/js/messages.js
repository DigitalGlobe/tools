// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// -------------------------------------------------------------------

(function(global, utils) {

%CheckIsBootstrapping();

// -------------------------------------------------------------------
// Imports

var ArrayJoin;
var Bool16x8ToString;
var Bool32x4ToString;
var Bool8x16ToString;
var callSiteReceiverSymbol =
    utils.ImportNow("call_site_receiver_symbol");
var callSiteFunctionSymbol =
    utils.ImportNow("call_site_function_symbol");
var callSitePositionSymbol =
    utils.ImportNow("call_site_position_symbol");
var callSiteStrictSymbol =
    utils.ImportNow("call_site_strict_symbol");
var callSiteWasmObjectSymbol =
    utils.ImportNow("call_site_wasm_obj_symbol");
var callSiteWasmFunctionIndexSymbol =
    utils.ImportNow("call_site_wasm_func_index_symbol");
var Float32x4ToString;
var formattedStackTraceSymbol =
    utils.ImportNow("formatted_stack_trace_symbol");
var GlobalObject = global.Object;
var Int16x8ToString;
var Int32x4ToString;
var Int8x16ToString;
var InternalArray = utils.InternalArray;
var internalErrorSymbol = utils.ImportNow("internal_error_symbol");
var ObjectHasOwnProperty;
var ObjectToString = utils.ImportNow("object_to_string");
var Script = utils.ImportNow("Script");
var stackTraceSymbol = utils.ImportNow("stack_trace_symbol");
var StringIndexOf;
var SymbolToString;
var toStringTagSymbol = utils.ImportNow("to_string_tag_symbol");
var Uint16x8ToString;
var Uint32x4ToString;
var Uint8x16ToString;

utils.Import(function(from) {
  ArrayJoin = from.ArrayJoin;
  Bool16x8ToString = from.Bool16x8ToString;
  Bool32x4ToString = from.Bool32x4ToString;
  Bool8x16ToString = from.Bool8x16ToString;
  Float32x4ToString = from.Float32x4ToString;
  Int16x8ToString = from.Int16x8ToString;
  Int32x4ToString = from.Int32x4ToString;
  Int8x16ToString = from.Int8x16ToString;
  ObjectHasOwnProperty = from.ObjectHasOwnProperty;
  StringIndexOf = from.StringIndexOf;
  SymbolToString = from.SymbolToString;
  Uint16x8ToString = from.Uint16x8ToString;
  Uint32x4ToString = from.Uint32x4ToString;
  Uint8x16ToString = from.Uint8x16ToString;
});

// -------------------------------------------------------------------

var GlobalError;
var GlobalTypeError;
var GlobalRangeError;
var GlobalURIError;
var GlobalSyntaxError;
var GlobalReferenceError;
var GlobalEvalError;


function NoSideEffectsObjectToString() {
  if (IS_UNDEFINED(this)) return "[object Undefined]";
  if (IS_NULL(this)) return "[object Null]";
  var O = TO_OBJECT(this);
  var builtinTag = %_ClassOf(O);
  var tag = %GetDataProperty(O, toStringTagSymbol);
  if (!IS_STRING(tag)) {
    tag = builtinTag;
  }
  return `[object ${tag}]`;
}

function IsErrorObject(obj) {
  return HAS_PRIVATE(obj, stackTraceSymbol);
}

function NoSideEffectsErrorToString() {
  var name = %GetDataProperty(this, "name");
  var message = %GetDataProperty(this, "message");
  name = IS_UNDEFINED(name) ? "Error" : NoSideEffectsToString(name);
  message = IS_UNDEFINED(message) ? "" : NoSideEffectsToString(message);
  if (name == "") return message;
  if (message == "") return name;
  return `${name}: ${message}`;
}

function NoSideEffectsToString(obj) {
  if (IS_STRING(obj)) return obj;
  if (IS_NUMBER(obj)) return %_NumberToString(obj);
  if (IS_BOOLEAN(obj)) return obj ? 'true' : 'false';
  if (IS_UNDEFINED(obj)) return 'undefined';
  if (IS_NULL(obj)) return 'null';
  if (IS_FUNCTION(obj)) {
    var str = %FunctionToString(obj);
    if (str.length > 128) {
      str = %_SubString(str, 0, 111) + "...<omitted>..." +
            %_SubString(str, str.length - 2, str.length);
    }
    return str;
  }
  if (IS_SYMBOL(obj)) return %_Call(SymbolToString, obj);
  if (IS_SIMD_VALUE(obj)) {
    switch (typeof(obj)) {
      case 'float32x4': return %_Call(Float32x4ToString, obj);
      case 'int32x4':   return %_Call(Int32x4ToString, obj);
      case 'int16x8':   return %_Call(Int16x8ToString, obj);
      case 'int8x16':   return %_Call(Int8x16ToString, obj);
      case 'uint32x4':   return %_Call(Uint32x4ToString, obj);
      case 'uint16x8':   return %_Call(Uint16x8ToString, obj);
      case 'uint8x16':   return %_Call(Uint8x16ToString, obj);
      case 'bool32x4':  return %_Call(Bool32x4ToString, obj);
      case 'bool16x8':  return %_Call(Bool16x8ToString, obj);
      case 'bool8x16':  return %_Call(Bool8x16ToString, obj);
    }
  }

  if (IS_RECEIVER(obj)) {
    // When internally formatting error objects, use a side-effects-free version
    // of Error.prototype.toString independent of the actually installed
    // toString method.
    if (IsErrorObject(obj) ||
        %GetDataProperty(obj, "toString") === ErrorToString) {
      return %_Call(NoSideEffectsErrorToString, obj);
    }

    if (%GetDataProperty(obj, "toString") === ObjectToString) {
      var constructor = %GetDataProperty(obj, "constructor");
      if (IS_FUNCTION(constructor)) {
        var constructor_name = %FunctionGetName(constructor);
        if (constructor_name != "") return `#<${constructor_name}>`;
      }
    }
  }

  return %_Call(NoSideEffectsObjectToString, obj);
}


function MakeGenericError(constructor, type, arg0, arg1, arg2) {
  var error = new constructor(FormatMessage(type, arg0, arg1, arg2));
  error[internalErrorSymbol] = true;
  return error;
}


/**
 * Set up the Script function and constructor.
 */
%FunctionSetInstanceClassName(Script, 'Script');
%AddNamedProperty(Script.prototype, 'constructor', Script,
                  DONT_ENUM | DONT_DELETE | READ_ONLY);
%SetCode(Script, function(x) {
  // Script objects can only be created by the VM.
  throw MakeError(kUnsupported);
});


// Helper functions; called from the runtime system.
function FormatMessage(type, arg0, arg1, arg2) {
  var arg0 = NoSideEffectsToString(arg0);
  var arg1 = NoSideEffectsToString(arg1);
  var arg2 = NoSideEffectsToString(arg2);
  try {
    return %FormatMessageString(type, arg0, arg1, arg2);
  } catch (e) {
    return "<error>";
  }
}


function GetLineNumber(message) {
  var start_position = %MessageGetStartPosition(message);
  if (start_position == -1) return kNoLineNumberInfo;
  var script = %MessageGetScript(message);
  var location = script.locationFromPosition(start_position, true);
  if (location == null) return kNoLineNumberInfo;
  return location.line + 1;
}


//Returns the offset of the given position within the containing line.
function GetColumnNumber(message) {
  var script = %MessageGetScript(message);
  var start_position = %MessageGetStartPosition(message);
  var location = script.locationFromPosition(start_position, true);
  if (location == null) return -1;
  return location.column;
}


// Returns the source code line containing the given source
// position, or the empty string if the position is invalid.
function GetSourceLine(message) {
  var script = %MessageGetScript(message);
  var start_position = %MessageGetStartPosition(message);
  var location = script.locationFromPosition(start_position, true);
  if (location == null) return "";
  return location.sourceText;
}


/**
 * Get information on a specific source position.
 * Returns an object with the following following properties:
 *   script     : script object for the source
 *   line       : source line number
 *   column     : source column within the line
 *   position   : position within the source
 *   sourceText : a string containing the current line
 * @param {number} position The source position
 * @param {boolean} include_resource_offset Set to true to have the resource
 *     offset added to the location
 * @return If line is negative or not in the source null is returned.
 */
function ScriptLocationFromPosition(position,
                                    include_resource_offset) {
  return %ScriptPositionInfo(this, position, !!include_resource_offset);
}


/**
 * If sourceURL comment is available returns sourceURL comment contents.
 * Otherwise, script name is returned. See
 * http://fbug.googlecode.com/svn/branches/firebug1.1/docs/ReleaseNotes_1.1.txt
 * and Source Map Revision 3 proposal for details on using //# sourceURL and
 * deprecated //@ sourceURL comment to identify scripts that don't have name.
 *
 * @return {?string} script name if present, value for //# sourceURL comment or
 * deprecated //@ sourceURL comment otherwise.
 */
function ScriptNameOrSourceURL() {
  if (this.source_url) return this.source_url;
  return this.name;
}


utils.SetUpLockedPrototype(Script, [
    "source",
    "name",
    "source_url",
    "source_mapping_url",
    "line_offset",
    "column_offset"
  ], [
    "locationFromPosition", ScriptLocationFromPosition,
    "nameOrSourceURL", ScriptNameOrSourceURL,
  ]
);


function GetStackTraceLine(recv, fun, pos, isGlobal) {
  return new CallSite(recv, fun, pos, false).toString();
}

// ----------------------------------------------------------------------------
// Error implementation

function CallSite(receiver, fun, pos, strict_mode) {
  // For wasm frames, receiver is the wasm object and fun is the function index
  // instead of an actual function.
  if (!IS_FUNCTION(fun) && !%IsWasmObject(receiver)) {
    throw MakeTypeError(kCallSiteExpectsFunction, typeof receiver, typeof fun);
  }

  if (IS_UNDEFINED(new.target)) {
    return new CallSite(receiver, fun, pos, strict_mode);
  }

  if (IS_FUNCTION(fun)) {
    SET_PRIVATE(this, callSiteReceiverSymbol, receiver);
    SET_PRIVATE(this, callSiteFunctionSymbol, fun);
  } else {
    SET_PRIVATE(this, callSiteWasmObjectSymbol, receiver);
    SET_PRIVATE(this, callSiteWasmFunctionIndexSymbol, TO_UINT32(fun));
  }
  SET_PRIVATE(this, callSitePositionSymbol, TO_INT32(pos));
  SET_PRIVATE(this, callSiteStrictSymbol, TO_BOOLEAN(strict_mode));
}

function CheckCallSite(obj, name) {
  if (!IS_RECEIVER(obj) || !HAS_PRIVATE(obj, callSitePositionSymbol)) {
    throw MakeTypeError(kCallSiteMethod, name);
  }
}

function CallSiteGetThis() {
  CheckCallSite(this, "getThis");
  return GET_PRIVATE(this, callSiteStrictSymbol)
      ? UNDEFINED : GET_PRIVATE(this, callSiteReceiverSymbol);
}

function CallSiteGetFunction() {
  CheckCallSite(this, "getFunction");
  return GET_PRIVATE(this, callSiteStrictSymbol)
      ? UNDEFINED : GET_PRIVATE(this, callSiteFunctionSymbol);
}

function CallSiteGetPosition() {
  CheckCallSite(this, "getPosition");
  return GET_PRIVATE(this, callSitePositionSymbol);
}

function CallSiteGetTypeName() {
  CheckCallSite(this, "getTypeName");
  return GetTypeName(GET_PRIVATE(this, callSiteReceiverSymbol), false);
}

function CallSiteIsToplevel() {
  CheckCallSite(this, "isTopLevel");
  return %CallSiteIsToplevelRT(this);
}

function CallSiteIsEval() {
  CheckCallSite(this, "isEval");
  return %CallSiteIsEvalRT(this);
}

function CallSiteGetEvalOrigin() {
  CheckCallSite(this, "getEvalOrigin");
  var script = %FunctionGetScript(GET_PRIVATE(this, callSiteFunctionSymbol));
  return FormatEvalOrigin(script);
}

function CallSiteGetScriptNameOrSourceURL() {
  CheckCallSite(this, "getScriptNameOrSourceURL");
  return %CallSiteGetScriptNameOrSourceUrlRT(this);
}

function CallSiteGetFunctionName() {
  // See if the function knows its own name
  CheckCallSite(this, "getFunctionName");
  return %CallSiteGetFunctionNameRT(this);
}

function CallSiteGetMethodName() {
  // See if we can find a unique property on the receiver that holds
  // this function.
  CheckCallSite(this, "getMethodName");
  return %CallSiteGetMethodNameRT(this);
}

function CallSiteGetFileName() {
  CheckCallSite(this, "getFileName");
  return %CallSiteGetFileNameRT(this);
}

function CallSiteGetLineNumber() {
  if (HAS_PRIVATE(this, callSiteWasmObjectSymbol)) {
    return GET_PRIVATE(this, callSiteWasmFunctionIndexSymbol);
  }
  CheckCallSite(this, "getLineNumber");
  return %CallSiteGetLineNumberRT(this);
}

function CallSiteGetColumnNumber() {
  CheckCallSite(this, "getColumnNumber");
  return %CallSiteGetColumnNumberRT(this);
}

function CallSiteIsNative() {
  CheckCallSite(this, "isNative");
  return %CallSiteIsNativeRT(this);
}

function CallSiteIsConstructor() {
  CheckCallSite(this, "isConstructor");
  return %CallSiteIsConstructorRT(this);
}

function CallSiteToString() {
  if (HAS_PRIVATE(this, callSiteWasmObjectSymbol)) {
    var funName = this.getFunctionName();
    var funcIndex = GET_PRIVATE(this, callSiteWasmFunctionIndexSymbol);
    var pos = this.getPosition();
    if (IS_NULL(funName)) funName = "<WASM UNNAMED>";
    return funName + " (<WASM>[" + funcIndex + "]+" + pos + ")";
  }

  var fileName;
  var fileLocation = "";
  if (this.isNative()) {
    fileLocation = "native";
  } else {
    fileName = this.getScriptNameOrSourceURL();
    if (!fileName && this.isEval()) {
      fileLocation = this.getEvalOrigin();
      fileLocation += ", ";  // Expecting source position to follow.
    }

    if (fileName) {
      fileLocation += fileName;
    } else {
      // Source code does not originate from a file and is not native, but we
      // can still get the source position inside the source string, e.g. in
      // an eval string.
      fileLocation += "<anonymous>";
    }
    var lineNumber = this.getLineNumber();
    if (lineNumber != null) {
      fileLocation += ":" + lineNumber;
      var columnNumber = this.getColumnNumber();
      if (columnNumber) {
        fileLocation += ":" + columnNumber;
      }
    }
  }

  var line = "";
  var functionName = this.getFunctionName();
  var addSuffix = true;
  var isConstructor = this.isConstructor();
  var isMethodCall = !(this.isToplevel() || isConstructor);
  if (isMethodCall) {
    var typeName = GetTypeName(GET_PRIVATE(this, callSiteReceiverSymbol), true);
    var methodName = this.getMethodName();
    if (functionName) {
      if (typeName && %_Call(StringIndexOf, functionName, typeName) != 0) {
        line += typeName + ".";
      }
      line += functionName;
      if (methodName &&
          (%_Call(StringIndexOf, functionName, "." + methodName) !=
           functionName.length - methodName.length - 1)) {
        line += " [as " + methodName + "]";
      }
    } else {
      line += typeName + "." + (methodName || "<anonymous>");
    }
  } else if (isConstructor) {
    line += "new " + (functionName || "<anonymous>");
  } else if (functionName) {
    line += functionName;
  } else {
    line += fileLocation;
    addSuffix = false;
  }
  if (addSuffix) {
    line += " (" + fileLocation + ")";
  }
  return line;
}

utils.SetUpLockedPrototype(CallSite, ["receiver", "fun", "pos"], [
  "getThis", CallSiteGetThis,
  "getTypeName", CallSiteGetTypeName,
  "isToplevel", CallSiteIsToplevel,
  "isEval", CallSiteIsEval,
  "getEvalOrigin", CallSiteGetEvalOrigin,
  "getScriptNameOrSourceURL", CallSiteGetScriptNameOrSourceURL,
  "getFunction", CallSiteGetFunction,
  "getFunctionName", CallSiteGetFunctionName,
  "getMethodName", CallSiteGetMethodName,
  "getFileName", CallSiteGetFileName,
  "getLineNumber", CallSiteGetLineNumber,
  "getColumnNumber", CallSiteGetColumnNumber,
  "isNative", CallSiteIsNative,
  "getPosition", CallSiteGetPosition,
  "isConstructor", CallSiteIsConstructor,
  "toString", CallSiteToString
]);


function FormatEvalOrigin(script) {
  var sourceURL = script.nameOrSourceURL();
  if (sourceURL) {
    return sourceURL;
  }

  var eval_origin = "eval at ";
  if (script.eval_from_function_name) {
    eval_origin += script.eval_from_function_name;
  } else {
    eval_origin +=  "<anonymous>";
  }

  var eval_from_script = script.eval_from_script;
  if (eval_from_script) {
    if (eval_from_script.compilation_type == COMPILATION_TYPE_EVAL) {
      // eval script originated from another eval.
      eval_origin += " (" + FormatEvalOrigin(eval_from_script) + ")";
    } else {
      // eval script originated from "real" source.
      if (eval_from_script.name) {
        eval_origin += " (" + eval_from_script.name;
        var location = eval_from_script.locationFromPosition(
            script.eval_from_script_position, true);
        if (location) {
          eval_origin += ":" + (location.line + 1);
          eval_origin += ":" + (location.column + 1);
        }
        eval_origin += ")";
      } else {
        eval_origin += " (unknown source)";
      }
    }
  }

  return eval_origin;
}


function FormatErrorString(error) {
  try {
    return %_Call(ErrorToString, error);
  } catch (e) {
    try {
      return "<error: " + e + ">";
    } catch (ee) {
      return "<error>";
    }
  }
}


function GetStackFrames(raw_stack) {
  var internal_raw_stack = new InternalArray();
  %MoveArrayContents(raw_stack, internal_raw_stack);
  var frames = new InternalArray();
  var sloppy_frames = internal_raw_stack[0];
  for (var i = 1; i < internal_raw_stack.length; i += 4) {
    var recv = internal_raw_stack[i];
    var fun = internal_raw_stack[i + 1];
    var code = internal_raw_stack[i + 2];
    var pc = internal_raw_stack[i + 3];
    // For traps in wasm, the bytecode offset is passed as (-1 - offset).
    // Otherwise, lookup the position from the pc.
    var pos = IS_NUMBER(fun) && pc < 0 ? (-1 - pc) :
      %FunctionGetPositionForOffset(code, pc);
    sloppy_frames--;
    frames.push(new CallSite(recv, fun, pos, (sloppy_frames < 0)));
  }
  return frames;
}


// Flag to prevent recursive call of Error.prepareStackTrace.
var formatting_custom_stack_trace = false;


function FormatStackTrace(obj, raw_stack) {
  var frames = GetStackFrames(raw_stack);
  if (IS_FUNCTION(GlobalError.prepareStackTrace) &&
      !formatting_custom_stack_trace) {
    var array = [];
    %MoveArrayContents(frames, array);
    formatting_custom_stack_trace = true;
    var stack_trace = UNDEFINED;
    try {
      stack_trace = GlobalError.prepareStackTrace(obj, array);
    } catch (e) {
      throw e;  // The custom formatting function threw.  Rethrow.
    } finally {
      formatting_custom_stack_trace = false;
    }
    return stack_trace;
  }

  var lines = new InternalArray();
  lines.push(FormatErrorString(obj));
  for (var i = 0; i < frames.length; i++) {
    var frame = frames[i];
    var line;
    try {
      line = frame.toString();
    } catch (e) {
      try {
        line = "<error: " + e + ">";
      } catch (ee) {
        // Any code that reaches this point is seriously nasty!
        line = "<error>";
      }
    }
    lines.push("    at " + line);
  }
  return %_Call(ArrayJoin, lines, "\n");
}


function GetTypeName(receiver, requireConstructor) {
  if (IS_NULL_OR_UNDEFINED(receiver)) return null;
  if (IS_PROXY(receiver)) return "Proxy";

  var constructor = %GetDataProperty(TO_OBJECT(receiver), "constructor");
  if (!IS_FUNCTION(constructor)) {
    return requireConstructor ? null : %_Call(NoSideEffectsToString, receiver);
  }
  return %FunctionGetName(constructor);
}


// Format the stack trace if not yet done, and return it.
// Cache the formatted stack trace on the holder.
var StackTraceGetter = function() {
  var formatted_stack_trace = UNDEFINED;
  var holder = this;
  while (holder) {
    var formatted_stack_trace =
      GET_PRIVATE(holder, formattedStackTraceSymbol);
    if (IS_UNDEFINED(formatted_stack_trace)) {
      // No formatted stack trace available.
      var stack_trace = GET_PRIVATE(holder, stackTraceSymbol);
      if (IS_UNDEFINED(stack_trace)) {
        // Neither formatted nor structured stack trace available.
        // Look further up the prototype chain.
        holder = %object_get_prototype_of(holder);
        continue;
      }
      formatted_stack_trace = FormatStackTrace(holder, stack_trace);
      SET_PRIVATE(holder, stackTraceSymbol, UNDEFINED);
      SET_PRIVATE(holder, formattedStackTraceSymbol, formatted_stack_trace);
    }
    return formatted_stack_trace;
  }
  return UNDEFINED;
};


// If the receiver equals the holder, set the formatted stack trace that the
// getter returns.
var StackTraceSetter = function(v) {
  if (IsErrorObject(this)) {
    SET_PRIVATE(this, stackTraceSymbol, UNDEFINED);
    SET_PRIVATE(this, formattedStackTraceSymbol, v);
  }
};


// Use a dummy function since we do not actually want to capture a stack trace
// when constructing the initial Error prototytpes.
var captureStackTrace = function() {};


// Set up special error type constructors.
function SetUpError(error_function) {
  %FunctionSetInstanceClassName(error_function, 'Error');
  var name = error_function.name;
  var prototype = new GlobalObject();
  if (name !== 'Error') {
    %InternalSetPrototype(error_function, GlobalError);
    %InternalSetPrototype(prototype, GlobalError.prototype);
  }
  %FunctionSetPrototype(error_function, prototype);

  %AddNamedProperty(error_function.prototype, 'name', name, DONT_ENUM);
  %AddNamedProperty(error_function.prototype, 'message', '', DONT_ENUM);
  %AddNamedProperty(
      error_function.prototype, 'constructor', error_function, DONT_ENUM);

  %SetCode(error_function, function(m) {
    if (IS_UNDEFINED(new.target)) return new error_function(m);

    try { captureStackTrace(this, error_function); } catch (e) { }
    // Define all the expected properties directly on the error
    // object. This avoids going through getters and setters defined
    // on prototype objects.
    if (!IS_UNDEFINED(m)) {
      %AddNamedProperty(this, 'message', TO_STRING(m), DONT_ENUM);
    }
  });

  %SetNativeFlag(error_function);
  return error_function;
};

GlobalError = SetUpError(global.Error);
GlobalEvalError = SetUpError(global.EvalError);
GlobalRangeError = SetUpError(global.RangeError);
GlobalReferenceError = SetUpError(global.ReferenceError);
GlobalSyntaxError = SetUpError(global.SyntaxError);
GlobalTypeError = SetUpError(global.TypeError);
GlobalURIError = SetUpError(global.URIError);

utils.InstallFunctions(GlobalError.prototype, DONT_ENUM,
                       ['toString', ErrorToString]);

function ErrorToString() {
  if (!IS_RECEIVER(this)) {
    throw MakeTypeError(kCalledOnNonObject, "Error.prototype.toString");
  }

  var name = this.name;
  name = IS_UNDEFINED(name) ? "Error" : TO_STRING(name);

  var message = this.message;
  message = IS_UNDEFINED(message) ? "" : TO_STRING(message);

  if (name == "") return message;
  if (message == "") return name;
  return `${name}: ${message}`
}

function MakeError(type, arg0, arg1, arg2) {
  return MakeGenericError(GlobalError, type, arg0, arg1, arg2);
}

function MakeRangeError(type, arg0, arg1, arg2) {
  return MakeGenericError(GlobalRangeError, type, arg0, arg1, arg2);
}

function MakeSyntaxError(type, arg0, arg1, arg2) {
  return MakeGenericError(GlobalSyntaxError, type, arg0, arg1, arg2);
}

function MakeTypeError(type, arg0, arg1, arg2) {
  return MakeGenericError(GlobalTypeError, type, arg0, arg1, arg2);
}

function MakeURIError() {
  return MakeGenericError(GlobalURIError, kURIMalformed);
}

// Boilerplate for exceptions for stack overflows. Used from
// Isolate::StackOverflow().
var StackOverflowBoilerplate = MakeRangeError(kStackOverflow);
utils.InstallGetterSetter(StackOverflowBoilerplate, 'stack',
                          StackTraceGetter, StackTraceSetter)

// Define actual captureStackTrace function after everything has been set up.
captureStackTrace = function captureStackTrace(obj, cons_opt) {
  // Define accessors first, as this may fail and throw.
  %object_define_property(obj, 'stack', { get: StackTraceGetter,
                                          set: StackTraceSetter,
                                          configurable: true });
  %CollectStackTrace(obj, cons_opt ? cons_opt : captureStackTrace);
};

GlobalError.captureStackTrace = captureStackTrace;

%InstallToContext([
  "get_stack_trace_line_fun", GetStackTraceLine,
  "make_error_function", MakeGenericError,
  "make_range_error", MakeRangeError,
  "make_type_error", MakeTypeError,
  "message_get_column_number", GetColumnNumber,
  "message_get_line_number", GetLineNumber,
  "message_get_source_line", GetSourceLine,
  "no_side_effects_to_string_fun", NoSideEffectsToString,
  "stack_overflow_boilerplate", StackOverflowBoilerplate,
]);

utils.Export(function(to) {
  to.ErrorToString = ErrorToString;
  to.MakeError = MakeError;
  to.MakeRangeError = MakeRangeError;
  to.MakeSyntaxError = MakeSyntaxError;
  to.MakeTypeError = MakeTypeError;
  to.MakeURIError = MakeURIError;
});

});
