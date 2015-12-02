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
	Delegate<void()> func(this, &BinFlushingQueue::Loop);
	m_thread.Invoke(func);
}

BinFlushingQueue::~BinFlushingQueue()
{
	m_continue = false;
	m_event.Release();
	m_thread.Finalize();
}

void BinFlushingQueue::Add(BinLogger* pLogger)
{
	CsLocker lock(m_synchronizer);
	m_loggers.push_back(pLogger);
}

void BinFlushingQueue::Remove(BinLogger* pLogger)
{
	CsLocker lock(m_synchronizer);
	auto it = std::remove(m_loggers.begin(), m_loggers.end(), pLogger);
	m_loggers.erase(it, m_loggers.end());
}

void BinFlushingQueue::Loop()
{
	for (; m_continue; m_event.WaitFor(cSleepIntervalInMs))
	{
		Flush();
	}
	Flush();
}

void BinFlushingQueue::Flush()
{
	CsLocker lock(m_synchronizer);
	for (auto element : m_loggers)
	{
		element->Flush();
	}
}
