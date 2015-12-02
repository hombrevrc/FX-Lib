//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketEx.h"


namespace
{
	Socket CreateAcceptor(uint16_t port, int mode, int /*backlog*/)
	{
		Socket result(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		result.ThrowIfInvalid("Couldn't create a new socket");

		int status = result.Bind(port, mode);
		result.ThrowIfInvalid("Couldn't bind a socket");
		
		status = result.Listen();
		result.ThrowIfInvalid("Couldn't listen a socket");

		return result;
	}

	Socket Connect(Socket& client, Socket& acceptor)
	{
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int status = acceptor.GetSockName(reinterpret_cast<sockaddr*>(&addr), &len);
		if (SOCKET_ERROR == status)
		{
			return INVALID_SOCKET;
		}
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		client.Connect(reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
		client.ThrowIfInvalid("Couldn't connect a socket");

		Socket result = acceptor.Accept(nullptr, nullptr);
		result.ThrowIfInvalid("Couldn't accept a new socket");
		return result;
	}
}

SocketEx::SocketEx(SOCKET s) : m_continue(), m_socket(s)
{
	Socket acceptor = CreateAcceptor(0, 0, 1);
	m_client.Create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_client.ThrowIfInvalid("Couldn't create a new socket");
	m_server = Connect(m_client, acceptor);
}

SocketEx::SocketEx(Socket&& s) : m_continue(), m_socket(std::move(s))
{
	Socket acceptor = CreateAcceptor(0, 0, 1);
	m_client.Create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_client.ThrowIfInvalid("Couldn't create a new socket");
	m_server = Connect(m_client, acceptor);
}

SocketEx::SocketEx()
{

}

void SocketEx::Finalize()
{
	if (m_continue)
	{
		m_continue = false;
		char ch = 0;
		m_client.Send(&ch, sizeof(ch));
	}
}
int SocketEx::Send(const char* buffer, const int length)
{
	timeval timeout;
	timeout.tv_sec = std::numeric_limits<long>::max();
	timeout.tv_usec = 0;

	return Send(buffer, length, timeout);
}

Socket SocketEx::Accept()
{
	if (!m_continue)
	{
		return INVALID_SOCKET;
	}
/*
	fd_set set;
	FD_ZERO(&set);
	FD_SET(m_server, &set);
	FD_SET(m_acceptor, &set);
	const int status = select(0, &set, nullptr, nullptr, nullptr);
	if (SOCKET_ERROR == status)
	{
		return INVALID_SOCKET;
	}
	if (FD_ISSET(m_server, &set))
	{
		m_continue = false;
		char ch = 0;
		send(m_server, &ch, sizeof(ch), 0);
		return INVALID_SOCKET;
	}*/


	Socket result = m_socket.Accept(nullptr, nullptr);
	return result;
}

void SocketEx::ShutDown()
{
	m_socket.Shutdown(SD_BOTH);
}
