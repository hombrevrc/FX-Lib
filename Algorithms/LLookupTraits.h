//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once


namespace Sal
{
	template<typename N> class LLookupNumericTraits;
}

namespace Sal
{
	template<> class LLookupNumericTraits<float>
	{
	public:
		const size_t operator() (const float value, const float delta, const size_t threshold)
		{
			return static_cast<size_t>(value / delta * threshold);
		}
	};

	template<> class LLookupNumericTraits<double>
	{
	public:
		const size_t operator() (const double value, const double delta, const size_t threshold)
		{
			return static_cast<size_t>(value / delta * threshold);
		}
	};

	template<> class LLookupNumericTraits<int32_t>
	{
	public:
		const size_t operator() (const double value, const double delta, const size_t threshold)
		{
			return static_cast<size_t>(value / delta * threshold);
		}
	};

	template<> class LLookupNumericTraits<uint32_t>
	{
	public:
		const size_t operator() (const double value, const double delta, const size_t threshold)
		{
			return static_cast<size_t>(value / delta * threshold);
		}
	};
}
