//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#define SAL_TYPE_BEGIN()		template<typename Visitor> void Visit(Visitor& visitor)\
								{\
									visitor.Begin(*this);

#define SAL_TYPE_MEMBER(member)		visitor.Visit(#member, member);

#define SAL_TYPE_END				visitor.End(*this);\
								}

