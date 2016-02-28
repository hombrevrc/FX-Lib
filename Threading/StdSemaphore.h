//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class StdSemaphore
{
public:
	StdSemaphore(const uint32_t initialCount = 0, const uint32_t maxCount = (std::numeric_limits<uint32_t>::max)());
	StdSemaphore(const StdSemaphore&) = delete;
	StdSemaphore& operator = (const StdSemaphore&) = delete;

public:
	void Acquire();
	bool AcquireInMs(std::chrono::milliseconds timeoutInMs);
	void Release();

private:
	uint32_t m_count;
	uint32_t m_maxCount;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};