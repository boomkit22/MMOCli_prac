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
#include "Monsters/Monster.h"

using namespace std;

bool bLoading = false;

void UMMOGameInstance::Init()
{
	Super::Init();

	UNetworkGameInstanceSubsystem* NetworkSubsystem = GetSubsystem<UNetworkGameInstanceSubsystem>();

	if (NetworkSubsystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("NetworkSubsystem is valid"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("NetworkSubsystem is unvalid"));
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
	ConnectChattingServer();
}


void UMMOGameInstance::Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Shutdown"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("GameInstance Shutdown"));
	Super::Shutdown();

	//CustomShutdown();
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);


	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("GameInstance Shutdown"));
	if (_GameServerSession)
	{
		_GameServerSession->Disconnect();
		_GameServerSession.Reset();
	}

	if (_ChattingServerSession)
	{
		_ChattingServerSession->Disconnect();
		_ChattingServerSession.Reset();
	}
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

		UWorld* World = GetMMOWorld();
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

}

void UMMOGameInstance::HandleFieldMove(CPacket* packet)
{
	MonsterMap.Empty();
	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("HandleFieldMove"));
	
	uint8 Status;
	uint16 fieldID;

	*packet >> Status >> fieldID;

	if (!Status)
	{
		//�ʿ���� ����ΰͰ����� ������ �̵��϶� ������ �̵� �ؾ���
		return;
	}

	//ĳ���� �ʿ��ִ°� �ٻ��� �� Reset
	for (auto& pair : CharacterMap)
	{
		if (pair.Value)
		{
			pair.Value->Destroy();
		}
	}
	CharacterMap.Empty();

	for (auto& pair : MonsterMap)
	{
		if (pair.Value)
		{
			pair.Value->Destroy();
		}
	}
	MonsterMap.Empty();


	FName LevelName;
	switch (fieldID)
	{
		case FIELD_LOBBY:
		{
			LevelName = TEXT("/Game/Maps/LobbyMap");
		}
		break;
			
		case FIELD_GUARDIAN:
		{
			LevelName = TEXT("/Game/Maps/GuardianMap");
		}
		break;
				
		case FIELD_SPIDER:
		{
			LevelName = TEXT("/Game/Maps/SpiderMap");
		}
		break;

		default:
			break;
	}

	CurrentFieldID = fieldID;
	OpenLevel(LevelName);
}

void UMMOGameInstance::HandleSpawnMyCharacter(CPacket* packet)
{
	FVector SpawnLocation;
	PlayerInfo playerInfo;
	FRotator SpawnRotation;
	*packet >> playerInfo >> SpawnLocation >> SpawnRotation;
	
	UWorld* World = GetMMOWorld();
	UE_LOG(LogTemp, Warning, TEXT("HandleSpawnMyCharacter %d %lld"), CharacterMap.Num(), playerInfo.PlayerID);
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{

			AGamePlayerController* MyController = Cast<AGamePlayerController>(PlayerController);
			if (MyController)
			{

				AGameCharacter* GameCharacter = MyController->SpawnMyCharacter(SpawnLocation, playerInfo, SpawnRotation);
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
	FRotator SpawnRoation;
	*packet >> playerInfo >> SpawnLocation >> SpawnRoation;
	int64 PlayerID = playerInfo.PlayerID;

	if (GameCharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		// ĳ���� ����
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetMMOWorld()->GetName()));

		AGameCharacter* SpawnedCharacter = Cast<AGameCharacter>(GetMMOWorld()->SpawnActor<ARemoteGameCharacter>(RemoteGameCharacterClass, SpawnLocation, SpawnRoation, SpawnParams));
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

void UMMOGameInstance::HandleDespawnOtherCharacter(CPacket* packet)
{
	int64 PlayerID;
	*packet >> PlayerID;

	auto character = CharacterMap.Find(PlayerID);
	if (character)
	{
		(*character)->Destroy();
		CharacterMap.Remove(PlayerID);
	}
}

void UMMOGameInstance::HandleCharacterMove(CPacket* packet)
{
	int64 characterNo;
	FVector destination;
	FRotator StartRotation;
	*packet >> characterNo >> destination >> StartRotation ;

	UE_LOG(LogTemp, Warning, TEXT("HandleCharacterMove %lld"), characterNo);

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
		auto monster = MonsterMap.Find(TargetID);
		if (monster)
		{
			(*monster)->GetHit(Damage);
		}
	}
	
}

void UMMOGameInstance::HandleCharacterSkill(CPacket* packet)
{
	//�̰Ŵ� �� ĳ���� ���� ��������
	int64 CharacterNo;
	FVector StartLocation;
	FRotator StartRotation;
	int32 SkillID;
	*packet >> CharacterNo >>StartLocation >> StartRotation >>SkillID;

	auto character = CharacterMap.Find(CharacterNo);
	if (character)
	{
		(*character)->StopMove();
		(*character)->SetActorLocation(StartLocation);
		(*character)->SetActorRotation(StartRotation);
		(*character)->PlaySkill(SkillID);
	}

}

void UMMOGameInstance::HandleMonsterSkill(CPacket* packet)
{
	int64 MonsterID;
	FVector StartPosition;
	FRotator StartRotation;
	int32 SkillID;		
	*packet >> MonsterID >> StartPosition >> StartRotation >> SkillID;

	auto Monster = MonsterMap.Find(MonsterID);
	if (Monster && *Monster)
	{
		(*Monster)->StopMove();
		(*Monster)->SetActorRotation(StartRotation);
		if (SkillID == 1)
		{
			(*Monster)->Attack();
		}

	}
}

void UMMOGameInstance::HandleSignUp(CPacket* packet)
{
	USignUpOverlay* SignUpOverlay = nullptr;


	uint8 Status;
	*packet >> Status;
	UWorld* World = GetMMOWorld();
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
	PlayerInfo playerInfo;
	
	*packet >> Status >> playerInfo;

	//TODO:
	// LoginHud�����ͼ� VerticalBox�� �߰��ϱ�
	if (!Status)
	{
		//TODO: ����� ĳ���� ���� ����â �̷��� �ϸ� ���� �Ͱ�����
		return;
	}

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

					CharacaterSelectOverlay->AddCharacterEntry(playerInfo);
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

	GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Yellow, TEXT("HandleSelectPlayer"));
	//TODO: field�̵� ���
	uint16 fieldId = FIELD_LOBBY;
	CPacket* fieldMovePacket = CPacket::Alloc();
	GamePacketMaker::MP_CS_REQ_FIELD_MOVE(fieldMovePacket, fieldId);
	GetInstance()->SendPacket_GameServer(fieldMovePacket);
}

