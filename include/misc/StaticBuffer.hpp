// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Integer.hpp"

namespace util
{
    template <typename T, size_t Size>
    class StaticBuffer
    {
        u8 buffer[sizeof(T)*Size];
        u8* cursor = buffer;
    public:
        typedef T*          iterator;
        typedef T const*    const_iterator;
        typedef T           value_type;
        typedef std::size_t size_type;

        inline ~StaticBuffer() noexcept
        {
            for (T* i = begin(); i != end(); ++i)
                i->~T();
        }

        inline void push_back(T const& copy)
        {
            ASSERT(cursor != buffer+sizeof(T)*Size);
            T* t = reinterpret_cast<T*>(cursor);
            cursor += sizeof(T);
            new (t) T(copy);
        }

        template <typename... Params>
        inline void emplace_back(Params&&... params)
        {
            ASSERT(cursor != buffer+sizeof(T)*Size);
            T* t = reinterpret_cast<T*>(cursor);
            cursor += sizeof(T);
            new (t) T(params...);
        }

        inline T&       front()       { ASSERT(!empty()); return buffer[0]; }
        inline T const& front() const { ASSERT(!empty()); return buffer[0]; }

        inline T&       back()       { ASSERT(!empty()); return *((reinterpret_cast<T*>(cursor)-1)); }
        inline T const& back() const { ASSERT(!empty()); return *((reinterpret_cast<T*>(cursor)-1)); }

        inline bool     empty() const { return cursor == buffer; }
        inline size_type size() const { return cursor - buffer; }

        inline iterator begin() { return reinterpret_cast<T*>(buffer); }
        inline iterator end()   { return reinterpret_cast<T*>(cursor); }

        inline const_iterator begin() const { return reinterpret_cast<T const*>(buffer); }
        inline const_iterator end()   const { return reinterpret_cast<T const*>(cursor); }
    };
}
