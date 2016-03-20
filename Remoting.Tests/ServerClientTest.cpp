//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ServerClientTest.h"


ServerClientTest::ServerClientTest(IBuilder& builder) : m_builder(builder)
{
}

void ServerClientTest::RunSimple()
{
	m_acceptor.reset(m_builder.CreateAcceptor());
	m_connector.reset(m_builder.CreateConnector());
	m_server = std::make_unique<Server>(*m_acceptor, m_serverThreadPool);
	m_client = std::make_unique<Client>(*m_connector, m_clientThreadPool);

	m_server->SetCreateChannelHandler(ChannelHandler(this, &ServerClientTest::OnCreateChannel));
	m_server->SetMessageHandler(MessageHandler(this, &ServerClientTest::OnServerMessage));
	m_server->SetDeleteChannelHandler(ChannelHandler(this, &ServerClientTest::OnDeleteChannel));

	m_client->SetMessageHandler(MessageHandler(this, &ServerClientTest::OnClientMessage));

	m_server->Start();
	std::this_thread::sleep_for(std::chrono::seconds(1));

	m_client->Start();
	std::this_thread::sleep_for(std::chrono::seconds(5));

	m_client->Stop();

	std::this_thread::sleep_for(std::chrono::seconds(1));

	m_server->Stop();
}

void ServerClientTest::OnCreateChannel(Server& server, Channel& channel)
{
	server;
	channel;
}

void ServerClientTest::OnServerMessage(Channel& channel, const std::shared_ptr<MemoryStream>& stream)
{
	channel;
	stream;
}

void ServerClientTest::OnDeleteChannel(Server& server, Channel& channel)
{
	server;
	channel;
}

void ServerClientTest::OnClientMessage(Channel& channel, const std::shared_ptr<MemoryStream>& stream)
{
	channel;
	stream;
}
