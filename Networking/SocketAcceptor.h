//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SocketsPair.h"

class SocketAcceptor : public IAcceptor
{
public:
	SocketAcceptor(const uint16_t port, const bool localhostOnly);
	SocketAcceptor(const SocketAcceptor&) = delete;
	SocketAcceptor& operator = (const SocketAcceptor&) = delete;

public:
	virtual ITransport* Accept() override;
	virtual void StopAccepting() override;

private:
	Socket m_acceptor;
	SocketsPair m_pair;
};