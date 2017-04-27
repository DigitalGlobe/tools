// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_BIND_INTERNAL_H_
#define BASE_BIND_INTERNAL_H_

#include <stddef.h>

#include <tuple>
#include <type_traits>

#include "base/bind_helpers.h"
#include "base/callback_internal.h"
#include "base/memory/raw_scoped_refptr_mismatch_checker.h"
#include "base/memory/weak_ptr.h"
#include "base/template_util.h"
#include "base/tuple.h"
#include "build/build_config.h"

#if defined(OS_WIN)
#include "base/bind_internal_win.h"
#endif

namespace base {
namespace internal {

// See base/callback.h for user documentation.
//
//
// CONCEPTS:
//  Runnable -- A type (really a type class) that has a single Run() method
//              and a RunType typedef that corresponds to the type of Run().
//              A Runnable can declare that it should treated like a method
//              call by including a typedef named IsMethod.  The value of
//              this typedef is NOT inspected, only the existence.  When a
//              Runnable declares itself a method, Bind() will enforce special
//              refcounting + WeakPtr handling semantics for the first
//              parameter which is expected to be an object.
//  Functor -- A copyable type representing something that should be called.
//             All function pointers, Callback<>, and Runnables are functors
//             even if the invocation syntax differs.
//  RunType -- A function type (as opposed to function _pointer_ type) for
//             a Run() function.  Usually just a convenience typedef.
//  (Bound)Args -- A set of types that stores the arguments.
//
// Types:
//  RunnableAdapter<> -- Wraps the various "function" pointer types into an
//                       object that adheres to the Runnable interface.
//  ForceVoidReturn<> -- Helper class for translating function signatures to
//                       equivalent forms with a "void" return type.
//  FunctorTraits<> -- Type traits used determine the correct RunType and
//                     RunnableType for a Functor.  This is where function
//                     signature adapters are applied.
//  MakeRunnable<> -- Takes a Functor and returns an object in the Runnable
//                    type class that represents the underlying Functor.
//  InvokeHelper<> -- Take a Runnable + arguments and actully invokes it.
//                    Handle the differing syntaxes needed for WeakPtr<>
//                    support, and for ignoring return values.  This is separate
//                    from Invoker to avoid creating multiple version of
//                    Invoker<>.
//  Invoker<> -- Unwraps the curried parameters and executes the Runnable.
//  BindState<> -- Stores the curried parameters, and is the main entry point
//                 into the Bind() system, doing most of the type resolution.
//                 There are ARITY BindState types.

// HasRefCountedTypeAsRawPtr selects true_type when any of the |Args| is a raw
// pointer to a RefCounted type.
// Implementation note: This non-specialized case handles zero-arity case only.
// Non-zero-arity cases should be handled by the specialization below.
template <typename... Args>
struct HasRefCountedTypeAsRawPtr : std::false_type {};

// Implementation note: Select true_type if the first parameter is a raw pointer
// to a RefCounted type. Otherwise, skip the first parameter and check rest of
// parameters recursively.
template <typename T, typename... Args>
struct HasRefCountedTypeAsRawPtr<T, Args...>
    : std::conditional<NeedsScopedRefptrButGetsRawPtr<T>::value,
                       std::true_type,
                       HasRefCountedTypeAsRawPtr<Args...>>::type {};

// BindsArrayToFirstArg selects true_type when |is_method| is true and the first
// item of |Args| is an array type.
// Implementation note: This non-specialized case handles !is_method case and
// zero-arity case only.  Other cases should be handled by the specialization
// below.
template <bool is_method, typename... Args>
struct BindsArrayToFirstArg : std::false_type {};

template <typename T, typename... Args>
struct BindsArrayToFirstArg<true, T, Args...>
    : std::is_array<typename std::remove_reference<T>::type> {};

// HasRefCountedParamAsRawPtr is the same to HasRefCountedTypeAsRawPtr except
// when |is_method| is true HasRefCountedParamAsRawPtr skips the first argument.
// Implementation note: This non-specialized case handles !is_method case and
// zero-arity case only.  Other cases should be handled by the specialization
// below.
template <bool is_method, typename... Args>
struct HasRefCountedParamAsRawPtr : HasRefCountedTypeAsRawPtr<Args...> {};

template <typename T, typename... Args>
struct HasRefCountedParamAsRawPtr<true, T, Args...>
    : HasRefCountedTypeAsRawPtr<Args...> {};

// RunnableAdapter<>
//
// The RunnableAdapter<> templates provide a uniform interface for invoking
// a function pointer, method pointer, or const method pointer. The adapter
// exposes a Run() method with an appropriate signature. Using this wrapper
// allows for writing code that supports all three pointer types without
// undue repetition.  Without it, a lot of code would need to be repeated 3
// times.
//
// For method pointers and const method pointers the first argument to Run()
// is considered to be the received of the method.  This is similar to STL's
// mem_fun().
//
// This class also exposes a RunType typedef that is the function type of the
// Run() function.
//
// If and only if the wrapper contains a method or const method pointer, an
// IsMethod typedef is exposed.  The existence of this typedef (NOT the value)
// marks that the wrapper should be considered a method wrapper.

template <typename Functor>
class RunnableAdapter;

// Function.
template <typename R, typename... Args>
class RunnableAdapter<R(*)(Args...)> {
 public:
  // MSVC 2013 doesn't support Type Alias of function types.
  // Revisit this after we update it to newer version.
  typedef R RunType(Args...);

