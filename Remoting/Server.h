//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IAcceptor.h"

class Server
{
public:
	Server(IAcceptor& acceptor);
	Server(const Server&) = delete;
	Server& operator = (const Server&) = delete;
	~Server();

public:
	void Start();
	void Stop();

private:
	void Loop();

private:
	std::mutex m_mutex;
	IAcceptor& m_acceptor;

private:
	ThreadPool m_thread;
};