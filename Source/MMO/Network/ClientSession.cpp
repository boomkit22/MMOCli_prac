// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/ClientSession.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"


ClientSession::ClientSession()
{
}



ClientSession::~ClientSession()
{
	ClearSession();
}

bool ClientSession::Connect(FString IPText, int16 Port)
{
	//이렇게하면 Reconnect까지 일단 가능할것같은데
	ClearSession();

	//저장해 둘 필요는 있으려나
	_IpText = IPText;
	_Port = Port;

	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Socket"));
	// Connect
	FIPv4Address Ip;
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);
	Connected = Socket->Connect(*InternetAddr);

	//Connect 싪패했으면?
	/*
	* 로그인서버 Connect 실패? 서버 요청 에러 메시지
	* 게임 서버 Connect 실패? 서버 접속 에러 메시지
	* 채팅 서버 Connect 실패? 채팅보내는거 무시하면되나
	*/
	if (!Connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Failed")));
		return false;
	}

	StartNetwork();
	return true;
}



void ClientSession::StartNetwork()
{
	RecvWorker = MakeShared<RecvThread>(AsShared());
	SendWorker = MakeShared<SendThread>(AsShared());
}


void ClientSession::SendPacket(CPacket* packet)
{
	int enqueueVal = SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());
}



void ClientSession::Disconnect()
{
	ClearSession();
}



void ClientSession::ClearSession()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}

	if (RecvWorker)
	{
		RecvWorker->StopThread();
		RecvWorker = nullptr;
	}

	if (SendWorker)
	{
		SendWorker->StopThread();
		SendWorker = nullptr;
	}

	while (!RecvPacketQueue.IsEmpty())
	{
		CPacket* Packet = nullptr;
		if (RecvPacketQueue.Dequeue(Packet))
		{
			CPacket::Free(Packet);
		}
	}

	SendBuffer.ClearBuffer();
}


void ClientSession::NetworkDisconnect()
{
	if (Connected)
	{
		Connected = false;
		ClearSession();
		OnDisconnect();
	}
}
