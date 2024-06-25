// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SerializeBuffer.h"
#include "NetworkRingBuffer.h"
#include "RecvThread.h"
#include "SendThread.h"

/**
 * 
 */
class MMO_API ClientSession
{
public:
	ClientSession();
	ClientSession(FSocket* socket);
	~ClientSession();

	virtual void Connect();
	virtual void StartNetwork();
	virtual void SendPacket(CPacket* packet);
	virtual void Disconnect();
	virtual void HandleRecvPacket() = 0;

public:
	//수신 스레드
	TSharedPtr<RecvThread> RecvWorker;

	//송신 스레드
	TSharedPtr<SendThread> SendWorker;
	//SendThread* SendWorker = nullptr;
	
	FSocket* Socket;
	//TQueue는 single processor single consumer에서 안전하게 되어있다.
	//이 세션의 RecvPacketQueue를 메인쓰레드에서 접근해서 꺼내가서 처리해야한다.
	TQueue<CPacket*> RecvPacketQueue;
	NetworkRingBuffer SendBuffer;

	FString IpText;
	int16 Port;

	bool Connected;
};
