//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "StackFrame.h"
#include "StackTraceData.h"

class ExceptionStackTrace;

class StackTrace
{
public:
	StackTrace();
	StackTrace(const StackTrace& stackTrace);
	StackTrace& operator = (const StackTrace& stackTrace);
	~StackTrace();

private:
	StackTrace(StackTraceData* pData);

public:
	std::vector<StackFrame> GetFrames() const;

private:
	void Finalize();

private:
	StackTraceData* m_pData = nullptr;

private:
	friend class ExceptionStackTrace;
};
std::ostream& operator << (std::ostream& stream, const StackTrace& stackTrace);
