//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "IBuilder.h"

class ServerClientTest
{
public:
	ServerClientTest(IBuilder& builder);
	ServerClientTest(const ServerClientTest&) = delete;
	ServerClientTest& operator = (const ServerClientTest&) = delete;

public:
	void RunSimple();

private:
	void OnCreateChannel(Server& server, Channel& channel);
	void OnServerMessage(Channel& channel, const std::shared_ptr<MemoryStream>& stream);
	void OnDeleteChannel(Server& server, Channel& channel);

private:
	void OnClientMessage(Channel& channel, const std::shared_ptr<MemoryStream>& stream);

private:
	IBuilder& m_builder;
	ThreadPool m_serverThreadPool;
	ThreadPool m_clientThreadPool;

private:
	std::unique_ptr<IAcceptor> m_acceptor;
	std::unique_ptr<IConnector> m_connector;
	std::unique_ptr<Server> m_server;
	std::unique_ptr<Client> m_client;
};