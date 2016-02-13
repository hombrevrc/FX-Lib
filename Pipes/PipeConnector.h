//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class PipeConnector : public IConnector
{
public:
	PipeConnector(const std::wstring& name);
	PipeConnector(const PipeConnector&) = delete;
	PipeConnector& operator = (const PipeConnector&) = delete;

public:
	virtual IChannel* Connect() override;

private:
	std::wstring m_name;
};