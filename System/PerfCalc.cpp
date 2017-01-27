//==============================================================
// Copyright (c) 2017 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PerfCalc.h"

void PerfCalc::Add(uint64_t value)
{
	_count++;
	if (value < _min)
	{
		_min = value;
	}
	if (value > _max)
	{
		_max = value;
	}
	_total += value;
}

std::ostream& operator<<(std::ostream& stream, const PerfCalc& calc)
{
	stream << "{count = " << calc._count;

	if (0 != calc._count)
	{
		stream << ", average = " << static_cast<double>(calc._total) / calc._count;
		stream << ", min = " << calc._min;
		stream << ", max = " << calc._max;
	}
	stream << "}";
	return stream;
}
