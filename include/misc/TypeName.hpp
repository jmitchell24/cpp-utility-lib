// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <type_traits>
#include <string>
#include <memory>
#include <map>

#ifndef _MSC_VER
#include <cxxabi.h>
#endif

#define exp_type_name(exp) type_name<decltype(exp)>()

template <class T> std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;

    static std::map<unsigned, std::string> tnames;

    unsigned const tvalue = typeid(TR).hash_code();
    if (tnames.find(tvalue) != tnames.end())
        return tnames[tvalue];

    std::unique_ptr<char, void(*)(void*)> const own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
         if (std::is_const<TR>::value)           r += " const";
         if (std::is_volatile<TR>::value)        r += " volatile";
         if (std::is_lvalue_reference<T>::value) r += "&";
    else if (std::is_rvalue_reference<T>::value) r += "&&";
    return (tnames[tvalue] = r);
}

#define DEFINE_TYPENAME(x)    \
template<> struct TypeName<x> \
{ constexpr static char const* value() { return #x; } };

template <typename T> struct TypeName
{ constexpr static char const* value() { return "unknown"; } };

DEFINE_TYPENAME(char)
DEFINE_TYPENAME(unsigned char)
DEFINE_TYPENAME(short)
DEFINE_TYPENAME(unsigned short)
DEFINE_TYPENAME(int)
DEFINE_TYPENAME(unsigned int)
DEFINE_TYPENAME(long)
DEFINE_TYPENAME(unsigned long)
DEFINE_TYPENAME(float)
DEFINE_TYPENAME(double)
