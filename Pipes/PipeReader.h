//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "PipeStream.h"
#include "Constants.h"

class PipeReader : public PipeStream
{
public:
	PipeReader();
	PipeReader(const PipeReader&) = delete;
	PipeReader& operator = (const PipeReader&) = delete;

public:
	bool CanBeRead();
	uint32_t Read(const uint32_t size, void* pBuffer);

public:
	std::wstring ReadWString(const Timeout& timeout);
	void Read(const Timeout& timeout, const uint32_t size, void* pBuffer);

private:
	uint32_t ReadFromBuffer(uint32_t size, void* pBuffer);
	bool ReadBuffer();
};
