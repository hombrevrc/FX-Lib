//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "BinStream.h"


BinStream::BinStream() :
	m_pMaxSize(nullptr), m_pEntry(new BinEntry(nullptr, 0))
{
}

BinStream::BinStream(const char* type) :
	m_pMaxSize(nullptr), m_pEntry(new BinEntry(type, 0))
{
}

BinStream::BinStream(const char* type, uint32_t& size) :
	m_pMaxSize(&size), m_pEntry(new BinEntry(type, size))
{
}

BinStream::~BinStream()
{
	if (nullptr != m_pEntry)
	{
		delete m_pEntry;
		m_pEntry = nullptr;
	}
}

void BinStream::operator >> (BinLogger& logger)
{
	if (nullptr != m_pMaxSize)
	{
		*m_pMaxSize = m_pEntry->GetSize();
	}
	logger.Add(m_pEntry);
	m_pEntry = nullptr;
}

void BinStream::operator >> (std::ostream& stream)
{
	if (nullptr != m_pMaxSize)
	{
		*m_pMaxSize = m_pEntry->GetSize();
	}
	m_pEntry->Format(stream);
}
