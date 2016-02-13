//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#pragma once

#include "PipeWriter.h"
#include "PipeReader.h"

class PipeChannel : public IChannel
{
public:
	PipeChannel(Pipe&& pipe);
	PipeChannel(Pipe&& reader, Pipe&& writer);
	PipeChannel(const PipeChannel&) = delete;
	PipeChannel& operator = (const PipeChannel&) = delete;
	~PipeChannel();

public:
	virtual void Accept() override;
	virtual ChannelState Join(const bool reading, const bool writing) override;
	virtual uint32_t Write(const uint32_t size, const void* pBuffer) override;
	virtual uint32_t Read(const uint32_t size, void* pBuffer) override;
	virtual void WakeUp() override;

private:
	void JoinForReading();
	void JoinForWriting();
	void JoinForReadingOrWriting();

private:
	void Construct();

private:
	HANDLE m_event = nullptr;
	Pipe m_pipe;
	PipeReader m_reader;
	PipeWriter m_writer;
};