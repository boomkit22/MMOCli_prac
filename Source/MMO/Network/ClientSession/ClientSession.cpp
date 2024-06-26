// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSession/ClientSession.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "HAL/Runnable.h"

ClientSession::ClientSession()
{
}

ClientSession::~ClientSession()
{
}

bool ClientSession::Connect(FString IPText, int16 Port)
{
	//이렇게하면 Reconnect까지 일단 가능할것같은데
	Disconnect();

	//저장해 둘 필요는 있으려나
	_IpText = IPText;
	_Port = Port;

	// 소켓 생성
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Socket"));
	// Connect
	FIPv4Address Ip;
	FIPv4Address::Parse(IPText, Ip);

	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);
	bool connectSuccess = Socket->Connect(*InternetAddr);

	//Connect 싪패했으면?
	/*
	* 로그인서버 Connect 실패? 서버 요청 에러 메시지
	* 게임 서버 Connect 실패? 서버 접속 에러 메시지
	* 채팅 서버 Connect 실패? 채팅보내는거 무시하면되나
	*/
	if (!connectSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Failed")));
		return false;
	}

	Connected = true;
	StartNetwork();
	return true;
}



void ClientSession::StartNetwork()
{
	RecvWorker = new RecvThread(AsShared());
	SendWorker = new SendThread(AsShared());

}


void ClientSession::SendPacket(CPacket* packet)
{
	packet->Encode(0x77);
	int enqueueVal = SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());
	CPacket::Free(packet);
}



void ClientSession::Disconnect()
{
	if (Connected.Exchange(false))
	{
		ClearSession();
	}
}



void ClientSession::ClearSession()
{
	UE_LOG(LogTemp, Warning, TEXT("ClearSession"));

	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}

	if (RecvWorker)
	{
		UE_LOG(LogTemp, Warning, TEXT("RecvWorker Stop"));
		if (RecvWorker == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("RecvWorker is nullptr"));
		}

		UE_LOG(LogTemp, Warning, TEXT("before stop"));
		RecvWorker->StopThread();
		delete RecvWorker;
		RecvWorker = nullptr;

		UE_LOG(LogTemp, Warning, TEXT("after stop"));
	}

	if (SendWorker)
	{
		UE_LOG(LogTemp, Warning, TEXT("SendWorker Stop"));
		SendWorker->StopThread();
		delete SendWorker;
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
	if (Connected.Exchange(false))
	{
		ClearSession();
		OnDisconnect();
	}
}
