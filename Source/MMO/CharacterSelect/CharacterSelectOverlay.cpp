// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect/CharacterSelectOverlay.h"

#include "Components/Button.h"
#include "CharacterSelect/CreateCharacterOverlay.h"
#include "Components/VerticalBox.h"
#include "CharacterSelect/CharacterEntry.h"
#include "GameInstance/MMOGameInstance.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "PacketMaker/GamePacketMaker.h"
#include "CharacterSelect/CharacterEntry.h"


void UCharacterSelectOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CreateButton->OnClicked.AddDynamic(this, &UCharacterSelectOverlay::OnCreateButtonClicked);

	CPacket* reqCharacterListPacket = CPacket::Alloc();
	GamePacketMaker::MP_CS_REQ_PLAYER_LIST(reqCharacterListPacket);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(reqCharacterListPacket);
}

void UCharacterSelectOverlay::NativeDestruct()
{
	Super::NativeDestruct();
	if (CharacterListVerticalBox)
	{
		CharacterListVerticalBox->ClearChildren();
	}
}

void UCharacterSelectOverlay::AddCharacterEntry(UCharacterEntry* NewEntry)
{
	if (CharacterListVerticalBox && NewEntry)
	{
		CharacterListVerticalBox->AddChild(NewEntry);
	}
}

void UCharacterSelectOverlay::AddCharacterEntry(PlayerInfo playerInfo)
{
	UCharacterEntry* NewEntry = CreateWidget<UCharacterEntry>(UMMOGameInstance::GetMMOWorld(), UChaterEntryClass);
	if (NewEntry)
	{
		NewEntry->Init(static_cast<ECharacterClassType>(playerInfo.Class), FString::FromInt(playerInfo.Level), playerInfo.NickName);
		AddCharacterEntry(NewEntry);
		NewEntry->SetPlayerID(playerInfo.PlayerID);
	}
}

void UCharacterSelectOverlay::SetCharacterList(std::vector<PlayerInfo>& playerInfos)
{
	for (auto& playerInfo : playerInfos)
	{
		UCharacterEntry* NewEntry = CreateWidget<UCharacterEntry>(UMMOGameInstance::GetMMOWorld(), UChaterEntryClass);
		if (NewEntry)
		{
			NewEntry->SetDesiredSizeInViewport(FVector2D(300.f, 50.0f));
			NewEntry->Init(static_cast<ECharacterClassType>(playerInfo.Class), FString::FromInt(playerInfo.Level), playerInfo.NickName);
			AddCharacterEntry(NewEntry);
			NewEntry->SetPlayerID(playerInfo.PlayerID);
		}
	}
}

void UCharacterSelectOverlay::OnCreateButtonClicked()
{

	if (UCreateCharacterOverlayClass)
	{
		UCreateCharacterOverlay* CreateCharacterOverlay = CreateWidget<UCreateCharacterOverlay>(UMMOGameInstance::GetMMOWorld(), UCreateCharacterOverlayClass);
		if (CreateCharacterOverlay)
		{
			CreateCharacterOverlay->AddToViewport();
			CreateCharacterOverlay->SetCharacterSelectOverlay(this);
		}
	}
}
