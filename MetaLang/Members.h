//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "MemberInfo.h"

class Members
{
public:
	void Add(const MemberInfo& info);

public:
	const std::vector<MemberInfo>& GetMembers() const;

private:
	std::unordered_set<std::string> m_names;
	std::vector<MemberInfo> m_members;
};