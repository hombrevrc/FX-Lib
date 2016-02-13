//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Timeout.h"


Timeout::Timeout(const std::chrono::milliseconds interval) : m_finish(std::chrono::steady_clock::now() + interval)
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
