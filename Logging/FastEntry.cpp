//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "FastEntry.h"
#include "IFastHelper.h"


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

FastEntry::FastEntry(const uint32_t size) :
	Link(nullptr), Type(nullptr), m_pData(nullptr), m_size(), m_capacity(size)
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

FastEntry::FastEntry(FastEntry& entry) : 
	m_pData(entry.m_pData), m_size(entry.m_size)
{
	entry.m_pData = nullptr;
	entry.m_size = 0;
}

FastEntry::~FastEntry()
{
	uint8_t* pCurrent = m_pData;
	uint8_t* pEnd = m_pData + m_size;

	while (pCurrent < pEnd)
	{
		IFastHelper* pHelper = reinterpret_cast<IFastHelper*>(pCurrent);
		pHelper->Delete();
		pCurrent += pHelper->SizeOf();
	}

	delete[] m_pData;
	m_pData = nullptr;
}


void FastEntry::Format(std::ostream& stream) const
{
	const uint8_t* pCurrent = m_pData;
	const uint8_t* pEnd = m_pData + m_size;

	while (pCurrent < pEnd)
	{
		const IFastHelper* pHelper = reinterpret_cast<const IFastHelper*>(pCurrent);
		pHelper->Format(stream);
		pCurrent += pHelper->SizeOf();
	}
}

uint32_t FastEntry::GetSize() const
{
	return m_size;
}

void FastEntry::AcquireCurrentTime()
{
	TimePoint = SystemClock::now();
}

uint8_t* FastEntry::Allocate(const uint32_t size)
{
	const uint32_t newSize = m_size + size;
	if (newSize <= m_capacity)
	{
		uint8_t* result = m_pData + m_size;
		m_size = newSize;
		return result;
	}

	FastEntry guard(*this);

	uint32_t newCapacity = sizeof(void*);
	while (newCapacity < newSize)
	{
		newCapacity *= 2;
	}

	m_pData = new (std::nothrow)uint8_t[newCapacity];
	if (nullptr == m_pData)
	{
		return nullptr;
	}

	uint8_t* pCurrent = guard.m_pData;
	uint8_t* pEnd = guard.m_pData + guard.m_size;
	uint8_t* result = m_pData;

	while (pCurrent < pEnd)
	{
		IFastHelper* pHelper = reinterpret_cast<IFastHelper*>(pCurrent);
		pHelper->MoveTo(result);
		const uint32_t sizeOf = pHelper->SizeOf();

		pCurrent += sizeOf;
		result += sizeOf;
	}

	m_size = newSize;
	return result;
}

void FastEntry::Free(const uint32_t size)
{
	assert(m_size >= size);
	m_size -= size;
}
