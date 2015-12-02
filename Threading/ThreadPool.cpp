//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ThreadPool.h"


ThreadPool::ThreadPool(const uint32_t threadsNumber /* = 1 */) : m_continue(true), m_pFirst(nullptr), m_pLast(nullptr), m_event(0, threadsNumber)
{
	m_threads.reserve(threadsNumber);

	try
	{
		for (uint32_t index = 0; index < threadsNumber; ++index)
		{
			uint32_t id = 0;
			HANDLE handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &ThreadPool::ThreadFunction, this, 0, &id));
			m_threads.push_back(std::make_pair(id, handle));
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
	/*for (TaskBase* pCurrent = m_pFirst; nullptr != pCurrent;)
	{
		TaskBase* pTemp = pCurrent;
		pCurrent = pCurrent->GetNext();
		delete pTemp;
	}
	m_pFirst = nullptr;
	m_pLast = nullptr;*/
}

void ThreadPool::Join()
{
	for (const auto& element : m_threads)
	{
		WaitForSingleObject(element.second, INFINITE);
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

	m_synchronizer.Acquire();
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
	m_synchronizer.Release();
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
		m_event.WaitFor();
	}

	if (!m_continue)
	{
		return nullptr;
	}

	m_synchronizer.Acquire();

	TaskEx* result = m_pFirst;

	if (nullptr != result)
	{
		m_pFirst = result->Link;
		if (nullptr == m_pFirst)
		{
			m_pLast = nullptr;
		}
	}

	m_synchronizer.Release();

	return result;
}

uint32_t ThreadPool::ThreadFunction(void* pThis)
{
	ThreadPool* pThreadPool = reinterpret_cast<ThreadPool*>(pThis);
	pThreadPool->Loop();
	return 0;
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
	__try
	{
		DoStep(pTask);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	if (nullptr != pTask->Waiter)
	{
		pTask->Waiter->Release();
	}
}

void ThreadPool::DoStep(TaskEx* pTask)
{
	try
	{
		pTask->Invoke();
	}
	catch (const std::exception&)
	{
	}
}
