//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "FastLogger.h"

class FastOutput
{
public:
	FastOutput(const char* type, FastLogger* pLogger = nullptr) : m_type(type)
	{
		if (nullptr == m_pLogger)
		{
			pLogger = &FastLogger::GetGlobalLogger();
		}
		m_pLogger = pLogger;
	}

private:
	FastOutput(const FastOutput&) = delete;
	FastOutput& operator = (const FastOutput&) = delete;

public:
	inline void operator () (FastEntry* pEntry) const
	{
		pEntry->Type = m_type;
		m_pLogger->Add(pEntry);
	}

private:
	const char* m_type;
	FastLogger* m_pLogger;
};

class FastError : public FastOutput
{
public:
	FastError(FastLogger* pLogger = nullptr) : FastOutput("error", pLogger)
	{
	}
};

class FastEvent : public FastOutput
{
public:
	FastEvent(FastLogger* pLogger = nullptr) : FastOutput("event", pLogger)
	{
	}
};