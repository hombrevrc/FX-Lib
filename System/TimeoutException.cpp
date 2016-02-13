//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "TimeoutException.h"


TimeoutException::TimeoutException(const char* message) : std::runtime_error(message)
{
}

TimeoutException::TimeoutException(const std::string& message) : std::runtime_error(message)
{
}
