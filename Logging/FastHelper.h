//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IFastHelper.h"

#ifdef new
#define FX_LIB_NEW
#pragma push_macro("new")
#undef new
#endif



template<typename T>
class FastHelper : public IFastHelper
{
public:
	inline FastHelper(const T& value) : m_value(value)
	{
	}

	inline FastHelper(FastHelper&& helper) : m_value(std::move(helper.m_value))
	{
	}

public:
	FastHelper(const FastHelper&) = delete;
	FastHelper& operator = (const FastHelper&) = delete;

public:
	static constexpr uint32_t CalculateSizeOf()
	{
		return (sizeof(FastHelper<T>) + sizeof(void*) - 1) / sizeof(void*) * sizeof(void*);
	}

public:
	virtual uint32_t SizeOf() const override
	{
		return CalculateSizeOf();
	}

	virtual void MoveTo(uint8_t* pAddress) override
	{
		new(pAddress) FastHelper<T>(std::move(*this));
	}

	virtual void Format(std::ostream& stream) const override
	{
		stream << m_value;
	}

	virtual void Delete() override
	{
		m_value.~T();
	}

private:
	T m_value;
};

#ifdef FX_LIB_NEW
#pragma pop_macro("new")
#undef FX_LIB_NEW
#endif
