//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SharedMemoryAccess.h"

class SharedMemory
{
public:
	SharedMemory() = default;
	SharedMemory(SharedMemory&& arg);
	SharedMemory& operator = (SharedMemory&& arg);
	SharedMemory(const SharedMemory& arg) = delete;
	SharedMemory& operator = (const SharedMemory& arg) = delete;
	SharedMemory(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite, void* security = nullptr);
	SharedMemory(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite, void* security = nullptr);
	~SharedMemory();

public:
	void Construct(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite, void* security = nullptr);
	void Construct(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access = SharedMemoryAccess::ReadAndWrite, void* security = nullptr);
	void Finalize();

private:
	static std::wstring MakeFullname(const std::wstring& name);

private:
	void Reset();

public:
	template<typename T> const T& GetRef(const uint32_t offsetInBytes = 0) const
	{
		assert(offsetInBytes + sizeof(T) <= _size);
		return *reinterpret_cast<const T*>(_data + offsetInBytes);
	}

	template<typename T> T& GetRef(const uint32_t offsetInBytes = 0)
	{
		assert(offsetInBytes + sizeof(T) <= _size);
		return *reinterpret_cast<T*>(_data + offsetInBytes);
	}

public:
	template<typename T> const T* GetPtr(const uint32_t offsetInBytes = 0) const
	{
		assert(offsetInBytes + sizeof(T) <= _size);
		return reinterpret_cast<const T*>(_data + offsetInBytes);
	}

	template<typename T> T* GetPtr(const uint32_t offsetInBytes = 0)
	{
		assert(offsetInBytes + sizeof(T) <= _size);
		return reinterpret_cast<T*>(_data + offsetInBytes);
	}

private:
	HANDLE _file = nullptr;
	uint32_t _size = 0;
	uint8_t* _data = nullptr;
};
