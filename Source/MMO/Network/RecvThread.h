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
	RecvThread(ClientSession* s, FSocket* socket);
	~RecvThread();

	// FRunnable��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Shutdown();

private:
	bool bShutdown = false;
	FRunnableThread* Thread = nullptr;
	FSocket* Socket = nullptr;
	ClientSession* Session = nullptr;
	NetworkRingBuffer recvBuffer;
	
};
