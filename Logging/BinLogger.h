//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "BinEntry.h"

class BinLogger
{
public:
	BinLogger(const std::wstring& directory, const std::wstring& filename);
	BinLogger(const BinLogger&) = delete;
	BinLogger& operator = (const BinLogger&) = delete;
	~BinLogger();

public:
	void Add(BinEntry* pEntry);
	void Flush();

private:
	void Write(BinEntry& entry);
	void ReopenIfNeeded(uint64_t timeStamp);

private:
	const std::wstring m_directory;
	const std::wstring m_filename;

private:
	CriticalSection m_synchronizer;

private:
	BinEntry m_head;
	BinEntry* m_pLast;

private:
	uint64_t m_date = 0;
	std::ofstream m_stream;
};
