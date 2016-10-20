// Copyright 2013, James Mitchell, All rights reserved.

#include "CmlItem.hpp"
using namespace util;
using namespace util::cml;

Item::Item()
    : Value(), type()
{}

Item::Item(Segment const& value)
    : Value(value), type()
{}

Item::Item(Segment const& value, Segment const& type)
    : Value(value), type(type)
{}

Value::Value()
    : value()
{}

Value::Value(Segment const& value)
    : value(value.str())
{}

bool Value::empty() const 
{ 
	return value.empty(); 
}

string_t::size_type Value::size() const 
{ 
	return value.size(); 
}

string_t const& Value::str() const
{
	return value;
}

bool Value::tryParse(istream_t& is)
{
	is >> value;
	return !is.fail();
}

bool Value::tryParse(Segment const& s)
{
	value = s.str();
	return true;
}