//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "FastEntry.h"

class FastLogger
{
public:
	FastLogger(const std::filesystem::path& directory, const std::tstring& filename);
	FastLogger(const FastLogger&) = delete;
	FastLogger& operator = (const FastLogger&) = delete;
	~FastLogger();

public:
	static void Initialize(const std::filesystem::path& directory, const std::tstring& filename);
	static FastLogger& GetGlobalLogger();
	static void StopFlushingQueue();

public:
	void Add(FastEntry* pEntry);
	void Flush();
	void SwitchToSynchronousMode();

private:
	void Write(FastEntry& entry);
	void ReopenIfNeeded(SystemClock::time_point tp);

private:
	const std::filesystem::path m_directory;
	const std::tstring m_filename;

private:
	std::atomic<bool> m_synchronousMode;
	std::atomic<FastEntry*> m_trail;
	std::mutex m_synchronizer;

private:
	int32_t m_date = -1;
	std::ofstream m_stream;
};
