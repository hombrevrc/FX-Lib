#pragma once

#include "SystemClock.h"

class DateTime
{
public:
	uint32_t Year = 0;
	uint32_t Month = 0;
	uint32_t Day = 0;
	uint32_t Hour = 0;
	uint32_t Minute = 0;
	uint32_t Second = 0;
	uint32_t Millisecond = 0;
	uint32_t Microsecond = 0;

public:
	DateTime() = default;
	DateTime(SystemClock::time_point tp);
};

std::ostream& operator << (std::ostream& stream, const DateTime& dt);
