//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Socket.h"


Socket::Socket()
{
}

Socket::Socket(SOCKET s)
{
	m_socket = s;
}

Socket::Socket(int af, int type, int protocol)
{
	Create(af, type, protocol);
}

Socket::Socket(Socket&& s)
{
	m_socket = s.m_socket;
	s.m_socket = INVALID_SOCKET;
}

Socket& Socket::operator = (Socket&& s)
{
	if (this != &s)
	{
		Close();
		m_socket = s.m_socket;
		s.m_socket = INVALID_SOCKET;
	}
	return *this;
}

Socket::~Socket()
{
	Close();
}

bool Socket::IsInvalid() const
{
	const bool result = (INVALID_SOCKET == m_socket);
	return result;
}

void Socket::ThrowIfInvalid(const char* message) const
{
	if (IsInvalid())
	{
		const DWORD error = WSAGetLastError();
		std::stringstream stream;
		stream << message;
		stream << "; WSAGetLastError() = " << error;
		std::string st = stream.str();
		throw std::runtime_error(st);
	}
}

bool Socket::Create(int af, int type, int protocol)
{
	Close();
	m_socket = socket(af, type, protocol);
	const bool result = (INVALID_SOCKET == m_socket);
	return result;
}

void Socket::Close()
{
	if (INVALID_SOCKET != m_socket)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

int Socket::Bind(uint16_t port, int mode)
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = mode;
	service.sin_port = htons(port);

	const int result = bind(m_socket, (SOCKADDR*)&service, sizeof(service));
	return result;
}

int Socket::Listen(int backlog /* = SOMAXCONN */)
{
	const int result = listen(m_socket, backlog);
	return result;
}

Socket Socket::Accept(sockaddr* addr, int* addrlen)
{
	SOCKET result = accept(m_socket, addr, addrlen);
	return result;
}

int Socket::Connect(const sockaddr* name, int namelen)
{
	const int result = connect(m_socket, name, namelen);
	return result;
}

int Socket::Shutdown(int how /* = SD_BOTH */)
{
	const int result = shutdown(m_socket, how);
	return result;
}

int Socket::Send(const char * buf, int len, int flags /* = 0 */)
{
	const int result = send(m_socket, buf, len, flags);
	return result;
}

int Socket::Recv(char* buf, int len, int flags /* = 0 */)
{
	const int result = recv(m_socket, buf, len, flags);
	return result;
}

int Socket::GetSockOpt(int level, int optname, char* optval, int* optlen)
{
	const int result = getsockopt(m_socket, level, optname, optval, optlen);
	return result;
}

int Socket::GetSockName(sockaddr* name, int* namelen)
{
	const int result = getsockname(m_socket, name, namelen);
	return result;
}
