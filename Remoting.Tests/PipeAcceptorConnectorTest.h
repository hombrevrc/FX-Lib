//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "AcceptorConnectorTest.h"
#include "PipeBuilder.h"

TEST_CLASS(PipeAcceptorConnectorTest), public AcceptorConnectorTest
{
public:
	PipeAcceptorConnectorTest();

public:
	TEST_METHOD(Simple);
	TEST_METHOD(SimpleAndRead);
	TEST_METHOD(SimpleAndRead2);
	TEST_METHOD(SimpleAndRead3);

private:
	PipeBuilder m_pipeBuilder;
};