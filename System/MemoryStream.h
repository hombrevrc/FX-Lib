//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class MemoryStream
{
public:
	MemoryStream();
	MemoryStream(const MemoryStream&) = delete;
	MemoryStream& operator = (const MemoryStream&) = delete;
	~MemoryStream();

public:
	const uint32_t GetCapacity() const;
	const uint32_t GetSize() const;
	const uint32_t GetPosition() const;
	void SetSize(const uint32_t newSize);
	const uint8_t* GetData() const;
	uint8_t* GetData();

public:
	void SetPosition(const uint32_t newPosition);

public:
	void Write(const std::wstring& value);
	void Write(const std::string& value);
	void Write(const uint32_t size, const void* pData);

public:
	void Read(const uint32_t size, void* pData);

private:
	void EnsureSize(const uint32_t additionalSize);
	void EnsureCapacity(const uint32_t totalSize);

private:
	uint8_t* m_data = nullptr;
	uint32_t m_position = 0;
	uint32_t m_size = 0;
	uint32_t m_capacity = sizeof(m_buffer);

private:
	uint8_t m_buffer[4096];
};

std::wstring ReadWString(MemoryStream& stream);
double ReadDouble(MemoryStream& stream);
