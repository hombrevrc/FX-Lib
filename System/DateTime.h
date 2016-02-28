#pragma once

#include "SystemClock.h"

class DateTime
{
public:
	uint32_t Year;
	uint32_t Month;
	uint32_t Day;
	uint32_t Hour;
	uint32_t Minute;
	uint32_t Second;
	uint32_t Millisecond;
	uint32_t Microsecond;

public:
	DateTime();
	DateTime(SystemClock::time_point tp);
};
