//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SharedMemoryAccess.h"

class SharedMemory
{
public:
	typedef uint32_t size_type;
	typedef uint8_t value_type;
	typedef const uint8_t* const_iterator;
	typedef uint8_t* iterator;

public:
	SharedMemory() = default;
	SharedMemory(SharedMemory&& arg);
	SharedMemory& operator = (SharedMemory&& arg);
	SharedMemory(const SharedMemory& arg) = delete;
	SharedMemory& operator = (const SharedMemory& arg) = delete;
	SharedMemory(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite);
	SharedMemory(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite);
	~SharedMemory();

public:
	void Construct(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite);
	void Construct(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite);
	void Finalize();

private:
	void Reset();

public:
	uint32_t size() const;
	bool empty() const;

public:
	iterator begin();
	iterator end();

public:
	const_iterator begin() const;
	const_iterator end() const;

public:
	template<typename T> const T& at(const size_type offsetInBytes) const
	{
		if (offsetInBytes + sizeof(T) < m_size)
		{
			return *reinterpret_cast<const T*>(m_data + offsetInBytes);
		}
		throw std::out_of_range("SharedMemory<T>::at");
	}

	template<typename T> T& at(const size_type offsetInBytes)
	{
		if (offsetInBytes + sizeof(T) < m_size)
		{
			return *reinterpret_cast<T*>(m_data + offsetInBytes);
		}
		throw std::out_of_range("SharedMemory<T>::at");
	}

private:
	HANDLE m_file = nullptr;
	uint32_t m_size = 0;
	uint8_t* m_data = nullptr;
};
