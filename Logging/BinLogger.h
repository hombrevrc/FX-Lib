//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "BinEntry.h"

class BinLogger
{
public:
	BinLogger(const std::filesystem::path& directory, const std::tstring& filename);
	BinLogger(const BinLogger&) = delete;
	BinLogger& operator = (const BinLogger&) = delete;
	~BinLogger();

public:
	static void Initialize(const std::filesystem::path& directory, const std::tstring& filename);
	static BinLogger& GetGlobalLogger();
	static void StopFlushingQueue();

public:
	void Add(BinEntry* pEntry);
	void Flush();
	void SwitchToSynchronousMode();

private:
	void Write(BinEntry& entry);
	void ReopenIfNeeded(SystemClock::time_point tp);

private:
	const std::filesystem::path m_directory;
	const std::tstring m_filename;

private:
	std::atomic<bool> m_synchronousMode;
	std::atomic<BinEntry*> m_trail;
	std::mutex m_synchronizer;

private:
	int32_t m_date = -1;
	std::ofstream m_stream;
};
