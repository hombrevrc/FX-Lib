//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ITransport.h"

class IConnector
{
public:
	virtual ITransport* Connect() = 0;
	virtual ~IConnector() {};
};