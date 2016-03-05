//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SystemException.h"

#ifdef FX_LIB_WINDOWS

namespace
{
	const DWORD cFormatMessageFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
}

#endif

#ifdef FX_LIB_LINUX
namespace
{
    const uint32_t GetLastError()
    {
        return errno;
    }
}

#endif

SystemException::SystemException(const char* message) : std::runtime_error(message), m_errorCode(GetLastError())
{
	FormatMessage(message);
}

SystemException::SystemException(const std::string& message) : std::runtime_error(message), m_errorCode(GetLastError())
{
	FormatMessage(message.c_str());
}

SystemException::~SystemException()
{
}

char const* SystemException::what() const throw()
{
	return m_what.c_str();
}

const uint32_t SystemException::GetErrorCode() const
{
	return m_errorCode;
}

#ifdef FX_LIB_WINDOWS

void SystemException::FormatMessage(const char* message)
{
	std::ostringstream stream;
	stream << message << ". GetLastError() = " << m_errorCode;
	char* buffer = nullptr;
	const DWORD length = FormatMessageA(cFormatMessageFlags, nullptr, m_errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&buffer), 0, nullptr);
	if (length > 0)
	{
		try
		{
			stream << " (" << buffer << ')';
		}
		catch (const std::exception& /*ex*/)
		{
		}
		LocalFree(buffer);
	}
	m_what = stream.str();
}

#endif


#ifdef FX_LIB_LINUX

void SystemException::FormatMessage(const char* message)
{
	std::ostringstream stream;
	stream << message << ". errno = " << m_errorCode;
	m_what = stream.str();
}

#endif

