//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class SystemException : public std::runtime_error
{
public:
	SystemException(const char* message);
	SystemException(const std::string& message);

public:
	virtual char const* what() const override;

public:
	const uint32_t GetErrorCode() const;

private:
	void FormatMessage(const char* message);

private:
	const uint32_t m_errorCode = 0;
	std::string m_what;
};