  explicit RunnableAdapter(R(*function)(Args...))
      : function_(function) {
  }

  template <typename... RunArgs>
  R Run(RunArgs&&... args) const {
    return function_(std::forward<RunArgs>(args)...);
  }

 private:
  R (*function_)(Args...);
};

// Method.
template <typename R, typename T, typename... Args>
class RunnableAdapter<R(T::*)(Args...)> {
 public:
  // MSVC 2013 doesn't support Type Alias of function types.
  // Revisit this after we update it to newer version.
  typedef R RunType(T*, Args...);
  using IsMethod = std::true_type;

  explicit RunnableAdapter(R(T::*method)(Args...))
      : method_(method) {
  }

  template <typename Receiver, typename... RunArgs>
  R Run(Receiver&& receiver_ptr, RunArgs&&... args) const {
    // Clang skips CV qualifier check on a method pointer invocation when the
    // receiver is a subclass. Store the receiver into a const reference to
    // T to ensure the CV check works.
    // https://llvm.org/bugs/show_bug.cgi?id=27037
    T& receiver = *receiver_ptr;
    return (receiver.*method_)(std::forward<RunArgs>(args)...);
  }

 private:
  R (T::*method_)(Args...);
};

// Const Method.
template <typename R, typename T, typename... Args>
class RunnableAdapter<R(T::*)(Args...) const> {
 public:
  using RunType = R(const T*, Args...);
  using IsMethod = std::true_type;

  explicit RunnableAdapter(R(T::*method)(Args...) const)
      : method_(method) {
  }

  template <typename Receiver, typename... RunArgs>
  R Run(Receiver&& receiver_ptr, RunArgs&&... args) const {
    // Clang skips CV qualifier check on a method pointer invocation when the
    // receiver is a subclass. Store the receiver into a unqualified reference
    // to T to ensure the CV check works.
    // https://llvm.org/bugs/show_bug.cgi?id=27037
    const T& receiver = *receiver_ptr;
    return (receiver.*method_)(std::forward<RunArgs>(args)...);
  }

