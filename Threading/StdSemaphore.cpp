//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StdSemaphore.h"


StdSemaphore::StdSemaphore()
{
	Construct(0, std::numeric_limits<uint32_t>::max());
}

StdSemaphore::StdSemaphore(const uint32_t maxCount)
{
	Construct(0, maxCount);
}

StdSemaphore::StdSemaphore(const uint32_t initialCount, const uint32_t maxCount)
{
	Construct(initialCount, maxCount);
}

void StdSemaphore::Construct(const uint32_t initialCount, const uint32_t maxCount)
{
	if (0 == maxCount)
	{
		throw std::invalid_argument("maxCount parameter can not be zero");
	}

	if (initialCount > m_maxCount)
	{
		throw std::invalid_argument("initialCount can not be greater than maxCount");
	}

	m_count = initialCount;
	m_maxCount = maxCount;
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

bool StdSemaphore::AcquireInMs(const uint32_t timeoutInMs)
{
	std::chrono::milliseconds duration(timeoutInMs);
	const bool result = AcquireInMs(duration);
	return result;
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