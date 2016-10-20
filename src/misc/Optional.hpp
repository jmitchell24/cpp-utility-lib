// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include <cstdint>
#include <cstddef>

namespace util
{
    template <typename T> class Optional
    {
        static_assert(sizeof(uint8_t) == 1, "uint8_t is not defined as 1 byte");

        typedef uint8_t Byte;
        typedef Byte Data[sizeof(T)];

        bool flag;
        Data data;

        inline Optional(bool const flag)
            : flag(flag)
        {}

    public:
        inline Optional()
            : flag(false)
        {}

        inline Optional(T const& t)
            : flag(true)
        {
            reinterpret_cast<T&>(data) = t;
        }

        inline Optional(Optional const&)=default;

        template <typename... Params> static Optional<T> make(Params&&... params)
        {
            Optional<T> o(true);
            new (o.data) T(std::forward<Params>(params)...);
            return o;
        }

        inline bool is() const { return flag; }

        inline operator bool() const { return is(); }

        inline Optional<T>& operator = (T const& t)
        {
            flag = true;
            reinterpret_cast<T&>(data) = t;
            return *this;
        }

        inline Optional<T>& operator = (Optional<T> const&)=default;

        inline T      & operator* ()       { return reinterpret_cast<T&>      (data); }
        inline T const& operator* () const { return reinterpret_cast<T const&>(data); }

        inline T      * operator-> ()       { return reinterpret_cast<T*>(data); }
        inline T const* operator-> () const { return reinterpret_cast<T*>(data); }
    };

    template <typename T, typename... Params> Optional<T> optional(Params&&... params)
    {
        return Optional<T>::make(std::forward<Params>(params)...);
    }
}
