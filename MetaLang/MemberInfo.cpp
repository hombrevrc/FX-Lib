//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "MemberInfo.h"


MemberInfo::MemberInfo(const std::string name, const TypeInfo& type) : m_name(name), m_type(type)
{
}

const std::string& MemberInfo::GetName() const
{
	return m_name;
}

const TypeInfo& MemberInfo::GetType() const
{
	return m_type;
}

Attributes& MemberInfo::GetAttributes()
{
	return m_attributes;
}

const Attributes& MemberInfo::GetAttributes() const
{
	return m_attributes;
}
