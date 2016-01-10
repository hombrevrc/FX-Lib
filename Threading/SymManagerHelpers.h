//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

const uint32_t cMaximumSymbolLength = 4096;

struct CImageHelpLine64 : public IMAGEHLP_LINE64
{
	CImageHelpLine64()
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

struct CImagehlpModule64 : public IMAGEHLP_MODULE64
{
	CImagehlpModule64()
	{
		ZeroMemory(this, sizeof(*this));
		SizeOfStruct = sizeof(*this);
	}
};