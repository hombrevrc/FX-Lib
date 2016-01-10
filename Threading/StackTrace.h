//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "StackFrame.h"

class StackTraceData;

class StackTrace
{
public:
	StackTrace();
	StackTrace(const StackTrace&) = delete;
	StackTrace& operator = (const StackTrace&) = delete;
	~StackTrace();

public:
	const std::vector<StackFrame> GetFrames() const;

private:
	StackTraceData* m_data;
};

std::ostream& operator << (std::ostream& stream, const StackTrace& stackTrace);
