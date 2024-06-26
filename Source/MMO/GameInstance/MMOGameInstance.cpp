// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MMOGameInstance.h"
#include "Network/NetworkGameInstanceSubsystem.h"
#include "Network/GameServerSession.h"
#include "Network/LoginServerSession.h"
#include "Network/ChattingServerSession.h"

void UMMOGameInstance::Init()
{
	Super::Init();

	UNetworkGameInstanceSubsystem* NetworkSubsystem = GetSubsystem<UNetworkGameInstanceSubsystem>();
	if (NetworkSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NetworkSubsystem is valid"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NetworkSubsystem is unvalid"));
	}
	
	//TODO: session 생성

}


void UMMOGameInstance::Shutdown()
{
}

void UMMOGameInstance::ConnectGameServer()
{
	_GameServerSession->Connect(FString("127.0.0.1"), 10300);
}

void UMMOGameInstance::ConnectLoginServer()
{
	_LoginServerSession->Connect(FString("127.0.0.1"), 10301);
}

void UMMOGameInstance::ConnectChattingServer()
{
	_ChattingServerSession->Connect(FString("127.0.0.1"), 10302);
}


void UMMOGameInstance::SendPacket_GameServer(CPacket* packet)
{
	if (_GameServerSession)
	{
		_GameServerSession->SendPacket(packet);
	}
}

void UMMOGameInstance::SendPacket_LoginServer(CPacket* packet)
{
	if (_LoginServerSession)
	{
		_LoginServerSession->SendPacket(packet);
	}
}

void UMMOGameInstance::SendPacket_ChattingServer(CPacket* packet)
{
	if (_ChattingServerSession)
	{
		_ChattingServerSession->SendPacket(packet);
	}
}

void UMMOGameInstance::HandleLogin(CPacket* packet)
{
	//TODO: 성공 실패
	// if 성공
	// LoginServer 끊고
	// GameServer 연결
	
	// if 실패
	// 실패 메시지 출력
	/*
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && LoginOverlayClass)
		{
			Controller->bShowMouseCursor = true;
			LoginOverlay = CreateWidget<ULoginOverlay>(Controller, LoginOverlayClass);
			if (LoginOverlay)
			{
				LoginOverlay->AddToViewport();
			}
		}
	}
	*/
}

