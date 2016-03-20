//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Socket.h"
#include "SocketsPair.h"

class SocketTransport : public ITransport
{
public:
	SocketTransport(Socket&& socket);
	SocketTransport(const SocketTransport&) = delete;
	SocketTransport& operator = (const SocketTransport&) = delete;
	~SocketTransport();

public:
	virtual void Accept() override;
	virtual TransportState Join(const bool reading, const bool writing) override;
	virtual uint32_t Write(const uint32_t size, const void* buffer) override;
	virtual uint32_t Read(const uint32_t size, void* buffer) override;
	virtual void WakeUp() override;

private:
	Socket m_socket;
	SocketsPair m_pair;
};
