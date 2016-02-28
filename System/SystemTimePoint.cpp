//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "SystemTimePoint.h"

SystemTimePoint::duration SystemTimePoint::time_since_epoch()
{
	SystemTimePoint::duration result(m_value);
	return result;
}
