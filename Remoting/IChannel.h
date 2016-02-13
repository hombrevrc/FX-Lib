//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ChannelState.h"

class IChannel
{
public:
	virtual void Accept() = 0;
	virtual ChannelState Join(const bool reading, const bool writing) = 0;
	virtual uint32_t Write(const uint32_t size, const void* buffer) = 0;
	virtual uint32_t Read(const uint32_t size, void* buffer) = 0;
	virtual void WakeUp() = 0;
	virtual ~IChannel() {};
};