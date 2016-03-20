//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IBuilder.h"

class AcceptorConnectorTest
{
public:
	AcceptorConnectorTest(IBuilder& builder);
	AcceptorConnectorTest(const AcceptorConnectorTest&) = delete;
	AcceptorConnectorTest& operator = (const AcceptorConnectorTest&) = delete;

public:
	void RunSimple();
	void RunSimpleAndRead();
	void RunSimpleAndRead2();
	void RunSimpleAndRead3();

private:
	void Connect();
	void DoConnect(IAcceptor* pAcceptor, ITransport** ppServer);
	void DoWrite(const uint32_t size, const void* buffer, uint32_t* pWritten);
	void DoRead(const uint32_t size, void* buffer, uint32_t* pRead);

private:
	IBuilder& m_builder;
	ThreadPool m_thread;

private:
	std::unique_ptr<ITransport> m_server;
	std::unique_ptr<ITransport> m_client;
};