//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class FastEntry
{
public:
	FastEntry* Link;

public:
	const char* Type;
	SystemClock::time_point TimePoint;
	uint32_t ThreadId;

public:
	FastEntry(const uint32_t size);
	FastEntry& operator = (const FastEntry&) = delete;
	~FastEntry();

public:
	uint8_t* Allocate(const uint32_t size);
	void Free(const uint32_t size);
	void AcquireCurrentTime();
	void Format(std::ostream& stream) const;
	uint32_t GetSize() const;

private:
	FastEntry(FastEntry& entry);

private:
	uint8_t* m_pData;
	uint32_t m_size;
	uint32_t m_capacity;
};