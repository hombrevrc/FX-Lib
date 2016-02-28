//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool(const uint32_t threadsNumber /* = 1 */) : m_event(0, threadsNumber)
{
	m_threads.reserve(threadsNumber);

	try
	{
		for (uint32_t index = 0; index < threadsNumber; ++index)
		{
			std::thread thread(&ThreadPool::Loop, this);
			m_threads.push_back(std::move(thread));
		}
	}
	catch (const std::exception&)
	{
		Finalize();
		throw;
	}
}

ThreadPool::~ThreadPool()
{
	Finalize();
}

void ThreadPool::Finalize()
{
	m_continue = false;
	Finalizing();
	Join();

	DeleteTasks();
}

void ThreadPool::JoinAndFinalize()
{
	Finalizing();
	Join();

	assert(nullptr == m_pFirst);
	assert(nullptr == m_pLast);
	DeleteTasks();
}

void ThreadPool::Finalizing()
{
	const size_t count = m_threads.size();
	for (size_t index = 0; index < count; ++index)
	{
		Release();
	}
}

void ThreadPool::DeleteTasks()
{
	for (TaskEx* pCurrent = m_pFirst; nullptr != pCurrent;)
	{
		TaskEx* pTemp = pCurrent;
		pCurrent = pCurrent->Link;
		delete pTemp;
	}
	m_pFirst = nullptr;
	m_pLast = nullptr;
}

void ThreadPool::Join()
{
	for (auto& element : m_threads)
	{
		if (element.joinable())
		{
			element.join();
		}
	}
	m_threads.clear();
}

void ThreadPool::AddTask(TaskEx* pTask)
{
	assert(nullptr != pTask);
	assert(m_continue);

	if (nullptr != pTask->Waiter)
	{
		pTask->Waiter->Acquire();
	}

	m_synchronizer.lock();
	if (nullptr == m_pFirst)
	{
		assert(nullptr == m_pLast);
		m_pFirst = pTask;
		m_pLast = pTask;
	}
	else
	{
		m_pLast->Link = pTask;
		m_pLast = pTask;
	}
	m_synchronizer.unlock();
	Release();
}

uint32_t ThreadPool::NumberOfTasks() const 
{
	uint32_t result = static_cast<uint32_t>(m_counter + m_threads.size());
	return result;
}

void ThreadPool::Release()
{
	int32_t counter = m_counter.fetch_add(1);
	if (counter < 0)
	{
		m_event.Release();
	}
}

ThreadPool::TaskEx* ThreadPool::WaitFor()
{
	int32_t counter = m_counter.fetch_sub(1);
	if (counter < 1)
	{
		m_event.Acquire();
	}

	if (!m_continue)
	{
		return nullptr;
	}

	m_synchronizer.lock();

	TaskEx* result = m_pFirst;

	if (nullptr != result)
	{
		m_pFirst = result->Link;
		if (nullptr == m_pFirst)
		{
			m_pLast = nullptr;
		}
	}

	m_synchronizer.unlock();

	return result;
}

void ThreadPool::Loop()
{
	for (TaskEx* pTask = WaitFor(); nullptr != pTask; pTask = WaitFor())
	{
		Step(pTask);
		delete pTask;
	}
}

void ThreadPool::Step(TaskEx* pTask)
{
	try
	{
		pTask->Invoke();
	}
	catch(...)
	{
	}

	if (nullptr != pTask->Waiter)
	{
		pTask->Waiter->Release();
	}
}
