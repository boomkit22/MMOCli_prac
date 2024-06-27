// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MMOGameInstance.h"
#include "Network/NetworkGameInstanceSubsystem.h"
#include "ClientSession/GameServerSession.h"
#include "ClientSession/LoginServerSession.h"
#include "ClientSession/ChattingServerSession.h"
#include "PacketMaker/LoginPacketMaker.h"
#include "PacketMaker/GamePacketMaker.h"
#include "PacketMaker/ChattingPacketMaker.h"
//#if WITH_EDITOR
//#include "Editor.h"
//#endif
#include "Login/LoginHUD.h"
#include "Login/CharacterSelectOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Character/GamePlayerController.h"


bool bLoading = false;

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

	FWorldDelegates::OnPostWorldCreation;

	//FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UMMOGameInstance::OnLevelLoaded);
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UMMOGameInstance::OnLevelLoaded);
	//FWorldDelegates::
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
	return _GameServerSession->Connect(FString("127.0.0.1"), 10303);
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

void UMMOGameInstance::HandleLoginLogin(CPacket* packet)
{
	int64 accountNo;
	uint8 status;
	TCHAR GameServerIP[16];
	uint16 GameServerPort;
	TCHAR ChatServerIP[16];
	uint16 ChatServerPort;

	*packet >> accountNo;
	AccountId = accountNo;
	*packet >> status;
	packet->GetData((char*)GameServerIP, 16 * sizeof(TCHAR));
	*packet >> GameServerPort;
	packet->GetData((char*)ChatServerIP, 16 * sizeof(TCHAR));
	*packet >> ChatServerPort;

	UE_LOG(LogTemp, Warning, TEXT("Handle login status : %d"), status);
	
	if (status == 1)
	{
		// 로그인 성공
		// 로그인 서버 끊고
		// 게임 서버 연결하고
		DisconnectLoginServer();
		bool bConnectSuccess = ConnectGameServer();
		if (!bConnectSuccess)
		{
			//TODO: 로그인 실패창 띄우기
			//Text : 게임서버 연결 실패
		}

		CPacket* gameServerLoginPacket = CPacket::Alloc();
		GamePacketMaker::MP_CS_REQ_LOGIN(gameServerLoginPacket, accountNo);
		SendPacket_GameServer(gameServerLoginPacket);
	}
	else {
		//TODO:
		// 로그인 실패창 띄우기
		// Text: 계정 없음
	}

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

void UMMOGameInstance::HandleLoginEcho(CPacket* packet)
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

	//Echo Test
	/*
	* 
	CPacket* echoPacket = CPacket::Alloc();
	LoginPacketMaker::MP_CS_REQ_ECHO(echoPacket);
	SendPacket_LoginServer(echoPacket);
	*/
}

void UMMOGameInstance::HandleGameLogin(CPacket* packet)
{
	/*
	게임서버 로그인은 언제 실패해야하지
	redis에 세션키 등록안돼있을때?
	*/
	//여기서 응답오면
	int64 AccountNo;
	uint8 Status;
	uint16 CharacterLevel;

	*packet >> AccountNo >> Status >> CharacterLevel;
	
	if (Status)
	{
		//로그인 성공하면?
		// 1. OverLay 변경
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* Controller = World->GetFirstPlayerController();
			if (Controller)
			{
				AHUD* HUD = Controller->GetHUD();
				ALoginHUD* LoginHUD = Cast<ALoginHUD>(HUD);
				if (LoginHUD)
				{
					// CharacterSelectOverlay 클래스로 오버레이 변경
					LoginHUD->ChangeOverlay(LoginHUD->GetCharacterSelectOverlayClass());
				}
			}
		}
	}
	else {
		// TODO: 로그인 실패에서 할일
		// 로그인 실패
		// 다시 로그인 씬 가는게 좋은 선택인가
	}
}

void UMMOGameInstance::HandleFieldMove(CPacket* packet)
{
	bLoading = true;
	UE_LOG(LogTemp, Warning, TEXT("bLoading %d"), bLoading);
	UE_LOG(LogTemp, Warning, TEXT("Open Level 1"));
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/DefaultMap"), false);
	UE_LOG(LogTemp, Warning, TEXT("Open Level 2"));
}

void UMMOGameInstance::HandleSpawnMyCharacter(CPacket* packet)
{
	FVector SpawnLocation;
	*packet >> SpawnLocation;
	UE_LOG(LogTemp, Warning, TEXT("HandleSpawn1"));
	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleSpawn2"));
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("HandleSpawn3"));

			AGamePlayerController* MyController = Cast<AGamePlayerController>(PlayerController);
			if (MyController)
			{
				UE_LOG(LogTemp, Warning, TEXT("HandleSpawn4"));
				MyController->SpawnMyCharacter(SpawnLocation);
			}
		}
	}

}

bool UMMOGameInstance::Tick(float DeltaTime)
{


	if (_GameServerSession->IsConnecetd() && !bLoading)
	{
		//UE_LOG(LogTemp, Warning, TEXT("bLoading %d"), bLoading);
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
//#if WITH_EDITOR
//	if (GEditor)
//	{
//		UWorld* World = GEditor->PlayWorld;
//		if (World)
//		{
//			UGameInstance* g = World->GetGameInstance();
//
//			if (auto* GameInstance = Cast<UMMOGameInstance>(World->GetGameInstance()))
//			{
//				return GameInstance;
//			}
//		}
//	}
//	UE_LOG(LogTemp, Warning, TEXT("editor nullptr!"));
//	return nullptr;
//#endif
	if (GWorld)
	{
		if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
		{
			return GameInstance;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("nullptr!"));

	return nullptr;
}

void UMMOGameInstance::OnLevelLoaded(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("OnLevelLoaded!"));
	bLoading = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Blue, TEXT("OnWorldInit!"));
	/*FLatentActionInfo Info;
	UGameplayStatics::LoadStreamLevelBySoftObjectPtr(World, LoadingDrill->DrillSublevel, true, false, Info);*/
}

