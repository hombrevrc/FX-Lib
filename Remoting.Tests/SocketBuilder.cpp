//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SocketBuilder.h"


namespace
{
	const uint16_t cTestPort = 1234;
	const std::string cTestAddress = "localhost";
}

IAcceptor* SocketBuilder::CreateAcceptor()
{
	IAcceptor* result = new SocketAcceptor(cTestPort, true);
	return result;
}

IConnector* SocketBuilder::CreateConnector()
{
	IConnector* result = new SocketConnector(cTestAddress, cTestPort);
	return result;
}
