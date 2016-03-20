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

void Channel::Send(MemoryStream& stream)
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
	m_endPoint.Initialize(this);
	Loop();
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
			//doRead();
		}
	}
}

bool Channel::TryToReadSize()
{
	const uint32_t position = m_message.GetPosition();
	if (position >= sizeof(uint32_t))
	{
		return true;
	}

	if (m_message.GetSize() < sizeof(uint32_t))
	{
		m_message.SetSize(sizeof(uint32_t));
	}

	uint8_t* pData = m_message.GetData() + position;
	uint32_t read = m_pChannel->Read(sizeof(uint32_t) - position, pData);

	const uint32_t newPosition = position + read;
	m_message.SetPosition(newPosition);

	if (newPosition < sizeof(uint32_t))
	{
		return false;
	}

	m_message.SetPosition(0);
	uint32_t size = 0;
	m_message.Read(sizeof(uint32_t), &size);
	m_message.SetSize(size + sizeof(uint32_t));
	m_message.Write(sizeof(uint32_t), &size);
	return true;
}

bool Channel::TryToReadData()
{
	const uint32_t size = m_message.GetSize();
	const uint32_t position = m_message.GetPosition();
	uint8_t* pData = m_message.GetData() + position;
	uint32_t read = m_pChannel->Read(size - position, pData);

	const uint32_t newPosition = position + read;
	m_message.SetPosition(newPosition);
	if (newPosition >= size)
	{
		return true;
	}
	return false;
}
