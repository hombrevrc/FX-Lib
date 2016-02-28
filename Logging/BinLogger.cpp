//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "BinLogger.h"
#include "BinFlushingQueue.h"


using namespace std;

namespace
{
	PhysicalSingleton<BinFlushingQueue> gQueue;
	PhysicalSingleton<BinLogger> gGlobalLogger;
}

namespace
{
	std::ostream& operator << (std::ostream& stream, FILETIME fTime)
	{
		SYSTEMTIME sTime;
		ZeroMemory(&sTime, sizeof(sTime));
		if(!FileTimeToSystemTime(&fTime, &sTime))
		{
			throw std::runtime_error("Couldn't convert file time to system time");
		}

		stream << setw(4) << setfill('0') << sTime.wYear << '-';
		stream << setw(2) << setfill('0') << sTime.wMonth << '-';
		stream << setw(2) << setfill('0') << sTime.wDay;
		stream << ' ';
		stream << setw(2) << setfill('0') << sTime.wHour << ':';
		stream << setw(2) << setfill('0') << sTime.wMinute << ':';
		stream << setw(2) << setfill('0') << sTime.wSecond << ".";
		stream << setw(3) << setfill('0') << sTime.wMilliseconds;

		return stream;
	}
}

BinLogger::BinLogger(const std::filesystem::path& directory, const std::tstring& filename) :
	m_directory(directory), m_filename(filename)
{
	if (!std::filesystem::create_directories(m_directory))
	{
		if (!std::filesystem::exists(m_directory))
		{
			throw SystemException("Couldn't create a new directory");
		}
	}

	gQueue->Add(this);
}

BinLogger::~BinLogger()
{
	gQueue->Remove(this);
}

void BinLogger::Initialize(const std::filesystem::path& directory, const std::tstring& filename)
{
	gGlobalLogger.GetOrCreate(directory, filename);
}

BinLogger& BinLogger::GetGlobalLogger()
{
	return gGlobalLogger.Get();
}

void BinLogger::StopFlushingQueue()
{
	gQueue->Stop();
}

void BinLogger::SwitchToSynchronousMode()
{
	m_synchronousMode = true;
}

void BinLogger::Add(BinEntry* pEntry)
{
	pEntry->Link = m_trail;
	pEntry->AcquireCurrentTime();
	while(!m_trail.compare_exchange_weak(pEntry->Link, pEntry))
	{
	}

	if (m_synchronousMode)
	{
		Flush();
	}
}

void BinLogger::Flush()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);

	BinEntry* pTrail = m_trail;
	while(!m_trail.compare_exchange_weak(pTrail, nullptr))
	{
	}

	BinEntry* pHead = nullptr;
	for (BinEntry* pCurrent = pTrail; nullptr != pCurrent;)
	{
		BinEntry* pNext = pCurrent->Link;
		pCurrent->Link = pHead;
		pHead = pCurrent;
		pCurrent = pNext;
	}

	for (BinEntry* pCurrent = pHead; nullptr != pCurrent;)
	{
		Write(*pCurrent);
		BinEntry* pTemp = pCurrent;
		pCurrent = pCurrent->Link;
		delete pTemp;
	}
}

void BinLogger::Write(BinEntry& entry)
{
	try
	{
		ReopenIfNeeded(entry.TimePoint);
		SYSTEMTIME time;
		ZeroMemory(&time, sizeof(time));
		FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&entry.TimePoint), &time);

		std::ostringstream stream;
		stream << reinterpret_cast<FILETIME&>(entry.TimePoint);
		stream << ", " << entry.ThreadId;
		if (nullptr != entry.Type)
		{
			stream << ", " << entry.Type;
		}
		stream << ">: ";
		entry.Format(stream);
		m_stream << stream.str() << std::endl;
	}
	catch (const std::exception& ex)
	{
		m_stream << ex.what() << std::endl;
	}
}

void BinLogger::ReopenIfNeeded(SystemClock::time_point tp)
{
	typedef std::chrono::duration<int32_t, std::ratio<24 * 3600> > days;
	days date = std::chrono::duration_cast<days>(tp.time_since_epoch());
	if (date.count() <= m_date)
	{
		return;
	}

	DateTime dt(tp);

	tostringstream stream;
	stream << dt.Year << '-' << dt.Month << '-' << dt.Day << '_' << m_filename << ".log";
	tstring filename = stream.str();
	std::filesystem::path path = m_directory / filename;

	if (m_stream.is_open())
	{
		m_stream.flush();
		m_stream.close();
	}

	m_stream.open(path, std::ios::app);
	if (m_stream.is_open())
	{
		m_date = date.count();
	}
}
