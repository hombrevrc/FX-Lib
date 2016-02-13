//==============================================================
// Copyright (c) 2016 by Viktar Marmysh mailto:marmysh@gmail.com
//==============================================================

#include "stdafx.h"


const std::wstring cPipeName = L"MyPipeName";

IChannel* pClient = nullptr;



void Accept()
{
	PipeAcceptor acceptor(cPipeName);
	IChannel* pServer = acceptor.Accept();
	pServer->Accept();

	for (;;)
	{
		ChannelState state = pServer->Join(true, true);
		char ch = 0;
		uint32_t written =  pServer->Write(sizeof(ch), &ch);
		if (0 == written)
		{
			char buffer[128 * 1024];
			uint32_t read = pClient->Read(sizeof(buffer), buffer);
			continue;
		}
	}



	delete pServer;
}


void Connect()
{
	PipeConnector connector(cPipeName);
	pClient = connector.Connect();
}

void Connect2()
{
	PipeConnector connector(cPipeName);
	IChannel* pClient = connector.Connect();
	pClient = connector.Connect();
}


void Test()
{

	ThreadPool thread;
	Delegate<void()> func = Accept;
	TaskWaiter waiter;
	thread.InvokeEx(func, waiter);

	Connect();


	waiter.Join();

}


void PipeAccept(Pipe* pPipe)
{
	HANDLE event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	pPipe->Accept(event);
	CloseHandle(event);
}

void WriteData(Pipe* pipe)
{
	Sleep(10000);


	char ch = 0;

	for (;;)
	{
		pipe->Write(sizeof(ch), &ch);
		ch++;
		Sleep(1000);
	}
}



int main()
{
	ThreadPool thread;

	Pipe writer = Pipe::CreateWriter(cPipeName);

	Delegate<void(Pipe*)> func = PipeAccept;
	thread.Invoke(func, &writer);

	Pipe reader = Pipe::OpenReader(cPipeName, 5000);

	func = WriteData;
	thread.Invoke(func, &writer);

	char buffer[64 * 1024];
	char* pData = buffer;
	uint32_t count = sizeof(buffer);

	


	for (; count > 0;)
	{
		uint32_t size = reader.Read(count, pData);

		if (size > 0)
		{
			pData += size;
			count -= size;
			std::cout << "size = " << size << std::endl;
			continue;
		}


		Sleep(5000);

		reader.Join();

		uint32_t transmittedBytes = 0;
		reader.GetOverlappedResult(transmittedBytes);
		if (transmittedBytes > 0)
		{
			pData += transmittedBytes;
			count -= transmittedBytes;
		}

		std::cout << "transmittedBytes = " << transmittedBytes << std::endl;

	}



}