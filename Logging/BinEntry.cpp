//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "BinEntry.h"

#ifdef new
#undef new
#endif


#ifdef FX_LIB_LINUX
namespace
{
    uint32_t GetCurrentThreadId()
    {
        return 0;
    }
}
#endif

BinEntry::BinEntry(const char* type, const uint32_t size) :
	Link(nullptr), Type(type), m_pData(nullptr), m_size(), m_capacity(size)
{
	ThreadId = GetCurrentThreadId();
	if (size > 0)
	{
		m_pData = new(std::nothrow)uint8_t[size];
		if (nullptr == m_pData)
		{
			m_size = 0;
			m_capacity = 0;
		}
	}
}

BinEntry::~BinEntry()
{
	delete[] m_pData;
	m_pData = nullptr;
}

void BinEntry::Write(FormatFunc func, const void* pData, const uint32_t size)
{
	const uint32_t newSize = m_size + sizeof(FormatFunc) + size;
	if ((newSize < m_capacity) || ReallocIfNeeded(newSize))
	{
		FormatFunc* pFunc = reinterpret_cast<FormatFunc*>(m_pData + m_size);
		*pFunc = func;
		memcpy(1 + pFunc, pData, size);
		m_size = newSize;
	}
}

bool BinEntry::ReallocIfNeeded(const uint32_t newSize)
{
	uint32_t newCapacity = sizeof(FormatFunc);
	while (newCapacity < newSize)
	{
		newCapacity *= 2;
	}

	uint8_t* pNewData = new (std::nothrow)uint8_t[newCapacity];
	if (nullptr == pNewData)
	{
		return false;
	}

	memcpy(pNewData, m_pData, m_size);
	delete[] m_pData;
	m_pData = pNewData;
	m_capacity = newCapacity;
	return true;
}

const uint8_t* BinEntry::DoAStringFormat(const uint8_t* pData, std::ostream& stream)
{
	const char* pTypedData = reinterpret_cast<const char*>(pData);
	const size_t length = strlen(pTypedData);
	stream.write(pTypedData, length);
	const uint8_t* result = pData + 1 + length;
	return result;
}

#ifdef FX_LIB_WINDOWS

const uint8_t* BinEntry::DoWStringFormat(const uint8_t* pData, std::ostream& stream)
{
	const wchar_t* pTypedData = reinterpret_cast<const wchar_t*>(pData);
	const size_t length = wcslen(pTypedData);
	stream << CW2A(pTypedData);
	const uint8_t* result = pData + 1 + length;
	return result;
}

#endif

void BinEntry::Format(std::ostream& stream) const
{
	const uint8_t* pCurrent = m_pData;
	const uint8_t* pEnd = m_pData + m_size;

	while (pCurrent < pEnd)
	{
		FormatFunc func = *(reinterpret_cast<const FormatFunc*>(pCurrent));
		pCurrent += sizeof(FormatFunc);
		pCurrent = func(pCurrent, stream);
	}
}

void BinEntry::AcquireCurrentTime()
{
	TimePoint = SystemClock::now();
}
