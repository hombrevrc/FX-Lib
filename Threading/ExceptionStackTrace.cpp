//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ExceptionStackTrace.h"
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

ExceptionStackTrace::ExceptionStackTrace()
{
	StackTraceData* pData = gStackTraceData;
	if (nullptr == pData)
	{
		pData = new StackTraceData();
		gStackTraceData = pData;
		AddVectoredExceptionHandler(TRUE, VectoredHandler);
	}
	pData->Acquire();
}

ExceptionStackTrace::~ExceptionStackTrace()
{
	StackTraceData* pData = gStackTraceData;
	if (pData->Release())
	{
		delete pData;
		gStackTraceData = nullptr;
		RemoveVectoredExceptionHandler(VectoredHandler);
	}
}

StackTrace ExceptionStackTrace::GetStackTrace()
{
	std::exception_ptr ptr = std::current_exception();
	if (!ptr)
	{
		throw std::runtime_error("Exception stack trace can be obtained only from catch section only");
	}

	StackTraceData* pData = gStackTraceData;
	if (nullptr == pData)
	{
		throw std::runtime_error("Exception stack trace is not initialized");
	}

	return pData;
}
