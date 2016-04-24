//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "FastFlushingQueue.h"
#include "FastLogger.h"


namespace
{
	const uint32_t cSleepIntervalInMs = 1024;
}

FastFlushingQueue::FastFlushingQueue() : m_continue(true), m_event(0, 1)
{
	m_thread = std::thread(&FastFlushingQueue::Loop, this);
}

FastFlushingQueue::~FastFlushingQueue()
{
	Stop();
}

void FastFlushingQueue::Add(FastLogger* pLogger)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_continue)
	{
		m_loggers.push_back(pLogger);
	}
	else
	{
		pLogger->SwitchToSynchronousMode();
	}
}

void FastFlushingQueue::Remove(FastLogger* pLogger)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	auto it = std::remove(m_loggers.begin(), m_loggers.end(), pLogger);
	m_loggers.erase(it, m_loggers.end());
}

void FastFlushingQueue::Stop()
{
	m_continue = false;
	m_event.Release();
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void FastFlushingQueue::Loop()
{
	for (; m_continue; m_event.AcquireInMs(cSleepIntervalInMs))
	{
		Flush();
	}
	SwithchToSynchronousMode();
	Flush();
}

void FastFlushingQueue::SwithchToSynchronousMode()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	for (auto element : m_loggers)
	{
		element->SwitchToSynchronousMode();
        element->Flush();
	}
	m_loggers.clear();
}

void FastFlushingQueue::Flush()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	for (auto element : m_loggers)
	{
		element->Flush();
	}
}
