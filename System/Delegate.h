//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "Task.h"

class Generic
{
};

namespace Internal
{
	template <size_t Size> class FDelegateConverter;
	const int SIZE_OF_GENERIC_METHOD = sizeof(void (Generic::*)());
}
/// <summary>
/// Single inheritance
/// </summary>
namespace Internal
{
	template<> struct FDelegateConverter < SIZE_OF_GENERIC_METHOD >
	{
		template <typename T, class TMethod, class GenericMethod>
		inline static Generic* Convert(T *pThis, TMethod method, GenericMethod& genericMethod)
		{
			genericMethod = reinterpret_cast<GenericMethod>(method);
			return reinterpret_cast<Generic*>(pThis);
		}
	};
}

namespace Internal
{
	template<> class FDelegateConverter < SIZE_OF_GENERIC_METHOD + sizeof(int32_t) >
	{
	public:
		template <typename T, typename Method, typename GenericMethod>
		inline static Generic* Convert(T* pThis, Method method, GenericMethod& genericMethod)
		{
			union
			{
				Method func;
				struct
				{
					GenericMethod funcaddress;	// points to the actual member function
					int delta;					// #BYTES to be added to the 'this' pointer
				}s;
			} u;
			// Check that the horrible_cast will work
			static_assert(sizeof(method) == sizeof(u.s), "Incorrect horrible cast");
			u.func = method;
			genericMethod = u.s.funcaddress;
			return reinterpret_cast<Generic *>(reinterpret_cast<uint8_t*>(pThis)+u.s.delta);
		}
	};

	struct MicrosoftVirtualMFP
	{
		void (Generic::*codeptr)(); // points to the actual member function
		int delta;					// #bytes to be added to the 'this' pointer
		int vtable_index;			// or 0 if no virtual inheritance
	};

	struct GenericVirtual : virtual public Generic
	{
		typedef GenericVirtual * (GenericVirtual::*ProbePtrType)();
		GenericVirtual * GetThis() { return this; }
	};

	// __virtual_inheritance classes go here
	template<> class FDelegateConverter < SIZE_OF_GENERIC_METHOD + 2 * sizeof(int32_t) >
	{
	public:
		template <typename T, typename Method, typename GenericMethod>
		inline static Generic *Convert(T *pThis, Method method, GenericMethod &bound_func)
		{
			union
			{
				Method func;
				Generic* (T::*ProbeFunc)();
				MicrosoftVirtualMFP s;
			} u;

			u.func = method;
			bound_func = reinterpret_cast<GenericMethod>(u.s.codeptr);

			union
			{
				GenericVirtual::ProbePtrType virtfunc;
				MicrosoftVirtualMFP s;
			} u2;
			// Check that the horrible_cast<>s will work
			static_assert(sizeof(method) == sizeof(u.s) && sizeof(method) == sizeof(u.ProbeFunc) && sizeof(u2.virtfunc) == sizeof(u2.s), "Incorrect horrible cast");
			u2.virtfunc = &GenericVirtual::GetThis;
			u.s.codeptr = u2.s.codeptr;
			return (pThis->*u.ProbeFunc)();
		}
	};

}
namespace Internal
{
	template <> struct FDelegateConverter < SIZE_OF_GENERIC_METHOD + 2* sizeof(int) + sizeof(void*)>
	{
		template <class T, class Method, class GenericMethod>
		inline static Generic *Convert(T *pThis, Method method, GenericMethod &bound_func)
		{
			// The member function pointer is 16 bytes long. We can't use a normal cast, but
			// we can use a union to do the conversion.
			union
			{
				Method func;
				struct
				{
					GenericMethod funcaddress; // points to the actual member function
					int delta;		// #bytes to be added to the 'this' pointer
					int vtordisp;		// #bytes to add to 'this' to find the vtable
					int vtable_index; // or 0 if no virtual inheritance
				} s;
			} u;
			// Check that the horrible_cast will work
			static_assert(sizeof(Method) == sizeof(u.s), "Incorrect horrible cast");
			u.func = method;
			bound_func = u.s.funcaddress;
			int virtual_delta = 0;
			if (u.s.vtable_index)
			{ // Virtual inheritance is used
				// First, get to the vtable. 
				// It is 'vtordisp' bytes from the start of the class.
				const int * vtable = *reinterpret_cast<const int *const*>(reinterpret_cast<const char *>(pThis)+u.s.vtordisp);

				// 'vtable_index' tells us where in the table we should be looking.
				virtual_delta = u.s.vtordisp + *reinterpret_cast<const int *>(reinterpret_cast<const char *>(vtable)+u.s.vtable_index);
			}
			// The int at 'virtual_delta' gives us the amount to add to 'this'.
			// Finally we can add the three components together. Phew!
			return reinterpret_cast<Generic *>(reinterpret_cast<char *>(pThis)+u.s.delta + virtual_delta);
		};
	};
}

