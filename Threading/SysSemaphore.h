//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class SysSemaphore
{
public:
	SysSemaphore();
	SysSemaphore(const uint32_t maxCount);
	SysSemaphore(const uint32_t initialCount, const uint32_t maxCount);
	SysSemaphore(const SysSemaphore&) = delete;
	SysSemaphore& operator = (const SysSemaphore&) = delete;
	~SysSemaphore();

public:
	void Release();
	void Release(const uint32_t count);
	void Acquire();
	bool AcquireInMs(const std::chrono::milliseconds timeoutInMs);
	bool AcquireInMs(const uint32_t timeoutInMs);

private:
	void Construct(const uint32_t initialCount, const uint32_t maxCount);

private:
	HANDLE m_handle;
};
