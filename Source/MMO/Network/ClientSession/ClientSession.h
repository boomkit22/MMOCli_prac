// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructure/SerializeBuffer.h"
#include "DataStructure/NetworkRingBuffer.h"
#include "WorkerThread/RecvThread.h"
#include "WorkerThread/SendThread.h"

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
	//수신 스레드
	RecvThread* RecvWorker= nullptr;

	//송신 스레드
	SendThread* SendWorker = nullptr;
	//SendThread* SendWorker = nullptr;

	UPROPERTY()
	FSocket* Socket = nullptr;
	//TQueue는 single processor single consumer에서 안전하게 되어있다.
	//이 세션의 RecvPacketQueue를 메인쓰레드에서 접근해서 꺼내가서 처리해야한다.
	NetworkRingBuffer SendBuffer;
	
	//연결할 IP, Port
	FString _IpText;
	int16 _Port;

	TAtomic<bool> Connected = false;

public:
	FORCEINLINE bool IsConnecetd() const { return Connected; }

private:
	void StartNetwork();
	void ClearSession();
	void NetworkDisconnect();
};
