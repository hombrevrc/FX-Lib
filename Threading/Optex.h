//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SysSemaphore.h"

class Optex
{
public:
	Optex(const std::tstring& name);
	Optex(const Optex&) = delete;
	Optex& operator = (const Optex&) = delete;

public:
	static std::wstring MakeFullname(const std::wstring& name);
	static uint32_t SharedMemorySize();

public:
	void lock();
	void unlock();

private:
	SharedMemory m_memory;
	SysSemaphore m_event;
};
