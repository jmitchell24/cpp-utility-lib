// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include <cmath>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace util
{
namespace varargs
{
    template<size_t... Is> struct integer_sequence {};

    namespace detail
    {
        template<size_t N, size_t... Is>
        struct make_integer_sequence_h
            : make_integer_sequence_h<N-1, N-1, Is...>
        {};

        template<size_t... Is>
        struct make_integer_sequence_h<0, Is...>
        {
            using type = integer_sequence<Is...>;
        };
    }

    template<size_t N> using make_integer_sequence = typename util::varargs::detail::make_integer_sequence_h<N>::type;

    namespace detail
    {
        template<typename... Params> struct TupleHelper
        {
            template<class IntegerSequence> struct InnerTupleHelper;
            template<size_t... Offsets> struct InnerTupleHelper<integer_sequence<Offsets...>> // deduce the integers
            {
                typedef std::tuple<Params...> Tuple;

                template <typename Predicate>
                inline static auto call(Tuple& tuple, Predicate const& predicate)
                -> decltype(predicate(std::get<Offsets>(tuple)...))
                { return predicate(std::get<Offsets>(tuple)...); }

                template <typename Predicate>
                inline static auto call(Tuple const& tuple, Predicate const& predicate)
                -> decltype(predicate(std::get<Offsets>(tuple)...))
                { return predicate(std::get<Offsets>(tuple)...); }

                inline static void set(Tuple const& tuple, Params&... params)
                { setImpl<0>(tuple,params...); }

            private:
                template <size_t I, typename Argument>
                inline static void setImpl(Tuple const& tuple, Argument& argument)
                { argument = std::get<I>(tuple); }

                template <size_t I, typename Argument, typename... Arguments>
                inline static void setImpl(Tuple const& tuple, Argument& argument, Arguments&... arguments)
                {
                    argument = std::get<I>(tuple);
                    setImpl<I+1>(tuple,arguments...);
                }
            };
        };
    }

    template <typename... Params>
    using TupleHelper = typename varargs::detail::TupleHelper<Params...>::template InnerTupleHelper<make_integer_sequence<sizeof...(Params)>>;


    template <size_t I, size_t Param, size_t... Params> struct PackValueIndex
    { std::size_t static constexpr value = PackValueIndex<I-1,Params...>::value; };

    template <size_t Param, size_t... Params> struct PackValueIndex<0,Param,Params...>
    { std::size_t static constexpr value = Param; };

    template <size_t I, size_t... Params> struct PackValueByIndex
    { std::size_t static constexpr value = PackValueIndex<I, Params...>::value; };

    template <size_t I> struct PackValueByIndex<I>
    { std::size_t static constexpr value = I; };

    template <size_t I, typename Components, size_t... Offsets> struct Fill
    {
        inline static void fillVec(Components& a, Components const& b)
        {
            Fill<I-1,Components,Offsets...>::fillVec(a,b);
            a[I] = b[PackValueByIndex<I,Offsets...>::value];
        }

        inline static void fillGen(Components& a, Components const& b)
        {
            Fill<I-1,Components,Offsets...>::fillGen(a,b);
            a[PackValueByIndex<I,Offsets...>::value] = b[I];
        }
    };

    template <typename Components, size_t... Offsets> struct Fill<0,Components,Offsets...>
    {
        inline static void fillVec(Components& a, Components const& b)
        { a[0] = b[PackValueByIndex<0,Offsets...>::value]; }

        inline static void fillGen(Components& a, Components const& b)
        { a[PackValueByIndex<0,Offsets...>::value] = b[0]; }
    };

    template<typename Param> inline bool logAnd(Param param)
    { return (bool)param; }

    template<typename Param, typename... Params> inline bool logAnd(Param param, Params... params)
    { return (bool)param && logAnd(params...); }

    template<typename Param> inline bool isInf(Param param)
    { return std::isinf(param); }

    template<typename Param, typename... Params> inline bool isInf(Param param, Params... params)
    { return std::isinf(param) || isInf(params...); }

    template<typename Param> inline bool isNan(Param param)
    { return std::isnan(param); }

    template <typename Param, typename... Params> inline bool isNan(Param param, Params... params)
    { return std::isnan(param) || isNan(params...); }

    template<typename Param> inline Param sum(Param param)
    { return param; }

    template <typename Param, typename... Params> inline auto sum(Param param, Params... params) -> typename std::common_type<Param,Params...>::type
    { return param + sum(params...); }

    template<typename Param> inline Param squareSum(Param param)
    { return (param*param); }

    template <typename Param, typename... Params> inline auto squareSum(Param param, Params... params) -> typename std::common_type<Param,Params...>::type
    { return (param*param) + squareSum(params...); }

    template<typename A, typename B> inline auto max(A a, B b) -> typename std::common_type<A,B>::type
    { return (a < b) ? b : a; }

    template<typename A, typename B, typename... Params> inline auto max(A a, B b, Params... params) -> typename std::common_type<A,B,Params...>::type
    { return (a < b) ? max(b, params...) : max(a, params...); }

    template<typename A, typename B> inline auto min(A a, B b) -> typename std::common_type<A,B>::type
    { return (a > b) ? b : a; }

    template<typename A, typename B, typename... Params> inline auto min(A a, B b, Params... params) -> typename std::common_type<A,B,Params...>::type
    { return (a > b) ? min(b, params...) : min(a, params...); }

    template<typename A, typename B> inline auto same(A a, B b) -> typename std::common_type<A,B>::type
    { return a == b; }

    template<typename A, typename B, typename... Params> inline auto same(A a, B b, Params... params) -> typename std::common_type<A,B,Params...>::type
    { return (a == b) ? same(b, params...) : false; }
}
}
