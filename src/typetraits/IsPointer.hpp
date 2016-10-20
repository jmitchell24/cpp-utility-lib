// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"

namespace util
{
	template <typename T> struct is_pointer           { DEFINE_VALUE(false); };
	template <typename T> struct is_pointer<T*>       { DEFINE_VALUE(true); };
	template <typename T> struct is_pointer<T const*> { DEFINE_VALUE(true); };
}