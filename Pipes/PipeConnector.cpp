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
	Pipe pipe = Pipe::OpenReader(m_name, cPipeTimeoutInMs);


	MemoryStream stream;
	pipe.Join();




	PipeChannel* result = new PipeChannel(std::move(pipe));
	return result;
}
