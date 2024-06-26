// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ClientSession/ClientSession.h"
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

private:
	FTSTicker::FDelegateHandle TickDelegateHandle;
	bool Tick(float DeltaTime);
};
