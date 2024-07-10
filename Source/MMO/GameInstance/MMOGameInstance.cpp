// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MMOGameInstance.h"
#include "Network/NetworkGameInstanceSubsystem.h"
#include "ClientSession/GameServerSession.h"
#include "ClientSession/ChattingServerSession.h"
#include "PacketMaker/GamePacketMaker.h"
#include "PacketMaker/ChattingPacketMaker.h"
#if WITH_EDITOR
#include "Editor.h"
#endif
#include "Login/LoginHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Character/GamePlayerController.h"
#include "Type.h"
#include "Character/GameCharacter.h"
#include "Character/RemoteGameCharacter.h"
#include "HUD/MMOHUD.h"
#include "HUD/MMOChatOverlay.h"
#include "Login/LoginOverlay.h"
#include "Login/SignUpOverlay.h"
#include <vector>
#include "CharacterSelect/CharacterSelectHUD.h"
#include "CharacterSelect/CharacterSelectOverlay.h"
using namespace std;

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
	
	//TODO: session ����
	_GameServerSession = MakeShared<GameServerSession>();
	_ChattingServerSession = MakeShared<ChattingServerSession>();

	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UMMOGameInstance::Tick));

	FWorldDelegates::OnPostWorldCreation;

	//FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UMMOGameInstance::OnLevelLoaded);
	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UMMOGameInstance::OnLevelLoaded);
	//FWorldDelegates::

	ConnectGameServer();
}


void UMMOGameInstance::Shutdown()
{
	Super::Shutdown();
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);

	UE_LOG(LogTemp, Warning, TEXT("GameInstacne Shutdown"));
	_GameServerSession->Disconnect();
	_ChattingServerSession->Disconnect();

	_GameServerSession.Reset();
	_ChattingServerSession.Reset();

	CharacterMap.Empty();


}

bool UMMOGameInstance::ConnectGameServer()
{
	return _GameServerSession->Connect(FString("127.0.0.1"), 10303);
}


bool UMMOGameInstance::ConnectChattingServer()
{
	return _ChattingServerSession->Connect(FString("127.0.0.1"), 10302);
}

void UMMOGameInstance::DisconnectGameServer()
{
	_GameServerSession->Disconnect();;
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


void UMMOGameInstance::SendPacket_ChattingServer(CPacket* packet)
{
	if (_ChattingServerSession)
	{
		_ChattingServerSession->SendPacket(packet);
	}
}

void UMMOGameInstance::OpenLevel(FName LevelName)
{
	bLoading = true;
	UGameplayStatics::OpenLevel(GetInstance(), LevelName, false);
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

	
	*packet >> AccountNo >> Status;
	
	if (Status)
	{
		//Login ����
		OpenLevel(TEXT("/Game/Maps/CharacterSelectMap"));
	}
	else {
		// Login ����
				// TODO: �α��� ���п��� ����
		// �α��� ����
		// �ٽ� �α��� �� ���°� ���� �����ΰ�

		//�α�
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Login Fail"));

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
					//TSubclassOf<UCharacterSelectOverlay> characterSelectOvelay = LoginHUD->GetCharacterSelectOverlayClass();
					ULoginOverlay* LoginOverlay = Cast<ULoginOverlay>(LoginHUD->GetCurrentOverlay());
					if (LoginOverlay)
					{
						LoginOverlay->AddFailWidgetToViewport();
					}
				}
			}
		}
	}

	//if (Status)
	//{
	//	bool bConnectChattingSuccess = ConnectChattingServer();
	//	if (!bConnectChattingSuccess)
	//	{
	//		DisconnectGameServer();
	//	}

	//	//�α��� ��Ŷ ������
	//	CPacket* chatLoginPacket = CPacket::Alloc();
	//	ChattingPacketMaker::MP_CS_REQ_LOGIN(chatLoginPacket, AccountNo, NickName);
	//	SendPacket_ChattingServer(chatLoginPacket);

	//	//�α��� �����ϸ�?
	//	// 1. OverLay ����
	//	UWorld* World = GetWorld();
	//	if (World)
	//	{
	//		APlayerController* Controller = World->GetFirstPlayerController();
	//		if (Controller)
	//		{
	//			AHUD* HUD = Controller->GetHUD();
	//			ALoginHUD* LoginHUD = Cast<ALoginHUD>(HUD);
	//			if (LoginHUD)
	//			{
	//				// CharacterSelectOverlay Ŭ������ �������� ����
	//				//TSubclassOf<UCharacterSelectOverlay> characterSelectOvelay = LoginHUD->GetCharacterSelectOverlayClass();
	//				LoginHUD->ChangeOverlay(LoginHUD->GetCharacterSelectOverlayClass());
	//				UCharacterSelectOverlay* CharacterSelectOverlay = Cast<UCharacterSelectOverlay>(LoginHUD->GetCurrentOverlay());
	//				if (CharacterSelectOverlay)
	//				{
	//					FString NickNameStr = NickName;
	//					CharacterSelectOverlay->SetCharacterSelectText(NickNameStr, CharacterLevel);
	//				}
	//			}
	//		}
	//	}
	//}
	
}

