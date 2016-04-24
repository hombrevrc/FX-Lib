//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class IFastHelper
{
public:
	virtual uint32_t SizeOf() const= 0;
	virtual void MoveTo(uint8_t* pAddress) = 0;
	virtual void Format(std::ostream& stream) const = 0;
	virtual void Delete() = 0;
};