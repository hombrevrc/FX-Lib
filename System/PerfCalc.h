//==============================================================
// Copyright (c) 2017 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class PerfCalc
{
public:
	void Add(uint64_t value);

private:
	uint64_t _count = 0;
	uint64_t _min = (std::numeric_limits<uint64_t>::max)();
	uint64_t _max = 0;
	uint64_t _total = 0;

private:
	friend std::ostream& operator << (std::ostream& stream, const PerfCalc& calc);
};