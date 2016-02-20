//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "BinEntry.h"
#include "BinLogger.h"

class BinStream
{
public:
	BinStream();
	BinStream(const char* type);
	BinStream(const char* type, uint32_t& size);
	BinStream(const BinStream&) = delete;
	BinStream& operator = (const BinStream&) = delete;
	~BinStream();

public:
	template<typename T> BinStream& operator << (const T& value)
	{
		static_assert(std::is_fundamental<T>::value | std::is_enum<T>::value |
			std::is_pointer<T>::value | std::is_pod<T>::value, "Only fundamental and enum types");
		m_pEntry->Write(value);
		return *this;
	}
	inline BinStream& operator << (const void* pointer)
	{
		m_pEntry->Write(pointer);
		return *this;
	}
	inline BinStream& operator << (const char* st)
	{
		m_pEntry->Write(st);
		return *this;
	}
	inline BinStream& operator << (const std::string& st)
	{
		m_pEntry->Write(st);
		return *this;
	}
	inline BinStream& operator << (const std::wstring& st)
	{
		m_pEntry->Write(st);
		return *this;
	}
	inline BinStream& operator << (std::ostream& (*arg)(std::ostream&))
	{
		m_pEntry->Write(arg);
		return *this;
	}
	inline BinStream& operator << (std::ios_base& (*arg)(std::ios_base&))
	{
		m_pEntry->Write(arg);
		return *this;
	}

public:
	void operator >> (BinLogger& logger);
	void operator >> (BinLogger* pLogger);
	void operator >> (std::ostream& stream);

public:
	inline void Write(FormatFunc func, const void* pData, const uint32_t size)
	{
		m_pEntry->Write(func, pData, size);
	}
	template<typename T> inline void Write(const T& arg)
	{
		m_pEntry->Write(arg);
	}
	inline BinEntry* GetEntry()
	{
		return m_pEntry;
	}

private:
	uint32_t* m_pMaxSize;
	BinEntry* m_pEntry;
};


#define BIN_STREAM_SIZE_HINT2(x, y) x##y
#define BIN_STREAM_SIZE_HINT(x, y) BIN_STREAM_SIZE_HINT2(x, y)
#define BIN_STREAM(type) static uint32_t BIN_STREAM_SIZE_HINT(__binStreamSizeHint, __LINE__) = 0; BinStream(type, BIN_STREAM_SIZE_HINT(__binStreamSizeHint, __LINE__))