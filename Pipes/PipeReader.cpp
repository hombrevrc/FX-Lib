//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeReader.h"


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
		const uint32_t status = ReadFromBuffer(size, pData + result);
		result += status;

		if (!ReadBuffer())
		{
			break;
		}
	}

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
