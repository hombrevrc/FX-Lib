//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "PipeStream.h"
#include "Constants.h"

class PipeReader : public PipeStream
{
public:
	PipeReader() = default;
	PipeReader(const PipeReader&) = delete;
	PipeReader& operator = (const PipeReader&) = delete;

public:
	bool CanBeRead();
	uint32_t Read(const uint32_t size, void* pBuffer);

private:
	uint32_t ReadFromBuffer(uint32_t size, void* pBuffer);
	bool ReadBuffer();
};
