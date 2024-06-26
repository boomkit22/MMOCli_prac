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
class MMO_API ClientSession : public TSharedFromThis<ClientSession>
{
	friend class RecvThread;
	friend class SendThread;

public:
	ClientSession();
	virtual ~ClientSession();

	bool Connect(FString IPText, int16 port);

	void SendPacket(CPacket* packet);
	void Disconnect();
	virtual void HandleRecvPacket() = 0;
	virtual void OnDisconnect() = 0;

protected:
	TQueue<CPacket*> RecvPacketQueue;

private:
	//���� ������
	TSharedPtr<RecvThread> RecvWorker;

	//�۽� ������
	TSharedPtr<SendThread> SendWorker;
	//SendThread* SendWorker = nullptr;
	FSocket* Socket;
	//TQueue�� single processor single consumer���� �����ϰ� �Ǿ��ִ�.
	//�� ������ RecvPacketQueue�� ���ξ����忡�� �����ؼ� �������� ó���ؾ��Ѵ�.
	NetworkRingBuffer SendBuffer;
	
	//������ IP, Port
	FString _IpText;
	int16 _Port;

	TAtomic<bool> Connected = false;

private:
	void StartNetwork();
	void ClearSession();
	void NetworkDisconnect();
};
