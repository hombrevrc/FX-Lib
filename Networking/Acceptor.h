//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SocketEx.h"

class Acceptor
{
public:
	Acceptor(uint16_t port);
	Acceptor(const Acceptor&) = delete;
	Acceptor& operator = (const Acceptor&) = delete;
	~Acceptor();

private:
	bool Construct(uint16_t port, SOCKET& acceptor);

public:
	Socket Accept();
	void Finalize();

private:
	SocketEx m_socket;
};