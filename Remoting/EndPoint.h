//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Delegates.h"

class EndPoint
{
public:
	virtual ~EndPoint();

public:
	virtual void Initialize(Channel* pChannel) = 0;
	virtual bool Finalize(Channel* pChannel) = 0;
	virtual MessageHandler GetMessageHandler() = 0;
	virtual ThreadPool& GetThreadPool() = 0;
};