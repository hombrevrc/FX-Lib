//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class StackFrame
{
public:
	void* Address = nullptr;
	uint32_t LineNumber = 0;
	std::string FunctionName;
	std::string FullFunctionName;
	std::string ModulePath;
	std::string FilePath;

public:
	std::string ModuleName() const;
	std::string FileName() const;
};

std::ostream& operator << (std::ostream& stream, const StackFrame& stackFrame);
