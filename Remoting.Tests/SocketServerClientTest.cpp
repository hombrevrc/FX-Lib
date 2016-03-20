//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketServerClientTest.h"


SocketServerClientTest::SocketServerClientTest() : ServerClientTest(m_socketBuilder)
{
}

void SocketServerClientTest::Simple()
{
	RunSimple();
}

