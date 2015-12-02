//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Semaphore.h"
#include "CriticalSection.h"

namespace Internal
{
	class TimerTask
	{
	public:
		uint64_t PeriodInMs;
		std::multimap<uint64_t, TaskBase<Internal::TimerTask>* >::iterator* Pointer;

	public:
		TimerTask() : PeriodInMs(), Pointer(nullptr), m_counter(1)
		{
		}

		virtual ~TimerTask()
		{
		}

	public:
		void Acquire()
		{
			m_counter.fetch_add(1);
		}

		void Release()
		{
			const uint32_t value = m_counter.fetch_sub(1);
			if (1 == value)
			{
				delete this;
			}
		}

	private:
		std::atomic<uint32_t> m_counter;
	};
}

class Timer
{
private:
	typedef TaskBase<Internal::TimerTask> TaskEx;

public:
	Timer();
	Timer(const Timer&) = delete;
	Timer& operator = (const Timer&) = delete;
	~Timer();

public:
	template<typename TFunc, typename ... TArgs>
	void StartTimer(const std::string& name, const uint64_t offsetInMs, const uint64_t periodInMs, const TFunc& func, const TArgs&... args)
	{
		TaskEx* pTask = func.CreateTask<Internal::TimerTask>(args...);
		auto_ptr<TaskEx> task(pTask);
		StartTimer(name, offsetInMs, periodInMs, pTask);
		task.release();
	}

	bool StopTimer(const std::string& name);

public:
	void StartTimer(const std::string& name, const uint64_t offsetInMs, const uint64_t periodInMs, TaskEx* pTask);

private:
	static void ThreadFunction(Timer* pTimer);
	void MainLoop();
	uint32_t Loop();
	uint32_t Step();
	void DoStep(TaskEx* pTask);

private:
	volatile bool m_continue;
	CriticalSection m_synchronizer;
	Semaphore m_event;
	std::thread m_thread;

private:
	typedef std::multimap<uint64_t, TaskEx*>::iterator iterator;

private:
	std::multimap<uint64_t, TaskEx*> m_timers;
	std::unordered_map<std::string, iterator> m_nameToTimer;
};
