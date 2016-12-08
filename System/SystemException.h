//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class SystemException : public std::runtime_error
{
public:
	SystemException(const char* message);
	SystemException(const std::string& message);
	virtual ~SystemException();

public:
	virtual char const* what() const throw() override;

public:
	const uint32_t GetErrorCode() const;

private:
	void FormatMessage(const char* message);

private:
	const uint32_t _errorCode = 0;
	std::string _what;
};