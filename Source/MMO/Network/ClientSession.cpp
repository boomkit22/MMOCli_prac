// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ClientSession.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"


ClientSession::ClientSession()
{
}

ClientSession::ClientSession(FSocket* socket)
{
	Socket = socket;
}

ClientSession::~ClientSession()
{
	if (RecvWorker)
	{
		RecvWorker->Shutdown();
		RecvWorker = nullptr;
	}

	if (SendWorker)
	{
		SendWorker->Shutdown();
		RecvWorker = nullptr;
	}
}

void ClientSession::Connect()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}

	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Socket"));
	FIPv4Address Ip;
	FIPv4Address::Parse(IpText, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);

	Connected = Socket->Connect(*InternetAddr);

	if (Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Success")));

		StartNetwork();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Failed")));
	}

}

void ClientSession::StartNetwork()
{
	RecvWorker = MakeShared<RecvThread>(this, Socket);
	SendWorker = MakeShared<SendThread>(Socket, &SendBuffer);

	/*RecvWorker = new RecvThread(this, Socket);
	SendWorker = new SendThread(Socket, &SendBuffer);*/
}


void ClientSession::SendPacket(CPacket* packet)
{
	int enqueueVal = SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());
}



void ClientSession::Disconnect()
{
	if (RecvWorker)
	{
		RecvWorker->Shutdown();
		RecvWorker = nullptr;
	}

	if (SendWorker)
	{
		SendWorker->Shutdown();
		SendWorker = nullptr;
	}

	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

