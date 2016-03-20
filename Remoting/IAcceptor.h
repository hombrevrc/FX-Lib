//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ITransport.h"

class IAcceptor
{
public:
	virtual ITransport* Accept() = 0;
	virtual void StopAccepting() = 0;
	virtual ~IAcceptor() {};
};