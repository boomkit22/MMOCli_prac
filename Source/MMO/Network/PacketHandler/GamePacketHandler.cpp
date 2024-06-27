// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandler/GamePacketHandler.h"
#include "DataStructure/SerializeBuffer.h"
#include "GameInstance/MMOGameInstance.h"
#include "Packet.h"
#include "Kismet/GameplayStatics.h"

void GamePacketHandler::HandlePacket(CPacket* packet)
{
	int16 packetType;
	*packet >> packetType;

	UE_LOG(LogTemp, Warning, TEXT("HandlePacket packetType : %d"), packetType)
	switch (packetType)
	{
	case PACKET_SC_GAME_RES_LOGIN:
	{
		HandleLogin(packet);
	}
	break;

	case PACKET_SC_GAME_RES_FIELD_MOVE:
	{
		HandleFieldMove(packet);
	}
	break;

	case PACKET_SC_GAME_SPAWN_MY_CHARACTER:
	{
		HandleSpawnMyCharacter(packet);
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

void GamePacketHandler::HandleFieldMove(CPacket* packet)
{
	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleFieldMove(packet);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Handle Login GAme instacne null"));
	}
}

void GamePacketHandler::HandleSpawnMyCharacter(CPacket* packet)
{
	if (UMMOGameInstance* GameInstance = UMMOGameInstance::GetInstance())
	{
		GameInstance->HandleSpawnMyCharacter(packet);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Handle Login GAme instacne null"));
	}
}
