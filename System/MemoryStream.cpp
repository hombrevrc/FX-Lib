//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "MemoryStream.h"


MemoryStream::MemoryStream()
{
	m_data = m_buffer;
}

MemoryStream::~MemoryStream()
{
	if (m_capacity > sizeof(m_buffer))
	{
		delete[] m_data;
	}
}

const uint32_t MemoryStream::GetCapacity() const
{
	return m_capacity;
}

const uint32_t MemoryStream::GetSize() const
{
	if (m_position > m_size)
	{
		return m_position;
	}
	return m_size;
}

const uint32_t MemoryStream::GetPosition() const
{
	return m_position;
}

void MemoryStream::SetSize(const uint32_t newSize)
{
	if (newSize > m_capacity)
	{
		EnsureCapacity(newSize);
	}
	m_size = newSize;
	m_position = 0;
}

const uint8_t* MemoryStream::GetData() const
{
	return m_data;
}

uint8_t* MemoryStream::GetData()
{
	return m_data;
}

void MemoryStream::SetPosition(const uint32_t newPosition)
{
	if (newPosition >= m_size)
	{
		throw std::runtime_error("Invalid new position");
	}
	m_position = newPosition;
}

void MemoryStream::Write(const std::wstring& value)
{
	const uint32_t additionalSize = static_cast<uint32_t>(sizeof(uint32_t) + sizeof(wchar_t) * value.length());
	EnsureSize(additionalSize);

	uint32_t* pointer = reinterpret_cast<uint32_t*>(m_data + m_position);
	*pointer = static_cast<uint32_t>(value.length());
	++pointer;
	memcpy(pointer, value.data(), sizeof(wchar_t) * value.length());
	m_position += additionalSize;
}

void MemoryStream::Write(const std::string& value)
{
	const uint32_t additionalSize = static_cast<uint32_t>(sizeof(uint32_t) + value.length());
	EnsureSize(additionalSize);

	uint32_t* pointer = reinterpret_cast<uint32_t*>(m_data + m_position);
	*pointer = static_cast<uint32_t>(value.length());
	++pointer;
	memcpy(pointer, value.data(), value.length());
	m_position += additionalSize;
}

void MemoryStream::Write(const uint32_t size, const void* pData)
{
	EnsureSize(size);
	uint32_t* pointer = reinterpret_cast<uint32_t*>(m_data + m_position);
	memcpy(pointer, pData, size);
	m_position += size;
}

void MemoryStream::Read(const uint32_t size, void* pData)
{
	const uint32_t newPosition = m_position + size;
	if (newPosition > m_size)
	{
		throw std::runtime_error("End of stream has been reached");
	}
	memcpy(pData, m_data + m_position, size);
	m_position = newPosition;
}

void MemoryStream::EnsureSize(const uint32_t additionalSize)
{
	uint32_t newSize = GetSize() + additionalSize;
	if (newSize > m_capacity)
	{
		EnsureCapacity(newSize);
	}
}

void MemoryStream::EnsureCapacity(const uint32_t totalSize)
{
	assert(totalSize > m_capacity);
	uint32_t newCapacity = m_capacity;

	do
	{
		newCapacity *= 2;
	} while (newCapacity < totalSize);

	uint8_t* newData = new uint8_t[newCapacity];
	memcpy(newData, m_data, GetSize());

	if (m_capacity > sizeof(m_buffer))
	{
		delete[] m_data;
	}

	m_data = newData;
	m_capacity = newCapacity;
}
