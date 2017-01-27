//==============================================================
// Copyright (c) 2017 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ThreadContextSwitches.h"


namespace
{
	typedef struct _UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef LONG KPRIORITY;
	typedef LONG KWAIT_REASON;

	typedef struct _CLIENT_ID {
		HANDLE  UniqueProcess;
		HANDLE  UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;

	typedef struct _SYSTEM_THREAD_INFORMATION {
		LARGE_INTEGER   KernelTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   CreateTime;
		ULONG   WaitTime;
		PVOID   StartAddress;
		CLIENT_ID   ClientId;
		KPRIORITY   Priority;
		LONG    BasePriority;
		ULONG   ContextSwitches;
		ULONG   ThreadState;
		KWAIT_REASON    WaitReason;
	} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

	typedef struct _SYSTEM_PROCESS_INFORMATION { // Information Class 5
		ULONG   NextEntryOffset;
		ULONG   NumberOfThreads;
		LARGE_INTEGER   WorkingSetPrivateSize;
		ULONG   HardFaultCount;
		ULONG   NumberOfThreadsHighWatermark;
		ULONGLONG   CycleTime;
		LARGE_INTEGER   CreateTime;
		LARGE_INTEGER   UserTime;
		LARGE_INTEGER   KernelTime;
		UNICODE_STRING  ImageName;
		KPRIORITY   BasePriority;
		HANDLE  UniqueProcessId;
		HANDLE  InheritedFromUniqueProcessId;
		ULONG   HandleCount;
		ULONG   SessionId;
		ULONG_PTR   UniqueProcessKey;
		SIZE_T  PeakVirtualSize;
		SIZE_T  VirtualSize;
		ULONG   PageFaultCount;
		SIZE_T  PeakWorkingSetSize;
		SIZE_T  WorkingSetSize;
		SIZE_T  QuotaPeakPagedPoolUsage;
		SIZE_T  QuotaPagedPoolUsage;
		SIZE_T  QuotaPeakNonPagedPoolUsage;
		SIZE_T  QuotaNonPagedPoolUsage;
		SIZE_T  PagefileUsage;
		SIZE_T  PeakPagefileUsage;
		SIZE_T  PrivatePageCount;
		LARGE_INTEGER ReadOperationCount;
		LARGE_INTEGER WriteOperationCount;
		LARGE_INTEGER OtherOperationCount;
		LARGE_INTEGER ReadTransferCount;
		LARGE_INTEGER WriteTransferCount;
		LARGE_INTEGER OtherTransferCount;
		SYSTEM_THREAD_INFORMATION Threads[1];
	} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;
}

namespace
{
	const DWORD STATUS_SUCCESS = 0x00000000;
	const DWORD STATUS_INFO_LENGTH_MISMATCH = 0xC0000004;
}

namespace
{
	typedef DWORD (__stdcall *NtQuerySystemInformationFunc)(DWORD, uint8_t*, DWORD, DWORD*);
	NtQuerySystemInformationFunc NtQuerySystemInformation = reinterpret_cast<NtQuerySystemInformationFunc>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQuerySystemInformation"));
}

uint32_t ThreadContextSwitches::GetContextSwitchesNumberOfCurrentThread()
{
	const HANDLE processId = reinterpret_cast<HANDLE>(static_cast<size_t>(GetCurrentProcessId()));
	const HANDLE threadId = reinterpret_cast<HANDLE>(static_cast<size_t>(GetCurrentThreadId()));

	uint8_t* buffer = nullptr;
	for (;;)
	{
		DWORD count = static_cast<DWORD>(_buffer.size());
		if (count > 0)
		{
			buffer = &_buffer.front();
		}
		DWORD needs = 0;
		DWORD status = NtQuerySystemInformation(5, buffer, count, &needs);
		if (STATUS_SUCCESS == status)
		{
			break;
		}

		if (STATUS_INFO_LENGTH_MISMATCH == status)
		{
			_buffer.resize(needs);
		}
		else
		{
			throw SystemException("Couldn't get system information");
		}
	}

	ULONG offset = 0;
	for (;;)
	{
		SYSTEM_PROCESS_INFORMATION* pProcess = (PSYSTEM_PROCESS_INFORMATION)(buffer + offset);
		offset += pProcess->NextEntryOffset;
		SYSTEM_THREAD_INFORMATION* current = pProcess->Threads;
		SYSTEM_THREAD_INFORMATION* end = current + pProcess->NumberOfThreads;

		for (; current < end; ++current)
		{
			if (processId != current->ClientId.UniqueProcess)
			{
				break;
			}
			if (threadId == current->ClientId.UniqueThread)
			{
				return current->ContextSwitches;
			}
		}
	}
}
