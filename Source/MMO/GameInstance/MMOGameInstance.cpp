// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MMOGameInstance.h"
#include "Network/NetworkGameInstanceSubsystem.h"
#include "ClientSession/GameServerSession.h"
#include "ClientSession/LoginServerSession.h"
#include "ClientSession/ChattingServerSession.h"
#include "PacketMaker/LoginPacketMaker.h"
#include "PacketMaker/GamePacketMaker.h"
#include "PacketMaker/ChattingPacketMaker.h"
#include "Editor.h"

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
	_GameServerSession = MakeShared<GameServerSession>();
	_LoginServerSession = MakeShared<LoginServerSession>();
	_ChattingServerSession = MakeShared<ChattingServerSession>();

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UMMOGameInstance::Tick));
}


void UMMOGameInstance::Shutdown()
{
	Super::Shutdown();
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	UE_LOG(LogTemp, Warning, TEXT("GameInstacne Shutdown"));
	_GameServerSession->Disconnect();
	_LoginServerSession->Disconnect();
	_ChattingServerSession->Disconnect();

	_GameServerSession.Reset();
	_LoginServerSession.Reset();
	_ChattingServerSession.Reset();
}

bool UMMOGameInstance::ConnectGameServer()
{
	return _GameServerSession->Connect(FString("127.0.0.1"), 10300);
}

bool UMMOGameInstance::ConnectLoginServer()
{
	return _LoginServerSession->Connect(FString("127.0.0.1"), 10301);
}

bool UMMOGameInstance::ConnectChattingServer()
{
	return _ChattingServerSession->Connect(FString("127.0.0.1"), 10302);
}

void UMMOGameInstance::DisconnectGameServer()
{
	_GameServerSession->Disconnect();;
}

void UMMOGameInstance::DisconnectLoginServer()
{
	_LoginServerSession->Disconnect();
}

void UMMOGameInstance::DisconnectChattingServer()
{
	_ChattingServerSession->Disconnect();
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

void UMMOGameInstance::HandleEcho(CPacket* packet)
{
	//TODO : 다시 패킷 보내기
	//TODO: log찍기
	static int colorIndex = 0;
	colorIndex++;
	FColor color;
	if (colorIndex % 3 == 0)
	{
		color = FColor::Red;
	}
	else if (colorIndex % 3 == 1)
	{
		color = FColor::Green;
	}
	else if (colorIndex % 3 == 2)
	{
		color = FColor::Blue;
	}


	GEngine->AddOnScreenDebugMessage(1, 1.f, color, FString::Printf(TEXT("EchoReceieved")));

	CPacket* echoPacket = CPacket::Alloc();
	LoginPacketMaker::MP_CS_REQ_ECHO(echoPacket);
	SendPacket_LoginServer(echoPacket);
}

bool UMMOGameInstance::Tick(float DeltaTime)
{
	if (_GameServerSession->IsConnecetd())
	{
		_GameServerSession->HandleRecvPacket();
	}

	if (_LoginServerSession->IsConnecetd())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Tick"));
		//_LoginServerSession->HandleRecvPacket();
		_LoginServerSession->HandleRecvPacket();
	}

	if (_ChattingServerSession->IsConnecetd())
	{
		_ChattingServerSession->HandleRecvPacket();
	}
	return true;
}

UMMOGameInstance* UMMOGameInstance::GetInstance()
{
#if WITH_EDITOR
	if (GEditor)
	{
		UWorld* World = GEditor->PlayWorld;
		if (World)
		{
			UGameInstance* g = World->GetGameInstance();

			if (auto* GameInstance = Cast<UMMOGameInstance>(World->GetGameInstance()))
			{
				return GameInstance;
			}
		}
	}
	return nullptr;
#endif
	if (GWorld)
	{
		if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
		{
			return GameInstance;
		}
	}

	return nullptr;
}

