//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeAcceptor.h"
#include "PipeTransport.h"


PipeAcceptor::PipeAcceptor(const std::wstring& name) : m_name(name)
{
	m_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (nullptr == m_event)
	{
		throw SystemException("Couldn't create a new event");
	}
}

PipeAcceptor::~PipeAcceptor()
{
	StopAccepting();
	CloseHandle(m_event);
}

ITransport* PipeAcceptor::Accept()
{
	ResetEvent(m_event);
	Pipe pipe = Pipe::CreateAcceptor(m_name);

	if (pipe.Accept(m_event))
	{
		PipeTransport* result = new PipeTransport(std::move(pipe));
		return result;
	}

	return nullptr;
}

void PipeAcceptor::StopAccepting()
{
	SetEvent(m_event);
}
