//==============================================================
// Copyright (c) 2017 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class ThreadContextSwitches
{
public:
	uint32_t GetContextSwitchesNumberOfCurrentThread();

private:
	std::vector<uint8_t> _buffer;
};