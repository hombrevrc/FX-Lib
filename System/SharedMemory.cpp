//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SharedMemory.h"
#include "SystemException.h"


SharedMemory::SharedMemory(SharedMemory&& arg) :
	m_file(arg.m_file), m_size(arg.m_size), m_data(arg.m_data)
{
	arg.Reset();
}

SharedMemory& SharedMemory::operator=(SharedMemory&& arg)
{
	if (this != &arg)
	{
		Finalize();

		m_file = arg.m_file;
		m_size = arg.m_size;
		m_data = arg.m_data;

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

	if ((SharedMemoryAccess::Write == access) || (SharedMemoryAccess::ReadAndWrite == access))
	{
		m_file = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(sizeInBytes), name.c_str());
	}
	else if(SharedMemoryAccess::Read == access)
	{
		m_file = OpenFileMapping(FILE_MAP_READ, TRUE, name.c_str());
	}
	else
	{
		throw std::runtime_error("SharedMemory::Construct(): incorrect access");
	}

	if (nullptr == m_file)
	{
		throw SystemException("SharedMemory::Construct(): couldn't create a new file mapping");
	}

	if (SharedMemoryAccess::Read == access)
	{
		m_data = reinterpret_cast<uint8_t*>(MapViewOfFile(m_file, FILE_MAP_READ, 0, 0, 0));
	}
	else if (SharedMemoryAccess::Write == access)
	{
		m_data = reinterpret_cast<uint8_t*>(MapViewOfFile(m_file, FILE_MAP_WRITE, 0, 0, 0));
	}
	else
	{
		assert(SharedMemoryAccess::ReadAndWrite == access);
		m_data = reinterpret_cast<uint8_t*>(MapViewOfFile(m_file, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0));
	}
	if (nullptr == m_data)
	{
		CloseHandle(m_file);
		m_file = nullptr;
		throw SystemException("SharedMemory::Construct(): couldn't create a new view of file");
	}
	m_size = sizeInBytes;
}

void SharedMemory::Reset()
{
	m_file = nullptr;
	m_size = 0;
	m_data = nullptr;
}

void SharedMemory::Finalize()
{
	if (nullptr != m_data)
	{
		UnmapViewOfFile(m_data);
	}
	if (nullptr != m_file)
	{
		CloseHandle(m_file);
	}
	Reset();
}

SharedMemory::iterator SharedMemory::begin()
{
	assert(m_data);
	return m_data;
}

SharedMemory::const_iterator SharedMemory::begin() const
{
	assert(m_data);
	return m_data;
}

SharedMemory::iterator SharedMemory::end()
{
	assert(m_data);
	return m_data + m_size;
}

SharedMemory::const_iterator SharedMemory::end() const
{
	assert(m_data);
	return m_data + m_size;
}

uint32_t SharedMemory::size() const
{
	return m_size;
}

bool SharedMemory::empty() const
{
	if (0 == m_size)
	{
		return true;
	}
	return false;
}