template<typename Signature> class Delegate;

template<typename R, typename ... TArgs> class Delegate < R(TArgs...) >
{
public:
	typedef R Signature(TArgs...args);
	typedef R(Generic::*GenericMethod)(TArgs...args);

public:
	inline Delegate() : m_pThis(nullptr), m_method(nullptr)
	{
	}

	template<typename TThis, typename TClass> Delegate(TThis* pThis, R(TClass::*method)(TArgs...args)) : m_pThis(nullptr), m_method(nullptr)
	{
		TClass* pClassThis = pThis;
		m_pThis = Internal::FDelegateConverter<sizeof(method)>::Convert(pClassThis, method, m_method);
	}

	template<typename TThis, typename TClass> Delegate(TThis* pThis, R(TClass::*method)(TArgs...args) const) : m_pThis(nullptr), m_method(nullptr)
	{
		TClass* pClassThis = pThis;
		m_pThis = Internal::FDelegateConverter<sizeof(method)>::Convert(pClassThis, method, m_method);
	}

public:

#ifndef _WIN64

public:
	Delegate(R(__cdecl * func)(TArgs...args))
	{
		Construct(func);
	}

	Delegate(R(__stdcall * func)(TArgs...args))
	{
		Construct(func);
	}

	Delegate(R(__fastcall * func)(TArgs...args))
	{
		Construct(func);
	}

#else
	Delegate(R (*func)(TArgs...args)) : m_pThis(nullptr), m_method(nullptr)
	{
		Construct(func);
	}
#endif

private:
	template<typename TFunc> void Construct(TFunc func)
	{
		m_pThis = reinterpret_cast<Generic*>(func);
		m_method = reinterpret_cast<GenericMethod>(&Delegate< R(TArgs...) >::InvokeFunction < TFunc >);
	}

	template<typename TFunc> R InvokeFunction(TArgs... args)
	{
		TFunc func = reinterpret_cast<TFunc>(this);
		return func(args...);
	}

public:
	inline R operator()(const TArgs&... args)
	{
		return (m_pThis->*m_method)(args...);
	}

public:
	template<typename TBase, typename ... TParams> TaskBase<TBase>* CreateTask(const TParams&... args) const
	{
		TaskBase<TBase>* result = new Task<TBase, Delegate<Signature>, TArgs...>(*this, args...);
		return result;
	}

public:
	static int32_t Compare(const Delegate& first, const Delegate& second)
	{
		if (first.m_pThis < second.m_pThis)
		{
			return -1;
		}
		if (first.m_pThis > second.m_pThis)
		{
			return 1;
		}

		if (first.m_method < second.m_method)
		{
			return -1;
		}

		if (first.m_method > second.m_method)
		{
			return 1;
		}

		return 0;
	}

public:
	inline bool operator == (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison == 0);
	}

	inline bool operator != (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison != 0);
	}

	inline bool operator < (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison < 0);
	}

	inline bool operator > (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison > 0);
	}

	inline bool operator <= (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison <= 0);
	}

	inline bool operator >= (const Delegate& other)
	{
		const int32_t comparison = Compare(*this, other);
		return (comparison > 0);
	}

private:
	Generic* m_pThis;
	GenericMethod m_method;
};
