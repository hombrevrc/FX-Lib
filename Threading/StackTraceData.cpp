//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StackTraceData.h"

StackTraceData::StackTraceData(const uint32_t counter) : m_counter(counter)
{
}

void StackTraceData::Acquire()
{
	m_counter.fetch_add(1);
}

bool StackTraceData::Release()
{
	const uint32_t counter = m_counter.fetch_sub(1);
	if (1 == counter)
	{
		return true;
	}
	return false;
}
