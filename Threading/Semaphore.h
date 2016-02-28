//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Semaphore
{
public:
	Semaphore(const uint32_t maxCount);
	Semaphore(const uint32_t initialCount, const uint32_t maxCount);
	Semaphore(const Semaphore&) = delete;
	Semaphore& operator = (const Semaphore&) = delete;
	~Semaphore();

public:
	void Release();
	void Release(const uint32_t count);
	void Acquire();
	bool AcquireInMs(const std::chrono::milliseconds timeoutInMs);
	bool AcquireInMs(const uint32_t timeoutInMs);

private:
	HANDLE m_handle;
};
