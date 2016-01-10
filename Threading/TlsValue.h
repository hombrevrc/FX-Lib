//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<class T> class TlsValue
{
public:
	TlsValue()
	{
		m_tlsIndex = TlsAlloc();
		if (TLS_OUT_OF_INDEXES == m_tlsIndex)
		{
			throw SystemException("TlsValue::TlsValue(): couldn't allocate a new TLS index");
		}
	}

	~TlsValue()
	{
		TlsFree(m_tlsIndex);
	}

public:
	TlsValue(const TlsValue&) = delete;
	TlsValue& operator = (const TlsValue&) = delete;

public:
	void operator = (const T argument)
	{
		void* value = reinterpret_cast<void*>(argument);
		TlsSetValue(m_tlsIndex, value);
	}

	operator T ()
	{
		void* value = TlsGetValue(m_tlsIndex);
		T result = reinterpret_cast<T>(value);
		return result;
	}

private:
	DWORD m_tlsIndex = TLS_OUT_OF_INDEXES;
};
