//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "FastStream.h"


FastStream::FastStream() :
	m_pMaxSize(nullptr), m_pEntry(new FastEntry(0))
{
}


FastStream::FastStream(uint32_t& size) :
	m_pMaxSize(&size), m_pEntry(new FastEntry(size))
{
}

FastStream::~FastStream()
{
	if (nullptr != m_pEntry)
	{
		delete m_pEntry;
		m_pEntry = nullptr;
	}
}
