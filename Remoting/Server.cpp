//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Server.h"
#include "Channel.h"


Server::Server(IAcceptor& acceptor, ThreadPool& threadPool) : m_acceptor(acceptor), m_threadPool(threadPool)
{
}

Server::~Server()
{
	Stop();
}

void Server::SetCreateChannelHandler(ChannelHandler handler)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		throw std::runtime_error("Couldn't set create channel handler, if server is running");
	}
	m_createChannel = handler;
}

void Server::SetDeleteChannelHandler(ChannelHandler handler)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		throw std::runtime_error("Couldn't set close channel handler, if server is running");
	}
	m_deleteChannel = handler;
}

void Server::SetMessageHandler(MessageHandler handler)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		throw std::runtime_error("Couldn't set message handler, if server is running");
	}
	m_message = handler;
}

void Server::Start()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		throw std::runtime_error("Server is already running");
	}
	m_thread = std::thread(&Server::Loop, this);
}

void Server::Stop()
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		m_acceptor.StopAccepting();
		m_thread.join();
	}
}

bool Server::IsStarted() const
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	if (m_thread.joinable())
	{
		return true;
	}
	return false;
}

bool Server::Finalize(Channel* pChannel)
{
	std::unique_lock<std::mutex> lock(m_synchronizer);
	auto it = std::remove(m_channels.begin(), m_channels.end(), pChannel);
	if (m_channels.end() == it)
	{
		return false;
	}
	m_channels.erase(it, m_channels.end());
	return true;
}

MessageHandler Server::GetMessageHandler()
{
	return m_message;
}

void Server::Loop()
{
	for (;;)
	{
		std::unique_ptr<ITransport> stream(m_acceptor.Accept());
		if (!stream)
		{
			break;
		}

		std::unique_ptr<Channel> channel(new Channel(*this, stream.get()));
		m_channels.reserve(1 + m_channels.size());
		m_createChannel(*this, *channel);
		m_channels.push_back(channel.get());
		channel.release();
		stream.release();
	}
}

void Server::Initialize(Channel* pChannel)
{
	ITransport& transport = pChannel->GetTransport();
	transport.Accept();
}

ThreadPool& Server::GetThreadPool()
{
	return m_threadPool;
}
