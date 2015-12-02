//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename T> class LiteSingleton
{
public:
	LiteSingleton() : m_instance()
	{
	}
	LiteSingleton(const LiteSingleton& singleton) : m_instance()
	{
		const T* ptr = singleton.m_instance;
		if (nullptr != ptr)
		{
			throw runtime_error("Couldn't copy an initialized lite singleton");
		}
	}
	LiteSingleton& operator = (const LiteSingleton& singleton)
	{
		if (this != &singleton)
		{
			const T* ptr = m_instance;
			if (nullptr != ptr)
			{
				throw runtime_error("Couldn't assign an initialized lite singleton");
			}
			ptr = singleton.m_instance;
			if (nullptr != ptr)
			{
				throw runtime_error("Couldn't copy an initialized lite singleton");
			}
		}
		return *this;
	}
	~LiteSingleton()
	{
		T* ptr = m_instance;
		m_instance = nullptr;
		if (nullptr != ptr)
		{
			delete ptr;
		}
	}

public:
	template<typename ... TArgs> T& GetOrCreate(const TArgs&... args)
	{
		T* result = m_instance;
		if (nullptr != result)
		{
			return *result;
		}

		std::unique_ptr<T> guard(new T(args...));
		T* ptr = guard.get();

		for (;;)
		{
			result = m_instance;
			if (nullptr != result)
			{
				return *result;
			}
			if (m_instance.compare_exchange_strong(result, ptr))
			{
				guard.release();
				return *m_instance;
			}
		}
	}

private:
	std::atomic<T*> m_instance;
};
