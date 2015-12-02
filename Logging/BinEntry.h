//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

typedef const uint8_t* (*FormatFunc)(const uint8_t* pData, std::ostream& stream);

class BinEntry
{
public:
	BinEntry* Link;

public:
	const char* Type;
	uint64_t TimeStamp;
	uint32_t ThreadId;

public:
	BinEntry(const char* type, const uint32_t size);
	BinEntry(const BinEntry&) = delete;
	BinEntry& operator = (const BinEntry&) = delete;
	~BinEntry();

public:
	void Format(std::ostream& stream) const;
	uint32_t GetSize() const;
	void AcquireCurrentTime();

public:
	template<typename T> inline void Write(const T& arg)
	{
		DoWrite(arg);
	}
	void Write(const std::string& st);
	void Write(const std::wstring& st);
	void Write(std::ostream& (*arg)(std::ostream&));
	void Write(std::ios_base& (*arg)(std::ios_base&));

public:
	void Write(FormatFunc func, const void* pData, const uint32_t size);

private:
	template<typename T> inline void DoWrite(const T& arg)
	{
		const uint32_t newSize = m_size + sizeof(FormatFunc) + sizeof(T);
		if ((newSize <= m_capacity) || ReallocIfNeeded(newSize))
		{
			FormatFunc* pFunc = reinterpret_cast<FormatFunc*>(m_pData + m_size);
			*pFunc = &BinEntry::DoFormat<T>;
			T* pData = reinterpret_cast<T*>(1 + pFunc);
			*pData = arg;
			m_size = newSize;
		}
	}

	template<typename T> static const uint8_t* DoFormat(const uint8_t* pData, std::ostream& stream)
	{
		const T* pTypedData = reinterpret_cast<const T*>(pData);
		stream << (*pTypedData);
		const uint8_t* result = reinterpret_cast<const uint8_t*>(1 + pTypedData);
		return result;
	}

	static const uint8_t* DoAStringFormat(const uint8_t* pData, std::ostream& stream);
	static const uint8_t* DoWStringFormat(const uint8_t* pData, std::ostream& stream);

private:
	bool ReallocIfNeeded(const uint32_t newSize);

private:
	uint8_t* m_pData;
	uint32_t m_size;
	uint32_t m_capacity;
};

#pragma region inline methods

inline uint32_t BinEntry::GetSize() const
{
	return m_size;
}

inline void BinEntry::Write(const std::string& st)
{
	Write(&BinEntry::DoAStringFormat, st.c_str(), static_cast<uint32_t>(1 + st.length()));
}

inline void BinEntry::Write(const std::wstring& st)
{
	Write(&BinEntry::DoWStringFormat, st.c_str(), static_cast<uint32_t>(sizeof(wchar_t)* (1 + st.length())));
}

inline void BinEntry::Write(std::ostream& (*arg)(std::ostream&))
{
	DoWrite(arg);
}

inline void BinEntry::Write(std::ios_base& (*arg)(std::ios_base&))
{
	DoWrite(arg);
}

#pragma endregion