void UMMOGameInstance::HandleSpawnMonster(CPacket* packet)
{
	MonsterInfo monsterInfo;
	FVector SpawnLocation;
	FRotator SpawnRotation;
	*packet >> monsterInfo >> SpawnLocation >> SpawnRotation;


	if (MonsterClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMonster* SpawnedMonster = Cast<AMonster>(GetMMOWorld()->SpawnActor<AMonster>(MonsterClass, SpawnLocation, SpawnRotation, SpawnParams));
		if (SpawnedMonster)
		{
			SpawnedMonster->SetMonsterProperties(monsterInfo);
			MonsterMap.Add(monsterInfo.MonsterID, SpawnedMonster);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Spawned Monster is null"));
		}
	}
}

void UMMOGameInstance::HandleMonsterMove(CPacket* packet)
{
	int64 MonsterID;
	FVector destination;
	FRotator StartRotation;
	*packet >> MonsterID >> destination >> StartRotation;

	//GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, FString::Printf(TEXT("Handle Move Monster, %lld"), MonsterID));
	auto Monster = MonsterMap.Find(MonsterID);
	if (Monster && *Monster)
	{

		(*Monster)->SetActorRotation(StartRotation, ETeleportType::TeleportPhysics);
		(*Monster)->SetDestination(destination);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.5f, FColor::Red, TEXT("Monster is null"));
	}
}

void UMMOGameInstance::HandleCharacterStop(CPacket* packet)
{
	int64 CharacterID;
	FVector StopLocation;
	FRotator StopRotation;
	*packet >> CharacterID >> StopLocation >> StopRotation;
	// ĳ���� ã�Ƽ� ���߰� SetDestination
	auto character = CharacterMap.Find(CharacterID);
	if (character && *character)
	{
		(*character)->StopMove();
		(*character)->SetActorLocation(StopLocation);
		(*character)->SetActorRotation(StopRotation);
	}
}

void UMMOGameInstance::HandleMonsterStop(CPacket* packet)
{
	int64 MonsterID;
	FVector StopLocation;
	FRotator StopRotation;
	*packet >> MonsterID >> StopLocation >> StopRotation;

	auto Monster = MonsterMap.Find(MonsterID);
	if (Monster && *Monster)
	{
		(*Monster)->StopMove();
		(*Monster)->SetActorLocation(StopLocation);
		(*Monster)->SetActorRotation(StopRotation);
	}
}

void UMMOGameInstance::HandleCharacterDeath(CPacket* packet)
{
	int64 CharacterID;
	FVector DeathLocation;
	FRotator DeathRotation;
	*packet >> CharacterID >> DeathLocation >> DeathRotation;

	auto character = CharacterMap.Find(CharacterID);
	if (character && *character)
	{
		(*character)->Death();
	}
}

void UMMOGameInstance::HandleMonsterDeath(CPacket* packet)
{
	int64 MonsterID;
	FVector DeathLocation;
	FRotator DeathRotation;
	*packet >> MonsterID >> DeathLocation >> DeathRotation;

	auto Monster = MonsterMap.Find(MonsterID);
	if (Monster && *Monster)
	{
		(*Monster)->Death();
	}
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
	UWorld* World = GetMMOWorld();
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

	if (_ChattingServerSession->IsConnecetd() && !bLoading)
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

