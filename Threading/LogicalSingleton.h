//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SharedExclusiveLock.h"

template<typename T> class DelayDestroyer
{
public:
	DelayDestroyer(const std::shared_ptr<T>& accessor) : m_accessor(accessor)
	{
		std::thread(&DelayDestroyer<T>::ThreadFunction, this);
	}

private:
	static void ThreadFunction(DelayDestroyer* pThis)
	{
		pThis->Loop();
	}

	void Loop()
	{
		std::chrono::milliseconds duration(1024);

		for (; !m_accessor.unique();)
		{
			std::this_thread::sleep_for(duration);
		}
	}

private:
	std::shared_ptr<T> m_accessor;
};

template<typename T> class LogicalSingleton
{
public:
	LogicalSingleton() = default;
	LogicalSingleton(const LogicalSingleton&) = delete;
	LogicalSingleton& operator = (const LogicalSingleton&) = delete;

public:
	std::shared_ptr<T> GetOrCreate()
	{
		{
			SharedLocker lock(m_synchronizer);
			if (m_instance)
			{
				return m_instance;
			}
		}

		ExclusiveLocker lock(m_synchronizer);
		if (!m_instance)
		{
			m_instance = make_shared<T>();
		}
		return m_instance;
	}

	void Recreate()
	{
		std::shared_ptr<T> newInstance = std::make_shared<T>();
		std::shared_ptr<T> oldInstance;
		{
			ExclusiveLocker lock(m_synchronizer);
			oldInstance = m_instance;
			m_instance = newInstance;
		}

		std::chrono::milliseconds duration(1024);

		for (; !oldInstance.unique();)
		{
			std::this_thread::sleep_for(duration);
		}
	}

	void RecreateAndDelayDestroy()
	{
		std::shared_ptr<T> newInstance = std::make_shared<T>();
		std::shared_ptr<T> oldInstance;
		{
			ExclusiveLocker lock(m_synchronizer);
			oldInstance = m_instance;
			m_instance = newInstance;
		}
		new DelayDestroyer<T>(oldInstance);
	}

	void Destroy()
	{
		ExclusiveLocker lock(m_synchronizer);
		m_instance = std::shared_ptr<T>();
	}

private:
	SharedExclusiveLock m_synchronizer;
	std::shared_ptr<T> m_instance;
};
