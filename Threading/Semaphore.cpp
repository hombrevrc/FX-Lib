//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Semaphore.h"


Semaphore::Semaphore(const uint32_t maxCount)
{
	m_handle = CreateSemaphore(nullptr, 0, maxCount, nullptr);
	if (nullptr == m_handle)
	{
		throw std::runtime_error("Semaphore::Semaphore(): couldn't create a new semaphore");
	}
}

Semaphore::Semaphore(const uint32_t initialCount, const uint32_t maxCount)
{
	m_handle = CreateSemaphore(nullptr, initialCount, maxCount, nullptr);
	if (nullptr == m_handle)
	{
		throw std::runtime_error("Semaphore::Semaphore(): couldn't create a new semaphore");
	}
}

Semaphore::~Semaphore()
{
	CloseHandle(m_handle);
	m_handle = nullptr;
}

void Semaphore::Release()
{
	ReleaseSemaphore(m_handle, 1, nullptr);
}

void Semaphore::Release(const uint32_t count)
{
	ReleaseSemaphore(m_handle, count, nullptr);
}

bool Semaphore::WaitFor(const uint32_t timeoutInMs)
{
	const DWORD status = WaitForSingleObject(m_handle, static_cast<DWORD>(timeoutInMs));
	const bool result = (WAIT_OBJECT_0 == status);
	return result;
}

void Semaphore::WaitFor()
{
	WaitFor(INFINITE);
}
