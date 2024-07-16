// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "PacketMaker/GamePacketMaker.h"
#include "PacketMaker/ChattingPacketMaker.h"
#include "Character/GameCharacter.h"

void UCharacterEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &UCharacterEntry::OnSelectButtonClicked);
	}

	if (CharacterButton)
	{
		CharacterButton->OnClicked.AddDynamic(this, &UCharacterEntry::OnCharacterButtonClicked);
	}
}

void UCharacterEntry::Init(ECharacterClassType characterClassType, FString level, FString id)
{
	CharacterClassType = characterClassType;

	switch (characterClassType)
	{
		case ECharacterClassType::CCT_Axe:
		{
			WeaponImage->SetBrushFromTexture(AxeImage);
		}
		break;

		case ECharacterClassType::CCT_Sword:
		{
			WeaponImage->SetBrushFromTexture(SwordImage);
		}
		break;
	}

	LevelText->SetText(FText::FromString(level));
	IDText->SetText(FText::FromString(id));
}

void UCharacterEntry::SetPlayerID(int64 playerID)
{
	PlayerID = playerID;
}

void UCharacterEntry::OnSelectButtonClicked()
{
	// 캐릭터 한개 선택하면
	// 캐릭터 선택 응답쏴주고
	// 응답오면 필드이동하고
	// 스폰 쏴주고
	CPacket* reqSelectCharacterPacket = CPacket::Alloc();
	GamePacketMaker::MP_CS_REQ_SELECT_PLAYER(reqSelectCharacterPacket, PlayerID);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(reqSelectCharacterPacket);

	CPacket* chatLoginPAcket = CPacket::Alloc();
	int64 accountId = UMMOGameInstance::GetInstance()->GetAccountId();

	FText idText = IDText->GetText();
	FString idString = idText.ToString();
	TCHAR id[ID_LEN];
	FMemory::Memset(id, 0, sizeof(TCHAR) * ID_LEN);
	FMemory::Memcpy(id, *idString, sizeof(TCHAR) * idString.Len());

	ChattingPacketMaker::MP_CS_REQ_LOGIN(chatLoginPAcket, accountId, id);
	UMMOGameInstance::GetInstance()->SendPacket_ChattingServer(chatLoginPAcket);
}

void UCharacterEntry::OnCharacterButtonClicked()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CharacterButtonClicked"));


	if (GameCharacterClass)
	{
		FActorSpawnParameters SpawnParams;
		FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
		FVector spawnLocation = FVector(0.0f, 0.0f, 130.0f);

		if (SpawnedCharacter != nullptr)
		{
			SpawnedCharacter->Destroy();
			SpawnedCharacter = nullptr;
		}

		SpawnedCharacter = Cast<AGameCharacter>(UMMOGameInstance::GetMMOWorld()->SpawnActor<AActor>(GameCharacterClass, spawnLocation, Rotation, SpawnParams));
		SpawnedCharacter->Initialize(CharacterClassType);
	}
}
