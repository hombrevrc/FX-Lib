//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Members.h"


void Members::Add(const MemberInfo& info)
{
	const std::string& name = info.GetName();
	if (m_names.count(name) > 0)
	{
		throw std::runtime_error("Duplicate name = " + name);
	}
	m_names.insert(name);
	m_members.push_back(info);
}

const std::vector<MemberInfo>& Members::GetMembers() const
{
	return m_members;
}
