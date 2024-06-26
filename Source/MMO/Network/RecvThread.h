// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkRingBuffer.h"
/**
 * 
 */

class FSocket;
class ClientSession;

class MMO_API RecvThread : public FRunnable
{
public:
	RecvThread(TSharedPtr<ClientSession> clientSession);

	~RecvThread();

	// FRunnable��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void StopThread();

private:
	bool bShutdown = false;
	FRunnableThread* Thread = nullptr;
	FSocket* Socket = nullptr;
	TWeakPtr<ClientSession> Session;
	NetworkRingBuffer recvBuffer;
	
};
