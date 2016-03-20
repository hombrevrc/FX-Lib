//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "PipeStream.h"
#include "Constants.h"

class PipeWriter : public PipeStream
{
public:
	PipeWriter();
	PipeWriter(const PipeWriter&) = delete;
	PipeWriter& operator = (const PipeWriter&) = delete;

public:
	bool CanBeWritten();
	uint32_t Write(const uint32_t size, const void* pBuffer);

private:
	bool WriteBuffer();
};