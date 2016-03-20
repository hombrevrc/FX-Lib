//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once
#include "Pipe.h"

class PipeAcceptor : public IAcceptor
{
public:
	PipeAcceptor(const std::wstring& name);
	PipeAcceptor(const PipeAcceptor&) = delete;
	PipeAcceptor& operator = (const PipeAcceptor&) = delete;
	virtual ~PipeAcceptor();

public:
	virtual ITransport* Accept() override;
	virtual void StopAccepting() override;

private:
	HANDLE m_event;
	const std::wstring m_name;
};