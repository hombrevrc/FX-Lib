//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Socket.h"

class SocketsPair
{
public:
	SocketsPair();
	SocketsPair(const SocketsPair&) = delete;
	SocketsPair& operator = (const SocketsPair&) = delete;

public:
	SOCKET GetSocket();
	void Acquire();
	void Release();

private:
	std::mutex m_synchronizer;
	bool m_wasReleased = false;
	Socket m_primary;
	Socket m_secondary;
};