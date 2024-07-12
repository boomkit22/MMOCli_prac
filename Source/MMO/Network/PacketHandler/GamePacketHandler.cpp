// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketHandler/GamePacketHandler.h"
#include "DataStructure/SerializeBuffer.h"
#include "Packet.h"
#include "Kismet/GameplayStatics.h"

void GamePacketHandler::HandlePacket(CPacket* packet)
{
	if(GameInstance == nullptr)
	{
		GameInstance = UMMOGameInstance::GetInstance();
	}

	int16 packetType;
	*packet >> packetType;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Handle Packet Type : %d"), packetType));

	switch (packetType)
	{
	case PACKET_SC_GAME_RES_LOGIN:
	{
		GameInstance->HandleGameLogin(packet);
	}
	break;

	case PACKET_SC_GAME_RES_FIELD_MOVE:
	{
		GameInstance->HandleFieldMove(packet);
	}
	break;

	case PACKET_SC_GAME_SPAWN_MY_CHARACTER:
	{
		GameInstance->HandleSpawnMyCharacter(packet);
	}
	break;

	case PACKET_SC_GAME_SPAWN_OTHER_CHAACTER:
	{
		GameInstance->HandleSpawnOhterCharacter(packet);
	}
	break;

	case PACKET_SC_GAME_RES_CHARACTER_MOVE:
	{
		GameInstance->HandleCharacterMove(packet);
	}
	break;

	case PACKET_SC_GAME_RES_DAMAGE:
	{
		GameInstance->HandleDamage(packet);
	}
	break;

	case PACKET_SC_GAME_RES_CHARACTER_SKILL:
	{
		GameInstance->HandleCharacterSkill(packet);
	}
	break;

	case PACKET_SC_GAME_RES_SIGN_UP:
	{
		GameInstance->HandleSignUp(packet);
	}
	break;

	case PACKET_SC_GAME_RES_PLAYER_LIST:
	{
		GameInstance->HandlePlayerList(packet);
	}
	break;

	case PACKET_SC_GAME_RES_CREATE_PLAYER:
	{
		GameInstance->HandleCreatePlayer(packet);
	}
	break;

	case PACKET_SC_GAME_RES_SELECT_PLAYER:
	{
		GameInstance->HandleSelectPlayer(packet);
	}
	break;

	case PACKET_SC_GAME_SPAWN_MONSTER:
	{
		GameInstance->HandleSpawnMonster(packet);
	}
	break;

	case PACKET_SC_GAME_MONSTER_MOVE:
	{
		GameInstance->HandleMonsterMove(packet);
	}	
	break;

	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unknown Packet Type : %d"), packetType));
		break;
	}

	CPacket::Free(packet);
}

