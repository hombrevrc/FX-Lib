//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename T>
inline T EnumBitwiseOr(T first, T second)
{
	static_assert(std::is_enum<T>::value, "EnumBitwiseOr can be used for enumerations only");
	static_assert(sizeof(T) <= sizeof(uint64_t), "Unexpected size of enumeration type");

	const uint64_t f = static_cast<uint64_t>(first);
	const uint64_t s = static_cast<uint64_t>(second);

	const T result = static_cast<T>(f | s);
	return result;
}

template<typename T>
inline T EnumBitwiseAnd(T first, T second)
{
	static_assert(std::is_enum<T>::value, "EnumBitwiseOr can be used for enumerations only");
	static_assert(sizeof(T) <= sizeof(uint64_t), "Unexpected size of enumeration type");

	const uint64_t f = static_cast<uint64_t>(first);
	const uint64_t s = static_cast<uint64_t>(second);

	const T result = static_cast<T>(f & s);
	return result;
}
