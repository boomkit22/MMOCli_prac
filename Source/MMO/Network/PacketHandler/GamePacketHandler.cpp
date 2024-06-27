// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandler/GamePacketHandler.h"
#include "DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "Packet.h"

void GamePacketHandler::HandlePacket(CPacket* packet)
{
	int16 packetType;
	*packet >> packetType;

	switch (packetType)
	{
	case PACKET_SC_GAME_RES_LOGIN:
	{
		HandleLogin(packet);
	}
	break;

	default:
		break;
	}

	CPacket::Free(packet);
}

void GamePacketHandler::HandleLogin(CPacket* packet)
{
	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleGameLogin(packet);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Handle Login GAme instacne null"));
	}
}
