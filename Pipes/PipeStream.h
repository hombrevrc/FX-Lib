//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Pipe.h"

class PipeStream
{
public:
	PipeStream();

public:
	void Bind(Pipe&& pipe);
	HANDLE GetPipeEventHandle();

protected:
	bool m_pendingIO = false;
	Pipe m_pipe;
	MemoryStream m_stream;
};