//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IBuilder.h"

class SocketBuilder : public IBuilder
{
public:
	virtual IAcceptor* CreateAcceptor() override;
	virtual IConnector* CreateConnector() override;
};