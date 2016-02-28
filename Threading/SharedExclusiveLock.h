//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SysSemaphore.h"

/// <summary>
/// Supports at least 1024 threads
/// </summary>
class  SharedExclusiveLock
{
public:
	SharedExclusiveLock();
	SharedExclusiveLock(const SharedExclusiveLock&) = delete;
	SharedExclusiveLock& operator = (const SharedExclusiveLock&) = delete;

public:
	void AcquireShared();
	void ReleaseShared();
	void AcquireExclusive();
	void ReleaseExclusive();
	bool TryAcquireShared();
	bool TryAcquireExclusive();

private:
	volatile LONG m_value;
	SysSemaphore m_sharedWakeEvent;
	SysSemaphore m_exclusiveWakeEvent;
};

// Many threads can shared acquire in the same time.
class SharedLocker
{
public:
	inline SharedLocker(SharedExclusiveLock& instance):m_instance(instance)
	{
		instance.AcquireShared();
	}

	inline ~SharedLocker()
	{
		m_instance.ReleaseShared();
	}

public:
	SharedLocker(const SharedLocker&) = delete;
	SharedLocker& operator = (const SharedLocker&) = delete;

private:

	SharedExclusiveLock& m_instance;
};

// Only one thread can exclusively acquire and no threads can shared acquire in the same time.
class ExclusiveLocker
{
public:
	inline ExclusiveLocker(SharedExclusiveLock& instance) : m_instance(instance)
	{
		instance.AcquireExclusive();
	}

	inline ~ExclusiveLocker()
	{
		m_instance.ReleaseExclusive();
	}

public:
	ExclusiveLocker(const ExclusiveLocker&) = delete;
	ExclusiveLocker& operator = (const ExclusiveLocker&) = delete;

private:
	SharedExclusiveLock& m_instance;
};
