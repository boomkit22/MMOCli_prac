// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NetworkRingBuffer;
class FSocket;

class MMO_API SendThread : public FRunnable
{
public:
	//TODO: �̰� �׳� ClientSession �����ڷ� �޴°� ���ڴµ�
	SendThread(FSocket* socket, NetworkRingBuffer* sendBuffer)
	{
		Socket = socket;
		SendBuffer = sendBuffer;
		Thread = FRunnableThread::Create(this, TEXT("SendThread"));

	}
	~SendThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Shutdown();

private:
	bool bShutdown = false;

private:
	FSocket* Socket = nullptr;
	FRunnableThread* Thread = nullptr;
	NetworkRingBuffer* SendBuffer = nullptr;
};
