// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class NetworkRingBuffer;
class FSocket;
class ClientSession;

class MMO_API SendThread : public FRunnable
{
public:
	//TODO: 이건 그냥 ClientSession 생성자로 받는게 좋겠는데
	SendThread(TSharedPtr<ClientSession> clientSession);
	~SendThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void StopThread();

private:
	bool bShutdown = false;

private:
	TWeakPtr<ClientSession> Session;
	FSocket* Socket = nullptr;
	FRunnableThread* Thread = nullptr;
	NetworkRingBuffer* SendBuffer = nullptr;
};
