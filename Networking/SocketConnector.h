//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SocketsPair.h"

class SocketConnector : public IConnector
{
public:
	SocketConnector(const std::string& address, const uint16_t port);
	SocketConnector(const SocketConnector&) = delete;
	SocketConnector& operator = (const SocketConnector&) = delete;

public:
	virtual ITransport* Connect() override;

private:
	const std::string& m_address;
	const uint16_t m_port;
};