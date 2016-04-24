//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "ChannelPtr.h"
#include "Channel.h"


ChannelPtr::ChannelPtr() : m_channel(nullptr)
{
}

void ChannelPtr::Store(Channel* pChannel)
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	m_channel = pChannel;
}

Channel* ChannelPtr::Load()
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	return m_channel;
}

Channel* ChannelPtr::Exchange(Channel* pChannel)
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	Channel* result = m_channel;
	m_channel = pChannel;
	return result;
}

void ChannelPtr::Send(const MemoryStream& stream)
{
	std::lock_guard<std::mutex> lock(m_synchronizer);
	if (nullptr != m_channel)
	{
		m_channel->Send(stream);
	}
}
