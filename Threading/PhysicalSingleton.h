//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename T> class PhysicalSingleton
{
public:
	PhysicalSingleton() : m_instance(nullptr)
	{
	}

	~PhysicalSingleton()
	{
		T* pInstance = m_instance;
		if (nullptr != pInstance)
		{
			m_instance = nullptr;
			delete pInstance;
		}
	}

public:
	PhysicalSingleton(const PhysicalSingleton&) = delete;
	PhysicalSingleton& operator = (const PhysicalSingleton&) = delete;

public:
	template<typename ... TArgs> T& GetOrCreate(const TArgs&... args)
	{
		T* result = m_instance;
		if (nullptr == result)
		{
			std::unique_lock<std::mutex> lock(m_synchronizer);
			result = m_instance;
			if (nullptr == result)
			{
				result = new T(args...);
				m_instance = result;
			}
		}
		return *result;
	}

	T& Get()
	{
		T* result = m_instance;
		if (nullptr == result)
		{
			throw std::runtime_error("Singleton instance is not created");
		}
		return *result;
	}

public:
	T* operator -> ()
	{
		return &GetOrCreate();
	}

private:
	std::mutex m_synchronizer;
	std::atomic<T*> m_instance;
};
