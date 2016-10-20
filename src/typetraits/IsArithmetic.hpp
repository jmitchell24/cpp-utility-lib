// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

namespace util
{
	template <typename T> struct is_integral       { bool static const value = false; };
	template <typename T> struct is_floating_point { bool static const value = false; };

#define DECLARE_IS_INTEGRAL_SPECIALIZATION(type) \
	template <> struct is_integral<signed type>   { bool static const value = true; }; \
	template <> struct is_integral<unsigned type> { bool static const value = true; }; 

	DECLARE_IS_INTEGRAL_SPECIALIZATION(char)
	DECLARE_IS_INTEGRAL_SPECIALIZATION(short)
	DECLARE_IS_INTEGRAL_SPECIALIZATION(int)
	DECLARE_IS_INTEGRAL_SPECIALIZATION(long)
	DECLARE_IS_INTEGRAL_SPECIALIZATION(long long)
#undef DECLARE_IS_INTEGRAL_SPECIALIZATION

#define DECLARE_IS_FLOATING_POINT_SPECIALIZATION(type) \
	template <> struct is_floating_point<type> { bool static const value = true; }; 

	DECLARE_IS_FLOATING_POINT_SPECIALIZATION(float)
	DECLARE_IS_FLOATING_POINT_SPECIALIZATION(double)
	DECLARE_IS_FLOATING_POINT_SPECIALIZATION(long double)
#undef DECLARE_IS_FLOATING_POINT_SPECIALIZATION

	template <typename T> struct is_arithmetic { bool static const value = is_integral<T>::value || is_floating_point<T>::value; };
}