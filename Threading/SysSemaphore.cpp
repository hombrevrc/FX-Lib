//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SysSemaphore.h"


SysSemaphore::SysSemaphore()
{
	Construct(0, std::numeric_limits<int32_t>::max(), nullptr, nullptr);
}

SysSemaphore::SysSemaphore(const int32_t maxCount)
{
	Construct(0, maxCount, nullptr, nullptr);
}

SysSemaphore::SysSemaphore(const int32_t initialCount, const int32_t maxCount)
{
	Construct(initialCount, maxCount, nullptr, nullptr);
}

SysSemaphore::SysSemaphore(const std::string& name, const int32_t initialCount, const int32_t maxCount, void* security /* = nullptr */)
{
	std::wstring nameTemp = CA2W(name.c_str());
	Construct(initialCount, maxCount, nameTemp.c_str(), security);
}

SysSemaphore::SysSemaphore(const std::wstring& name, const int32_t initialCount, const int32_t maxCount, void* security /* = nullptr */)
{
	Construct(initialCount, maxCount, name.c_str(), security);
}

void SysSemaphore::Construct(const int32_t initialCount, const int32_t maxCount, const wchar_t* pName, void* security)
{
	if (nullptr != pName)
	{
		std::wstring fullName = MakeFullname(pName);
		m_handle = CreateSemaphore(reinterpret_cast<LPSECURITY_ATTRIBUTES>(security), initialCount, maxCount, fullName.c_str());
	}
	else
	{
		m_handle = CreateSemaphore(reinterpret_cast<LPSECURITY_ATTRIBUTES>(security), initialCount, maxCount, pName);
	}


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

void SysSemaphore::Release(const int32_t count)
{
	ReleaseSemaphore(m_handle, count, nullptr);
}

bool SysSemaphore::AcquireInMs(const std::chrono::milliseconds timeoutInMs)
{
	const bool result = AcquireInMs(static_cast<int32_t>(timeoutInMs.count()));
	return result;
}

bool SysSemaphore::AcquireInMs(const uint32_t timeoutInMs)
{
	const DWORD status = WaitForSingleObject(m_handle, static_cast<DWORD>(timeoutInMs));
	const bool result = (WAIT_OBJECT_0 == status);
	return result;
}

std::wstring SysSemaphore::MakeFullname(const std::wstring& name)
{
	std::wstring result = name + L"_semaphore";
	return result;
}

void SysSemaphore::Acquire()
{
	AcquireInMs(INFINITE);
}
