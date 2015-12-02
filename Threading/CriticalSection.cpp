//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&m_section);
}

CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_section);
}

void CriticalSection::Acquire()
{
	EnterCriticalSection(&m_section);
}

void CriticalSection::Release()
{
	LeaveCriticalSection(&m_section);
}
