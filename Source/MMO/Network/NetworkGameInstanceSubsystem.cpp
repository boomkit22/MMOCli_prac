// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkGameInstanceSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Engine/Engine.h"

void UNetworkGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker
	(FTickerDelegate::CreateUObject(this, &UNetworkGameInstanceSubsystem::Tick));
}

bool UNetworkGameInstanceSubsystem::Tick(float DeltaTime)
{
	//이걸로 로그인 틱 테스트할가
	UE_LOG(LogTemp, Warning, TEXT("UNetworkGameInstanceSubsystem::Tick"));

	
	return true; // return true to keep the ticker running, false to stop
}
