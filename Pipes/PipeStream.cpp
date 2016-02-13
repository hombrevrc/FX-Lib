//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "PipeStream.h"
#include "Constants.h"


PipeStream::PipeStream()
{
	m_stream.SetSize(PIPE_BUFFER_SIZE);
}

HANDLE PipeStream::GetPipeEventHandle()
{
	return m_pipe.GetEventHandle();
}

void PipeStream::Bind(Pipe&& pipe)
{
	m_pipe = std::move(pipe);
}
