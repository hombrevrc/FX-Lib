//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "MemoryStream.h"


MemoryStream::MemoryStream()
{
	_data = _buffer;
}

MemoryStream::~MemoryStream()
{
	if (_capacity > sizeof(_buffer))
	{
		delete[] _data;
	}
}

const uint32_t MemoryStream::GetCapacity() const
{
	return _capacity;
}

const uint32_t MemoryStream::GetSize() const
{
	if (_position > _size)
	{
		return _position;
	}
	return _size;
}

const uint32_t MemoryStream::GetPosition() const
{
	return _position;
}

void MemoryStream::Clear()
{
	_size = 0;
	_position = 0;
}

void MemoryStream::SetSize(const uint32_t newSize)
{
	if (newSize > _capacity)
	{
		EnsureCapacity(newSize);
	}
	_size = newSize;
	_position = 0;
}

const uint8_t* MemoryStream::GetData() const
{
	return _data;
}

uint8_t* MemoryStream::GetData()
{
	return _data;
}

void MemoryStream::SetPosition(const uint32_t newPosition)
{
	if (_position > _size)
	{
		_size = _position;
	}
	if (newPosition > _size)
	{
		throw std::runtime_error("Invalid new position");
	}
	_position = newPosition;
}

void MemoryStream::Write(const std::wstring& value)
{
	const uint32_t additionalSize = static_cast<uint32_t>(sizeof(wchar_t) * (1 + value.length()));
	Write(additionalSize, value.c_str());
}

void MemoryStream::Write(const std::string& value)
{
	const uint32_t additionalSize = static_cast<uint32_t>(1 + value.length());
	Write(additionalSize, value.c_str());
}

void MemoryStream::Write(const uint32_t size, const void* pData)
{
	EnsureSize(size);
	uint32_t* pointer = reinterpret_cast<uint32_t*>(_data + _position);
	memcpy(pointer, pData, size);
	_position += size;
}

void MemoryStream::Read(const uint32_t size, void* pData)
{
	const uint32_t newPosition = _position + size;
	if (newPosition > _size)
	{
		throw std::runtime_error("End of stream has been reached");
	}
	memcpy(pData, _data + _position, size);
	_position = newPosition;
}

void MemoryStream::EnsureSize(const uint32_t additionalSize)
{
	uint32_t newSize = GetSize() + additionalSize;
	if (newSize > _capacity)
	{
		EnsureCapacity(newSize);
	}
}

void MemoryStream::EnsureCapacity(const uint32_t totalSize)
{
	assert(totalSize > _capacity);

	if (totalSize >= std::numeric_limits<uint32_t>::max() / 2)
	{
		throw std::bad_alloc();
	}

	uint32_t newCapacity = _capacity;

	do
	{
		newCapacity *= 2;
	} while (newCapacity < totalSize);

	uint8_t* newData = new uint8_t[newCapacity];
	memcpy(newData, _data, GetSize());

	if (_capacity > sizeof(_buffer))
	{
		delete[] _data;
	}

	_data = newData;
	_capacity = newCapacity;
}

namespace
{
	template<typename T> T ReadType(MemoryStream& stream)
	{
		T result = T();
		stream.Read(sizeof(T), &result);
		return result;
	}
}

double ReadDouble(MemoryStream& stream)
{
	return ReadType<double>(stream);
}
