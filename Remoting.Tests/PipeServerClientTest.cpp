//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeServerClientTest.h"


PipeServerClientTest::PipeServerClientTest() : ServerClientTest(m_pipeBuilder)
{
}

void PipeServerClientTest::Simple()
{
	RunSimple();
}

