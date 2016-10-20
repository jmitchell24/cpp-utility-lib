// Copyright 2014, James Mitchell, All rights reserved.

#include "PlyNumber.hpp"
#include "../Exception.hpp"
#include "../Assertion.hpp"
using namespace util::ply;
using namespace std;

#include <iostream>
#include <iomanip>

Segment static const SEG_DATUM_TYPES[] = {
    ("char"),
    ("uchar"),
    ("short"),
    ("ushort"),
    ("int"),
    ("uint"),
    ("float"),
    ("double"),
    ("invalid")
};

unsigned util::ply::getTypeSize(NumberType type)
{
    switch (type)
    {
#define TYPE_SIZE_CASE(TYPE_VALUE) \
case TYPE_VALUE: return sizeof(typename get_number_type<TYPE_VALUE>::type);
        TYPE_SIZE_CASE(CHAR  ) TYPE_SIZE_CASE(UCHAR )
        TYPE_SIZE_CASE(SHORT ) TYPE_SIZE_CASE(USHORT)
        TYPE_SIZE_CASE(INT   ) TYPE_SIZE_CASE(UINT  )
        TYPE_SIZE_CASE(FLOAT ) TYPE_SIZE_CASE(DOUBLE)
        default:break;
#undef TYPE_SIZE_CASE
    }
    return 0;
}

bool util::ply::tryParseNumberType(SegmentT<char> const& s, NumberType& t)
{
    for (unsigned i = 0; i < INVALID; ++i)
    {
        if (s.same(SEG_DATUM_TYPES[i]))
        {
            t = (NumberType)i;
            return true;
        }
    }
    return false;
}

String util::ply::toStringNumberType(NumberType t)
{
    return SEG_DATUM_TYPES[(unsigned)t].str();
}

Number::Number (NumberType const type, byte const* const b, unsigned const size)
    : type(type)
{ memcpy(data, b, size); }

Ostream& Number::outAscii(Ostream& os, bool const print_pretty, unsigned const precision) const
{
	os.precision(precision);
	if (print_pretty)
	{
		switch (type)
		{
			case CHAR  : os << std::left << std::setw(3) << (signed)   castGet<typename get_number_type<CHAR  >::type>(); break;
			case UCHAR : os << std::left << std::setw(3) << (unsigned) castGet<typename get_number_type<UCHAR >::type>(); break;
			case SHORT : os << std::left << std::setw(5) <<            castGet<typename get_number_type<SHORT >::type>(); break;
			case USHORT: os << std::left << std::setw(5) <<            castGet<typename get_number_type<USHORT>::type>(); break;
			case INT   : os << std::left << std::setw(9) <<            castGet<typename get_number_type<INT   >::type>(); break;
			case UINT  : os << std::left << std::setw(9) <<            castGet<typename get_number_type<UINT  >::type>(); break;
			case FLOAT : os << std::left << std::setw(precision+4) << std::fixed << castGet<typename get_number_type<FLOAT >::type>(); break;
			case DOUBLE: os << std::left << std::setw(precision+7) << std::fixed << castGet<typename get_number_type<DOUBLE>::type>(); break;
			default:break;
		}
	}
	else
	{
		switch (type)
		{
			case CHAR  : os << (signed)      castGet<typename get_number_type<CHAR  >::type>(); break;
			case UCHAR : os << (unsigned)    castGet<typename get_number_type<UCHAR >::type>(); break;
			case SHORT : os <<               castGet<typename get_number_type<SHORT >::type>(); break;
			case USHORT: os <<               castGet<typename get_number_type<USHORT>::type>(); break;
			case INT   : os <<               castGet<typename get_number_type<INT   >::type>(); break;
			case UINT  : os <<               castGet<typename get_number_type<UINT  >::type>(); break;
			case FLOAT : os << std::fixed << castGet<typename get_number_type<FLOAT >::type>(); break;
			case DOUBLE: os << std::fixed << castGet<typename get_number_type<DOUBLE>::type>(); break;
			default:break;
		}
	}

	return os;
}

void Number::outBin(Ostream& os) const
{
    os.write((char*)data, getTypeSize((NumberType)type));
}

NumberType Number::getType() const
{
    return (NumberType)type;
}
