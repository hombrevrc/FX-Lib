//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Optex.h"


namespace
{
	const uint32_t cSpinAttempts = 4000;
}

namespace
{
	struct OptexData
	{
		std::atomic<uint32_t>	lock;
		uint32_t				counter;
		uint32_t				threadID;
	};
}

namespace
{
	bool TryLock(const uint32_t threadId, OptexData& data)
	{
		// if the thread locked the critical section already then we use recursion counter
		if (threadId == data.threadID)
		{
			data.counter++;
			return true;
		}

		uint32_t attempt = 0;
		for (; attempt < cSpinAttempts; attempt++)
		{
			uint32_t oldValue = 0;
			if (data.lock.compare_exchange_strong(oldValue, 1))
			{
				data.threadID = threadId;	// for recursion supporting
				data.counter = 1;			// initial value of recursion counter
				return true;
			}
		}
		return false;
	}
}

Optex::Optex(const std::tstring& name) : m_memory(MakeFullname(name), sizeof(OptexData)), m_event(MakeFullname(name), 0, 1)
{
}

std::wstring Optex::MakeFullname(const std::wstring& name)
{
	return name + L"_optex";
}

uint32_t Optex::SharedMemorySize()
{
	return sizeof(OptexData);
}

void Optex::lock()
{
	const uint32_t threadId = GetCurrentThreadId();
	OptexData& data = m_memory.GetRef<OptexData>(0);
	if (!TryLock(threadId, data))
	{
		const uint32_t lock = data.lock.fetch_add(1);
		if (0 != lock)
		{
			m_event.Acquire();
		}
		data.threadID = threadId;	// for recursion supporting
		data.counter = 1;			// initial value of recursion counter
	}
}

void Optex::unlock()
{
	OptexData& data = m_memory.GetRef<OptexData>(0);

	assert(GetCurrentThreadId() == data.threadID);

	assert(data.counter > 0);
	data.counter--;

	if (0 == data.counter)
	{
		data.threadID = 0;
		const uint32_t lock = data.lock.fetch_sub(1);
		if (lock > 1)
		{
			m_event.Release();
		}
	}
}
