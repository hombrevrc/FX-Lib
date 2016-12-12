//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "SysSemaphore.h"

class Optex
{
public:
	Optex(const std::tstring& name, void* security = nullptr);
	Optex(const Optex&) = delete;
	Optex& operator = (const Optex&) = delete;

public:
	void lock();
	void unlock();

private:
	static std::wstring MakeFullname(const std::wstring& name);

private:
	SharedMemory m_memory;
	SysSemaphore m_event;
};
