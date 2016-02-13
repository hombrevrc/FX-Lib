//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Pipe.h"
#include "Constants.h"


using namespace std::chrono;

namespace
{
	const std::wstring cPipePrefix = L"\\\\.\\pipe\\";
	const DWORD cDuplexMode = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
	const DWORD cReaderMode = PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED;
	const DWORD cWriterMode = PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED;
	const DWORD cPipeMode = PIPE_TYPE_BYTE | PIPE_WAIT;
}

Pipe::Pipe()
{
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));
	m_overlapped.hEvent = CreateEvent(nullptr, TRUE, TRUE, nullptr);
	if (nullptr == m_overlapped.hEvent)
	{
		throw SystemException("Couldn't create a new event");
	}
}

Pipe::Pipe(Pipe&& pipe)
{
	AcquireFrom(pipe);
}

Pipe& Pipe::operator = (Pipe&& pipe)
{
	if (this != &pipe)
	{
		Finalize();
		AcquireFrom(pipe);
	}
	return *this;
}

Pipe::~Pipe()
{
	Finalize();
}

void Pipe::AcquireFrom(Pipe& pipe)
{
	m_pipe = pipe.m_pipe;
	memcpy(&m_overlapped, &pipe.m_overlapped, sizeof(m_overlapped));

	pipe.m_pipe = INVALID_HANDLE_VALUE;
	ZeroMemory(&pipe.m_overlapped, sizeof(m_overlapped));
}

void Pipe::Finalize()
{
	if (INVALID_HANDLE_VALUE != m_pipe)
	{
		CloseHandle(m_pipe);
		m_pipe = INVALID_HANDLE_VALUE;
	}

	if (nullptr != m_overlapped.hEvent)
	{
		CloseHandle(m_overlapped.hEvent);
		m_overlapped.hEvent = nullptr;
	}
	ZeroMemory(&m_overlapped, sizeof(m_overlapped));
}

Pipe Pipe::CreateWriter(const std::wstring& name)
{
	std::wstring st = cPipePrefix + name;
	HANDLE pipe = CreateNamedPipe(st.c_str(), cWriterMode, cPipeMode, 1, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, INFINITE, nullptr);
	if (INVALID_HANDLE_VALUE == pipe)
	{
		throw SystemException("Couldn't create a new named pipe");
	}

	Pipe result;
	result.m_pipe = pipe;
	return result;
}

Pipe Pipe::CreateReader(const std::wstring& name)
{
	std::wstring st = cPipePrefix + name;
	HANDLE pipe = CreateNamedPipe(st.c_str(), cReaderMode, cPipeMode, 1, PIPE_BUFFER_SIZE, PIPE_BUFFER_SIZE, INFINITE, nullptr);
	if (INVALID_HANDLE_VALUE == pipe)
	{
		throw SystemException("Couldn't create a new named pipe");
	}

	Pipe result;
	result.m_pipe = pipe;
	return result;
}

Pipe Pipe::Open(const std::wstring& name, const DWORD mode, const uint32_t timeoutInMs)
{
	std::wstring st = cPipePrefix + name;
	steady_clock::time_point start = steady_clock::now();

	for (;;)
	{
		HANDLE pipe = CreateFile(st.c_str(), mode, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
		if (INVALID_HANDLE_VALUE != pipe)
		{
			Pipe result;
			result.m_pipe = pipe;
			return result;
		}

		steady_clock::time_point time = steady_clock::now();
		milliseconds interval = duration_cast<milliseconds>(time - start);

		if (interval.count() >= timeoutInMs)
		{
			throw TimeoutException("Couldn't open a pipe");
		}

		const DWORD timeout = static_cast<DWORD>(timeoutInMs - interval.count());
		if (!WaitNamedPipe(st.c_str(), timeout))
		{
			std::this_thread::sleep_for(milliseconds(16));
		}
	}
}

Pipe Pipe::OpenReader(const std::wstring& name, const uint32_t timeoutInMs)
{
	return Open(name, GENERIC_READ, timeoutInMs);
}

Pipe Pipe::OpenWriter(const std::wstring& name, const uint32_t timeoutInMs)
{
	return Open(name, GENERIC_WRITE, timeoutInMs);
}

HANDLE Pipe::GetEventHandle()
{
	return m_overlapped.hEvent;
}

bool Pipe::Accept(HANDLE signaledEvent)
{
	const bool result = Accept(signaledEvent, INFINITE);
	return result;
}

bool Pipe::Accept(HANDLE signaledEvent, const uint32_t timeoutInMs)
{
	BOOL connectStatus = ConnectNamedPipe(m_pipe, &m_overlapped);
	if (connectStatus)
	{
		throw SystemException("Couldn't connect a named pipe");
	}

	const DWORD errorCode = GetLastError();
	if (ERROR_PIPE_CONNECTED == errorCode)
	{
		return true;
	}
	if (ERROR_IO_PENDING != errorCode)
	{
		throw SystemException("Couldn't connect a named pipe");
	}

	HANDLE handles[] = { m_overlapped.hEvent, signaledEvent };
	const DWORD waitingResult = WaitForMultipleObjects(_countof(handles), handles, FALSE, timeoutInMs);
	if (WAIT_OBJECT_0 + 1 == waitingResult)
	{
		return false;
	}
	if (WAIT_OBJECT_0 != waitingResult)
	{
		throw SystemException("Couldn't connect a named pipe");
	}

	if (!GetOverlappedResult())
	{
		throw SystemException("Couldn't connect a named pipe");
	}
	return true;
}

uint32_t Pipe::Write(const uint32_t size, const void* pBuffer)
{
	DWORD result = 0;
	if (0 == size)
	{
		return result;
	}

	const BOOL status = WriteFile(m_pipe, pBuffer, size, &result, &m_overlapped);

	if (!status)
	{
		const DWORD error = GetLastError();
		if (ERROR_IO_PENDING != error)
		{
			throw SystemException("Couldn't write to a pipe");
		}
	}

	return result;
}

void Pipe::Write(MemoryStream& stream)
{
	const uint32_t position = stream.GetPosition();
	const uint32_t size = stream.GetSize();

	const uint8_t* pData = stream.GetData() + position;
	const uint32_t status = Write(size - position, pData);
	stream.SetPosition(position + status);
}

uint32_t Pipe::Read(const uint32_t size, void* pBuffer)
{
	DWORD result = 0;
	const BOOL status = ReadFile(m_pipe, pBuffer, size, &result, &m_overlapped);

	if (!status)
	{
		const DWORD error = GetLastError();
		if (ERROR_IO_PENDING != error)
		{
			throw SystemException("Couldn't read from a pipe");
		}
	}

	return result;
}

bool Pipe::GetOverlappedResult()
{
	uint32_t transferredBytes = 0;
	const bool result = GetOverlappedResult(transferredBytes);
	return result;
}

bool Pipe::GetOverlappedResult(uint32_t& transferredBytes)
{
	DWORD numberOfBytesTransferred = 0;
	const BOOL status = ::GetOverlappedResult(m_pipe, &m_overlapped, &numberOfBytesTransferred, FALSE);
	transferredBytes = numberOfBytesTransferred;

	if (status)
	{
		return true;
	}
	return false;
}

void Pipe::Join()
{
	WaitForSingleObject(m_overlapped.hEvent, INFINITE);
}

void Pipe::WakeUp()
{
	SetEvent(m_overlapped.hEvent);
}
