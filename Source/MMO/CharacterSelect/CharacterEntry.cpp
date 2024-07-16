// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "PacketMaker/GamePacketMaker.h"
#include "PacketMaker/ChattingPacketMaker.h"
void UCharacterEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &UCharacterEntry::OnSelectButtonClicked);
	}
}

void UCharacterEntry::Init(ECharacterClassType characterClassType, FString level, FString id)
{
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
	// ĳ���� �Ѱ� �����ϸ�
	// ĳ���� ���� ������ְ�
	// ������� �ʵ��̵��ϰ�
	// ���� ���ְ�
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

