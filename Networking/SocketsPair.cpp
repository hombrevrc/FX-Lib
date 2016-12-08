//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketsPair.h"


namespace
{
	Socket CreateAcceptor()
	{
		Socket result(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		result.ThrowIfInvalid("Couldn't create a new acceptor");

		int status = result.Bind(0, 0);
		if (SOCKET_ERROR == status)
		{
			result.ThrowIfInvalid("Couldn't bind a socket");
		}

		status = result.Listen();
		if (SOCKET_ERROR == status)
		{
			result.ThrowIfInvalid("Couldn't start a socket listening");
		}

		return result;
	}

	Socket Connect(Socket& client, Socket& acceptor)
	{
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int status = acceptor.GetSockName(reinterpret_cast<sockaddr*>(&addr), &len);
		if (SOCKET_ERROR == status)
		{
			throw SystemException("Couldn't get a socket name");
		}
		status = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
		if (SOCKET_ERROR == status)
		{
			throw SystemException("Couldn't connect a socket");
		}
		status = client.Connect(reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
		if (SOCKET_ERROR == status)
		{
			throw SystemException("Couldn't connect a socket");
		}
		Socket result = acceptor.Accept(nullptr, nullptr);
		result.ThrowIfInvalid("Couldn't accept a new connection");
		return result;
	}

}

SocketsPair::SocketsPair()
{
	Socket acceptor = CreateAcceptor();
	m_primary.Create(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_primary.ThrowIfInvalid("Couldn't create a new primary socket");
	m_secondary = Connect(m_primary, acceptor);
	m_secondary.ThrowIfInvalid("Couldn't create a new secondary socket");
}

SOCKET SocketsPair::GetSocket()
{
	return m_secondary.GetHandle();
}

void SocketsPair::Acquire()
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	if (m_wasReleased)
	{
		char ch = 0;
		const int status = m_secondary.Recv(&ch, sizeof(ch));
		if (1 != status)
		{
			throw SystemException("Couldn't acquire a socket pair");
		}
		m_wasReleased = false;
	}
}

void SocketsPair::Release()
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	char ch = 0;
	const int status = m_primary.Send(&ch, sizeof(ch));
	if (1 != status)
	{
		throw SystemException("Couldn't release a socket pair");
	}
	m_wasReleased = true;
}
