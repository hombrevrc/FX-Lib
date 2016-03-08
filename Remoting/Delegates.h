//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

class Server;
class Channel;

typedef Delegate<void (Server& server, Channel& channel)> ChannelHandler;
typedef Delegate<void (Channel& channel, const std::shared_ptr<MemoryStream>& stream)> MessageHandler;
