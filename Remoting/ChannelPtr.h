//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Channel;

class ChannelPtr
{
public:
	ChannelPtr();

public:
	void Store(Channel* pChannel);
	Channel* Load();
	Channel* Exchange(Channel* pChannel);

public:
	void Send(const MemoryStream& stream);

private:
	Channel* m_channel;
	std::mutex m_synchronizer;
};