//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Timer.h"


namespace
{
	const uint64_t cMaxTimeoutInMs = std::numeric_limits<int32_t>::max();
}

Timer::Timer() : m_continue(true), m_event(0, 1)
{
	m_thread = std::thread(&Timer::ThreadFunction, this);
}

Timer::~Timer()
{
	m_continue = false;
	m_event.Release();
	m_thread.join();
}

void Timer::ThreadFunction(Timer* pTimer)
{
	pTimer->MainLoop();
}

void Timer::MainLoop()
{
	uint32_t timeoutInMs = 0;

	for (; m_continue; m_event.AcquireInMs(timeoutInMs))
	{
		timeoutInMs = Loop();
	}
}

uint32_t Timer::Loop()
{
	for (; m_continue;)
	{
		uint32_t result = Step();
		if (result > 0)
		{
			return result;
		}
	}
	return 0;
}

uint32_t Timer::Step()
{
	TaskEx* pTask = nullptr;

	{
		std::unique_lock<std::mutex> lock(m_synchronizer);
		if (m_timers.empty())
		{
			return static_cast<uint32_t>(cMaxTimeoutInMs);
		}

		auto it = m_timers.begin();
		const uint64_t now = GetTickCount64();

		if (now < it->first)
		{
			const uint64_t interval = std::min(it->first - now, cMaxTimeoutInMs);
			return static_cast<uint32_t>(interval);
		}

		pTask = it->second;

		if (pTask->PeriodInMs > 0)
		{
			auto it2 = m_timers.insert(std::make_pair(it->first + pTask->PeriodInMs, pTask));
			*(pTask->Pointer) = it2;
			pTask->Acquire();
		}

		m_timers.erase(it);
	}

	DoStep(pTask);

	pTask->Release();

	return 0;
}

void Timer::DoStep(TaskEx* pTask)
{
	try
	{
		pTask->Invoke();
	}
	catch (const std::exception&)
	{
	}
	catch (...)
	{
	}
}

bool Timer::StopTimer(const std::string& name)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);

	auto it = m_nameToTimer.find(name);
	if (m_nameToTimer.end() == it)
	{
		return false;
	}

	auto it2 = it->second;
	assert(m_timers.end() != it2);

	it2->second->Release();

	m_timers.erase(it2);
	m_nameToTimer.erase(it);

	return true;
}

void Timer::StartTimer(const std::string& name, const uint64_t offsetInMs, const uint64_t periodInMs, TaskEx* pTask)
{
	const uint64_t now = GetTickCount64();

	std::unique_lock<std::mutex> lock(m_synchronizer);

	if (m_nameToTimer.count(name) > 0)
	{
		throw std::runtime_error("Timer::StartTimer(): timer already exist; name = " + name);
	}

	iterator& it = m_nameToTimer[name];
	pTask->PeriodInMs = periodInMs;
	pTask->Pointer = &it;

	auto it2 = m_timers.insert(std::make_pair(now + offsetInMs, pTask));
	it = it2;
	m_event.Release();
}
