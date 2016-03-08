//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Pipe
{
public:
	Pipe();
	Pipe(Pipe&& pipe);
	Pipe& operator = (Pipe&& pipe);
	Pipe(const Pipe&) = delete;
	Pipe& operator = (const Pipe&) = delete;
	~Pipe();

public:
	static Pipe CreateAcceptor(const std::wstring& name);
	static Pipe CreateWriter(const std::wstring& name);
	static Pipe CreateReader(const std::wstring& name);
	static Pipe Open(const std::wstring& name, const DWORD mode, const uint32_t timeoutInMs);
	static Pipe OpenReader(const std::wstring& name, const uint32_t timeoutInMs);
	static Pipe OpenWriter(const std::wstring& name, const uint32_t timeoutInMs);

public:
	HANDLE GetEventHandle();

public:
	bool Accept(HANDLE signaledEvent);
	bool Accept(HANDLE signaledEvent, const uint32_t timeoutInMs);

public:
	uint32_t Write(const uint32_t size, const void* pBuffer);
	void Write(MemoryStream& stream);
	uint32_t Read(const uint32_t size, void* pBuffer);
	bool GetOverlappedResult();
	bool GetOverlappedResult(uint32_t& transferredBytes);
	void Join();
	void WakeUp();

private:
	void AcquireFrom(Pipe& pipe);
	void Finalize();

private:
	HANDLE m_pipe = INVALID_HANDLE_VALUE;
	OVERLAPPED m_overlapped;
};