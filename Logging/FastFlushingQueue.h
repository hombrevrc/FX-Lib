//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class FastLogger;

class FastFlushingQueue
{
public:
	FastFlushingQueue();
	FastFlushingQueue(const FastFlushingQueue&) = delete;
	FastFlushingQueue& operator = (const FastFlushingQueue&) = delete;
	~FastFlushingQueue();

public:
	void Add(FastLogger* pLogger);
	void Remove(FastLogger* pLogger);
	void Stop();

private:
	void Loop();
	void Flush();
	void SwithchToSynchronousMode();

private:
	std::atomic_bool m_continue;
	StdSemaphore m_event;

private:
	std::mutex m_synchronizer;
	std::thread m_thread;
	std::vector<FastLogger*> m_loggers;
};
