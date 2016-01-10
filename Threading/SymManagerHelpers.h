//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

const uint32_t cMaximumSymbolLength = 4096;

struct ImageHelpLine64 : public IMAGEHLP_LINE64
{
	ImageHelpLine64()
	{
		ZeroMemory(this, sizeof(*this));
		SizeOfStruct = sizeof(*this);
	}
};

struct SymbolInfo : public SYMBOL_INFO
{
	uint8_t Buffer[cMaximumSymbolLength];
	SymbolInfo()
	{
		ZeroMemory(this, sizeof(*this));
		SizeOfStruct = sizeof(SYMBOL_INFO);
		MaxNameLen = _countof(Buffer);
	}
};

struct ImageHelpModule64 : public IMAGEHLP_MODULE64
{
	ImageHelpModule64()
	{
		ZeroMemory(this, sizeof(*this));
		SizeOfStruct = sizeof(*this);
	}
};