//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "TypeInfo.h"
#include "Attributes.h"

class MemberInfo
{
public:
	MemberInfo(const std::string name, const TypeInfo& type);

public:
	Attributes& GetAttributes();

public:
	const std::string& GetName() const;
	const TypeInfo& GetType() const;
	const Attributes& GetAttributes() const;

private:
	std::string m_name;
	TypeInfo m_type;
	Attributes m_attributes;
};
