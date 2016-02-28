//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class StdSemaphore
{
public:
	StdSemaphore();
	StdSemaphore(const uint32_t maxCount);
	StdSemaphore(const uint32_t initialCount, const uint32_t maxCount);
	StdSemaphore(const StdSemaphore&) = delete;
	StdSemaphore& operator = (const StdSemaphore&) = delete;

public:
	void Acquire();
	bool AcquireInMs(std::chrono::milliseconds timeoutInMs);
	bool AcquireInMs(const uint32_t timeoutInMs);
	void Release();

private:
	void Construct(const uint32_t initialCount, const uint32_t maxCount);

private:
	uint32_t m_count;
	uint32_t m_maxCount;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};