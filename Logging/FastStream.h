//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "FastHelper.h"
#include "FastEntry.h"

#ifdef new
#define FX_LIB_NEW
#pragma push_macro("new")
#undef new
#endif


class FastStream
{
public:
	explicit FastStream();
	explicit FastStream(uint32_t& size);
	FastStream(const FastStream&) = delete;
	FastStream& operator = (const FastStream&) = delete;
	~FastStream();

public:
	template<typename T> void Write(const T& value)	const
	{
		const uint32_t size = FastHelper<T>::CalculateSizeOf();
		try
		{
			uint8_t* pAddress = m_pEntry->Allocate(size);
			if (nullptr != pAddress)
			{
				new(pAddress) FastHelper<T>(value);
			}
		}
		catch (...)
		{
			m_pEntry->Free(size);
		}
	}

public:
	template<typename T> inline void operator >> (T& logger)
	{
		if (nullptr != m_pMaxSize)
		{
			*m_pMaxSize = m_pEntry->GetSize();
		}
		logger(m_pEntry);
		m_pEntry = nullptr;
	}

	template<typename T> inline void operator >> (T&& logger)
	{
		(*this) >> logger;
	}

private:
	uint32_t* m_pMaxSize;
	mutable FastEntry* m_pEntry;
};

#ifdef FX_LIB_NEW
#pragma pop_macro("new")
#undef FX_LIB_NEW
#endif

template<typename T> inline FastStream& operator << (FastStream& stream, const T& value)
{
	static_assert(std::is_fundamental<T>::value | std::is_enum<T>::value | std::is_pointer<T>::value | std::is_pod<T>::value, "Only primitive types");
	stream.Write(value);
	return stream;
}

template<typename T> inline FastStream& operator << (FastStream&& stream, const T& value)
{
	return (stream << value);
}

#define FX_LIB_FAST_STREAM2(type, param)	inline FastStream& operator << (FastStream& stream, param value)\
											{\
												stream.Write<type>(value);\
												return stream;\
											}

#define FX_LIB_FAST_STREAM(type) FX_LIB_FAST_STREAM2(type, const type&)

FX_LIB_FAST_STREAM2(const char*, const char*)
FX_LIB_FAST_STREAM2(const wchar_t*, const wchar_t*)
FX_LIB_FAST_STREAM(std::string)


#define FAST_STREAM_SIZE_HINT2(x, y) x##y
#define FAST_STREAM_SIZE_HINT(x, y) FAST_STREAM_SIZE_HINT2(x, y)
#define FAST_STREAM() static uint32_t FAST_STREAM_SIZE_HINT(__fastStreamSizeHint, __LINE__) = 0; FastStream(FAST_STREAM_SIZE_HINT(__fastStreamSizeHint, __LINE__))


