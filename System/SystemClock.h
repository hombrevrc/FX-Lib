//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SystemTimePoint.h"

class SystemClock
{
public:
	typedef SystemTimePoint time_point;
	typedef time_point::duration duration;

public:
	static constexpr bool is_steady = false;

public:
	static time_point now();
};