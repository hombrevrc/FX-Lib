//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "StackTrace.h"

class StackTraceData;

class ExceptionStackTrace
{
public:
	ExceptionStackTrace();
	ExceptionStackTrace(const ExceptionStackTrace&) = delete;
	ExceptionStackTrace& operator = (const ExceptionStackTrace&) = delete;
	~ExceptionStackTrace();

public:
	static StackTrace GetStackTrace();

private:
	StackTraceData* m_data;
};

