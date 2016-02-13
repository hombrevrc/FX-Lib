//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IChannel.h"

class IConnector
{
public:
	virtual IChannel* Connect() = 0;
	virtual ~IConnector() {};
};