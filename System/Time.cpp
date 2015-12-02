//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Time.h"


namespace
{
	uint64_t GetQueryPerformanceFrequency()
	{
		uint64_t result = 0;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&result));
		return result;
	}
}

namespace
{
	const double gFrequency = static_cast<double>(GetQueryPerformanceFrequency());
}

uint64_t GetTimeCounterNow()
{
	uint64_t result = 0;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));
	return result;
}

double GetMillisecondsFromTimeCounter(uint64_t timeCounter)
{
	double result = timeCounter / gFrequency * 1000;
	return result;
}

double GetMillisecondsFromTimeCounter(double timeCounter)
{
	double result = timeCounter / gFrequency * 1000;
	return result;
}

double GetMicrosecondsFromTimeCounter(uint64_t timeCounter)
{
	double result = timeCounter / gFrequency * 1000000;
	return result;
}

double GetMicrosecondsFromTimeCounter(double timeCounter)
{
	double result = timeCounter / gFrequency * 1000000;
	return result;
}

double GetSecondsFromTimeCounter(uint64_t timeCounter)
{
	double result = timeCounter / gFrequency;
	return result;
}

double GetSecondsFromTimeCounter(double timeCounter)
{
	double result = timeCounter / gFrequency;
	return result;
}

uint64_t GetTicksFromMicroseconds(double timeInMks)
{
	assert(timeInMks >= 0);
	uint64_t result = static_cast<uint64_t>(timeInMks * gFrequency /1000000 + 0.5);
	return result;
}
