//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketTransport.h"


SocketTransport::SocketTransport(Socket&& socket) : m_socket(std::move(socket))
{
	m_socket.EnableNonBlockingMode();
	m_socket.EnableKeepAlive();
}

SocketTransport::~SocketTransport()
{
}

void SocketTransport::Accept()
{
}

TransportState SocketTransport::Join(const bool reading, const bool writing)
{
	fd_set rSet;
	rSet.fd_count = 1;
	rSet.fd_array[0] = m_pair.GetSocket();

	if (reading)
	{
		rSet.fd_array[rSet.fd_count++] = m_socket.GetHandle();
	}

	fd_set wSet;
	wSet.fd_count = 0;
	if (writing)
	{
		wSet.fd_array[wSet.fd_count++] = m_socket.GetHandle();
	}

	fd_set exSet;
	exSet.fd_count = 1;
	exSet.fd_array[0] = m_socket.GetHandle();

	select(0, &rSet, &wSet, &exSet, nullptr);

	if (FD_ISSET(m_socket.GetHandle(), &exSet))
	{
		throw SystemException("Couldn't join a socket");
	}

	if (FD_ISSET(m_pair.GetSocket(), &rSet))
	{
		m_pair.Acquire();
	}

	if (FD_ISSET(m_socket.GetHandle(), &rSet))
	{
		if (FD_ISSET(m_socket.GetHandle(), &wSet))
		{
			return TransportState::CanReadWrite;
		}
		else
		{
			return TransportState::CanRead;
		}
	}
	else
	{
		if (FD_ISSET(m_socket.GetHandle(), &wSet))
		{
			return TransportState::CanWrite;
		}
		else
		{
			return TransportState::None;
		}
	}
}

uint32_t SocketTransport::Write(const uint32_t size, const void* buffer)
{
	if (0 == size)
	{
		return 0;
	}

	const int status = m_socket.Send(reinterpret_cast<const char*>(buffer), static_cast<int>(size), 0);
	if (status > 0)
	{
		return static_cast<uint32_t>(status);
	}

	const DWORD code = WSAGetLastError();
	if (WSAEWOULDBLOCK == code)
	{
		return 0;
	}

	throw SystemException("Couldn't write data to a socket");
}

uint32_t SocketTransport::Read(const uint32_t size, void* buffer)
{
	if (0 == size)
	{
		return 0;
	}

	const int status = m_socket.Recv(reinterpret_cast<char*>(buffer), size, 0);
	if (status > 0)
	{
		return static_cast<uint32_t>(status);
	}

	const DWORD code = WSAGetLastError();
	if (WSAEWOULDBLOCK == code)
	{
		return 0;
	}

	throw SystemException("Couldn't read data from a socket");
}

void SocketTransport::WakeUp()
{
	m_pair.Release();
}
