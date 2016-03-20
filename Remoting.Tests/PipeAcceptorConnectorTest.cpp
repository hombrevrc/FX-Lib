//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeAcceptorConnectorTest.h"


PipeAcceptorConnectorTest::PipeAcceptorConnectorTest() : AcceptorConnectorTest(m_pipeBuilder)
{
}

void PipeAcceptorConnectorTest::Simple()
{
	RunSimple();
}

void PipeAcceptorConnectorTest::SimpleAndRead()
{
	RunSimpleAndRead();
}

void PipeAcceptorConnectorTest::SimpleAndRead2()
{
	RunSimpleAndRead2();
}

void PipeAcceptorConnectorTest::SimpleAndRead3()
{
	RunSimpleAndRead3();
}
