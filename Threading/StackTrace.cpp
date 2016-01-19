//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StackTrace.h"
#include "StackTraceData.h"
#include "SymManager.h"


StackTrace::StackTrace() : m_pData(new StackTraceData(1))
{
	m_pData->FramesNumber = CaptureStackBackTrace(0, _countof(m_pData->Frames), m_pData->Frames, nullptr);
}

StackTrace::StackTrace(StackTraceData* pData) : m_pData(new StackTraceData(1))
{
	m_pData->FramesNumber = pData->FramesNumber;
	memcpy(m_pData->Frames, pData->Frames, sizeof(void*) * pData->FramesNumber);
}

StackTrace::StackTrace(const StackTrace& stackTrace) : m_pData(stackTrace.m_pData)
{
	m_pData->Acquire();
}

StackTrace& StackTrace::operator=(const StackTrace& stackTrace)
{
	if (m_pData != stackTrace.m_pData)
	{
		m_pData->Release();
		m_pData = stackTrace.m_pData;
		m_pData->Acquire();
	}
	return *this;
}

StackTrace::~StackTrace()
{
	if (m_pData->Release())
	{
		delete m_pData;
	}
}

std::vector<StackFrame> StackTrace::GetFrames() const
{
	return SymManager::GetFrames(m_pData->FramesNumber, m_pData->Frames);
}

std::ostream& operator << (std::ostream& stream, const StackTrace& stackTrace)
{
	const std::vector<StackFrame>& frames = stackTrace.GetFrames();
	auto it = frames.begin();
	auto end = frames.end();

	if (it != end)
	{
		stream << *it;
		++it;
	}
	for (; it != end; ++it)
	{
		stream << std::endl << *it;
	}
	return stream;
}
