//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Monitor
{
public:
	inline Monitor() : m_generation()
	{
	}

public:
	void StartWriting()
	{
		++m_generation;
	}

	void FinishWriting()
	{
		++m_generation;
	}

public:
	uint64_t GetGeneration() const
	{
		return m_generation;
	}

private:
	volatile uint64_t m_generation;
};

class WriteLocker
{
public:
	WriteLocker(Monitor& monitor) : m_monitor(monitor)
	{
		monitor.StartWriting();
	}
	~WriteLocker()
	{
		m_monitor.FinishWriting();
	}
public:
	WriteLocker(const WriteLocker&) = delete;
	WriteLocker& operator = (const WriteLocker&) = delete;

private:
	Monitor& m_monitor;
};

class ReadLocker
{
public:
	ReadLocker(const Monitor& monitor, const uint32_t attempts = 1024 * 1024) : m_monitor(monitor), m_attempts(attempts)
	{
	}

public:
	ReadLocker(const ReadLocker&) = delete;
	ReadLocker& operator = (const ReadLocker&) = delete;

public:
	bool StartReading()
	{
		for (; m_attempts > 0; --m_attempts)
		{
			m_generation = m_monitor.GetGeneration();
			if (0 == (m_generation & 1))
			{
				return true;
			}
			SwitchToThread();
		}
		return false;
	}

	bool FinishReading()
	{
		const uint64_t genration = m_monitor.GetGeneration();
		const bool result = (genration == m_generation);
		return result;
	}

private:
	const Monitor& m_monitor;
	uint32_t m_attempts;
	uint64_t m_generation;
};
