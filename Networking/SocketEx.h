//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Socket.h"

class SocketEx
{
public:
	SocketEx();
	SocketEx(SOCKET s);
	SocketEx(Socket&& s);
	SocketEx(const SocketEx&) = delete;
	SocketEx& operator = (const SocketEx&) = delete;

public:
	int Send(const char* buffer, const int length);
	int Send(const char* buffer, const int length, const timeval& timeout);
	int Receive(char* buffer, const int length);
	int Receive(char* buffer, const int length, const timeval& timeout);
	Socket Accept();
	void ShutDown();
	void Finalize();

private:
	bool Construct(SOCKET& acceptor);

private:
	std::atomic<bool> m_continue = false;
	Socket m_socket;
	Socket m_server;
	Socket m_client;
};