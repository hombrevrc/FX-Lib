//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "TaskWaiter.h"


namespace
{
	const uint32_t cNeedToWakeUp = static_cast<uint32_t>(1 << 31);
}

TaskWaiter::TaskWaiter() : m_state()
{
}

TaskWaiter::~TaskWaiter()
{
	assert(0 == m_state);
}

void TaskWaiter::Acquire()
{
	m_state.fetch_add(1);
}

void TaskWaiter::Release()
{
	const uint32_t newState = m_state.fetch_sub(1) - 1;
	if (cNeedToWakeUp == newState)
	{
		StdSemaphore& event = GetEvent();
		event.Release();
	}
}

void TaskWaiter::Join()
{
	uint32_t state = m_state.fetch_or(cNeedToWakeUp);

	if (0 != state)
	{
		StdSemaphore& event = GetEvent();
		event.Acquire();
	}

	m_state.fetch_and(~cNeedToWakeUp);
}

StdSemaphore& TaskWaiter::GetEvent()
{
	return m_event.GetOrCreate(0, 1);
}
