//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IConnector.h"
#include "EndPoint.h"

class Client : public EndPoint
{
public:
	Client(IConnector& connectror, ThreadPool& threadPool);
	Client(const Client&) = delete;
	Client& operator = (const Client&) = delete;
	~Client();

public:
	void Start();
	void Stop();

public:
	virtual void Initialize(Channel* pChannel) override;
	virtual bool Finalize(Channel* pChannel) override;
	virtual MessageHandler GetMessageHandler() override;
	virtual ThreadPool& GetThreadPool() override;

public:
	void SetMessageHandler(MessageHandler handler);

private:
	void Loop();
	void Step();
	void DoStep();
	void Finalize();

private:
	IConnector& m_connectror;
	ThreadPool& m_threadPool;

private:
	std::mutex m_synchronizer;

private:
	MessageHandler m_message;
	std::atomic<Channel*> m_channel;

private:
	std::thread m_thread;
	std::atomic<bool> m_continue;
	StdSemaphore m_event;
};