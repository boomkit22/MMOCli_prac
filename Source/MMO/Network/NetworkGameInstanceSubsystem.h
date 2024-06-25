// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ClientSession.h"
#include "NetworkGameInstanceSubsystem.generated.h"

/**
 * 
 */
class FSocket;
UCLASS()
class MMO_API UNetworkGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void Connect();
	void SendPacket(CPacket* packet);

public:
	FSocket* Socket;
	FString IpText = TEXT("127.0.0.1");
	int16 Port = 20000;

	ClientSession* Session = nullptr;
	bool Connected = false;

	void ProcessRecvPcket();

public:
	void Handle_CREATE_MY_CHARACTER(uint32 id, FVector3f& position, float yaw, uint16 hp);

public:

private:
	FTSTicker::FDelegateHandle TickDelegateHandle;
	bool Tick(float DeltaTime);
};
