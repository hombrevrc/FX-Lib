//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class StackTraceData
{
public:
	uint32_t Counter = 0;
	uint32_t FramesNumber = 0;
	void* Frames[1024];
};