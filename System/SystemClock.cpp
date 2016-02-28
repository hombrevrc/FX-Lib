//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SystemClock.h"


SystemClock::time_point SystemClock::now()
{
	SystemClock::time_point result;
	GetSystemTimePreciseAsFileTime(reinterpret_cast<FILETIME*>(&result));
	return result;
}
