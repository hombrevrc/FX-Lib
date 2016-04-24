//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "FastLogger.h"
#include "FastFlushingQueue.h"


using namespace std;

namespace
{
	PhysicalSingleton<FastFlushingQueue> gQueue;
	PhysicalSingleton<FastLogger> gGlobalLogger;
}

FastLogger::FastLogger(const std::filesystem::path& directory, const std::tstring& filename) :
	m_directory(directory), m_filename(filename), m_synchronousMode(false), m_trail(nullptr)
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

FastLogger::~FastLogger()
{
	gQueue->Remove(this);
}

void FastLogger::Initialize(const std::filesystem::path& directory, const std::tstring& filename)
{
	gGlobalLogger.GetOrCreate(directory, filename);
}

FastLogger& FastLogger::GetGlobalLogger()
{
	return gGlobalLogger.Get();
}

void FastLogger::StopFlushingQueue()
{
	gQueue->Stop();
}

void FastLogger::SwitchToSynchronousMode()
{
	m_synchronousMode = true;
}

void FastLogger::Add(FastEntry* pEntry)
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

void FastLogger::Flush()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);

	FastEntry* pTrail = m_trail;
	while(!m_trail.compare_exchange_weak(pTrail, nullptr))
	{
	}

	FastEntry* pHead = nullptr;
	for (FastEntry* pCurrent = pTrail; nullptr != pCurrent;)
	{
		FastEntry* pNext = pCurrent->Link;
		pCurrent->Link = pHead;
		pHead = pCurrent;
		pCurrent = pNext;
	}

	for (FastEntry* pCurrent = pHead; nullptr != pCurrent;)
	{
		Write(*pCurrent);
		FastEntry* pTemp = pCurrent;
		pCurrent = pCurrent->Link;
		delete pTemp;
	}
}

void FastLogger::Write(FastEntry& entry)
{
	try
	{
		ReopenIfNeeded(entry.TimePoint);
		std::ostringstream stream;
		stream << DateTime(entry.TimePoint);
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

void FastLogger::ReopenIfNeeded(SystemClock::time_point tp)
{
	typedef std::chrono::duration<int32_t, std::ratio<24 * 3600> > days;
	days date = std::chrono::duration_cast<days>(tp.time_since_epoch());
	if (date.count() <= m_date)
	{
		return;
	}

	DateTime dt(tp);

	tostringstream stream;
	stream << std::setw(4) << std::setfill(L'0') << dt.Year;
	stream << '-';
	stream << std::setw(2) << std::setfill(L'0') << dt.Month;
	stream << '-';
	stream << std::setw(2) << std::setfill(L'0') << dt.Day;
	stream << '_' << m_filename << ".log";

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
