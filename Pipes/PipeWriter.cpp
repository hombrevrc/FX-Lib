//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeWriter.h"


PipeWriter::PipeWriter()
{
	m_stream.SetSize(0);
}

bool PipeWriter::CanBeWritten()
{
	if (m_pendingIO)
	{
		WriteBuffer();
	}
	const bool result = !m_pendingIO;
	return result;
}

uint32_t PipeWriter::Write(const uint32_t size, const void* pBuffer)
{
	if (!WriteBuffer())
	{
		return 0;
	}

	m_stream.Write(size, pBuffer);
	const uint32_t result = size;
	WriteBuffer();
	return result;
}

bool PipeWriter::WriteBuffer()
{
	if (m_pendingIO)
	{
		uint32_t transferredBytes = 0;
		if (!m_pipe.GetOverlappedResult(transferredBytes))
		{
			return false;
		}
		if (0 != transferredBytes)
		{
			return false;
		}

		const uint32_t position = m_stream.GetPosition();
		const uint32_t size = m_stream.GetSize();
		const uint32_t newPosition = position + transferredBytes;
		if (newPosition < size)
		{
			m_stream.SetPosition(newPosition);
			return false;
		}

		m_pendingIO = false;
		m_stream.SetSize(0);
		return true;
	}
	else
	{
		m_pipe.Write(m_stream);

		const uint32_t position = m_stream.GetPosition();
		const uint32_t size = m_stream.GetSize();
		if (position < size)
		{
			m_pendingIO = true;
			return false;
		}
		else
		{
			m_stream.SetSize(0);
			return true;
		}
	}
}
