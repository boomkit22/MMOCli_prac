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
	//�̷����ϸ� Reconnect���� �ϴ� �����ҰͰ�����
	ClearSession();

	//������ �� �ʿ�� ��������
	_IpText = IPText;
	_Port = Port;

	// ���� ����
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Socket"));
	// Connect
	FIPv4Address Ip;
	TSharedRef<FInternetAddr> InternetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddr->SetIp(Ip.Value);
	InternetAddr->SetPort(Port);
	Connected = Socket->Connect(*InternetAddr);

	//Connect ����������?
	/*
	* �α��μ��� Connect ����? ���� ��û ���� �޽���
	* ���� ���� Connect ����? ���� ���� ���� �޽���
	* ä�� ���� Connect ����? ä�ú����°� �����ϸ�ǳ�
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
