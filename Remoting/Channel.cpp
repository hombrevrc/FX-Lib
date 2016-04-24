//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Channel.h"
#include "Server.h"


Channel::Channel(EndPoint& endPoint, ITransport* pChannel) : m_endPoint(endPoint), m_pChannel(pChannel), m_continue(true)
{
	m_pPending = &m_first;
	m_pWriting = &m_second;
	m_thread = std::thread(&Channel::Run, this);
}

Channel::~Channel()
{
	if (m_isSelfDestruction)
	{
		m_thread.detach();
	}
	else
	{
		m_continue = false;
		m_pChannel->WakeUp();
		m_thread.join();
	}
}

EndPoint& Channel::GetEndPoint()
{
	return m_endPoint;
}

ITransport& Channel::GetTransport()
{
	return *m_pChannel;
}

void Channel::Send(const MemoryStream& stream)
{
	bool wakeUp = false;
	{
		std::unique_lock<std::mutex> lock(m_synchronizer);
		if (0 == m_pPending->GetSize())
		{
			wakeUp = true;
		}
		m_pPending->Write(stream.GetSize(), stream.GetData());
	}
	if (wakeUp)
	{
		m_pChannel->WakeUp();
	}
}

void Channel::Run()
{
	try
	{
		m_endPoint.Initialize(this);
		Loop();
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
}

void Channel::Loop()
{
	for (; m_continue;)
	{
		Step();
	}

	m_isSelfDestruction = m_endPoint.Finalize(this);
	if (m_isSelfDestruction)
	{
		delete this;
	}
}

void Channel::Step()
{
	const bool writing = ShouldWrite();
	m_pChannel->Join(true, writing);
	TryToWrite();
	TryToRead();
}

bool Channel::ShouldWrite() const
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_pWriting->GetPosition() < m_pWriting->GetSize())
	{
		return true;
	}
	if (m_pPending->GetPosition() < m_pPending->GetSize())
	{
		return true;
	}
	return false;
}

void Channel::TryToWrite()
{
	if (m_pWriting->GetPosition() >= m_pWriting->GetSize())
	{
		std::unique_lock<std::mutex> lock(m_synchronizer);
		std::swap(m_pWriting, m_pPending);
		m_pPending->SetSize(0);
		m_pWriting->SetPosition(0);
	}

	MemoryStream* pStream = m_pWriting;
	const uint32_t position = pStream->GetPosition();
	const uint32_t size = pStream->GetSize();
	const uint8_t* pData = pStream->GetData() + position;

	int32_t sent = m_pChannel->Write(size - position, pData);
	pStream->SetPosition(position + sent);
}

void Channel::TryToRead()
{
	if (TryToReadSize())
	{
		if (TryToReadData())
		{
			DoRead();
		}
	}
}

bool Channel::TryToReadSize()
{
	MemoryStream& message = GetMemoryStream();
	const uint32_t position = message.GetPosition();
	if (position >= sizeof(uint32_t))
	{
		return true;
	}

	if (message.GetSize() < sizeof(uint32_t))
	{
		message.SetSize(sizeof(uint32_t));
	}

	uint8_t* pData = message.GetData() + position;
	uint32_t read = m_pChannel->Read(sizeof(uint32_t) - position, pData);

	const uint32_t newPosition = position + read;
	message.SetPosition(newPosition);

	if (newPosition < sizeof(uint32_t))
	{
		return false;
	}

	message.SetPosition(0);
	uint32_t size = 0;
	message.Read(sizeof(uint32_t), &size);
	message.SetSize(size + sizeof(uint32_t));
	message.Write(sizeof(uint32_t), &size);
	return true;
}

bool Channel::TryToReadData()
{
	MemoryStream& message = GetMemoryStream();
	const uint32_t size = message.GetSize();
	const uint32_t position = message.GetPosition();
	uint8_t* pData = message.GetData() + position;
	uint32_t read = m_pChannel->Read(size - position, pData);

	const uint32_t newPosition = position + read;
	message.SetPosition(newPosition);
	if (newPosition >= size)
	{
		return true;
	}
	return false;
}

void Channel::DoRead()
{
	ThreadPool& thread = m_endPoint.GetThreadPool();
	MessageHandler handler = m_endPoint.GetMessageHandler();
	std::shared_ptr<MemoryStream> message = m_message;
	m_message.reset();
	message->SetPosition(0);
	thread.Invoke(handler, this, message);
}

MemoryStream& Channel::GetMemoryStream()
{
	if (!m_message)
	{
		m_message = std::make_shared<MemoryStream>();
	}
	return *m_message;
}
