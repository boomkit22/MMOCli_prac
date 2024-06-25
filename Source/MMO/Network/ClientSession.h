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
	//���� ������
	TSharedPtr<RecvThread> RecvWorker;

	//�۽� ������
	TSharedPtr<SendThread> SendWorker;
	//SendThread* SendWorker = nullptr;
	
	FSocket* Socket;
	//TQueue�� single processor single consumer���� �����ϰ� �Ǿ��ִ�.
	//�� ������ RecvPacketQueue�� ���ξ����忡�� �����ؼ� �������� ó���ؾ��Ѵ�.
	TQueue<CPacket*> RecvPacketQueue;
	NetworkRingBuffer SendBuffer;

	FString IpText;
	int16 Port;

	bool Connected;
};
