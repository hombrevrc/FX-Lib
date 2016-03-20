//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Client.h"
#include "Channel.h"


Client::Client(IConnector& connectror, ThreadPool& threadPool) :
	m_connectror(connectror), m_threadPool(threadPool), m_continue(false), m_event(0, 1), m_channel(nullptr)
{
}

Client::~Client()
{
	Stop();
}

void Client::Start()
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	m_continue = true;
	if (m_thread.joinable())
	{
		throw std::runtime_error("Client is already running");
	}
	m_thread = std::thread(&Client::Loop, this);
}

void Client::Stop()
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	m_continue = false;
	Finalize();
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void Client::Loop()
{
	for (; m_continue; m_event.Acquire())
	{
		Step();
	}
}

void Client::Step()
{
	try
	{
		DoStep();
	}
	catch (const std::exception& /*ex*/)
	{
	}
}

void Client::DoStep()
{
	Finalize();

	try
	{
		std::unique_ptr<ITransport> stream(m_connectror.Connect());
		Channel* pChannel = new Channel(*this, stream.get());
		m_channel.store(pChannel);
		stream.release();
	}
	catch (const std::exception& /*ex*/)
	{
	}
}

bool Client::Finalize(Channel* pChannel)
{
	Channel* pCurrent = m_channel.exchange(nullptr);
	const bool result = (pChannel == pCurrent);
	m_event.Release();
	return result;
}

void Client::Finalize()
{
	Channel* pChannel = m_channel.exchange(nullptr);
	if (nullptr != pChannel)
	{
		delete pChannel;
		pChannel = nullptr;
	}
}

void Client::Initialize(Channel* pChannel)
{
	pChannel;
}

MessageHandler Client::GetMessageHandler()
{
	return m_message;
}

ThreadPool& Client::GetThreadPool()
{
	return m_threadPool;
}

void Client::SetMessageHandler(MessageHandler handler)
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		throw std::runtime_error("Couldn't set message handler, if client is running");
	}
	m_message = handler;
}
