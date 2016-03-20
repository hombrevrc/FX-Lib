//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketConnector.h"
#include "SocketTransport.h"


namespace
{
	bool ResolveConnectionString(const std::string& address, const uint16_t port, sockaddr& addr)
	{
		char buffer[1 + 8 * sizeof(int)] = "";
		_itoa_s(port, buffer, 10);

		addrinfo hints;
		ZeroMemory(&hints, sizeof(hints));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo* pList = nullptr;
		const int status = getaddrinfo(address.c_str(), buffer, &hints, &pList);
		if ((0 != status) || (nullptr == pList->ai_addr))
		{
			return false;
		}
		addr = *(pList->ai_addr);
		freeaddrinfo(pList);
		return true;
	}
}

SocketConnector::SocketConnector(const std::string& address, const uint16_t port) :
	m_address(address), m_port(port)
{
}

ITransport* SocketConnector::Connect()
{
	Socket socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket.IsInvalid())
	{
		throw SystemException("Couldn't create a new socket");
	}

	sockaddr addr;
	ZeroMemory(&addr, sizeof(addr));
	if (!ResolveConnectionString(m_address, m_port, addr))
	{
		throw SystemException("Couldn't resolve connection string");
	}

	const int status = socket.Connect(&addr, sizeof(addr));
	if (SOCKET_ERROR == status)
	{
		throw SystemException("Couldn't connect a socket to a remote server");
	}

	ITransport* result = new SocketTransport(std::move(socket));
	return result;
}
