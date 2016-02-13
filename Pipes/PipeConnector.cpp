//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeConnector.h"
#include "PipeChannel.h"


namespace
{
	//const uint32_t cPipeTimeoutInMs = 5000;
	const uint32_t cPipeTimeoutInMs = INFINITE;
}

PipeConnector::PipeConnector(const std::wstring& name) : m_name(name)
{
}

IChannel* PipeConnector::Connect()
{
	Timeout timeout(PIPE_TIMEOUT);
	Pipe pipe = Pipe::OpenReader(m_name, timeout.ToIntervalInMs());

	PipeReader helper;
	helper.Bind(std::move(pipe));


	std::wstring writingId = helper.ReadWString(timeout);
	std::wstring readingId = helper.ReadWString(timeout);

	Pipe writer = Pipe::OpenWriter(writingId, timeout.ToIntervalInMs());
	Pipe reader = Pipe::OpenReader(readingId, timeout.ToIntervalInMs());

	PipeChannel* result = new PipeChannel(std::move(reader), std::move(writer));
	return result;
}
