//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "StdSemaphore.h"
#include "LiteSingleton.h"

class TaskWaiter
{
public:
	TaskWaiter();
	TaskWaiter(const TaskWaiter&) = delete;
	TaskWaiter& operator = (const TaskWaiter&) = delete;
	~TaskWaiter();

public:
	void Acquire();
	void Release();
	void Join();

private:
	StdSemaphore& GetEvent();

private:
	std::atomic<uint32_t> m_state;
	LiteSingleton<StdSemaphore> m_event;
};
