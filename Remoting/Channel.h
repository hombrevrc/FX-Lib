//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "ITransport.h"
#include "EndPoint.h"

class Channel
{
public:
	Channel(EndPoint& endPoint, ITransport* pChannel);
	Channel(const Channel&) = delete;
	Channel& operator = (const Channel&) = delete;
	~Channel();

public:
	EndPoint& GetEndPoint();
	ITransport& GetTransport();

public:
	void Send(const MemoryStream& stream);

private:
	void Run();
	void Loop();
	void Step();

private:
	bool ShouldWrite() const;
	void TryToWrite();
	void TryToRead();
	bool TryToReadSize();
	bool TryToReadData();
	void DoRead();
	MemoryStream& GetMemoryStream();

private:
	EndPoint& m_endPoint;
	ITransport* m_pChannel;

private:
	mutable std::mutex m_synchronizer;
	MemoryStream* m_pPending = nullptr;
	MemoryStream* m_pWriting = nullptr;
	MemoryStream m_first;
	MemoryStream m_second;
	std::shared_ptr<MemoryStream> m_message;

private:
	bool m_isSelfDestruction = false;
	std::atomic<bool> m_continue;
	std::thread m_thread;
};