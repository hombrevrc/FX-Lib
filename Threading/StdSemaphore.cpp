//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StdSemaphore.h"


StdSemaphore::StdSemaphore(const uint32_t initialCount /*= 0*/, const uint32_t maxCount /*= (std::numeric_limits<uint32_t>::max)()*/) :
	m_count(initialCount), m_maxCount(maxCount)
{
	if (0 == maxCount)
	{
		throw std::invalid_argument("maxCount parameter can not be zero");
	}

	if (initialCount > m_maxCount)
	{
		throw std::invalid_argument("initalCount can not be greater than maxCount");
	}
}

void StdSemaphore::Acquire()
{
	std::unique_lock<std::mutex> lk(m_mutex);
	if (0 == m_count)
	{
		m_condition.wait(lk);
	}
	assert(m_count > 0);
	assert(m_count <= m_maxCount);
	--m_count;
}

bool StdSemaphore::AcquireInMs(std::chrono::milliseconds timeoutInMs)
{
	std::unique_lock<std::mutex> lk(m_mutex);
	if (0 == m_count)
	{
		const std::cv_status status = m_condition.wait_for(lk, timeoutInMs);
		if (std::cv_status::timeout == status)
		{
			return false;
		}
	}
	assert(m_count > 0);
	assert(m_count <= m_maxCount);
	--m_count;
	return true;
}

void StdSemaphore::Release()
{
	bool wakeUp = false;
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_count < m_maxCount)
		{
			++m_count;
			wakeUp = true;
		}
	}
	if (wakeUp)
	{
		m_condition.notify_one();
	}
}