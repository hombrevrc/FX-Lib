//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

std::tstring GenerateGuid();

template<typename T>
void ReplaceAll(const std::basic_string<T>& oldValue, const std::basic_string<T>& newValue, std::basic_string<T>& text)
{
	if (!oldValue.empty())
	{
		for (size_t pos = text.find(oldValue, 0); std::string::npos != pos; pos = text.find(oldValue, pos))
		{
			text.replace(pos, oldValue.length(), newValue);
			pos += newValue.length();
		}
	}
}