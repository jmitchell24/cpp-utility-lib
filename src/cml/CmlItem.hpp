// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
namespace cml
{
    class Value
    {
        string_t value;

    public:
        Value();
        Value(Segment const& value);

        bool empty() const;
        string_t::size_type size() const;

        template <typename T> inline bool tryGet(T& val) const;
        template <typename T> inline T       get()       const;
        template <typename T> inline T       getDefault(T const& def);

        template <typename T> inline void set(T const& val);
        template <unsigned N> inline void set(char_t const(& val)[N]); // catches string literals

		string_t const& str() const;
		bool tryParse(istream_t& is);
		bool tryParse(Segment const& s);

		DEFINE_OBJECT_OSTREAM_OPERATOR(Value)
		DEFINE_OBJECT_ISTREAM_OPERATOR(Value)
    };

    class Item : public Value
    {
    public:
        Value type;

        Item();
        Item(Segment const& value);
        Item(Segment const& value, Segment const& type);
    };

	template <typename T> bool Value::tryGet(T& val) const { return util::tryParse<T>(value, val); }
	template <> inline bool Value::tryGet(string_t& val) const { return util::tryParse(Nest::unnest(value), val); }

    template <typename T> T Value::get() const { return parse<T>(value);     }
	template <> inline string_t Value::get() const { return parse<string_t>(Nest::unnest(value)); }

    template <typename T> T Value::getDefault(T const& def)
    {
        T t;
		if (util::tryParse(value, t))
            return t;
        set(def);
        return def;
    }

	template <> inline string_t Value::getDefault(string_t const& def)
	{
		string_t t;
		if (util::tryParse(Nest::unnest(value), t))
			return t;
		value = toString(def);
		return def;
	}

    template <typename T> inline void Value::set(T const& val) { value = toString<T>(val); }
    template <>           inline void Value::set(string_t const& val) { value = Nest::is(val) ? val : toString(Nest::nest(val)); }
    template <unsigned N> inline void Value::set(char_t const(& val)[N]) { set(string_t(val)); }
}
}