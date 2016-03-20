//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ServerClientTest.h"
#include "PipeBuilder.h"

TEST_CLASS(PipeServerClientTest) , public ServerClientTest
{
public:
	PipeServerClientTest();
	PipeServerClientTest(const PipeServerClientTest&) = delete;
	PipeServerClientTest& operator = (const PipeServerClientTest&) = delete;

public:
	TEST_METHOD(Simple);

private:
	PipeBuilder m_pipeBuilder;
};