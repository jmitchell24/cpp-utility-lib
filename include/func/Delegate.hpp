// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <cstddef>
#include <cstring>
#include <utility>
#include <memory>

#include "../string/Print.hpp"

namespace util
{
namespace detail
{
    class AnyClass;
    typedef void(AnyClass::*AnyMemFunc)();
    size_t constexpr AnyMemFuncSize = sizeof(AnyMemFunc);

    template<typename A, typename B> inline A implicit_cast(B b) noexcept { return b; }
    template<typename A, typename B> inline A evil_cast(B& b) noexcept
    {
        static_assert(sizeof(B) == sizeof(A), "Cannot use evil_cast");
        return reinterpret_cast<A>(b);
    }

    template<typename XMemFunc, typename AnyMemFunc> inline static void generalize(XMemFunc const x_memfunc, AnyMemFunc& any_memfunc) noexcept
    {
        static_assert(sizeof(XMemFunc) == AnyMemFuncSize, "Unsupported member function pointer on this compiler");
        any_memfunc = reinterpret_cast<AnyMemFunc>(x_memfunc);
    }

    template<typename X, typename XMemFunc, typename AnyMemFunc> inline static void generalize(X const volatile* const x_object, XMemFunc const x_memfunc, AnyClass*& any_object, AnyMemFunc& any_memfunc) noexcept
    {
        static_assert(sizeof(XMemFunc) == AnyMemFuncSize, "Unsupported member function pointer on this compiler");
        any_object  = reinterpret_cast<AnyClass*>(const_cast<X*>(x_object));
        any_memfunc = reinterpret_cast<AnyMemFunc>(x_memfunc);
    }

    template<class TAnyMemFunc, class TStaticFunc> struct Closure
    {
        union
        {
            AnyMemFunc  function;
            TAnyMemFunc member_function;
        };

        AnyClass* object;

        inline Closure() noexcept
            : function(nullptr), object(nullptr)
        {}

        inline Closure(Closure const&) noexcept =default;
        inline Closure& operator= (Closure const&) noexcept =default;

        inline void clear() noexcept { object = nullptr; function = nullptr; }

        inline bool equal(TStaticFunc const s) const noexcept { return s == nullptr ? empty() : s == reinterpret_cast<TStaticFunc>(this); }
        inline bool equal(Closure    const& m) const noexcept { return object == m.object && function == m.function; }
        inline bool less (Closure    const& m) const noexcept { return object != m.object ? object < m.object : std::memcmp(&function, &m.function, sizeof(function)) < 0; }
        inline bool empty()                    const noexcept { return object == nullptr && function == nullptr; }

        template<class X, class XMemFunc> inline void bindMemberFunction(X* x, XMemFunc x_memfunc) noexcept
        {
            generalize(x, x_memfunc, object, function);
        }

        template<class InvokerMemFunc> inline void bindStaticFunction(InvokerMemFunc const invoker, TStaticFunc const function) noexcept
        {
            if(function == nullptr)
                this->function = nullptr;
            else
                generalize(invoker, this->function);
            object = evil_cast<AnyClass*>(function);
        }
    };
}

