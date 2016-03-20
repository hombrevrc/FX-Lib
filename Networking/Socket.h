//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Socket
{
public:
	Socket();
	Socket(SOCKET s);
	Socket(int af, int type, int protocol);
	Socket(const Socket&) = delete;
	Socket(Socket&& s);
	Socket& operator = (const Socket&) = delete;
	Socket& operator = (Socket&& s);
	~Socket();

public:
	void EnableNonBlockingMode();
	void EnableKeepAlive();

public:
	bool IsInvalid() const;
	void ThrowIfInvalid(const char* message) const;
	SOCKET GetHandle() const;

public:
	bool Create(int af, int type, int protocol);
	void Close();
	int Bind(uint16_t port, int mode);
	int Listen(int backlog = SOMAXCONN);
	Socket Accept(sockaddr* addr, int* addrlen);
	int Connect(const sockaddr* name, int namelen);
	int Shutdown(int how = SD_BOTH);
	int Send(const char * buf, int len, int flags = 0);
	int Recv(char* buf, int len, int flags = 0);
	int GetSockOpt(int level, int optname, char* optval, int* optlen);
	int SetSockOpt(int level, int optname, const char* optval, int optlen);
	int GetSockName(sockaddr* name, int* namelen);

private:
	SOCKET m_socket = INVALID_SOCKET;
};