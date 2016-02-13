//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Timeout
{
public:
	Timeout(const std::chrono::milliseconds interval);

public:
	std::chrono::milliseconds ToInterval() const;

private:
	std::chrono::steady_clock::time_point m_finish;
};
