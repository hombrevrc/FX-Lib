//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

template<typename TBase> class TaskBase : public TBase
{
public:
	virtual void Invoke() = 0;
	virtual ~TaskBase()
	{
	}
};

template<> class TaskBase < void >
{
public:
	virtual void Invoke() = 0;
	virtual ~TaskBase()
	{
	}
};
