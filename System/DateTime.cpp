//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "DateTime.h"


DateTime::DateTime()
{
	ZeroMemory(this, sizeof(*this));
}

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
	Microsecond = (tp.m_value + 5) / 10 % 1000;
}
