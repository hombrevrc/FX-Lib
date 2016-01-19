//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SymManager.h"
#include "SymManagerHelpers.h"
#include "PhysicalSingleton.h"


namespace
{
	PhysicalSingleton<SymManager> gSymManager;
}

SymManager::SymManager()
{
	SymSetOptions(SYMOPT_PUBLICS_ONLY);
	const HANDLE process = GetCurrentProcess();
	std::string path = CollectSearchPdbPaths();
	const BOOL status = SymInitialize(process, const_cast<char*>(path.c_str()), TRUE);
	assert(status);
	UNREFERENCED_PARAMETER(status);
}

SymManager::~SymManager()
{
	const HANDLE process = GetCurrentProcess();
	SymCleanup(process);
}

std::string SymManager::CollectSearchPdbPaths()
{
	std::stringstream stream;
	char buffer[MAX_PATH] = "";
	GetModuleFileNameA(nullptr, buffer, _countof(buffer));
	char* pLast = strrchr(buffer, '\\');
	*pLast = 0;
	stream << buffer;
	std::string result = stream.str();
	return result;
}

std::vector<StackFrame> SymManager::GetFrames(const uint32_t framesNumber, void** frames)
{
	SymManager& manager = gSymManager.GetOrCreate();
	return manager.DoGetFrames(framesNumber, frames);
}

std::vector<StackFrame> SymManager::DoGetFrames(const uint32_t framesNumber, void** frames)
{
	std::vector<StackFrame> result;
	result.resize(framesNumber);

	CsLocker lock(m_synchronizer);
	for (uint32_t index = 0; index < framesNumber; ++index)
	{
		GetFrame(frames[index], result[index]);
	}
	return result;
}

void SymManager::GetFrame(void* address, StackFrame& frame)
{
	HANDLE process = GetCurrentProcess();
	DWORD64 address64 = reinterpret_cast<DWORD64>(address);

	frame.Address = address;
	// get source code file name and line number
	ImageHelpLine64 line;
	DWORD offset = 0;
	BOOL status = SymGetLineFromAddr64(process, address64, &offset, &line);
	if (status)
	{
		if (nullptr != line.FileName)
		{
			frame.FilePath = line.FileName;
		}
		frame.LineNumber = line.LineNumber;
	}

	// get decorated function name
	SymbolInfo symbol;
	status = SymFromAddr(process, address64, nullptr, &symbol);
	if (status)
	{
		char buffer[cMaximumSymbolLength] = "";
		UnDecorateSymbolName(symbol.Name, buffer, sizeof(buffer), UNDNAME_NAME_ONLY);
		frame.FunctionName = buffer;

		UnDecorateSymbolName(symbol.Name, buffer, sizeof(buffer), UNDNAME_COMPLETE);
		frame.FullFunctionName = buffer;
	}
	// get module info
	ImageHelpModule64 module;
	status = SymGetModuleInfo64(process, address64, &module);
	if (status)
	{
		frame.ModulePath = module.ImageName;
	}
}