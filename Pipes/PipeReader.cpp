//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeReader.h"


PipeReader::PipeReader()
{
	m_stream.SetSize(0);
}

bool PipeReader::CanBeRead()
{
	return ReadBuffer();
}

uint32_t PipeReader::Read(const uint32_t size, void* pBuffer)
{
	uint8_t* pData = reinterpret_cast<uint8_t*>(pBuffer);

	uint32_t result = ReadFromBuffer(size, pData);

	for (; result < size;)
	{
		if (!ReadBuffer())
		{
			break;
		}

		const uint32_t status = ReadFromBuffer(size, pData + result);
		result += status;
	}

	return result;
}

void PipeReader::Read(const Timeout& timeout, const uint32_t size, void* pBuffer)
{
	uint8_t* pCurrent = reinterpret_cast<uint8_t*>(pBuffer);
	uint8_t* const pEnd = pCurrent + size;

	for (;;)
	{
		const uint32_t status = Read(static_cast<uint32_t>(pEnd - pCurrent), pCurrent);
		pCurrent += status;
		if (pCurrent == pEnd)
		{
			break;
		}

		const uint32_t timeoutInMs = timeout.ToIntervalInMs();
		if (0 == timeoutInMs)
		{
			throw TimeoutException();
		}

		HANDLE event = m_pipe.GetEventHandle();
		WaitForSingleObject(event, timeoutInMs);
	}
}

std::wstring PipeReader::ReadWString(const Timeout& timeout)
{
	uint32_t size = 0;
	Read(timeout, sizeof(size), &size);
	MemoryStream stream;
	stream.SetSize(size * sizeof(wchar_t));
	wchar_t* pData = reinterpret_cast<wchar_t*>(stream.GetData());
	Read(timeout, size * sizeof(wchar_t), pData);

	std::wstring result(pData, pData + size);
	return result;
}

uint32_t PipeReader::ReadFromBuffer(const uint32_t size, void* pBuffer)
{
	uint32_t result = std::min(size, m_stream.GetSize() - m_stream.GetPosition());
	m_stream.Read(result, pBuffer);
	return result;
}

bool PipeReader::ReadBuffer()
{
	if (m_pendingIO)
	{
		assert(0 == m_stream.GetPosition());
		uint32_t transferredBytes = 0;
		if (!m_pipe.GetOverlappedResult(transferredBytes))
		{
			return false;
		}
		if (0 == transferredBytes)
		{
			return false;
		}
		m_stream.SetSize(transferredBytes);
		m_pendingIO = false;
		return true;
	}
	else
	{
		const uint32_t position = m_stream.GetPosition();
		const uint32_t size = m_stream.GetSize();
		if (position < size)
		{
			return true;
		}
		assert(m_stream.GetPosition() == m_stream.GetSize());
		m_stream.SetSize(PIPE_BUFFER_SIZE);
		const uint32_t status = m_pipe.Read(m_stream.GetSize(), m_stream.GetData());
		if (status > 0)
		{
			m_stream.SetSize(status);
			return true;
		}
		m_pendingIO = true;
		return false;
	}
}
