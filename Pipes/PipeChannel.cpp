//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeChannel.h"


PipeChannel::PipeChannel(Pipe&& pipe) : m_pipe(std::move(pipe))
{
	Construct();
}

PipeChannel::PipeChannel(Pipe&& reader, Pipe&& writer)
{
	Construct();
	m_reader.Bind(std::move(reader));
	m_writer.Bind(std::move(writer));
}

void PipeChannel::Construct()
{
	m_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (nullptr == m_event)
	{
		throw SystemException("Couldn't create a new event");
	}
}

PipeChannel::~PipeChannel()
{
	CloseHandle(m_event);
}

void PipeChannel::Accept()
{
	std::wstring readingId = GenerateGuid();
	std::wstring writingId = GenerateGuid();

	MemoryStream stream;
	stream.Write(readingId);
	stream.Write(writingId);

	const uint32_t size = stream.GetSize();
	const void* pData = stream.GetData();

	const uint32_t writtern = m_pipe.Write(size, pData);
	if (writtern != size)
	{
		throw SystemException("Couldn't write data to a pipe");
	}

	Pipe reader = Pipe::CreateReader(readingId);
	Pipe writer = Pipe::CreateWriter(writingId);

	if (!reader.Accept(m_event, PIPE_TIMEOUT))
	{
		throw SystemException("Couldn't establish a new pipe connection");
	}

	if (!writer.Accept(m_event, PIPE_TIMEOUT))
	{
		throw SystemException("Couldn't establish a new pipe connection");
	}

	m_reader.Bind(std::move(reader));
	m_writer.Bind(std::move(writer));
	m_pipe = Pipe();
}

ChannelState PipeChannel::Join(const bool reading, const bool writing)
{
	if (reading)
	{
		if (writing)
		{
			JoinForReadingOrWriting();
		}
		else
		{
			JoinForReading();
		}
	}
	else if (writing)
	{
		JoinForWriting();
	}

	ChannelState result = ChannelState::None;
	if (m_reader.CanBeRead())
	{
		result = EnumBitwiseOr(result, ChannelState::CanRead);
	}
	if (m_writer.CanBeWritten())
	{
		result = EnumBitwiseOr(result, ChannelState::CanWrite);
	}
	return result;
}

void PipeChannel::JoinForReading()
{
	if (!m_reader.CanBeRead())
	{
		HANDLE handles[] = { m_event, m_reader.GetPipeEventHandle() };
		WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE);
	}
}

void PipeChannel::JoinForWriting()
{
	if (!m_writer.CanBeWritten())
	{
		HANDLE handles[] = { m_event, m_writer.GetPipeEventHandle() };
		WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE);
	}
}

void PipeChannel::JoinForReadingOrWriting()
{
	if (!m_reader.CanBeRead() && !m_writer.CanBeWritten())
	{
		HANDLE handles[] = { m_event, m_reader.GetPipeEventHandle(), m_writer.GetPipeEventHandle() };
		WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE);
	}
}

uint32_t PipeChannel::Write(const uint32_t size, const void* pBuffer)
{
	const uint32_t result = m_writer.Write(size, pBuffer);
	return result;
}

uint32_t PipeChannel::Read(const uint32_t size, void* pBuffer)
{
	const uint32_t result = m_reader.Read(size, pBuffer);
	return result;
}

void PipeChannel::WakeUp()
{
	SetEvent(m_event);
}
