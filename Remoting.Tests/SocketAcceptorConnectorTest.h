//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "AcceptorConnectorTest.h"
#include "SocketBuilder.h"

TEST_CLASS(SocketAcceptorConnectorTest) , public AcceptorConnectorTest
{
public:
	SocketAcceptorConnectorTest();
	SocketAcceptorConnectorTest(const SocketAcceptorConnectorTest&) = delete;
	SocketAcceptorConnectorTest& operator = (const SocketAcceptorConnectorTest&) = delete;

public:
	TEST_METHOD(Simple);
	TEST_METHOD(SimpleAndRead);
	TEST_METHOD(SimpleAndRead2);
	TEST_METHOD(SimpleAndRead3);

private:
	SocketBuilder m_socketBuilder;
};