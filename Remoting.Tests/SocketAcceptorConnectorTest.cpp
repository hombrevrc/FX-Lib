//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketAcceptorConnectorTest.h"


SocketAcceptorConnectorTest::SocketAcceptorConnectorTest() : AcceptorConnectorTest(m_socketBuilder)
{
}

void SocketAcceptorConnectorTest::Simple()
{
	RunSimple();
}

void SocketAcceptorConnectorTest::SimpleAndRead()
{
	RunSimpleAndRead();
}

void SocketAcceptorConnectorTest::SimpleAndRead2()
{
	RunSimpleAndRead2();
}

void SocketAcceptorConnectorTest::SimpleAndRead3()
{
	RunSimpleAndRead3();
}