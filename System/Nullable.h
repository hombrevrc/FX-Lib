//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename T> class Nullable
{
public:
	Nullable() : m_hasValue(false)
	{
	}

	Nullable(const T& value)
	{
		Construct(value);
	}

	Nullable(const Nullable& value) : m_hasValue(value.m_hasValue)
	{
		if (m_hasValue)
		{
			Construct(value.GetValueIml());
		}
	}

	Nullable& operator = (const Nullable& value)
	{
		if (this != &value)
		{
			Reset();
			if (value.m_hasValue)
			{
				Construct(value.GetValueIml());
			}
		}
		return *this;
	}

	~Nullable()
	{
		Reset();
	}

public:
	bool HasValue() const
	{
		return m_hasValue;
	}

	const T& GetValue() const
	{
		if (!m_hasValue)
		{
			throw std::runtime_error("Nullable<T>::GetValue(): nullable object does not have object");
		}
		return GetValueIml();
	}

	T& GetValue()
	{
		const Nullable* pThis = this;
		const T& value = pThis->GetValue(); // call const version of GetValue();
		T& result = const_cast<T&>(value);
		return result;
	}

	void Reset()
	{
		if (m_hasValue)
		{
			void* memory = m_buffer;
			T* pInstance = reinterpret_cast<T*>(memory);
			pInstance;
			pInstance->~T();
			m_hasValue = false;
		}
	}

private:
	void Construct(const T& value)
	{
		void* memory = m_buffer;
		new (memory)T(value);
		m_hasValue = true;
	}

	const T& GetValueIml() const
	{
		const void* memory = m_buffer;
		const T* pInstance = reinterpret_cast<const T*>(memory);
		const T& result = *pInstance;
		return result;
	}

private:
	uint8_t m_buffer[sizeof(T)];
	bool m_hasValue;
};