void UMMOGameInstance::HandleFieldMove(CPacket* packet)
{
	OpenLevel(TEXT("/Game/Maps/FieldMap_1"));
}

void UMMOGameInstance::HandleSpawnMyCharacter(CPacket* packet)
{
	FVector SpawnLocation;
	PlayerInfo playerInfo;
	*packet >> playerInfo >> SpawnLocation;
	
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{

			AGamePlayerController* MyController = Cast<AGamePlayerController>(PlayerController);
			if (MyController)
			{
				AGameCharacter* GameCharacter = MyController->SpawnMyCharacter(SpawnLocation, playerInfo);
				if (GameCharacter)
				{
					CharacterMap.Add(playerInfo.PlayerID, GameCharacter);
				}else
				{
					UE_LOG(LogTemp, Error, TEXT("HandleSpawn game character null"));

				}

			}
		}
	}

}

void UMMOGameInstance::HandleSpawnOhterCharacter(CPacket* packet)
{
	PlayerInfo playerInfo;
	FVector SpawnLocation;

	*packet >> playerInfo >> SpawnLocation;
	int64 PlayerID = playerInfo.PlayerID;

	if (GameCharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // ���� ȸ��
		// ĳ���� ����
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));

		AGameCharacter* SpawnedCharacter = Cast<AGameCharacter>(GetWorld()->SpawnActor<ARemoteGameCharacter>(RemoteGameCharacterClass, SpawnLocation, Rotation, SpawnParams));
		if (SpawnedCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Handle spawn Ohter Character"));
			SpawnedCharacter->Initialize(static_cast<ECharacterClassType>(playerInfo.Class));
			SpawnedCharacter->SetPlayerID(PlayerID);
			SpawnedCharacter->InitCharAttributeComponent(100, playerInfo.NickName, playerInfo.Level);
			CharacterMap.Add(PlayerID, SpawnedCharacter);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Spawne Other Character is null"));
		}
	}
}

void UMMOGameInstance::HandleCharacterMove(CPacket* packet)
{
	int64 characterNo;
	FVector destination;
	FRotator StartRotation;
	*packet >> characterNo >> destination >> StartRotation ;

	auto character = CharacterMap.Find(characterNo);
	if (character)
	{
		(*character)->SetActorRotation(StartRotation);
		(*character)->SetDestination(destination);
	}
}

void UMMOGameInstance::HandleDamage(CPacket* packet)
{
	

	int32   AttackerType;
	int64   AttackerID;
	int32   TargetType;
	int64   TargetID;
	int32   Damage;
	*packet >> AttackerType >> AttackerID >> TargetType >> TargetID >> Damage;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Handle Damage :%lld\n"), TargetID));

	if(TargetType == TYPE_PLAYER)
	{
		auto target = CharacterMap.Find(TargetID);
		if (target)
		{
			(*target)->GetHit(Damage);
		}
	}
	else if (TargetType == TYPE_MONSTER)
	{
	}
	
}

void UMMOGameInstance::HandleCharacterSkill(CPacket* packet)
{
	//�̰Ŵ� �� ĳ���� ���� ��������
	int64 CharacterNo;
	FRotator StartRotation;
	int32 SkillID;
	*packet >> CharacterNo >> StartRotation >>SkillID;

	auto character = CharacterMap.Find(CharacterNo);
	if (character)
	{
		(*character)->SetActorRotation(StartRotation);
		(*character)->PlaySkill(SkillID);
	}

}

void UMMOGameInstance::HandleSignUp(CPacket* packet)
{
	USignUpOverlay* SignUpOverlay = nullptr;


	uint8 Status;
	*packet >> Status;
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
				//TSubclassOf<UCharacterSelectOverlay> characterSelectOvelay = LoginHUD->GetCharacterSelectOverlayClass();
				ULoginOverlay* LoginOverlay = Cast<ULoginOverlay>(LoginHUD->GetCurrentOverlay());
				if (LoginOverlay)
				{
					SignUpOverlay = LoginOverlay->GetSignUpOverlay();
				}
			}
		}
	}

	if (Status)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Sign up Succeed"));

		if (SignUpOverlay)
		{
			SignUpOverlay->AddSuccessWidgetToViewport();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Sign up Overlay null"));

		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Sign up Fail"));
		//�α�
		if (SignUpOverlay)
		{
			SignUpOverlay->AddFailWidgetToViewport();
		}
	}
}

