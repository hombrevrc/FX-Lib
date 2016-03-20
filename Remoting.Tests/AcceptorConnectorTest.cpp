//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "AcceptorConnectorTest.h"


AcceptorConnectorTest::AcceptorConnectorTest(IBuilder& builder) : m_builder(builder)
{
}

void AcceptorConnectorTest::RunSimple()
{
	Connect();
}

void AcceptorConnectorTest::RunSimpleAndRead()
{
	Connect();
	Assert::IsTrue(0 == m_client->Read(0, nullptr));

	char ch;
	Assert::IsTrue(0 == m_client->Read(0, nullptr));
	Assert::IsTrue(0 == m_client->Read(sizeof(ch), &ch));
	Assert::IsTrue(0 == m_client->Read(sizeof(ch), &ch));

	m_client->WakeUp();
	Assert::IsTrue(0 == m_client->Read(sizeof(ch), &ch));

	try
	{
		m_server.reset();
		Assert::IsTrue(0 == m_client->Read(sizeof(ch), &ch));
	}
	catch (const std::exception& /*ex*/)
	{
	}
}

void AcceptorConnectorTest::RunSimpleAndRead2()
{
	if (m_thread.ContainsThisThread())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_client->WakeUp();
		return;
	}

	Connect();

	TaskWaiter waiter;
	Delegate<void()> func(this, &AcceptorConnectorTest::RunSimpleAndRead2);
	m_thread.InvokeEx(func, waiter);
	m_client->Join(true, false);

	char ch;
	Assert::IsTrue(0 == m_client->Read(sizeof(ch), &ch));
	waiter.Join();
}

void AcceptorConnectorTest::RunSimpleAndRead3()
{
	if (m_thread.ContainsThisThread())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		m_server.reset();
		return;
	}

	Connect();

	TaskWaiter waiter;
	Delegate<void()> func(this, &AcceptorConnectorTest::RunSimpleAndRead3);
	m_thread.InvokeEx(func, waiter);

	try
	{
		m_client->Join(true, false);
		char ch = 0;
		m_client->Read(sizeof(ch), &ch);
		Assert::Fail(L"exception is expected");
	}
	catch (const std::exception& /*ex*/)
	{
	}
	waiter.Join();
}

void AcceptorConnectorTest::Connect()
{
	m_server.reset();
	m_client.reset();

	std::unique_ptr<IAcceptor> acceptor(m_builder.CreateAcceptor());
	std::unique_ptr<IConnector> connector(m_builder.CreateConnector());

	ITransport* pServer = nullptr;
	TaskWaiter waiter;

	Delegate<void(IAcceptor*, ITransport**)> func(this, &AcceptorConnectorTest::DoConnect);
	m_thread.InvokeEx(func, waiter, acceptor.get(), &pServer);

	m_client.reset(connector->Connect());
	waiter.Join();
	m_server.reset(pServer);
}

void AcceptorConnectorTest::DoConnect(IAcceptor* pAcceptor, ITransport** ppServer)
{
	std::unique_ptr<ITransport> channel(pAcceptor->Accept());
	channel->Accept();
	*ppServer = channel.release();
}

void AcceptorConnectorTest::DoWrite(const uint32_t size, const void* buffer, uint32_t* pWritten)
{
	*pWritten = m_server->Write(size, buffer);
}

void AcceptorConnectorTest::DoRead(const uint32_t size, void* buffer, uint32_t* pRead)
{
	*pRead = m_server->Read(size, buffer);
}
