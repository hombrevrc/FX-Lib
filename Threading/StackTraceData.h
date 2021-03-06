//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class StackTraceData
{
public:
	uint32_t FramesNumber = 0;
	void* Frames[1024];

public:
	StackTraceData() = default;
	StackTraceData(const uint32_t counter);

public:
	void Acquire();
	bool Release();

private:
	std::atomic<uint32_t> m_counter = 0;
};