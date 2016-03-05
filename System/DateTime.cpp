//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include <iomanip>
#include "stdafx.h"
#include "DateTime.h"


#ifdef FX_LIB_WINDOWS

DateTime::DateTime(SystemClock::time_point tp)
{
	SYSTEMTIME st;
	FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&tp), &st);

	Year = st.wYear;
	Month = st.wMonth;
	Day = st.wDay;
	Hour = st.wHour;
	Minute = st.wMinute;
	Second = st.wSecond;
	Millisecond = st.wMilliseconds;
	Microsecond = tp.m_value / 10 % 1000;
}

#endif

#ifdef FX_LIB_LINUX

DateTime::DateTime(SystemClock::time_point tp)
{
    const timeval* pValue = reinterpret_cast<const timeval*>(&tp);
    const tm& gm = *gmtime(&pValue->tv_sec);

	Year = static_cast<uint32_t >(gm.tm_year);
	Month = static_cast<uint32_t >(gm.tm_mon) + 1;
	Day = static_cast<uint32_t >(gm.tm_mday);
	Hour = static_cast<uint32_t >(gm.tm_hour);
	Minute = static_cast<uint32_t >(gm.tm_min);
	Second = static_cast<uint32_t >(gm.tm_sec);
	Millisecond = pValue->tv_usec / 1000;
	Microsecond = pValue->tv_usec % 1000;
}

#endif

std::ostream& operator << (std::ostream& stream, const DateTime& dt)
{
    stream << std::setw(4) << std::setfill('0') << dt.Year;
    stream << '-';
    stream << std::setw(2) << std::setfill('0') << dt.Month;
    stream << '-';
    stream << std::setw(2) << std::setfill('0') << dt.Day;

    stream << ' ';

    stream << std::setw(2) << std::setfill('0') << dt.Hour;
    stream << ':';
    stream << std::setw(2) << std::setfill('0') << dt.Minute;
    stream << ':';
    stream << std::setw(2) << std::setfill('0') << dt.Second;
    stream << '.';
    stream << std::setw(3) << std::setfill('0') << dt.Millisecond;
    stream << '.';
    stream << std::setw(3) << std::setfill('0') << dt.Microsecond;

    return stream;
}
