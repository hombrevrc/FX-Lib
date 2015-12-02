//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class CriticalSection
{
public:
	CriticalSection();
	CriticalSection(const CriticalSection&) = delete;
	CriticalSection& operator = (const CriticalSection&) = delete;
	~CriticalSection();

public:
	void Acquire();
	void Release();

private:
	CRITICAL_SECTION m_section;
};

class CsLocker
{
public:
	inline CsLocker(CriticalSection& instance) : m_instance(instance)
	{
		instance.Acquire();
	}

	inline ~CsLocker()
	{
		m_instance.Release();
	}

public:
	CsLocker(const CsLocker&) = delete;
	CsLocker& operator = (const CsLocker&) = delete;

private:
	CriticalSection& m_instance;
};
