//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SystemClock.h"
#include "SystemException.h"


#ifdef FX_LIB_WINDOWS

SystemClock::time_point SystemClock::now()
{
    SystemClock::time_point result;
    GetSystemTimePreciseAsFileTime(reinterpret_cast<FILETIME*>(&result));
    return result;
}

#endif

#ifdef FX_LIB_LINUX

SystemClock::time_point SystemClock::now()
{
    SystemClock::time_point result;
    gettimeofday(reinterpret_cast<timeval*>(&result), nullptr);
    return result;
}

#endif
