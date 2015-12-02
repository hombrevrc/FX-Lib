//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename T> class TypeTraits
{
public:
	typedef T MemberType;
};

template<typename T> class TypeTraits < const T& >
{
public:
	typedef T MemberType;
};

template<typename T> class TypeTraits < T& >
{
public:
	typedef T MemberType;
};

template<typename T> class TypeTraits < T&& >
{
public:
	typedef T MemberType;
};
