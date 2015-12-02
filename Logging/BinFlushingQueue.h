//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once


class BinLogger;

class BinFlushingQueue
{
public:
	BinFlushingQueue();
	BinFlushingQueue(const BinFlushingQueue&) = delete;
	BinFlushingQueue& operator = (const BinFlushingQueue&) = delete;
	~BinFlushingQueue();

public:
	void Add(BinLogger* pLogger);
	void Remove(BinLogger* pLogger);

private:
	void Loop();
	void Flush();

private:
	std::atomic_bool m_continue;
	Semaphore m_event;

private:
	CriticalSection m_synchronizer;
	ThreadPool m_thread;
	std::vector<BinLogger*> m_loggers;
};
