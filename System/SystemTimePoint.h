//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class DateTime;

class SystemTimePoint
{
public:
    #ifdef FX_LIB_WINDOWS
	typedef std::chrono::duration<uint64_t, std::ratio<1, 10000000> > duration;
    #endif

    #ifdef FX_LIB_LINUX
    typedef std::chrono::nanoseconds duration;
    #endif

public:
    SystemTimePoint() = default;

public:
	duration time_since_epoch();

private:
    #ifdef FX_LIB_WINDOWS
    uint64_t m_value;
    #endif

    #ifdef FX_LIB_LINUX
    timeval m_value;
    #endif

private:
	friend class DateTime;
};
