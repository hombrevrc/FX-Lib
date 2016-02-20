//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

std::filesystem::path GetStartupDirectory()
{
	wchar_t buffer[MAX_PATH] = L"";
	GetModuleFileNameW(nullptr, buffer, _countof(buffer));
	std::filesystem::path result = buffer;
	result = result.parent_path();
	return result;
}
