// Fill out your copyright notice in the Description page of Project Settings.


#include "Login/CharacterSelectOverlay.h"
#include "Components/Button.h"
#include "Network/DataStructure/SerializeBuffer.h"
#include "Network/PacketMaker/GamePacketMaker.h"
#include "GameInstance/MMOGameInstance.h"

void UCharacterSelectOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	CharacterSelectButton->OnClicked.AddDynamic(this, &UCharacterSelectOverlay::OnCharacterSelectButtonClicked);
}

void UCharacterSelectOverlay::OnCharacterSelectButtonClicked()
{
	// 이거 클릭하면 뭐해야하는데?
	// TODO: 필드이동 요청
	
	// 1. Req Field Move
	CPacket* packet = CPacket::Alloc();
	uint16 fieldID = 1;
	GamePacketMaker::MP_CS_REQ_FIELD_MOVE(packet, fieldID);
	UMMOGameInstance::GetInstance()->SendPacket_GameServer(packet);
	// 2. Wait For Response
	// 2. 1 Res FiledMove
	// 2. 2 Spawn My Character
	// 2. 3 Spanw Other Chater
	// 2. 4 Spawn Monster
}