    template <typename Return, typename... Params> class Delegate;
    template <typename Return, typename... Params> class Delegate<Return(Params...)>
    {
        typedef Return(*FunctionT)(Params...);
        typedef Return(detail::AnyClass::*AnyMemberT)(Params...);
        typedef detail::Closure<AnyMemberT, FunctionT> ClosureT;
        typedef std::shared_ptr<void> StorageT;

        ClosureT closure;
        StorageT storage;

        template <typename X> X* setStorage(X& x)
        {
            clear();
            storage.reset(operator new(sizeof(X)), destroyStorage<X>);
            return new (storage.get()) X(x);
        }

        template<typename X> static void destroyStorage(void* p) { delete reinterpret_cast<X*>(p); }

        template <typename X> X const*          setStorage(X const& x)          { return setStorage((X&)x); }
        template <typename X> X volatile*       setStorage(X volatile& x)       { return setStorage((X&)x); }
        template <typename X> X const volatile* setStorage(X const volatile& x) { return setStorage((X&)x); }


        /* Explaination:
         * This is a very strange (and brilliant) hack concocted by Don Clugston, from which this Delegate is based.
         * The delegate needs an object and a member function pointer to call
         * To make a static function call within this paradigm, you give the delegate
         * the delegate's own member function (invokeStaticFunction) and provide the actual static function pointer
         * as the 'this' pointer
         * so invokeStaticFunction is never called with a real this pointer, but a static function pointer
         * which gets cast and then called from within
         *
         * This only works when data pointers are the same size as function pointers
         */
        Return invokeStaticFunction(Params... p) const { return (*detail::evil_cast<FunctionT>(this))(p...); }

    public:
        template <typename T>
        using MemberSignature   = Return(T::*)(Params...);
        using FunctionSignature = FunctionT;
        using ReturnT           = Return;

        template <typename... Arguments> inline Delegate            (Arguments&&... arguments) { bind(std::forward<Arguments>(arguments)...); }
        template <typename... Arguments> inline Delegate& operator= (Arguments&&... arguments) { bind(std::forward<Arguments>(arguments)...); return *this; }

        inline bool operator== (std::nullptr_t const) const { return  empty(); }
        inline bool operator!= (std::nullptr_t const) const { return !empty(); }

        inline bool operator== (Delegate const& d) const { return closure.equal(d.closure); }
        inline bool operator!= (Delegate const& d) const { return !(*this == d); }
        inline bool operator<  (Delegate const& d) const { return closure.less (d.closure); }
        inline bool operator>  (Delegate const& d) const { return d < *this; }
        inline bool operator<= (Delegate const& d) const { return !(*this > d); }
        inline bool operator>= (Delegate const& d) const { return !(*this < d); }

        inline Return operator() (Params... p) const { return (closure.object->*(closure.member_function))(p...); }

        inline operator bool() const { return !empty();     }
        inline bool empty   () const { return closure.empty(); }
        inline bool equal   () const { return closure.equal(); }
        inline bool less    () const { return closure.less();  }

        inline void clear() { closure.clear(); storage.reset(); }

        inline void bind(                       ) { clear(); }
        inline void bind(std::nullptr_t const   ) { clear(); }
        inline void bind(Delegate       const& d) { storage = d.storage; closure = d.closure; }
        inline void bind(Delegate            & d) { bind((Delegate const&)d); }
        inline void bind(Delegate           && d) { bind(std::move(d)); }

        inline void bind(Return(*function)(Params... p)) { clear(); closure.bindStaticFunction(&Delegate::invokeStaticFunction, function); }

        template <typename X, typename Y> inline void bind(Y *               y, Return(X::*member)(Params...)               ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X *>              (y), member); }
        template <typename X, typename Y> inline void bind(Y const*          y, Return(X::*member)(Params...) const         ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X const*>         (y), member); }
        template <typename X, typename Y> inline void bind(Y volatile*       y, Return(X::*member)(Params...) volatile      ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X volatile*>      (y), member); }
        template <typename X, typename Y> inline void bind(Y const volatile* y, Return(X::*member)(Params...) const volatile) { clear(); closure.bindMemberFunction(detail::implicit_cast<X const volatile*>(y), member); }
        template <typename X> inline void bind(X const* x) { bind(x, &X::operator()); }

        template <typename X, typename Y> inline typename std::enable_if<!std::is_pointer<Y>::value,void>::type bind(Y &               y, Return(X::*member)(Params...)               ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X *>              (setStorage(y)), member); }
        template <typename X, typename Y> inline typename std::enable_if<!std::is_pointer<Y>::value,void>::type bind(Y const&          y, Return(X::*member)(Params...) const         ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X const*>         (setStorage(y)), member); }
        template <typename X, typename Y> inline typename std::enable_if<!std::is_pointer<Y>::value,void>::type bind(Y volatile&       y, Return(X::*member)(Params...) volatile      ) { clear(); closure.bindMemberFunction(detail::implicit_cast<X volatile*>      (setStorage(y)), member); }
        template <typename X, typename Y> inline typename std::enable_if<!std::is_pointer<Y>::value,void>::type bind(Y const volatile& y, Return(X::*member)(Params...) const volatile) { clear(); closure.bindMemberFunction(detail::implicit_cast<X const volatile*>(setStorage(y)), member); }
        template <typename X> inline typename std::enable_if<!std::is_pointer<X>::value,void>::type bind(X &               x) { bind(x, static_cast<Return(X::*)(Params...)>               ( &X::operator() )); }
        template <typename X> inline typename std::enable_if<!std::is_pointer<X>::value,void>::type bind(X const&          x) { bind(x, static_cast<Return(X::*)(Params...) const>         ( &X::operator() )); }
        template <typename X> inline typename std::enable_if<!std::is_pointer<X>::value,void>::type bind(X volatile&       x) { bind(x, static_cast<Return(X::*)(Params...) volatile>      ( &X::operator() )); }
        template <typename X> inline typename std::enable_if<!std::is_pointer<X>::value,void>::type bind(X const volatile& x) { bind(x, static_cast<Return(X::*)(Params...) const volatile>( &X::operator() )); }
    };

    template <typename Return, typename... Params> inline Delegate<Return(Params...)> delegate(Return (*function)(Params...) ) { return Delegate<Return(Params...)>(function); }

    template <typename X, typename Y, typename Return, typename... Params> inline Delegate<Return(Params...)> delegate(Y *               y, Return (X::* member)(Params...)               ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline Delegate<Return(Params...)> delegate(Y const*          y, Return (X::* member)(Params...) const         ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline Delegate<Return(Params...)> delegate(Y volatile*       y, Return (X::* member)(Params...) volatile      ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline Delegate<Return(Params...)> delegate(Y const volatile* y, Return (X::* member)(Params...) const volatile) { return Delegate<Return(Params...)>(y, member); }
    template <typename X> inline auto delegate(X *               x) -> decltype(delegate(x, &X::operator())) { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X const*          x) -> decltype(delegate(x, &X::operator())) { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X volatile*       x) -> decltype(delegate(x, &X::operator())) { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X const volatile* x) -> decltype(delegate(x, &X::operator())) { return delegate(x, &X::operator()); }

    template <typename X, typename Y, typename Return, typename... Params> inline typename std::enable_if<!std::is_pointer<Y>::value,Delegate<Return(Params...)>>::type delegate(Y &               y, Return (X::*member)(Params...)               ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline typename std::enable_if<!std::is_pointer<Y>::value,Delegate<Return(Params...)>>::type delegate(Y const&          y, Return (X::*member)(Params...) const         ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline typename std::enable_if<!std::is_pointer<Y>::value,Delegate<Return(Params...)>>::type delegate(Y volatile&       y, Return (X::*member)(Params...) volatile      ) { return Delegate<Return(Params...)>(y, member); }
    template <typename X, typename Y, typename Return, typename... Params> inline typename std::enable_if<!std::is_pointer<Y>::value,Delegate<Return(Params...)>>::type delegate(Y const volatile& y, Return (X::*member)(Params...) const volatile) { return Delegate<Return(Params...)>(y, member); }
    template <typename X> inline auto delegate(X &               x) -> typename std::enable_if<!std::is_pointer<X>::value,decltype(delegate(x, &X::operator()))>::type { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X const&          x) -> typename std::enable_if<!std::is_pointer<X>::value,decltype(delegate(x, &X::operator()))>::type { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X volatile&       x) -> typename std::enable_if<!std::is_pointer<X>::value,decltype(delegate(x, &X::operator()))>::type { return delegate(x, &X::operator()); }
    template <typename X> inline auto delegate(X const volatile& x) -> typename std::enable_if<!std::is_pointer<X>::value,decltype(delegate(x, &X::operator()))>::type { return delegate(x, &X::operator()); }
}
