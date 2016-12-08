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

public:
	void Clear();
	void SetSize(const uint32_t newSize);
	const uint8_t* GetData() const;
	uint8_t* GetData();

public:
	void SetPosition(const uint32_t newPosition);

public:
	void Write(const std::wstring& value);
	void Write(const std::string& value);
	void Write(const uint32_t size, const void* pData);
	template<typename T> void Write(const T& value);

public:
	void Read(const uint32_t size, void* pData);

private:
	void EnsureSize(const uint32_t additionalSize);
	void EnsureCapacity(const uint32_t totalSize);

private:
	uint8_t* _data = nullptr;
	uint32_t _position = 0;
	uint32_t _size = 0;
	uint32_t _capacity = sizeof(_buffer);

private:
	uint8_t _buffer[4096];
};

double ReadDouble(MemoryStream& stream);

template<typename T> void MemoryStream::Write(const T& value)
{
	static_assert(std::is_pod<T>::value | std::is_fundamental<T>::value, "Only POD and fundamental types");
	EnsureSize(sizeof(T));
	T* pointer = reinterpret_cast<T*>(_data + _position);
	*pointer = value;
	_position += sizeof(T);

}