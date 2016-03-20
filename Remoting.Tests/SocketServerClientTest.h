//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once


#include "ServerClientTest.h"
#include "SocketBuilder.h"

TEST_CLASS(SocketServerClientTest), public ServerClientTest
{
public:
	SocketServerClientTest();
	SocketServerClientTest(const SocketServerClientTest&) = delete;
	SocketServerClientTest& operator = (const SocketServerClientTest&) = delete;

public:
	TEST_METHOD(Simple);

private:
	SocketBuilder m_socketBuilder;
};
