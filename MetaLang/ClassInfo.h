//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class ClassInfo
{
public:
	ClassInfo();
	ClassInfo(const ClassInfo&) = delete;
	ClassInfo& operator = (const ClassInfo&) = delete;
	~ClassInfo();
};