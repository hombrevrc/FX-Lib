//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IChannel.h"

class IAcceptor
{
public:
	virtual IChannel* Accept() = 0;
	virtual void StopAccepting() = 0;
	virtual ~IAcceptor() {};
};