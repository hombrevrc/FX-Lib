//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Timeout.h"


Timeout::Timeout(const std::chrono::milliseconds interval) :
	m_finish(std::chrono::steady_clock::now() + interval)
{
}

Timeout::Timeout(const uint32_t intervalInMs) :
	m_finish(std::chrono::steady_clock::now() + std::chrono::milliseconds(intervalInMs))
{
}

std::chrono::milliseconds Timeout::ToInterval() const
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if (now >= m_finish)
	{
		return std::chrono::milliseconds();
	}

	std::chrono::milliseconds result = std::chrono::duration_cast<std::chrono::milliseconds>(m_finish - now);
	return result;
}

uint32_t Timeout::ToIntervalInMs() const
{
	std::chrono::milliseconds interval = ToInterval();
	if (interval.count() > std::numeric_limits<uint32_t>::max())
	{
		return std::numeric_limits<uint32_t>::max();
	}

	const uint32_t result = static_cast<uint32_t>(interval.count());
	return result;
}