 private:
  R (T::*method_)(Args...) const;
};


// ForceVoidReturn<>
//
// Set of templates that support forcing the function return type to void.
template <typename Sig>
struct ForceVoidReturn;

template <typename R, typename... Args>
struct ForceVoidReturn<R(Args...)> {
  // MSVC 2013 doesn't support Type Alias of function types.
  // Revisit this after we update it to newer version.
  typedef void RunType(Args...);
};


// FunctorTraits<>
//
// See description at top of file.
template <typename T>
struct FunctorTraits {
  using RunnableType = RunnableAdapter<T>;
  using RunType = typename RunnableType::RunType;
};

template <typename T>
struct FunctorTraits<IgnoreResultHelper<T>> {
  using RunnableType = typename FunctorTraits<T>::RunnableType;
  using RunType =
      typename ForceVoidReturn<typename RunnableType::RunType>::RunType;
};

template <typename T>
struct FunctorTraits<Callback<T>> {
  using RunnableType = Callback<T> ;
  using RunType = typename Callback<T>::RunType;
};


// MakeRunnable<>
//
// Converts a passed in functor to a RunnableType using type inference.

template <typename T>
typename FunctorTraits<T>::RunnableType MakeRunnable(const T& t) {
  return RunnableAdapter<T>(t);
}

template <typename T>
typename FunctorTraits<T>::RunnableType
MakeRunnable(const IgnoreResultHelper<T>& t) {
  return MakeRunnable(t.functor_);
}

template <typename T>
const typename FunctorTraits<Callback<T>>::RunnableType&
MakeRunnable(const Callback<T>& t) {
  DCHECK(!t.is_null());
  return t;
}


// InvokeHelper<>
//
// There are 3 logical InvokeHelper<> specializations: normal, void-return,
// WeakCalls.
//
// The normal type just calls the underlying runnable.
//
// We need a InvokeHelper to handle void return types in order to support
// IgnoreResult().  Normally, if the Runnable's RunType had a void return,
// the template system would just accept "return functor.Run()" ignoring
// the fact that a void function is being used with return. This piece of
// sugar breaks though when the Runnable's RunType is not void.  Thus, we
// need a partial specialization to change the syntax to drop the "return"
// from the invocation call.
//
// WeakCalls similarly need special syntax that is applied to the first
// argument to check if they should no-op themselves.
template <bool is_weak_call, typename ReturnType>
struct InvokeHelper;

template <typename ReturnType>
struct InvokeHelper<false, ReturnType> {
  template <typename Runnable, typename... RunArgs>
  static inline ReturnType MakeItSo(Runnable&& runnable, RunArgs&&... args) {
    return std::forward<Runnable>(runnable).Run(std::forward<RunArgs>(args)...);
  }
};

template <>
struct InvokeHelper<false, void> {
  template <typename Runnable, typename... RunArgs>
  static inline void MakeItSo(Runnable&& runnable, RunArgs&&... args) {
    std::forward<Runnable>(runnable).Run(std::forward<RunArgs>(args)...);
  }
};

template <>
struct InvokeHelper<true, void> {
  template <typename Runnable, typename BoundWeakPtr, typename... RunArgs>
  static void MakeItSo(Runnable&& runnable,
                       BoundWeakPtr&& weak_ptr,
                       RunArgs&&... args) {
    if (!weak_ptr) {
      return;
    }
    std::forward<Runnable>(runnable).Run(
        std::forward<BoundWeakPtr>(weak_ptr), std::forward<RunArgs>(args)...);
  }
};

#if !defined(_MSC_VER)

template <typename ReturnType>
struct InvokeHelper<true, ReturnType> {
  // WeakCalls are only supported for functions with a void return type.
  // Otherwise, the function result would be undefined if the the WeakPtr<>
  // is invalidated.
  static_assert(std::is_void<ReturnType>::value,
                "weak_ptrs can only bind to methods without return values");
};

#endif

// Invoker<>
//
// See description at the top of the file.
template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R Run(BindStateBase* base, UnboundArgs&&... unbound_args) {
    // Local references to make debugger stepping easier. If in a debugger,
    // you really want to warp ahead and step through the
    // InvokeHelper<>::MakeItSo() call below.
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args =
        std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(storage->runnable_,
                   storage->bound_args_,
                   MakeIndexSequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

  template <typename Runnable, typename BoundArgsTuple, size_t... indices>
  static inline R RunImpl(Runnable&& runnable,
                          BoundArgsTuple&& bound,
                          IndexSequence<indices...>,
                          UnboundArgs&&... unbound_args) {
    static constexpr bool is_method =
        HasIsMethodTag<typename std::decay<Runnable>::type>::value;

    using DecayedArgsTuple = typename std::decay<BoundArgsTuple>::type;
    static constexpr bool is_weak_call =
        IsWeakMethod<is_method,
                     typename std::tuple_element<
                         indices,
                         DecayedArgsTuple>::type...>::value;

    return InvokeHelper<is_weak_call, R>::MakeItSo(
        std::forward<Runnable>(runnable),
        Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unbound_args)...);
  }
};

// Used to implement MakeArgsStorage.
template <bool is_method, typename... BoundArgs>
struct MakeArgsStorageImpl {
  using Type = std::tuple<BoundArgs...>;
};

template <typename Obj, typename... BoundArgs>
struct MakeArgsStorageImpl<true, Obj*, BoundArgs...> {
  using Type = std::tuple<scoped_refptr<Obj>, BoundArgs...>;
};

// Constructs a tuple type to store BoundArgs into BindState.
// This wraps the first argument into a scoped_refptr if |is_method| is true and
// the first argument is a raw pointer.
// Other arguments are adjusted for store and packed into a tuple.
template <bool is_method, typename... BoundArgs>
using MakeArgsStorage = typename MakeArgsStorageImpl<
  is_method, typename std::decay<BoundArgs>::type...>::Type;

// Used to implement MakeUnboundRunType.
template <typename Functor, typename... BoundArgs>
struct MakeUnboundRunTypeImpl {
  using RunType = typename FunctorTraits<Functor>::RunType;
  using ReturnType = ExtractReturnType<RunType>;
  using Args = ExtractArgs<RunType>;
  using UnboundArgs = DropTypeListItem<sizeof...(BoundArgs), Args>;
  using Type = MakeFunctionType<ReturnType, UnboundArgs>;
};

// BindState<>
//
// This stores all the state passed into Bind() and is also where most
// of the template resolution magic occurs.
//
// Runnable is the functor we are binding arguments to.
//
// BoundArgs contains the storage type for all the bound arguments.
template <typename Runnable, typename... BoundArgs>
struct BindState final : public BindStateBase {
 private:
  using RunnableType = typename std::decay<Runnable>::type;

  static constexpr bool is_method = HasIsMethodTag<RunnableType>::value;

 public:
  template <typename... ForwardArgs>
  explicit BindState(RunnableType runnable, ForwardArgs&&... bound_args)
      : BindStateBase(&Destroy),
        runnable_(std::move(runnable)),
        bound_args_(std::forward<ForwardArgs>(bound_args)...) {}

  RunnableType runnable_;
  MakeArgsStorage<is_method, BoundArgs...> bound_args_;

 private:
  ~BindState() {}

  static void Destroy(BindStateBase* self) {
    delete static_cast<BindState*>(self);
  }
};

}  // namespace internal

// Returns a RunType of bound functor.
// E.g. MakeUnboundRunType<R(A, B, C), A, B> is evaluated to R(C).
template <typename Functor, typename... BoundArgs>
using MakeUnboundRunType =
    typename internal::MakeUnboundRunTypeImpl<Functor, BoundArgs...>::Type;

}  // namespace base

#endif  // BASE_BIND_INTERNAL_H_
