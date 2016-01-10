//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "StackFrame.h"


std::string StackFrame::ModuleName() const
{
	const size_t index = ModulePath.find_last_of('\\');
	if (std::string::npos == index)
	{
		return std::string();
	}
	return ModulePath.substr(index + 1);
}

std::string StackFrame::FileName() const
{
	const size_t index = FilePath.find_last_of('\\');
	if (std::string::npos == index)
	{
		return std::string();
	}
	return FilePath.substr(index + 1);
}

std::ostream& operator<<(std::ostream& stream, const StackFrame& stackFrame)
{
	stream << "0x" << stackFrame.Address << ": ";
	if (stackFrame.ModulePath.empty())
	{
		stream << "<unknown module>";
	}
	else
	{
		stream << stackFrame.ModuleName();
	}
	stream << "!" << stackFrame.FunctionName;
	if (!stackFrame.FilePath.empty())
	{
		stream << ' '<< stackFrame.FileName() << '(' << stackFrame.LineNumber << ')';
	}
	return stream;
}
