//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "BinLogger.h"
#include "BinFlushingQueue.h"


using namespace std;

namespace
{
	const uint64_t cTicksInDay = 864000000000LL;
}

namespace
{
	PhysicalSingleton<BinFlushingQueue> gQueue;
}

namespace
{
	std::ostream& operator << (std::ostream& stream, FILETIME fTime)
	{
		SYSTEMTIME sTime;
		ZeroMemory(&sTime, sizeof(sTime));
		if(!FileTimeToSystemTime(&fTime, &sTime))
		{
			throw runtime_error("Couldn't convert file time to system time");
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

BinLogger::BinLogger(const std::wstring& directory, const std::wstring& filename) :
	m_directory(directory), m_filename(filename), m_head(nullptr, 0), m_pLast(&m_head)
{
	gQueue->Add(this);
}

BinLogger::~BinLogger()
{
	gQueue->Remove(this);
}

void BinLogger::Add(BinEntry* pEntry)
{
	CsLocker lock(m_synchronizer);
	pEntry->AcquireCurrentTime();
	m_pLast->Link = pEntry;
	m_pLast = pEntry;
}

void BinLogger::Flush()
{
	BinEntry* pEnry = nullptr;
	{
		CsLocker lock(m_synchronizer);
		pEnry = m_head.Link;
		m_head.Link = nullptr;
		m_pLast = &m_head;
	}
	while (nullptr != pEnry)
	{
		Write(*pEnry);
		BinEntry* pTemp = pEnry;
		pEnry = pEnry->Link;
		delete pTemp;
	}
}

void BinLogger::Write(BinEntry& entry)
{
	try
	{
		ReopenIfNeeded(entry.TimeStamp);
		SYSTEMTIME time;
		ZeroMemory(&time, sizeof(time));
		FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&entry.TimeStamp), &time);

		stringstream stream;
		stream << reinterpret_cast<FILETIME&>(entry.TimeStamp);
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

void BinLogger::ReopenIfNeeded(uint64_t timeStamp)
{
	const uint64_t date = timeStamp / cTicksInDay;
	if (date <= m_date)
	{
		return;
	}

	SYSTEMTIME time;
	ZeroMemory(&time, sizeof(time));
	FileTimeToSystemTime(reinterpret_cast<FILETIME*>(&timeStamp), &time);

	wstringstream stream;
	stream << m_directory << time.wYear << '-' << time.wMonth << '-' << time.wDay << '_' << m_filename << ".log";
	wstring path = stream.str();
	if (m_stream.is_open())
	{
		m_stream.flush();
		m_stream.close();
	}

	m_stream.open(path.c_str(), std::ios::app);
	if (m_stream.is_open())
	{
		m_date = date;
	}
}
