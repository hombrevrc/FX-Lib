//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class SysSemaphore
{
public:
	SysSemaphore();
	SysSemaphore(const int32_t maxCount);
	SysSemaphore(const int32_t initialCount, const int32_t maxCount);
	SysSemaphore(const std::string& name, const int32_t initialCount, const int32_t maxCount, void* security = nullptr);
	SysSemaphore(const std::wstring& name, const int32_t initialCount, const int32_t maxCount, void* security = nullptr);
	SysSemaphore(const SysSemaphore&) = delete;
	SysSemaphore& operator = (const SysSemaphore&) = delete;
	~SysSemaphore();

public:
	void Release();
	void Release(const int32_t count);
	void Acquire();
	bool AcquireInMs(const std::chrono::milliseconds timeoutInMs);
	bool AcquireInMs(const uint32_t timeoutInMs);

private:
	static std::wstring MakeFullname(const std::wstring& name);

private:
	void Construct(const int32_t initialCount, const int32_t maxCount, const wchar_t* pName, void* security);

private:
	HANDLE m_handle;
};
