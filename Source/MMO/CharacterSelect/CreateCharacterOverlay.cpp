// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CreateCharacterOverlay.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "CharacterSelect/CharacterEntry.h"
#include "GameInstance/MMOGameInstance.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "PacketMaker/GamePacketMaker.h"
#include "Character/GameCharacter.h"

void UCreateCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	SwordClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnSwordClassButtonclicked);
	AxeClassButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnAxeClassButtonClicked);
    OkButton->OnClicked.AddDynamic(this, &UCreateCharacterOverlay::OnOkButtonClicked);
}

void UCreateCharacterOverlay::SetCharacterSelectOverlay(UCharacterSelectOverlay* characterSelectOverlay)
{
	CharacterSelectOverlay = characterSelectOverlay;
}

void UCreateCharacterOverlay::OnSwordClassButtonclicked()
{
    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));

        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        FVector spawnLocation = FVector(0.0f, 0.0f, 130.0f); // 예시 위치
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(UMMOGameInstance::GetMMOWorld()->GetName()));
        if (SpawnedCharacter != nullptr)
        {
            SpawnedCharacter->Destroy();
        }
        SpawnedCharacter = Cast<AGameCharacter>(UMMOGameInstance::GetMMOWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams));
        SpawnedCharacter->Initialize(ECharacterClassType::CCT_Sword);
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
        CharacterClassType = ECharacterClassType::CCT_Sword;
    }
}

void UCreateCharacterOverlay::OnAxeClassButtonClicked()
{
    if (GameCharacterClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("spawn33"));
        FActorSpawnParameters SpawnParams;
        FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f); // 예시 회전
        FVector spawnLocation = FVector(0.0f, 0.0f, 130.0f); // 예시 위치
        // 캐릭터 스폰
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(UMMOGameInstance::GetMMOWorld()->GetName()));
        if (SpawnedCharacter != nullptr)
        {
            SpawnedCharacter->Destroy();
            SpawnedCharacter = nullptr;
        }
        SpawnedCharacter = Cast<AGameCharacter>(UMMOGameInstance::GetMMOWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams));
        SpawnedCharacter->Initialize(ECharacterClassType::CCT_Axe);
        UE_LOG(LogTemp, Warning, TEXT("spawn2"));
        CharacterClassType = ECharacterClassType::CCT_Axe;
    }
}

void UCreateCharacterOverlay::OnOkButtonClicked()
{
    if (SpawnedCharacter != nullptr)
	{
		SpawnedCharacter->Destroy();
		SpawnedCharacter = nullptr;
	}

    if(ECharacterClassType::CCT_None == CharacterClassType)
	{
        RemoveFromParent();
		return;
	}

    //	TODO: 여기서 패킷 보내고
    // res받아서 추가하고
    // 서버에서는 db에 저장하고
    CPacket* ReqCreateCharacterPacket = CPacket::Alloc();
    uint16 Class;
    TCHAR NickName[NICKNAME_LEN];
    FString NickNameString = IDTextBox->GetText().ToString();
    FCString::Strcpy(NickName, *NickNameString);
    Class = static_cast<uint16>(CharacterClassType);
    GamePacketMaker::MP_CS_REQ_CREATE_PLAYER(ReqCreateCharacterPacket, Class, NickName);
    UMMOGameInstance::GetInstance()->SendPacket_GameServer(ReqCreateCharacterPacket);
	RemoveFromParent();

}
