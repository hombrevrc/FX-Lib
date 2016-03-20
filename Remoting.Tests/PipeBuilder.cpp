//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeBuilder.h"


namespace
{
	const std::wstring cPipeTestId = L"{C8A72DE6-56B1-4672-AB13-50FFCF822419}";
}

IAcceptor* PipeBuilder::CreateAcceptor()
{
	IAcceptor* result = new PipeAcceptor(cPipeTestId);
	return result;
}

IConnector* PipeBuilder::CreateConnector()
{
	IConnector* result = new PipeConnector(cPipeTestId);
	return result;
}
