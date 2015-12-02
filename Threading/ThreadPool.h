//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Semaphore.h"
#include "CriticalSection.h"
#include "TaskWaiter.h"


namespace Internal
{
	class ThreadPoolTask
	{
	public:
		TaskBase<ThreadPoolTask>* Link = nullptr;
		TaskWaiter* Waiter = nullptr;
	};
}

class  ThreadPool
{
private:
	typedef TaskBase<Internal::ThreadPoolTask> TaskEx;

public:
	ThreadPool(const uint32_t threadsNumber = 1);
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator = (const ThreadPool&) = delete;
	~ThreadPool();

public:
	void Finalize();
	void JoinAndFinalize();
	uint32_t NumberOfTasks() const;

public:
	template<typename TFunc, typename ... TArgs> void Invoke(const TFunc& func, const TArgs&... args)
	{
		TaskEx* pTask = func.CreateTask<Internal::ThreadPoolTask>(args...);
		AddTask(pTask);
	}

	template<typename TFunc, typename ... TArgs> void InvokeEx(const TFunc& func, TaskWaiter& waiter, const TArgs&... args)
	{
		TaskEx* pTask = func.CreateTask<Internal::ThreadPoolTask>(args...);
		pTask->Waiter = &waiter;
		AddTask(pTask);
	}

private:
	void AddTask(TaskEx* pTask);

private:
	bool Construct(const size_t threadsNumber);
	static unsigned int __stdcall ThreadFunction(void* pointer);

private:
	void Loop();
	void Join();
	void Dispose();
	void Step(TaskEx* pTask);
	void DoStep(TaskEx* pTask);

private:
	TaskEx* WaitFor();
	void Release();
	void Release(size_t count);
	void Finalizing();
	void DeleteTasks();

private:
	std::atomic<bool> m_continue;
	CriticalSection m_synchronizer;
	std::atomic<int32_t> m_counter;
	std::vector< std::pair<uint32_t, HANDLE> > m_threads;
	Semaphore m_event;
	TaskEx* m_pFirst;
	TaskEx* m_pLast;
};
