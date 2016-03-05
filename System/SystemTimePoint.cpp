//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SystemTimePoint.h"


#ifdef FX_LIB_WINDOWS

SystemTimePoint::duration SystemTimePoint::time_since_epoch()
{
	SystemTimePoint::duration result(m_value);
	return result;
}

#endif

#ifdef FX_LIB_LINUX

SystemTimePoint::duration SystemTimePoint::time_since_epoch()
{
    SystemTimePoint::duration result = std::chrono::seconds(m_value.tv_sec) + std::chrono::nanoseconds(m_value.tv_usec);
    return result;
}

#endif
