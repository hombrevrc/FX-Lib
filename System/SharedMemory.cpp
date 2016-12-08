//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SharedMemory.h"
#include "SystemException.h"


SharedMemory::SharedMemory(SharedMemory&& arg) :
	_file(arg._file), _size(arg._size), _data(arg._data)
{
	arg.Reset();
}

SharedMemory& SharedMemory::operator=(SharedMemory&& arg)
{
	if (this != &arg)
	{
		Finalize();

		_file = arg._file;
		_size = arg._size;
		_data = arg._data;

		arg.Reset();
	}
	return *this;
}

SharedMemory::SharedMemory(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access /*= SharedMemoryAccess::ReadAndWrite*/)
{
	Construct(name, sizeInBytes, access);
}

SharedMemory::SharedMemory(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access /*= SharedMemoryAccess::ReadAndWrite*/)
{
	Construct(name, sizeInBytes, access);
}

SharedMemory::~SharedMemory()
{
	Finalize();
}

void SharedMemory::Construct(const std::string& name, const uint32_t sizeInBytes, SharedMemoryAccess access /*= SharedMemoryAccess::ReadAndWrite*/)
{
	std::wstring nameTemp = CA2W(name.c_str());
	Construct(nameTemp, sizeInBytes, access);
}

void SharedMemory::Construct(const std::wstring& name, const uint32_t sizeInBytes, SharedMemoryAccess access /*= SharedMemoryAccess::ReadAndWrite*/)
{
	Finalize();

	std::wstring fullName = MakeFullname(name);

	if ((SharedMemoryAccess::Write == access) || (SharedMemoryAccess::ReadAndWrite == access))
	{
		_file = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(sizeInBytes), fullName.c_str());
	}
	else if(SharedMemoryAccess::Read == access)
	{
		_file = OpenFileMapping(FILE_MAP_READ, TRUE, fullName.c_str());
	}
	else
	{
		throw std::runtime_error("Incorrect access");
	}

	if (nullptr == _file)
	{
		throw SystemException("Couldn't create a new file mapping");
	}

	if (SharedMemoryAccess::Read == access)
	{
		_data = reinterpret_cast<uint8_t*>(MapViewOfFile(_file, FILE_MAP_READ, 0, 0, 0));
	}
	else if (SharedMemoryAccess::Write == access)
	{
		_data = reinterpret_cast<uint8_t*>(MapViewOfFile(_file, FILE_MAP_WRITE, 0, 0, 0));
	}
	else
	{
		assert(SharedMemoryAccess::ReadAndWrite == access);
		_data = reinterpret_cast<uint8_t*>(MapViewOfFile(_file, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0));
	}
	if (nullptr == _data)
	{
		CloseHandle(_file);
		_file = nullptr;
		throw SystemException("SharedMemory::Construct(): couldn't create a new view of file");
	}
	_size = sizeInBytes;
}

void SharedMemory::Reset()
{
	_file = nullptr;
	_size = 0;
	_data = nullptr;
}

void SharedMemory::Finalize()
{
	if (nullptr != _data)
	{
		UnmapViewOfFile(_data);
	}
	if (nullptr != _file)
	{
		CloseHandle(_file);
	}
	Reset();
}

std::wstring SharedMemory::MakeFullname(const std::wstring& name)
{
	return name + L"_memory";
}

