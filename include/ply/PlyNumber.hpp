// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include "../String.hpp"
#include "../Assertion.hpp"
#include "../misc/Integer.hpp"
#include "../typetraits/CallTraits.hpp"

#include "PlyTypes.hpp"

#define NUM_SIZE_BYTES sizeof(double)

namespace util
{
namespace ply
{
    enum NumberType { CHAR=0, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, INVALID, ANY/*ANY is not used in parsing*/ };

    template <typename> struct get_number_type_value { };
    template <> struct get_number_type_value<i8    > { NumberType static constexpr const value = CHAR;   };
    template <> struct get_number_type_value<u8    > { NumberType static constexpr const value = UCHAR;  };
    template <> struct get_number_type_value<i16   > { NumberType static constexpr const value = SHORT;  };
    template <> struct get_number_type_value<u16   > { NumberType static constexpr const value = USHORT; };
    template <> struct get_number_type_value<i32   > { NumberType static constexpr const value = INT;    };
    template <> struct get_number_type_value<u32   > { NumberType static constexpr const value = UINT;   };
    template <> struct get_number_type_value<float > { NumberType static constexpr const value = FLOAT;  };
    template <> struct get_number_type_value<double> { NumberType static constexpr const value = DOUBLE; };

    template <NumberType> struct get_number_type { };
    template <> struct get_number_type<CHAR  > { typedef i8     type; };
    template <> struct get_number_type<UCHAR > { typedef u8     type; };
    template <> struct get_number_type<SHORT > { typedef i16    type; };
    template <> struct get_number_type<USHORT> { typedef u16    type; };
    template <> struct get_number_type<INT   > { typedef i32    type; };
    template <> struct get_number_type<UINT  > { typedef u32    type; };
    template <> struct get_number_type<FLOAT > { typedef float  type; };
    template <> struct get_number_type<DOUBLE> { typedef double type; };

    template <typename T> static bool isTypeValid(NumberType t) { return get_number_type_value<T>::value == t; }

    unsigned getTypeSize(NumberType type);

    class Number
    {
        byte data[NUM_SIZE_BYTES];
        byte type;

    public:
        Number(NumberType const type, byte const* const b, unsigned const size);

        template <typename Num> Number(Num const n)
            : type(get_number_type_value<Num>::value)
        { memcpy(data, &n, sizeof(Num)); }

        Ostream& outAscii(Ostream& os, bool const print_pretty = false, unsigned const precision = 5) const;
        void outBin(Ostream& os) const;

        NumberType getType() const;

        template <typename Num> Num castGet() const
        {
            switch (type)
            {
#define NUMBER_CAST_CASE(TYPE_VALUE) \
case TYPE_VALUE: { typename get_number_type<TYPE_VALUE>::type x; memcpy(&x, data, sizeof(x)); return (Num)x; }
                NUMBER_CAST_CASE(CHAR  ) NUMBER_CAST_CASE(UCHAR )
                NUMBER_CAST_CASE(SHORT ) NUMBER_CAST_CASE(USHORT)
                NUMBER_CAST_CASE(INT   ) NUMBER_CAST_CASE(UINT  )
                NUMBER_CAST_CASE(FLOAT ) NUMBER_CAST_CASE(DOUBLE)
                default:break;
#undef NUMBER_CAST_CASE
            }
            return (Num)0;
        }
    };

    bool tryParseNumberType(Segment const& s, NumberType& t);
    String toStringNumberType(NumberType t);
}
}