void UMMOGameInstance::HandlePlayerList(CPacket* packet)
{
	uint8 playerCount;
	PlayerInfo playerInfo;
	vector<PlayerInfo> playerList;
	*packet >> playerCount;

	

	if (playerCount > 0)
	{
		for (int i = 0; i < playerCount; i++)
		{
			*packet >> playerInfo;
			playerList.push_back(playerInfo);
		}	
		//TODO: ä��â�� �޽��� ���
		UWorld* World = GetMMOWorld();
		if (World)
		{
			APlayerController* Controller = World->GetFirstPlayerController();
			if (Controller)
			{
				AHUD* HUD = Controller->GetHUD();
				ACharacterSelectHUD* CharacterSelectHUD = Cast<ACharacterSelectHUD>(HUD);
				if (CharacterSelectHUD)
				{
					UCharacterSelectOverlay* CharacaterSelectOverlay = CharacterSelectHUD->GetCharacterSelectOverlay();
					if (CharacaterSelectOverlay)
					{
						CharacaterSelectOverlay->SetCharacterList(playerList);
					}
				}
			}
		}
	}
	
}

void UMMOGameInstance::HandleCreatePlayer(CPacket* packet)
{
	uint8 Status;
	uint16 Class;
	TCHAR NickName[NICKNAME_LEN];
	*packet >> Status >> Class;
	packet->GetData((char*)NickName, NICKNAME_LEN * sizeof(TCHAR));

		//TODO:
	// LoginHud�����ͼ� VerticalBox�� �߰��ϱ�
	if (!Status)
	{
		//TODO: ����� ĳ���� ���� ����â �̷��� �ϸ� ���� �Ͱ�����
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			AHUD* HUD = Controller->GetHUD();
			ACharacterSelectHUD* CharacterSelectHUD = Cast<ACharacterSelectHUD>(HUD);
			if (CharacterSelectHUD)
			{
				UCharacterSelectOverlay* CharacaterSelectOverlay = CharacterSelectHUD->GetCharacterSelectOverlay();
				if (CharacaterSelectOverlay)
				{

					CharacaterSelectOverlay->AddCharacterEntry(static_cast<ECharacterClassType>(Class), 1, NickName);
				}
			}
		}
	}
}

void UMMOGameInstance::HandleSelectPlayer(CPacket* packet)
{
	uint8 Status;
	*packet >> Status;

	if (!Status)
	{
		return;
	}

	//TODO: field�̵� ���
	uint16 fieldId = 1;
	CPacket* fieldMovePacket = CPacket::Alloc();
	GamePacketMaker::MP_CS_REQ_FIELD_MOVE(fieldMovePacket, fieldId);
	SendPacket_GameServer(fieldMovePacket);
}


void UMMOGameInstance::HandleChatMessage(CPacket* packet)
{
	int64 AccountNo;
	TCHAR Nickname[20]; // null ����, 20�� �迭
	uint16 MessageLen;
	FString Message;

	*packet >> AccountNo;
	packet->GetData((char*)Nickname, 20 * sizeof(TCHAR)); // Nickname ����

	FString NikckNameStr = FString(Nickname);

	*packet >> MessageLen;
	TCHAR MessageArray[101]; // �ִ� 100���ڷ� ����, null ���ڸ� ���� ���� �߰�

	// �޽��� ���̰� �ִ� ���̸� �ʰ��ϴ��� Ȯ��
	int copyLen = FMath::Min((int)MessageLen, 200); // WCHAR ������ ��ȯ�ϹǷ� 200 ����Ʈ�� ����
	packet->GetData((char*)MessageArray, copyLen); // Message ����
	MessageArray[copyLen / 2] = '\0'; // null ���� ���� �߰�

	Message = FString(MessageArray); // WCHAR �迭�� FString���� ��ȯ


	FString TextOnOverlay = NikckNameStr + TEXT(": ") + Message;

	//TODO: ä��â�� �޽��� ���
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			AHUD* HUD = Controller->GetHUD();
			AMMOHUD* MMOOHUD = Cast<AMMOHUD>(HUD);
			if (MMOOHUD)
			{
				UMMOChatOverlay* ChatOverlay = MMOOHUD->GetMMOChatOverlay();
				if (ChatOverlay)
				{
					ChatOverlay->OnRecvMessage(TextOnOverlay);
				}
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

	if (_ChattingServerSession->IsConnecetd())
	{
		_ChattingServerSession->HandleRecvPacket();
	}
	return true;
}

UWorld* UMMOGameInstance::GetMMOWorld()
{
#if WITH_EDITOR
	if (GEditor)
	{
		UWorld* World = GEditor->PlayWorld;
		if (World)
		{
			return World;
		}
	}

	if (GWorld)
	{
		return GWorld;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Blue, TEXT("editor nullptr!"));
	return nullptr;
#endif

	return GWorld;
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

	if (GWorld)
	{
		if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
		{
			return GameInstance;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Blue, TEXT("editor nullptr!"));
	return nullptr;
#endif
	if (GWorld)
	{
		if (auto* GameInstance = Cast<UMMOGameInstance>(GWorld->GetGameInstance()))
		{
			return GameInstance;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Blue, TEXT("nullptr!"));
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

