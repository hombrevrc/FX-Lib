//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SysSemaphore.h"


SysSemaphore::SysSemaphore()
{
	Construct(0, std::numeric_limits<int32_t>::max());
}

SysSemaphore::SysSemaphore(const uint32_t maxCount)
{
	Construct(0, maxCount);
}

SysSemaphore::SysSemaphore(const uint32_t initialCount, const uint32_t maxCount)
{
	Construct(initialCount, maxCount);
}

void SysSemaphore::Construct(const uint32_t initialCount, const uint32_t maxCount)
{
	m_handle = CreateSemaphore(nullptr, initialCount, maxCount, nullptr);
	if (nullptr == m_handle)
	{
		throw SystemException("Couldn't create a new semaphore");
	}
}

SysSemaphore::~SysSemaphore()
{
	if (nullptr != m_handle)
	{
		CloseHandle(m_handle);
		m_handle = nullptr;
	}
}

void SysSemaphore::Release()
{
	ReleaseSemaphore(m_handle, 1, nullptr);
}

void SysSemaphore::Release(const uint32_t count)
{
	ReleaseSemaphore(m_handle, count, nullptr);
}

bool SysSemaphore::AcquireInMs(const std::chrono::milliseconds timeoutInMs)
{
	const bool result = AcquireInMs(static_cast<uint32_t>(timeoutInMs.count()));
	return result;
}

bool SysSemaphore::AcquireInMs(const uint32_t timeoutInMs)
{
	const DWORD status = WaitForSingleObject(m_handle, static_cast<DWORD>(timeoutInMs));
	const bool result = (WAIT_OBJECT_0 == status);
	return result;
}

void SysSemaphore::Acquire()
{
	AcquireInMs(INFINITE);
}
