//==============================================================
// Copyright (c) 2015 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"
#include "Acceptor.h"

Acceptor::Acceptor(uint16_t /*port*/)
{
}

Socket Acceptor::Accept()
{
	return m_socket.Accept();
}

void Acceptor::Finalize()
{
	m_socket.Finalize();
}
