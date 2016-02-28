//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "BinFlushingQueue.h"
#include "BinLogger.h"


namespace
{
	const uint32_t cSleepIntervalInMs = 1024;
}

BinFlushingQueue::BinFlushingQueue() : m_continue(true), m_event(0, 1)
{
	m_thread = std::thread(&BinFlushingQueue::Loop, this);
}

BinFlushingQueue::~BinFlushingQueue()
{
	Stop();
}

void BinFlushingQueue::Add(BinLogger* pLogger)
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

void BinFlushingQueue::Remove(BinLogger* pLogger)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	auto it = std::remove(m_loggers.begin(), m_loggers.end(), pLogger);
	m_loggers.erase(it, m_loggers.end());
}

void BinFlushingQueue::Stop()
{
	m_continue = false;
	m_event.Release();
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void BinFlushingQueue::Loop()
{
	for (; m_continue; m_event.AcquireInMs(cSleepIntervalInMs))
	{
		Flush();
	}
	SwithchToSynchronousMode();
	Flush();
}

void BinFlushingQueue::SwithchToSynchronousMode()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	for (auto element : m_loggers)
	{
		element->SwitchToSynchronousMode();
	}
	m_loggers.clear();
}

void BinFlushingQueue::Flush()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	for (auto element : m_loggers)
	{
		element->Flush();
	}
}
