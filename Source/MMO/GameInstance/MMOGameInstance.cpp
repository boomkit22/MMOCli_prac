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
#include "Login/CharacterSelectOverlay.h"
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
	TCHAR NickName[20];
	uint32 Exp;

	*packet >> AccountNo >> Status >> CharacterLevel;
	packet->GetData((char*)NickName, ID_LEN * sizeof(TCHAR));
	*packet >> Exp;

	if (Status)
	{
		bool bConnectChattingSuccess = ConnectChattingServer();
		if (!bConnectChattingSuccess)
		{
			DisconnectGameServer();
		}

		//로그인 패킷 보내기
		CPacket* chatLoginPacket = CPacket::Alloc();
		ChattingPacketMaker::MP_CS_REQ_LOGIN(chatLoginPacket, AccountNo, NickName);
		SendPacket_ChattingServer(chatLoginPacket);

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
					//TSubclassOf<UCharacterSelectOverlay> characterSelectOvelay = LoginHUD->GetCharacterSelectOverlayClass();
					LoginHUD->ChangeOverlay(LoginHUD->GetCharacterSelectOverlayClass());
					UCharacterSelectOverlay* CharacterSelectOverlay = Cast<UCharacterSelectOverlay>(LoginHUD->GetCurrentOverlay());
					if (CharacterSelectOverlay)
					{
						FString NickNameStr = NickName;
						CharacterSelectOverlay->SetCharacterSelectText(NickNameStr, CharacterLevel);
					}
				}
			}
		}
	}
	else {
		// TODO: 로그인 실패에서 할일
		// 로그인 실패
		// 다시 로그인 씬 가는게 좋은 선택인가

		//로그
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
					// CharacterSelectOverlay 클래스로 오버레이 변경
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
	bLoading = true;
	UE_LOG(LogTemp, Warning, TEXT("bLoading %d"), bLoading);
	UE_LOG(LogTemp, Warning, TEXT("Open Level 1"));
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Maps/FieldMap_1"), false);
	UE_LOG(LogTemp, Warning, TEXT("Open Level 2"));
}

void UMMOGameInstance::HandleSpawnMyCharacter(CPacket* packet)
{
	int64 PlayerID;
	FVector SpawnLocation;
	uint16 Level;
	TCHAR NickName[20];

	*packet >> PlayerID >> SpawnLocation >> Level;
	packet->GetData((char*)NickName, ID_LEN * sizeof(TCHAR));


	
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{

			AGamePlayerController* MyController = Cast<AGamePlayerController>(PlayerController);
			if (MyController)
			{
				AGameCharacter* GameCharacter = MyController->SpawnMyCharacter(PlayerID, SpawnLocation, Level, NickName);
				if (GameCharacter)
				{
					CharacterMap.Add(PlayerID, GameCharacter);
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
	int64 PlayerID;
	FVector SpawnLocation;
	uint16 Level;
	TCHAR NickName[20];

	*packet >> PlayerID >> SpawnLocation >> Level;
	packet->GetData((char*)NickName, ID_LEN * sizeof(TCHAR));


	if (GameCharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
		// 캐릭터 스폰
		UE_LOG(LogTemp, Warning, TEXT("%s"), *(GetWorld()->GetName()));

		AGameCharacter* SpawnedCharacter = Cast<AGameCharacter>(GetWorld()->SpawnActor<ARemoteGameCharacter>(RemoteGameCharacterClass, SpawnLocation, Rotation, SpawnParams));
		if (SpawnedCharacter)
		{
			SpawnedCharacter->SetPlayerID(PlayerID);
			SpawnedCharacter->InitCharAttributeComponent(100, NickName, Level);
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
	//이거는 이 캐릭터 뺴고 보내야지
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
				// CharacterSelectOverlay 클래스로 오버레이 변경
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
		//로그
		if (SignUpOverlay)
		{
			SignUpOverlay->AddFailWidgetToViewport();
		}
	}
}

void UMMOGameInstance::HandleChatMessage(CPacket* packet)
{
	int64 AccountNo;
	TCHAR Nickname[20]; // null 포함, 20자 배열
	uint16 MessageLen;
	FString Message;

	*packet >> AccountNo;
	packet->GetData((char*)Nickname, 20 * sizeof(TCHAR)); // Nickname 추출

	FString NikckNameStr = FString(Nickname);

	*packet >> MessageLen;
	TCHAR MessageArray[101]; // 최대 100글자로 제한, null 문자를 위한 공간 추가

	// 메시지 길이가 최대 길이를 초과하는지 확인
	int copyLen = FMath::Min((int)MessageLen, 200); // WCHAR 단위로 변환하므로 200 바이트로 제한
	packet->GetData((char*)MessageArray, copyLen); // Message 추출
	MessageArray[copyLen / 2] = '\0'; // null 종료 문자 추가

	Message = FString(MessageArray); // WCHAR 배열을 FString으로 변환


	FString TextOnOverlay = NikckNameStr + TEXT(": ") + Message;

	//TODO: 채팅창에 메시지 출력
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

