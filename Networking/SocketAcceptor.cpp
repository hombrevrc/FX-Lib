//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketAcceptor.h"
#include "SocketTransport.h"


namespace
{
	Socket CreateAcceptor(const uint16_t port, const bool localhostOnly)
	{
		Socket result(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (result.IsInvalid())
		{
			throw SystemException("Couldn't create a new socket");
		}
		int mode = INADDR_ANY;
		if (localhostOnly)
		{
			int status = inet_pton(AF_INET, "127.0.0.1", &mode);
			if (SOCKET_ERROR == status)
			{
				throw SystemException("Couldn't get IP address from 127.0.0.1");
			}
		}

		int status = result.Bind(port, mode);
		if (SOCKET_ERROR == status)
		{
			throw SystemException("Couldn't bind socket");
		}

		status = result.Listen(SOMAXCONN);
		if (SOCKET_ERROR == status)
		{
			throw SystemException("Couldn't start a socket listening");
		}

		return result;
	}
}

SocketAcceptor::SocketAcceptor(const uint16_t port, const bool localhostOnly)
{
	m_acceptor = CreateAcceptor(port, localhostOnly);
}

ITransport* SocketAcceptor::Accept()
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(m_pair.GetSocket(), &set);
	FD_SET(m_acceptor.GetHandle(), &set);

	const int status = select(0, &set, nullptr, nullptr, nullptr);
	if (SOCKET_ERROR == status)
	{
		throw SystemException("Couldn't accept a new socket");
	}

	if (FD_ISSET(m_pair.GetSocket(), &set))
	{
		m_pair.Acquire();
		return nullptr;
	}

	Socket socket = m_acceptor.Accept(nullptr, nullptr);
	ITransport* result = new SocketTransport(std::move(socket));
	return result;
}

void SocketAcceptor::StopAccepting()
{
	m_pair.Release();
}
