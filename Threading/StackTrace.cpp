//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StackTrace.h"
#include "TlsValue.h"
#include "StackTraceData.h"
#include "SymManager.h"


namespace
{
	TlsValue<StackTraceData*> gStackTraceData;
}

namespace
{
	LONG CALLBACK VectoredHandler(_In_ PEXCEPTION_POINTERS /*ExceptionInfo*/)
	{
		StackTraceData* pData = gStackTraceData;
		pData->FramesNumber = CaptureStackBackTrace(0, _countof(pData->Frames), pData->Frames, nullptr);
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

StackTrace::StackTrace()
{
	StackTraceData* pData = gStackTraceData;
	if (nullptr == pData)
	{
		pData = new StackTraceData();
		gStackTraceData = pData;
		AddVectoredExceptionHandler(TRUE, VectoredHandler);
	}
	pData->Counter++;
}

StackTrace::~StackTrace()
{
	StackTraceData* pData = gStackTraceData;
	pData->Counter--;
	if (0 == pData->Counter)
	{
		delete pData;
		gStackTraceData = nullptr;
		RemoveVectoredExceptionHandler(VectoredHandler);
	}
}

const std::vector<StackFrame> StackTrace::GetFrames() const
{
	StackTraceData* pData = gStackTraceData;
	return SymManager::GetFrames(pData->FramesNumber, pData->Frames);
}

std::ostream& operator<<(std::ostream& stream, const StackTrace& stackTrace)
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
