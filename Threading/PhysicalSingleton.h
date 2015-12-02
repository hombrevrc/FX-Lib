//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "CriticalSection.h"

template<typename T> class PhysicalSingleton
{
public:
	PhysicalSingleton() : m_instance(nullptr)
	{
	}

	~PhysicalSingleton()
	{
		if (nullptr != m_instance)
		{
			delete m_instance;
		}
	}

public:
	PhysicalSingleton(const PhysicalSingleton&) = delete;
	PhysicalSingleton& operator = (const PhysicalSingleton&) = delete;

public:
	T& GetOrCreate()
	{
		if (nullptr == m_instance)
		{
			CsLocker lock(m_synchronizer);
			if (nullptr == m_instance)
			{
				m_instance = new T();
			}
		}
		return const_cast<T&>(*m_instance);
	}

public:
	T* operator -> ()
	{
		return &GetOrCreate();
	}

private:
	CriticalSection m_synchronizer;
	volatile T* m_instance;
};
