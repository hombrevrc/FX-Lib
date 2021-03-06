//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IAcceptor.h"
#include "EndPoint.h"

class Server : public EndPoint
{
public:
	Server(IAcceptor& acceptor, ThreadPool& threadPool);
	Server(const Server&) = delete;
	Server& operator = (const Server&) = delete;
	~Server();

public:
	void SetCreateChannelHandler(ChannelHandler handler);
	void SetDeleteChannelHandler(ChannelHandler handler);
	void SetMessageHandler(MessageHandler handler);

public:
	void Start();
	void Stop();

public:
	bool IsStarted() const;

public:
	virtual void Initialize(Channel* pChannel) override;
	virtual bool Finalize(Channel* pChannel) override;
	virtual MessageHandler GetMessageHandler() override;
	virtual ThreadPool& GetThreadPool() override;

private:
	void Loop();

private:
	mutable std::mutex m_synchronizer;
	IAcceptor& m_acceptor;
	ThreadPool& m_threadPool;

private:
	ChannelHandler m_createChannel;
	ChannelHandler m_deleteChannel;
	MessageHandler m_message;

private:
	std::thread m_thread;
	std::vector<Channel*> m_channels;
};