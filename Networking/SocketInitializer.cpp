//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketInitializer.h"


SocketInitializer::SocketInitializer()
{
	const WORD version = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(version, &data))
	{
		throw SystemException("Couldn't initialize windows sockets.");
	}
}

SocketInitializer::~SocketInitializer()
{
	WSACleanup();
}
