//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "StackFrame.h"
#include "CriticalSection.h"

class SymManager
{
public:
	SymManager();
	SymManager(const SymManager&) = delete;
	SymManager& operator = (const SymManager&) = delete;
	~SymManager();

public:
	static std::vector<StackFrame> GetFrames(const uint32_t framesNumber, void** frames);

private:
	std::vector<StackFrame> DoGetFrames(const uint32_t framesNumber, void** frames);
	void GetFrame(void* address, StackFrame& frame);

private:
	std::string CollectSearchPdbPaths();

private:
	CriticalSection m_synchronizer;
};