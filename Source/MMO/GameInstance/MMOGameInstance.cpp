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
#include "Login/LoginHUD.h"
#include "Login/CharacterSelectOverlay.h"

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
	
	//TODO: session ����
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
		// �α��� ����
		// �α��� ���� ����
		// ���� ���� �����ϰ�
		DisconnectLoginServer();
		bool bConnectSuccess = ConnectGameServer();
		if (!bConnectSuccess)
		{
			//TODO: �α��� ����â ����
			//Text : ���Ӽ��� ���� ����
		}

		CPacket* gameServerLoginPacket = CPacket::Alloc();
		GamePacketMaker::MP_CS_REQ_LOGIN(gameServerLoginPacket, accountNo);
		SendPacket_GameServer(gameServerLoginPacket);
	}
	else {
		//TODO:
		// �α��� ����â ����
		// Text: ���� ����
	}

	//TODO: ���� ����
	// if ����
	// LoginServer ����
	// GameServer ����
	
	// if ����
	// ���� �޽��� ���
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
	//TODO : �ٽ� ��Ŷ ������
	//TODO: log���
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
	���Ӽ��� �α����� ���� �����ؾ�����
	redis�� ����Ű ��Ͼȵ�������?
	*/
	//���⼭ �������
	int64 AccountNo;
	uint8 Status;
	uint16 CharacterLevel;

	*packet >> AccountNo >> Status >> CharacterLevel;
	
	if (Status)
	{
		//�α��� �����ϸ�?
		// 1. OverLay ����
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
					// CharacterSelectOverlay Ŭ������ �������� ����
					LoginHUD->ChangeOverlay(LoginHUD->GetCharacterSelectOverlayClass());
				}
			}
		}
	}
	else {
		// TODO: �α��� ���п��� ����
		// �α��� ����
		// �ٽ� �α��� �� ���°� ���� �����ΰ�
	}
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

