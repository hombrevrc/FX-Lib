//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class SocketInitializer
{
public:
	SocketInitializer();
	SocketInitializer(const SocketInitializer&) = delete;
	SocketInitializer& operator = (const SocketInitializer&) = delete;
	~SocketInitializer();
};