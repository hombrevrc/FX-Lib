//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "TaskBase.h"
#include "TypeTraits.h"

template<typename ... TArgs> class TaskArgs;

template<> class TaskArgs < >
{
public:
	template<typename TFunc> inline void Invoke(TFunc& func)
	{
		func();
	}

};

template<typename T> class TaskArgs<T>
{
public:
	template<typename T2> TaskArgs(const T2& arg) : m_arg(arg)
	{
	}

public:
	template<typename TFunc, typename ... TArgs2> inline void Invoke(TFunc& func, TArgs2&... args)
	{
		func(args..., m_arg);
	}

private:
	typename TypeTraits<T>::MemberType m_arg;
};

template<typename T, typename ... TArgs> class TaskArgs < T, TArgs... >
{
public:
	template<typename T2, typename ... TArgs2> TaskArgs(const T2& arg, const TArgs2&... args) : m_arg(arg), m_args(args...)
	{
	}

public:
	template<typename TFunc, typename ... TArgs2> inline void Invoke(TFunc& func, TArgs2&... args)
	{
		m_args.Invoke(func, args..., m_arg);
	}
private:
	typename TypeTraits<T>::MemberType m_arg;
	TaskArgs<TArgs...> m_args;
};

template<typename TBase, typename TFunc, typename ... TArgs> class Task : public TaskBase<TBase>
{
public:
	template<typename ... TArgs2> Task(const TFunc& func, const TArgs2&... args) : m_func(func), m_args(args...)
	{
	}

public:
	virtual void Invoke() override
	{
		m_args.Invoke(m_func);
	}

private:
	TFunc m_func;
	TaskArgs<TArgs...> m_args;
};
