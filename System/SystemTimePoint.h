//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class DateTime;

class SystemTimePoint
{
public:
	typedef std::chrono::duration<uint64_t, std::ratio<1, 10000000> > duration;

public:
	duration time_since_epoch();

private:
	uint64_t m_value;

private:
	friend class DateTime;
};
