//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ExceptionStackTrace.h"
#include "TlsValue.h"
#include "StackTraceData.h"
#include "SymManager.h"
#include "PhysicalSingleton.h"


namespace
{
	TlsValue<StackTraceData*> gStackTraceData;
}

namespace
{
	LONG CALLBACK VectoredHandler(_In_ PEXCEPTION_POINTERS /*ExceptionInfo*/)
	{
		StackTraceData* pData = gStackTraceData;
		if (nullptr != pData)
		{
			pData->FramesNumber = CaptureStackBackTrace(0, _countof(pData->Frames), pData->Frames, nullptr);
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

namespace
{
	class ExceptionStackTraceRegistrator
	{
	public:
		ExceptionStackTraceRegistrator()
		{
			AddVectoredExceptionHandler(TRUE, VectoredHandler);
		}
		~ExceptionStackTraceRegistrator()
		{
			RemoveVectoredExceptionHandler(VectoredHandler);
		}
	};

	PhysicalSingleton<ExceptionStackTraceRegistrator> gExceptionStackTraceRegistrator;
}

ExceptionStackTrace::ExceptionStackTrace()
{
	gExceptionStackTraceRegistrator.GetOrCreate();

	StackTraceData* pData = gStackTraceData;
	if (nullptr == pData)
	{
		pData = new StackTraceData();
		gStackTraceData = pData;
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
	return pData;
}
