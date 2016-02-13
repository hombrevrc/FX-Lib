//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class TimeoutException : public std::runtime_error
{
public:
	TimeoutException();
	TimeoutException(const char* message);
	TimeoutException(const std::string& message);
};